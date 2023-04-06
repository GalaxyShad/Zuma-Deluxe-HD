#include "../HQC.h"

#include <SDL2/SDL.h>
#include <stdarg.h>
#include <stdio.h>

void HQC_RaiseErrorFormat(const char* format, ...) {
    va_list args;
    va_start(args, format);
    HQC_RaiseErrorHeaderFormat(NULL, format, args);
    va_end(args);
}


void HQC_RaiseErrorHeaderFormat(const char* header, const char* format, ...) {
    va_list args;
    va_start(args, format);

    int len = vsnprintf(NULL, 0, format, args);
    va_end(args);

    // format message
    char* msg = malloc(len + 1); 
    va_start(args, format);
    vsnprintf(msg, len + 1, format, args);
    va_end(args);

    SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_ERROR,
        header ? header : "Fatal",
        msg,
        NULL
    );

    exit(1);
}


void HQC_Log(const char* format, ...) {
    va_list args;

    va_start(args, format);
    int len = vsnprintf(NULL, 0, format, args);
    va_end(args);

    // format message
    char* msg = malloc(len + 1); // or use heap allocation if implementation doesn't support VLAs
    va_start(args, format);
    vsnprintf(msg, len + 1, format, args);
    va_end(args);

    printf(msg);
    printf("\n");
}