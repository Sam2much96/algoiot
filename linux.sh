#!/bin/bash 


$(g++ hello_world.cpp \
-I include  \
-I include/utility \
-L include/AlgoIoT.cpp \
-L include/Ed25519.cpp \
-L include/RNG.cpp \
-L include/Curve25519.cpp \
-L include/SHA512.cpp \
-L include/Hash.cpp \
-L include/BigNumberUtil.cpp \
-L include/NoiseSource.cpp \
-L include/Crypto.cpp\ include/Crypto.cpp include/AlgoIoT.cpp include/Ed25519.cpp include/Curve25519.cpp include/SHA512.cpp include/Hash.cpp include/BigNumberUtil.cpp include/ArduinoJson.h include/RNG.cpp include/NoiseSource.cpp) 