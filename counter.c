#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

#define NUM_PIXELS 25 // quantidade de LEds da matriz

// frames da animação no formato hexadecimal que representam cada um dos números de 0 a 9, específicos para a matriz 5x5 da placa de desenvolvimento BitLogLab
//                                           0          1          2          3          4          5          6          7          8          9
uint32_t counter_animation_frames[10] = {0x1FCCF3F, 0x03C0F03, 0x1FC7C7F, 0x1FC3F1F, 0x1BDFF03, 0x1F1FF1F, 0x1F1FF7F, 0x1FC0F03, 0x1FCFF3F, 0x1FCFF1F};

 double *apply_intensity_frame(uint32_t frame, double intensity)
{
    double *frames = (double *)calloc(1, sizeof(double));
    size_t counter = 0;

    for (size_t j = 0; j < NUM_PIXELS; j++)
    {
        // NÃO UTILIZAR EM PRODUÇÃO
        // printf("%b\t & \t%025b\t", frame, 0x1 << (NUM_PIXELS - j) - 1);
        int binary = (int)frame & (0x1 << ((NUM_PIXELS - j) - 1));
        // NÃO UTILIZAR EM PRODUÇÃO
        // printf("= \t%025b\n", binary);

        if (binary > 0)
        {
            frames[counter] = intensity;
        }
        else
        {
            frames[counter] = 0.0;
        }
        // NÃO UTILIZAR EM PRODUÇÃO
        // printf("%025b\t %.1f\n", binary, frames[counter]);
        counter++;
    }
    free(frames);
    return frames;
}

uint32_t rgb(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 16) |
           ((uint32_t)(g) << 24) |
           (uint32_t)(b << 8);
}

void apply_color_frame(double *number, PIO pio, uint sm, uint8_t r, uint8_t g, uint8_t b)
{
    for (size_t i = 0; i < NUM_PIXELS; ++i)
    {
        // printf("%.1f\n", number[(NUM_PIXELS-1) - i]);
        uint32_t color = rgb(number[(NUM_PIXELS - 1) - i] * r, number[(NUM_PIXELS - 1) - i] * g, number[(NUM_PIXELS - 1) - i] * b);
        pio_sm_put_blocking(pio, sm, color);
    }
}

void show_number(PIO pio, uint sm, uint8_t r, uint8_t g, uint8_t b, double intensity, size_t frame)
{
    // printf("%d %d %d %d %.1f %d\n", sm, r, g, b, intensity, frame);
    apply_color_frame(apply_intensity_frame(counter_animation_frames[frame], intensity), pio, sm, r, g, b);
}
