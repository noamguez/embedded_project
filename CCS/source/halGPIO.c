#include  "../header/halGPIO.h"     // private library - HAL layer
#include "stdio.h"
unsigned int deg=90;

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/////////// ALL FUNCTION DESCRIPTION IN H//////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

void ser_output(char *str);
char smsg[5];
unsigned int info[4];
char counter_str2[5];    //to print counter


char Msg[5];
unsigned int i=0;         //for task 4
int j=0;                //for task 4
int st4_flag = 0;      //for task 4
int script_in_flag=0;
int flash_reset=0;
int state_flag = 0;     //for task 4
int LDR_colab_addr=0x1000;
int script1_addr=0x1020;
char *Flash_ptr;                          // Flash pointer
char getdeg[4];
int pd=0;
int globaldelay=65000;
void sysConfig(void){ 
	GPIOconfig();
	StopAllTimers();
	lcd_init();
	lcd_clear();
	UART_init();
	P1DIR=0b11110000;
	P2DIR=0b01111010;
	P1SEL=0b00001111;
	P2SEL=0b00000000;
	P1SEL2=BIT1|BIT2;
	motormove(0,20); //set def deg
}


//---------------------------------------------------------------------

void timerz(int a){
    TIMER_A0_config(a);
    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ int until Byte RXed
}

void enterLPM(unsigned char LPM_level){
	if (LPM_level == 0x00) 
	  _BIS_SR(LPM0_bits);     /* Enter Low Power Mode 0 */
        else if(LPM_level == 0x01) 
	  _BIS_SR(LPM1_bits);     /* Enter Low Power Mode 1 */
        else if(LPM_level == 0x02) 
	  _BIS_SR(LPM2_bits);     /* Enter Low Power Mode 2 */
	else if(LPM_level == 0x03) 
	  _BIS_SR(LPM3_bits);     /* Enter Low Power Mode 3 */
        else if(LPM_level == 0x04) 
	  _BIS_SR(LPM4_bits);     /* Enter Low Power Mode 4 */
}

void enable_interrupts(){
  _BIS_SR(GIE);
}

void disable_interrupts(){
  _BIC_SR(GIE);
}

void lcd_cmd(unsigned char c){

    LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h

    if (LCD_MODE == FOURBIT_MODE)
    {
        LCD_DATA_WRITE &= ~OUTPUT_DATA;// clear bits before new write
        LCD_DATA_WRITE |= ((c >> 4) & 0x0F) << LCD_DATA_OFFSET;
        lcd_strobe();
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
        LCD_DATA_WRITE |= (c & (0x0F)) << LCD_DATA_OFFSET;
        lcd_strobe();
    }
    else
    {
        LCD_DATA_WRITE = c;
        lcd_strobe();
    }
}

void lcd_data(unsigned char c){

    LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h

    LCD_DATA_WRITE &= ~OUTPUT_DATA;
    LCD_RS(1);
    if (LCD_MODE == FOURBIT_MODE)
    {
            LCD_DATA_WRITE &= ~OUTPUT_DATA;
            LCD_DATA_WRITE |= ((c >> 4) & 0x0F) << LCD_DATA_OFFSET;
            lcd_strobe();
            LCD_DATA_WRITE &= (0xF0 << LCD_DATA_OFFSET) | (0xF0 >> 8 - LCD_DATA_OFFSET);
            LCD_DATA_WRITE &= ~OUTPUT_DATA;
            LCD_DATA_WRITE |= (c & 0x0F) << LCD_DATA_OFFSET;
            lcd_strobe();
    }
    else
    {
            LCD_DATA_WRITE = c;
            lcd_strobe();
    }

    LCD_RS(0);
}

void lcd_puts(const char * s){

    while(*s)
        lcd_data(*s++);
}

