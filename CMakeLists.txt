cmake_minimum_required(VERSION 3.13)


set(PROGRAM_NAME hello_world)
set(PICO_BOARD pico_w)

# Wolf SSL Libraries
set(WOLFSSL_DIR "${CMAKE_CURRENT_LIST_DIR}/../lib/wolfssl" CACHE STRING "Course Common Lib" )
include("wolfssl_import.cmake")

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/utility
    ${CMAKE_CURRENT_SOURCE_DIR}/webserver

    )

project(my_project)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()


# for parsing html as c file
# doesnt work
message("Running makefsdata python script")
execute_process(COMMAND
    py ${CMAKE_CURRENT_SOURCE_DIR}/webserver/makefsdata.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)
add_executable(${PROGRAM_NAME}
    hello_world.cpp
    
    # aLgo Wallet variables
    include/AlgoIoT.cpp
    include/Ed25519.cpp
    include/Curve25519.cpp
    include/SHA512.cpp
    include/Hash.cpp
    include/BigNumberUtil.cpp
    include/Crypto.cpp

    #WifiHelper
    webserver/WifiHelper.cpp
    
    # Real time clock
    ${PICO_SDK_PATH}/lib/lwip/src/apps/sntp/sntp.c

    # Wolf ssl
    ${WOLFSSL_DIR}/wolfcrypt/test/test.c
    ${WOLFSSL_DIR}/wolfcrypt/src/ed25519.c
    
   
    )

# Add pico_stdlib library which aggregates commonly used features
# pico_cyw43 for wifi
# pico_lwip_http for http server
target_link_libraries(
    ${PROGRAM_NAME} 
    pico_stdlib
    hardware_adc

    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_http

    #real time clock
    hardware_rtc

    # wolfssl
    wolfssl
    )

#Usb and  Uart Serial Output# 
# 0 is off 1 is on?`    1
pico_enable_stdio_usb(${PROGRAM_NAME} TRUE)
pico_enable_stdio_uart(${PROGRAM_NAME} FALSE)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${PROGRAM_NAME})
