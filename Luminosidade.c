#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "display.h"
#include "bh1750.h"

#define I2C_PORT i2c0

int main()
{
    stdio_init_all();
    
    i2c_inst_t *i2c = bh1750_init(i2c0);
    bh1750_init(i2c0);  
    display_init(); 
    float lux;
    char buffer[20];
   

    while (true) {
        bh1750_read_lux(i2c, &lux);
        sprintf(buffer, "Lum: %.2f lux", lux);
        display_status_msg(buffer);
        
        sleep_ms(1000);
    }
}
