/*

  U32Plain.pde
  
  U8glib Example

  m2tklib = Mini Interative Interface Toolkit Library

  >>> Before compiling: Please remove comment from the constructor of the 
  >>> connected graphics display (see below).

  Copyright (C) 2012  olikraus@gmail.com

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  SCL (SPI Clock)   Pin 13
  SI (MOSI)         Pin 11
  CS (Chip Select)  Pin 10
  MISO (Pin 12) is not used, but can not be reused as generic I/O
  
*/

#include "U8glib.h"
#include "M2tk.h"
#include "utility/m2ghu8g.h"

// setup u8g object, please remove comment from one of the following constructor calls
// IMPORTANT NOTE: The complete list of supported devices is here: http://code.google.com/p/u8glib/wiki/device

//U8GLIB_NHD27OLED_BW u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD27OLED_2X_BW u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD27OLED_GR u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD27OLED_2X_GR u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD31OLED_BW u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD31OLED_2X_BW u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD31OLED_GR u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD31OLED_2X_GR u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGS102 u8g(13, 11, 10, 9);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGM132 u8g(13, 11, 10, 9);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGM128 u8g(13, 11, 10, 9);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_ST7920_128X64_1X u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_128X64_4X u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_128X64_1X u8g(18, 16, 17);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_ST7920_128X64_4X u8g(18, 16, 17);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_ST7920_192X32_1X u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_192X32_4X u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_192X32_1X u8g(18, 16, 17);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_ST7920_192X32_4X u8g(18, 16, 17);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_ST7920_192X32_1X u8g(13, 11, 10);	// SPI Com: SCK = en = 13, MOSI = rw = 11, CS = di = 10
//U8GLIB_ST7920_192X32_4X u8g(10);		// SPI Com: SCK = en = 13, MOSI = rw = 11, CS = di = 10, HW SPI
//U8GLIB_ST7920_202X32_1X u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_202X32_4X u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_202X32_1X u8g(18, 16, 17);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_ST7920_202X32_4X u8g(18, 16, 17);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_LM6059 u8g(13, 11, 10, 9);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_LM6063 u8g(13, 11, 10, 9);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGXL160_BW u8g(10, 9);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGXL160_GR u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGXL160_2X_BW u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGXL160_2X_GR u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_PCD8544 u8g(13, 11, 10, 9, 8);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, Reset = 8
//U8GLIB_PCF8812 u8g(13, 11, 10, 9, 8);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, Reset = 8
//U8GLIB_KS0108_128 u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 14, 15, 17, 16); 		// 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs1=14, cs2=15,di=17,rw=16
//U8GLIB_LC7981_160X80 u8g(8, 9, 10, 11, 4, 5, 6, 7,  18, 14, 15, 17, 16); 	// 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs=14 ,di=15,rw=17, reset = 16
//U8GLIB_LC7981_240X64 u8g(8, 9, 10, 11, 4, 5, 6, 7,  18, 14, 15, 17, 16); 	// 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs=14 ,di=15,rw=17, reset = 16
//U8GLIB_LC7981_240X128 u8g(8, 9, 10, 11, 4, 5, 6, 7,  18, 14, 15, 17, 16); 	// 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs=14 ,di=15,rw=17, reset = 16
//U8GLIB_ILI9325D_320x240 u8g(18,17,19,U8G_PIN_NONE,16 );  			// 8Bit Com: D0..D7: 0,1,2,3,4,5,6,7 en=wr=18, cs=17, rs=19, rd=U8G_PIN_NONE, reset = 16
//U8GLIB_SBN1661_122X32 u8g(8,9,10,11,4,5,6,7,14,15, 17, U8G_PIN_NONE, 16); 	// 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 cs1=14, cs2=15,di=17,rw=16,reset = 16
//U8GLIB_SSD1306_128X64 u8g(13, 11, 10, 9);	// SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_SSD1306_128X64 u8g(10, 9);		// HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);	// HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//U8GLIB_SSD1306_128X32 u8g(13, 11, 10, 9);	// SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_SSD1306_128X32 u8g(10, 9);             // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);	// HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//U8GLIB_SSD1309_128X64 u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_SSD1327_96X96_GR u8g(U8G_I2C_OPT_NONE);	// I2C
//U8GLIB_SSD1327_96X96_2X_GR u8g(U8G_I2C_OPT_NONE);	// I2C
//U8GLIB_NHD_C12864 u8g(13, 11, 10, 9, 8);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, RST = 8
//U8GLIB_NHD_C12832 u8g(13, 11, 10, 9, 8);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, RST = 8
//U8GLIB_T6963_240X128 u8g(8, 9, 10, 11, 4, 5, 6, 7, 14, 15, 17, 18, 16); // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7, cs=14, a0=15, wr=17, rd=18, reset=16
//U8GLIB_T6963_240X64 u8g(8, 9, 10, 11, 4, 5, 6, 7, 14, 15, 17, 18, 16); // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7, cs=14, a0=15, wr=17, rd=18, reset=16
//U8GLIB_T6963_128X64 u8g(8, 9, 10, 11, 4, 5, 6, 7, 14, 15, 17, 18, 16); // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7, cs=14, a0=15, wr=17, rd=18, reset=16


// DOGS102 shield 
// uint8_t uiKeyUpPin = 5;
// uint8_t uiKeyDownPin = 3;
// uint8_t uiKeySelectPin = 4;

// DOGM132, DOGM128 and DOGXL160 shield
uint8_t uiKeyUpPin = 7;         
uint8_t uiKeyDownPin = 3;
uint8_t uiKeySelectPin = 2;

// Edit the following long int number
uint32_t number = 1234;

// Definition of the m2tklib menu
M2_U32NUM(el_num, "a1c4", &number);

// M2tk init
M2tk m2(&el_num, m2_es_arduino, m2_eh_2bs, m2_gh_u8g_bfs);

// U8glib draw procedure: Just call the M2tklib draw procedure
void draw(void) {
    m2.draw();
}

// Arduino setup procedure (called only once)
void setup() {
  // Connect u8glib with m2tklib
  m2_SetU8g(u8g.getU8g(), m2_u8g_box_icon);

  // Assign u8g font to index 0
  m2.setFont(0, u8g_font_7x13);

  // Setup keys
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyDownPin);
}

// Arduino loop procedure
void loop() {
  m2.checkKey();
  if ( m2.handleKey() ) {
    u8g.firstPage();  
    do {
      draw();
    } while( u8g.nextPage() );
  }
}


