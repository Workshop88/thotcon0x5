#include "U8glib.h"


//#define DEBUG 1
#define DISPLAY_WIDTH 102
#define L_FONT_HEIGHT 10
#define L_FONT_LENGTH 17 //printable characters per line


//Key States
#define KEY_NONE 0
#define KEY_PREV 1
#define KEY_NEXT 2
#define KEY_SELECT 3
#define KEY_BACK 4

//Menu SM States
#define MAIN_STATE 255
#define GAME_STATE 0
#define SCHED_STATE 1
#define LINK_STATE 2
#define REG_STATE 3
#define HELP_STATE 4
#define TRACK_ONE_STATE (SCHED_STATE << 4)
#define TRACK_TWO_STATE (SCHED_STATE << 5)
#define LABS_STATE  (SCHED_STATE << 6)
#define VILLAGE_STATE (SCHED_STATE << 7)
#define PARTY_STATE 5

//menu global strings and consts for draw states and such
#define MENU_ITEMS 5
#define TRACK1_ITEMS 16
#define TRACK2_ITEMS 13
#define LAB_ITEMS 7
#define VILLAGE_ITEMS 7

U8GLIB_DOGS102 u8g(0,4); //teesy 
uint8_t uiKeyPrev = 17;
uint8_t uiKeyNext = 18;
uint8_t uiKeySelect = 14;
uint8_t uiKeyBack = 15;
uint8_t uiKeyRight = 19;
uint8_t uiKeyLeft = 20;


/* U8GLIB_DOGS102 u8g(17,11); //PROTO BOARD
//Leonardo based pin assignements must have updated pins_arduino.h for pin 30
uint8_t uiKeyPrev = 4;
uint8_t uiKeyNext = 0;
uint8_t uiKeySelect = 12;
uint8_t uiKeyBack = 6;
uint8_t uiKeyRight = 1;
uint8_t uiKeyLeft = 30;
*/


uint8_t uiKeyCodeFirst = KEY_NONE;
uint8_t uiKeyCodeSecond = KEY_NONE;
uint8_t uiKeyCode = KEY_NONE;

void uiSetup(void) {
  // initialize input keys , look to optimize w/ straight reg manipulation
  
  pinMode(uiKeyPrev, INPUT);           // set pin to input
  digitalWrite(uiKeyPrev, HIGH);       // turn on pullup resistors
  pinMode(uiKeyNext, INPUT);           // set pin to input
  digitalWrite(uiKeyNext, HIGH);       // turn on pullup resistors
  pinMode(uiKeySelect, INPUT);           // set pin to input
  digitalWrite(uiKeySelect, HIGH);       // turn on pullup resistors
  pinMode(uiKeyBack, INPUT);           // set pin to input
  digitalWrite(uiKeyBack, HIGH);       // turn on pullup resistors
}


void uiStep(void) { // optimize out digitalRead
  uiKeyCodeSecond = uiKeyCodeFirst;
  if ( digitalRead(uiKeyPrev) == LOW )
    uiKeyCodeFirst = KEY_PREV;
  else if ( digitalRead(uiKeyNext) == LOW )
    uiKeyCodeFirst = KEY_NEXT;
  else if ( digitalRead(uiKeySelect) == LOW )
    uiKeyCodeFirst = KEY_SELECT;
  else if ( digitalRead(uiKeyBack) == LOW )
    uiKeyCodeFirst = KEY_BACK;
  else 
    uiKeyCodeFirst = KEY_NONE;
  
  if ( uiKeyCodeSecond == uiKeyCodeFirst )
    uiKeyCode = uiKeyCodeFirst;
  else
    uiKeyCode = KEY_NONE;
}



