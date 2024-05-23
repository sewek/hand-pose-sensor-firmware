
FILE(GLOB sources main.c ble/ble_common.c)
#FILE(GLOB sources ble/ble_common.c)
#FILE(GLOB sources ble/services/sensor_service.c)
#FILE(GLOB sources imu/imu.c)
target_sources(app PRIVATE ${sources})
#zephyr_include_directories(.)
