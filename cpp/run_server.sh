#!/bin/bash
# a script that initialize iptables with rule that drop all 
# connection to ssh 22 port and run server

# THIS SCRIPT NEED SUDO PRIVILEGES BECAUSE OF IPTABLES MODIFICATION

iptables -C INPUT -p tcp --dport 22 -j DROP &>/dev/null

if [ $? = 1 ]; then
    iptables -A INPUT -p tcp --dport 22 -j DROP
fi

cd bin/
./app_server
cd ..
