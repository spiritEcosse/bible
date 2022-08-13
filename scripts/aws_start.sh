#!/bin/bash

sudo zypper -n install python3-pip &&
sudo pip install awscli &&
aws --version &&
echo "-------------------------------- Start instance --------------------------------- " &&
export AWS_REGION=us-east-1 &&
aws ec2 start-instances --instance-ids "${EC2_INSTANCE}" &&
sleep 20