void lcd_init(){

    char init_value;

    if (LCD_MODE == FOURBIT_MODE) init_value = 0x3 << LCD_DATA_OFFSET;
    else init_value = 0x3F;

    LCD_RS_DIR(OUTPUT_PIN);
    LCD_EN_DIR(OUTPUT_PIN);
    LCD_RW_DIR(OUTPUT_PIN);
    LCD_DATA_DIR |= OUTPUT_DATA;
    LCD_RS(0);
    LCD_EN(0);
    LCD_RW(0);

    DelayMs(15);
    LCD_DATA_WRITE &= ~OUTPUT_DATA;
    LCD_DATA_WRITE |= init_value;
    lcd_strobe();
    DelayMs(5);
    LCD_DATA_WRITE &= ~OUTPUT_DATA;
    LCD_DATA_WRITE |= init_value;
    lcd_strobe();
    DelayUs(200);
    LCD_DATA_WRITE &= ~OUTPUT_DATA;
    LCD_DATA_WRITE |= init_value;
    lcd_strobe();

    if (LCD_MODE == FOURBIT_MODE){
        LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
        LCD_DATA_WRITE |= 0x2 << LCD_DATA_OFFSET; // Set 4-bit mode
        lcd_strobe();
        lcd_cmd(0x28); // Function Set
    }
    else lcd_cmd(0x3C); // 8bit,two lines,5x10 dots

    lcd_cmd(0xF); //Display On, Cursor On, Cursor Blink
    lcd_cmd(0x1); //Display Clear
    lcd_cmd(0x6); //Entry Mode
    lcd_cmd(0x80); //Initialize DDRAM address to zero
}

void lcd_strobe(){
  LCD_EN(1);
  asm("NOP");
 // asm("NOP");
  LCD_EN(0);
}

void DelayUs(unsigned int cnt){

    unsigned char i;
    for(i=cnt ; i>0 ; i--) asm("nop"); // tha command asm("nop") takes raphly 1usec

}

void DelayMs(unsigned int cnt){

    unsigned char i;
    for(i=cnt ; i>0 ; i--) DelayUs(1000); // tha command asm("nop") takes raphly 1usec

}

void delay(unsigned int t){  //
    volatile unsigned int i;

    for(i=t; i>0; i--);
}


#pragma vector = TIMER0_A0_VECTOR // For delay
__interrupt void TimerA_ISR (void)
{
    //ser_output("Pragma timer0 \r\n");
    StopAllTimers();
    LPM0_EXIT;
}
#pragma vector = TIMER1_A0_VECTOR // For delay
__interrupt void TimerA1_ISR (void)
{
    //ser_output("Pragma timer1 \r\n");
    StopAllTimers();
    LPM0_EXIT;
}
//*********************************************************************
//                         ADC10 ISR
//*********************************************************************
#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
    __bic_SR_register_on_exit(CPUOFF);
}

//*********************************************************************
//                           TX ISR
//*********************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0TX_VECTOR))) USCI0TX_ISR (void)
#else
#error Compiler not supported!
#endif
{
    //UCA0TXBUF = '=';                  //to test
    //ser_output("entered key: ");
    //UCA0TXBUF = UCA0RXBUF;                  //to test
    //ser_output("   => \r\n");
    IE2 &= ~UCA0TXIE;                       // Disable USCI_A0 TX interrupt
}


