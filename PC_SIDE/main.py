import serial as ser
import time
from matplotlib import pyplot as plt
import matplotlib
import PySimpleGUI as sg

matplotlib.use('TkAgg')
import numpy as np
import keyboard

LDR_ARRAY = [0] * 51  # definition of the global ldr discance array from 0 to 50 cm

'''
FUNCTION EXPLAINED FOR README.TXT

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


'''


###############################################################################
###########   OPTION 1 OBJECT DETECTOR FUNCTION ###############################
###############################################################################

def object_detector():
    # Replace this with your object detector code
    print("Object Detector is running...")
    # """
    drawPlot("objects")
    define_uart()
    write_uart('1')  # mod in C
    # """
    while True:
        try:
            print("--------------------------------------------------------")
            # """
            integers = [int(num) for num in read_uart().split(',')]
            print("all data gathered:", integers)
            if integers[0] == -1:
                write_uart('8')
                s.close()
                # plt.close('all')
                print("Full scan completed!")
                break
            degree = integers[0]
            # ldr1 = integers[1]
            # ldr2 = integers[2]
            dist_mesured = integers[1]
            progressSensor(degree)

            # NEED TO MESSURE AND CHECH MIDDLE OF LDR
            # DISTANCE OF LDR USE find_in_array_ldr(value) AFTER COLABB

            if dist_mesured > 10:
                addDot(degree, dist_mesured, markerrsize=10, markerr='s', colorr='yellow')
            # """
            if keyboard.is_pressed('q'):
                plt.close('all')
                print("User need to Quit the application")
                # """
                break
        except KeyboardInterrupt:
            # '''
            plt.close('all')
            print('Keyboard Interrupt')
            # '''
            break


###############################################################################
###########   OPTION 2 LIGHT DETECTOR FUNCTION ################################
###############################################################################
def light_detector():
    # Check if LDR_ARRAY has non-zero values
    if any(val != 0 for val in LDR_ARRAY[1:51]):
        # Replace this with your light detector code
        print("Light Detector is running...")
        # """
        drawPlot("objects")
        define_uart()
        write_uart('3')  # mod in C
        str = read_uart()  # FOR WHAT YOU SENT WHEM GETTING IN THE MOD IN C
        print(str)
        # """
        while True:
            try:
                print("--------------------------------------------------------")
                # """
                integers = [int(num) for num in read_uart().split(',')]
                print("all data gathered:", integers)
                if integers[0] == -1:
                    write_uart('8')
                    s.close()
                    # plt.close('all')
                    print("Full scan completed!")
                    break
                degree = integers[0]
                ldr1 = integers[1]
                ldr2 = integers[2]
                dist_mesured = find_in_array_ldr ((ldr1 + ldr2)/2)
                progressSensor(degree)

                # NEED TO MESSURE AND CHECH MIDDLE OF LDR
                # DISTANCE OF LDR USE find_in_array_ldr(value) AFTER COLABB

                if dist_mesured > 10 and dist_mesured < 50:
                    if abs(ldr1-ldr2)<100:
                        addDot(degree, dist_mesured, markerrsize=10, markerr='*', colorr='red')
                    else:
                        addDot(degree, dist_mesured, markerrsize=10, markerr='s', colorr='yellow')
                # """
                if keyboard.is_pressed('q'):
                    # """
                    # TRYING TO CHANGE THE STATE IN C!!!!!!!
                    if True:  # trying to break the 3 mod
                        s.reset_input_buffer()
                        s.reset_output_buffer()
                        enableTX = True
                        enableRX = False
                        write_uart('1')
                        write_uart('8')
                        print("YEAH OR FUCK?")
                    # THE IF TRUE DOESNT WORK

                    plt.close('all')
                    print("User need to Quit the application")
                    # """
                    break
            except KeyboardInterrupt:
                # '''
                plt.close('all')
                print('Keyboard Interrupt')
                # '''
                break
    else:
        print("Need to make collaboration first.")


def find_in_array_ldr(value):  # distance for LDR
    i = 5
    gap = 8000
    smalest_gap_int = 0
    while (i < 51):
        if (abs(value - LDR_ARRAY[i]) < gap):
            smalest_gap_int = i
            gap = abs(value - LDR_ARRAY[i])
            print(gap)
        i = i + 1

    return smalest_gap_int


