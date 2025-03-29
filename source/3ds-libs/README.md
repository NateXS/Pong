# 3ds-libs
Multi purpose C++ library for easier Homebrew development on the Nintendo 3DS [Work in progress]

## Features
- Easy sprite rendering
- Scene system with tree structure
- Easy audio playing. Supported formats: `OGG`
- Text console with automatic newline and scrolling 

## Example
```cpp
#include <3ds.h> // import the necessary DevkitARM 3DS Homebrew library
#include "3ds-libs/include/Console.h"// import Console from 3ds-libs

int main(int argc, char **argv) {
    gfxInitDefault(); // initialize graphics

    Console *console = new Console(GFX_TOP);// create a console on the top screen
    console->print("Hello World!");
    Console->print("Automatic new line :D");

    //main loop
    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        u32 kHeld = keysHeld();
        if (kDown & KEY_START) {
            console->print("See you later");
            break;// stop the main loop
        }

        gfxFlushBuffers();
        console->render();// render the console
        gspWaitForVBlank();
    }
    // program exits
    gfxExit(); // unitialize graphics
    return 0;
}
```
## Dependencies
First install [everything needed for DevkitPRO and set up your environment](https://www.3dbrew.org/wiki/Setting_up_Development_Environment) if you haven't yet.  
Then install these packages:
- 3ds-libogg
- 3ds-libvorbisidec
```
pacman -S 3ds-libogg 3ds-libvorbisidec
```

## Installing the library
Please first follow the **Dependencies** section.
### Uncompiled library method
Assuming you are using the [DevkitPro app template Makefile](https://github.com/devkitPro/3ds-examples/blob/master/templates/application/Makefile) or a similar Makefile and file structure, and **3ds-libs** is not compiled, follow these steps.
1. `cd source` to go to your directory with your CPP files.
2. `git clone https://github.com/Wolfyxon/3ds-libs` to download the library.
3. Add this somewhere in your Makefile

 ```make
3DS_LIBS     := source/3ds-libs
3DS_LIBS_SRC := $(3DS_LIBS)/source
3DS_LIBS_INC := $(3DS_LIBS)/include
 ```
This will add variables with the paths to the files of the library. Modify the `source/3ds-libs` if you downloaded the library somewhere else.

4. Modify the `SOURCES` variable

Add
```
$(3DS_LIBS_SRC) $(3DS_LIBS_SRC)/renderable
```
To include the .CPP files for compilation. Example setup:
```make
SOURCES		 :=	source $(3DS_LIBS_SRC) $(3DS_LIBS_SRC)/renderable
```

6. Modify the `INCLUDES` variable

Add
```
include $(3DS_LIBS_INC) $(3DS_LIBS_INC)/renderable
```
To include the necessary .H files for compilation. Example setup:
```make
INCLUDES	 :=	include $(3DS_LIBS_INC) $(3DS_LIBS_INC)/renderable
```
7. Modify the `LIBS` variable

Add `-lvorbisidec -logg` at the end of it. **The order must be exactly the same**. Example setup:
```make
LIBS	:= -lcitro2d -lcitro3d -lctru -lm -lvorbisidec -logg
```
This will include the OGG libraries necessary for playing OGG audio

8. Enjoy! Now you can use the `#include` statement to import the headers you need for your code. Example:
```cpp
#include "3ds-libs/include/Console.h"
```
