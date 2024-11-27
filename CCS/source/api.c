#include  "../header/api.h"         // private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
#include "stdio.h"
#include "stdlib.h"
//-------------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
/////////// ALL FUNCTION DESCRIPTION IN H//////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

char smsg[5];
unsigned int info[4];
double t2 = 40;

int collabed=0;
char counter_str[5];    //to print counter
unsigned int counter = 0;
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
void count_LCD(){
    lcd_clear();lcd_home();cursor_off; //reset LCD
    lcd_puts("State 2:Counting");
    lcd_new_line;
    while(state==state7){
        //itoa(count_up,count_up_str); // DONT work at this compiler???
        sprintf(counter_str, "%d", counter);  // Convert the integer to a string using sprintf cuz itoa dont work
        counter++;
        lcd_puts(counter_str);
        //__delay_cycles(500);  //0.5 second delay
        timerz(65000);
        timerz(65000);
        lcd_new_line;
    }
}
///------------------------


void ser_output(char *str){
    while(*str != 0) {
        while (!(IFG2&UCA0TXIFG));
        UCA0TXBUF = *str++;
        }
}

unsigned int contsonic(int m){
    int kk;
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
    ltoa(kk,sonicstr);
    if (m == 2 | m==3){
        lcd_clear();
        lcd_puts("Telemeter:");lcd_puts(sonicstr);lcd_puts(" cm");
    }
    if (m == 1 | m==3){
        //ser_output("continuous sonic "); ser_output(sonicstr);ser_output(" \r\n");
        ser_output(sonicstr);ser_output("\n");

        timerz(50000);
    }
    //IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
    //enable_interrupts();
    return kk;
}
//-------------------------------------------------------------------------------------------

//================================The scan===========================================
void scan(int mode){
    int cnt=0;
    int a=-3;
    int ii=0;
    while(state==state1 || state==state3 || state==state4){
        a = a + 3;
        motormove(a,20);
        if (a >= 180) break;
        a = a % 180;
        collectdata(mode);
        StopAllTimers();
    }
    ser_output("-1\n");
    motormove(0,20);
}
//================================The motor===========================================
void motormove(int a, int b){
    int ii;
    deg = a;
    a = 40 + 1.2*a;
    for (ii=0;ii<b;ii++){
        P2OUT = P2OUT | 0b00010000;
        timerz(1 + a);
        P2OUT = P2OUT & 0b11101111;
        timerz(3099-a);
        }
}
//=================Scan every thing but send ONLY needed data by mode=========

void collectdata(int mode){
    info[3]=contsonic(0);
    info[1]=LDR_ADC(1);
    info[2]=LDR_ADC(2);
    info[0]=deg;


    //==================used to test in terminal=======================
    /*
    //ser_output("collected info: deg = ");
    ltoa(info[0],smsg);ser_output(smsg); //degree
    ser_output(" \r\n");
    //ser_output(" LDR1 = ");
    ltoa(info[1],smsg);ser_output(smsg); //ldr1
    //ser_output(" LDR2 = ");
    ser_output(" \r\n");// needed to catch in python
    ltoa(info[2],smsg);ser_output(smsg); //ldr2
    //ser_output(" Dist = ");
    ser_output(" \r\n");
    ltoa(info[3],smsg);ser_output(smsg); //dist
    ser_output(" \r\n");
    */

    //=================send only necessary data to UART================
    if (mode == 1){ //without LDR
        ltoa(info[0],smsg);ser_output(smsg);ser_output(",");
        //ltoa(info[1],smsg);ser_output(smsg);ser_output(",");
        //ltoa(info[2],smsg);ser_output(smsg);ser_output(",");
        ltoa(info[3],smsg);ser_output(smsg);ser_output("\n");
    }
    if (mode == 3){ //without sonic
        ltoa(info[0],smsg);ser_output(smsg);ser_output(",");
        ltoa(info[1],smsg);ser_output(smsg);ser_output(",");
        ltoa(info[2],smsg);ser_output(smsg);ser_output("\n");
        //ltoa(info[3],smsg);ser_output(smsg);ser_output("\n");
    }
    if (mode == 4){ //all
        ltoa(info[0],smsg);ser_output(smsg);ser_output(",");
        ltoa(info[1],smsg);ser_output(smsg);ser_output(",");
        ltoa(info[2],smsg);ser_output(smsg);ser_output(",");
        ltoa(info[3],smsg);ser_output(smsg);ser_output("\n");
    }


    //============optional, show on LCD, no unnecessary UART uses===========
    lcd_clear();
    lcd_puts("Deg=");ltoa(info[0],smsg);lcd_puts(smsg);
    lcd_puts(" Dist=");ltoa(info[3],smsg);lcd_puts(smsg);
    lcd_new_line;
    lcd_puts("LDR:");ltoa(info[1],smsg);lcd_puts(smsg);lcd_puts(" & ");ltoa(info[2],smsg);lcd_puts(smsg);
    timerz(500);
}


