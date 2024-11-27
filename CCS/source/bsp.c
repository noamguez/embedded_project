#include  "../header/bsp.h"    // private library - BSP layer
//-----------------------------------------------------------------------------  
//           GPIO configuration
//-----------------------------------------------------------------------------
void GPIOconfig(void){
  WDTCTL = WDTHOLD | WDTPW;		// Stop WDT
  // RGB configuration
  RGBArrPortDir |= 0x07;
  RGBArrPortOut = 0x00;
  RGBArrPortSEL &= ~0x07;
  // LCD configuration
  LCD_DATA_WRITE &= ~0xFF;
  LCD_DATA_DIR |= 0xF0;    // P1.4-P1.7 To Output('1')
  LCD_DATA_SEL &= ~0xF0;   // Bit clear P1.4-P1.7
  LCD_CTL_SEL  &= ~0xE0;   // Bit clear P2.5-P2.7
  //PB config
  // dir and sel configured with others
  PBsArrIntEn |= PB0;
  PBsArrIntEdgeSel |=PB0;
  PBsArrIntPend&=  ~PB0;
  _BIS_SR(GIE);                     // enable interrupts globally
}
//-------------------------------------------------------------------------------------
//            ADC config
//-------------------------------------------------------------------------------------
void ADC_config(void){
      ADC10CTL0 = ADC10SHT_2 + ADC10ON+ SREF_0 + ADC10IE;
      ADC10CTL1 = INCH_3 + ADC10SSEL_3;
      ADC10AE0 |= BIT3;
}
//-------------------------------------------------------------------------------------
//            Timer A0 and A1 config
//-------------------------------------------------------------------------------------
void StopAllTimers(void){
    TACTL = MC_0; // halt timer A
    TA1CTL = MC_0; // halt timer A

}
void TIMER_A0_config(unsigned int counter){
    TACCR0 = counter;
    TACCTL0 = CCIE;
    TA0CTL = TASSEL_2 + MC_1 + ID_3;
}
void TIMER_A1_config(unsigned int counter){
    TA1CCR0 = counter;
    TA1CCTL0 = CCIE;
    TA1CTL = TASSEL_2 + MC_1 + ID_3;
}

void FlashConfig(){ //from hannan
    WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
     if (CALBC1_1MHZ==0xFF)                    // If calibration constant erased
     {
       while(1);                               // do not load, trap CPU!!
     }
     DCOCTL = 0;                               // Select lowest DCOx and MODx settings
     BCSCTL1 = CALBC1_1MHZ;                    // Set DCO to 1MHz
     DCOCTL = CALDCO_1MHZ;
     FCTL2 = FWKEY + FSSEL0 + FN1;             // MCLK/3 for Flash Timing Generator
}
//-------------------------------------------------------------------------------------
//                              UART config
//-------------------------------------------------------------------------------------
void UART_init(void){
    if (CALBC1_1MHZ==0xFF)                     // If calibration constant erased
      {
        while(1);                              // do not load, trap CPU!!
      }
    DCOCTL = 0;                                // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;                     // Set DCO
    DCOCTL = CALDCO_1MHZ;

    P2DIR = 0xFF;                              // All P2.x outputs
    P2OUT = 0;                                 // All P2.x reset
    P1SEL = BIT1 + BIT2 ;                      // P1.1 = RXD, P1.2=TXD
    P1SEL2 = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
    P1DIR |= RXLED + TXLED;
    P1OUT &= 0x00;

    UCA0CTL1 |= UCSSEL_2;                       // CLK = SMCLK
    UCA0BR0 = 104;                              //
    UCA0BR1 = 0x00;                             //
    UCA0MCTL = UCBRS0;                          //
    UCA0CTL1 &= ~UCSWRST;                       // **Initialize USCI state machine**
}
