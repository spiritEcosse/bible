#!/bin/bash

sudo zypper -n install python3-pip &&
sudo pip install awscli &&
aws --version &&
echo "-------------------------------- Start instance --------------------------------- " &&
mkdir ~/.aws/ &&
echo "[default]
region = ${AWS_REGION}" > ~/.aws/config &&
aws ec2 start-instances --instance-ids "${EC2_INSTANCE}" &&
sleep 20 &&
export BACKUP_HOST=$(aws ec2 describe-instances --instance-ids "${EC2_INSTANCE}" --query "Reservations[*].Instances[*].[PublicIpAddress]" --output text)
