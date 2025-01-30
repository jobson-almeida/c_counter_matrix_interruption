#ifndef COUNTER
#define COUNTER

// função para configurar a intensidade de cada LED em cada frame
double *apply_intensity_frame(uint32_t frame, double intensity);

// exibir frame / representação de um número
void show_number(PIO pio, uint sm, uint8_t r, uint8_t g, uint8_t b, double intensity, size_t frame);

#endif