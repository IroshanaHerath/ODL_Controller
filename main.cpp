/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: IROSHANA
 *
 * Created on December 16, 2020, 9:57 AM
 */

#include "serialib.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <iostream>
#include <bits/stdc++.h>
#include <time.h>
 
using namespace std;


#if defined (_WIN32) || defined(_WIN64)
    #define SERIAL_PORT "COM5"
#endif
#ifdef __linux__
    #define SERIAL_PORT "/dev/ttyACM0"
#endif

serialib serial;
char buf[1000];

int startPosition, endPosition, step, iterations;
#define TIMEOUT 20
#define MAXSTEP 627125

int initSerial()
{
    printf("Initializing Serial Port : %s\n",SERIAL_PORT);
    // Connection to serial port
    char errorOpening = serial.openDevice(SERIAL_PORT, 9600);


    // If connection fails, return the error code otherwise, display a success message
    if (errorOpening!=1)
    {
        printf ("Failed connection to %s\n",SERIAL_PORT);
        exit(0);
    }
    sleep(5);
    printf ("Successful connection to %s\n",SERIAL_PORT);    
    return 1;
}

float getParam()
{
    float val = -1;
    time_t start = time (NULL);
    
    char value[10];
    while(time (NULL) - start < TIMEOUT)
    {
        memset(buf, 0, 1000*sizeof(buf[0]));
        if (serial.available()) 
        {
            serial.readString(buf,'\n', 1000);
            printf("%s", buf);
            if(memcmp(buf, "TIME", 4) == 0 || memcmp(buf, "STEP", 4) == 0)
            {
                strcpy(value, buf+5);
                printf("Value = %s", value);
                val = std::stof(value);
            }
            if(memcmp(buf, "Done", 4) == 0)
                return val;
        }
    }
    return val;
}

int checkDone()
{
    int len = 0;
    time_t start = time (NULL);
    while(time (NULL) - start < TIMEOUT)
    {
        memset(buf, 0, 1000*sizeof(buf[0]));
        if (serial.available()) 
        {
            len = serial.readString(buf,'\n', 1000);
            printf("%s", buf);
            if(memcmp(buf, "Done", 4) == 0)
                return 1;
        }
    }
    return 0;
}

int initDevice(){
    
    /*std::string cmd;
    cmd = "J?";*/
    //serial.flushReceiver();
    //serial.writeString("J?\r\n");
    /*serial.writeChar('\r');
    serial.writeChar('\n');*/
    //serial.flushReceiver();
    sleep(1);
    serial.writeChar('J');
    usleep(200000);
    serial.writeChar('?');
    usleep(200000);
    serial.writeChar('\r');
    usleep(200000);
    serial.writeChar('\n');
    usleep(10000);
    //serial.flushReceiver();
    
    if(checkDone() > 0)
    {
        printf("OLD connection Success...\n");
    }    
    else
    {
        printf("No Response from OLD...!!!\n");
        return 0;
    }
    sleep(1);
    //serial.flushReceiver();
    //cmd = "GR";
    //serial.flushReceiver();
    //serial.writeString("GR\r\n");
    /*serial.writeChar('\r');
    serial.writeChar('\n');*/
    serial.writeChar('G');
    usleep(200000);
    serial.writeChar('R');
    usleep(200000);
    serial.writeChar('\r');
    usleep(200000);
    serial.writeChar('\n');
    usleep(10000);
    //serial.flushReceiver();
    
    if(checkDone() > 0)
    {
        printf("Moved To Home Position...\n");
    }    
    else
    {
        printf("Moving to Home Position Failed...!!!\n");
        return 0;
    }
    sleep(1);
    //serial.flushReceiver();
    //cmd = "S?";
    //serial.flushReceiver();
    //serial.writeString("S?\r\n");
    /*serial.writeChar('\r');
    serial.writeChar('\n');*/
    serial.writeChar('S');
    usleep(200000);
    serial.writeChar('?');
    usleep(200000);
    serial.writeChar('\r');
    usleep(200000);
    serial.writeChar('\n');
    usleep(10000);
    //serial.flushReceiver();
    
    if(getParam() == 0)
    {
        printf("Home Count STEP is 0...\n");
    }    
    else
    {
        printf("Home Count STEP is not 0...!!!\n");
        return 0;
    }
    sleep(1);
    //serial.flushReceiver();
    //cmd = "T?";
    //serial.flushReceiver();
    //serial.writeString("T?\r\n");
    /*serial.writeChar('\r');
    serial.writeChar('\n');*/
    serial.writeChar('T');
    usleep(200000);
    serial.writeChar('?');
    usleep(200000);
    serial.writeChar('\r');
    usleep(200000);
    serial.writeChar('\n');
    usleep(10000);
    //serial.flushReceiver();
    
    if(getParam() == 0)
    {
        printf("Home Time Delay is 0...\n");
    }    
    else
    {
        printf("Home Time Delay is not 0...!!!\n");
        return 0;
    }
    
    printf("OLD Initialized Successfully...\n");
    return 1;
    
}

