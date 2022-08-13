#!/bin/bash

sudo zypper -n install python3-pip
sudo pip install awscli
aws --version
aws ec2 start-instances --instance-ids "${EC2_INSTANCE}"
sleep 20
