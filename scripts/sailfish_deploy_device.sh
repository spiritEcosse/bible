#!/bin/bash

source scripts/header.sh

main() {
  prepare_aws_instance &&
  echo "-------------------------------- Ssh remote command --------------------------------- " &&
  ssh -i "${ID_FILE}" "${EC2_INSTANCE_USER}"@"${EC2_INSTANCE_HOST}" "\
      $(typeset -f sfdk_deploy_to_device) &&
      sfdk_deploy_to_device
  "
}

main
