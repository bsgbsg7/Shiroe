// See SetupX_Template.h for all options available
#define USER_SETUP_ID 36

// #define RPI_DISPLAY_TYPE
#define ST7796_DRIVER
// #define USE_DMA


#if defined(CONFIG_IDF_TARGET_ESP32S3)
    // //Generic ESP32-S3 setup
    // #define TFT_MISO 37     //37
    // #define TFT_MOSI 35    //35
    // #define TFT_SCLK 36     //36
    #define TFT_MISO 13     //37
    #define TFT_MOSI 11    //35
    #define TFT_SCLK 12     //36
    #define TFT_CS    10 // Not connected
    #define TFT_DC    9
    #define TFT_RST   14  // Connect reset to ensure display initialises

    #define TFT_BL   4            // LED back-light control pin
    #define TFT_BACKLIGHT_ON HIGH  // Level to turn ON back-light (HIGH or LOW)

    #define TFT_SPI_MODE SPI_MODE0
#elif defined(CONFIG_IDF_TARGET_ESP32)
    // // Generic ESP32 setup
    // #define TFT_MISO 19
    // #define TFT_MOSI 23
    // #define TFT_SCLK 18
    // #define TFT_CS    15
    // #define TFT_DC    2
    // #define TFT_RST   4 // Connect reset to ensure display initialises

    // #define TFT_BL   32            // LED back-light control pin
    // #define TFT_BACKLIGHT_ON LOW  // Level to turn ON back-light (HIGH or LOW)

    // #define TOUCH_CS 27    // Chip select pin (T_CS) of touch screen
       // Generic ESP32 setup
    #define TFT_MISO 19 // (leave TFT SDO disconnected if other SPI devices share MISO)
    #define TFT_MOSI 23
    #define TFT_SCLK 18
    #define TFT_CS    15  // Chip select control pin
    #define TFT_DC    2 // Data Command control pin
    #define TFT_RST   4  // Reset pin (could connect to RST pin)

    #define TOUCH_CS 27     // Chip select pin (T_CS) of touch screen`

    #define TFT_BL   32            // LED back-light control pin
    #define TFT_BACKLIGHT_ON LOW  // Level to turn ON back-light (HIGH or LOW)
#endif




#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT


#define SPI_FREQUENCY  40000000

#define SPI_TOUCH_FREQUENCY  2500000
