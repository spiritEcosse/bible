#!/bin/bash

tar -zcf "${FILE}" build &&
export BACKUP_HOST=$(aws ec2 describe-instances --instance-ids "${EC2_INSTANCE}" --query "Reservations[*].Instances[*].[PublicIpAddress]" --output text)
scp -i "${ID_FILE}" "${FILE}" "${BACKUP_USER}"@"${BACKUP_HOST}":~/backups/
