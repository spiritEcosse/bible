#!/bin/bash

source scripts/header.sh

main() {
  prepare_aws_instance
  ssh -i "${ID_FILE}" "${EC2_INSTANCE_USER}"@"${EC2_INSTANCE_HOST}" "\
      $(typeset -f sailfish_run_tests_on_aws) &&
      export ARCH=\"$ARCH\" &&
      sailfish_run_tests_on_aws
  "
}

main
