# LROdaq

############## 
#to test 
python rc.py in one terminal
g++ server.c -o server
./server in another terminal
Point web-browser to http://localhost:8080/rc

Python Bottle:
rc.py -- example of a runcontrol website. Communicates with server.c

C++:

server.c -- example of socket programming. Passing data between python rc.py and server

###################################

client class - C base class for UDP reading/writing on a dedicated port.
card class -- holds protocol for communicating with a AMC card

#################################