//*********************************************************************
//                         RX ISR
//*********************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
        if(UCA0RXBUF == '1' && st4_flag == 0 && script_in_flag==0){
        state = state1;
        //ser_output("changed to state1 \r\n");
        IE2 |= UCA0TXIE;
    }
    else if(UCA0RXBUF == '2' && st4_flag == 0 && script_in_flag==0){
        state = state2;
        //ser_output("changed to state2 \r\n");
        IE2 |= UCA0TXIE;
    }
    else if(UCA0RXBUF == '3' && st4_flag == 0 && script_in_flag==0){
        state = state3;
        //ser_output("changed to state3 \r\n");
        IE2 |= UCA0TXIE;
    }
    else if(UCA0RXBUF == '4' && st4_flag == 0 && script_in_flag==0){
        state = state4;
        //ser_output("changed to state4 \r\n");
        IE2 |= UCA0TXIE;
    }
    else if(UCA0RXBUF == '5' && st4_flag == 0 && script_in_flag==0){
        state = state5;
        //ser_output("changed to state5 \r\n");
        IE2 |= UCA0TXIE;
    }
    else if(UCA0RXBUF == '6' && st4_flag == 0 && script_in_flag==0){
        state = state6;
        //ser_output("changed to state6 \r\n");
        IE2 |= UCA0TXIE;
    }
    else if(UCA0RXBUF == '7' && st4_flag == 0 && script_in_flag==0){
        state = state7;
        //ser_output("changed to state7 \r\n");
        IE2 |= UCA0TXIE;
    }
    else if(UCA0RXBUF == '8' && st4_flag == 0 && script_in_flag==0){
        state = state8;
        //ser_output("changed to state8 \r\n");
        IE2 |= UCA0TXIE;
    }
    else if(UCA0RXBUF == '9' && st4_flag == 0 && script_in_flag==0){
            state = state9;
            //ser_output("changed to state8 \r\n");
            IE2 |= UCA0TXIE;
        }
    if(UCA0RXBUF == 's'  && st4_flag == 0 && script_in_flag==0){ //collaborate start
                state = state9;
                state9mode = 's';
                IE2 |= UCA0TXIE;
            }
    if(UCA0RXBUF == 'c'  && st4_flag == 0 && script_in_flag==0){ //collaborate read
            state = state9;
            state9mode = 'c';
            IE2 |= UCA0TXIE;
        }

    else if(UCA0RXBUF == 'r' && st4_flag == 0 && script_in_flag==0){
            st4_flag = 1;
            //ser_output("waiting for input new deg \r\n");
            IE2 |= UCA0TXIE;
        }
    else if(st4_flag == 1 && script_in_flag==0){
            getdeg[pd] = UCA0RXBUF; //got deg[0] , deg[1] , deg[2]
            pd++;
            //ser_output("got digit \r\n");
            if (pd==3){
                getdeg[3]='\n';
                st4_flag = 0;
                pd = atoi(getdeg);
                motormove(pd,30);
                pd = 0;
                //ser_output("got new deg: ");ser_output(getdeg);ser_output(" \r\n");
            }
            IE2 |= UCA0TXIE;
        }
    else if(UCA0RXBUF == 'a'  && st4_flag == 0 && script_in_flag==0){
        state5mode = 'a';
        script_in_flag=1;
        IE2 |= UCA0TXIE;
        }
    else if(UCA0RXBUF == 'b'  && st4_flag == 0 && script_in_flag==0){
        state5mode = 'b';
        script_in_flag=1;
        IE2 |= UCA0TXIE;
        }
    else if(UCA0RXBUF == 'c'  && st4_flag == 0 && script_in_flag==0){
        state5mode = 'c';
        script_in_flag=1;
        IE2 |= UCA0TXIE;
        }
    else if(UCA0RXBUF == 'd'  && st4_flag == 0 && script_in_flag==0){
            state = state5;
            state5mode = 'd';
            IE2 |= UCA0TXIE;
            }
    else if(UCA0RXBUF == 'e'  && st4_flag == 0 && script_in_flag==0){
                state = state5;
                state5mode = 'e';
                IE2 |= UCA0TXIE;
                }
    else if(UCA0RXBUF == 'f'  && st4_flag == 0 && script_in_flag==0){
                state = state5;
                state5mode = 'f';
                IE2 |= UCA0TXIE;
                }
    else if (script_in_flag==1){

        if(state5mode == 'a' && flash_reset==0){
            set_flash(0x1040);// first script
            flash_reset=1;}
        else if(state5mode == 'b' && flash_reset==0){
            set_flash(0x1080);// second script
            flash_reset=1;}
        else if(state5mode == 'c' && flash_reset==0){
            set_flash(0x1000);// third script overwrite the ldr collab
            flash_reset=1;}
        if (UCA0RXBUF != 'z')
            flash_w_auto(UCA0RXBUF);
        else if(UCA0RXBUF == 'z'){
            script_in_flag=0;
            flash_reset=0;
            state = state8;
            IE2 |= UCA0TXIE;
        }
    }
    switch(lpm_mode){
    case mode0:
        LPM0_EXIT; // must be called from ISR only
        break;
    case mode1:
        LPM1_EXIT; // must be called from ISR only
        break;
    case mode2:
        LPM2_EXIT; // must be called from ISR only
        break;
    case mode3:
        LPM3_EXIT; // must be called from ISR only
        break;
    case mode4:
        LPM4_EXIT; // must be called from ISR only
        break;
    }
}

