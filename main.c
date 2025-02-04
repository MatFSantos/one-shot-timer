#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#include "modules/led.h"
#include "modules/push_button.h"

bool routine_active = false;

void init_gpio(){
    // inicializa o LED RGB
    init_rgb_led();

    // inicializa o botão
    init_push_button(PIN_BUTTON_A);
}
int64_t turn_off_callback(alarm_id_t id, void *user_data) {
    uint8_t led = *(uint8_t *)user_data;
    led_manipulate(led, false);
    printf("led: %d\n", led);
    if (led == 11) routine_active = false;

    return 0;
}

int main() {
    stdio_init_all();
    init_gpio();

    uint8_t leds[] = {PIN_RED_LED,PIN_BLUE_LED,PIN_GREEN_LED}; // matrix of LEDs pin

    while(1) {
        // verifica se o botão foi pressionado, já com o debounce de 200 ms
        // verifica também se a rotina já não está em execução
        if (!push_button_get(PIN_BUTTON_A, true) && !routine_active) {
            rgb_led_manipulate(true, true, true); // acende os três leds
            routine_active = true;
            
            for (uint8_t i = 0; i < 3; i++) {
                add_alarm_in_ms(3000 * (i + 1), turn_off_callback, &leds[i], false);
            }
        }
        sleep_ms(10);
    }
}