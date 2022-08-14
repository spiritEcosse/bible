#!/bin/bash

echo "${IDENTITY_FILE}" > "${ID_FILE}" &&
chmod 600 "${ID_FILE}" &&
mkdir -p ~/.ssh/ &&
touch ~/.ssh/known_hosts &&
BACKUP_HOST=$(aws ec2 describe-instances --instance-ids "${EC2_INSTANCE}" --query "Reservations[*].Instances[*].[PublicIpAddress]" --output text) &&
echo "BACKUP_HOST: ${BACKUP_HOST}" &&
echo "EC2_INSTANCE: ${EC2_INSTANCE}" &&
ssh-keyscan -H "${BACKUP_HOST}" >> ~/.ssh/known_hosts &&
scp -i "${ID_FILE}" "${BACKUP_USER}"@"${BACKUP_HOST}":~/backups/"${FILE}" .

RESULT_SCP=$?

if [ $RESULT_SCP -eq 0 ]; then
	tar -xf "${FILE}" ;
else
	echo "Doesn't exist: ${FILE}";
fi
