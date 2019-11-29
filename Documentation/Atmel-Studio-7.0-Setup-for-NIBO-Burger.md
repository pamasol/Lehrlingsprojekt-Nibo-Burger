This is a step-by-step guide to setup Atmel Studio 7.0 for programming NIBO Burger on a **Windows 10** 64 bit operating system. It is based on a German blog entry from [roboter.cc](https://www.roboter.cc/index.php?option=com_kunena&view=topic&catid=19&id=2614&Itemid=20) where user AitanaAlmaguer uploaded the content from this Wiki page as [PDF in German](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/files/3652530/ProgrammierungdesNIBOBurgersmitAtmelStudio7.0.pdf) language.

Please make sure to install the software as described [here]( https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger#installing-software) before following this guide.

## USBasp programmer

[USBasp](https://www.fischl.de/usbasp/) is a USB in-circuit programmer for Atmel AVR controllers. It simply consists of an ATMega88 or an ATMega8 and a couple of passive components. The programmer uses a firmware-only USB driver, no special USB controller is needed.

**NIBO Burger** comes with an **integrated USBasp programmer**. It consists mainly of an ATtiny44A. This microcontroller serves as programming interface (bootloader) for the main processor (ATmega16A) as well as charge controller.
To program the NIBO Burger in Atmel Studio, the USBasp programmer must be setup as follows.

1. Open Atmel Studio > Klick **Tools** > **External Tools...**

![01_tools_external_tools](https://user-images.githubusercontent.com/7021907/66304606-ece03f80-e8fd-11e9-9703-1f5a0dde468f.png)

2. Click **Add** button and fill in forms as shown below. Make sure **Use Output window** is checked. Click **OK** button in a second step.

![02_tools_external_tools](https://user-images.githubusercontent.com/7021907/66304607-ed78d600-e8fd-11e9-8409-3836b5b95306.png)

**Title:** `USBASP`

**Command:** `C:\WinAVR-20100110\bin\avrdude.exe`

> Path to avrdude.exe parameter may differs on your operating system.

**Arguments:** `avrdude -c usbasp -p atmega16 -U flash:w:$(ProjectDir)Debug\$(TargetName).hex:i`

3. Click **Tools** > **Customize**.

![03_customize](https://user-images.githubusercontent.com/7021907/66304608-ed78d600-e8fd-11e9-8e43-786487799952.png)

4. Click tab **Commands** and radio button **Toolbar**

5. Click drop-down **Device and Debugger**

6. Click button **Add Command...**. Look for category **Tools** and click **External Command 1** as shown in screenshot below.

![04_customize](https://user-images.githubusercontent.com/7021907/66304609-ed78d600-e8fd-11e9-9e5d-d44a87e9808e.png)

7. If everything went well, button **USBASP** will be shown in the toolbar.

![14_button_toolbar](https://user-images.githubusercontent.com/7021907/66306564-3e8ac900-e902-11e9-9f91-c9b6a85b4c11.png)


## Creating a project

> Following steps can be bypassed if you load a project template as described in the [next chapter](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/wiki/Atmel-Studio-7.0-Setup-for-NIBO-Burger#adding-and-using-a-project-template).

1. Open Atmel Studio > Click **File** > **New** > **Project...**

2. Choose **GCC C++ Executable Project** as shown in image below.

![05_new_project](https://user-images.githubusercontent.com/7021907/66304610-ed78d600-e8fd-11e9-930e-2eb99865ce3f.png)

3. Search for **atmega16a**, choose appropriate microcontroller and click **OK**.

![06_atmega](https://user-images.githubusercontent.com/7021907/66304611-ed78d600-e8fd-11e9-9b14-ba9ef78f9b6d.png)

4. Click **Project** > **Project Properties...** as shown in sceenshot below.

![07_get_application_properties](https://user-images.githubusercontent.com/7021907/66304612-ee116c80-e8fd-11e9-82c8-ea2f90aa31a5.png)

5. Click **Toolchain** > **Directories** and add NIBORoboLib (`C:\Program Files\NiboRoboLib\src\`) directory at first position as shown below. Make sure checkbox **Relative Path** is **not** checked.

![08_directories](https://user-images.githubusercontent.com/7021907/66304613-ee116c80-e8fd-11e9-961d-fd480fe22f2a.png)

> Path may differs on your operating system.

6. Click **Toolchain** > **General** and check following two checkboxes.

![09_general](https://user-images.githubusercontent.com/7021907/66304614-ee116c80-e8fd-11e9-8052-0bf56151bbfb.png)

7. Click **Toolchain** > **Libraries** and add all libraries (file ending `.a`) that are in folder `C:\Program Files\NiboRoboLib\lib\niboburger-m16-15`. Just fill in the **name** of libraries **without** the `.a` at the end. Make sure checkbox **Relative Path** is **not** checked.

![10_libraries](https://user-images.githubusercontent.com/7021907/66304615-ee116c80-e8fd-11e9-856e-56b0212b2ab6.png)

> Path may differs on your operating system.

6. Now you should be able to compile a NIBO Burger program with clicking following button.

![15_button_compile](https://user-images.githubusercontent.com/7021907/66307426-b9ed7a00-e904-11e9-8d3a-c5e2bc89415b.png)


## Adding and using a project template

Instead creating a new project from the ground up, it is also possible to import a template.

1. Download the template as ZIP package from [here](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/files/3652529/NIBOBurger_Vorlage.zip).

2. Open Atmel Studio > Click **File** > **Import** > **Project Template...** as shown in the screenshot below.

![16_project_template_import](https://user-images.githubusercontent.com/7021907/66314519-726eea00-e914-11e9-97ea-0c821e029f30.png)

3. Select the ZIP file and click **OK**.

![18_choose_project_template](https://user-images.githubusercontent.com/7021907/66316314-c29b7b80-e917-11e9-95c2-cce67efb0ef0.png)

4. Now you are able to use the project template. When creating a new project just choose `NIBO Burger_Vorlage` as shown below.

![17_start_project_with_project_template](https://user-images.githubusercontent.com/7021907/66314949-24a6b180-e915-11e9-91db-c61ddff22d4a.png)

5. The template does not include the archive libraries (file ending `.a`). Add them via  **Project** > **Project Properties...** > Tab **Toolchain** > In the Explorer click **AVR/GNU Linker** > **Libraries**.