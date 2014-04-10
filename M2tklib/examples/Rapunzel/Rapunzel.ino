/*

  Rapunzel.pde

  Short story Rapunzel, taken from gutenberg.org
  Copyright expired at least for US and Germany.
  See www.gutenberg.org for more information.
  
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

*/

#include "U8glib.h"
#include "M2tk.h"
#include "utility/m2ghu8g.h"

// setup u8g object, please remove comment from one of the following constructor calls

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
U8GLIB_DOGM128 u8g(13, 11, 10, 9);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
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


// define pefered u8g font
#define FONT u8g_font_5x7r

// number of lines, must be a string.
// height= 64 --> 6 lines
#define LINES "6"


// DOGS102 shield 
// uint8_t uiKeyUpPin = 5;
// uint8_t uiKeyDownPin = 3;
// uint8_t uiKeySelectPin = 4;

// DOGM132, DOGM128 and DOGXL160 shield
uint8_t uiKeyUpPin = 7;         
uint8_t uiKeyDownPin = 3;
uint8_t uiKeySelectPin = 2;



char part1[] M2_SECTION_PROGMEM =
"RAPUNZEL\n"
"\n"
"There were once a man\n"
"and a woman who had long\n"
"in vain wished for a\n"
"child. At length the\n"
"woman hoped that God was\n"
"about to grant her\n"
"desire. These people had\n"
"a little window at the\n"
"back of their house from\n"
"which a splendid garden\n"
"could be seen, which was\n"
"full of the most\n"
"beautiful flowers and\n"
"herbs. It was, however,\n"
"surrounded by a high\n"
"wall, and no one dared\n"
"to go into it because it\n"
"belonged to an\n"
"enchantress, who had\n"
"great power and was\n"
"dreaded by all the\n"
"world. One day the woman\n"
"was standing by this\n"
"window and looking down\n"
"into the garden, when\n"
"she saw a bed which was\n"
"planted with the most\n"
"beautiful rampion\n"
"(rapunzel), and it\n"
"looked so fresh and\n"
"green that she longed\n"
"for it, she quite pined\n"
"away, and began to look\n"
"pale and miserable. Then\n"
"her husband was alarmed,\n"
"and asked: 'What ails\n"
"you, dear wife?' 'Ah,'\n"
"she replied, 'if I can't\n"
"eat some of the rampion,\n"
"which is in the garden\n"
"behind our house, I\n"
"shall die.' The man, who\n"
"loved her, thought:\n"
"'Sooner than let your\n"
"wife die, bring her some\n"
"of the rampion yourself,\n"
"let it cost what it\n"
"will.' At twilight, he\n"
"clambered down over the\n"
"wall into the garden of\n"
"the enchantress, hastily\n"
"clutched a handful of\n"
"rampion, and took it to\n"
"his wife. She at once\n"
"made herself a salad of\n"
"it, and ate it greedily.\n"
"It tasted so good to\n"
"her--so very good, that\n"
"the next day she longed\n"
"for it three times as\n"
"much as before. If he\n"
"was to have any rest,\n"
"her husband must once\n"
"more descend into the\n"
"garden. In the gloom of\n"
"evening therefore, he\n"
"let himself down again;\n"
"but when he had\n"
"clambered down the wall\n"
"he was terribly afraid,\n"
"for he saw the\n"
"enchantress standing\n"
"before him. 'How can you\n"
"dare,' said she with\n"
"angry look, 'descend\n"
"into my garden and steal\n"
"my rampion like a thief?\n"
"You shall suffer for\n"
"it!' 'Ah,' answered he,\n"
"'let mercy take the\n"
"place of justice, I only\n"
"made up my mind to do it\n"
"out of necessity. My\n"
"wife saw your rampion\n"
"from the window, and\n"
"felt such a longing for\n"
"it that she would have\n"
"died if she had not got\n"
"some to eat.' Then the\n"
"enchantress allowed her\n"
"anger to be softened,\n"
"and said to him: 'If the\n"
"case be as you say, I\n"
"will allow you to take\n"
"away with you as much\n"
"rampion as you will,\n"
"only I make one\n"
"condition, you must give\n"
"me the child which your\n"
"wife will bring into the\n"
"world; it shall be well\n"
"treated, and I will care\n"
"for it like a mother.'\n"
"The man in his terror\n"
"consented to everything,\n"
"and when the woman was\n"
"brought to bed, the\n"
"enchantress appeared at\n"
"once, gave the child the\n"
"name of Rapunzel, and\n"
"took it away with her.\n"
"\n"
"Rapunzel grew into the\n"
"most beautiful child\n"
"under the sun. When she\n"
"was twelve years old,\n"
"the enchantress shut her\n"
"into a tower, which lay\n"
"in a forest, and had\n"
"neither stairs nor door,\n"
"but quite at the top was\n"
"a little window. When\n"
"the enchantress wanted\n"
"to go in, she placed\n"
"herself beneath it and\n"
"cried:\n"
"\n"
" 'Rapunzel, Rapunzel,\n"
"  Let down your hair to\n"
"  me.'\n"
"\n"
"Rapunzel had magnificent\n"
"long hair, fine as spun\n"
"gold, and when she heard\n"
"the voice of the\n"
"enchantress she\n"
"unfastened her braided\n"
"tresses, wound them\n"
"round one of the hooks\n"
"of the window above, and\n"
"then the hair fell\n"
"twenty ells down, and\n"
"the enchantress climbed\n"
"up by it.\n"
"\n"
"After a year or two, it\n"
"came to pass that the\n"
"king's son rode through\n"
"the forest and passed by\n"
"the tower. Then he heard\n"
"a song, which was so\n"
"charming that he stood\n"
"still and listened. This\n"
"was Rapunzel, who in her\n"
"solitude passed her time\n"
"in letting her sweet\n"
"voice resound. The\n"
"king's son wanted to\n"
"climb up to her, and\n"
"looked for the door of\n"
"the tower, but none was\n"
"to be found. He rode\n"
"home, but the singing\n"
"had so deeply touched\n"
"his heart, that every\n"
"day he went out into the\n"
"forest and listened to\n"
"it. Once when he was\n"
"thus standing behind a\n"
"tree, he saw that an\n"
"enchantress came there,\n"
"and he heard how she\n"
"cried:\n"
"\n"
" 'Rapunzel, Rapunzel,\n"
"  Let down your hair to\n"
"  me.'\n"
;

