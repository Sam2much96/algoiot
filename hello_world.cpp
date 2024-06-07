// Connet To Pico Serial on Ubuntu : https://www.gibbard.me/using_the_raspberry_pi_pico_on_ubuntu/
// .run command in Terminal :picocom -b 115200 /dev/ttyACM0

#include <stdio.h>
#include <pico/stdlib.h>

// Boolean
#include <stdbool.h>

// Uart
#include <hardware/uart.h>

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
// #define Wallet AlgoIoT// Wallet
#include "include/AlgoIoT.h"
#include "include/base32decode.h"

// Wifi and Web Server
#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "webserver/WifiHelper.h"
#include "webserver/lwipopts.h"

// web hooks / handlers
#include "webserver/ssi.h"
#include "webserver/cgi.h"

// for API calls and requests
// #include "webserver/requests.h"

#define WIFI_SSID "Galaxy A0220d1" //"itel A60"
#define WIFI_PASSWORD "rxlz8491"   //"00000001"

#define APP_NAME "Hardware Wallet Dev"
#define MNEMONIC "tank game arrive train bring taxi tackle popular bacon gasp tell pigeon error step leaf zone suit chest next swim luggage oblige opinion about execute"
int main()
{
    // Blinky Led
    /*
        To DO:
        (1) Setup Uart As debug (Done)
        (2) Setup SD Card as Logging
        (3) Build Wallet and Send Outupt to Uart debug (Done)
        (4) Implement CUstom HTTP CLient CLass (Done)
        (5) Implement Port Forwarding
        (6) Data to send should be json
        (7) Implement Webserver for Board A (done)
        (8) Implement HTTP Requests (done)
        (9) Implement Multi-threading
        (10) Capture notes from the webserver for llm prompting
        (11) Sd card read and capture

    */

    // init all io for serial and usb debugging
    stdio_init_all();

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

    // Set up our UART with the required speed.
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);

    // const char *mnemonicQr = "tank game arrive train bring taxi tackle popular bacon gasp tell pigeon error step leaf zone suit chest next swim luggage oblige opinion about execute \n";
    //  Pass node debug to uart puts
    //  AlgoIot Wallet Setup

    // Print the derived address
    // printf("pub key: %d", (char *)Wallet.m_senderAddressBytes);
    sleep_ms(10000);
    // make request to api server
    //
    // https_get_request();
    
    if( WifiHelper::isJoined())
    {
        // Initialise web server
        httpd_init();
        printf("Http server initialised\n");

        // Configure SSI and CGI handler
        ssi_init();
        printf("SSI Handler initialised\n");
        cgi_init();
        printf("CGI Handler initialised\n");

    }

    // wallet
    //  create Algoiot object as Wallet
    AlgoIoT Wallet(APP_NAME, MNEMONIC);
    uint8_t *address;

    // Derive the Algorand address associated with this mnemonic
    address = Wallet.generateAlgorandAddress(Wallet.m_senderAddressBytes, address);
    printf("address: %s\n", address);

    while (true)
    {

        // Blink LED
        uart_puts(UART_ID, (char *)address); // Send Board data as output via Uart, send as Json Document
        sleep_ms(3000);                      // sleep for 3 secs
    }
}