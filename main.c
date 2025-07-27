#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "display.h"
#include "bh1750.h"

#define I2C_PORT i2c0
#define SERVO_PIN 2

uint16_t angle_to_duty(float angle) {
    float pulse_ms = 0.5f + (angle / 180.0f) * 2.0f;
    return (uint16_t)((pulse_ms / 20.0f) * 20000.0f);
}

void servo_init(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(pin);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 125.0f);
    pwm_config_set_wrap(&config, 20000);
    pwm_init(slice, &config, true);
}

void servo_set_angle(uint pin, float angle) {
    pwm_set_gpio_level(pin, angle_to_duty(angle));
}

void girar_servo_completo(uint pin) {
    for (int angle = 0; angle <= 180; angle++) {
        servo_set_angle(pin, angle);
        sleep_ms(10);
    }
    for (int angle = 180; angle >= 0; angle--) {
        servo_set_angle(pin, angle);
        sleep_ms(10);
    }
}

int main() {
    stdio_init_all();
    i2c_inst_t *i2c = bh1750_init(I2C_PORT);
    display_init(); 
    servo_init(SERVO_PIN);

    float lux;
    char buffer[32];
    bool ja_girou = false;

    while (true) {
        bh1750_read_lux(i2c, &lux);
        sprintf(buffer, "Lum: %.2f lux", lux);
        display_status_msg(buffer);

        if (lux < 10.0f && !ja_girou) {
            girar_servo_completo(SERVO_PIN);
            ja_girou = true;
        }

        if (lux >= 10.0f && ja_girou) {
            ja_girou = false; // permite nova ativação quando a luz apagar de novo
        }

        sleep_ms(500);
    }

    return 0; 
}
