#!/bin/bash

g++ src/main.cpp src/network/Network.cpp src/network/Server.cpp -I/usr/local/include -lsfml-graphics -lsfml-window -lsfml-network -lsfml-system -lpthread -L/usr/local/lib -o snake-net-server -std=c++11
