#!/bin/bash

tar -zcf "${FILE}" build &&
BACKUP_HOST=$(aws_get_host) &&
scp -i "${ID_FILE}" "${FILE}" "${BACKUP_USER}"@"${BACKUP_HOST}":~/backups/
