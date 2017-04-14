project(template CXX C ASM)
cmake_minimum_required(VERSION 3.8)


set(CMAKE_ASM_COMPILER arm-none-eabi-as)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_AR arm-none-eabi-ar)

set(COMMON_FLAGS "-DSTM32F10X_HD -DSTM32F103xB -DUSE_STDPERIPH_DRIVER                                      \
    -mcpu=cortex-m3 -lc -specs=nosys.specs -mthumb -mthumb-interwork -ffunction-sections -fdata-sections -g -fno-common -fmessage-length=0")

set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${COMMON_FLAGS} -std=gnu99")
set(CMAKE_ASM_FLAGS "-mthumb -mcpu=cortex-m3")

set(HEX_FILE ${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.bin)

set(USER_INCLUDE users)
set(USER_TOOL_INCLUDE users/tool)
set(CMSIS_DEVICE_INCLUDE driver/CMSIS/CM3/CoreSupport)
set(CMSIS_INCLUDE driver/CMSIS/CM3/DeviceSupport/ST/STM32F10x)
set(STD_INCLUDE driver/STM32F10x_StdPeriph_Driver/inc)

file(GLOB_RECURSE USER_INCLUDE_F ${USER_INCLUDE}/*.h ${USER_INCLUDE}/*.hpp)
file(GLOB_RECURSE USER_TOOL_INCLUDE_F ${USER_INCLUDE}/tool/*.h ${USER_INCLUDE}/tool/*.hpp)
file(GLOB_RECURSE CMSIS_DEVICE_INCLUDE_F ${CMSIS_DEVICE_INCLUDE}/*.h)
file(GLOB_RECURSE CMSIS_INCLUDE_F ${CMSIS_INCLUDE}/*.h)
file(GLOB_RECURSE STD_INCLUDE_F ${STD_INCLUDE}/*.h)

file(GLOB_RECURSE USER_SOURCES users/*.c users/*.cpp)
file(GLOB_RECURSE USER_TOOL_SOURCES users/tool/*.c users/tool/*.cpp)
file(GLOB_RECURSE STD_SOURCES driver/STM32F10x_StdPeriph_Driver/src/*.c)
file(GLOB_RECURSE CMSIS_SYSTEM driver/CMSIS/CM3/DeviceSupport/ST/STM32F10x/*.c)
file(GLOB_RECURSE CMSIS_STARTUP driver/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_hd.s)

set(SOURCE_FILES ${USER_INCLUDE_F} ${USER_TOOL_INCLUDE_F} ${USER_TOOL_SOURCES} ${CMSIS_DEVICE_INCLUDE_F} ${CMSIS_INCLUDE_F} 
    ${STD_INCLUDE_F} ${USER_SOURCES} ${STD_SOURCES} ${CMSIS_STARTUP} ${CMSIS_SYSTEM} STM32F103C8Tx_FLASH.ld)

include_directories(${USER_TOOL_INCLUDE} ${USER_INCLUDE} ${CMSIS_DEVICE_INCLUDE} ${CMSIS_INCLUDE} ${STD_INCLUDE}
        /usr/lib/arm-none-eabi/include)

add_executable(${PROJECT_NAME}.elf ${SOURCE_FILES})

set(CMAKE_EXE_LINKER_FLAGS "-Wl,-gc-sections -T ${CMAKE_SOURCE_DIR}/STM32F103C8Tx_FLASH.ld")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${PROJECT_SOURCE_DIR}/build/${PROJECT_NAME}.map")


add_custom_command(TARGET ${PROJECT_NAME}.elf PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/build
    COMMENT "Creating build directory")
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
    COMMENT "Building ${HEX_FILE}\nBuilding ${BIN_FILE}")

add_custom_target(flash
    stm32flash -w ${BIN_FILE} /dev/ttyUSB0)
add_custom_target(chmod
    sudo chmod 777 /dev/ttyUSB0)

