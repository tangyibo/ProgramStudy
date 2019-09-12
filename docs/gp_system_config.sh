#!/bin/bash

#Source params
#source params.sh
#Install dependencies of GP by yum
hostname=`hostname`
yum install -y net-tools which openssh-clients openssh-server less zip unzip iproute.x86_64

#Shutdown selinux and set to shutdown automatically boot
setenforce 0
sed -i "s#^SELINUX=.*#SELINUX=disabled#g" /etc/selinux/config


#Change some values of System Option in /etc/sysctl.conf
if [ "`cat /etc/sysctl.conf | grep -c '#xfs_mount_options'`" != 0 ] 
        then
        sed -i "s\^#xfs_mount_options =.*\#xfs_mount_options = rw,noatime,inode64,allocsize=16m\g" /etc/sysctl.conf
else
        echo "" >> /etc/sysctl.conf
        echo "#xfs_mount_options = rw,noatime,inode64,allocsize=16m" >> /etc/sysctl.conf
fi

if [ "`cat /etc/sysctl.conf | grep -c kernel.shmmax`" != 0 ] 
	then
	sed -i "s#^kernel.shmmax =.*#kernel.shmmax = 500000000#g" /etc/sysctl.conf
else
	echo "" >> /etc/sysctl.conf
	echo "kernel.shmmax = 500000000" >> /etc/sysctl.conf
fi

if [ "`cat /etc/sysctl.conf | grep -c kernel.shmmni`" != 0 ]
	then
	sed -i "s#^kernel.shmmni =.*#kernel.shmmni = 4096#g" /etc/sysctl.conf
else
	echo "" >> /etc/sysctl.conf
        echo "kernel.shmmni = 4096" >> /etc/sysctl.conf
fi

if [ "`cat /etc/sysctl.conf | grep -c kernel.shmall`" != 0 ]
	then
	sed -i "s#^kernel.shmall =.*#kernel.shmall = 4000000000#g" /etc/sysctl.conf
else
        echo "" >> /etc/sysctl.conf
        echo "kernel.shmall = 4000000000" >> /etc/sysctl.conf
fi

if [ "`cat /etc/sysctl.conf | grep -c kernel.sem`" != 0 ]
        then
        sed -i "s#^kernel.sem =.*#kernel.sem = 500 1024000 200 4096#g" /etc/sysctl.conf
else
        echo "" >> /etc/sysctl.conf
        echo "kernel.sem = 500 1024000 200 4096" >> /etc/sysctl.conf
fi

if [ "`cat /etc/sysctl.conf | grep -c kernel.sysrq`" != 0 ]
        then
        sed -i "s#^kernel.sysrq =.*#kernel.sysrq = 1#g" /etc/sysctl.conf
else
        echo "" >> /etc/sysctl.conf
        echo "kernel.sysrq = 1" >> /etc/sysctl.conf
fi

if [ "`cat /etc/sysctl.conf | grep -c kernel.core_uses_pid`" != 0 ]
        then
        sed -i "s#^kernel.core_uses_pid =.*#kernel.core_uses_pid = 1#g" /etc/sysctl.conf
else
        echo "" >> /etc/sysctl.conf
        echo "kernel.core_uses_pid = 1" >> /etc/sysctl.conf
fi

if [ "`cat /etc/sysctl.conf | grep -c kernel.msgmnb`" != 0 ]
        then
        sed -i "s#^kernel.msgmnb =.*#kernel.msgmnb = 65536#g" /etc/sysctl.conf
else
        echo "" >> /etc/sysctl.conf
        echo "kernel.msgmnb = 65536" >> /etc/sysctl.conf
fi

if [ "`cat /etc/sysctl.conf | grep -c kernel.msgmax`" != 0 ]
        then
        sed -i "s#^kernel.msgmax =.*#kernel.msgmax = 65536#g" /etc/sysctl.conf
else
        echo "" >> /etc/sysctl.conf
        echo "kernel.msgmax = 65536" >> /etc/sysctl.conf
fi

if [ "`cat /etc/sysctl.conf | grep -c kernel.msgmni`" != 0 ]
        then
        sed -i "s#^kernel.msgmni =.*#kernel.msgmni = 2048#g" /etc/sysctl.conf
else
        echo "" >> /etc/sysctl.conf
        echo "kernel.msgmni = 2048" >> /etc/sysctl.conf
fi

if [ "`cat /etc/sysctl.conf | grep -c net.ipv4.tcp_syncookies`" != 0 ]
        then
        sed -i "s#^net.ipv4.tcp_syncookies =.*#net.ipv4.tcp_syncookies = 1#g" /etc/sysctl.conf
else
        echo "" >> /etc/sysctl.conf
        echo "net.ipv4.tcp_syncookies = 1" >> /etc/sysctl.conf
fi

if [ "`cat /etc/sysctl.conf | grep -c net.ipv4.ip_forward`" != 0 ]
        then
        sed -i "s#^net.ipv4.ip_forward =.*#net.ipv4.ip_forward = 0#g" /etc/sysctl.conf
