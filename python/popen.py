# -*-encoding:utf-8-*-
import sys
from subprocess import Popen, PIPE

def test_popen():
    cmd = 'ls -l /home/tangyibo/'
    p = Popen(cmd, shell=True, stdout=PIPE, stderr=PIPE)
    p.wait()
    line = p.stderr.readline()
    print line
    if line[:6] == "ERROR " or p.returncode != 0:
        print 'error:%s' % line,
    else:
        list = p.stdout.readlines()

        for var in list:
            print '%s' % var,

if __name__ == '__main__':
    reload(sys)
    sys.setdefaultencoding('utf-8')

    test_popen()
