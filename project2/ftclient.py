#Programmer Name: Wei Huang
#Program Name: ftclient.py
#Description: This program is a chat application of API between server and client. Mos t of the code is fixed usage.
#             reference from  http://docs.python.org/release/2.6.5/library/internet.html and Gregmankes's project in github that posted on Nov 29,2016. 
#                                                                                              https://github.com/gregmankes/cs372-project2
#                             http://stackoverflow.com/questions/24196932/how-can-i-get-the-ip-address-of-eth0-in-python
#Course Name: cs372
#Date: 2019/8/13


from socket import *
import sys

def contact():
    # get IP address ----learn from stackoverflow.com
    s = socket(AF_INET, SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))
    return s.getsockname()[0]

def connect_server():
    # get the name of the server
    servername = sys.argv[1]
    serverport = int(sys.argv[2])
    # get the port of the server
    clientsocket = socket(AF_INET,SOCK_STREAM)
    clientsocket.connect((servername, serverport))
    #get the socket from the sever
    return clientsocket


def setup_data():
    # if argv[3] is "-l",port arg is 4
    if sys.argv[3] == "-l":
        serverport = int(sys.argv[4])
        serversocket = socket(AF_INET, SOCK_STREAM)
        serversocket.bind(('', serverport))
        serversocket.listen(1)
        data, addr = serversocket.accept()
    # if argv[3] is "-g",port arg is 5
    elif sys.argv[3] == "-g":
        serverport = int(sys.argv[5])
        serversocket = socket(AF_INET, SOCK_STREAM)
        serversocket.bind(('', serverport))
        serversocket.listen(1)
        data, addr = serversocket.accept()
    return data

# the function get the file list of the argv[3] is "-l" 
def get_file_l(data):
    file = data.recv(100)
    while file!= "done":
        print file
        file = data.recv(100)

# the function get the file list of the argv[3] is "-g" 
def get_file_g(data):
    file = open(sys.argv[4],"w")
    buff = data.recv(1000)
    while "__done__" not in buff:
        file.write(buff)
        buff = data.recv(1000)


# such as main function
if __name__ == "__main__":
    #check the all enter error of command line.
    if len(sys.argv) < 5 or len(sys.argv) > 6:
        print "request: python ftclient.py <flip1-3> <server_port> -l <data_port> or python ftclient.py <flip1-3> <server_port> -g <file_to_get> <data_port>" 
        exit(1)
    elif (sys.argv[1] != "flip1" and sys.argv[1] != "flip2" and sys.argv[1] != "flip3"):
        print "request: python ftclient.py <flip1-3> <server_port> -l <data_port> or python ftclient.py <flip1-3> <server_port> -g <file_to_get> <data_port>"
        exit(1)
    elif (int(sys.argv[2]) > 65535 or int(sys.argv[2]) < 1024):
        print "please correct port number"
        exit(1)
    elif (sys.argv[3] != "-l" and sys.argv[3] != "-g"):
        print "request: python ftclient.py <flip1-3> <server_port> -l <data_port> or python ftclient.py <flip1-3> <server_port> -g <file_to_get> <data_port>"
        exit(1)
    elif (sys.argv[3] == "-l" and (int(sys.argv[4]) > 65535 or int(sys.argv[4]) < 1024)):
        print "Invalid number of arguments"
        exit(1)
    elif (sys.argv[3] == "-g" and (int(sys.argv[5]) > 65535 or int(sys.argv[5]) < 1024)):
        print "Invalid number of arguments"
  
  #chartting part
    clientsocket = connect_server()
 # if argv[3] is "-l",port arg is 4
    if sys.argv[3] == "-l":
        print "File list"
        clientsocket.send(sys.argv[4])
     # if argv[3] is "-g",port arg is 5
    elif sys.argv[3] == "-g":
        print "File {}".format(sys.argv[4])
        clientsocket.send(sys.argv[5])
    clientsocket.recv(1024)

    # send command
    if sys.argv[3] == "-l":
        clientsocket.send("l")
    else:
        clientsocket.send("g")  
    clientsocket.recv(1024)
    clientsocket.send(contact())
    feedback = clientsocket.recv(1024)

    # if the server not work
    if feedback == "bad":
        print "Server received an invalid command"
        exit(1)
    # get the file name and check if the file exist
    if sys.argv[3] == "-g":
        clientsocket.send(sys.argv[4])       
        feedback = clientsocket.recv(1024)
        if feedback != "File found":
            print "File not found"
            exit(1)
     #begin to receive data
    data = setup_data()
   # filf list
    if sys.argv[3] == "-l":
        get_file_l(data)
    #specific file
    elif(sys.argv[3] == "-g"):
        get_file_g(data)
    # close 
    data.close()