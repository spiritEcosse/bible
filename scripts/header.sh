#!/bin/bash

# e - script stops on error (return != 0)
# u - error if undefined variable
# o pipefail - script fails if one of piped command fails
# x - output each line (debug)
set -euo pipefail

aws_get_host() {
  aws ec2 describe-instances --instance-ids "${EC2_INSTANCE}" --query "Reservations[*].Instances[*].[PublicIpAddress]" --output text
}

aws_stop() {
  aws ec2 stop-instances --instance-ids "${EC2_INSTANCE}"
}

aws_get_instance_status() {
  aws ec2 describe-instance-status --instance-ids "${EC2_INSTANCE}" --query "InstanceStatuses[*].InstanceState.Name" --output text
}

linux_install_aws() {
  sudo zypper -n install python3-pip &&
  sudo pip install awscli &&
  aws --version &&
  echo "-------------------------------- Start instance --------------------------------- " &&
  mkdir ~/.aws/ &&
  echo "[default]
  region = ${AWS_REGION}" > ~/.aws/config
}

set_up_instance_aws_host_to_known_hosts () {
  echo "-------------------------------- Set up deploy host to known hosts --------------------------------- " &&
  grep "$1" ~/.ssh/known_hosts
  STATUS_PREV_COMMAND=$?

  if [[ ! $STATUS_PREV_COMMAND -eq 0 ]]; then
      echo "#start $1" >> ~/.ssh/known_hosts &&
      ssh-keyscan -H "$1" >> ~/.ssh/known_hosts &&
      echo "#end $1" >> ~/.ssh/known_hosts &&
      ssh -i "${ID_FILE}" "${EC2_INSTANCE_USER}"@"$1" "sudo shutdown +60" &&
      [ -d ".idea" ] &&
      sed -i '' -e "s/[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}/$1/g" .idea/webServers.xml .idea/sshConfigs.xml
  fi
}

aws_start() {
  echo '================================ Aws Start server ==================================='

  which aws
  RESULT_CODE=$?

  if [ ! $RESULT_CODE -eq 0 ]; then
    linux_install_aws
    RESULT_CODE=$?
  fi

  if [[ $RESULT_CODE -eq 0 ]]; then
    if [[ $(aws_get_instance_status) != "running" ]]; then
      echo "-------------------------------- Aws instance start --------------------------------- " &&
      aws ec2 start-instances --instance-ids "${EC2_INSTANCE}" &&
      sleep 50
    else
      echo "-------------------------------- Aws instance already started --------------------------------- "
    fi
    echo '================================ End Aws start server ==================================='
  else
  	echo "Its not possible to run ec2 instance.";
  	exit 1;
  fi
}

sfdk_deploy_to_device() {
  source ~/.zshrc &&
  eval sfdk tools list &&
  eval sfdk device list &&
  eval sfdk config device=\'Xperia 10 - Dual SIM \(ARM\)\' &&
  eval sfdk config target=SailfishOS-4.4.0.58-armv7hl &&
  cd build-bible-SailfishOS_4_4_0_58_armv7hl_in_sailfish_sdk_build_engine_ubuntu-Debug &&
  eval sfdk build ../bible &&
  eval sfdk deploy --sdk &&
  eval sfdk device exec /usr/bin/bible
}

sfdk_run_app_on_device() {
  # devel-su usermod -a -G systemd-journal nemo
  source ~/.zshrc &&
  eval sfdk device list &&
  eval sfdk config device=\'Xperia 10 - Dual SIM \(ARM\)\' &&
  eval sfdk device exec /usr/bin/bible &
  eval sfdk device exec journalctl -f /usr/bin/bible
}

prepare_aws_instance() {
  aws_start &&
  echo "-------------------------------- Ssh Session --------------------------------- " &&
  EC2_INSTANCE_HOST=$(aws_get_host) &&
  set_up_instance_aws_host_to_known_hosts "${EC2_INSTANCE_HOST}"
}