char part2[] M2_SECTION_PROGMEM =
"Then Rapunzel let down\n"
"the braids of her hair,\n"
"and the enchantress\n"
"climbed up to her. 'If\n"
"that is the ladder by\n"
"which one mounts, I too\n"
"will try my fortune,'\n"
"said he, and the next\n"
"day when it began to\n"
"grow dark, he went to\n"
"the tower and cried:\n"
"\n"
" 'Rapunzel, Rapunzel,\n"
"  Let down your hair to\n"
"  me.'\n"
"\n"
"Immediately the hair\n"
"fell down and the king's\n"
"son climbed up.\n"
"\n"
"At first Rapunzel was\n"
"terribly frightened when\n"
"a man, such as her eyes\n"
"had never yet beheld,\n"
"came to her; but the\n"
"king's son began to talk\n"
"to her quite like a\n"
"friend, and told her\n"
"that his heart had been\n"
"so stirred that it had\n"
"let him have no rest,\n"
"and he had been forced\n"
"to see her. Then\n"
"Rapunzel lost her fear,\n"
"and when he asked her if\n"
"she would take him for\n"
"her husband, and she saw\n"
"that he was young and\n"
"handsome, she thought:\n"
"'He will love me more\n"
"than old Dame Gothel\n"
"does'; and she said yes,\n"
"and laid her hand in\n"
"his. She said: 'I will\n"
"willingly go away with\n"
"you, but I do not know\n"
"how to get down. Bring\n"
"with you a skein of silk\n"
"every time that you\n"
"come, and I will weave a\n"
"ladder with it, and when\n"
"that is ready I will\n"
"descend, and you will\n"
"take me on your horse.'\n"
"They agreed that until\n"
"that time he should come\n"
"to her every evening,\n"
"for the old woman came\n"
"by day. The enchantress\n"
"remarked nothing of\n"
"this, until once\n"
"Rapunzel said to her:\n"
"'Tell me, Dame Gothel,\n"
"how it happens that you\n"
"are so much heavier for\n"
"me to draw up than the\n"
"young king's son--he is\n"
"with me in a moment.'\n"
"'Ah! you wicked child,'\n"
"cried the enchantress.\n"
"'What do I hear you say!\n"
"I thought I had\n"
"separated you from all\n"
"the world, and yet you\n"
"have deceived me!' In\n"
"her anger she clutched\n"
"Rapunzel's beautiful\n"
"tresses, wrapped them\n"
"twice round her left\n"
"hand, seized a pair of\n"
"scissors with the right,\n"
"and snip, snap, they\n"
"were cut off, and the\n"
"lovely braids lay on the\n"
"ground. And she was so\n"
"pitiless that she took\n"
"poor Rapunzel into a\n"
"desert where she had to\n"
"live in great grief and\n"
"misery.\n"
"\n"
"On the same day that she\n"
"cast out Rapunzel,\n"
"however, the enchantress\n"
"fastened the braids of\n"
"hair, which she had cut\n"
"off, to the hook of the\n"
"window, and when the\n"
"king's son came and\n"
"cried:\n"
"\n"
" 'Rapunzel, Rapunzel,\n"
"  Let down your hair to\n"
"  me.'\n"
"\n"
"she let the hair down.\n"
"The king's son ascended,\n"
"but instead of finding\n"
"his dearest Rapunzel, he\n"
"found the enchantress,\n"
"who gazed at him with\n"
"wicked and venomous\n"
"looks. 'Aha!' she cried\n"
"mockingly, 'you would\n"
"fetch your dearest, but\n"
"the beautiful bird sits\n"
"no longer singing in the\n"
"nest; the cat has got\n"
"it, and will scratch out\n"
"your eyes as well.\n"
"Rapunzel is lost to you;\n"
"you will never see her\n"
"again.' The king's son\n"
"was beside himself with\n"
"pain, and in his despair\n"
"he leapt down from the\n"
"tower. He escaped with\n"
"his life, but the thorns\n"
"into which he fell\n"
"pierced his eyes. Then\n"
"he wandered quite blind\n"
"about the forest, ate\n"
"nothing but roots and\n"
"berries, and did naught\n"
"but lament and weep over\n"
"the loss of his dearest\n"
"wife. Thus he roamed\n"
"about in misery for some\n"
"years, and at length\n"
"came to the desert where\n"
"Rapunzel, with the twins\n"
"to which she had given\n"
"birth, a boy and a girl,\n"
"lived in wretchedness.\n"
"He heard a voice, and it\n"
"seemed so familiar to\n"
"him that he went towards\n"
"it, and when he\n"
"approached, Rapunzel\n"
"knew him and fell on his\n"
"neck and wept. Two of\n"
"her tears wetted his\n"
"eyes and they grew clear\n"
"again, and he could see\n"
"with them as before. He\n"
"led her to his kingdom\n"
"where he was joyfully\n"
"received, and they lived\n"
"for a long time\n"
"afterwards, happy and\n"
"contented.\n"
;

