#!/bin/bash

echo "${IDENTITY_FILE}" > "${ID_FILE}" &&
chmod 600 "${ID_FILE}" &&
echo "$PWD"

scp -i "${ID_FILE}" "${BACKUP_SERVER}":~/backups/"${FILE}" .

RESULT_SCP=$?

if [ $RESULT_SCP -eq 0 ]; then
	tar -xf "${FILE}" ;
else
	echo "Doesn't exist: ${FILE}";
fi