###############################################################################
###########   OPTION 3 OBJECT & LIGHT FUNCTION ################################
###############################################################################
def object_and_lights():
    # Check if LDR_ARRAY has non-zero values
    if any(val != 0 for val in LDR_ARRAY[1:51]):
        # Replace this with your object and light detector code
        print("Object and Light Detector is running...")
        # """
        drawPlot("objects")
        define_uart()
        write_uart('4')
        write_uart('4')  # mod in C
        str = read_uart()  # FOR WHAT YOU SENT WHEM GETTING IN THE MOD IN C
        print(str)
        # """
        while True:
            try:
                print("--------------------------------------------------------")
                # """

                integers = [int(num) for num in read_uart().split(',')]
                print("all data gathered:", integers)
                if integers[0] == -1:
                    write_uart('8')
                    s.close()
                    # plt.close('all')
                    print("Full scan completed!")
                    break
                degree = integers[0]
                ldr1 = integers[1]
                ldr2 = integers[2]
                dist_mesured = integers[3]
                dist_mesured_ldr = find_in_array_ldr ((ldr1 + ldr2)/2)
                progressSensor(degree)

                # NEED TO MESSURE AND CHECH MIDDLE OF LDR
                # DISTANCE OF LDR USE find_in_array_ldr(value) AFTER COLABB

                if dist_mesured_ldr < 1000:
                    if abs(ldr1 - ldr2) < 100 and ldr1 < 800 and ldr2 < 800:
                        addDot(degree, dist_mesured_ldr, "red", markerr='*', markerrsize=20)
                    elif ldr1 < 800 or ldr2 < 800:
                        addDot(degree, dist_mesured_ldr, "orange", markerrsize=15)
                    elif ldr1 < 900 or ldr2 < 900:
                        addDot(degree, dist_mesured_ldr, "yellow", markerrsize=10)
                if dist_mesured >10:
                    addDot(degree, dist_mesured)
                # """
                if keyboard.is_pressed('q'):
                    # """
                    # TRYING TO CHANGE THE STATE IN C!!!!!!!
                    if True:  # trying to break the 3 mod
                        s.reset_input_buffer()
                        s.reset_output_buffer()
                        enableTX = True
                        enableRX = False
                        write_uart('1')
                        write_uart('8')
                        print("YEAH OR FUCK?")
                    # THE IF TRUE DOESNT WORK

                    plt.close('all')
                    print("User need to Quit the application")
                    # """
                    break
            except KeyboardInterrupt:
                # '''
                plt.close('all')
                print('Keyboard Interrupt')
                # '''
                break
    else:
        print("Need to make collaboration first.")


###############################################################################
###########   OPTION 4 TELEMETR FUNCTION ######################################
###############################################################################
def telemeter(angle):
    # print("Telemeter is running...2")
    drawPlot("objects")
    define_uart()

    char_set = str(angle).zfill(3)  # get new deg
    write_uart('r')  # get new deg
    write_uart(char_set[0])  # get new deg
    write_uart(char_set[1])  # get new deg
    write_uart(char_set[2])  # get new deg

    write_uart('8')
    time.sleep(1)
    write_uart('2')

    #str4 = read_uart()
    #print(str4)
    while True:
        try:
            str4 = read_uart()  # FOR WHAT YOU SENT WHEM GETTING IN THE MOD IN C
            dist = int(str4)
            print("received distance: <", dist, ">")
            addDot(angle, dist, markerr='s', markerrsize=20)
            plt.pause(0.001)
            # here need to make this functions

            if keyboard.is_pressed('q'):
                print("User need to Quit the application")
                break


        except KeyboardInterrupt:
            print('Keyboard Interrupt')
            write_uart('8')
            s.reset_input_buffer()
            s.reset_output_buffer()
            plt.close('all')
            s.close()
            break


###############################################################################
###########   OPTION 5 SCRIPT FUNCTIONS #######################################
###############################################################################


def convert_script():
    print("ha")
    # need to convert the script options to send


def send_script1():
    define_uart()
    write_uart('a')
    s.close()
    define_uart()
    file_path = r"C:\Users\User\OneDrive\Desktop\dcs project\scripts\script1_code.txt"
    read_and_translate_file(file_path)
    write_uart('z')
    s.close()
def send_script2():
    define_uart()
    write_uart('b')
    s.close()
    define_uart()
    file_path = r"C:\Users\User\OneDrive\Desktop\dcs project\scripts\script1_code.txt"
    read_and_translate_file(file_path)
    write_uart('z')
    s.close()

