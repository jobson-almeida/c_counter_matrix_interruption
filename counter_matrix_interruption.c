#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "counter_matrix_interruption.pio.h"

// pino de saída
#define MATRIX 7

int main()
{
    stdio_init_all();

    PIO pio = pio0;
    uint offset = pio_add_program(pio, &counter_matrix_interruption_program);
    uint sm = pio_claim_unused_sm(pio, true);
    counter_matrix_interruption_program_init(pio, sm, offset, MATRIX);

    while (true)
    {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
