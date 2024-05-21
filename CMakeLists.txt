cmake_minimum_required(VERSION 3.13)


set(PROGRAM_NAME hello_world)
set(PICO_BOARD pico_w)

#set(ARDUINO_LIBS ../arduino-esp32-master)
#set(ESP_LIBS ../esp-idf-master)

# Wolf SSL Libraries
set(WOLFSSL_DIR "${CMAKE_CURRENT_LIST_DIR}/../lib/wolfssl" CACHE STRING "Course Common Lib" )


SET(FREERTOS_KERNEL_PATH "${CMAKE_CURRENT_LIST_DIR}/../lib/FreeRTOS-Kernel" CACHE STRING "Course Common Lib")
SET(FREERTOS_CONFIG_FILE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/port/FreeRTOS-Kernel" CACHE STRING "Local Config")
include_directories("${FREERTOS_CONFIG_FILE_DIRECTORY}") 


# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)


include("wolfssl_import.cmake")

# Free RTos is needed for Embedded Sockets Implementation
include(FreeRTOS_Kernel_import.cmake)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/utility
    ${CMAKE_CURRENT_SOURCE_DIR}/webserver

    # Impl Arduino Src
    #${ARDUINO_LIBS}
    #${ESP_LIBS}

   
)




project(my_project)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()


# for parsing html as c file
message("Running makefsdata python script")

execute_process(COMMAND
    py ${CMAKE_CURRENT_SOURCE_DIR}/webserver/makefsdata.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

add_executable(${PROGRAM_NAME}
    hello_world.cpp
    
    # aLgo Wallet variables
    # Depreciate all libs to duino or esp libs
    include/AlgoIoT.cpp
    include/Ed25519.cpp
    include/Curve25519.cpp
    include/SHA512.cpp
    include/Hash.cpp
    include/BigNumberUtil.cpp
    include/Crypto.cpp

    # arduino nd esp 32 libs?
    #${ARDUINO_LIBS}/libraries/HTTPClient/src/HTTPClient.cpp
    #${ARDUINO_LIBS}/libraries/HTTPClient/src/HTTPClient.h
    
    #WifiHelper
    webserver/WifiHelper.cpp

    # Real time clock
    ${PICO_SDK_PATH}/lib/lwip/src/apps/sntp/sntp.c

    #HTTPRequest
    webserver/requests.hpp

    # Free RTOS
    port/FreeRTOS-Kernel/IdleMemory.c
    port/FreeRTOS-Kernel/logging_levels.h
    port/FreeRTOS-Kernel/logging_stack.h
   
)

# Add pico_stdlib library which aggregates commonly used features
# pico_cyw43 for wifi
# pico_lwip_http for http server
# pico_lwip_mbedtls & pico_mbedtls  for tls communication
target_link_libraries(
    ${PROGRAM_NAME} 
    pico_stdlib
    hardware_adc
    hardware_rtc
    #pico_cyw43_arch_lwip_threadsafe_background # reguired for server hosting
    pico_lwip_http
    pico_lwip_mbedtls
    pico_mbedtls 

    pico_cyw43_arch_lwip_sys_freertos
    FreeRTOS-Kernel-Heap4
    wolfssl
    )

#Usb and  Uart Serial Output# 
# 0 is off 1 is on?`    1
pico_enable_stdio_usb(${PROGRAM_NAME} TRUE)
pico_enable_stdio_uart(${PROGRAM_NAME} FALSE)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${PROGRAM_NAME})
