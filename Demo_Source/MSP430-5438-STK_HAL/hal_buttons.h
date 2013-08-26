/*******************************************************************************
    Filename: hal_buttons.h

***************************************************************************/
#ifndef HAL_BUTTONS_H
#define HAL_BUTTONS_H

#define BUTTON_PORT_DIR   P1DIR
#define BUTTON_PORT_SEL   P1SEL
#define BUTTON_PORT_OUT   P1OUT
#define BUTTON_PORT_REN   P1REN
#define BUTTON_PORT_IE    P1IE
#define BUTTON_PORT_IES   P1IES
#define BUTTON_PORT_IFG   P1IFG
#define BUTTON_PORT_IN    P1IN

#define BUTTON_SELECT     BIT4
#define BUTTON_DOWN       BIT2
#define BUTTON_UP         BIT3
#define BUTTON_RIGHT      BIT5
#define BUTTON_LEFT       BIT6
#define BUTTON_S1         BIT7
#define BUTTON_S2         BIT1
#define BUTTON_ALL        0xFE

extern volatile unsigned char buttonsPressed;

/*-------------------------------------------------------------
 *                  Function Prototypes 
 * ------------------------------------------------------------*/ 
extern void halButtonsInit(unsigned char buttonsMask);
extern unsigned char halButtonsPressed(void);
extern void halButtonsInterruptEnable(unsigned char buttonIntEnableMask);
extern void halButtonsInterruptDisable(unsigned char buttonIntEnableMask);
extern void halButtonsShutDown();

#endif /* HAL_BUTTONS_H */
