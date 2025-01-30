#include "counter.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "counter_matrix_interruption.pio.h"

#include <string.h>
#include <stdlib.h>

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

volatile uint32_t last_time = 0; // variável auxiliar para deboucing
volatile int counter = 0;

PIO pio;
uint sm;
uint8_t r = 0, g = 0, b = 255;
double intensity = 0.1;

static void counter_matrix_interruption_gpio_irq_handler(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    // verifica se passou tempo o bastante desde o último evento
    if (current_time - last_time > 250000) // 250 ms de debouncing
    {
        last_time = current_time; // atualiza o tempo do último evento

        if (gpio_get(BUTTON_A) == 0)
        {
            if (counter < 9)
                counter++;
            else
                counter = 0;

            // printf("número %d\n", counter);
            show_number(pio, sm, r, g, b, intensity, counter);
        }
        if (gpio_get(BUTTON_B) == 0)
        {
            if (counter > 0)
                counter--;
            else
                counter = 9;

            // printf("número %d\n", counter);
            show_number(pio, sm, r, g, b, intensity, counter);
        }
    }
    gpio_acknowledge_irq(gpio, events); // limpa a interrupção
}

int main()
{
    stdio_init_all();

    pio = pio0;
    uint offset = pio_add_program(pio, &counter_matrix_interruption_program);

    // uint
    sm = pio_claim_unused_sm(pio, true);
    counter_matrix_interruption_program_init(pio, sm, offset, MATRIX);

    // interrupções para exibir os frames que representam os números de 0-9
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &counter_matrix_interruption_gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &counter_matrix_interruption_gpio_irq_handler);

    // inicializar dos LEDs - GPIO 11, 12 e 13
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

    show_number(pio, sm, r, g, b, intensity, 0);

    while (true)
    {
        gpio_put(LED_RED, !gpio_get(LED_RED));
        gpio_put(LED_GREEN, !gpio_get(LED_GREEN));
        gpio_put(LED_BLUE, !gpio_get(LED_BLUE));
        sleep_ms(200);
    }
}
