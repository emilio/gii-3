#!/bin/bash

cd "$(dirname "$0")"

LOG_FILE=$(mktemp)
checked_command() {
  echo -e "\e[34mtest:\e[0m $@"
  $@ > $LOG_FILE 2>&1
  if [ $? -ne 0 ]; then
    echo -e "\e[34mtest:\e[0m \e[91mcommand failed:\e[0m $@"
    echo -e "\e[34mtest:\e[0m \e[91moutput:\e[31m"
    cat $LOG_FILE
    echo -e "\e[0m"
    rm $LOG_FILE
    exit 1
  fi
}

sudo perl ../app.pl -d

for file in *.pl; do
  checked_command perl $file;
done

# TODO: This is not as correct as it should be
sudo killall perl
rm $LOG_FILE
