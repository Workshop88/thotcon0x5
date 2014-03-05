#include <Dogm.h>
extern const dog_pgm_uint8_t vendetta[];
extern const dog_pgm_uint8_t jedi[];

int a0Pin = PIN_A0_DEFAULT;     // address line a0 for the dogm module
Dogm dogm(a0Pin);  // initialize the dogm library

void setup() {
}

void loop() {
  dogm.start();
  do{
    
    dogm.setFont(jedi);
    dogm.setHLine(0,102,65);
    dogm.setXY(5,50);
    dogm.drawStr("THOTCON 0X5");
    dogm.setXY(5,40); 
    dogm.drawStr("THE QUICK BROWN ");
    dogm.setXY(5,30);
    dogm.drawStr("FOX JUMPS OVER ");
    dogm.setXY(5,20);
    dogm.drawStr("THE LAZY DOG ");
    dogm.setXY(5,10);
    dogm.drawStr("1234567890");
    dogm.setXY(5,0);
    dogm.drawStr("STAR WARS");  
  } while( dogm.next() );
  delay(100);
}
