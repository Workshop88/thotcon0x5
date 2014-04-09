#include "U8glib.h"
U8GLIB_DOGS102 u8g(17,11); //leonardo

unsigned long next_sec_time;
uint8_t fps, frame_cnt;

int sensorPin = 0;  // analog input

// configuration values
uint8_t uiKeyLeftPin = 30;
uint8_t uiKeyRightPin = 1;
uint8_t uiMinIncrement = 2*8;
uint8_t uiMaxIncrement = 4*8;

//copied from some other sketch
//uint8_t uiKeyPrev = 4;
//uint8_t uiKeyNext = 0;
//uint8_t uiKeySelect = 12;
//uint8_t uiKeyBack = 6;
//uint8_t uiKeyRight = 1;
//uint8_t uiKeyLeft = 30;

// output values
uint8_t playerLocation = 0;

// internal values
uint8_t isKeyPad = 0;
uint8_t uiActualIncrement = 2*8;











#include <stdio.h>

#define DISPLAY_HEIGHT 64

/*================================================================*/
/* definitions and structures */
/*================================================================*/

///#define BO_F1 font_5x7
///#define BO_F2 font_6x12
///#define BO_F3 font_8x13

#define BO_FP 4

#define BO_AREA_WIDTH 7
#define BO_AREA_HEIGHT 3

#define BO_BRICK_WIDTH ( (int16_t)10 << BO_FP )
#define BO_BRICK_HEIGHT ( (int16_t)4 << BO_FP )

#define BRICK_WIDTH 10
#define BRICK_HEIGHT 4

#define BALL_WIDTH 4
#define BALL_HEIGHT 4

#define BO_AREA_UNIT_X ( BO_BRICK_WIDTH + ( (int16_t)4 << BO_FP ) )
#define BO_AREA_UNIT_Y ( BO_BRICK_HEIGHT + ( (int16_t)4 << BO_FP ) )
#define BO_AREA_ORIG_X ( (int16_t)2 << BO_FP )
#define BO_AREA_ORIG_Y ( (int16_t)(DISPLAY_HEIGHT - 32) << BO_FP )

/* dimensions of the overall game field (BO_FP units) */
#define BO_FIELD_WIDTH ( BO_AREA_ORIG_X + BO_AREA_UNIT_X * BO_AREA_WIDTH )
#define BO_FIELD_HEIGHT ( BO_AREA_ORIG_Y + BO_AREA_UNIT_Y * BO_AREA_HEIGHT )

/* pixel values */
#define BO_FIELD_X0 2
#define BO_FIELD_Y0 2
#define BO_FIELD_X1 ( (uint8_t)( BO_FIELD_X0 + ( (BO_FIELD_WIDTH)>>BO_FP ) ) )
#define BO_FIELD_Y1 ( (uint8_t)( BO_FIELD_Y0 + ( (BO_FIELD_HEIGHT) >> BO_FP ) ) )
#define BO_FIELD_PIX_WIDTH ( (uint8_t)( (BO_FIELD_WIDTH) >> BO_FP ) )
#define BO_FIELD_PIX_HEIGHT ( (uint8_t)( (BO_FIELD_HEIGHT) >> BO_FP ) )

/* brick states */
#define BO_BRICK_NONE 0
#define BO_BRICK_CLOSE8 1 
#define BO_BRICK_CLOSE7 2 
#define BO_BRICK_CLOSE6 3 
#define BO_BRICK_CLOSE5 4 
#define BO_BRICK_CLOSE4 5 
#define BO_BRICK_CLOSE3 6
#define BO_BRICK_CLOSE2 7
#define BO_BRICK_CLOSE_START 8
#define BO_BRICK_NORMAL 10
#define BO_BRICK_SOLID 11
#define BO_BRICK_BALL 12
#define BO_BRICK_NO_REFLECTION 13

struct bo_ball
{
  int16_t dx;	/* fixed point 12.4 */
  int16_t dy;  /* fixed point 12.4 */
  int16_t x0;	/* fixed point 12.4 */
  int16_t x1;	/* fixed point 12.4 */
  int16_t y0;   /* fixed point 12.4 */
  int16_t y1;   /* fixed point 12.4 */
  int16_t lbr_x;	/* last brick reflection */
  int16_t lbr_y;	/* last brick reflection */
  unsigned is_ball_reflection;
  unsigned is_ball_lost;
};

