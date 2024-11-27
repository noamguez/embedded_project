#ifndef _halGPIO_H_
#define _halGPIO_H_

#include  "../header/bsp.h"    		// private library - BSP layer
#include  "../header/app.h"    		// private library - APP layer

char state9mode;        //s for start c to colab read
char state5mode;
char state6mode;
extern enum FSMstate state;   // global variable
extern enum SYSmode lpm_mode; // global variable

extern double t2;
extern unsigned int deg;
extern char getdeg[4];
extern int pd;
extern void sysConfig(void);
extern int globaldelay;



extern void ser_output(char *str); // writes string to uart
extern char smsg[5];
extern unsigned int info[4];



extern char Msg[5];
extern void delay(unsigned int);
extern void enterLPM(unsigned char);
extern void enable_interrupts();
extern void disable_interrupts();


extern void timerz(int a);

//flash

extern void set_flash(int addr); //prepare the writing for flash
extern void flash_w_addr(char value, int addr); //write with addr
extern void flash_w_auto(char value); //write and progrees a place automaticly
extern void flash_r_addr(char* value, int addr); //read the char
extern void stop_flash_w(); //stops the writing
extern void flash_write_str(char* string, int length);
extern void scan2(int mode, int aa ,int bb); //for script func 07
extern void count_LCD_inc(unsigned int x); //for script func 01
extern void count_LCD_dec(unsigned int x); //for script func 02
extern unsigned int contsonic2(int d); //for script func 06

extern __interrupt void PBs_handler(void);
extern __interrupt void PBs_handler_P2(void);
extern __interrupt void Timer_A0(void);
extern __interrupt void Timer_A1(void);
extern __interrupt void USCI0RX_ISR(void);
extern __interrupt void USCI0TX_ISR(void);

#endif

// #define CHECKBUSY    1  // using this define, only if we want to read from LCD

#ifdef CHECKBUSY
    #define LCD_WAIT lcd_check_busy()
#else
    #define LCD_WAIT DelayMs(5)
#endif

/*----------------------------------------------------------
  CONFIG: change values according to your port pin selection
------------------------------------------------------------*/
#define LCD_EN(a)   (!a ? (P2OUT&=~0X02) : (P2OUT|=0X02)) // P2.5 is lcd enable pin
#define LCD_EN_DIR(a)   (!a ? (P2DIR&=~0X02) : (P2DIR|=0X02)) // P2.5 pin direction

#define LCD_RS(a)   (!a ? (P2OUT&=~0X08) : (P2OUT|=0X08)) // P2.6 is lcd RS pin
#define LCD_RS_DIR(a)   (!a ? (P2DIR&=~0X08) : (P2DIR|=0X08)) // P2.6 pin direction

#define LCD_RW(a)   (!a ? (P2OUT&=~0X20) : (P2OUT|=0X20)) // P2.7 is lcd RW pin
#define LCD_RW_DIR(a)   (!a ? (P2DIR&=~0X20) : (P2DIR|=0X20)) // P2.7 pin direction

#define LCD_DATA_OFFSET 0x04 //data pin selection offset for 4 bit mode, variable range is 0-4, default 0 - Px.0-3, no offset


/*---------------------------------------------------------
  END CONFIG
-----------------------------------------------------------*/
#define FOURBIT_MODE    0x0
#define EIGHTBIT_MODE   0x1
#define LCD_MODE        FOURBIT_MODE

#define OUTPUT_PIN      1
#define INPUT_PIN       0
#define OUTPUT_DATA     (LCD_MODE ? 0xFF : (0x0F << LCD_DATA_OFFSET))
#define INPUT_DATA      0x00

#define LCD_STROBE_READ(value)  LCD_EN(1), \
                asm("nop"), asm("nop"), \
                value=LCD_DATA_READ, \
                LCD_EN(0)

#define lcd_cursor(x)       lcd_cmd(((x)&0x7F)|0x80)
#define lcd_clear()         lcd_cmd(0x01)
#define lcd_putchar(x)      lcd_data(x)
#define lcd_goto(x)         lcd_cmd(0x80+(x))
#define lcd_cursor_right()  lcd_cmd(0x14)
#define lcd_cursor_left()   lcd_cmd(0x10)
#define lcd_display_shift() lcd_cmd(0x1C)
#define lcd_home()          lcd_cmd(0x02)
#define cursor_off          lcd_cmd(0x0C)
#define cursor_on           lcd_cmd(0x0F)
#define lcd_function_set    lcd_cmd(0x3C) // 8bit,two lines,5x10 dots
#define lcd_new_line        lcd_cmd(0xC0)

extern void lcd_cmd(unsigned char);
extern void lcd_data(unsigned char);
extern void lcd_puts(const char * s);
extern void lcd_init();
extern void lcd_strobe();
extern void DelayMs(unsigned int);
extern void DelayUs(unsigned int);

extern __interrupt void PB0_LDR_calib(void); //PB handler
/*
 *  Delay functions for HI-TECH C on the PIC18
 *
 *  Functions available:
 *      DelayUs(x)  Delay specified number of microseconds
 *      DelayMs(x)  Delay specified number of milliseconds
*/






