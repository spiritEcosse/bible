#!/bin/bash

tar -zcf "${FILE}" build &&
scp -i "${ID_FILE}" "${FILE}" "${BACKUP_USER}"@"${BACKUP_HOST}":~/backups/