bo_ball bo_ball1_obj;
bo_ball bo_ball2_obj;
bo_ball bo_ball3_obj;

struct bo_player
{
  int16_t w;	/* fixed point 12.4 */
  int16_t x0;	/* fixed point 12.4 */
  int16_t x1;	/* fixed point 12.4 */
  int16_t y0;   /* fixed point 12.4 */
  int16_t y1;   /* fixed point 12.4 */
};

/*================================================================*/
/* global variables */
/*================================================================*/


uint8_t bo_area[BO_AREA_HEIGHT][BO_AREA_WIDTH];
int16_t bo_player_brick_points; /* only written to... could  be removed */
int16_t bo_remaining_bricks;
int16_t bo_no_reflection_cnt = 0;

















// setup the user interface
void uiSetup(void) {
  // configure input keys 
  pinMode(uiKeyLeftPin, INPUT);           // set pin to input
  digitalWrite(uiKeyLeftPin, HIGH);       // turn on pullup resistors
  pinMode(uiKeyRightPin, INPUT);           // set pin to input
  digitalWrite(uiKeyRightPin, HIGH);       // turn on pullup resistors
  // configure internal variables
  isKeyPad = 0;
  // assign some (more or less) useful values to the output variables
  playerLocation = 127;
}

// calculate new output values
void uiStep(void) {
  uint8_t is_key_pressed = 0;
  uint8_t real_inc;
  if ( isKeyPad == 0 )
  {
    int sensorValue;
    // assume, that no keypad is used and derive the location from analog input
    sensorValue = analogRead(sensorPin);
    sensorValue >>= 2;
    playerLocation = sensorValue;
    // now check if some keys are pressed
    if ( digitalRead(uiKeyLeftPin) == LOW )
      isKeyPad = 1;
    if ( digitalRead(uiKeyRightPin) == LOW )
      isKeyPad = 1;
  }
  if ( isKeyPad != 0 )
  {
    real_inc = uiActualIncrement;
    real_inc >>= 3;
    if ( digitalRead(uiKeyLeftPin) == LOW )
    {
      is_key_pressed = 1;
      if ( playerLocation <= 255 -  real_inc )
	playerLocation += real_inc;
    }
    if ( digitalRead(uiKeyRightPin) == LOW )
    {
      is_key_pressed = 1;
      if ( playerLocation >= 0+  real_inc )
	playerLocation -= real_inc;
    }
    if ( is_key_pressed == 0 )
    {
      uiActualIncrement = uiMinIncrement;
    }
    else
    {
      if ( uiActualIncrement < uiMaxIncrement )
	uiActualIncrement++;
    }
  }
}


void setup() {
  ///u8g.setRot180();
  u8g.setColorIndex(1);
  
  uiSetup();
  bo_Setup(0);
  next_sec_time = millis() + 1000UL;
  fps = 0;
  frame_cnt = 0;
}

void loop() {
  u8g.firstPage();  
  do {
    bo_Draw();
    bo_DrawFPS(fps);
  } while( u8g.nextPage() );
  
  
  uiStep();
  bo_SetX(playerLocation);
  bo_Step();  
  
  frame_cnt++;
  if ( next_sec_time < millis() )
  {
    fps = frame_cnt;
    frame_cnt = 0;
    next_sec_time = millis() + 1000UL;
  }
}








void bo_DrawFPS(uint8_t fps) {
  
}


/*================================================================*/
/* API */
/*================================================================*/

bo_player bo_player_obj;
uint8_t bo_step_state;
uint8_t bo_timer;
uint8_t bo_level;

#define BO_STATE_IN_GAME 1
#define BO_STATE_LOST 2
#define BO_STATE_RESTART 3
#define BO_STATE_COMLETED 4
#define BO_STATE_INTRO 5
#define BO_STATE_INTRO1 6