def send_script3():
    define_uart()
    write_uart('c')
    s.close()
    define_uart()
    file_path = r"C:\Users\User\OneDrive\Desktop\dcs project\scripts\script1_code.txt"
    read_and_translate_file(file_path)
    write_uart('z')
    s.close()


def run_script1():
    define_uart()
    write_uart('d')
    s.close()


def run_script2():
    define_uart()
    write_uart('e')
    s.close()


def run_script3():
    define_uart()
    write_uart('f')
    s.close()


def opcode_to_hex(opcode):
    opcodes_dict = {
        "inc_lcd": "01",
        "dec_lcd": "02",
        "rra_lcd": "03",
        "set_delay": "04",
        "clear_lcd": "05",
        "servo_deg": "06",
        "servo_scan": "07",
        "sleep": "08",
    }
    return opcodes_dict.get(opcode, "00")

def int_to_hex(value):
    return format(value, '02X').zfill(2)

def line_to_hex(line):
    parts = line.strip().split()
    opcode = parts[0]
    if opcode == "servo_deg":
        arg1 = int_to_hex(int(parts[1]))
        return opcode_to_hex(opcode) + arg1 + "00"
    elif opcode == "servo_scan":
        arg1, arg2 = parts[1].split(',')
        arg1 = int_to_hex(int(arg1))
        arg2 = int_to_hex(int(arg2))
        return opcode_to_hex(opcode) + arg1 + arg2
    else:
        if len(parts) > 1:
            arg1 = int_to_hex(int(parts[1]))
            return opcode_to_hex(opcode) + arg1 + "00"
        else:
            return opcode_to_hex(opcode) + "0000"

def translate_file(file_path):
    translated_lines = []
    with open(file_path, 'r') as file:
        for line in file:
            hex_code = line_to_hex(line)
            translated_lines.append(hex_code.ljust(6, '0'))
    return "".join(translated_lines)


def read_and_translate_file(file_path):

    translated_code = translate_file(file_path)
    for r in translated_code:
        write_uart(r)



###############################################################################
###########   OPTION 6 COLLAB LDR FUNCTIONS ###################################
###############################################################################
# Collaborate LDR Functions
def measure_and_insert_to_flash():  # for now this is state9 option s
    define_uart()
    write_uart('7')
    s.close()


def take_flash_value_to_python():
    print("Collaborate LDR: Take Flash Value to Python is running...")
    global LDR_ARRAY  # 10 values from 5 to 50 cm to mesure light distance
    define_uart()
    write_uart('9')
    i = 0
    while (i < 10):
        LDR_ARRAY[5 * i + 5] = int(read_uart()[0:4])
        if i == 0:  # filling the lenear part between both samples
            slope = LDR_ARRAY[5] / 5
            starting = 0
            for j in range(1, 5):
                LDR_ARRAY[j] = starting + slope * j
        else:
            slope = (LDR_ARRAY[5 * i + 5] - LDR_ARRAY[5 * (i - 1) + 5]) / 5
            starting = LDR_ARRAY[5 * (i - 1) + 5]
            for j in range(1, 5):
                LDR_ARRAY[5 * i + j] = starting + slope * j
        #  print(LDR_ARRAY[i]) # tes\t
        i = i + 1
    s.close()
    for i in range(0, 51):
        print(LDR_ARRAY[i])

###############################################################################
###########       UART FUNCTIONS ##############################################
###############################################################################

def define_uart():
    global s
    s = ser.Serial('COM13', baudrate=9600, bytesize=ser.EIGHTBITS,
                   parity=ser.PARITY_NONE, stopbits=ser.STOPBITS_ONE,
                   # write_timeout=1,
                   timeout=1)  # timeout of 1 sec so that the read and write operations are blocking,
    # after the timeout the program continues
    # clear buffers
    enableTX = True
    enableRX = True
    s.reset_input_buffer()
    s.reset_output_buffer()


def read_uart():  # ser_output(" \r\n");// needed to white in c to catch in python
    global s
    chr = b''
    while chr[-1:] != b'\n':
        chr += s.read(1)

    return chr.decode('ascii')


def write_uart(char):
    global s
    s.write(bytes(char, 'ascii'))
    time.sleep(0.05)  # delay for accurate read/write operations on both ends


###############################################################################
###########       DINAMIC PLOT FUNCTIONS ######################################
###############################################################################


