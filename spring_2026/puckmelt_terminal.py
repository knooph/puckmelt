import socket

MY_MACHINE = "127.0.0.1" #for simulating robot connection locallu
HOST = "192.168.137.230"   # ESP IP address
PORT = 32 # ESP WiFi Port

robot = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print("Connecting to Puckmelt...")
robot.connect((HOST, PORT))
#program hangs here until connected
print("Connected to Puckmelt!")

buffer = ""
while (buffer != "quit"):
    buffer = input("> ")
    robot.send(bytes(buffer.strip(),'utf-8'))