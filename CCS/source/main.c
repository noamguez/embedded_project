#include  "../header/api.h"    		// private library - API layer
#include  "../header/app.h"    		// private library - APP layer
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include <math.h>
enum FSMstate state;
enum SYSmode lpm_mode;
void main(void){
  state = state8;  // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET
  sysConfig();     // Configure GPIO, Stop Timers, Init LCD, UART
  while(1){
	switch(state){
	case state1:
	    scan(1);  //MODE 1:    object detector
	    state = state8;
	    break;
	case state2:
	    contsonic(3); //MODE 2:  telemeter
	    break;
	case state3:
	    scan(3);   //MODE 3:   light sources
	    state = state8;
		break;
	case state4:
	    scan(4);  //MODE 4:    object and light detector
        state = state8;
		break;
    case state5:
        //should be script mode
        if(state5mode=='d')
            run_script(1);
        else if(state5mode=='e')
            run_script(2);
        else if(state5mode=='f')
            run_script(3);
        state = state8;
        break;
    case state6:
        //----------------------------------------------------------------------------------------------------------------
        //set_flash(0x1020);
        //flash_write_str("test",4);
        break;
    case state7:
        motormove(90,30);
        calibrate_LDR();
        state = state8;
        break;
    case state9:
        send_colaboration();
        state = state8;
        break;
    case state8: //sleep
        sleep();
        break;
	}
  }
}


  
  
  
  

