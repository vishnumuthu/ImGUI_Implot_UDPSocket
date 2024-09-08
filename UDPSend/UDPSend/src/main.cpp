// C++ UDP Transmitter

#include "Network.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

#pragma once

int main()
{
    std::string IP = "127.0.0.1";
    int PORT = 8888;

    try
    {
        WSASession Session;
        UDPSocket Socket;
        std::string data = "hello world";
        char buffer[100];

        // create a clock and start timer
        clock_t TimeZero = clock(); //Start timer
        std::cout << "Start time: " << TimeZero << std::endl;

        double deltaTime = 0;
        double hz_value = 10;
        double reset_time = 1000 / hz_value;

        // generate random seed using time 
        srand(time(0));

        while (1)
        {

            // get delta time in milliseconds
            deltaTime = (clock() - TimeZero);

            // compare if delta time is 2 or more seconds
            if (deltaTime > reset_time) {

                struct tm newtime;
                time_t now = time(0);
                localtime_s(&newtime, &now);
                //std::cout << "Current local time : " << newtime.tm_hour << ":" << newtime.tm_min << ":" << newtime.tm_sec <<std::endl;

                // generate new random number
                int i = rand() % 100 + 1;
                //std::cout  << ": New random : " << i << "\n";
                data = std::to_string(newtime.tm_hour) +":" + std::to_string(newtime.tm_min) + ":" + std::to_string(newtime.tm_sec) + " " + std::to_string(i);
                std::cout << data << std::endl;
                Socket.SendTo(IP, PORT, data.c_str(), data.size());

                //reset the clock timers
                TimeZero = clock();
            }

            
        }
    }
    catch (std::exception& ex)
    {
        std::cout << ex.what();
    }
}
