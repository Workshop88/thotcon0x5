#include "U8glib.h"
#include "M2tk.h"
#include "utility/m2ghu8g.h"

// teensy - to be removed in final
U8GLIB_DOGS102 u8g(0,4);

///////Key States/////////

#define KEY_NONE 0
#define KEY_PREV 1
#define KEY_NEXT 2
#define KEY_SELECT 3
#define KEY_BACK 4

////////Machine Stats////////
#define GAME_STATE 0
#define SCHED_STATE 1
#define LINK_STATE 2
#define REG_STATE 3
#define HELP_STATE 4


////////////BREAKER GLOBALS and DEFINES/////////

unsigned long next_sec_time;
uint8_t fps, frame_cnt;

int sensorPin = 0;  // analog input

// configuration values
uint8_t uiKeyLeftPin = 20;
uint8_t uiKeyRightPin = 19;
uint8_t uiMinIncrement = 2*8;
uint8_t uiMaxIncrement = 4*8;
// output values
uint8_t playerLocation = 0;

// internal values
uint8_t isKeyPad = 0;
uint8_t uiActualIncrement = 2*8;



/*================================================================*/
/* definitions and structures */
/*================================================================*/

#define DISPLAY_HEIGHT 64

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






// game states

#define BO_STATE_IN_GAME 1
#define BO_STATE_LOST 2
#define BO_STATE_RESTART 3
#define BO_STATE_COMLETED 4
#define BO_STATE_INTRO 5
#define BO_STATE_INTRO1 6



/*================================================================*/
/* API */
/*================================================================*/

bo_player bo_player_obj;
uint8_t bo_step_state;
uint8_t bo_timer;
uint8_t bo_level;






