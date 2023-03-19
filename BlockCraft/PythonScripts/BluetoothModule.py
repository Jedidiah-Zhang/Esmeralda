import serial
import time
import random
import sys
import requests

COMport = sys.argv[1] #'COM3'
baudrate = int(sys.argv[2]) #9600

s = serial.Serial(COMport,baudrate, timeout=None)


while True:

    package = s.readline() #If sending a byte at a time

    print(package)

s.close()


# packet_type = "001"


# blocks = []


# def addBlock(params):
#     ID = f'{params[0]:06b}'
#     loc = f'{params[1]:05b}'
#     pos = f'{params[2]:08b}' #only goes 3 blocks high for now, so will add 6 bits in front
#     rot = f'{params[3]:02b}'

#     blockPackage = packet_type + loc + pos + ID + rot

#     return blockPackage

# blocks.append(addBlock([1,0,0,0]))#Tower 1
# blocks.append(addBlock([3,0,1,0]))
# blocks.append(addBlock([1,0,2,0]))
# blocks.append(addBlock([4,0,3,0]))

# blocks.append(addBlock([1,4,0,0]))#Tower 2
# blocks.append(addBlock([3,4,1,0]))
# blocks.append(addBlock([1,4,2,0]))
# blocks.append(addBlock([4,4,3,0]))

# blocks.append(addBlock([1,20,0,0]))#Tower 3
# blocks.append(addBlock([3,20,2,0]))
# blocks.append(addBlock([1,20,2,0]))
# blocks.append(addBlock([4,20,3,0]))

# blocks.append(addBlock([1,24,0,0]))#Tower 4
# blocks.append(addBlock([3,24,1,0]))
# blocks.append(addBlock([1,24,2,0]))
# blocks.append(addBlock([4,24,3,0]))

# blocks.append(addBlock([1,5,0,0]))#Wall 1
# blocks.append(addBlock([1,10,0,0]))
# blocks.append(addBlock([1,15,0,0]))

# blocks.append(addBlock([1,1,0,0]))#Wall 2
# blocks.append(addBlock([1,2,0,0]))
# blocks.append(addBlock([1,3,0,0]))

# blocks.append(addBlock([1,9,0,0]))#Wall 3
# blocks.append(addBlock([1,14,0,0]))
# blocks.append(addBlock([1,19,0,0]))

# blocks.append(addBlock([1,21,0,0]))#Wall 4
# blocks.append(addBlock([2,22,0,0]))
# blocks.append(addBlock([1,23,0,0]))

# for i in range(5):#len(blocks)):
#     payload={'BlockPackage': str(blocks[i])}
#     response = requests.request("POST", url , data=payload)
#     time.sleep(random.randint(1, 3))