void bo_Draw() {
    draw_bricks();
    draw_ball(&bo_ball1_obj);
    draw_ball(&bo_ball2_obj);
    draw_ball(&bo_ball3_obj);
    draw_player(&bo_player_obj);
    ///draw_field(bo_level);
    
    /*
    if ( bo_step_state == BO_STATE_INTRO1 )
    {
      DOG_PSTR_P s1 = DOG_PSTR("THOTCON 0x5");
      DOG_PSTR_P s2 = DOG_PSTR(BO_BUILD);
      s16 o = ((dog_sin(((s16)bo_timer)*3))/21);
      u8 w;
      
      w = dog_GetStrWidthP(BO_F2, s1);
      dog_DrawStrP(BO_FIELD_X0 + (BO_FIELD_PIX_WIDTH-w)/2, 18+o, BO_F2, s1);
      
      w = dog_GetStrWidthP(BO_F2, s2);
      dog_DrawStrP(BO_FIELD_X0 + (BO_FIELD_PIX_WIDTH-w)/2, 10+o, BO_F2, s2);
    }
    if ( bo_step_state == BO_STATE_LOST )
    {
      DOG_PSTR_P s = DOG_PSTR("Game Over");
      u8 w = dog_GetStrWidthP(BO_F3, s);
      dog_DrawStrP(BO_FIELD_X0 + (BO_FIELD_PIX_WIDTH-w)/2, 10+(bo_timer>>4), BO_F3, s);
    }
    if ( bo_step_state == BO_STATE_COMLETED )
    {
      DOG_PSTR_P s = DOG_PSTR("Completed");
      u8 w = dog_GetStrWidthP(BO_F3, s);
      dog_DrawStrP(BO_FIELD_X0 + (BO_FIELD_PIX_WIDTH-w)/2, 20-(bo_timer>>4), BO_F3, s);
    }
    */
}

void bo_SetX(uint8_t pos) {
  bo_SetPlayerPos(&bo_player_obj, pos);
}

void bo_Step(void)
{
    if ( bo_no_reflection_cnt >= 3 )
    {
      bo_no_reflection_cnt-=3;
    }
    else
    {
      bo_no_reflection_cnt=0;
    }
    switch(bo_step_state)
    {
      case BO_STATE_INTRO:
	  bo_step_state = BO_STATE_INTRO1;
	  bo_timer = 127;
	break;
      case BO_STATE_INTRO1:
	bo_timer--;
	if ( bo_timer == 0 )
	  bo_step_state = BO_STATE_IN_GAME;
	break;
      case BO_STATE_IN_GAME:
	bo_DoBallStep(&bo_ball1_obj, &bo_player_obj);
	bo_DoBallStep(&bo_ball2_obj, &bo_player_obj);
	bo_DoBallStep(&bo_ball3_obj, &bo_player_obj);
	if ( bo_ball1_obj.is_ball_lost != 0 && bo_ball2_obj.is_ball_lost != 0  && bo_ball3_obj.is_ball_lost != 0 )
	{
	  bo_step_state = BO_STATE_LOST;
	  bo_timer = 80;
	}
	if ( bo_remaining_bricks == 0 )
	{
	  bo_step_state = BO_STATE_COMLETED;
	  bo_timer = 80;
	}
	break;
      case BO_STATE_LOST:
	bo_timer--;
	if ( bo_timer == 0 )
	  bo_step_state = BO_STATE_RESTART;
	break;
      case BO_STATE_RESTART:
	bo_step_state = BO_STATE_IN_GAME;
	bo_Setup(0);
	break;
      case BO_STATE_COMLETED:
	bo_timer--;
	if ( bo_timer == 0 )
	{
	  bo_step_state = BO_STATE_IN_GAME;
	  bo_Setup(bo_level+1);
	}
    }
}

void bo_Setup(uint8_t level)
{
  /* level = 4; */
  bo_no_reflection_cnt = 0;
  bo_level = level;
  bo_SetupBall(&bo_ball1_obj);
  bo_SetupBall(&bo_ball2_obj);
  bo_ball2_obj.is_ball_lost = 1;
  bo_SetupBall(&bo_ball3_obj);
  bo_ball3_obj.is_ball_lost = 1;
  bo_SetupPlayer(&bo_player_obj);
  bo_SetupLevel(level);
  if ( level == 0 )
    bo_step_state = BO_STATE_INTRO;
  else
    bo_step_state = BO_STATE_IN_GAME;
}




/*================================================================*/
/* setup */
/*================================================================*/

