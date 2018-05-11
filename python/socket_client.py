# -*-encoding:utf-8-*-
import sys
import socket

def test_connector():
    HOST = '127.0.0.1'
    PORT = 50007
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    while True:
        cmd = raw_input("Please input cmd:")
        s.sendall(cmd)
        data = s.recv(1024)
        print data
    s.close()

if __name__ == '__main__':
    reload(sys)
    sys.setdefaultencoding('utf-8')
    test_connector()