extern M2tk m2;
M2_EXTERN_ALIGN(el_top);

uint8_t total_lines1 = 0;
uint8_t first_visible_line1 = 0;

uint8_t total_lines2 = 0;
uint8_t first_visible_line2 = 0;

void goto_top_fn(m2_el_fnarg_p fnarg) {
  m2.setRoot(&el_top);
}

M2_INFOP(el_info1, "W60l" LINES, &first_visible_line1, &total_lines1, part1, goto_top_fn);
M2_VSB(el_vsb1, "W2r1l" LINES, &first_visible_line1, &total_lines1);
M2_LIST(el_list1) = { &el_info1, &el_vsb1};
M2_HLIST(el_part1, NULL, el_list1);

M2_INFOP(el_info2, "W60l" LINES, &first_visible_line2, &total_lines2, part2, goto_top_fn);
M2_VSB(el_vsb2, "W2r1l" LINES, &first_visible_line2, &total_lines2);
M2_LIST(el_list2) = { &el_info2, &el_vsb2};
M2_HLIST(el_part2, NULL, el_list2);

M2_LABEL(el_goto_title, NULL, "Rapunzel");
M2_ROOT(el_goto_part1, NULL, "Part 1", &el_part1);
M2_ROOT(el_goto_part2, NULL, "Part 2", &el_part2);
M2_LIST(list_menu) = {&el_goto_title, &el_goto_part1, &el_goto_part2};
M2_VLIST(el_menu_vlist, NULL, list_menu);
M2_ALIGN(el_top, "W64H64", &el_menu_vlist);
M2tk m2(&el_top, m2_es_arduino, m2_eh_4bs, m2_gh_u8g_fb); 



void draw(void) {
  m2.draw();
}

void setup(void) {
  // Connect u8glib with m2tklib
  m2_SetU8g(u8g.getU8g(), m2_u8g_box_icon);

  // Assign u8g font to index 0
  m2.setFont(0, FONT);

  // Setup keys
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_PREV, uiKeyUpPin);
  m2.setPin(M2_KEY_NEXT, uiKeyDownPin);
}

void loop() {
  m2.checkKey();
  if ( m2.handleKey() != 0 ) {
    u8g.firstPage();  
    do {
      draw();
    } while( u8g.nextPage() );
  }
}

