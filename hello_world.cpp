// Connet To Pico Serial on Ubuntu : https://www.gibbard.me/using_the_raspberry_pi_pico_on_ubuntu/
// .run command in Terminal :picocom -b 115200 /dev/ttyACM0

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
#define UART_ID uart0
#define BAUD_RATE 9600
#define UART_TX_PIN 0

// LED

#define LED_PIN CYW43_WL_GPIO_LED_PIN

// Wallet Setup
// #define Wallet AlgoIoT

// Wifi and Web Server
#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"

#include "webserver/lwipopts.h"

// web hooks / handlers
#include "webserver/ssi.h"
#include "webserver/cgi.h"

#define WIFI_SSID "Airtel 4G MiFi_4C91"
#define WIFI_PASSWORD "52892608"
int main()
{
    // Blinky Led
    /*
        To DO:
        (1) Setup Uart As debug (Done)
        (2) Setup SD Card as Logging
        (3) Build Wallet and Send Outupt to Uart debug
        (4) Implement CUstom HTTP CLient CLass
        (5) Implement Port Forwarding
        (6) Data to send should be json
        (7) Implement Webserver for Board A
        (8) Implement HTTP Requests in a second thread using libcurl : https://github.com/JosephP91/curlcpp
        (9) Implement Multi-threading
        (10) Capture notes from the webserver for llm prompting
        (11) Sd card read and capture

    */

    // init all io for serial and usb debugging
    stdio_init_all();

    // init wifi
    cyw43_arch_init();

    // enable sta mode on wifi
    cyw43_arch_enable_sta_mode();

    // Configure ADC
    adc_init();

    // initialize temparature sensor
    adc_set_temp_sensor_enabled(true);

    // select 5th channel of adc as input
    adc_select_input(4);

    // LED Setup
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Set up our UART with the required speed.
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);

    // Connect to the WiFI network - loop until connected
    while (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000) != 0)
    {
        printf("Attempting to connect...\n");
    }
    // Print a success message once connected
    printf("Connected! \n");

    // Initialise web server
    httpd_init();
    printf("Http server initialised\n");

    // Configure SSI and CGI handler
    ssi_init();
    printf("SSI Handler initialised\n");
    cgi_init();
    printf("CGI Handler initialised\n");

    // Pass node debug to uart puts
    // AlgoIot Wallet Setup
    const char *app_name = "Whatever nggq\n";
    const char *mnemonic = "tank game arrive train bring taxi tackle popular bacon gasp tell pigeon error step leaf zone suit chest next swim luggage oblige opinion about execute";

    // create Algoiot object
    AlgoIoT Wallet(app_name, mnemonic);

    while (true)
    {
        // Temp calculator depreciated to ssl.h
        // Get Raw Output of ADC
        uint16_t raw = adc_read();

        // conversion logic
        const float conversion = 3.3f / (1 << 12);

        // multiply by voltage of the adc diode
        float voltage = raw * conversion;

        // convert voltage to temparature value
        float temparature = 27 - (voltage - 0.706) / 0.001721;

        // convert floats to char pointer
        char buffer[64];
        int ret = snprintf(buffer, sizeof buffer, "Temp: %f C\n", temparature);

        // print to usb serial interface
        // printf("Temparature: %f C\n", temparature);
        sleep_ms(250);

        // Blink LED
        gpio_put(LED_PIN, 1);
        //  printf("Hello, world!\n");
        uart_puts(UART_ID, buffer); // Send Board data as output via Uart, send as Json Document
        sleep_ms(300);
        gpio_put(LED_PIN, 0);
        sleep_ms(3000);
        // uart_puts(UART_ID, buffer);
        // sleep_ms(300);
        //  return 0;
    }
}