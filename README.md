# NXP Application Code Hub
[<img src="https://mcuxpresso.nxp.com/static/icon/nxp-logo-color.svg" width="100"/>](https://www.nxp.com)

## How to use FlexIO state mode to generate center aligned PWM on MCXN Series MCU
This software accompanies application note, which describes how to use FlexIO state mode to generate center aligned PWM on MCXN Series MCU.

This application note describes how to use FlexIO state mode to generate center aligned PWM waveform on MCXN Series MCU.

MCXN Series MCU has eight FlexIO shifters which can implement up to eight states. In every state, different level logic can be output on up to eight FlexIO data pins. The transitions between states can be controlled by up to eight FlexIO timers.

Controlling state transitions using external input pins is a common demonstration method for FlexIO. However, there are many instances where users simply want to use a state machine controlled by a timer sequence without considering external input states. This application note is based on such a starting point, using timers to control the state machine composed of shifters, to generate center aligned PWM waveforms.

The config tool provided by NXP can quickly generate functional code for FlexIO. One of the purposes of this application is to demonstrate to users how to use the config tool to implement the state machine function of FlexIO, making the implementation simple and fast.

 Performance:

The FlexIO timer can use the MCU's system clock(150MHz) as its clock source.

1) For example, to generate a 2 kHz PWM wave, the resolution per cycle can reach 16 bits. 
2) If generating a 250 kHz PWM wave with a period of 40 microseconds, the resolution can reach 600 points (40 microseconds divided by the inverse of 150 MHz). it is about 9 bits resolution for full range tuning.

#### Boards: FRDM-MCXN947
#### Categories: Motor Control
#### Peripherals: GPIO, PWM
#### Toolchains: MCUXpresso IDE

