# Note 3 as graphics tablet

Use your smartphone (or tablet) with stylus as a graphics tablet and/or as a touchpad on Linux. 
Currently supports only Samsung Galaxy Note 3, but with a few tweaks can work for any device (requires a minimum knowledge of `libinput`). 

### Usage

Install `adb`. 
Clone the repository and simply compile with `gcc yei2.c`. 
Connect your device via USB and ensure that is visible by `adb`. 

### Customizations

Edit the following global variables at the top of the source file to customize the behaviour. 
- `landscape`: set to use your device in landscape mode
- `touchpad_enabled`: set to use your finger as if the screen was a touchpad
- `pen_enabled`: set to use the stylus as a graphics tablet pen
- `adb_command`: the command used to get the input data from the device; you can customize it pass specific options to `adb` (e.g. if you want to connect your device via Wi-Fi)
