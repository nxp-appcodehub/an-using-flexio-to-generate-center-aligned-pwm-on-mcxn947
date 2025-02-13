/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_flexio.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "peripherals.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_FLEXIO_BASEADDR  FLEXIO0
#define DEMO_FLEXIO_OUTPUTPIN 0U /* Select FXIO_D0 as PWM output */
#define DEMO_FLEXIO_TIMER_CH  0U /* Flexio timer0 used */

#define DEMO_FLEXIO_CLOCK_FREQUENCY CLOCK_GetFlexioClkFreq()
#define DEMO_FLEXIO_FREQUENCY       100000U

#define FLEXIO_MAX_FREQUENCY (DEMO_FLEXIO_CLOCK_FREQUENCY / 2U)
#define FLEXIO_MIN_FREQUENCY (DEMO_FLEXIO_CLOCK_FREQUENCY / 512U)

/* flexio timer number */
#define FLEXIO_TIMER_CHANNELS (8)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Configures the timer as a 8-bits PWM mode to generate the PWM waveform
 *
 * @param freq_Hz PWM frequency in hertz, range is [FLEXIO_MIN_FREQUENCY, FLEXIO_MAX_FREQUENCY]
 * @param duty Specified duty in unit of %, with a range of [0, 100]
 */
static status_t flexio_pwm_init(uint32_t freq_Hz, uint32_t duty);

/*!
 * @brief Set PWM output in idle status (high or low).
 *
 * @param base               FlexIO peripheral base address
 * @param timerChannel       FlexIO timer channel
 * @param idleStatus         True: PWM output is high in idle status; false: PWM output is low in idle status
 */
static void FLEXIO_SetPwmOutputToIdle(FLEXIO_Type *base, uint8_t timerChannel, bool idleStatus);

#if defined(FSL_FEATURE_FLEXIO_HAS_PIN_STATUS) && FSL_FEATURE_FLEXIO_HAS_PIN_STATUS
/*!
 * @brief Get the pwm dutycycle value.
 *
 * @param base        FlexIO peripheral base address
 * @param timerChannel  FlexIO timer channel
 * @param channel     FlexIO as pwm output channel number
 *
 * @return Current channel dutycycle value.
 */
static uint8_t PWM_GetPwmOutputState(FLEXIO_Type *base, uint8_t timerChannel, uint8_t channel);
#endif

/*!
 * @brief Get pwm duty cycle value.
 */
static uint8_t s_flexioGetPwmDutyCycle[FLEXIO_TIMER_CHANNELS] = {0};

/*******************************************************************************
 * Variables
 *******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
static status_t flexio_pwm_init(uint32_t freq_Hz, uint32_t duty)
{
    assert((freq_Hz < FLEXIO_MAX_FREQUENCY) && (freq_Hz > FLEXIO_MIN_FREQUENCY));

    uint32_t lowerValue = 0; /* Number of clock cycles in high logic state in one period */
    uint32_t upperValue = 0; /* Number of clock cycles in low logic state in one period */
    uint32_t sum        = 0; /* Number of clock cycles in one period */
    flexio_timer_config_t fxioTimerConfig;

    /* Configure the timer DEMO_FLEXIO_TIMER_CH for generating PWM */
    fxioTimerConfig.triggerSelect   = FLEXIO_TIMER_TRIGGER_SEL_SHIFTnSTAT(0U);
    fxioTimerConfig.triggerSource   = kFLEXIO_TimerTriggerSourceInternal;
    fxioTimerConfig.triggerPolarity = kFLEXIO_TimerTriggerPolarityActiveLow;
    fxioTimerConfig.pinConfig       = kFLEXIO_PinConfigOutput;
    fxioTimerConfig.pinPolarity     = kFLEXIO_PinActiveHigh;
    fxioTimerConfig.pinSelect       = DEMO_FLEXIO_OUTPUTPIN; /* Set pwm output */
    fxioTimerConfig.timerMode       = kFLEXIO_TimerModeDisabled;
    fxioTimerConfig.timerOutput     = kFLEXIO_TimerOutputOneNotAffectedByReset;
    fxioTimerConfig.timerDecrement  = kFLEXIO_TimerDecSrcOnFlexIOClockShiftTimerOutput;
    fxioTimerConfig.timerDisable    = kFLEXIO_TimerDisableNever;
    fxioTimerConfig.timerEnable     = kFLEXIO_TimerEnabledAlways;
    fxioTimerConfig.timerReset      = kFLEXIO_TimerResetNever;
    fxioTimerConfig.timerStart      = kFLEXIO_TimerStartBitDisabled;
    fxioTimerConfig.timerStop       = kFLEXIO_TimerStopBitDisabled;

    /* Calculate timer lower and upper values of TIMCMP */
    /* Calculate the nearest integer value for sum, using formula round(x) = (2 * floor(x) + 1) / 2 */
    /* sum = DEMO_FLEXIO_CLOCK_FREQUENCY / freq_H */
    sum = (DEMO_FLEXIO_CLOCK_FREQUENCY * 2 / freq_Hz + 1) / 2;

    /* Calculate the nearest integer value for lowerValue, the high period of the pwm output */
    lowerValue = (sum * duty) / 100;
    /* Calculate upper value, the low period of the pwm output */
    upperValue = sum - lowerValue - 2;

    fxioTimerConfig.timerCompare = ((upperValue << 8U) | (lowerValue));

    if ((duty > 0) && (duty < 100))
    {
        /* Set Timer mode to kFLEXIO_TimerModeDual8BitPWM to start timer */
        fxioTimerConfig.timerMode = kFLEXIO_TimerModeDual8BitPWM;
    }
    else if (duty == 100)
    {
        fxioTimerConfig.pinPolarity = kFLEXIO_PinActiveLow;
    }
    else if (duty == 0)
    {
        /* Set high level as active level */
        fxioTimerConfig.pinPolarity = kFLEXIO_PinActiveHigh;
    }
    else
    {
        return kStatus_Fail;
    }

    FLEXIO_SetTimerConfig(DEMO_FLEXIO_BASEADDR, DEMO_FLEXIO_TIMER_CH, &fxioTimerConfig);

    s_flexioGetPwmDutyCycle[DEMO_FLEXIO_TIMER_CH] = duty;

    return kStatus_Success;
}

