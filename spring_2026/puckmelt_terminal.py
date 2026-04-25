import socket

MY_MACHINE = "127.0.0.1" #for simulating robot connection locallu
HOST = "192.168.137.124"   # ESP IP address
PORT = 32 # ESP WiFi Port

robot = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print("Connecting to Puckmelt...")
robot.connect((HOST, PORT))
#program hangs here until connected
print("Connected to Puckmelt!")


# # STACK OVERFLOW COPY PASTE #############
# import select
# import sys

# def timeout_input(timeout, prompt="", timeout_value=None):
#     sys.stdout.write(prompt)
#     sys.stdout.flush()
#     ready, _, _ = select.select([sys.stdin], [], [], timeout)
#     if ready:
#         return sys.stdin.readline().rstrip('\n')
#     else:
#         sys.stdout.write('\n')
#         sys.stdout.flush()
#         return timeout_value
# ######################################

buffer = ""
while (buffer != "quit"):
    buffer = input("> ")
    robot.send(bytes(buffer.strip(),'utf-8'))

# while True:
#     data_in = robot.recv(1024).decode()
#     out = ""
#     data_rows = data_in.splitlines()
#     for row in data_rows:
#         log = row.split(',')
#         if log[len(log)-1] != "":
#             out += '\n' + log[len(log)-1]
    
#     if out != "":
#         print(out)
    