unsigned int LDR_ADC(unsigned int a){
        ADC_config();

        ADC10CTL0 |= ENC + ADC10SC;             // Start sampling
        __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
        ADC10CTL0 &= ~ADC10ON; // Don't get into interrupt
        unsigned int volt = ADC10MEM;

        sprintf(counter_str, "%d", volt);  // Convert the integer to a string using sprintf cuz itoa dont work
        if (a==0){
            lcd_clear();
            lcd_puts("LDR meas:");
            lcd_puts(counter_str);
            lcd_puts(" mV");
        }
        if (a==0){
            ser_output("LDR right meas:");
            ser_output(counter_str);
            //ser_output(" \r\n");
        }
        //=================================== other LDR==============
        ADC10CTL0 = ADC10SHT_2 + ADC10ON+ SREF_0 + ADC10IE;
        ADC10CTL1 = INCH_0 + ADC10SSEL_3;
        ADC10AE0 |= BIT0;
        //==============================================================
        ADC10CTL0 |= ENC + ADC10SC;             // Start sampling
        __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
        ADC10CTL0 &= ~ADC10ON; // Don't get into interrupt
        unsigned int volt2 = ADC10MEM;
        sprintf(counter_str, "%d", volt2);  // Convert the integer to a string using sprintf cuz itoa dont work
        if (a==0){
            lcd_new_line;
            lcd_puts("LDR meas:");
            lcd_puts(counter_str);
            lcd_puts(" mV");
        }
        if (a==0) {ser_output(" LDR left  meas:");
            ser_output(counter_str);
            ser_output(" \r\n");
            timerz(500);
        }
        if (a==1) return volt;
        if (a==2) return volt2;

}
//-------------------------------------------------------------------------------------------
void clear_counters(){
    disable_interrupts();
    lcd_clear();lcd_home();cursor_off; //reset LCD
    counter = 0;
    enable_interrupts();
    state = state8; //and go sleep
}
void sleep(){
    lcd_clear();lcd_home();cursor_off; //reset LCD
    lcd_puts("State 8:Sleeping");
            IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
            __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}


//-----------------------------------------------------------------------------------
//----------------------------FLASH AND SCRIPT---------------------------------------
//-----------------------------------------------------------------------------------