def addDot(angle, dist, colorr="blue", markerr='o', markerrsize=5):  # when you spot something
    global pols
    theta = angle * (np.pi / 180.0)
    pols.set_data([theta], [dist])
    pols.set_color(colorr)
    pols.set_marker(markerr)
    pols.set_markersize(markerrsize)
    ax.draw_artist(pols)


def drawPlot(whichPlot):  # objects/lights/objectsANDlights #to create the plot
    global fig
    global line1
    global ax
    global angle
    global pols
    global dists
    angle = 0
    if (whichPlot == "objects"):
        fig = plt.figure(facecolor='w')
        ax = fig.add_subplot(1, 1, 1, polar=True, facecolor='g')
        line1, = ax.plot([], color='b', linewidth=4.0)
        pols, = ax.plot([], linestyle='', marker='o',
                        markeredgecolor='w', markeredgewidth=1.0, markersize=5.0,
                        alpha=0.9)
    elif (whichPlot == "lights"):
        fig = plt.figure(facecolor='k')
        ax = fig.add_subplot(1, 1, 1, polar=True, facecolor='#006b70')
        line1, = ax.plot([], color='r', linewidth=4.0)
        pols, = ax.plot([], linestyle='', marker='o', markerfacecolor='y',
                        markeredgecolor='w', markeredgewidth=1.0, markersize=5.0,
                        alpha=0.9)
    elif (whichPlot == "objectsANDlights"):
        fig = plt.figure(facecolor='gray')
        ax = fig.add_subplot(1, 1, 1, polar=True, facecolor='purple')
        line1, = ax.plot([], color='y', linewidth=4.0)
        pols, = ax.plot([], linestyle='', marker='o', markerfacecolor='r',
                        markeredgecolor='w', markeredgewidth=1.0, markersize=5.0,
                        alpha=0.9)
    fig.canvas.toolbar.pack_forget()
    fig.canvas.manager.set_window_title('Ultrasonic Radar Plot')
    mng = plt.get_current_fig_manager()
    mng.window.state('zoomed')

    ax.tick_params(axis='both', colors='w')
    r_max = 100.0
    ax.set_ylim([0.0, r_max])
    ax.set_xlim([0.0, np.pi])
    ax.set_position([-0.05, -0.05, 1.1, 1.05])
    ax.set_rticks(np.linspace(0.0, r_max, 5))
    ax.set_thetagrids(np.linspace(0, 180, 10))
    ax.grid(color='w', alpha=0.4)

    angles = np.arange(0, 181, 1)
    theta = angles * (np.pi / 180.0)
    fig.canvas.draw()
    dists = np.ones((len(angles),))
    fig.show()
    fig.canvas.blit(ax.bbox)
    fig.canvas.flush_events()


def progressSensor(theta):  # to progress the sensor to the place were at
    global angle
    axbackground = fig.canvas.copy_from_bbox(ax.bbox)
    r_max = 100.0
    while (angle < theta):
        fig.canvas.restore_region(axbackground)
        line1.set_data(np.repeat((angle * (np.pi / 180)), 2), np.linspace(0.0, r_max, 2))
        ax.draw_artist(line1)
        fig.canvas.blit(ax.bbox)
        fig.canvas.flush_events()
        angle = angle + 1
        theta = theta % 180
        angle = angle % 180
        # time.sleep(0.05)
    fig.canvas.restore_region(axbackground)


###############################################################################
###########       Main GUI FUNCTION      ######################################
###############################################################################

#
# Main menu layout
layout_main_menu = [
    [sg.Button("Object Detector", key='-OBJ-')],
    [sg.Button("Telemeter", key='-TELEMETER-')],
    [sg.Button("Light Detector", key='-LIGHT-')],
    [sg.Button("Object and Lights", key='-OBJ_LIGHT-')],

    [sg.Button("Script Mode", key='-SCRIPT_MODE-')],
    [sg.Button("Collaborate LDR", key='-COLLAB_LDR-')],
]

# Script mode layout
layout_script_mode = [
    [sg.Button("Send Script 1 to Flash", key='-SEND_SCRIPT1-')],
    [sg.Button("Send Script 2 to Flash", key='-SEND_SCRIPT2-')],
    [sg.Button("Send Script 3 to Flash", key='-SEND_SCRIPT3-')],
    [sg.Button("Run Script 1 from Flash", key='-RUN_SCRIPT1-')],
    [sg.Button("Run Script 2 from Flash", key='-RUN_SCRIPT2-')],
    [sg.Button("Run Script 3 from Flash", key='-RUN_SCRIPT3-')],
    [sg.Button("Back to Main Menu", key='-BACK_MAIN_MENU-')],
]

