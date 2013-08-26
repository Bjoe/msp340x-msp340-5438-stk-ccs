#ifndef HAL_LCD_NOKIA3310_H
#define HAL_LCD_NOKIA3310_H

#include <stdint.h>

#define LCD_FREQ_HZ  ( 1000000UL )

#define LCD_SPI_SEL         P9SEL
#define LCD_SPI_DIR         P9DIR
#define LCD_OUT             P9OUT

#define LCD_DC_PIN          BIT7
#define LCD_RESET_PIN       BIT6
#define LCD_SCLK_PIN         BIT3
//#define LCD_MOSI_PIN        BIT2
#define LCD_SDIN_PIN        BIT1
#define LCD_SCE_PIN         BIT0

/*
                                *********************************
                                *  ----->        X              *
                                * |                             *
                                * |                             *
                                * |                             *
                                * V                             *
                                *  Y                            *
                                *                               *
                                *                               *
                                *                               *
                                *                               *
                                *                               *
                                *                               *
                                *********************************
*/
#define N3310_B_X               83
#define N3310_H_Y               47
#define N3310_PIXELX            84
#define N3310_PIXELY            48
#define N3310_CACHE_SIZE        ((N3310_PIXELX * N3310_PIXELY) / 8)

// Lcd Commands (normal Functionset)
#define N3310_FUNCTIONSET       0x20
#define N3310_SETXADDR          0x80
#define N3310_SETYADDR          0x40
#define N3310_DISPLAYCONTROL    0x08

// Lcd Commands (extended Functionset)
#define N3310_BIAS              0x10    // Bias 1:48
#define N3310_TEMPCOEF          0x04    // TF = 2
#define N3310_SETVOP            0x80

#define N3310_EXT_INSTRSET      0x01

#define N3310_BIAS_1_100        0x00
#define N3310_BIAS_1_80         0x01
#define N3310_BIAS_1_65         0x02
#define N3310_BIAS_1_48         0x03
#define N3310_BIAS_1_40_1_34    0x04
#define N3310_BIAS_1_24         0x05
#define N3310_BIAS_1_18_1_16    0x06
#define N3310_BIAS_1_10_1_9_1_8 0x07


#define N3310_TEMPCOEFF00       0x00
#define N3310_TEMPCOEFF01       0x01
#define N3310_TEMPCOEFF02       0x02
#define N3310_TEMPCOEFF03       0x03

#define N3310_DISPLAYFLUSH      0x01
#define N3310_DISPLAYNORMAL     0x04
#define N3310_DISPLAYINVERT     0x05

#define INVERT_TEXT             BIT0
#define OVERWRITE_TEXT          BIT1

/*-------------------------------------------------------------
 *                  Function Prototypes
 * ------------------------------------------------------------*/
extern void halLcdInit(void);
extern void halLcdSendCommand(uint8_t data) ;
extern void halLcdSetContrast(uint8_t contrastLevel);

//Move to specified LCD address
extern void halLcdSetAddress(uint8_t x, uint8_t y);

//Clear LCD Screen
extern void halLcdClearScreen(void);

//Invert black to white and vice versa
extern void halLcdReverse(uint8_t inverse);

extern void halLcdPrintLine(char string[], uint8_t line, uint8_t textStyle);

#endif
