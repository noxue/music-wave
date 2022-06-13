#define MINIAUDIO_IMPLEMENTATION
#include "SDL.h"
#include <pthread.h>
#include <stdbool.h>
#include <miniaudio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LINE_MAX 1500
#define LINE_HEIGHT 300

bool is_exit = false;

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
    if(frameCount==0)exit(0);
    ma_decoder *pDecoder = (ma_decoder *)pDevice->pUserData;
    if (pDecoder == NULL)
    {
        return;
    }

    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);
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

    /*
    if (argc < 2)
    {
        printf("No input file.\n");
        return -1;
    }
*/
    result = ma_decoder_init_file("1.mp3", NULL, &decoder);
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
    if (SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        SDL_Window *window = NULL;
        SDL_Renderer *renderer = NULL;

        if (SDL_CreateWindowAndRenderer(1000, 800, SDL_WINDOW_RESIZABLE, &window, &renderer) == 0)
        {
            SDL_bool done = SDL_FALSE;

            // 多线程
            pthread_t thread;
            pthread_create(&thread, NULL, thread_func, NULL);

            while (!done)
            {
                SDL_Event event;

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);

                SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
                pthread_mutex_lock(&dots_mutex);

                for (int i = 0; i < LINE_MAX; i++)
                {
                    SDL_RenderDrawLine(renderer, i, LINE_HEIGHT + dots_left[i], i + 1, LINE_HEIGHT + dots_left[i + 1]);
                }

                SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
                for (int i = 0; i < LINE_MAX; i++)
                {
                    SDL_RenderDrawLine(renderer, i, LINE_HEIGHT * 2 + dots_right[i], i + 1, LINE_HEIGHT * 2 + dots_right[i + 1]);
                }

                // for (int i = LINE_MAX; i < LINE_MAX * 2; i++)
                // {
                //
                // }
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
                        case SDLK_UP:

                            break;
                        }
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
    }
    is_exit = true;
    SDL_Quit();
    return 0;
}