/**********************************************************************//**
    Filename: hal_board.h

***************************************************************************/
#ifndef HAL_BOARD_H
#define HAL_BOARD_H

#define LED_PORT_DIR      P10DIR
#define LED_PORT_OUT      P10OUT
#define LED_1             BIT6
#define LED_2             BIT7

#define CLK_PORT_DIR      P11DIR //outputs clocks to testpoints
#define CLK_PORT_OUT      P11OUT
#define CLK_PORT_SEL      P11SEL

/*----------------------------------------------------------------
 *                  Function Prototypes
 *----------------------------------------------------------------
 */
static void halBoardGetSystemClockSettings(unsigned char systemClockSpeed, 
                                           unsigned char *setDcoRange,
                                           unsigned char *setVCore,
                                           unsigned int  *setMultiplier);

extern void halBoardOutputSystemClock(void);
extern void halBoardStopOutputSystemClock(void);
extern void halBoardInit(void);
void hal430SetSystemClock(unsigned long req_clock_rate, unsigned long ref_clock_rate);
void hal430SetSubSystemMasterClock();

#endif /* HAL_BOARD_H */
