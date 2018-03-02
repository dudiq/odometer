
Arduino odometer
=

This is simple project for write own odometer for enduro motorcycle with hall sensor and ssd1306 i2c display screen (128x32)

> Screens are switching by long hold of center button (500ms+)

## Screens

### "Main" screcen
- `current distance`. Can be **dropped** by `center` button click
- `local distance`. Can be **corrected** by `left/right` buttons

### "One km" screen
Define how much `ticks` in one kilometer (for set correct length of wheel). Special mode for riders. Need to set zero value before start `conditional kilometer` stay this screen and ride. When kilometer ends, click to `center` button to define correct value and switch to next screen.
>for set new value, use `center` button. `left/rigth` - for correction. `center long hold` - next screen

### "Diam, mm" screen
Define `diameter` of wheel in millimeters.
- top number - is `new` value
- bottom number - is `old` value
>for set new value, use `center` button. `left/rigth` - for correction. `center long hold` - next screen

### "Len, mm" screen
Define `length` of wheel in millimeters.
- top number - is `new` value
- bottom number - is `old` value
>for set new value, use `center` button. `left/rigth` - for correction. `center long hold` - next screen

### "Info" screen
Shows info about `Engine`, `Oil`, `Gear` counter.

- `left/right` - for switch between of counter
- `center` - for drop current counter

### Equipment:

- Arduino nano (atmega328p)
- Hall sensor (ky003)
- I2C display based on ssd1306 chip (0.91` 128x32 I2C OLED)
- Fram FM24CL16
- wires
- 3 buttons
- 3 resistor (10kOm)
- and any type of power for arduino only

MIT idudiq 2018