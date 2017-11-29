import RPi.GPIO as GPIO
import math
import os
from datetime import datetime
from time import sleep

INPUT_WIRE = 16

GPIO.setmode(GPIO.BOARD)
GPIO.setup(INPUT_WIRE, GPIO.IN)

while True:
        value = 1
        while value:
                value = GPIO.input(INPUT_WIRE)

        startTime = datetime.now()

        command = []
        
        numOnes = 0
        
        previousVal = 0
        
        while True:
            
            if value != previousVal:
                
                now = datetime.now()
                pulseLength = now- startTime
                startTime = now
                
                command.append((previousVal, pulseLength.microseconds))
                
            if value:
                numOnes = numOnes + 1
            else:
                numOnes = 0
            
            if numOnes > 10000:
                break
            
            previousVal = value
            value = GPIO.input(INPUT_WIRE)
        print("Start")
	for (val, pulse)in command:
		print val, pulse
        binaryString = "".join(map(lambda x: "1" if x[1]>1000 else "0", filter(lambda x: x[0]==1,command)))
        print(binaryString)
        print("End")
        print("Size of array is " + str(len(command)))
