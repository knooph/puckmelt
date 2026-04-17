"""
This script simulates the robot sending information and the command terminal
"""

import socket
import math
import random
random.seed(1)

robot = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
robot.bind(("0.0.0.0", 5000))
robot.listen(1)
print("Waiting for connection...")
laptop, address = robot.accept()
print("Connection established!")

time = 0.01
data_1 = 10
while True:
    time += 0.01
    random.randint(-1,1)
    data = f"{time},{data_1+random.randint(-1,1)},{math.sin(time)},1\n"
    laptop.send(bytes(data,'utf-8'))