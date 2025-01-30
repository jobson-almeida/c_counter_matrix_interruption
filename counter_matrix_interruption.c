#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "counter_matrix_interruption.pio.h"

// GPIO da matriz de LEDs
#define MATRIX 7

// GPIOs dos botões para incrementar e decrementar as representações numéricas (0-9) 
// na matriz de LEDs
#define BUTTON_A 5
#define BUTTON_B 6

// GPIOs dos LEDs
#define LED_RED 11
#define LED_GREEN 12
#define LED_BLUE 13

int main()
{
    stdio_init_all();

    PIO pio = pio0;
    uint offset = pio_add_program(pio, &counter_matrix_interruption_program);
    uint sm = pio_claim_unused_sm(pio, true);
    counter_matrix_interruption_program_init(pio, sm, offset, MATRIX);

// inicializar dos LEDs - GPIO 11, 12 e 13 ////////////////////////////
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);

    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);

    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);

    // inicializar o botão A - GPIO5  
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    // inicializar o botão B - GPIO6  
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B); 

    while (true)
    {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
