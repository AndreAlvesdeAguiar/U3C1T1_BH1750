#include "display.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include <string.h>
#include <stdio.h>

// Área de renderização do display
static struct render_area frame_area = {
    .start_column = 0,
    .end_column = ssd1306_width - 1,
    .start_page = 0,
    .end_page = ssd1306_n_pages - 1
};

// Inicialização do I2C e do display SSD1306
void display_init(void) {
    // Inicializa o I2C
    i2c_init(i2c1, SSD1306_I2C_CLOCK * 1000);
    gpio_set_function(SSD1306_I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(SSD1306_I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(SSD1306_I2C_SDA);
    gpio_pull_up(SSD1306_I2C_SCL);

    // Inicializa o display SSD1306
    ssd1306_init();

    // Define o tamanho do buffer de renderização
    calculate_render_area_buffer_length(&frame_area);
}

// Mostra "Conectando ao Wi-Fi..." no display
void write_led() {
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    char *text[] = {"Conectando", "ao", "Wi-Fi..."};
    int y = 0;
    for (uint i = 0; i < sizeof(text) / sizeof(text[0]); i++) {
        ssd1306_draw_string(ssd, 5, y, text[i]);
        y += 8;
    }
    render_on_display(ssd, &frame_area);
}

// Exibe o endereço IP no display
void display_ip_address(uint8_t *ip_address) {
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    char ip_text[20];
    snprintf(ip_text, sizeof(ip_text), "IP: %d.%d.%d.%d", 
             ip_address[0], ip_address[1], ip_address[2], ip_address[3]);

    ssd1306_draw_string(ssd, 5, 0, "Wi-Fi Conectado!");
    ssd1306_draw_string(ssd, 5, 10, ip_text);

    render_on_display(ssd, &frame_area);
}

// Exibe o status no display
void display_status(int status) {
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    char status_text[20];
    snprintf(status_text, sizeof(status_text), "Status: %d", status);

    ssd1306_draw_string(ssd, 5, 0, "MQTT Status:");
    ssd1306_draw_string(ssd, 5, 10, status_text);

    render_on_display(ssd, &frame_area);
}

// Exibe o status no display
void display_status_msg(char status_msg[]) {
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    char status_text[128];
    snprintf(status_text, sizeof(status_text), "%s", status_msg);

    
    ssd1306_draw_string(ssd, 5, 0, status_text);

    render_on_display(ssd, &frame_area);
}

void display_joystick(uint16_t vrx_value, uint16_t vry_value) {
    // Cria o buffer para o display
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    // Buffers para armazenar as strings formatadas
    char x_axis_text[20];
    char y_axis_text[20];

    // Formata as strings com os valores do joystick
    snprintf(x_axis_text, sizeof(x_axis_text), "%u °C", vrx_value);
    snprintf(y_axis_text, sizeof(y_axis_text), "%u %%", vry_value);

    // Desenha as strings no display
    ssd1306_draw_string(ssd, 5, 0, "Temperatura: ");  // Desenha na linha superior
    ssd1306_draw_string(ssd, 5, 10, x_axis_text); // Desenha na linha logo abaixo
    ssd1306_draw_string(ssd, 5, 20, "Umidade do Solo: ");  // Desenha na linha superior
    ssd1306_draw_string(ssd, 5, 30, y_axis_text);  // Desenha na linha superior

    // Atualiza o display
    render_on_display(ssd, &frame_area);
}
