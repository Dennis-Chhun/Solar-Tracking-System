/* ========================================
 *
 * Project Name		: Stepper Motor Timing
 * File Name	    : main.c
 * Version 			: **
 * Device Used		: CY8C5888LTI-LP097
 * Software Used    : PSoC Creator 3.1 SP2
 * Compiler    		: ARM GCC 4.8.4, ARM RVDS Generic, ARM MDK Generic
 * Related Hardware	: CY8CKIT059 PSoC 5 LP Prototyping Kit 
 * Owner			: Dennis Chhun
 *
 * ========================================
*/

#include <device.h>
#include "stdio.h"


void down_high2(){
    uint32 count=0;
    while(count<440){
    Stepper_Write(0b1000);
    CyDelay(3);
    Stepper_Write(0b0100);
    CyDelay(3);
    Stepper_Write(0b0010);
    CyDelay(3);
    Stepper_Write(0b0001);
    CyDelay(3);
    count++;
    }
}

void down_level2(){
    uint32 count=0;
    while(count<145){
    Stepper_Write(0b1000);
    CyDelay(3);
    Stepper_Write(0b0100);
    CyDelay(3);
    Stepper_Write(0b0010);
    CyDelay(3);
    Stepper_Write(0b0001);
    CyDelay(3);
    count++;
    }
}

void high_down2(){
    uint32 count=0;
    while(count<440){
    Stepper_Write(0b0001);
    CyDelay(3);
    Stepper_Write(0b0010);
    CyDelay(3);
    Stepper_Write(0b0100);
    CyDelay(3);
    Stepper_Write(0b1000);
    CyDelay(3);
    count++;
    }
}

void high_level2(){
    uint32 count=0;
    while(count<380){
    Stepper_Write(0b0001);
    CyDelay(3);
    Stepper_Write(0b0010);
    CyDelay(3);
    Stepper_Write(0b0100);
    CyDelay(3);
    Stepper_Write(0b1000);
    CyDelay(3);
    count++;
    }
}

void level_high2(){
    uint32 count=0;
    while(count<380){
    Stepper_Write(0b1000);
    CyDelay(3);
    Stepper_Write(0b0100);
    CyDelay(3);
    Stepper_Write(0b0010);
    CyDelay(3);
    Stepper_Write(0b0001);
    CyDelay(3);
    count++;
    }
}

void level_down2(){
    uint32 count=0;
    while(count<145){
    Stepper_Write(0b0001);
    CyDelay(3);
    Stepper_Write(0b0010);
    CyDelay(3);
    Stepper_Write(0b0100);
    CyDelay(3);
    Stepper_Write(0b1000);
    CyDelay(3);
    count++;
    }
}


int main()
{
    /* Variable to store ADC result */
    uint32 Output;
    /* Variable to store UART received character */
    uint8 Ch;

    /* Start the components */
    UART_1_Start();

    /* Send message to verify COM port is connected properly */
    UART_1_PutString("COM Port Open");
    
    for(;;)
    {        
        /* Non-blocking call to get the latest data recieved  */
        Ch = UART_1_GetChar();
        if(Ch=='f'){
            down_high2();
        }
        if(Ch=='b'){
            high_down2();
        }
        if(Ch=='h'){
            high_level2();
        }
        if(Ch=='l'){
            down_level2();
        }
        if(Ch=='m'){
            level_high2();
        }
        if(Ch=='n'){
            level_down2();
        }
    }
}


/* [] END OF FILE */
