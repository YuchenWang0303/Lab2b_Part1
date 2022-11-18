# Lab2b_Part1: Yuchen Wang Worked with Katrina Ji

### Teammate: Yuchen Wang, Katrina Ji

---
# GIF of this part

![IMG_9685](https://user-images.githubusercontent.com/105755054/200071264-ddc996b9-6a6e-47c2-8de0-6e2bd2409831.gif)

---
# Modified Parts:

```
        boot_pin_address = (ADDRESS) 0xd0000004;                //This is an address
        full_gpio_register_value = (VALUE) *boot_pin_address;   //This is the value at the above address
        pin_21_selection_mask = 1u << 21;                       //pin 21 is the QTPY_BOOT_PIN --> unsigned 1
        selected_pin_state = full_gpio_register_value & pin_21_selection_mask;
        
        shifted_pin_21_state = selected_pin_state >> 21;        //can be used as a bool;
```

  In this section, instead of using the GPIO function in the SDK to read the BOOT PIN state, we use the BOOT PIN address to read its value directly. But we need to use a mask to get the specific bit value we want, and the BOOT PIN is in the 21st bit of that memory, so we'll use a 32-bit length mask with 20-bit of 1 to get the status of BOOT PIN.

---
# Explain:

<img width="573" alt="Screen Shot 2022-11-18 at 13 37 02" src="https://user-images.githubusercontent.com/105755054/202778368-df826a15-4a64-458c-a4b6-a3cd23e7eef3.png">

We know that each GPIO corresponds to a certain Bit of the corresponding address, so we know that if we can find this bit, we can know the status of the GPIO. Depending on the datasheet of rp2040, we know that the based address of SIO is 0xd0000000, and the GPIO offset value is 0x00000004, hence the address of GPIO is 0xd0000004. When we read 0xd0000004, we can get all GPIO status.
