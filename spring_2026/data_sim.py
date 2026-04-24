"""
This script simulates the robot sending information and the command terminal
"""

import socket
import math
import random
import time
random.seed(1)

time.sleep(0.2)
robot = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
robot.bind(("0.0.0.0", 5000))
robot.listen(1)
print("Waiting for connection...")
laptop, address = robot.accept()
print("Connection established!")
data_1 = 10
current = 0
while True:
    time.sleep(0.2)
    current += 0.2
    random.randint(-1,1)
    data = f"{data_1+random.randint(-1,1)},{math.sin(current)},1\n"
    laptop.send(bytes(data,'utf-8'))