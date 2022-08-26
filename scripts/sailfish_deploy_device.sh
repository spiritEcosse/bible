#!/bin/bash

source scripts/header.sh

main() {
  prepare_aws_instance &&
  echo "-------------------------------- Ssh remote command --------------------------------- " &&
  ssh -i "${DEPLOY_SSH_KEY}" "${DEPLOY_USER}"@"${EC2_INSTANCE_HOST}" "\
      $(typeset -f sfdk_deploy_to_device) &&
      sfdk_deploy_to_device
  "
}

main
