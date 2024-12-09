# Zuma-Deluxe-HD
A remake of the legendary 2003 PopCap game with improved textures and support for 16:9 aspect ratio. Made by me and [s4lat](https://github.com/s4lat).

![preview](https://user-images.githubusercontent.com/52833080/162574925-d6c9f657-c2e4-43e3-860d-412e60f14834.png)

## 2024-2025 Project State 
Currently, work is being done on version 2.0.0*, which features a significantly revamped project architecture with an emphasis on code readability and maintainability. 
The "engine" API is now called HQC Framework, 
a own ECS API has been added,
and the chain movement logic has been reworked 
to achieve smoother and more accurate to original behavior. 
There are plans to replace many of the used libraries 
and support a greater number of platforms, 
including replacing the BASS LIB with an open-source analog. 
However, at this stage, this version is not yet playable. <br/>

If you want to build a playable build, you need the [v0.1.0-release branch](https://github.com/GalaxyShad/Zuma-Deluxe-HD/tree/release-v0.1.0). Stay tuned!

*when I have free time between university assignments :D

# Description of v0.1.0-release version


## [Gameplay video on YouTube](https://www.youtube.com/watch?v=VZYsNUUAozM&ab_channel=GalaxyShad)

## 🎥 Media 
![Zuma_Deluxe_HD_Remake](https://github.com/GalaxyShad/Zuma-Deluxe-HD/assets/52833080/a84a71f4-385a-4d01-b4ce-b9ccf47d6f85)
![Zuma_Deluxe_HD_Remake (1)](https://github.com/GalaxyShad/Zuma-Deluxe-HD/assets/52833080/a006a96f-805f-46d1-ac8f-340524df3cdd)

## Direct link to download the latest build
 [⬇️ [Stable] ZumaHD v0.1.0 Windows.zip ](https://github.com/GalaxyShad/Zuma-Deluxe-HD/releases/download/v0.1.0/zuma-hd-v0.1.0-windows.zip)

## ⛓️ Dependencies
* [SDL2 	   2.26.5](https://www.libsdl.org)
* [SDL2_Image 	2.6.3](https://www.libsdl.org/projects/SDL_image)
* [Expat 		2.5.0](https://github.com/libexpat/libexpat)
* [BASS and BASS FX](http://www.un4seen.com)

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

## 10.04.2023 Status
Currently project framework is under reconstruction, check ["api-rework" branch](https://github.com/GalaxyShad/Zuma-Deluxe-HD/tree/api-rework).
