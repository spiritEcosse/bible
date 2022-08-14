#!/bin/bash

aws ec2 stop-instances --instance-ids "${EC2_INSTANCE}"
