Atmel Studio 7 supports setting up and use different compilers through what they call "Flavours".

## Add flavour in Atmels Studio Settings

1. Click **Tools** > **Options...** 

![11_Add_Flower](https://user-images.githubusercontent.com/7021907/66304616-eeaa0300-e8fd-11e9-8576-626caffe4b13.png)

2. Click **Toolchain** and in drop-down choose **Atmel AVR 8-bit (CPP language)**

3. Click **Add Flower** and add **WinAVR** with path `C:\WinAVR-20100110\bin`

![12_Add_Flower](https://user-images.githubusercontent.com/7021907/66304617-eeaa0300-e8fd-11e9-945c-6b4b8500d993.png)

> Path may differs on your operating system.

4. Do step above as well for **Atmel AVR 8-bit (C language)**

![13_Add_Flower](https://user-images.githubusercontent.com/7021907/66304618-eeaa0300-e8fd-11e9-9a1b-d0e7cdedbc78.png)

## Use flavour in a project

1. Go to the project **Properties**

2. Select **Advanced** and then in the drop-down **Toolchain Flavour** select **WinAVR** as shown in the screenshot below.

![23_Choose_Flower_in_Project](https://user-images.githubusercontent.com/7021907/66546056-5f3e6300-eb3c-11e9-9afd-5cc03fc7ebfc.png)