# -*-encoding:utf-8-*-
import sys
import os
import socket

def handle_client(client,address):
    client.settimeout(60)
    print "%s get a client[%s] from %s" % (os.getpid(), str(client), address)

    while True:
        try:
            msg = client.recv(1024)
            if not msg or len(msg) <= 0:
                print "recv msg is 0,close client [%s]" % address
                break
            else:
                client.sendall(msg)
                print "send:%s" % msg
        except Exception as x:
            print "exception happen,close client [%s],errror:%s" % (address, x.args)
            break

    client.close()


def test_tcpserver():
    bind_ip = '0.0.0.0'
    bind_port = 50007
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind((bind_ip, bind_port))
    server.listen(10)
    print "start listen on %s:%d"%(bind_ip,bind_port)

    while True:
        try:
            client, address = server.accept()

            pid = os.fork()
            if pid < 0:
                print "fork error"
                sys.exit(2)
            elif pid > 0:  # father
                print 'child process %d' % pid
                continue
            else:         # child
                handle_client(client,address)
                break
        except Exception as x:
            print "exception accept,error:%s" % x.args
            continue

if __name__ == '__main__':
    reload(sys)
    sys.setdefaultencoding('utf-8')
    test_tcpserver()
