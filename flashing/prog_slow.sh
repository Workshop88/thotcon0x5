avrdude -P comport -b 19200 -c avrisp -p m32u4 -v -e -U lfuse:w:0xff:m -U hfuse:w:0xd8:m -U efuse:w:0xcd:m
avrdude -P comport -b 19200 -c avrisp -p m32u4 -v -e -U flash:w:Thotcon0x5.hex -U flash:w:oper.hex -U lock:w:0x2f:m
