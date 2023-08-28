
#define USER_SETUP_ID 1000

#define TFT_PARALLEL_8_BIT

//#define ILI9341_DRIVER
#define ST7796_DRIVER
//#define ILI9488_DRIVER

// ESP32 S3 pins used for the parallel interface TFT

// #define TFT_CS    10
// #define TFT_DC    9  // Data Command control pin - must use a GPIO in the range 0-31        //DC就是RS
// #define TFT_RST  14

// #define TFT_WR    15  // Write strobe control pin - must use a GPIO in the range 0-31
// #define TFT_RD    6

// #define TFT_D0   1  // Must use GPIO in the range 0-31 for the data bus
// #define TFT_D1   2  // so a single register write sets/clears all bits
// #define TFT_D2   7
// #define TFT_D3   8
// #define TFT_D4   3
// #define TFT_D5   18
// #define TFT_D6    17
// #define TFT_D7    16

// #define TFT_BL   4                                            // LED back-light control pin
// #define TFT_BACKLIGHT_ON LOW  // Level to turn ON back-light (HIGH or LOW)

#define TFT_CS    10
#define TFT_DC    9  // Data Command control pin - must use a GPIO in the range 0-31        //DC就是RS
#define TFT_RST  14

#define TFT_WR    15  // Write strobe control pin - must use a GPIO in the range 0-31
#define TFT_RD    6

#define TFT_D0   16  // Must use GPIO in the range 0-31 for the data bus
#define TFT_D1   17  // so a single register write sets/clears all bits
#define TFT_D2   18
#define TFT_D3   3
#define TFT_D4   8 
#define TFT_D5   7
#define TFT_D6   2
#define TFT_D7   1

#define TFT_BL   4                                            // LED back-light control pin
#define TFT_BACKLIGHT_ON LOW  // Level to turn ON back-light (HIGH or LOW)


#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF

#define SMOOTH_FONT
