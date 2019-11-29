If you setup Atmel Studio as described in [Atmel Studio 7.0 Setup for NIBO Burger](https://github.com/pamasol/Lehrlingsprojekt-Nibo-Burger/wiki/Atmel-Studio-7.0-Setup-for-NIBO-Burger), the NIBO Burger libraries are included via project settings as archive libraries (file ending `.a`).

If you want to include the libraries as `.c` and `.h` files, instead as archive libraries, you can follow this guide.

The integration of c files in the project has advantages like:

* Other users do not have to download the library separately. They can just clone the git repo and they are ready to go.
* You can easily navigate through the library and check the functions from the library you are using in your code.

Open Atmel Studio 7.0 and perform following steps.

1. Click **File** > **New** > **Project...** > Choose **NIBO Burger_Vorlage1** as shown in screenshot below.

![19_create_project_based_on_template](https://user-images.githubusercontent.com/7021907/66502731-2cf21e80-eac6-11e9-96d0-31a0f7e32603.png)

2. Add folder **niboruger** with `.c`, `.cpp` and `.h` files to the project as shown in screenshot below. NIBO Burger library can be downloaded from [here](https://sourceforge.net/projects/nibo/).  Click the small icon in the solution exporer that shows all files. **Right click** on folder niboburger and **Include In Project**.

![20_Add_folder_to_project](https://user-images.githubusercontent.com/7021907/66502732-2cf21e80-eac6-11e9-95e4-27ee7b8e43b5.png)

3. Go to project settings and remove `C:\Program Files\NiboRoboLib\src\` path in **Directories**. Add relative project path `..`.

![21_relative_path](https://user-images.githubusercontent.com/7021907/66502735-2d8ab500-eac6-11e9-9619-9ff066849240.png)

4. Since the library includes two motor pid libraries (`motpid.c` and `motpid2.c`), do not compile `motpid2.c`. Otherwise you will get an error. In **Properties** on **Build Action** choose **None** in the drop-down as shown in screenshot below.

![22_dont_compile_motpid2](https://user-images.githubusercontent.com/7021907/66502736-2d8ab500-eac6-11e9-9a0e-67e46616e3ca.png)

5. Now you should be fine and able to compile the project without errors.






