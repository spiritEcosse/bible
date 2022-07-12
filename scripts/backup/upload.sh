#!/bin/bash

tar --exclude="${FILE}" -zcf "${FILE}" . &&
scp -i "${ID_FILE}" "${FILE}" "${BACKUP_SERVER}":~/backups/
