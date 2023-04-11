#pragma once

typedef struct SceneProps {
    void (*load)(void);
    void (*update)(void);
    void (*draw)(void);
} SceneProps;


