#ifndef USEFULTYPES_H
#define USEFULTYPES_H

typedef struct v2i {
    int x, y;
} v2i_t;

typedef struct v2f {
    float x, y;
} v2f_t;

static v2f_t v2f_t_default = { 0,0 };

typedef struct frect {
    float x, y, width, height;
} frect_t;

typedef struct irect {
    int x, y, width, height;
} irect_t;

#endif