## Table of Contents
1. [Software](#step1)
2. [Hardware](#step2)
3. [Setup](#step3)
4. [Results](#step4)
6. [Support](#step5)
7. [Release Notes](#step6)

## 1. Software<a name="step1"></a>
- [MCUXpresso IDE V11.10 or later](https://www.nxp.com/design/design-center/software/development-software/mcuxpresso-software-and-tools-/mcuxpresso-integrated-development-environment-ide:MCUXpresso-IDE).
- SDK_2_16_000_FRDM-MCXN947
- MCUXpresso for Visual Studio Code: This example supports MCUXpresso for Visual Studio Code, for more information about how to use Visual Studio Code please refer [here](https://www.nxp.com/design/training/getting-started-with-mcuxpresso-for-visual-studio-code:TIP-GETTING-STARTED-WITH-MCUXPRESSO-FOR-VS-CODE).

## 2. Hardware<a name="step2"></a>
- Type-C USB cable
- FRDM-MCXN947
- Personal Computer
- Logic device: Saleae logic pro16
- Some Dupont wires

## 3. Setup<a name="step3"></a>
### 3.1 Step 1

Connect logic device to FRDM-MCXN236 with some DuPont wires, as shown below:

![hardware](images/hardware.png)

### 3.2 Step 2

- Import the project to MCUXpresso IDE.

1. Open MCUXpresso IDE, in the Quick Start Panel, choose **Import from Application Code Hub**.

   ​	![](images/import_project_1.png)

2. Enter the demo name in the search bar.

   ![](images/import_project_2.png) 

3. Click **Copy GitHub link**, MCUXpresso IDE will automatically retrieve project attributes, then click **Next>**.

   ​	![](images/import_project_3.png)

4. Select **main** branch and then click **Next>**, Select the MCUXpresso project, click **Finish** button to complete import.

   ​	![](images/import_project_4.png)

- Connect the micro USB cable between the PC host and the USB port (J17) on the board.
- Compile and download to the board.
- Reset and run.

## 4. Results<a name="step4"></a>
Reset the board, open the logic app on PC, and capture the timing as below.



![](images/result.png)

## 5. Support<a name="step5"></a>
*Provide URLs for help here.*

#### Project Metadata

<!----- Boards ----->
[![Board badge](https://img.shields.io/badge/Board-FRDM&ndash;MCXN947-blue)]()

<!----- Categories ----->
[![Category badge](https://img.shields.io/badge/Category-MOTOR%20CONTROL-yellowgreen)](https://github.com/search?q=org%3Anxp-appcodehub+motor_control+in%3Areadme&type=Repositories)

<!----- Peripherals ----->
[![Peripheral badge](https://img.shields.io/badge/Peripheral-GPIO-yellow)](https://github.com/search?q=org%3Anxp-appcodehub+gpio+in%3Areadme&type=Repositories)
[![Peripheral badge](https://img.shields.io/badge/Peripheral-PWM-yellow)](https://github.com/search?q=org%3Anxp-appcodehub+pwm+in%3Areadme&type=Repositories)

<!----- Toolchains ----->
[![Toolchain badge](https://img.shields.io/badge/Toolchain-MCUXPRESSO%20IDE-orange)](https://github.com/search?q=org%3Anxp-appcodehub+mcux+in%3Areadme&type=Repositories)

Questions regarding the content/correctness of this example can be entered as Issues within this GitHub repository.

>**Warning**: For more general technical questions regarding NXP Microcontrollers and the difference in expected functionality, enter your questions on the [NXP Community Forum](https://community.nxp.com/)

[![Follow us on Youtube](https://img.shields.io/badge/Youtube-Follow%20us%20on%20Youtube-red.svg)](https://www.youtube.com/NXP_Semiconductors)
[![Follow us on LinkedIn](https://img.shields.io/badge/LinkedIn-Follow%20us%20on%20LinkedIn-blue.svg)](https://www.linkedin.com/company/nxp-semiconductors)
[![Follow us on Facebook](https://img.shields.io/badge/Facebook-Follow%20us%20on%20Facebook-blue.svg)](https://www.facebook.com/nxpsemi/)
[![Follow us on Twitter](https://img.shields.io/badge/X-Follow%20us%20on%20X-black.svg)](https://x.com/NXP)

## 6. Release Notes<a name="step6"></a>
| Version | Description / Update                           | Date                        |
|:-------:|------------------------------------------------|----------------------------:|
| 1.0     | Initial release on Application Code Hub        | 1/24/2025 |

<small>
<b>Trademarks and Service Marks</b>: There are a number of proprietary logos, service marks, trademarks, slogans and product designations ("Marks") found on this Site. By making the Marks available on this Site, NXP is not granting you a license to use them in any fashion. Access to this Site does not confer upon you any license to the Marks under any of NXP or any third party's intellectual property rights. While NXP encourages others to link to our URL, no NXP trademark or service mark may be used as a hyperlink without NXP’s prior written permission. The following Marks are the property of NXP. This list is not comprehensive; the absence of a Mark from the list does not constitute a waiver of intellectual property rights established by NXP in a Mark.
</small>
<br>
<small>
NXP, the NXP logo, NXP SECURE CONNECTIONS FOR A SMARTER WORLD, Airfast, Altivec, ByLink, CodeWarrior, ColdFire, ColdFire+, CoolFlux, CoolFlux DSP, DESFire, EdgeLock, EdgeScale, EdgeVerse, elQ, Embrace, Freescale, GreenChip, HITAG, ICODE and I-CODE, Immersiv3D, I2C-bus logo , JCOP, Kinetis, Layerscape, MagniV, Mantis, MCCI, MIFARE, MIFARE Classic, MIFARE FleX, MIFARE4Mobile, MIFARE Plus, MIFARE Ultralight, MiGLO, MOBILEGT, NTAG, PEG, Plus X, POR, PowerQUICC, Processor Expert, QorIQ, QorIQ Qonverge, RoadLink wordmark and logo, SafeAssure, SafeAssure logo , SmartLX, SmartMX, StarCore, Symphony, Tower, TriMedia, Trimension, UCODE, VortiQa, Vybrid are trademarks of NXP B.V. All other product or service names are the property of their respective owners. © 2021 NXP B.V.
</small>