void bo_CalcRemainingBricks(void)
{
  int w,h;
  int x,y;
  bo_remaining_bricks = 0;
  w = BO_AREA_WIDTH;
  h = BO_AREA_HEIGHT;
  for( y = 0; y < h; y++)
  {
    for( x = 0; x < w; x++)
    {
      if ( bo_area[y][x] == BO_BRICK_NORMAL || bo_area[y][x] == BO_BRICK_BALL || bo_area[y][x] == BO_BRICK_NO_REFLECTION )
	bo_remaining_bricks++;
    }
  }
}

void bo_SetupLevel(uint8_t level) {
  int x, y;
  
  for( y = 0; y < BO_AREA_HEIGHT; y++) {
    for( x = 0; x < BO_AREA_WIDTH; x++) {
      bo_area[y][x] = BO_BRICK_BALL;
    }
  }
  
  bo_CalcRemainingBricks();
}

void bo_SetupBall(struct bo_ball *b)
{
  b->dx = ( 1 << BO_FP ) / 2;
  b->dy = ( 1 << BO_FP ) / 2;
  b->x0 = ( 1 << BO_FP );
  b->y0 = ( 1 << BO_FP ) + 2 * ( 1 << BO_FP );
  b->x1 = 5 * ( 1 << BO_FP );
  b->y1 = 5 * ( 1 << BO_FP ) + 2 * ( 1 << BO_FP );
  
  b->lbr_x = -1;
  b->lbr_y = -1;
  
  b->is_ball_lost = 0;
  b->is_ball_reflection = 0;
}

/* pos is between 0 and 255 */
void bo_SetPlayerPos( struct bo_player *p, uint8_t pos )
{
  uint8_t tmp;
  tmp = ( ( BO_FIELD_WIDTH - p->w - BO_FP ) >> BO_FP ) ;
  tmp = (uint8_t)( ( (uint16_t)tmp * (uint16_t)pos ) / 255 );
  p->x0 = ( (int16_t)tmp ) << BO_FP;
  p->x1 = p->x0 + p->w;
  p->y0 = (int16_t)1 << BO_FP;
  p->y1 = (int16_t)3 << BO_FP;
}

void bo_SetupPlayer( struct bo_player *p )
{
  p->w = (int16_t)22 * (int16_t)( 1 << BO_FP );
  bo_SetPlayerPos( p, 128 );
}

/*================================================================*/
/* graphics */
/*================================================================*/


void draw_bricks() {
  int x, y;
  int fx, fy;
  
  for( y = 0; y < BO_AREA_HEIGHT; y++ ) {
    for( x = 0; x < BO_AREA_WIDTH; x++ ) {
      fx = BO_AREA_ORIG_X + x * BO_AREA_UNIT_X;
      fy = BO_AREA_ORIG_Y + y * BO_AREA_UNIT_Y;
      fx >>= BO_FP;
      fy >>= BO_FP;
      fx += BO_FIELD_X0;
      fy += BO_FIELD_Y0;
      draw_brick(fx, fy, bo_area[y][x]);
    }
  }
}

