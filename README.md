# uAlgo-PicoW
Algorand IoT tools and microSDK: An Arduino sketch for the ESP32 wireless microcontroller, demonstrating the "notarization" of sensors data via the Algorand blockchain

- For all information on how to use the project see:
https://github.com/GT50-SRL/algoiot/blob/main/doc/AlgoIoT.md

- For pitch project presentation see: https://github.com/GT50-SRL/algoiot/blob/main/doc/Pitch%20presentation%20IoT%20Algorand%5B1.0%5D.pdf
- For instructions for testing see: https://github.com/GT50-SRL/algoiot/blob/main/doc/AlgoIoT_Testing_Instructions.md

- Video presentation download and see: https://github.com/GT50-SRL/algoiot/blob/main/doc/AlgoIoT%20accompanying%20video.mp4 

![AlgoIOT_Logo1](https://github.com/Sam2much96/uAlgo-PicoW/blob/dev/Schematic_hackathon1_2024-05-20.png)

This is an example of a transaction performed directly from the device using AlgoIoT: https://testnet.algoexplorer.io/tx/OWXK33Z6DGWDTYRKU7ZD5K2GMFNDVYMWR3QUHRBY3TUDM4DIQAIQ

# Requirements

- PicoSDK
- PicoSDK Development Environment
- Arduino Crypto Libraries
- ESP 32 Crypto Libraries
- Base32 encoder and Decoder libraries

# Disclaimer
- The pico W hosts and runs all calculcations for the uAlgo-Pico W wallet, board 2 hosts the animations and display as I/O device. It is not necessary to build the and is just my implementation, you can as well build this library on a new pico w, pico or any other microcontroller sung the rp2040 chipset

# Uses
- (1) A Decentralised Trading Bot
- (2) A Hardware Wallet
- (3) Serializing IoT data to the Blockchain e.g Temparature, Voltage 

# Documentation
- < link to documentation>

# Micro Algo

Micro Algo is a WIP algorand wallet running on a Raspberry Pico W. It ports over algoiot <include link to algoiot>, an arduino algorand skecth to the pico w and implements Pico and Pico W libraries for cryptography, notably LWIP network libraries for http client

# W I P
-(1) HTTP client for pico w is new and is still being implemented for micro algo-picoW for submitting Transactions
-(2) MicroPython Port

# Third Party SOftware
- (1) Algoiot
- (2) esp32 crypto libraries <link>
- (3) Arduino Crypto Libraries <link>
- (4) PicoBot <link>
- (5) Ported Base32 encoder and decoder <https://github.com/NetRat>