# Zuma-Deluxe-HD
A remake of the legendary 2003 PopCap game with improved textures and support for 16:9 aspect ratio. Made by me and [s4lat](https://github.com/s4lat).

![preview](https://user-images.githubusercontent.com/52833080/162574925-d6c9f657-c2e4-43e3-860d-412e60f14834.png)

> [!NOTE]
> **Project now (2026-10-26) should build on MacOS and Linux.**
  >
  > Tested on:
  > - Windows 10 - x86_64
  > - MacOS 26.6.2 (25G83) - M1 ARM
  > - Linux: linuxmint 7.0.0 - x86_64
  > - Linux: arch 7.2.11 - x86_64 
  > 
  > thx for testing on linux: [@Adolf-Eleador-II](https://github.com/Adolf-Eleador-II), [@ilanufriev](https://github.com/ilanufriev), [@Tarat0r](https://github.com/Tarat0r)

> [!WARNING]
> **Audio is temporarily unavailable :(** 
>
> Sound is currently not implemented because I prioritized porting the game to other platforms. However, I am already researching open-source audio libraries to fix this. Stay tuned!


## [Gameplay video on YouTube](https://www.youtube.com/watch?v=VZYsNUUAozM&ab_channel=GalaxyShad)

## 🎥 Media 
![Zuma_Deluxe_HD_Remake](https://github.com/GalaxyShad/Zuma-Deluxe-HD/assets/52833080/a84a71f4-385a-4d01-b4ce-b9ccf47d6f85)
![Zuma_Deluxe_HD_Remake (1)](https://github.com/GalaxyShad/Zuma-Deluxe-HD/assets/52833080/a006a96f-805f-46d1-ac8f-340524df3cdd)

## Direct link to download the latest build
 [⬇️ [Stable] ZumaHD v0.1.0 Windows.zip ](https://github.com/GalaxyShad/Zuma-Deluxe-HD/releases/download/v0.1.0/zuma-hd-v0.1.0-windows.zip)

## ⛓️ Dependencies
* [SDL2](https://github.com/libsdl-org/SDL/tree/SDL2)
* [Expat](https://github.com/libexpat/libexpat)

## 🛠️ Build via CMake
```
CMake -G <your generator>
```

## 🛠️ Build via console
```
<your compiler> src\*.c -I include\ -o bin\Zuma.exe -lSDL2 -lSDL2main -lSDL2_Image -lBass -lBass_Fx -llibexpat -mwindows
```

## 🔗 Sources used
Thanks to [hclxing](https://hclxing.wordpress.com/2017/06/27/reverse-engineering-zuma-deluxes-level-file/) for the article on how paths are stored in .dat files.

## 📃 License 
Source code is under MIT License, but all assets (in content folder) belong to [PopCap and EA games](https://www.ea.com/ea-studios/popcap)