void draw_brick(uint8_t ox, uint8_t oy, uint8_t brick_status) {
  int w = BRICK_WIDTH;
  int h = BRICK_HEIGHT;
  
  switch(brick_status) {
	case BO_BRICK_NORMAL:
	  u8g.drawBox( ox, oy, w, h );
	  return;
	case BO_BRICK_SOLID:
	  u8g.drawBox( ox, oy, w, h );
          u8g.setColorIndex(0);
	  u8g.drawBox( ox + 1, oy + 1, w - 2, h - 2 );
          u8g.setColorIndex(1);
	  return;
	case BO_BRICK_BALL:
	  u8g.drawBox( ox, oy, w, h );
          u8g.setColorIndex(0);
	  u8g.drawVLine( ox + 2, oy + 1, oy + 3 );
	  u8g.drawPixel( ox + 1, oy + 2 );
	  u8g.drawPixel( ox + 3, oy + 2 );
          u8g.setColorIndex(1);
	  return;
	case BO_BRICK_NO_REFLECTION:
	  {
	    const unsigned char b[1] = { 0xaa };
	    ///dog_SetBox(ox, oy, ox+w, oy+h);
	    ///dog_ClrHLine(ox, ox+4, oy);
	    ///dog_SetHBitmap(ox, oy, b, 5); 
	    ///dog_ClrHLine(ox, ox+4, oy+h);
	    ///dog_SetHBitmap(ox, oy+h, b, 5); 
	    ///dog_ClrHLine(ox+w-4, ox+w, oy);
	    ///dog_SetHBitmap(ox+w-4, oy, b, 5); 
	    ///dog_ClrHLine(ox+w-4, ox+w, oy+h);
	    ///dog_SetHBitmap(ox+w-4, oy+h, b, 5); 
	  }
	  return;
	case BO_BRICK_CLOSE_START:
	  ///dog_SetBox(ox-2, oy+1, ox+w+2, oy+h);
	  return;
	case BO_BRICK_CLOSE2:
	  ///dog_SetBox(ox-3, oy+1, ox+w+3, oy+h);
	  return;
	case BO_BRICK_CLOSE3:
	  ///dog_SetBox(ox-4, oy+1, ox+w+4, oy+h-1);
	  return;
	case BO_BRICK_CLOSE4:
	  ///dog_SetBox(ox-5, oy+1, ox+w+5, oy+h-1);
	  return;
	case BO_BRICK_CLOSE5:
	  ///dog_SetBox(ox-6, oy+1, ox+w+6, oy+h-1);
	  return;
	case BO_BRICK_CLOSE6:
	  ///dog_SetBox(ox-7, oy+2, ox+w+7, oy+h-1);
	  return;
	case BO_BRICK_CLOSE7:
	  ///dog_SetBox(ox-8, oy+2, ox+w+8, oy+h-1);
	  return;
	case BO_BRICK_CLOSE8:
	  ///dog_SetBox(ox-9, oy+2, ox+w+9, oy+h-1);
	  return;
	default:
	  return;
  }
}

void draw_ball( struct bo_ball *b ) {
  if ( b->is_ball_lost )
    return;
  
  u8g.drawBox( ( b->x0 >> BO_FP ) + BO_FIELD_X0, ( b->y0 >> BO_FP ) + BO_FIELD_Y0, BALL_WIDTH, BALL_HEIGHT );
}

void draw_player( struct bo_player *p ) {
  uint8_t ox, oy, w, h;
  
  ox = p->x0 >> BO_FP;
  oy = p->y0 >> BO_FP;
  ox += BO_FIELD_X0;
  oy += BO_FIELD_Y0;
  
  w = p->w >> BO_FP;
  h = ( p->y1 - p->y0 ) >> BO_FP;
  
  u8g.drawBox( ox, oy, w, h );
}




/*================================================================*/
/* aux functions */
/*================================================================*/


uint8_t bo_IsIntersection( int16_t a_min, int16_t a_max, int16_t b_min, int16_t b_max) {
  if ( a_max < b_min )
    return 0;
  if ( a_min > b_max )
    return 0;
  return 1;
}

uint8_t bo_IsBallIntersection( struct bo_ball *b, int16_t x0, int16_t x1, int16_t y0, int16_t y1 ) {
  if ( b->is_ball_lost )
    return 0;
  if ( bo_IsIntersection( b->x0, b->x1, x0, x1 ) == 0 )
    return 0;
  if ( bo_IsIntersection( b->y0, b->y1, y0, y1 ) == 0 )
    return 0;
  return 1;
}

/*================================================================*/
/* intersection and reflection */
/*================================================================*/

/*===== brick and area intersection =====*/

/*
  test versus brick at position (x,y) [12.4]
  (x,y) is the lower left point of the brick
*/
uint8_t bo_IsBallBrickIntersection( struct bo_ball *b, int16_t x, int16_t y ) {
  return bo_IsBallIntersection( b, x, x + BO_BRICK_WIDTH, y, y + BO_BRICK_HEIGHT );
}

