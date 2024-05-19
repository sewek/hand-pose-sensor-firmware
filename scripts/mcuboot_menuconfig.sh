#!/bin/bash

./entrypoint.sh west build -t mcuboot_menuconfig -c -p always -b hand_pose_sensor -- -DBOARD_ROOT=./