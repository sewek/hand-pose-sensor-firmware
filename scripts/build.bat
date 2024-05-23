@echo off

entrypoint.bat west build -c -p always -b hand_pose_sensor -- -DBOARD_ROOT=./