/* (x,y) is the lower left point of the brick */
void bo_DoBallBrickReflection( struct bo_ball *b, int16_t x, int16_t y ) {
  int16_t mx, my;
  
  if ( b->lbr_x == x && b->lbr_y == y )
    return;

  /* calculate middle of the ball */
  mx = ( b->x0 + b->x1 ) / 2;
  my = ( b->y0 + b->y1 ) / 2;
  
  if ( mx >= x && mx <= x + BO_BRICK_WIDTH ) {
    /* top/bottom or edge reflection */
    if ( my >= y && my <= y + BO_BRICK_HEIGHT ) {
      /* edge reflection */
      b->dx = - b->dx;
      b->dy = - b->dy;      
      b->is_ball_reflection = 1;
    } else {
      /* top/bottom or edge reflection */
      b->dy = - b->dy;      
      b->is_ball_reflection = 1;
    }
  } else {
    /* must be left or right reflection */
    b->dx = - b->dx;
    b->is_ball_reflection = 1;
  }
  
  b->lbr_x = x;
  b->lbr_y = y;
}

/* check area for the specified ball */
void bo_CheckBrickArea( struct bo_ball *b ) {
  int16_t gx;
  int16_t gy;
  uint8_t ix;
  uint8_t iy;
  uint8_t is_reflect = 0;
  
  gy = BO_AREA_ORIG_Y;
  iy = 0;
  
  while( iy < BO_AREA_HEIGHT )
  {
    gx = BO_AREA_ORIG_X;
    ix = 0;
    while( ix < BO_AREA_WIDTH )
    {
      switch ( bo_area[iy][ix] )
      {
	case BO_BRICK_NORMAL:
	case BO_BRICK_SOLID:
	case BO_BRICK_NO_REFLECTION:
	  if ( is_reflect == 0 )
	    if ( bo_IsBallBrickIntersection(b, gx, gy) )
	    {
	      if ( bo_area[iy][ix] == BO_BRICK_NO_REFLECTION )
		bo_no_reflection_cnt = 9*256;
	      if ( bo_no_reflection_cnt == 0 )
	      {
		bo_DoBallBrickReflection(b, gx, gy);
		is_reflect = 1;
	      }
	      bo_player_brick_points++;
	      if ( bo_area[iy][ix] != BO_BRICK_SOLID )
	      {
		bo_area[iy][ix] = BO_BRICK_CLOSE_START;
	      }
	    }
	  break;
	case BO_BRICK_BALL:
	  if ( is_reflect == 0 )
	    if ( bo_IsBallBrickIntersection(b, gx, gy) )
	    {
	      bo_ball *bb;
	      /* look for lost ball */
	      if ( bo_ball1_obj.is_ball_lost != 0 )
		bb = &bo_ball1_obj;
	      else if ( bo_ball2_obj.is_ball_lost != 0 )
		bb = &bo_ball2_obj;
	      else if ( bo_ball3_obj.is_ball_lost != 0 )
		bb = &bo_ball3_obj;
	      else
		  bb = NULL;
	      if ( bb != NULL )
		*bb = *b;		/* clone the ball */
	      /* reflection only applies to the original ball */
	      bo_DoBallBrickReflection(b, gx, gy);
	      is_reflect = 1;
	      bo_player_brick_points++;
	      bo_area[iy][ix] = BO_BRICK_CLOSE_START;
	      return;
	    }
	  break;
	case BO_BRICK_CLOSE_START:
	  bo_area[iy][ix] = BO_BRICK_CLOSE2;
	  break;
	case BO_BRICK_CLOSE2:
	  bo_area[iy][ix] = BO_BRICK_CLOSE3;
	  break;
	case BO_BRICK_CLOSE3:
	  bo_area[iy][ix] = BO_BRICK_CLOSE4;
	  break;
	case BO_BRICK_CLOSE4:
	  bo_area[iy][ix] = BO_BRICK_CLOSE5;
	  break;
	case BO_BRICK_CLOSE5:
	  bo_area[iy][ix] = BO_BRICK_CLOSE6;
	  break;
	case BO_BRICK_CLOSE6:
	  bo_area[iy][ix] = BO_BRICK_CLOSE7;
	  break;
	case BO_BRICK_CLOSE7:
	  bo_area[iy][ix] = BO_BRICK_CLOSE8;
	  break;
	case BO_BRICK_CLOSE8:
	  bo_area[iy][ix] = BO_BRICK_NONE;
	  bo_remaining_bricks--;
	  break;
	default:
	  break;
      }
      gx += BO_AREA_UNIT_X;
      ix++;
    }
    gy += BO_AREA_UNIT_Y;
    iy++;
  }  
}

/*===== field intersection =====*/

