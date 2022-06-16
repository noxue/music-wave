#define MINIAUDIO_IMPLEMENTATION
#include "SDL.h"
#include <pthread.h>
#include <stdbool.h>
#include <miniaudio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LINE_MAX 1300
#define LINE_HEIGHT 250

bool is_exit = false;
char *filename = NULL;
int dots_left[LINE_MAX] = {0};
int dots_right[LINE_MAX] = {0};
pthread_mutex_t dots_mutex;

// 添加元素
void add(int *lines_array, int v)
{

    // 集体左移1位
    for (int i = 0; i < LINE_MAX - 1; i++)
    {
        lines_array[i] = lines_array[i + 1];
    }
    // 新值添加在最后
    lines_array[LINE_MAX - 1] = v;
}

void data_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
    ma_decoder *pDecoder = (ma_decoder *)pDevice->pUserData;
    if (pDecoder == NULL)
    {
        return;
    }

    ma_result result = ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);
    if (result != MA_SUCCESS)
    {
        is_exit = true;
        return;
    }
    pthread_mutex_lock(&dots_mutex);
    for (int i = 0; i < frameCount; i++)
    {
        add(dots_left, ((float *)pOutput)[2 * i] * LINE_HEIGHT);
        add(dots_right, ((float *)pOutput)[2 * i + 1] * LINE_HEIGHT);
    }
    pthread_mutex_unlock(&dots_mutex);

    (void)pInput;
}

void *thread_func(void *arg)
{
    ma_result result;
    ma_decoder decoder;
    ma_device_config deviceConfig;
    ma_device device;

    result = ma_decoder_init_file(filename, NULL, &decoder);
    if (result != MA_SUCCESS)
    {
        printf("Could not load file");
        return NULL;
    }

    ma_waveform_config sineWaveConfig;
    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate = decoder.outputSampleRate;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = &decoder;

    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS)
    {
        printf("Failed to open playback device.\n");
        ma_decoder_uninit(&decoder);
        return NULL;
    }

    if (ma_device_start(&device) != MA_SUCCESS)
    {
        printf("Failed to start playback device.\n");
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        return NULL;
    }
    while (!is_exit)
    {
        SDL_Delay(10);
    }

    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);

    return NULL;
}

int main(int argc, char *argv[])
{
    // 获取第一个参数
    if (argc < 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    filename = argv[1];
    printf("%s\n", argv[1]);

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Window *window = SDL_CreateWindow("音乐波形图 - 不学网(noxue.com)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, LINE_MAX, LINE_HEIGHT * 3.5, SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // SDL_CreateWindowAndRenderer(LINE_MAX, LINE_HEIGHT * 3.5, SDL_WINDOW_RESIZABLE, &window, &renderer) == 0
    if (window == NULL || renderer == NULL)
    {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_bool done = SDL_FALSE;

    // 多线程
    pthread_t thread;
    pthread_create(&thread, NULL, thread_func, NULL);

    // 获取当前时间戳,用于限制帧数
    uint32_t last_time = SDL_GetTicks();

    while (!done)
    {
        // 一秒40帧
        uint32_t current_time = SDL_GetTicks();
        if (current_time - last_time < 1000 / 40)
        {
            SDL_Delay(1000 / 40 - (current_time - last_time));
        }

        last_time = current_time;

        SDL_Event event;

        SDL_SetRenderDrawColor(renderer, 0, 0, 30, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        pthread_mutex_lock(&dots_mutex);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
        for (int i = 0; i < LINE_MAX; i++)
        {
            SDL_RenderDrawLine(renderer, i, LINE_HEIGHT + dots_left[i], i + 1, LINE_HEIGHT + dots_left[i + 1]);
        }

        SDL_SetRenderDrawColor(renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
        for (int i = 0; i < LINE_MAX; i++)
        {
            SDL_RenderDrawLine(renderer, i, LINE_HEIGHT * 2.5 + dots_right[i], i + 1, LINE_HEIGHT * 2.5 + dots_right[i + 1]);
        }

        pthread_mutex_unlock(&dots_mutex);

        SDL_RenderPresent(renderer);
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                done = SDL_TRUE;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    done = SDL_TRUE;
                }
            }
        }
    }

    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
    }
    if (window)
    {
        SDL_DestroyWindow(window);
    }

    is_exit = true;
    SDL_Quit();
    return 0;
}