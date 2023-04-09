# Zuma-Deluxe-HD
A remake of the legendary 2003 PopCap game with improved textures and support for 16:9 aspect ratio. Made by me and [s4lat](https://github.com/s4lat).

![preview](https://user-images.githubusercontent.com/52833080/162574925-d6c9f657-c2e4-43e3-860d-412e60f14834.png)

# Sources used
Thanks to [hclxing](https://hclxing.wordpress.com/2017/06/27/reverse-engineering-zuma-deluxes-level-file/) for the article on how paths are stored in .dat files.

# Requirements
* [SDL2](https://www.libsdl.org)
* [SDL Image](https://www.libsdl.org/projects/SDL_image)
* [Expat](https://github.com/libexpat/libexpat)
* [BASS and BASS FX](http://www.un4seen.com)

# Build args
```
<your compiler> src\*.c -I include\ -o bin\Zuma.exe -lSDL2 -lSDL2main -lSDL2_Image -lBass -lBass_Fx -llibexpat -mwindows
```


