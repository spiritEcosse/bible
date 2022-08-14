#!/bin/bash

export BACKUP_HOST="$(aws ec2 describe-instances --instance-ids \"${EC2_INSTANCE}\"--query \"Reservations[*].Instances[*].[PublicIpAddress]\" --output text)"
