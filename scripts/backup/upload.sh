#!/bin/bash

tar -zcf "${FILE}" build &&
../ec2_get_ip.sh &&
scp -i "${ID_FILE}" "${FILE}" "${BACKUP_USER}"@"${BACKUP_HOST}":~/backups/
