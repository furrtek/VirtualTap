# How to program the CPLD

Download a version of Quartus which supports the Max V series, and buy (or build) a fake $30 Altera USB Blaster cable.
Connect the 6 JTAG pins on the board to the programmer, provide power to the board and write one of the `.pof` files.

# Creating custom palettes

The 8 default palettes can be modified to suit your needs by editing the code section following `// Palette LUT`.

Each line of the `case` statement defines the RGB values for each of the 4 colors of a given palette, top to bottom from 0 to 7.

The 24 bits represent the 2-bit values for each R, G and B component for each color index, left to right from brightest to darkest.

All combinations of colors are possible but the output is limited by the hardware to 2-bit per component, so the available colors are limited to the following table (which is coincidentally the Sega Master System palette):

![Virtualtap possible colors](colors.png)
