/******************************************************************************
* Copyright © 2024-2035 Light Zhang <mapaware@hotmail.com>, MapAware, Inc.
* ALL RIGHTS RESERVED.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
******************************************************************************/
/**
 * @file mapaware-main.c
 * @brief MapAware SDL Window Application.
 *
 * @author mapaware@hotmail.com
 * @copyright © 2024-2030 mapaware.top All Rights Reserved.
 * @version 0.0.5
 *
 * @since 2024-09-29 10:20:21
 * @date 2024-10-29 01:40:06
 *
 * @note
 *
 *   SDL 下载:
 *     https://libsdl.org/
 *
 *     https://code.visualstudio.com/docs/editor/variables-reference
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// SDL
#if defined(_MSC_VER)
  // link to SDL2.lib and SDL2.dll required
  # include <SDL.h>
  # define SDL2_LOGFILE "C:\\TEMP\\MapAware_MSVC-SDL2.log"
#elif defined(__MINGW64__)
  # include <SDL2/SDL.h>
  # define SDL2_LOGFILE "C:\\TEMP\\MapAware_MINGW-SDL2.log"
#else
  # include <SDL2/SDL.h>
  # define SDL2_LOGFILE "/tmp/MapAware-SDL2.log"
#endif


FILE *log_file = NULL;

// 自定义日志回调函数，将日志输出到指定文件
void SdlLogCallback(void* userdata, int category, SDL_LogPriority priority, const char* message)
{
    if (log_file == NULL) {
        // 打开日志文件

        log_file = fopen(SDL2_LOGFILE, "w");
        if (log_file == NULL) {
            printf("Cannot open log file:%s\n", SDL2_LOGFILE);
            abort();
        }
    }

    // 将日志写入文件
    fprintf(log_file, "SDL2: %s\n", message);

    // 立即刷新文件缓冲区，确保日志被写入
    fflush(log_file);
}


/**
 * MapAware 程序主入口
 *    sdl2.dll 必须放在本程序目录下
 */
#undef main

int main(int argc, char * argv[])
{
    // 背景图片: earth.bmp 放在本程序的同目录下
    char path[256];
    snprintf(path, 255, "%s", argv[0]);
    if (strrchr(path, '/')) {
        *strrchr(path, '/') = '\0';
        strcat(path, "/earth.bmp");
    } else if (strrchr(path, '\\')) {
        *strrchr(path, '\\') = '\0';
        strcat(path, "\\earth.bmp");
    } else {
        // bad path
        return -1;
    }
    path[255] = '\0';

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        abort();
    }

    // 设置自定义的日志回调函数
    SDL_LogSetOutputFunction(SdlLogCallback, NULL);
    SDL_Log("BMP FILE: %s", path);

    SDL_Window * win = NULL;

    SDL_Log("SDL_CreateWindow flags=SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP");
    win = SDL_CreateWindow("sdlWindow",
                SDL_WINDOWPOS_CENTERED,  // 居中
                SDL_WINDOWPOS_CENTERED,
                640, 480,
                SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);  //  SDL_WINDOW_RESIZABLE

    if (! win) {
        SDL_Log("SDL_CreateWindow error: %s", SDL_GetError());

        // 第一次失败，则用不同的参数二次调用创建窗口
        SDL_Log("SDL_CreateWindow flags=SDL_WINDOW_FULLSCREEN_DESKTOP");
        win = SDL_CreateWindow("sdlWindow",
                SDL_WINDOWPOS_CENTERED,  // 居中
                SDL_WINDOWPOS_CENTERED,
                640, 480,
                SDL_WINDOW_FULLSCREEN_DESKTOP);  //  SDL_WINDOW_RESIZABLE
        if (! win) {
            SDL_Log("SDL_CreateWindow error: %s", SDL_GetError());
            SDL_Quit();
            return 1;
        }

        SDL_Log("SDL_CreateWindow ok\n");
    }

    // 创建一个将绘制到窗口的渲染器，-1 指定我们要加载任何一个
    // 视频驱动程序支持我们传递的标志
    // 标志： SDL_RENDERER_ACCELERATED :想使用硬件加速渲染
    // SDL_RENDERER_PRESENTVSYNC :希望渲染器的当前功能（更新屏幕）是与显示器的刷新率同步
    SDL_Renderer *ren = NULL;

    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (! ren) {
        SDL_Log("SDL_CreateRenderer error: %s\n", SDL_GetError());

        ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);
        if (! ren) {
            SDL_Log("SDL_CreateRenderer error: %s\n", SDL_GetError());
            SDL_DestroyWindow(win);
            SDL_Quit();
            return 1;
        }

        SDL_Log("SDL_CreateRenderer ok\n");
    }

    // 加载图片做窗口背景
    SDL_Surface *bmp = SDL_LoadBMP(path);
    if (bmp == NULL) {
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Log("SDL_LoadBMP error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // To use a hardware accelerated texture for rendering we can create one from
    // the surface we loaded
    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);

    // We no longer need the surface
    SDL_FreeSurface(bmp);
    if (tex == NULL){
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Log("SDL_CreateTextureFromSurface error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // A sleepy rendering loop, wait for 5 seconds and render and present the screen each time
    for (int i = 0; i < 100; ++i) {
        //First clear the renderer
        SDL_RenderClear(ren);

        //Draw the texture
        SDL_RenderCopy(ren, tex, NULL, NULL);

        //Update the screen
        SDL_RenderPresent(ren);

        // Take a quick break after all that hard work
        SDL_Delay(50);
    }

    //Clean up our objects and quit
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);

    SDL_Log("sdlWindow app exit with 0");

    SDL_Quit();

    return 0;
}