else
        echo "" >> /etc/sysctl.conf
        echo "net.ipv4.ip_forward = 0" >> /etc/sysctl.conf
fi

if [ "`cat /etc/sysctl.conf | grep -c net.ipv4.conf.default.accept_source_route`" != 0 ]
        then
        sed -i "s#^net.ipv4.conf.default.accept_source_route =.*#net.ipv4.conf.default.accept_source_route = 0#g" /etc/sysctl.conf
else
        echo "" >> /etc/sysctl.conf
        echo "net.ipv4.conf.default.accept_source_route = 0" >> /etc/sysctl.conf
fi

if [ "`cat /etc/sysctl.conf | grep -c net.ipv4.tcp_tw_recycle`" != 0 ]
        then
        sed -i "s#^net.ipv4.tcp_tw_recycle =.*#net.ipv4.tcp_tw_recycle = 1#g" /etc/sysctl.conf
else
        echo "" >> /etc/sysctl.conf
        echo "net.ipv4.tcp_tw_recycle = 1" >> /etc/sysctl.conf
fi

if [ "`cat /etc/sysctl.conf | grep -c net.ipv4.tcp_max_syn_backlog`" != 0 ]
        then
        sed -i "s#^net.ipv4.tcp_max_syn_backlog =.*#net.ipv4.tcp_max_syn_backlog = 4096#g" /etc/sysctl.conf
else
        echo "" >> /etc/sysctl.conf
        echo "net.ipv4.tcp_max_syn_backlog = 4096" >> /etc/sysctl.conf
fi

if [ "`cat /etc/sysctl.conf | grep -c net.ipv4.conf.all.arp_filter`" != 0 ]
        then
        sed -i "s#^net.ipv4.conf.all.arp_filter =.*#net.ipv4.conf.all.arp_filter = 1#g" /etc/sysctl.conf
else
        echo "" >> /etc/sysctl.conf
        echo "net.ipv4.conf.all.arp_filter = 1" >> /etc/sysctl.conf
fi

if [ "`cat /etc/sysctl.conf | grep -c net.ipv4.ip_local_port_range`" != 0 ]
        then
        sed -i "s#^net.ipv4.ip_local_port_range =.*#net.ipv4.ip_local_port_range = 10000 65535#g" /etc/sysctl.conf
else
        echo "" >> /etc/sysctl.conf
        echo "net.ipv4.ip_local_port_range = 10000 65535" >> /etc/sysctl.conf
fi

if [ "`cat /etc/sysctl.conf | grep -c net.core.netdev_max_backlog`" != 0 ]
        then
        sed -i "s#^net.core.netdev_max_backlog =.*#net.core.netdev_max_backlog = 10000#g" /etc/sysctl.conf
else
        echo "" >> /etc/sysctl.conf
        echo "net.core.netdev_max_backlog = 10000" >> /etc/sysctl.conf
fi

if [ "`cat /etc/sysctl.conf | grep -c net.core.rmem_max`" != 0 ]
        then
        sed -i "s#^net.core.rmem_max =.*#net.core.rmem_max = 2097152#g" /etc/sysctl.conf
else
        echo "" >> /etc/sysctl.conf
        echo "net.core.rmem_max = 2097152" >> /etc/sysctl.conf
fi

if [ "`cat /etc/sysctl.conf | grep -c net.core.wmem_max`" != 0 ]
        then
        sed -i "s#^net.core.wmem_max =.*#net.core.wmem_max = 2097152#g" /etc/sysctl.conf
else
        echo "" >> /etc/sysctl.conf
        echo "net.core.wmem_max = 2097152" >> /etc/sysctl.conf
fi

if [ "`cat /etc/sysctl.conf | grep -c vm.overcommit_memory`" != 0 ]
        then
        sed -i "s#^vm.overcommit_memory =.*#vm.overcommit_memory = 2#g" /etc/sysctl.conf
else
        echo "" >> /etc/sysctl.conf
        echo "vm.overcommit_memory = 2 ###If have Oracle,should be setting 1" >> /etc/sysctl.conf
fi

#Add values in file /etc/security/limits.conf
if [ "`cat /etc/security/limits.conf | grep -c "* soft"`" != "2" ];then
echo "* soft nofile 65536" >> /etc/security/limits.conf
echo "* soft nproc 131072" >> /etc/security/limits.conf
fi

if [ "`cat /etc/security/limits.conf | grep -c "* hard"`" != "2" ];then
echo "* hard nofile 65536" >> /etc/security/limits.conf
echo "* hard nproc 131072" >> /etc/security/limits.conf
fi


#Add values in file /etc/rc.d/rc.local
if [ "`cat /etc/rc.d/rc.local | grep -c "blockdev --setra 16384"`" != "1" ];then
echo "blockdev --setra 16384 /dev/sd*" >> /etc/rc.d/rc.local
fi

#Startup IPMI service and set to start automatically boot
#service ipmi start



#chkconfig ipmi on

#/etc/inittab
sed -i 's/^id:.*:initdefault:/id:3:initdefault:/g' /etc/inittab

wait