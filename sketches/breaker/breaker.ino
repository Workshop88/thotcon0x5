#include "U8glib.h"


U8GLIB_DOGS102 u8g(17,11); //leonardo
//U8GLIB_DOGS102 u8g(0,4);  // teensy


void draw() {
  
}

void setup() {
  
  // flip screen, if required
   u8g.setRot180();
  
  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);
  
  // assign default color value
  
  u8g.setColorIndex(1);         // pixel on
}

void loop() {
  update();
  
  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
}


void update() {
  
}
