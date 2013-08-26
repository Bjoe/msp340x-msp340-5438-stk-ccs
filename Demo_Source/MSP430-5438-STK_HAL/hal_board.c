/**
 * @file  hal_board.c
 *
******************************************************************************/
#include "msp430.h"
#include "hal_MSP430-5438-STK.h"

/**********************************************************************//**
 * @brief  Initializes ACLK, MCLK, SMCLK outputs on P11.0, P11.1,
 *         and P11.2, respectively.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void halBoardOutputSystemClock(void) //outputs clock to testpoints
{
  CLK_PORT_DIR |= 0x07;
  CLK_PORT_SEL |= 0x07;
}

/**********************************************************************//**
 * @brief  Stops the output of ACLK, MCLK, SMCLK on P11.0, P11.1, and P11.2.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void halBoardStopOutputSystemClock(void)
{
  CLK_PORT_OUT &= ~0x07;
  CLK_PORT_DIR |= 0x07;	
  CLK_PORT_SEL &= ~0x07;
}

/**********************************************************************//**
 * @brief  Initializes all GPIO configurations.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void halBoardInit(void)
{
  //Tie unused ports
  PAOUT  = 0;
  PADIR  = 0xFFFF;
  PASEL  = 0;
  PBOUT  = 0;
  PBDIR  = 0xFFFF;
  PBSEL  = 0;
  PCOUT  = 0;
  PCDIR  = 0xFFFF;
  PCSEL  = 0x000C;      // P5.2 + P5.3 = XT2 (18 MHz)
  PDOUT  = 0;
  PDDIR  = 0xFFFF;
  PDSEL  = 0x0003;      // P7.0 + P7.1 = XT1 (32,768 kHz)
  PEOUT  = 0;
  PEDIR  = 0xFFFF;
  PESEL  = 0;
  PFOUT  = 0;
  PFDIR  = 0xFFFF;
  PFSEL  = 0;
  PJOUT  = 0;
  PJDIR  = 0xFF;
}

/**********************************************************************//**
 * @brief  Set function for MCLK frequency.
 *
 *
 * @return none
 *************************************************************************/
void hal430SetSystemClock(unsigned long req_clock_rate, unsigned long ref_clock_rate)
{
  /* Convert a Hz value to a KHz value, as required
   *  by the Init_FLL_Settle() function. */
  unsigned long ulCPU_Clock_KHz = req_clock_rate / 1000UL;

  //Make sure we aren't overclocking
  if(ulCPU_Clock_KHz > 25000L)
  {
    ulCPU_Clock_KHz = 25000L;
  }

  //Set VCore to a level sufficient for the requested clock speed.
  if(ulCPU_Clock_KHz <= 8000L)
  {
    SetVCore(PMMCOREV_0);
  }
  else if(ulCPU_Clock_KHz <= 12000L)
  {
    SetVCore(PMMCOREV_1);
  }
  else if(ulCPU_Clock_KHz <= 20000L)
  {
    SetVCore(PMMCOREV_2);
  }
  else
  {
    SetVCore(PMMCOREV_3);
  }

  //Set the DCO
  Init_FLL_Settle( ( unsigned short )ulCPU_Clock_KHz, req_clock_rate / ref_clock_rate );
}

/**********************************************************************//**
 * @brief  Set function for SMCLK (Sub-System Master Clock) frequency.
 *
 *
 * @return none
 *************************************************************************/
void hal430SetSubSystemMasterClock()
{
  XT2_Start(XT2DRIVE_2);

  //UCSCTL5 |= 0x0500;         //ACLK divisor = 32
  SELECT_SMCLK(SELS__XT2CLK);  //SMCLK=XT2
}
