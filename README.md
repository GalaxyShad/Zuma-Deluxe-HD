# Zuma-Deluxe-HD
A remake of the legendary 2003 PopCap game with improved textures and support for 16:9 aspect ratio. Made by me and [s4lat](https://github.com/s4lat).

![preview](https://user-images.githubusercontent.com/52833080/162574925-d6c9f657-c2e4-43e3-860d-412e60f14834.png)

# Links
[Gameplay video](https://www.youtube.com/watch?v=VZYsNUUAozM&ab_channel=GalaxyShad)

# Sources used
Thanks to [hclxing](https://hclxing.wordpress.com/2017/06/27/reverse-engineering-zuma-deluxes-level-file/) for the article on how paths are stored in .dat files.

# Dependencies
* [SDL2 	   2.26.5](https://www.libsdl.org)
* [SDL2_Image 	2.6.3](https://www.libsdl.org/projects/SDL_image)
* [Expat 		2.5.0](https://github.com/libexpat/libexpat)
* [BASS and BASS FX](http://www.un4seen.com)

# Build via CMake
```
CMake -G <your generator>
```

# Build via console
```
<your compiler> src\*.c -I include\ -o bin\Zuma.exe -lSDL2 -lSDL2main -lSDL2_Image -lBass -lBass_Fx -llibexpat -mwindows
```

# License 
Source code is under MIT License, but all assets (in content folder) belong to [PopCap and EA games](https://www.ea.com/ea-studios/popcap)

# 10.04.2023 Status
Currently project framework is under reconstruction, check ["api-rework" branch](https://github.com/GalaxyShad/Zuma-Deluxe-HD/tree/api-rework).
