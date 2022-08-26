#!/bin/bash

source build/scripts/header.sh

tar -zcf "${FILE}" build &&
BACKUP_HOST=$(aws_get_host) &&
scp -i "${ID_FILE}" "${FILE}" "${EC2_INSTANCE_USER}"@"${BACKUP_HOST}":~/backups/
