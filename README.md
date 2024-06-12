# uAlgo-PicoW
A Firmware sketch for the Raspberry Pico W microcontroller, demonstrating hardware wallet functionalities for Algorand blockchain on the Raspberry Pico aand Pico W

- For all information on how to use the project see:
https://github.com/GT50-SRL/algoiot/blob/main/doc/AlgoIoT.md


![AlgoIOT_Logo1](https://github.com/Sam2much96/uAlgo-PicoW/blob/dev/logo.png)
![board_schematics](https://github.com/Sam2much96/algoiot/blob/dev/Schematic_hackathon1_2024-05-20.png)
![AlgoIOT_Logo2](https://github.com/GT50-SRL/algoiot/assets/2614303/ba16d833-7ad2-47f5-8bae-315dbd41f9d2)
![test](https://github.com/Sam2much96/uAlgo-PicoW/blob/dev/2024-06-09-162435_1366x768_scrot.png)


# What It Is
Traditional wallets (Hot Wallets) store your Secret keys in the web browser or on your smartphone. This is a security risk because if your device is ever hacked or damaged, your funds are gone & malicious actors can do whatever they want with it; So this device here acts as an opensource hardware wallet (cold wallet). It stores your secret keys on the Pico w's flashrom, and the device itself can be programmed to approve transactions on your behalf. The pico w also hosts a local webserver on your wifi network which can be used alongside the hardware for storing and decoding secret keys securely and automating transactions. The demo video below shows the device decoding a Testnet secret key into it's associated Blockchain Address. 

![Demo_Video](https://img.youtube.com/vi/eb43BSEec0U/maxresdefault.jpg)(https://www.youtube.com/shorts/eb43BSEec0U)

# Requirements

- (1) PicoSDK
- (2) PicoSDK Development Environment
- (3) Arduino Crypto Libraries
- (4) ESP 32 Crypto Libraries
- (5) Base32 encoder and Decoder libraries

# Disclaimer
- The pico W hosts and runs all calculcations for the uAlgo-Pico W wallet, board 2 hosts the animations and display as I/O device. It is not necessary to build the and it's just used in my implementation for easy debugging, you can as well build this library on a fresh pico w, pico or any other microcontroller supporting the rp2040 chipset

# Uses
- (1) A Decentralised Trading Bot
- (2) A Hardware Wallet
- (3) Serializing IoT data to the Blockchain e.g Temparature, Voltage 

# Documentation
- < link to documentation>

# What Can I Do With It?
Virtually anything, it's an opensource sketch and can be used to program an algorand wallet alongside any other existing pico or pico w functionalities

# Micro Algo

Micro Algo is a WIP algorand wallet running on a Raspberry Pico W. It ports over algoiot <include link to algoiot>, an arduino algorand skecth to the pico w and implements Pico and Pico W libraries for cryptography, notably LWIP network libraries for http client

# W I P
-(1) HTTP client for pico w is new and is still being implemented for micro algo-picoW for submitting Transactions
-(2) MicroPython Port < link to picobot>

# Third Party SOftware
- (1) Algoiot
- (2) esp32 crypto libraries <link>
- (3) Arduino Crypto Libraries <link>
- (4) PicoBot <link>
- (5) Ported Base32 encoder and decoder <https://github.com/NetRat>

# How To Build
- (1) Setup PICO_SDK_PATH for building for pico c++ 
- (2) Build the Local Webpage for self hosting on the pico w

```
$ cd webserver
$ python3 makesfdata.py
```

- (3) Build the Project 

```
$ cd build
$ cmake .. && make
```

- (4) Copy and Paste the Build .uf2 in the pico w after being put in bootsel mode
- (5) Run this command in Terminal for debugging

```
$ picocom -b 115200 /dev/ttyACM0
```