/*!
 * brief Set PWM output in idle status (high or low).
 *
 * param base               FlexIO peripheral base address
 * param timerChannel       FlexIO timer channel
 * param idleStatus         True: PWM output is high in idle status; false: PWM output is low in idle status
 */
static void FLEXIO_SetPwmOutputToIdle(FLEXIO_Type *base, uint8_t timerChannel, bool idleStatus)
{
    flexio_timer_config_t fxioTimerConfig;

    /* Configure the timer DEMO_FLEXIO_TIMER_CH for generating PWM */
    fxioTimerConfig.triggerSelect   = FLEXIO_TIMER_TRIGGER_SEL_SHIFTnSTAT(0U);
    fxioTimerConfig.triggerSource   = kFLEXIO_TimerTriggerSourceInternal;
    fxioTimerConfig.triggerPolarity = kFLEXIO_TimerTriggerPolarityActiveLow;
    fxioTimerConfig.pinConfig       = kFLEXIO_PinConfigOutput;
    fxioTimerConfig.pinSelect       = DEMO_FLEXIO_OUTPUTPIN; /* Set pwm output */
    fxioTimerConfig.timerMode       = kFLEXIO_TimerModeDisabled;
    fxioTimerConfig.timerOutput     = kFLEXIO_TimerOutputOneNotAffectedByReset;
    fxioTimerConfig.timerDecrement  = kFLEXIO_TimerDecSrcOnFlexIOClockShiftTimerOutput;
    fxioTimerConfig.timerDisable    = kFLEXIO_TimerDisableNever;
    fxioTimerConfig.timerEnable     = kFLEXIO_TimerEnabledAlways;
    fxioTimerConfig.timerReset      = kFLEXIO_TimerResetNever;
    fxioTimerConfig.timerStart      = kFLEXIO_TimerStartBitDisabled;
    fxioTimerConfig.timerStop       = kFLEXIO_TimerStopBitDisabled;
    fxioTimerConfig.timerCompare    = 0U;

    /* Clear TIMCMP register */
    base->TIMCMP[timerChannel] = 0;

    if (idleStatus)
    {
        /* Set low level as active level */
        fxioTimerConfig.pinPolarity = kFLEXIO_PinActiveLow;
    }
    else
    {
        /* Set high level as active level */
        fxioTimerConfig.pinPolarity = kFLEXIO_PinActiveHigh;
    }

    FLEXIO_SetTimerConfig(DEMO_FLEXIO_BASEADDR, timerChannel, &fxioTimerConfig);

    s_flexioGetPwmDutyCycle[timerChannel] = 0;
}

#if defined(FSL_FEATURE_FLEXIO_HAS_PIN_STATUS) && FSL_FEATURE_FLEXIO_HAS_PIN_STATUS
/*!
 * brief Get the pwm dutycycle value.
 *
 * param base          FlexIO peripheral base address
 * param timerChannel  FlexIO timer channel
 * param channel       FlexIO as pwm output channel number
 *
 * return Current channel dutycycle value.
 */
static uint8_t PWM_GetPwmOutputState(FLEXIO_Type *base, uint8_t timerChannel, uint8_t channel)
{
    if ((base->PIN & (1U << channel)) ^ (base->TIMCTL[timerChannel] & FLEXIO_TIMCTL_PINPOL_MASK))
    {
        return kFLEXIO_PwmHigh;
    }
    else
    {
        return kFLEXIO_PwmLow;
    }
}
#endif

/*!
 * @brief Main function
 */
int main(void)
{
    uint8_t duty            = 0;
    uint8_t idleState       = 0;
    uint32_t dutyCycleValue = 0;
    uint32_t idleStateValue = 0;
    flexio_config_t fxioUserConfig;

    /* Init board hardware */
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach FRO HF to FLEXIO */
    CLOCK_SetClkDiv(kCLOCK_DivFlexioClk, 4u);
    CLOCK_AttachClk(kFRO_HF_to_FLEXIO);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitBootPeripherals();
    while(1)
    {

    }

}
