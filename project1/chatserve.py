#Programmer Name: Wei Huang
#Program Name: chatserve.py
#Description: This program is a chat application of API between server and client. Mos t of the code is fixed usage.
#             reference from  http://docs.python.org/release/2.6.5/library/internet.html and Gregmankes's project in github that posted on nov 29,2016.
#Course Name: cs372
#Date: 2019/7/29

import sys
from socket import*
# The funtion get user name and sen the name to server.
def connect(connectionsocket, username):
        clientname = connectionsocket.recv(1024)
        connectionsocket.send(username)
        return clientname
        
        
# such as main function
if __name__ == "__main__":
  #make sure the arguement is correct
  if len(sys.argv) != 2:
    print "Please enter specify the port number"
    exit(1)
    # get the port number
  serverport = int(sys.argv[1])
  serversocket =socket(AF_INET, SOCK_STREAM)
  #get thee spcific port 
  serversocket.bind(('', serverport))
  #waitting the message from the spcific port
  serversocket.listen(1)
  #ask user name
  
  username = raw_input("Please enter a user name in 10 characters: ")
  print "The server is ready to receive messages"
      
      #chartting part
  while 1:
          connectionsocket, address = serversocket.accept()
          clientname=connect(connectionsocket, username)
           
          while 1:
              #receive message
              received = connectionsocket.recv(500)             
              # if we received nothing that closed connection
              if received == "":
                  print "Connection closed"
                  break              
              # print the clients name and their message
              print "{}> {}".format(clientname, received)
              # server send message to user
              text = ""
              while len(text) == 0 or len(text) > 500:
                text = raw_input("{}> ".format(username))
              # if the message is \q that closed connection
              if text == "\q":
                  print "Connection closed"   
                  break
              connectionsocket.send(text)
        # project claose
          connectionsocket.close()
