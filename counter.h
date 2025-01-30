#ifndef COUNTER
#define COUNTER
 
#include "hardware/pio.h"

// função para configurar a intensidade de cada LED em cada frame
double *apply_intensity_frame(uint32_t frame, double intensity);

// função para aplicar cor aos leds conforme parâmetros definidos pelo usuário e a intensidade predefinida no frame
void apply_color_frame(double *desenho, PIO pio, uint sm, uint8_t r, uint8_t g, uint8_t b);

// exibir frame / representação de um número
void show_number(PIO pio, uint sm, uint8_t r, uint8_t g, uint8_t b, double intensity, size_t frame);

#endif