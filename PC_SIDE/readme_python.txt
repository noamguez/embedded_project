
`main()`: This is the main function that creates the GUI using PySimpleGUI and handles the events from different buttons, executing the corresponding functions for each option.

button functions:
button 1:
`object_detector()`: this function sends the mcu to function the motor and ultrasonic to find objects in the scanning range.
                     draw all it finds in the design in python 
                     press 'q' key is pressed to stop the application.
button 2:
`light_detector()`: this function sends the mcu to function the motor and LDR to find light source in the scanning range.
                    draw all it finds in the design in python 
                    press 'q' key is pressed to stop the application.
sub function:                        
`find_in_array_ldr(value)`: This function is used to find the closest value in the `LDR_ARRAY` to the given `value` (distance for LDR).
	
button 3:
`object_and_lights()`: this function sends the mcu to function the motor and both ultrasonic and LDR to find objects and light source in the scanning range.
                       draw all it finds in the design in python 
                       press 'q' key is pressed to stop the application.

button 4:                          
`telemeter()`: this functions mesures dinamicly the distance from an object in a given angle.
	       press 'q' key is pressed to stop the application.

button 5:
option 1,2,3
`send_script1()`, `send_script2()`, `send_script3()`: These functions are supposed to send scripts to the flash memory. They are currently implemented as pop-up messages.

option 4,5,6
`run_script1()`, `run_script2()`, `run_script3()`: These functions are supposed to run scripts from the flash memory. They are currently implemented as pop-up messages.

sub function:
convert_script(), converts the given high level code to a simple one to send to the mcu

button 6:  
8. `measure_and_insert_to_flash()`: This function is used for collaboration with LDR. It writes 's' to the UART and reads values to fill the `LDR_ARRAY`.

`take_flash_value_to_python()`: This function is used for collaboration with LDR. It writes 'c' to the UART and reads values from the flash memory to fill the `LDR_ARRAY`.

UART functions:
`define_uart()`: This function sets up the UART (Serial) communication.

`read_uart()`: This function reads data from the UART (Serial).

`write_uart()`: This function writes data to the UART (Serial).

Dinamic plot Funcions:
`drawPlot(whichPlot)`: This function is used to create a polar plot. The plot color and marker style depend on the `whichPlot` parameter.

`progressSensor(theta)`: This function is used to progress the sensor to a specified angle.

`addDot(angle, dist)`: This function is used to add a point (dot) to the polar plot at a specified angle and distance.
