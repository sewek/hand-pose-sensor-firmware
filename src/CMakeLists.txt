
FILE(GLOB sources ./*.c)
target_sources(app PRIVATE ${sources})
zephyr_include_directories(.)