# SMD counter

## About project
The device is designed to count the number of radio components in the SMD tape. The counting is based on an optical principle using an optocoupler.
The project was developed as a training project for working with STM32F103 microcontroller peripherals. The device uses the basic peripherals such as: GPIO, interrupts,    timers, I2C, ADC, etc. I also used the state machine pattern. The firmware is written with CMSIS. The circuitry of the device and the PCB were also developed. The case was modeled and 3D printed.

  
  
  <img src="https://github.com/Ivanchenko59/SMD_counter/blob/master/images/3d_case.png" width = "80%">
 
  
## Demo Video

Brief demonstration of the device menu
[![Watch the video](https://img.youtube.com/vi/vhZqCzfXYpA/maxresdefault.jpg)](https://youtu.be/vhZqCzfXYpA)

Detailed video with a demonstration of work
[![Watch the video](https://img.youtube.com/vi/8hMTs-ckCMA/maxresdefault.jpg)](https://youtu.be/8hMTs-ckCMA)



## Hardware
<img src="https://github.com/Ivanchenko59/SMD_counter/blob/master/images/3d_pcb_preview.png" width = "80%">

To display information, used 0.96-inch OLED display with an I2C interface. Menu control is carried out using three buttons. The board also has a CP2102 UART converter for communicating with a computer, as well as an RFID module for reading tags, for organizing a warehouse and tracking the presence of components (not implemented at the moment).

<img src="https://github.com/Ivanchenko59/SMD_counter/blob/master/pcb%26schematic/2d_pcb_preview.png" width = "40%"> <img src="https://github.com/Ivanchenko59/SMD_counter/blob/master/images/pcb_2.jpg" width = "40%">

The device is powered by two 1.5V AA batteries. To increase the voltage to 3.3V used a step-up DC-DC converter based on the NCP1402 chip.

### Detailed circuit schematic

<img src="https://github.com/Ivanchenko59/SMD_counter/blob/master/pcb%26schematic/Schematic.jpg" width = "100%">

## Menu organization

After turning on the device, the animation of the running SMD tape will be played on the screen.

<img src="https://github.com/Ivanchenko59/SMD_counter/blob/master/images/start_anim.jpg" width = "50%">

After that, the user enters the main menu, from which you can enter the settings menu by holding the OK button, or start reading with a single press. On the screen there is a battery level, as well as a step size (for example, if two leads on the tape correspond to one component, the step is set to 2).

<img src="https://github.com/Ivanchenko59/SMD_counter/blob/master/images/main_menu.jpg" width = "50%">

In order to count the number of SMD elements in the tape, press the OK button, and pass the tape through the guide slot in the housing. On the screen you will see the current number of counted items in the tape.

<img src="https://github.com/Ivanchenko59/SMD_counter/blob/master/images/count_menu.jpg" width = "50%">

After the end of counting, and long holding the OK button, you will be taken to the save menu, where you can save the current value, change it, or continue counting.

<img src="https://github.com/Ivanchenko59/SMD_counter/blob/master/images/save_menu.jpg" width = "50%">

Edit menu for changing the current value looks like this:

<img src="https://github.com/Ivanchenko59/SMD_counter/blob/master/images/edit_save_menu.jpg" width = "50%">

After saving, the user returns to the main menu, and the calculated value is transferred to the database, which is accessed through the settings.
The settings menu looks like this:

<img src="https://github.com/Ivanchenko59/SMD_counter/blob/master/images/settings_menu.jpg" width = "50%">

