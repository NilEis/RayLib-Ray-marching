#include "video/video_main.h"
#include "raylib.h"

int video_init(int width, int height, const char *title)
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(width, height, title);
    return 0;
} 