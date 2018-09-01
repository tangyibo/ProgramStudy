#include <iostream>
#include <string>
#include <assert.h>
#include <vector>
#include <boost/bind.hpp>
#include "ZooKeeperCpp.h"

using namespace std;

// define data callback
void dataCallback(const std::string &path, const std::string &value)
{
	cout << "data changed: " << " path=" << path << ", data=" << value << endl;
}

// define children callback
void childrenCallback(const std::string &path, const vector<string> &children)
{
	cout << "children changed: " << ", path=" << path << ", children=";
	for(size_t i = 0; i < children.size(); ++i)
	{
		if(i != 0)
			cout << ",";
		cout << children[i];
	}
	cout << endl;
}

// define ZooKeeper object
ZooKeeper zk; 


int main()
{
	// init ZooKeeper	
	if(!zk.init("127.0.0.1:2181"))
	{
		cout << "init zk failed." << endl;
		return -1;
	}

	// set log
	// zk.setDebugLogLevel();
	// zk.setConsoleLog();
	zk.setFileLog();

        //创建持久节点
        if(zk.exists("/mysql/host"))
        {
                zk.deleteNode("/mysql/host");
                cout<<"delte exist node /mysql/host"<<endl;
        }
        ZkRet zr = zk.createNode("/mysql/host","127.0.0.1",true);
        assert(zr || zr.nodeExist());
        zr=zk.setData("/mysql/host","192.168.1.1");
        assert(zr);
        assert(zk.exists("/mysql/host"));

        if(zk.exists("/mysql/port"))
        {
                zk.deleteNode("/mysql/port");
                cout<<"delte exist node /mysql/port"<<endl;
        }
        zr=zk.createNode("/mysql/port","3306",true);
        assert(zr || zr.nodeExist());
        assert(zk.exists("/mysql/port"));

        if(zk.exists("/mysql/dbname"))
        {
                zk.deleteNode("/mysql/dbname");
                cout<<"delte exist node /mysql/dbname"<<endl;
        }
        zr=zk.createNode("/mysql/dbname","testdb",true);
        assert(zr || zr.nodeExist());
        assert(zk.exists("/mysql/dbname"));

        //创建临时节点
        if(zk.exists("/tmp/hello"))
        {
                zk.deleteNode("/tmp/hello");
                cout<<"delte exist node /tmp/hello"<<endl;
        }
        assert(zk.createEphemeralNode("/tmp/hello", "hello world", true));
        assert(zk.exists("/tmp/hello"));

        //创建顺序节点
        string rpath;
        assert(zk.createSequenceNode("/seq/test_", "test_seq", rpath, true));
        assert(zk.exists(rpath));
        cout<<"Create sequence node is "<<rpath<<endl;

        string value;
        if(zk.getData("/mysql/host", value))
        {
                cout<<"Mysql.host="<<value<<endl;
        }

        if(zk.getData("/mysql/port", value))
        {
                cout<<"Mysql.port="<<value<<endl;
        }

        if(zk.getData("/mysql/dbname", value))
        {
                cout<<"Mysql.dbname="<<value<<endl;
        }

	// test for data watch
	if(!zk.watchData("/mysql/host", boost::bind(&dataCallback, _1, _2)))
	{
		cout << "watch data failed, path=/mysq/host" << endl;
	}
	
        // test for children watch
	if(!zk.watchChildren("/mysql", boost::bind(&childrenCallback, _1, _2)))
	{
		cout << "watch children failed, path=/mysql" << endl;
	}

	
	while(1)
	{
		sleep(1);
	}

	return 0;
}
