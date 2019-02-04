import serial
import time
import os
from datetime import datetime

ser = serial.Serial('COM3',57600,timeout=0)
f = open('data.csv','w')

while 1:
    try:
        incoming = str(ser.readline())
        if(incoming != "b\'\'"):
            
            timeNow = datetime.now().strftime('%H:%M:%S')
            
            print(incoming[2:-1] + "," + timeNow)
            
            f.write(incoming[2:-1] + "," + timeNow + os.linesep)
            
    except serial.serialutil.SerialException:
        print("Data could not be read")
        f.close()
        exit()
    time.sleep(30)  
