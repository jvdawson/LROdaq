from bottle import request, route, run, template
from subprocess import Popen, call, PIPE
import time
import socket
#is there a problem with the order of starting of rc.py and server?
##############################

def get_status():
    """This function asks the server for its status """
    s.send('status')
    data = s.recv(1024)
    print(data)
    status={'state':data}
    return (status)

def startacq():
    """This function asks the server to start the acquistion and updates status"""
    s.send('start')
    data = s.recv(1024)
    print(data)
    status={'state':data}
    return (status)

def stopacq():
    """This function asks the server to start the acquistion and updates status"""
    s.send('stop')
    data = s.recv(1024)
    print(data)
    status={'state':data}
    return (status)
#s.close()?

#### global ?
status={'state':'ready'} #ready to go 
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.settimeout(5) #5 seconds ?


while True:
    try:
        print("try to connect to server")
        s.connect(("127.0.0.1", 6086))
        #get status
        status = get_status()
        break
    except socket.error as msg:
        print("Caught exception socket.error : %s" % msg)
        time.sleep(10) #delay 10 seconds
    #need to check if server is running, might need to call this again though
#####################################################
 

ips = { 'ip': ['172.16.4.13','172.16.4.14','172.16.4.15']}
 
############################################################
@route('/rc')
def rc():
    return template('rc.tpl', get_status() ,ips )
@route('/rc', method='POST')
def do_rc():
#  check = [ request.forms.get('start') , request.forms.get('stop')];
    if  request.forms.get('start')  == 'true':
        print("start") #want elseif
        status = startacq()
    if  request.forms.get('stop')  == 'true':
        print("stop")
        status = stopacq()
    if  request.forms.get('abort')  == 'true':
        print("abort")
        status={'state':'abort'}

#do something with the status

    return template('rc.tpl',status , ips) #WORKING!!!
#main
# NEED TO WORK ON FLOW -- going backwards and forwards..
run(host='apcpcwa105', port=8080)
#localhost