// setup the user interface
void uiSetup(void) {
  // configure input keys 
  pinMode(uiKeyLeftPin, INPUT);           // set pin to input
  digitalWrite(uiKeyLeftPin, HIGH);       // turn on pullup resistors
  pinMode(uiKeyRightPin, INPUT);           // set pin to input
  digitalWrite(uiKeyRightPin, HIGH);       // turn on pullup resistors
  // configure internal variables
  isKeyPad = 1;
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




void breaker_loop() {
  
 
  while (bo_step_state != BO_STATE_LOST)
  {
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
}








void bo_DrawFPS(uint8_t fps) {
  
}




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







//////GLYPHS////////////////

static uint8_t ox5 [] U8G_PROGMEM = {

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x00, 0x00, 0x00, 0x0F, 0x03, 0x83, 0xCF, 0xFC, 0x0F, 0x03, 0x83, 0xCF, 0xFC, 0x31, 0xC3,

0xE7, 0xCE, 0x00, 0x31, 0xC1, 0xE7, 0x0F, 0xFC, 0xF0, 0xF1, 0xFF, 0x0F, 0xFC, 0xF0, 0xF0, 0x7E,

0x00, 0x0F, 0xF0, 0xF0, 0x7E, 0x00, 0x0F, 0xF0, 0xF1, 0xFF, 0x00, 0x0F, 0x38, 0xC1, 0xE7, 0x0E,

0x0F, 0x38, 0xC3, 0xE7, 0xCE, 0x0F, 0x0F, 0x03, 0x83, 0xC7, 0xFC, 0x0F, 0x03, 0x83, 0xC7, 0xFC,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x00, 0x00, 0x00, 

};

static uint8_t logo [] U8G_PROGMEM = {

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x00, 0x00, 0x00, 0x1F, 0xF9, 0xC1, 0xE3, 0xFE, 0x07, 0xFE, 0x0F, 0xE0, 0xFF, 0x87, 0x07,

0x1F, 0xF9, 0xC1, 0xE3, 0xFE, 0x07, 0xFE, 0x0F, 0xE0, 0xFF, 0x87, 0x07, 0x03, 0xC1, 0xC1, 0xE7,

0x07, 0x80, 0xF0, 0x3C, 0x79, 0xC1, 0xE7, 0xC7, 0x03, 0xC1, 0xC1, 0xE7, 0x07, 0x80, 0xF0, 0x3C,

0x79, 0xC1, 0xE7, 0xF7, 0x03, 0xC1, 0xC1, 0xE7, 0x07, 0x80, 0xF0, 0x70, 0x01, 0xC1, 0xE7, 0xF7,

0x03, 0xC1, 0xFF, 0xE7, 0x07, 0x80, 0xF0, 0x70, 0x01, 0xC1, 0xE7, 0xFF, 0x03, 0xC1, 0xFF, 0xE7,

0x07, 0x80, 0xF0, 0x70, 0x01, 0xC1, 0xE7, 0xFF, 0x03, 0xC1, 0xC1, 0xE7, 0x07, 0x80, 0xF0, 0x70,

0x01, 0xC1, 0xE7, 0x3F, 0x03, 0xC1, 0xC1, 0xE7, 0x07, 0x80, 0xF0, 0x3C, 0x79, 0xC1, 0xE7, 0x3F,

0x03, 0xC1, 0xC1, 0xE7, 0x07, 0x80, 0xF0, 0x3C, 0x79, 0xC1, 0xE7, 0x0F, 0x03, 0xC1, 0xC1, 0xE3,

0xFE, 0x00, 0xF0, 0x0F, 0xE0, 0xFF, 0x87, 0x07, 0x03, 0xC1, 0xC1, 0xE3, 0xFE, 0x00, 0xF0, 0x0F,

0xE0, 0xFF, 0x87, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

};


static uint8_t icon [] U8G_PROGMEM = {

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC,

0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,

0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E,

0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E,

0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x70, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x0E,

0x70, 0x03, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x70, 0x07, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x0E,

0x70, 0x0F, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x70, 0x1F, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x0E,

0x70, 0x3F, 0xFC, 0x00, 0x00, 0x07, 0x80, 0x0E, 0x70, 0x3F, 0xFC, 0x00, 0x00, 0x0F, 0xC0, 0x0E,

0x70, 0x7F, 0xF8, 0x00, 0x00, 0x1F, 0xC0, 0x0E, 0x70, 0x7F, 0xF0, 0x00, 0x00, 0x3F, 0xC0, 0x0E,

0x70, 0x7F, 0xE0, 0x00, 0x00, 0x7F, 0xC0, 0x0E, 0x70, 0x7F, 0xC0, 0x00, 0x00, 0xFF, 0x80, 0x0E,

0x70, 0x3F, 0x83, 0xC0, 0x01, 0xFF, 0x00, 0x0E, 0x70, 0x1F, 0x07, 0xE0, 0x03, 0xFE, 0x00, 0x0E,

0x70, 0x00, 0x0F, 0xF0, 0x07, 0xFC, 0x00, 0x0E, 0x70, 0x00, 0x0F, 0xF0, 0x0F, 0xF8, 0x00, 0x0E,

0x70, 0x00, 0x0F, 0xF0, 0x1F, 0xF0, 0x00, 0x0E, 0x70, 0x00, 0x0F, 0xF0, 0x3F, 0xE0, 0x00, 0x0E,

0x70, 0x00, 0x07, 0xE0, 0x7F, 0xC0, 0x00, 0x0E, 0x70, 0x00, 0x03, 0xC0, 0xFF, 0x80, 0x00, 0x0E,

0x70, 0x00, 0x00, 0x01, 0xFF, 0x00, 0x00, 0x0E, 0x70, 0x00, 0x00, 0x03, 0xFE, 0x00, 0x00, 0x0E,

0x70, 0x00, 0x00, 0x07, 0xFC, 0x00, 0x00, 0x0E, 0x70, 0x00, 0x00, 0x0F, 0xF8, 0x00, 0x00, 0x0E,

0x70, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x0E, 0x70, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x0E,

0x70, 0x00, 0x00, 0x7F, 0xE0, 0x00, 0x00, 0x0E, 0x70, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0x0E,

0x70, 0x00, 0x01, 0xFF, 0x81, 0xC0, 0x00, 0x0E, 0x70, 0x00, 0x03, 0xFF, 0x07, 0xE0, 0x00, 0x0E,

0x70, 0x00, 0x07, 0xFE, 0x0F, 0xF0, 0x00, 0x0E, 0x70, 0x00, 0x0F, 0xFC, 0x0F, 0xF0, 0x00, 0x0E,

0x70, 0x00, 0x1F, 0xF8, 0x0F, 0xF0, 0x00, 0x0E, 0x70, 0x00, 0x3F, 0xF0, 0x0F, 0xF0, 0x00, 0x0E,

0x70, 0x00, 0x7F, 0xE0, 0x07, 0xF0, 0x78, 0x0E, 0x70, 0x00, 0xFF, 0xC0, 0x07, 0xE0, 0xFC, 0x0E,

0x70, 0x01, 0xFF, 0x80, 0x01, 0x81, 0xFE, 0x0E, 0x70, 0x03, 0xFF, 0x00, 0x00, 0x03, 0xFE, 0x0E,

0x70, 0x03, 0xFE, 0x00, 0x00, 0x07, 0xFE, 0x0E, 0x70, 0x03, 0xF8, 0x00, 0x00, 0x0F, 0xFE, 0x0E,

0x70, 0x03, 0xF0, 0x00, 0x00, 0x1F, 0xFE, 0x0E, 0x70, 0x03, 0xE0, 0x00, 0x00, 0x3F, 0xFC, 0x0E,

0x70, 0x00, 0xC0, 0x00, 0x00, 0x3F, 0xF8, 0x0E, 0x70, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x0E,

0x70, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xE0, 0x0E, 0x70, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xC0, 0x0E,

0x70, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x80, 0x0E, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E,

0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E,

0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E,

0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,

0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

};

static uint8_t link[] U8G_PROGMEM = {

0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x30, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0xC0, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x80, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,

0x03, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,

0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,

0x0C, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,

0x08, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,

0xF0, 0x40, 0x20, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x90, 0xC0, 0x30, 0x90, 0x00, 0x00, 0x00, 0x00,

0x8D, 0x3F, 0xCB, 0x10, 0x00, 0x00, 0x00, 0x00, 0x87, 0x3F, 0xCC, 0x10, 0x00, 0x00, 0x00, 0x00,

0xFA, 0xFF, 0xF7, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x9E, 0xFF, 0xF7, 0x90, 0x00, 0x00, 0x00, 0x00,

0x93, 0x00, 0x0C, 0x90, 0x00, 0x00, 0x00, 0x00, 0x93, 0x00, 0x0C, 0x90, 0x00, 0x00, 0x00, 0x00,

0x90, 0x30, 0xC0, 0x90, 0x00, 0x00, 0x00, 0x00, 0x90, 0x30, 0xC0, 0x90, 0x00, 0x00, 0x00, 0x00,

0x4C, 0x00, 0x03, 0x20, 0x00, 0x00, 0x00, 0x00, 0x44, 0x00, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00,

0x43, 0x0F, 0x0C, 0x20, 0x00, 0x00, 0x00, 0x00, 0x41, 0x0F, 0x08, 0x20, 0x00, 0x00, 0x00, 0x00,

0x41, 0xCF, 0x38, 0x20, 0x00, 0x00, 0x00, 0x00, 0x41, 0x40, 0x28, 0x20, 0x00, 0x00, 0x00, 0x00,

0x32, 0x3F, 0xC4, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x04, 0x80, 0x00, 0x00, 0x00, 0x00,

0x12, 0x00, 0x04, 0x80, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00,

0x10, 0x3F, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x10, 0x3F, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00,

0x1F, 0xF0, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xF0, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00,

0x10, 0x3F, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x10, 0x3F, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00,

0x10, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00,

0x0F, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF0, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00,

0x03, 0xF0, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF0, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

};




uint8_t uiKeyPrev = 17;
uint8_t uiKeyNext = 18;
uint8_t uiKeySelect = 14;
uint8_t uiKeyBack = 15;
uint8_t uiKeyRight = 19;
uint8_t uiKeyLeft = 20;

//*/
/*
//U8GLIB_DOGS102 u8g(17,11); //PROTO BOARD

//Leonardo based pin assignements must have updated pins_arduino.h for pin 30
uint8_t uiKeyPrev = 4;
uint8_t uiKeyNext = 0;
uint8_t uiKeySelect = 12;
uint8_t uiKeyBack = 6;
uint8_t uiKeyRight = 1;
uint8_t uiKeyLeft = 30;


uint8_t uiKeyCodeFirst = KEY_NONE;
uint8_t uiKeyCodeSecond = KEY_NONE;
uint8_t uiKeyCode = KEY_NONE;
*/

//////////

/*

Define UI menu elements 

*/
void state_machine(uint8_t);

const char *el_main_getstr(uint8_t i, uint8_t msg) {
  const char *s = "";
  if ( i == 0 )
     s = "Game";
  else if (i == 1)
     s = "Schedule";
  else if ( i == 2 )
     s = "GameLink";
  else if ( i == 3 )
     s = "Register";
  else if ( i == 4 )
     s = "Help";
  if (msg == M2_STRLIST_MSG_GET_STR){
  } else if (msg == M2_STRLIST_MSG_SELECT) {
     state_machine(i);
  }
  return s;
}  


uint8_t idle_count = 0;  //track idle cycles
uint8_t el_main_first = 0;
uint8_t el_main_cnt = 5;

uint8_t schedule_first=0;
uint8_t schedule_cnt=48;

M2_EXTERN_INFOP(el_welcome_info);

m2_menu_entry schedule_data[] = 
{
  { "TRACK ONE", NULL },
  { ". 0830 - DOORS", NULL },
  { ". 0945 - WELCOME.0", &el_welcome_info },
  { ". 1000 - KEYNOTE.1", NULL },
  { ". 1100 - TALK.1", NULL },
  { ". 1200 - TALK.2", NULL },
  { ". 1300 - BREAK", NULL},
  { ". 1400 - TALK.3", NULL },
  { ". 1500 - KEYNOTE.2", NULL},
  { ". 1600 - TALK.4", NULL},
  { ". 1700 - BREAK", NULL},
  { ". 1730 - TALK.5", NULL},
  { ". 1830 - TALK.6", NULL},
  { ". 1930 - TALK.7", NULL},
  { ". 2030 - HACKER BREW RESULTS", NULL},
  { ". 2100 - CLOSING", NULL},
  { ". 2120 - SYS64738", NULL},
  { "TRACK 2", NULL },
  { ". 1100 - TURBO.1", NULL},
  { ". 1130 - TURBO.2", NULL},
  { ". 1200 - TURBO.3", NULL},
  { ". 1230 - TURBO.4", NULL},
  { ". 1300 - BREAK", NULL},
  { ". 1400 - TURBO.5", NULL},
  { ". 1430 - TURBO.6", NULL},
  { ". 1500 - TURBO.7", NULL},
  { ". 1530 - BREAK", NULL},
  { ". 1600 - TURBO.8", NULL},
  { ". 1630 - TURBO.9", NULL},
  { ". 1700 - TURBO.10", NULL},
  { ". 1730 - TURBO.11", NULL},
  { ". 1800 - ./SHUTDOWN", NULL},
  { "LABS", NULL },
  { ". 0900 - LAB.1", NULL},
  { ". 1000 - LAB.2", NULL},
  { ". 1100 - LAB.3", NULL},
  { ". 1200 - LAB.4", NULL},
  { ". 1300 - BREAK", NULL},
  { ". 1400 - LAB.5/6", NULL},
  { ". 1630 - BREAK", NULL},
  { ". 1700 - LAB.7", NULL},
  { ". 1800 - LAB.8", NULL},
  { ". 1900 - ./SHUTDOWN", NULL},
  { "VILLAGE", NULL},
  { "PARTY", NULL},
  {NULL, NULL},
};
char welcome[] M2_SECTION_PROGMEM = "c7five\0";
extern M2tk m2; 
M2_EXTERN_2LMENU(el_schedule);
void schedule_root(m2_el_fnarg_p fnarg){
 m2.setRoot(&el_schedule); 
}

M2_2LMENU(el_schedule, "e5l6w100", &schedule_first, &schedule_cnt, schedule_data, 43, 45, '\0'); 
//M2_VSB(el_schedule_vsb, "l6W4r1", &schedule_first, &schedule_cnt);
//M2_LIST(list_schedule) = {&el_schedule,&el_schedule_vsb};
//M2_HLIST(el_schedule_hlist, NULL, list_schedule);
M2_STRLIST(el_main, "l6w100", &el_main_first, &el_main_cnt, el_main_getstr);
M2_INFOP(el_welcome_info, "W60l6", (uint8_t *) 1, (uint8_t *) 1, welcome, schedule_root);
M2tk m2(&el_main, m2_es_arduino, m2_eh_4bs, m2_gh_u8g_bfs);

uint8_t state;

void state_machine(uint8_t state)
{
  switch(state) {
    case GAME_STATE:  u8g.undoRotation(); breaker_loop(); u8g.setRot180(); break;
    case SCHED_STATE: m2.setRoot(&el_schedule); break;
    case LINK_STATE: draw_link(); m2.setRoot(&el_main); break;
    case REG_STATE: break;
    case HELP_STATE: draw_icon(); break;
    default: state = 2; m2.setRoot(&el_main); break;
  }
    return;
}

void draw_link()
{
  u8g.firstPage();
  do {
    u8g.drawBitmapP(35, 8, 8, 64, link);
  } while (u8g.nextPage() );
  delay(3000);
}

void draw_icon()
{
  u8g.firstPage();
  do {
    u8g.drawBitmapP(19, 0, 8, 64, icon);
  } while (u8g.nextPage() );
  delay(3000);
  
}

void scroll_intro()
{
  uint8_t y= 0;
 
  
  for (y =0 ; y <= 52; y++)
  {
      u8g.firstPage();
      do {
      u8g.drawBitmapP(0,(64-y),12,20,logo);
      } while (u8g.nextPage() );
    delay(50);
  }
  delay(1000);
  u8g.firstPage();
  do{
    u8g.drawBitmapP(0,12,12,20,logo);
    u8g.drawBitmapP(31,24,5,20,ox5);
  } while (u8g.nextPage());
  delay (3000);
}

void setup(){
  m2_SetU8g(u8g.getU8g(), m2_u8g_box_icon);
  m2.setFont(0, u8g_font_6x12r); 
  m2.setPin(M2_KEY_SELECT, uiKeySelect);
  m2.setPin(M2_KEY_NEXT, uiKeyNext);
  m2.setPin(M2_KEY_PREV, uiKeyPrev);
  m2.setPin(M2_KEY_HOME, uiKeyBack);
  
  //  u8g.setColorIndex(1);
  
  uiSetup();
  bo_Setup(0);
  next_sec_time = millis() + 1000UL;
  fps = 0;
  frame_cnt = 0;
  
  u8g.setRot180();
  scroll_intro();
  u8g.undoRotation();
  draw_icon();
  u8g.setRot180();
}

void loop()
{
  if (idle_count > 1000)
  {
     draw_icon();
     delay(10000); 
  }
  m2.checkKey();
  if (m2.handleKey() )
  {
     u8g.firstPage();
     do{
        m2.draw();
     } while (u8g.nextPage() );
  }
  else
  {
    idle_count++;
  }
}
