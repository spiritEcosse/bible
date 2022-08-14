#!/bin/bash

sudo zypper -n install python3-pip &&
sudo pip install awscli &&
aws --version &&
echo "-------------------------------- Start instance --------------------------------- " &&
mkdir ~/.aws/ &&
echo "[default]
region = ${AWS_REGION}" > ~/.aws/config &&
aws ec2 start-instances --instance-ids "${EC2_INSTANCE}" &&
sleep 20