const char mm1_str[] PROGMEM = "Game";
const char mm2_str[] PROGMEM = "Schedule";
const char mm3_str[] PROGMEM = "GameLink";
const char mm4_str[] PROGMEM = "Register";
const char mm5_str[] PROGMEM = "Help";
const char *main_menu_str[MENU_ITEMS] PROGMEM = {mm1_str ,mm2_str ,mm3_str ,mm4_str ,mm5_str};
const char sm1_str[] PROGMEM = "Track One";
const char sm2_str[] PROGMEM = "Track Two";
const char sm3_str[] PROGMEM = "Labs";
const char sm4_str[] PROGMEM = "Village";
const char sm5_str[] PROGMEM = "Party";
const char *sched_menu_str[MENU_ITEMS] PROGMEM  = {sm1_str, sm2_str,sm3_str ,sm4_str ,sm5_str};
const char _1t1t_str[] PROGMEM = "0830";
const char _1t2t_str[] PROGMEM = "0945";
const char _1t3t_str[] PROGMEM = "1000";
const char _1t4t_str[] PROGMEM = "1100";
const char _1t5t_str[] PROGMEM = "1200";
const char _1t6t_str[] PROGMEM = "1300";
const char _1t7t_str[] PROGMEM = "1400";
const char _1t8t_str[] PROGMEM = "1500";
const char _1t9t_str[] PROGMEM = "1600";
const char _1t10t_str[] PROGMEM = "1700";
const char _1t11t_str[] PROGMEM = "1730";
const char _1t12t_str[] PROGMEM = "1830";
const char _1t13t_str[] PROGMEM = "1930";
const char _1t14t_str[] PROGMEM = "2030";
const char _1t15t_str[] PROGMEM = "2100";
const char _1t16t_str[] PROGMEM = "2120";
const char *track1_time_str[TRACK1_ITEMS] PROGMEM = {_1t1t_str,_1t2t_str,_1t3t_str,_1t4t_str,_1t5t_str,_1t6t_str,_1t7t_str,_1t8t_str,_1t9t_str,_1t10t_str,_1t11t_str,_1t12t_str,_1t13t_str,_1t14t_str,_1t15t_str,_1t16t_str};  
const char _2t1t_str[] PROGMEM = "1100";
const char _2t2t_str[] PROGMEM = "1130";
const char _2t3t_str[] PROGMEM = "1200";
const char _2t4t_str[] PROGMEM = "1230";
const char _2t5t_str[] PROGMEM = "1300";
const char _2t6t_str[] PROGMEM = "1430";
const char _2t7t_str[] PROGMEM = "1500";
const char _2t8t_str[] PROGMEM = "1530";
const char _2t9t_str[] PROGMEM = "1600";
const char _2t10t_str[] PROGMEM = "1630";
const char _2t11t_str[] PROGMEM = "1700";
const char _2t12t_str[] PROGMEM = "1730";
const char _2t13t_str[] PROGMEM = "1800";
const char *track2_time_str[TRACK2_ITEMS] PROGMEM = {_2t1t_str,_2t2t_str,_2t3t_str,_2t4t_str,_2t5t_str,_2t6t_str,_2t7t_str,_2t8t_str,_2t9t_str,_2t10t_str,_2t11t_str,_2t12t_str,_2t13t_str};  
const char lt3_str[] PROGMEM = "1100";
const char lt4_str[] PROGMEM = "1200";
const char lt5_str[] PROGMEM = "1300";
const char lt6_str[] PROGMEM = "1400";
const char lt7_str[] PROGMEM = "1600";
const char lt8_str[] PROGMEM = "1700";
const char lt9_str[] PROGMEM = "1800";
const char *lab_time_str[LAB_ITEMS] PROGMEM = {lt3_str,lt4_str,lt5_str,lt6_str,lt7_str,lt8_str,lt9_str};
const char _1t1tl_str[] PROGMEM = "DOORS";
const char _1t2tl_str[] PROGMEM = "HELLO";
const char _1t3tl_str[] PROGMEM = "UFOs\n and \nGovernment";
const char _1t4tl_str[] PROGMEM = "Crowdsourcing\nYour Cisco\nFirewalls";
const char _1t5tl_str[] PROGMEM = "Just What\nThe Doctor\nOrdered?";
const char _1t6tl_str[] PROGMEM = "BREAK";
const char _1t7tl_str[] PROGMEM = "The Perfectly\nLetimate Project";
const char _1t8tl_str[] PROGMEM = "It Ain't\nRocket Science";
const char _1t9tl_str[] PROGMEM = "Introducing idb";
const char _1t10tl_str[] PROGMEM = "BREAK";
const char _1t11tl_str[] PROGMEM = "Surveilling IP\nCera Security";
const char _1t12tl_str[] PROGMEM = "Phishing Frenzy";
const char _1t13tl_str[] PROGMEM = "Breaking Bus\nTickets";
const char _1t14tl_str[] PROGMEM = "HACKER BREW";
const char _1t15tl_str[] PROGMEM = "CLOSING";
const char _1t16tl_str[] PROGMEM = "";
const char *track1_title_str[TRACK1_ITEMS] PROGMEM = {_1t1tl_str,_1t2tl_str,_1t3tl_str,_1t4tl_str,_1t5tl_str,_1t6tl_str,_1t7tl_str,_1t8tl_str,_1t9tl_str,_1t10tl_str,_1t11tl_str,_1t12tl_str,_1t13tl_str,_1t14tl_str,_1t15tl_str,_1t16tl_str};
const char _1t1n_str[] PROGMEM = "OPEN";
const char _1t2n_str[] PROGMEM = "c7five";
const char _1t3n_str[] PROGMEM = "Richard Thieme";
const char _1t4n_str[] PROGMEM = "Claudius && Guay";
const char _1t5n_str[] PROGMEM = "Scott Erven";
const char _1t6n_str[] PROGMEM = "DRINK BEER \n EAT FOODS";
const char _1t7n_str[] PROGMEM = "O'Connor && Dobbe";
const char _1t8n_str[] PROGMEM = "David Mortman";
const char _1t9n_str[] PROGMEM = "Daniel A. Mayer";
const char _1t10n_str[] PROGMEM = "DRINK MORE BEER \n EAT MORE FOOD";
const char _1t11n_str[] PROGMEM = "Mark Stanislov";
const char _1t12n_str[] PROGMEM = "McCann \n Ringwood";
const char _1t13n_str[] PROGMEM = "Mike Jackson";
const char _1t14n_str[] PROGMEM = "CONTEST RESULTS";
const char _1t15n_str[] PROGMEM = "REMARKS";
const char _1t16n_str[] PROGMEM = "SYS64738";
const char *track1_name_str[TRACK1_ITEMS] PROGMEM = {_1t1n_str,_1t2n_str,_1t3n_str,_1t4n_str,_1t5n_str,_1t6n_str,_1t7n_str,_1t8n_str,_1t9n_str,_1t10n_str,_1t11n_str,_1t12n_str,_1t13n_str,_1t14n_str,_1t15n_str,_1t16n_str};
const char _2t1tl_str[] PROGMEM = "Digitial Attacks\nin the\nPhysical Realm";
const char _2t2tl_str[] PROGMEM = "Wireless\nDrone Strikes";
const char _2t3tl_str[] PROGMEM = "Poking Through\nto the Heart\nof a Binary";
const char _2t4tl_str[] PROGMEM = "Retroactive\nWiretapping";
const char _2t5tl_str[] PROGMEM = "BREAK";
const char _2t6tl_str[] PROGMEM = "Your Personal\nInformation\nIs Online?";
const char _2t7tl_str[] PROGMEM = "Electronic\nDevice Searches\nat the Border";
const char _2t8tl_str[] PROGMEM = "P.I.S.S.E.D";
const char _2t9tl_str[] PROGMEM = "BREAK";
const char _2t10tl_str[] PROGMEM = "Traits Between\nA Soc Eng\n& a Sociopath";
const char _2t11tl_str[] PROGMEM = "10 Commandments\nof IR\n(For Hackers)";
const char _2t12tl_str[] PROGMEM = "Yelling at\nmanagement...";
const char _2t13tl_str[] PROGMEM = "Bypassing\n  EMET 4.1";
const char *track2_title_str[TRACK2_ITEMS] PROGMEM = {_2t1tl_str,_2t2tl_str,_2t3tl_str,_2t4tl_str,_2t5tl_str,_2t6tl_str,_2t7tl_str,_2t8tl_str,_2t9tl_str,_2t10tl_str,_2t11tl_str,_2t12tl_str,_2t13tl_str};
const char _2t1n_str[] PROGMEM = "Grape Ape";
const char _2t2n_str[] PROGMEM = "Parker Schmitt";
const char _2t3n_str[] PROGMEM = "whistlepig";
const char _2t4n_str[] PROGMEM = "John Bbenek";
const char _2t5n_str[] PROGMEM = "DRINK BEER \n EAT FOODS";
const char _2t6n_str[] PROGMEM = "whitehat1969";
const char _2t7n_str[] PROGMEM = "Merrill";
const char _2t8n_str[] PROGMEM = "Joe Cicero";
const char _2t9n_str[] PROGMEM = "DRINK MORE BEER \n EAT MORE FOODS";
const char _2t10n_str[] PROGMEM = "J. Singleton";
const char _2t11n_str[] PROGMEM = "Lesley Carhart";
const char _2t12n_str[] PROGMEM = "Alex Muentz";
const char _2t13n_str[] PROGMEM = "Jared DeMott";
const char *track2_name_str[TRACK2_ITEMS] PROGMEM = {_2t1n_str,_2t2n_str,_2t3n_str,_2t4n_str,_2t5n_str,_2t6n_str,_2t7n_str,_2t8n_str,_2t9n_str,_2t10n_str,_2t11n_str,_2t12n_str,_2t13n_str};
const char lab2tl_str[] PROGMEM = "Breaking\nEncryption\nw/ Oscilloscopes";
const char lab3tl_str[] PROGMEM = "Building a GPU\nCracking Rig";
const char lab4tl_str[] PROGMEM = "Synthetic Mobile\nMalware Analysis";
const char lab6tl_str[] PROGMEM = "The Cavalry is Us";
const char lab8tl_str[] PROGMEM = "Hardware\nHacking for\nCheap Dummies";
const char lab9tl_str[] PROGMEM = "Targeted Malware";
const char lab10tl_str[] PROGMEM = "";
const char *lab_title_str[LAB_ITEMS] PROGMEM = {lab2tl_str,lab3tl_str,lab4tl_str,lab6tl_str,lab8tl_str,lab9tl_str,lab10tl_str};
const char lab2n_str[] PROGMEM = "Duehr && Balducci";
const char lab3n_str[] PROGMEM = "Fosaaen && Gruber";
const char lab4n_str[] PROGMEM = "David Shaw";
const char lab6n_str[] PROGMEM = "Brand && Erven \n Corman";
const char lab8n_str[] PROGMEM = "Kevin Bong";
const char lab9n_str[] PROGMEM = "wartortell \n fuzzynop";
const char lab10n_str[] PROGMEM = "./SHUTDOWN";
const char *lab_name_str[LAB_ITEMS] PROGMEM = {lab2n_str,lab3n_str,lab4n_str,lab6n_str,lab8n_str,lab9n_str,lab10n_str};
const char vt1_str[] PROGMEM = "Table 0";
const char vt2_str[] PROGMEM = "Table 1";
const char vt3_str[] PROGMEM = "Table 2";
const char vt4_str[] PROGMEM = "Table 3";
const char vt5_str[] PROGMEM = "Table 4";
const char vt6_str[] PROGMEM = "Table 5";
const char vt7_str[] PROGMEM = "Table 6";
const char *village_time_str[VILLAGE_ITEMS] PROGMEM = {vt1_str,vt2_str,vt3_str,vt4_str,vt5_str,vt6_str,vt7_str};   
const char v1tl_str[] PROGMEM = "Workshop 88" ;
const char v2tl_str[] PROGMEM = "Hackers\n for\n Charity ";
const char v3tl_str[] PROGMEM = "Lock Pick \nVillage";
const char v4tl_str[] PROGMEM = "HACKER BREW\nCONTEST";
const char v5tl_str[] PROGMEM = "OWASP";
const char v6tl_str[] PROGMEM = "Circle City Con";
const char v7tl_str[] PROGMEM = "The Cavalry";
const char *village_title_str[VILLAGE_ITEMS] PROGMEM = {v1tl_str,v2tl_str,v3tl_str,v4tl_str,v5tl_str,v6tl_str,v7tl_str};
 