//-----------------------------------------------------------------------------------
//----------------------------FLASH for LDR calibration------------------------------
//-----------------------------------------------------------------------------------
//PB handler
#pragma vector=PORT2_VECTOR
  __interrupt void PB0_LDR_calib(void){
    delay(debounceVal);
        if(PBsArrIntPend & PB0){
          PBsArrIntPend &= ~PB0;
          __bic_SR_register_on_exit(LPM0_bits); //out from sleep
        }
        /*switch(lpm_mode){
        case mode0:
             LPM0_EXIT; // must be called from ISR only
             break;

        case mode1:
             LPM1_EXIT; // must be called from ISR only
             break;

        case mode2:
             LPM2_EXIT; // must be called from ISR only
             break;

        case mode3:
             LPM3_EXIT; // must be called from ISR only
             break;

        case mode4:
             LPM4_EXIT; // must be called from ISR only
             break;
    }*/
}

  void set_flash(int addr){ //from hannan
      Flash_ptr = (char *) addr;                // Initialize Flash pointer  // 0x1000,0x1040,0x1080
      FCTL1 = FWKEY + ERASE;                    // Set Erase bit
      FCTL3 = FWKEY;                            // Clear Lock bit
      *Flash_ptr = 0;                           // Dummy write to erase Flash segment
      FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation
  }


  void flash_w_addr(char value, int addr){ //from hannan

      Flash_ptr = (char *) addr;
      *Flash_ptr = (char)value;
  }

  void flash_w_auto(char value){ //from hannan
      *Flash_ptr++ = (char)value;               // Write value to flash
  }

  void flash_r_addr(char* value, int addr){
          Flash_ptr = (char *) addr;
         *value=*Flash_ptr++ ;               // read value from flash
    }

  void stop_flash_w(){ //from hannan
       FCTL1 = FWKEY;                            // Clear WRT bit
       FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
   }

  void flash_write_str(char* string, int length){

      int i;
      for (i=0; i<length; i++){
          flash_w_auto(string[i]);
      }
  }

//-----------------------------------------------------------------------------------
//----------------------------FLASH AND SCRIPT---------------------------------------
//-----------------------------------------------------------------------------------

  void scan2(int mode, int aa ,int bb){
      int cnt=0;
      int a=-3;
      int ii=0;
      while(aa<=bb){
          motormove(aa,20);
          if (aa >= 180) break;
          aa = aa + 3;
          aa = aa % 180;
          collectdata(mode);
          StopAllTimers();
      }
      //ser_output("-1\n");
      motormove(0,20);
  }

  //-----------------------------------------

  void count_LCD_inc(unsigned int x){
      lcd_clear();lcd_home();cursor_off; //reset LCD
      lcd_puts("Count up to ");
      sprintf(counter_str2, "%d", x);
      lcd_puts(counter_str2);
      lcd_new_line;
      unsigned int cnt_inc=0;
      while(cnt_inc <= x){
          //itoa(count_up,count_up_str); // DONT work at this compiler???
          sprintf(counter_str2, "%d", cnt_inc);  // Convert the integer to a string using sprintf cuz itoa dont work
          cnt_inc++;
          lcd_puts(counter_str2);
          //__delay_cycles(500);  //0.5 second delay
          timerz(globaldelay);
          lcd_new_line;
      }
  }
  //-------------------------------------
  void count_LCD_dec(unsigned int x){
      lcd_clear();lcd_home();cursor_off; //reset LCD
      lcd_puts("Count dw fr ");
      sprintf(counter_str2, "%d", x);
      lcd_puts(counter_str2);
      lcd_new_line;
      unsigned int cnt_dec=0;
      while(0 <= x){
          //itoa(count_up,count_up_str); // DONT work at this compiler???
          sprintf(counter_str2, "%d", x);  // Convert the integer to a string using sprintf cuz itoa dont work
          x--;
          lcd_puts(counter_str2);
          //__delay_cycles(500);  //0.5 second delay
          timerz(globaldelay);

          lcd_new_line;
          if (x<1) break;
      }
  }

  unsigned int contsonic2(int d){
      motormove(d,30);
      int kk;
      int mm=3;
      char sonicstr[5];
      P2OUT |= 0b01000000;
      __delay_cycles(10);
      P2OUT &= ~0b01000000;
      while (!(P2IN & 0b00000100));
      while ((P2IN &  0b00000100));
      //while (state == state4){
      kk=0;
      P2OUT |= 0b01000000;
      __delay_cycles(10);
      P2OUT &= ~0b01000000;
      while (!(P2IN & 0b00000100));
      while ((P2IN &  0b00000100)){
          kk++;
          __delay_cycles(50);
      }
      ltoa(kk,sonicstr,10);
      if (mm == 2 | mm==3){
          lcd_clear();
          lcd_puts("Telemeter:");lcd_puts(sonicstr);lcd_puts(" cm");
      }
      if (mm == 1 | mm==3){
          //ser_output("continuous sonic "); ser_output(sonicstr);ser_output(" \r\n");
          //ser_output(sonicstr);ser_output("\n");

          timerz(50000);
      }
      //IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
      //enable_interrupts();
      return kk;
  }
