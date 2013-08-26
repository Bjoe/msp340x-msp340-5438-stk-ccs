#include "hal_lcd_nokia3310.h"

#include "msp430.h"
#include "FreeRTOS.h"
#include "hal_lcd_font.h"
#include "hal_UCS.h"

/**********************************************************************//**
 * @brief  Sends data to the LCD using the format specified
 *         by the LCD Guide.
 *
 * @param  data data for transmission
 *
 * @return none
 *************************************************************************/
void halLcdSendCommand(uint8_t data)
{
  LCD_OUT &= ~LCD_SCE_PIN;            //CS = 0 --> Start Transfer
  LCD_OUT &= ~LCD_DC_PIN;                     // sending command

  while (!(UCB2IFG & UCTXIFG));           // Wait for TXIFG
  UCB2TXBUF = data;                       // Load data

  while((UCB2STAT & UCBUSY) != 0);

  LCD_OUT |= LCD_SCE_PIN;             //CS = 1 --> Stop Transfer
}

/**********************************************************************//**
 * @brief  Initializes the USCI module, LCD device for communication.
 *
 * - Sets up the SPI2C Communication Module
 * - Performs Nokia LCD Initialization Procedure
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void halLcdInit(void)
{
  LCD_SPI_SEL |= LCD_SDIN_PIN + LCD_SCLK_PIN;
  LCD_SPI_DIR |= LCD_DC_PIN + LCD_RESET_PIN + LCD_SCLK_PIN + LCD_SDIN_PIN + LCD_SCE_PIN;

  portENTER_CRITICAL();
  {
    // Initialize the USCI_B2 module for SPI operation
    UCB2CTL1 |= UCSWRST;

    //set format:
    //set clock polarity and phase (UCCKPH = 0; UCCKPL = 1) --> bit7=0; bit6=1
    //MSB first; bit0 (UCSYNC) = 1; bit3 (master mode) = 1; --> bit5=1; bit4=0; bit3 =3; bit0=1;
    //set mode: 3pin SPI --> bit1=0; bit2=3;
    //UCB2CTL0 = 0b01101001 = 0x69
    UCB2CTL0 = UCMST + UCSYNC + UCCKPL + UCMSB; // 3-pin, 8-bit SPI master
    UCB2CTL1 |= UCSSEL0 + UCSSEL1;                       // SMCLK

    unsigned int prescaler = configLFXT_CLOCK2_HZ / LCD_FREQ_HZ;
    UCB2BR0 = prescaler % 256;
    UCB2BR1 = prescaler / 256;

    UCB2CTL1 &= ~UCSWRST;                     // Release USCI state machine
  }
  portEXIT_CRITICAL();

  LCD_OUT &= ~LCD_RESET_PIN;         // Reset LCD
  // Wait a minimum of 100ms after issuing "start oscillation"
  // command
  __delay_cycles(2500000);
  LCD_OUT |= LCD_RESET_PIN;
  __delay_cycles(2500000);                   //Reset Pulse

  // Send sequence of command
  halLcdSendCommand(N3310_FUNCTIONSET | N3310_EXT_INSTRSET);  // N3310_FUNCTIONSETEXT    0x21    Enable extended Functionset
  halLcdSendCommand(N3310_TEMPCOEF | N3310_TEMPCOEFF01);     // N3310_TEMPCOEF          0x06    Set Tempcoefficient
  halLcdSendCommand(N3310_BIAS | N3310_BIAS_1_48);                // N3310_BIAS              0x13    Set Bias
  halLcdSendCommand(N3310_FUNCTIONSET);        // N3310_FUNCTIONSET     0x20    Enable normal Functionset
  halLcdSetContrast(0x40); // Set Contrast
}

/**********************************************************************//**
 * @brief  Set function for the contrast level of the LCD.
 *
 * @param  contrastLevel The target contrast level
 *
 * @return none
 *************************************************************************/
void halLcdSetContrast(uint8_t contrastLevel)
{
  halLcdSendCommand(N3310_FUNCTIONSET | N3310_EXT_INSTRSET);
  halLcdSendCommand(N3310_SETVOP | contrastLevel);
  halLcdSendCommand(N3310_FUNCTIONSET);
}

/**********************************************************************//**
 * @brief  Inverts the grayscale values of the LCD display (Black <> white).
 *
 * @param  inverse
 *
 * @return none
 *************************************************************************/
void halLcdReverse(uint8_t inverse)
{
  if (inverse == 1)
    halLcdSendCommand(N3310_DISPLAYCONTROL | N3310_DISPLAYINVERT);
  else
    halLcdSendCommand(N3310_DISPLAYCONTROL | N3310_DISPLAYNORMAL);
}

/**********************************************************************//**
 * @brief  Clears entire LCD CGRAM as well as LCD_MEM.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void halLcdClearScreen(void)
{
  halLcdSendCommand(N3310_DISPLAYCONTROL);     // LCD blank
  halLcdSendCommand(N3310_DISPLAYCONTROL | N3310_DISPLAYNORMAL);      // N3310_DISPLAYNORMAL        0x0C    Set Display to normal Mode
  halLcdSendCommand(N3310_SETXADDR);
  halLcdSendCommand(N3310_SETYADDR);
}

/**********************************************************************//**
 * @brief  Sets the pointer location in the LCD.
 *
 * @param  x The target X pointer location in the LCD.
 * @param  y The target Y pointer location in the LCD.
 *
 * @return none
 *************************************************************************/
void halLcdSetAddress(uint8_t x, uint8_t y)
{
  //Optional calculations +
  //in case the position is out of the screen:
  //if X > 83 it goes to the next (or more) line.
  //if Y > 5 it return to the 0, 1, 2... line
  y = y + x / 84;
  y = y % 6;
  x = x % 84;
  //Optional calculations -
  //index = 84 * y + x;

  halLcdSendCommand(x | N3310_SETXADDR);
  halLcdSendCommand(y | N3310_SETYADDR);
}

/**********************************************************************//**
 * @brief  Displays a string on the LCD on the specified line.
 *
 * @param  String[]  The string to be displayed on LCD.
 *
 * @param  Line      The line on the LCD on which to print the string.
 *
 * @param  TextStyle Value that specifies whether the string is to be
 *                   inverted or overwritten.
 *                   - Invert    = 0x01
 *                   - Overwrite = 0x04
 *
 * @return none
 *************************************************************************/
void halLcdPrintLine(char string[], uint8_t line, uint8_t textStyle)
{
  int i = 0;
  int j = 0;

  LCD_OUT &= ~LCD_SCE_PIN;            //CS = 0 --> Start Transfer
  LCD_OUT |= LCD_DC_PIN;              // sending data

  while (string[i] != 0)
  {
      for (j = 0; j < 5; j++) {
        while (!(UCB2IFG & UCTXIFG));           // Wait for TXIFG
        UCB2TXBUF= fontLookup[string[i] - 32][j];
      }
      while (!(UCB2IFG & UCTXIFG));           // Wait for TXIFG
      UCB2TXBUF= 0x00;    //space between signs
      i++;
  }

  while((UCB2STAT & UCBUSY) != 0);

  LCD_OUT |= LCD_SCE_PIN;             //CS = 1 --> Stop Transfer
}