void bo_CheckField(bo_ball *b) {
  uint8_t is_reflection = 0;
  
  if ( b->x0 <= 0 ) {
    b->dx = -b->dx;
    is_reflection = 1;
  }
  if ( b->x1 >= BO_FIELD_WIDTH ) {
    b->dx = -b->dx;
    is_reflection = 1;
  }
  if ( b->y0 <= 0 ) {
    b->is_ball_lost = 1;
    b->dy = -b->dy;
    is_reflection = 1;
  }
  if ( b->y1 >= BO_FIELD_HEIGHT ) {
    b->dy = -b->dy;
    is_reflection = 1;
  }
  if ( is_reflection != 0 ) {
    b->is_ball_reflection = 1;
    /* reset last brick reflection */
    b->lbr_x = -1;
    b->lbr_y = -1;
  }
}

/*===== player intersection =====*/

uint8_t bo_IsBallPlayerIntersection( struct bo_ball *b, struct bo_player *p ) {
  if ( b->is_ball_lost )
    return 0;
  if ( bo_IsIntersection( b->x0, b->x1, p->x0, p->x1 ) == 0 )
    return 0;
  if ( bo_IsIntersection( b->y0, b->y1, p->y0, p->y1 ) == 0 )
    return 0;
  return 1;
}

void bo_DoBallPlayerReflection( struct bo_ball *b, struct bo_player *p ) {
  int16_t mx, my;
  int16_t x;
  
  if ( b->is_ball_lost )
    return;

  /* reset last brick reflection */
  b->lbr_x = -1;
  b->lbr_y = -1;  
  
  /* calculate middle of the ball */
  mx = ( b->x0 + b->x1 ) / 2;
  my = ( b->y0 + b->y1 ) / 2 ;
  if ( mx >= p->x0 && mx <= p->x1 ) {
    /* top/bottom or edge reflection */
    if ( my >= p->y0 && my <= p->y1 ) {
      /* edge reflection */
      b->dx = - b->dx;
      b->dy = - b->dy;
      b->is_ball_reflection = 1;
    } else {
      /* top/bottom or edge reflection */
      
      b->dy = - b->dy;
      
      /* reduce dx, depending on how far the middle of the player paddle was hit */ 
      x = ( p->x1 + p->x0 ) / 2;
      x = x - mx;
      
      /* x can be between 0 an (p->w <<BO_FP)/2 */
      /* normalize x to 2<<(BO_FP)/2 */
      x /= p->w >> BO_FP;

      b->dx = -x;
      
      b->is_ball_reflection = 1;
    }
  } else {
    /* must be left or right reflection */
    b->dx = - b->dx;
    b->is_ball_reflection = 1;
  }
}

void bo_CheckPlayer( struct bo_ball *b, struct bo_player *p ) {
  if ( bo_IsBallPlayerIntersection(b, p) != 0 ) {
    bo_DoBallPlayerReflection(b, p);
  }
}

/*================================================================*/
/* handle a single ball */
/*================================================================*/

void bo_DoDeltaLimit( struct bo_ball *b ) { 
  if ( b->dx >= ( 1 << BO_FP ) ) {
    b->dx = ( 1 << BO_FP );
  }
  if ( b->dy >= ( 1 << ( BO_FP - 1 ) ) ) {
    b->dy = ( 1 << ( BO_FP - 1 ) );
  }
  
  if ( b->dy == 0 ) {
    b->dy = -( 1 << ( BO_FP - 1 ) );
    bo_DoBallDelta(b);
  }
}

void bo_DoBallDelta( struct bo_ball *b ) {
  b->x0 += b->dx;
  b->x1 += b->dx;
  b->y0 += b->dy;
  b->y1 += b->dy;
}

void bo_DoBallStep( struct bo_ball *b, struct bo_player *p ) {
  bo_DoDeltaLimit(b);
  bo_DoBallDelta(b);
  
  b->is_ball_reflection = 0;
  bo_CheckBrickArea(b);
  if ( b->is_ball_reflection == 0 ) {
    bo_CheckField(b);
    if ( b->is_ball_reflection == 0 ) {
      if ( p != NULL )
	bo_CheckPlayer(b, p);
    }
  }  
  if ( b->is_ball_reflection != 0 ) {
    bo_DoBallDelta(b);
  }
}
