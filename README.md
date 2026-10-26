# Zuma-Deluxe-HD
A remake of the legendary 2003 PopCap game with improved textures and support for 16:9 aspect ratio. Made by [GalaxyShad](https://github.com/GalaxyShad) and [s4lat](https://github.com/s4lat).

![preview](https://user-images.githubusercontent.com/52833080/162574925-d6c9f657-c2e4-43e3-860d-412e60f14834.png)

> [!NOTE]
> **Since 2026-10-26 the project builds on Windows, macOS and Linux.**
>
> Tested on:
> - Windows 10 - x86_64
> - macOS 26.6.2 (25G83) - M1 ARM
> - Linux Mint (kernel 7.0.0) - x86_64
> - Arch Linux (kernel 7.2.11) - x86_64
>
> Thanks for testing on Linux: [@Adolf-Eleador-II](https://github.com/Adolf-Eleador-II), [@ilanufriev](https://github.com/ilanufriev), [@Tarat0r](https://github.com/Tarat0r)

> [!WARNING]
> **Audio is temporarily unavailable :(**
>
> Sound is currently not implemented because I prioritized porting the game to other platforms. Choosing an open-source audio library is discussed in [#18](https://github.com/GalaxyShad/Zuma-Deluxe-HD/issues/18) - ideas are welcome!

## Gameplay video on YouTube
[![Gameplay video](https://img.youtube.com/vi/VZYsNUUAozM/0.jpg)](https://www.youtube.com/watch?v=VZYsNUUAozM)

## Media
![Zuma Deluxe HD screenshot](https://github.com/GalaxyShad/Zuma-Deluxe-HD/assets/52833080/a84a71f4-385a-4d01-b4ce-b9ccf47d6f85)
![Zuma Deluxe HD gameplay](https://github.com/GalaxyShad/Zuma-Deluxe-HD/assets/52833080/a006a96f-805f-46d1-ac8f-340524df3cdd)

## Latest Windows Release (2021)
 [⬇ [Stable] ZumaHD v0.1.0 Windows x86.zip ⬇](https://github.com/GalaxyShad/Zuma-Deluxe-HD/releases/download/v0.1.0/zuma-hd-v0.1.0-windows.zip) 

> [!NOTE]
> This build is from 2021 and is only for Windows x86. The current code has changed a lot since then, so to play the latest version build it from source.

## How to Build

### Requirements
* C compiler: GCC, Clang or MSVC
* [CMake](https://cmake.org/download/) 4.0 or newer
* Git

Deps: [SDL2](https://github.com/libsdl-org/SDL/tree/SDL2) and [Expat](https://github.com/libexpat/libexpat) are git submodules and are built together with the game, [stb_image](https://github.com/nothings/stb) is in `thirdparty/stb`. You don't need to install them.

### Get the code
```
git clone --recurse-submodules https://github.com/GalaxyShad/Zuma-Deluxe-HD.git
cd Zuma-Deluxe-HD
```
If you already cloned without submodules:
```
git submodule update --init --recursive
```

### Build
```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

### Run
The game and the `content` folder are placed in `build/bin`. Run the game from this folder, because `settings.txt` and `progress.bin` are saved in the current folder:
```
cd build/bin
./ZumaHD
```
On Windows run `ZumaHD.exe`.

## Contributing
This is a hobby project, and any help is very welcome! I'm always happy to see pull requests - big or small.

Some ways to help:
* Fix a bug or improve the code
* Help bring the audio back - see [#18](https://github.com/GalaxyShad/Zuma-Deluxe-HD/issues/18)
* Test the game on your OS and tell how it went
* Report bugs or share ideas in [issues](https://github.com/GalaxyShad/Zuma-Deluxe-HD/issues)

Not sure where to start? Just open an issue or a draft pull request, and we'll figure it out together.

## Sources
Thanks to [hclxing](https://hclxing.wordpress.com/2017/06/27/reverse-engineering-zuma-deluxes-level-file/) for the article on how paths are stored in .dat files.

## License 
Source code is under MIT License, but all assets (in content folder) belong to [PopCap and EA games](https://www.ea.com/ea-studios/popcap)
