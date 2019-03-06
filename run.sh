#!/usr/bin/env bash

USB_PORT="/dev/ttyUSB0"
FQBN="esp8266:esp8266:nodemcu"
DIR="main"
MODE=$1
BROKER_IP="192.168.0.104"
BASE_TOPIC="homie/"
DEVICE_ID="gong"
FIRMWARE_PATH="build/main.ino.bin"

if [ "$MODE" = "ota" ] || [ -z "$MODE" ]
then
    echo "Updating via OTA..."
        python ota/ota_updater.py -l $BROKER_IP -t $BASE_TOPIC -i $DEVICE_ID $FIRMWARE_PATH
    exit 1
fi

if [ "$MODE" = "get" ] || [ -z "$MODE" ]
then
    echo "Cleaning repo..."
    git reset --hard
    echo "Getting new changes..."
    git pull
fi

cd $DIR

if [ "$MODE" = "compile" ] || [ -z "$MODE" ]
then
    echo "Compiling..."
    arduino-cli compile --fqbn $FQBN
fi

if [ "$MODE" = "upload" ] || [ -z "$MODE" ]
then
    echo "Uploading..."
    arduino-cli upload -p $USB_PORT --fqbn $FQBN
fi

cd ../

exit 1