int runDevice(){
    
    for(int i = 0; i < iterations; i++){
        int currentPosition = startPosition;
        if(currentPosition < 0 || currentPosition > MAXSTEP){
            printf("Step %d Out of Range...!!!\n", currentPosition);
            return 0;
        }
        
        
        
        for(int j = 0; j < abs(endPosition - startPosition)/abs(step) + 1; j++){
            std::string cmd = "S" + std::to_string(currentPosition);
            printf("Command : %s", cmd.c_str());
            uint32_t num = currentPosition;
            char digits[6], cnt = 0;
            do{
                digits[cnt] = num%10;
                printf("Command : %d", digits[cnt]);
                cnt++;
                num /= 10;
            }while(num > 0);
            printf("\n");
            //serial.writeString(cmd.c_str());
            //serial.flushReceiver();
            usleep(20000);
            serial.writeChar('S');
            usleep(100000);
            for(int k = cnt; k > 0 ; k--){
                serial.writeChar(digits[k-1] + '0');
                usleep(100000);
            }
            //serial.writeChar('\r');
            //serial.writeChar('\n');
            serial.writeChar('\r');
            usleep(100000);
            serial.writeChar('\n');
            usleep(100000);
            auto start = std::chrono::high_resolution_clock::now();
            //printf("Command Sent....\n");
            
            if(checkDone() > 0)
            {
                auto elapsed = std::chrono::high_resolution_clock::now() - start;
                long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        
                printf("Step %d Success...\n", currentPosition);
                printf("Time for Step Operation : %lu us\n", microseconds);
                currentPosition += step;
                if(currentPosition < 0 || currentPosition > MAXSTEP){
                    printf("Step %d Out of Range...!!!\n", currentPosition);
                    return 1;
                }
            }    
            else
            {
                printf("Step %d failed...!!!\n", currentPosition);
                return 0;
            }
            sleep(1);
        }
        
        
    }
    
    printf("Device Operation Successfully Completed...\n");
    return 1;
}

int getUserInputs()
{
    printf("Enter Start Position : ");
    scanf("%d", &startPosition);
    printf("Value : %d\n", startPosition);
    
    
    printf("Enter End Position : ");
    scanf("%d", &endPosition);
    printf("Value : %d\n", endPosition);
    
    printf("Enter Step Size : ");
    scanf("%d", &step);
    printf("Value : %d\n", step);
    
    printf("Enter Number of Iterations : ");
    scanf("%d", &iterations);
    printf("Value : %d\n", iterations);
    
    return 1;
}

int main( /*int argc, char *argv[]*/)
{

    initSerial();
    if(initDevice() == 0)
    {
        printf("Problem in Device Initializing...!!!\n");
        return 0 ;
    }
    else
    {
        if(getUserInputs()){
            printf("Got the User Inputs Successfully\n");
        }
        if(runDevice() == 0){
            printf("Device Operation Failed...\n");
        }
    }
    
    //printf("Command : %s\n", cmd.c_str());
    //serial.flushReceiver();
    /*serial.writeString("S2000\r\n");
    usleep(10000);*/
    /*serial.writeChar('S');
    usleep(1000);
    serial.writeChar('8');
    usleep(1000);
    serial.writeChar('0');
    usleep(1000);
    serial.writeChar('0');
    usleep(1000);
    //serial.writeString("\r\n\0");
    serial.writeChar('\r');
    usleep(1000);
    serial.writeChar('\n');
    usleep(1000);
    serial.flushReceiver();
    
    if(checkDone() > 0)
    {
        //printf("Step %d Success...\n", currentPosition);
        //currentPosition += step;
    }*/
    /*serial.writeChar('S');
    serial.writeChar('3');
    serial.writeChar('0');
    serial.writeChar('0');
    serial.writeString("\r\n\0");
    /*serial.writeChar('\r');
    serial.writeChar('\n');*/
    //sleep(1);
    serial.closeDevice();
    printf("END of Program\n");
    return 0 ;
}

