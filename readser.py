import serial
import datetime

ser = serial.Serial('/dev/ttyUSB0',115200)

start = datetime.datetime.today()

while True:
    line = ser.readLine()
    red = line[37:41]
    print(red)

    if(int(red,base=16) > 300):
        end = datetime.datetime.today()
        seconds = (end-start).seconds
        print(seconds)
        print("     Game!")
