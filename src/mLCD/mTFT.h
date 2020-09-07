#ifndef __ILI9341_H
#define __ILI9341_H

//==============================================================================
//==============================================================================
//==============================================================================

#byte TRISC = 0xF94
#byte TRISD = 0xF95

#byte PORTC = 0xF82
#byte PORTD = 0xF83

#bit  SDO=PORTC.5
#bit  SDI=PORTC.4
#bit  SCK=PORTC.3

#bit  TRIS_SDO=TRISC.5
#bit  TRIS_SDI=TRISC.4
#bit  TRIS_SCK=TRISC.3


#define pl() printf("LINE=%d\n",__LINE__);
//==============================================================================
// Connection of module TFT.
//
// VCC      <<<>>> 5 V.
// GND      <<<>>> GND.
// TFT_CS   <<<>>> PORTD.0 You can change this pin.
// TFT_REST <<<>>> PORTD.1 You can change this pin.
// TFT_D/C  <<<>>> PORTD.2 You can change this pin.
// TFT_SDI  <<<>>> Connect to SDO MCU.
// TFT_SCK  <<<>>> Connect to SCK MCU.
// TFT_SDO  <<<>>> Connect to SDI MCU.
//
//==============================================================================
#byte TRISD = 0xF95
#byte PORTD = 0xF83

#bit  TFT_CS=PORTD.0
#bit  TRIS_CS=TRISD.0

#bit  TFT_RES=PORTD.1
#bit  TRIS_RES=TRISD.1

#bit  TFT_DC=PORTD.2
#bit  TRIS_DC=TRISD.2

//==============================================================================
// Connection of MCP3202.
//
// VCC      <<<>>> 5 V.
// GND      <<<>>> GND.
// MCP_CS   <<<>>> PORTD.3 You can change this pin.
// CH0      <<<>>> Y+
// CH1      <<<>>> X+
// MCP_SDI  <<<>>> Connect to SDO MCU.
// MCP_SCK  <<<>>> Connect to SCK MCU.
// MCP_SDO  <<<>>> Connect to SDI MCU.
// 
//==============================================================================

#bit TRIS_Yp = TRISD.7
#bit TRIS_Xp = TRISD.6
#bit TRIS_Ym = TRISD.5
#bit TRIS_Xm = TRISD.4

#bit TRIS_TS_CS = TRISD.3
#bit TS_CS = PORTD.3

#bit Yp = PORTD.7
#bit Xp = PORTD.6
#bit Ym = PORTD.5
#bit Xm = PORTD.4

//==============================================================================
//==============================================================================
#byte INTCON=0xFF2
#byte PIE1=0xF9D
#byte PIR1 = 0xf9e

#byte SSPBRG= 0xfaf
#byte SSPBUF= 0xfc9
#byte SSPADD= 0xfc8
#byte SSPSTAT= 0xfc7
#byte SSPCON1= 0xfc6
#byte SSPCON2= 0xfc5

#bit  SSPIF= PIR1.3
#bit  WCOL= SSPCON1.3
//==============================================================================
// These are some colors that you can use for fonts and graphics.
//==============================================================================                         // RRRRRGGGGGGBBBBB
#define BLACK              0b0000000000000000
#define BRIGHTBLUE         0b0000000000011111
#define BRIGHTGREEN        0b0000011111100000
#define BRIGHTCYAN         0b0000011111111111
#define BRIGHTRED          0b1111100000000000
#define BRIGHTMAGENTA      0b1111100000011111
#define BRIGHTYELLOW       0b1111111111100000
#define BLUE               0b0000000000010000
#define GREEN              0b0000010000000000
#define CYAN               0b0000010000010000
#define RED                0b1000000000000000
#define MAGENTA            0b1000000000010000
#define BROWN              0b1111110000000000
#define LIGHTGRAY          0b1000010000010000
#define DARKGRAY           0b0100001000001000
#define LIGHTBLUE          0b1000010000011111
#define LIGHTGREEN         0b1000011111110000
#define LIGHTCYAN          0b1000011111111111
#define LIGHTRED           0b1111110000010000
#define LIGHTMAGENTA       0b1111110000011111
#define YELLOW             0b1111111000000000
#define WHITE              0b1111111111111111

#define GRAY0              0b1110011100011100
#define GRAY1              0b1100011000011000   
#define GRAY2              0b1010010100010100   
#define GRAY3              0b1000010000010000
#define GRAY4              0b0110001100001100
#define GRAY5              0b0100001000001000
#define GRAY6              0b0010000100000100


//==============================================================================
// Define number of pixels in width and height.
// New name for data types.
//==============================================================================
#define TFT_W 240
#define TFT_H 320

//==============================================================================
// Sampling Size to fix touch value.
//==============================================================================
#define SMPLSIZE 10

//==============================================================================
// Function Prototypes.
//==============================================================================
signed char WriteSPI(unsigned char data);
void TFT_Init(void);
void TFT_Reset(void);
void TFT_Delay(void);
void TFT_WriteCommand(Unsigned char command);
void TFT_WriteParameter(Unsigned char parameter);
void TFT_FillScreen(Unsigned long color);
void TFT_ColumnRow(Unsigned int16 x, Unsigned int16 y);
void TFT_ColumnPage(Unsigned int16 x1, Unsigned int16 x2, Unsigned int16 y1, Unsigned int16 y2);
unsigned int16 TFT_RGBConvert(Unsigned int16 red8, Unsigned int16 green8, Unsigned int16 blue8);
//------------------------------------------------------------------------------
void TFT_Pixel(Unsigned int16 x, Unsigned int16 y, Unsigned int16 color);
void TFT_Box(Unsigned int16 x1, Unsigned int16 y1, Unsigned int16 x2, Unsigned int16 y2, Unsigned int16 color);
void TFT_Line(Unsigned int16 x1, Unsigned int16 y1, Unsigned int16 x2, Unsigned int16 y2, Unsigned int16 color);
void TFT_Rectangle(Unsigned int16 x1, Unsigned int16 y1, Unsigned int16 x2, Unsigned int16 y2, Unsigned int16 color);
void TFT_Circle(unsigned int16 SX, unsigned int16 SY, unsigned int16 Radius,unsigned int16 Fill, unsigned int16 Color);
void test_drawing(void);

void touch_init(void);
int16 Read_X(void);
int16 Read_Y(void);
unsigned int16 convert_X(unsigned int16 X);
unsigned int16 convert_Y(unsigned int16 Y);
signed int16 get_touch_Xval(signed int16* X_val);
signed int16 get_touch_Yval(signed int16* Y_val);
#endif
