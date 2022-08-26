#!/bin/bash

source scripts/header.sh


echo "${IDENTITY_FILE}" > "${ID_FILE}" &&
chmod 600 "${ID_FILE}" &&
mkdir -p ~/.ssh/ &&
touch ~/.ssh/known_hosts &&
prepare_aws_instance &&
scp -i "${ID_FILE}" "${BACKUP_USER}"@"${EC2_INSTANCE_HOST}":~/backups/"${FILE}" .

RESULT_SCP=$?

if [ $RESULT_SCP -eq 0 ]; then
	tar -xf "${FILE}" ;
else
	echo "Doesn't exist: ${FILE}";
fi