# Collaborate LDR layout
layout_colab_ldr = [
    [sg.Button("Measure and Insert to Flash", key='-MEASURE_INSERT-')],
    [sg.Button("Take Flash Value to Python", key='-TAKE_FLASH-')],
    [sg.Button("Back to Main Menu", key='-BACK_MAIN_MENU-')],
]


def main():
    # telemeter(90)
    # Create the main application window
    window = sg.Window("My Project GUI", layout_main_menu)

    # Event loop to handle events
    while True:
        event, values = window.read()

        if event == sg.WINDOW_CLOSED:
            break
        elif event == '-OBJ-':
            object_detector()
        elif event == '-LIGHT-':
            light_detector()
        elif event == '-OBJ_LIGHT-':
            object_and_lights()
        elif event == '-TELEMETER-':
            print("Telemeter is running...")

            # Get user input for the desired angle
            layout_input_angle = [
                [sg.Text("Enter the desired angle (0-180):"), sg.InputText(size=(5, 1), key="-ANGLE-")],
                [sg.Button("Start Telemeter", key='-START_TELE-')],
                [sg.Button("Back to Main Menu", key='-BACK_MAIN_MENU-')],
            ]

            window_input_angle = sg.Window("Input Angle", layout_input_angle)

            while True:
                event_input_angle, values_input_angle = window_input_angle.read()

                if event_input_angle == sg.WINDOW_CLOSED or event_input_angle == '-BACK_MAIN_MENU-':
                    window_input_angle.close()  # Close the input angle window
                    break
                elif event_input_angle == '-START_TELE-':
                    angle_input = values_input_angle['-ANGLE-']
                    try:
                        angle_input = int(angle_input)
                        if 0 <= angle_input <= 180:
                            print(angle_input, "OKOKOKO")
                            telemeter(angle_input)
                        else:
                            sg.popup("Invalid angle input! Please enter an angle between 0 and 180.")
                    except ValueError:
                        sg.popup("Invalid angle input! Please enter a valid integer.")
        elif event == '-SCRIPT_MODE-':
            window.hide()  # Hide main menu window
            window_script_mode = sg.Window("Script Mode", layout_script_mode)
            while True:
                event_script_mode, values_script_mode = window_script_mode.read()
                if event_script_mode == sg.WINDOW_CLOSED:
                    break
                elif event_script_mode == '-SEND_SCRIPT1-':
                    send_script1()
                elif event_script_mode == '-SEND_SCRIPT2-':
                    send_script2()
                elif event_script_mode == '-SEND_SCRIPT3-':
                    send_script3()
                elif event_script_mode == '-RUN_SCRIPT1-':
                    run_script1()
                elif event_script_mode == '-RUN_SCRIPT2-':
                    run_script2()
                elif event_script_mode == '-RUN_SCRIPT3-':
                    run_script3()
                elif event_script_mode == '-BACK_MAIN_MENU-':
                    window_script_mode.close()  # Close script mode window
                    window.un_hide()  # Show main menu window

        elif event == '-COLLAB_LDR-':
            window.hide()  # Hide main menu window
            window_colab_ldr = sg.Window("Collaborate LDR", layout_colab_ldr)
            while True:
                event_colab_ldr, values_colab_ldr = window_colab_ldr.read()
                if event_colab_ldr == sg.WINDOW_CLOSED:
                    break
                elif event_colab_ldr == '-MEASURE_INSERT-':
                    measure_and_insert_to_flash()
                elif event_colab_ldr == '-TAKE_FLASH-':
                    take_flash_value_to_python()
                elif event_colab_ldr == '-BACK_MAIN_MENU-':
                    window_colab_ldr.close()  # Close collaborate LDR window
                    window.un_hide()  # Show main menu window

    # Close the main window when the event loop exits
    window.close()


if __name__ == "__main__":
    main()

# def main():
#    global LDR_ARRAY  # 51 values from 0 to 50 cm to messure light distance
# define_uart()
#    for i in range(0,51):
#        print(LDR_ARRAY[i])
#    print(find_in_array_ldr(2475))
# colab_ldr_flash()
# objectandlights()
#