uint8_t menu_current = 0;
uint8_t menu_redraw = 0;
uint8_t info_current = 0;
uint8_t last_key_code = KEY_NONE;
uint8_t global_state = MAIN_STATE;

void drawMenu(const char **str) {
  uint8_t i;
  u8g_uint_t d;
  
  u8g.firstPage();
  do  {
    u8g.setFont(u8g_font_6x12r);
    u8g.setFontRefHeightText();
    u8g.setFontPosTop();

    for( i = 0; i < MENU_ITEMS; i++ ) {
      d = (DISPLAY_WIDTH-u8g.getStrWidthP((u8g_uint_t*)pgm_read_word(&(str[i]))))/2;
      u8g.setDefaultForegroundColor();
      if ( i == menu_current ) {
        u8g.drawBox(0, i*L_FONT_HEIGHT+1, DISPLAY_WIDTH, L_FONT_HEIGHT);
        u8g.setDefaultBackgroundColor();
      }
      u8g.drawStrP(d, i*L_FONT_HEIGHT,(const u8g_uint_t*)pgm_read_word(&str[i]));  
    }
  } while( u8g.nextPage() );
}

void drawMenuInfo(){
  uint8_t track; //= ((global_state >>4) & 1) ? 0: ((global_state >>5)&1) ? 1: ((global_state >>6)&1) ? 2 : 3;
  const char** time_str;
  const char** title_str;
  const char** name_str;
  byte pos= 0;
  uint16_t ptr= 0;
  char trimmed[L_FONT_LENGTH] = {'\0'};
  u8g_uint_t d=0;
  byte line = 2;
  
  switch (global_state){ //set pointers based on track
      case TRACK_ONE_STATE:
         track = 0;
         time_str = track1_time_str;
         title_str = track1_title_str;
         name_str = track1_name_str;
         break;
      case TRACK_TWO_STATE:
         track = 1;
         time_str = track2_time_str;
         title_str = track2_title_str;
         name_str = track2_name_str;
         break;
      case LABS_STATE:
         track = 2;
         time_str = lab_time_str;
         title_str = lab_title_str;
         name_str = lab_name_str;
         break;
      case VILLAGE_STATE: 
        track = 3;
        time_str = village_time_str;
        title_str = village_title_str;
        name_str = track1_name_str;
      break;
  }
  u8g.firstPage();
  do  { // draw page
    u8g.setFont(u8g_font_6x12r);
    u8g.setFontRefHeightText();
    u8g.setFontPosTop();
    // Track
    d = (DISPLAY_WIDTH-u8g.getStrWidthP((u8g_uint_t*)pgm_read_word(&sched_menu_str[track])))/2;
    u8g.drawStrP(d, 0,(const u8g_uint_t*)pgm_read_word(&sched_menu_str[track]));
    //Time
    d = (DISPLAY_WIDTH-u8g.getStrWidthP((u8g_uint_t*)pgm_read_word(&time_str[info_current])))/2;
    u8g.drawStrP(d, L_FONT_HEIGHT,(const u8g_uint_t*)pgm_read_word(&time_str[info_current]));
    u8g.drawLine(0,2*L_FONT_HEIGHT,DISPLAY_WIDTH,2*L_FONT_HEIGHT);
    //Talk
     if (u8g.getStrWidthP((u8g_uint_t*)pgm_read_word(&title_str[info_current])) > DISPLAY_WIDTH )
     //string too long, trim and print
     {
         pos = 0;
         line = 2;
         ptr = (uint16_t)pgm_read_word(&title_str[info_current]);
         do
         {
           trimmed[pos++] = (char) pgm_read_byte(ptr) ; //there should be bounds checking here but we control the strings beware stack corruption
           if ((char) pgm_read_byte(ptr++) == '\0') break;
           if ((char) pgm_read_byte(ptr) == '\n'){         
             #if defined DEBUG
             Serial.println(trimmed);
             #endif
             trimmed[pos] = '\0';
             d = (DISPLAY_WIDTH - u8g.getStrWidth(trimmed))/2;
             u8g.drawStr(d,line*L_FONT_HEIGHT,trimmed);
             pos = 0;
             line++;
             ptr++;
           }
         } while (pgm_read_byte(ptr) != '\0' );
         trimmed[pos] = '\0';
         d = (DISPLAY_WIDTH - u8g.getStrWidth(trimmed))/2;
         u8g.drawStr(d,line*L_FONT_HEIGHT,trimmed);
     }
     else {   //string fits, just print     
     d = (DISPLAY_WIDTH-u8g.getStrWidthP((u8g_uint_t*)pgm_read_word(&title_str[info_current])))/2;
     u8g.drawStrP(d,2*L_FONT_HEIGHT+1,(const u8g_uint_t*)pgm_read_word(&title_str[info_current]));
     }
    //Speaker
     if (u8g.getStrWidthP((u8g_uint_t*)pgm_read_word(&name_str[info_current])) > DISPLAY_WIDTH )
     //string too long, trim and print
     {
         pos = 0;
         ptr = (uint16_t)pgm_read_word(&name_str[info_current]);
         do
         {
           trimmed[pos++] = (char) pgm_read_byte(ptr) ; //there should be bounds checking here but we control the strings beware stack corruption
           if ((char) pgm_read_byte(ptr++) == '\0') break;
           if ((char) pgm_read_byte(ptr) == '\n'){         
             #if defined DEBUG
             Serial.println(trimmed);
             #endif
             trimmed[pos] = '\0';
             d = (DISPLAY_WIDTH - u8g.getStrWidth(trimmed))/2;
             u8g.drawStr(d,line*L_FONT_HEIGHT,trimmed);
             pos = 0;
             line++;
             ptr++;
           }
         } while (pgm_read_byte(ptr) != '\0' );
         trimmed[pos] = '\0';
         d = (DISPLAY_WIDTH - u8g.getStrWidth(trimmed))/2;
         u8g.drawStr(d,line*L_FONT_HEIGHT,trimmed);
     }
     else {
     if (global_state == VILLAGE_STATE) continue;
     d = (DISPLAY_WIDTH-u8g.getStrWidthP((u8g_uint_t*)pgm_read_word(&name_str[info_current])))/2;
     u8g.drawStrP(d,5*L_FONT_HEIGHT,(const u8g_uint_t*)pgm_read_word(&name_str[info_current]));
     }
  } while( u8g.nextPage() );
 
}

