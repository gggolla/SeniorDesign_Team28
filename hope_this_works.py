# Modified from Seungchul Lee's code
# iSystems Design Lab.
# http://isystems.unist.ac.kr/
# UNIST

import serial
import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation

ser = serial.Serial('COM3', 9600)

n = 201
fig = plt.figure(figsize=(12, 9))

ax1 = fig.add_subplot(3, 1, 1)
ax2 = fig.add_subplot(3, 1, 2)
ax3 = fig.add_subplot(3, 1, 3)

graphX, = ax1.plot([], [], 'b', label = 'X')
graphY, = ax2.plot([], [], 'r', label = 'Y')
graphZ, = ax3.plot([], [], 'g', label = 'Z')
axes = [ax1, ax2, ax3]

for ax in axes:
    ax.set_xlim(0, n-1)
    ax.set_ylabel('Gyroscope (degrees/sec)')
    ax.legend(loc='upper right')
    ax.grid(True)

ax1.set_title('Real-time sensor data')
ax3.set_xlabel('Data points')
    
t = list(range(0, n))
accX = [0] * n
accY = [0] * n
accZ = [0] * n
gyroX = [0] * n
gyroY = [0] * n
gyroZ = [0] * n

def init():
    graphX.set_data([], [])
    graphY.set_data([], [])
    graphZ.set_data([], [])
    return graphX, graphY, graphZ,

def animate(i):

    while (ser.inWaiting() == 0):
        pass

    arduinoString = ser.readline().decode("utf-8")
    dataArray = arduinoString.split(',')

    accX.append(float(dataArray[0])/(32767/2)) # Default accel sensitivity   
    accY.append(float(dataArray[1])/(32767/2))    
    accZ.append(float(dataArray[2])/(32767/2))
    accX.pop(0)
    accY.pop(0)
    accZ.pop(0)

    gyroX.append(float(dataArray[3])/131) # Default gyro sensitivity   
    gyroY.append(float(dataArray[4])/131)    
    gyroZ.append(float(dataArray[5])/131)
    gyroX.pop(0)
    gyroY.pop(0)
    gyroZ.pop(0)

    graphX.set_data(t, accX)
    graphY.set_data(t, accY)
    graphZ.set_data(t, accZ)

    print(dataArray)

    return graphX, graphY, graphZ,

delay = 0
anim = animation.FuncAnimation(fig, animate, init_func=init,
                               interval=delay, blit=True)

plt.show()

ser.close()
