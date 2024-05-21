// Connet To Pico Serial on Ubuntu : https://www.gibbard.me/using_the_raspberry_pi_pico_on_ubuntu/
// .run command in Terminal :picocom -b 115200 /dev/ttyACM0
//
/*
    To DO:
    (1) Setup Uart As debug (Done)
    (2) Setup SD Card as Logging (implemented in board B)
    (3) Build Wallet and Send Outupt to Uart debug (done)
    (4) Implement CUstom HTTP CLient CLass (Sockets isn't well implemented in pico-sdk requires FreeRtos sockets libraries)
    (5) Implement Port Forwarding (depreciated feature)
    (6) Data to send should be json/structs (done)
    (7) Implement Webserver for Board A (done)
    (8) Implement HTTP Requests in a second thread using libcurl : https://github.com/JosephP91/curlcpp
    (9) Implement Multi-threading using freertos (done)
    (10) Capture notes from the webserver for llm prompting (cut feature)
    (11) Sd card read and capture (cut feature)

*/

#include <stdio.h>
#include <pico/stdlib.h>

// Boolean
#include <stdbool.h>

// Uart
#include <hardware/uart.h>

// Wallet
#include "include/AlgoIoT.h"

// Get Chip Temparature
#include <hardware/adc.h>

//  Uart Setup
//  Board B Uses Tx Pin
#define UART_ID uart1
#define BAUD_RATE 9600
#define UART_TX_PIN 4

// LED

#define LED_PIN CYW43_WL_GPIO_LED_PIN

// Wallet Setup
// #define Wallet AlgoIoT

// Wifi Controls, Web Server Config file
#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"

#include "webserver/lwipopts.h"
#include "webserver/WifiHelper.h"

// web hooks / handlers
#include "webserver/ssi.h"
#include "webserver/cgi.h"

// Net Code
#include "lwip/ip4_addr.h"
// #include "lwip/sockets.h"
#include "lwip/dns.h"

// Time
#include "pico/util/datetime.h"
#include <time.h>
#include "hardware/rtc.h"

#define WIFI_SSID "Galaxy A0220d1"
#define WIFI_PASSWORD "rxlz8491"

// HTTP Requests
// #include "webserver/requests.hpp"

// FreeRTOS OS Task Scheduler
#include <FreeRTOS.h>
#include <task.h>
#define TASK_PRIORITY (tskIDLE_PRIORITY + 1UL)
// define os tasks to run
void server_task(void *pvParameters)
{
    // Initialise web server
    httpd_init();
    printf("Http server initialised\n");

    // Configure SSI and CGI handler
    ssi_init();
    printf("SSI Handler initialised\n");
    cgi_init();
    printf("CGI Handler initialised\n");

    while (1)
    {
    }
};
void http_request_task(void *pvParameters)
{
    // Test HTTP Request
    // HTTPReq test;
    // testTrans.testTrans();

    // test.test();
    printf("Http Request is unimplemented");
};
void uart_task(void *pvParameters)
{
    // Set up our UART with the required speed.
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);

    datetime_t d;
    // board data to send across network, Concatonate ip addr and temp data to struct
    struct INFO
    {

        char name[50];
        char ip[20];
        int temparature;
        char datetime[20];
        int year;
        int month;
        int day;
        int hour;
        int min;
        int sec;
    };

    struct INFO my_board;
    {
        /* data */
    };
    char frame[120];

    // save ip addr
    // strcpy(my_board.ip, ipStr);

    // get current
    rtc_get_datetime(&d);

    // printf("RTC: %d-%d-%d %d:%d:%d\n",
    //        d.year,
    //        d.month,
    //        d.day,
    //        d.hour,
    //        d.min,
    //        d.sec);

    // copy struct to character
    // memcpy(frame, (char *)&my_board, sizeof(frame));

    // Blink LED : refactored to ssi or cgi handler for server control
    // gpio_put(LED_PIN, 1);
    // printf("Hello, world! %d\n", my_board.hour);
    // rtc data is corrupted
    std::string rtc_time = std::to_string(("\n RTC:  %d-%d-%d %d:%d:%d\n",
                                           d.year,
                                           d.month,
                                           d.day,
                                           d.hour,
                                           d.min,
                                           d.sec)); // convert int to str

    printf(rtc_time.c_str());
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    printf("LED tuned ON!\n");
    vTaskDelay(500); // Delay by TICKS defined by FreeRTOS priorities
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    printf("LED turned OFF\n");
    uart_puts(UART_ID, rtc_time.c_str()); // Send Board data as output via Uart, send as Json Document

    while (1)
    {
    }
};

