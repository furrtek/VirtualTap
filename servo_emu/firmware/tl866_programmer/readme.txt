I could not flash the hex with success (if flashed but not worked) with an arduino mega 2560 as ISP and a TL866 II plus.

So, after a couple hours, i created this hex by recompiling @furrtek's original C file with avr studio 4.19 (had to patch the msys-1.0.dll file in WinAVR-20100110 to make it work and manually configure the directories for the avr.gcc and make files).

Please follow the instructions in the servo_emu.c file regarding fuses when you are flashing with xgpro.