void calibrate_LDR(){
    lcd_clear();
    lcd_init();
    char ldrval[4];
    char cm[3];
    int ldr1, ldr2,ldrav;
    char c; //for test
    char *cpoint=&c; //for test
    int i; //for test
    int numofcheck=0;
    delay(500000);
    FlashConfig();
    set_flash(0x1000);
    for(numofcheck=0; numofcheck<10; numofcheck++){
        lcd_clear();
        lcd_init();
        lcd_puts("waiting PB0 ");
        sprintf(cm, "%d", 5 + numofcheck*5);
        lcd_cmd(0xC0);
        lcd_puts("put at: ");
        lcd_puts(cm);
        lcd_puts(" cm");
        enable_interrupts();
        enterLPM(lpm_mode);  // expect to PB0 Press
        disable_interrupts();

        if(state != state7) //for now the colab state
            break;

        lcd_clear();
        lcd_init();
        ldr1=LDR_ADC(1);//mesured
        ldr2=LDR_ADC(2);
        ldrav=(ldr1+ldr2)/2;
        sprintf(ldrval, "%d", ldrav);
        //set_flash(0x1000+4*numofcheck);
        flash_write_str(ldrval,4);


        /*test it works
        lcd_puts(ldrval);
        lcd_cmd(0xC0);
        for(i=0;i<sizeof(ldrval); i++){
            flash_r_addr(cpoint,0x1000+4*numofcheck+i);
            lcd_data(c);
            }
        enable_interrupts();
        enterLPM(lpm_mode);  // expect to PB0 Press
        disable_interrupts();*/
    }
    /*test
     * for(numofcheck=0; numofcheck<10; numofcheck++){
        lcd_clear();
        lcd_init();
        sprintf(cm, "%d", 5 + numofcheck*5);
        lcd_puts("at: ");
        lcd_puts(cm);
        lcd_puts(" cm is");
        lcd_cmd(0xC0);
        for(i=0;i<sizeof(ldrval); i++){
            flash_r_addr(cpoint,0x1000+4*numofcheck+i);
            lcd_data(c);
            }
        enable_interrupts();
        enterLPM(lpm_mode);  // expect to PB0 Press
        disable_interrupts();
    }*/
    lcd_clear();
    lcd_init();
    lcd_puts("collaboration done!");
    collabed =1;
    delay(500000);
    enable_interrupts();
    state = state8; //and go sleep
    }

void send_colaboration(){
    /*if (collabed != 1)
        state= state8;*/ // need to create break if there is no collab
    char ldrval[5];
    char c;
    char *cpoint=&c;
    int i,numofcheck;
    while(state==state9){
        lcd_clear();
        lcd_init();
        lcd_puts("chill!");
        for (numofcheck=0; numofcheck<10; numofcheck++){
            for(i=0;i<sizeof(ldrval)-1; i++){
                flash_r_addr(cpoint,0x1000+4*numofcheck+i);
                ldrval[i]=c;
                }
            ser_output(ldrval);
            ser_output(" \r\n");// needed to catch in python
        }
        delay(50000000);
        state= state8;
    }

}

void run_script(int script){
    int address;
    if(script == 1){
        address=0x1040;
        }// first script
    else if(script == 2){
        address=0x1080;
        }// second script
    else if(script == 3){
        address=0x1000;
        }// third script overwrite the ldr collab
    char line[7];
    char c;
    char *cpoint=&c;
    int i, numoffunc=0;
    do
    {
        for(i=0;i<sizeof(line)-1; i++){
            flash_r_addr(cpoint,address+6*numoffunc+i);
            line[i]=c;
            }
        numoffunc++;
    }
    while(func(line) != 1 && numoffunc<10);
    state=state8;
}


int func(char hexString[]){
    int decimals[3];
    //hexStringToDecimal(hexString, decimals);
    //printf("%d, %d, %d\n", decimals[0], decimals[1], decimals[2]);
    int i,j;
    char pair[3];

    for (i = 0; i < 6; i += 2) {
        memcpy(pair, hexString + i, 2);
        pair[2] = '\0';

        decimals[i / 2] = (int)strtol(pair, NULL, 16);
    }
    int a = decimals[1];
    int b = decimals[2];
    if (decimals[0]==1){
        count_LCD_inc(a);
        return 0;}
    if (decimals[0]==2){
        count_LCD_dec(a);
        return 0;}
    if (decimals[0]==3){
        //lcd_puts("rra_lcd(%d);");
        return 0;}
    if (decimals[0]==4){
        globaldelay= a*(65000/50);
        return 0;}
    if (decimals[0]==5){
        lcd_clear();
       return 0;}
    if (decimals[0]==6){
        for (j=0; j<5; j++)
            contsonic2(a);
        return 0;}
    if (decimals[0]==7){
        scan2(3,a,b);
        return 0;}
    if (decimals[0]==8){
        lcd_puts("sleep();");
        return 1;}
}
