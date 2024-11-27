#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer

extern void count_LCD(); // COUNTING IN LCD
extern unsigned int LDR_ADC(); // MESURING THE LDR
extern void clear_counters();
extern void collectdata(int mode); // COLLECT THE DATA DEPENDS ON MODE: 1. DEGREE AND OBJECT 2. DEGREE AND LDR 4 DEGREE AND LDE AND OBJECT
extern void ser_output(char *str); // writes string to uart
extern void scan(int mode); //USING COLLECTDATA WITH MOTOR MOVE TOGETHER
extern unsigned int sonicshot(int m); // SINGLE MESSURE
extern unsigned int contsonic(int m);// TELEMETER OR CONTINU MESURE
extern void motormove(int a,int b); // A IS DEGREE AND B THE TIME FOR MOVEMENT
extern void sleep();   //6
extern void calibrate_LDR(); // CALIBRATE THE LDR ARRRAY TO FLASH
extern void send_colaboration(); // SENDING TO PYTHON FROM FLASH
extern void run_script(int script); // RUNS THE SCRIPT FROM FLASJ USING FUNC
extern int func(char hexString[]); // READS THE LINE OF SCRIPT AND PERFORM
#endif