void menu_state()
{
    #if defined(DEBUG)
    Serial.print("G_state: ");
    Serial.println(global_state);
    Serial.print("Menu_curr: "); Serial.println(menu_current);
    Serial.print("Info_curr: "); Serial.println(info_current);
    #endif
    switch(global_state) {
    case GAME_STATE: /* u8g.undoRotation(); breaker_loop(); u8g.setRot180(); */ break;
    case SCHED_STATE: menu_redraw = 1; drawMenu(sched_menu_str); menu_redraw = 0; break;
    case LINK_STATE: /*draw_link(); m2.setRoot(&el_main);*/ break;
    case REG_STATE: break;
    case HELP_STATE:/* draw_icon();*/ break;
    case TRACK_ONE_STATE:
    case TRACK_TWO_STATE:
    case LABS_STATE:  
    case VILLAGE_STATE:
    case PARTY_STATE:
      drawMenuInfo();
      break;
    default: drawMenu(main_menu_str); break;
  }
    return;
}


void updateMenu(void) {
  if ( uiKeyCode != KEY_NONE && last_key_code == uiKeyCode ) {
    return;
  }
  last_key_code = uiKeyCode;
  byte max_items = 0;
  switch (global_state){
    case TRACK_ONE_STATE: max_items = TRACK1_ITEMS; break;
    case TRACK_TWO_STATE: max_items = TRACK2_ITEMS; break;
    case LABS_STATE:  max_items = LAB_ITEMS; break;
    case VILLAGE_STATE: max_items = VILLAGE_ITEMS; break;
    default: max_items = MENU_ITEMS; break;
  }
  switch ( uiKeyCode ) {
    case KEY_NEXT: //down
      if (global_state < 16 || global_state == MAIN_STATE){
        menu_current++;
        if ( menu_current >= MENU_ITEMS )
          menu_current = 0;
      }
      else {
        info_current++;
        if ( info_current >= max_items )
          info_current = 0;
      }
      menu_redraw = 1;
      break;
    case KEY_PREV: //up
      if (global_state < 16 || global_state == MAIN_STATE) {
         if ( menu_current == 0 )
          menu_current = MENU_ITEMS;
        menu_current--;
      }
      else {
        
        if ( info_current == 0 )
          info_current = max_items;
        info_current--;
      }
      menu_redraw = 1;
      break;
    case KEY_SELECT:
      if ( global_state == MAIN_STATE)
      {
        global_state = menu_current;
        menu_current = 0;
        menu_state();
      }
      else if ( global_state == SCHED_STATE)
      {
        global_state = (menu_current == (MENU_ITEMS - 1)) ? PARTY_STATE : ( SCHED_STATE << (4 + menu_current));
        if (info_current > max_items ) info_current = max_items;
        menu_state();

      }
       
      break;
   case KEY_BACK:
      global_state = ((global_state > 15) && (global_state != MAIN_STATE) ) ? SCHED_STATE : MAIN_STATE;
      menu_current = 0;
      menu_state();
      break;
   }

}

void setup() {
  // rotate screen, if required
  u8g.setRot180();
  
  uiSetup();                                // setup key detection and debounce algorithm
  menu_redraw = 1;     // force initial redraw
  #if defined(DEBUG)
     Serial.begin(9600);
  #endif 

}

void loop() {  

  uiStep();                                     // check for key press
    
  if (  menu_redraw != 0 ) {
 
      menu_state();
  } 
  menu_redraw = 0;
  

  updateMenu();                            // update menu bar
  
}
