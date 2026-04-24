import socket

MY_MACHINE = "127.0.0.1" #for simulating robot connection locallu
HOST = "192.168.137.172"   # ESP IP address
PORT = 32 # ESP WiFi Port

robot = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print("Connecting to Puckmelt...")
robot.connect((HOST, PORT))
#program hangs here until connected
print("Connected to Puckmelt!")

buffer = ""
while (buffer != "quit"):
    buffer = input("> ")
    if (buffer == "help"):
        print("kill - set all controls to 0\nrestart - call ESP restart\nconfigure\n\tmaxrpm [int] - sets the maximum motor rpm\n\ttimeint [float] - sets the time interval between frames for physics calculations\n\tflipsens [int] - sets the flip sensitivty percent of +-1G\naccel (x - tangent, y - normal)\n\tscale x/y/z [float] - sets the scale factor for the corresponding axis on the accelerometer\n\toffset x/y/z [float] - sets an offset for the corresponding axis on the accelerometer")
    robot.send(bytes(buffer.strip(),'utf-8'))