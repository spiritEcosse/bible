#!/bin/bash

# e - script stops on error (return != 0)
# u - error if undefined variable
# o pipefail - script fails if one of piped command fails
# x - output each line (debug)
set -euox pipefail


if [[ -z ${ARCH+x} ]]; then
  ARCH=$(uname -m)
fi

get_name_platform() {
  awk -F= '$1=="ID" { print $2 ;}' /etc/os-release
}

if [[ -z ${PLATFORM+x} ]]; then
  PLATFORM=$(get_name_platform)
fi

FILE=${PLATFORM}_${ARCH}.tar.gz
BUILD_FOLDER="${HOME}/${PLATFORM}_${ARCH}"
DESTINATION_PATH="/home/ec2-user/"

aws_get_host() {
  EC2_INSTANCE_HOST=$(aws ec2 describe-instances --instance-ids "${EC2_INSTANCE}" --query "Reservations[*].Instances[*].[PublicIpAddress]" --output text)
  while [[ "$EC2_INSTANCE_HOST" == "None" ]]
  do
    sleep 5
    aws_get_host
  done
}

aws_wait_status_running() {
  EC2_INSTANCE_STATUS=$(aws_get_instance_status)
  while [[ "$EC2_INSTANCE_STATUS" != "running" ]]
  do
    sleep 5
    aws_wait_status_running
  done
}

aws_stop() {
  aws ec2 stop-instances --instance-ids "${EC2_INSTANCE}"
}

aws_get_instance_status() {
  aws ec2 describe-instance-status --instance-ids "${EC2_INSTANCE}" --query "InstanceStatuses[*].InstanceState.Name" --output text
}

linux_install_aws() {
  sudo zypper -n install python3-pip
  sudo pip install awscli
  aws --version
  mkdir ~/.aws/
  echo "[default]
  region = ${AWS_REGION}" > ~/.aws/config
}

set_up_instance_aws_host_to_known_hosts () {
  if ! grep "$1" ~/.ssh/known_hosts; then
      SSH_KEYSCAN=$(ssh-keyscan -T 180 -H "$1")
      printf "#start %s\n%s\n#end %s" "$1" "$SSH_KEYSCAN" "$1" >> ~/.ssh/known_hosts
      ssh -i "${ID_FILE}" "${EC2_INSTANCE_USER}"@"$1" "sudo shutdown +60"

      if [[ -d ".idea" ]]; then
        sed -i '' -e "s/[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}/$1/g" .idea/webServers.xml .idea/sshConfigs.xml
      fi
  fi
}

aws_start() {
  if ! which aws; then
    linux_install_aws
  fi

  if [[ $(aws_get_instance_status) != "running" ]]; then
    aws ec2 start-instances --instance-ids "${EC2_INSTANCE}"
    aws_wait_status_running
    sleep 60
  fi
}

sfdk_deploy_to_device() {
  source ~/.zshrc &&
  eval sfdk tools list &&
  eval sfdk device list &&
  eval sfdk config device=\'Xperia 10 - Dual SIM \(ARM\)\' &&
  eval sfdk config target=SailfishOS-4.4.0.58-"$ARCH" &&
  cd build-bible-SailfishOS_4_4_0_58_"$ARCH"_in_sailfish_sdk_build_engine_ubuntu-Debug &&
  eval sfdk build ../bible &&
  eval sfdk deploy --sdk &&
  eval sfdk device exec /usr/bin/bible &
  eval sfdk device exec journalctl -f /usr/bin/bible
}

sailfish_run_tests_on_aws() {
  source ~/.zshrc
  eval sfdk tools list
  eval sfdk config target=SailfishOS-4.4.0.58-"$ARCH"
  cd build-bible-SailfishOS_4_4_0_58_"$ARCH"_in_sailfish_sdk_build_engine_ubuntu-Debug
  eval sfdk build ../bible -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/usr -DBUILD_TESTING=ON -DCODE_COVERAGE=ON
  eval sfdk build-shell ctest --output-on-failure
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
  aws_start
  aws_get_host
  set_up_instance_aws_host_to_known_hosts "${EC2_INSTANCE_HOST}"
}

download_backup() {
  cd /home/mersdk/
  echo "${IDENTITY_FILE}" > "${ID_FILE}"
  chmod 600 "${ID_FILE}"
  mkdir -p ~/.ssh/
  touch ~/.ssh/known_hosts
  prepare_aws_instance
  SEC=$SECONDS
  scp -i "${ID_FILE}" "${EC2_INSTANCE_USER}@${EC2_INSTANCE_HOST}:${DESTINATION_PATH}${FILE}" .
  echo "after scp : $(( SECONDS - SEC ))"
  SEC=$SECONDS
  tar -xf "${FILE}"
  echo "after tar : $(( SECONDS - SEC ))"
  ls -la .
}

upload_backup() {
#  cd /home/mersdk/
#  tar -zcf "${FILE}" "${BUILD_FOLDER}" &&
#  scp -i "${ID_FILE}" "${FILE}" "${EC2_INSTANCE_USER}"@"${EC2_INSTANCE_HOST}":~/backups/
  aws_stop
}

mb2_cmake_build() {
  cd ${BUILD_FOLDER}
  mb2 build-init
  mb2 build-requires
  mb2 cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/usr -DBUILD_TESTING=ON -DCODE_COVERAGE=ON
  mb2 cmake --build .
}

mb2_run_tests() {
  cd ${BUILD_FOLDER}
  mb2 build-shell ctest --output-on-failure
}

mb2_run_ccov_all_capture() {
  cd ${BUILD_FOLDER}
  mkdir ccov
  mb2 build-shell make ccov-all-capture
}

codecov_push_results() {
  cd ${BUILD_FOLDER}
  curl -Os https://uploader.codecov.io/latest/linux/codecov &&
  chmod +x codecov &&
  ./codecov -t ${CODECOV_TOKEN} -f ccov/all-merged.info
}

rsync_share_to_build() {
  cd "${BUILD_FOLDER}"
  ls -la .
  sudo rsync -rv --checksum --ignore-times --info=progress2 --stats --human-readable --exclude '.git/modules' /share/ .
  sudo chown -R mersdk:mersdk .
  ls -la .
}

code_coverage() {
  download_backup
  rsync_share_to_build
  mb2_cmake_build
#  upload_backup
#  mb2_run_tests
#  mb2_run_ccov_all_capture
#  codecov_push_results
}
