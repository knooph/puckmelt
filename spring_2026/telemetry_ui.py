"""
This script plots real time data coming off of the meltybrain and acts as a terminal to send commands to the meltybrain.
"""

import socket
import pyqtgraph as pq
from pyqtgraph.Qt import QtCore, QtWidgets
from collections import deque

######################################################################## Connect to Robot
MY_MACHINE = "127.0.0.1"
HOST = "127.0.0.1"   # ESP IP address
PORT = 5000 # ESP WiFi Port

robot = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print("Connecting to Puckmelt...")
robot.connect((HOST, PORT))
#program hangs here until connected
print("Connected to Puckmelt!")
#######################################################################



app = QtWidgets.QApplication([]) #This sets up the actual event loop handler
window = pq.GraphicsLayoutWidget() #the canvas that will hold plots in the app
plot = window.addPlot() #create a plot object inside the window
curve = plot.plot() #add data to the plot
window.show()

datapoints = 100
time,ct_xl,tn_xl,z_xl = [deque(maxlen=datapoints) for _ in range(4)] #only hold thelast 1000 datapoints
def update():
    partitioned = map(lambda x: x.split(','), robot.recv(1024).decode().splitlines())
    partitioned.pop()
    partitioned.pop(0)
    for data_point in partitioned:
        if len(data_point) == 3:
            try:
                time.append(float(data_point[0]))
                ct_xl.append(float(data_point[1]))
                tn_xl.append(float(data_point[2]))
                z_xl.append(float(data_point[3]))
            except:
                continue
        
    curve.setData(time,ct_xl)

timer = QtCore.QTimer() #creates the internal clock for the event loop
timer.timeout.connect(update) #this is pretty much set an event callback for when the timer runs out
timer.start(10) #start the timer that goes every 10ms

app.exec()