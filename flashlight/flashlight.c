#include "pico/stdlib.h"
#include <stdio.h>
#include "neopixel.h"
#include "hardware/gpio.h"
#include "register.h"

#define QTPY_BOOT_PIN 21

// Function as a mask to change the register
ADDRESS boot_pin_address;
VALUE full_gpio_register_value;
VALUE pin_21_selection_mask;
VALUE selected_pin_state;
VALUE shifted_pin_21_state;

// sio base is 0xd0000000
// gpio input offset is 0x00000004
// full register address is 0xd0000004

typedef struct {
    uint32_t last_serial_byte;
    uint32_t button_is_pressed;
    uint32_t light_color;
} Flashlight; 

void render_to_console(Flashlight status) {
    // adjust console window height to match 'frame size'
    for (uint8_t i=0; i<10; i++) { 
        printf("\n");
    }
    printf("button_is_pressed:  0x%08x\n",   status.button_is_pressed);
    printf("light_color:        0x%08x\n",   status.light_color);
}  

int main() {

    stdio_init_all();
    gpio_init(QTPY_BOOT_PIN);
    gpio_set_dir(QTPY_BOOT_PIN, GPIO_IN);
    neopixel_init();

    Flashlight status;
    status.last_serial_byte =  0x00000000;
    status.button_is_pressed = 0x00000000;
    status.light_color =       0x000000ff;

    while (true) {
        status.last_serial_byte = getchar_timeout_us(0); // don't block main loop
        switch(status.last_serial_byte) { // poll every cycle
            case 'r':
                status.light_color = 0x00ff0000;
                break;
            case 'g':
                status.light_color = 0x0000ff00;
                break;
            case 'b':
                status.light_color = 0x000000ff;
                break;
            case 'w':
                status.light_color = 0x00ffffff;
                break;
        }

        boot_pin_address = (ADDRESS) 0xd0000004;                //This is an address
        full_gpio_register_value = (VALUE) *boot_pin_address;   //This is the value at the above address
        pin_21_selection_mask = 1u << 21;                       //pin 21 is the QTPY_BOOT_PIN --> unsigned 1
        selected_pin_state = full_gpio_register_value & pin_21_selection_mask;
        
        shifted_pin_21_state = selected_pin_state >> 21;        //can be used as a bool;
        if (shifted_pin_21_state){    
            status.button_is_pressed = 0x00000000;
            
        } else {
            status.button_is_pressed = 0x00000001;
        }
        if (status.button_is_pressed) { // poll every cycle
            neopixel_set_rgb(status.light_color);

        } else {
            neopixel_set_rgb(0x00000000);
        }
        render_to_console(status);
        sleep_ms(10); // don't DDOS the serial console
    }
    return 0;
}                  
