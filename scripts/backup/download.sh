#!/bin/bash

source build/scripts/header.sh

echo '================================ Start download backup ===================================' &&
echo "${IDENTITY_FILE}" > "${ID_FILE}" &&
chmod 600 "${ID_FILE}" &&
mkdir -p ~/.ssh/ &&
touch ~/.ssh/known_hosts &&
prepare_aws_instance &&
scp -i "${ID_FILE}" "${EC2_INSTANCE_USER}"@"${EC2_INSTANCE_HOST}":~/backups/"${FILE}" . &&
echo '================================ End download backup ==================================='
