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

// GPIO do botão de seleção do joystick, definido para alterar a cor dos LEDs
#define BUTTON_C 22

// GPIOs dos LEDs 
#define LED_RED 13

volatile uint32_t last_time = 0; // variável auxiliar para deboucing
volatile int number_index = 0;   // index 0 corresponde ao número 0
volatile int color_index = 0;

PIO pio;
uint sm;
double intensity = 0.1;

// cores primárias no padrão RGB
RGBColor color[15] = {
    {255, 0, 0}, {255, 255, 255}, {0, 255, 0}, {0, 0, 255}, {255, 255, 0}, {0, 255, 255}, {255, 0, 255}, {192, 192, 192}, {128, 128, 128}, {128, 0, 0}, {128, 128, 0}, {0, 128, 0}, {128, 0, 128}, {0, 128, 128}, {0, 0, 128}};

static void counter_matrix_interruption_gpio_irq_handler(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    // verificar se passou tempo o bastante desde o último evento
    if (current_time - last_time > 250000) // 250 ms de debouncing
    {
        last_time = current_time; // atualiza o tempo do último evento

        if (gpio_get(BUTTON_A) == 0)
        {
            if (number_index < 9)
                number_index++;
            else
                number_index = 0;

            // printf("número %d\n", number_index);
            show_number(pio, sm, color[color_index].r, color[color_index].g, color[color_index].b, intensity, number_index);
        }
        if (gpio_get(BUTTON_B) == 0)
        {
            if (number_index > 0)
                number_index--;
            else
                number_index = 9;

            // printf("número %d\n", number_index);
            show_number(pio, sm, color[color_index].r, color[color_index].g, color[color_index].b, intensity, number_index);
        }
        if (gpio_get(BUTTON_C) == 0)
        {
            if (color_index < 14)
            {
                color_index++;
            }
            else
            {
                color_index = 0;
            }
            // printf("color %d\n", color_index);
            show_number(pio, sm, color[color_index].r, color[color_index].g, color[color_index].b, intensity, number_index);
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

    // habilitar as interrupções para exibir os frames que representam os números de 0-9
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &counter_matrix_interruption_gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &counter_matrix_interruption_gpio_irq_handler);

    // habilitar a interrupção para alterar a cor dos LEDs
    gpio_set_irq_enabled_with_callback(BUTTON_C, GPIO_IRQ_EDGE_FALL, true, &counter_matrix_interruption_gpio_irq_handler);

    // inicializar dos LEDs - GPIO 11, 12 e 13
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
 
    // inicializar o botão A - GPIO5
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    // inicializar o botão B - GPIO6
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    // inicializar o botão C - GPI22
    gpio_init(BUTTON_C);
    gpio_set_dir(BUTTON_C, GPIO_IN);
    gpio_pull_up(BUTTON_C);

    // exibir o primeiro número do vetor - 0
    show_number(pio, sm, color[color_index].r, color[color_index].g, color[color_index].b, intensity, 0);

    while (true)
    {
        gpio_put(LED_RED, !gpio_get(LED_RED)); 
        sleep_ms(200);
    }
}
