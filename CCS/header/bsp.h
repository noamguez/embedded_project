#ifndef _bsp_H_
#define _bsp_H_
#include  <msp430g2553.h>



#define   debounceVal      10000 //maybe useful for button

// RGB abstraction
#define RGBArrPortOut      P2OUT
#define RGBArrPortDir      P2DIR
#define RGBArrPortSEL      P2SEL

// LCDs abstraction
#define LCD_DATA_WRITE     P1OUT
#define LCD_DATA_DIR       P1DIR
#define LCD_DATA_READ      P1IN
#define LCD_DATA_SEL       P1SEL
#define LCD_CTL_SEL        P2SEL

//   Buzzer abstraction
#define BuzzPortSel        P2SEL
#define BuzzPortDir        P2DIR
#define BuzzPortOut        P2OUT

// PushButtons abstraction
#define PBsArrPort	       P2IN
#define PBsArrIntPend	   P2IFG
#define PBsArrIntEn	       P2IE
#define PBsArrIntEdgeSel   P2IES
#define PBsArrPortSel      P2SEL
#define PBsArrPortDir      P2DIR
#define PBsArrPortOut      P2OUT
#define PB0                0x01  // P1.0

#define TXLED BIT0
#define RXLED BIT6
#define TXD BIT2
#define RXD BIT1

extern void GPIOconfig(void);
extern void ADC_config(void);
extern void TIMER_A0_config(unsigned int counter);
extern void TIMER_A1_config(unsigned int counter);
extern void StopAllTimers(void);
extern void UART_init(void);
extern void FlashConfig();
#endif