void wifi_task(void *pvParameters)
{

    // init wifi

    if (WifiHelper::init())
    {
        printf("Wifi Controller Initialised\n");
    }
    else
    {
        printf("Failed to initialise controller\n");
        // return 0;
    }

    printf("Connecting to WiFi... %s \n", WIFI_SSID);

    if (WifiHelper::join(WIFI_SSID, WIFI_PASSWORD))
    {
        printf("Connect to Wifi\n");
    }
    else
    {
        printf("Failed to connect to Wifi \n");
    }
    // Connection Failed Retry
    if (!WifiHelper::isJoined())
    {
        printf("AP Link is down\n");

        if (WifiHelper::join(WIFI_SSID, WIFI_PASSWORD))
        {
            printf("Connect to Wifi\n");
        }
        else
        {
            printf("Failed to connect to Wifi \n");
        }
        // Print MAC Address
        char macStr[20];
        WifiHelper::getMACAddressStr(macStr);
        printf("MAC ADDRESS: %s\n", macStr);

        // Print IP Address
        char ipStr[20];
        WifiHelper::getIPAddressStr(ipStr);
        printf("IP ADDRESS: %s\n", ipStr);

        WifiHelper::sntpAddServer("0.africa.pool.ntp.org");
        WifiHelper::sntpAddServer("1.africa.pool.ntp.org");
        WifiHelper::sntpAddServer("2.africa.pool.ntp.org");
        WifiHelper::sntpAddServer("3.africa.pool.ntp.org");
        WifiHelper::sntpSetTimezone(0);
        WifiHelper::sntpStartSync();
    };
}
void wallet_task(void *pvParameters)
{
    // AlgoIot Wallet Setup
    const char *app_name = "Whatever nggq\n";
    const char *mnemonic = "tank game arrive train bring taxi tackle popular bacon gasp tell pigeon error step leaf zone suit chest next swim luggage oblige opinion about execute";

    // create Algoiot object
    AlgoIoT Wallet(app_name, mnemonic);

    while (1)
    {
    }
};

void start_tasks()
{
    // Create Your Task
    xTaskCreate(
        wifi_task,   // Task to be run
        "WIFI_TASK", // Name of the Task for debugging and managing its Task Handle
        1024,        // Stack depth to be allocated for use with task's stack (see docs)
        NULL,        // Arguments needed by the Task (NULL because we don't have any)
        1,           // Task Priority - Higher the number the more priority [max is (configMAX_PRIORITIES - 1) provided in FreeRTOSConfig.h]
        NULL         // Task Handle if available for managing the task
    );

    xTaskCreate(wallet_task, "UART_TASK", 1024, NULL, 1, NULL);
    xTaskCreate(uart_task, "UART_TASK", 1024, NULL, 1, NULL);

    // Should start you scheduled Tasks (such as the LED_Task above)
    vTaskStartScheduler();

    while (true)
    {
        // Your program should never get here
    };
};

int main(void)
{
    // init all io for serial and usb debugging
    stdio_init_all();

    // create queue for storing data between tasks, default 1
    // xQueue = xQueueCreate(1, sizeof(uint));
    start_tasks();
    return 0;
}
