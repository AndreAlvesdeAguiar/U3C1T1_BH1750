#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

// Definições de pinos do I2C e velocidade
#define SSD1306_I2C_SDA 14
#define SSD1306_I2C_SCL 15
#define SSD1306_I2C_CLOCK 400  // Velocidade do I2C em kHz

// Protótipos de funções
void display_init(void);
void write_led(void);
void display_ip_address(uint8_t *ip_address);
void display_status(int status);
void display_status_msg(char status_msg[]) ;
void display_joystick(uint16_t vrx_value, uint16_t vry_value);

#endif // DISPLAY_H
