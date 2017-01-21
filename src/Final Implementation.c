/*******************************************************************************
* Project Name		: Final Implementation
* File Name			: main.c
* Version 			: **
* Device Used		: CY8C5888LTI-LP097
* Software Used		: PSoC Creator 3.1 SP2
* Compiler    		: ARM GCC 4.8.4, ARM RVDS Generic, ARM MDK Generic
* Related Hardware	: CY8CKIT059 PSoC 5 LP Prototyping Kit 
* Owner				: Dennis Chhun
*
* Description:
*  This example project demonstrates how the comparator's input can be multiplexed 
*  using the Amux component.
*
*******************************************************************************/
#include <project.h>
#include "stdio.h"
#define TRANSMIT_BUFFER_SIZE  16
uint8 interrupt_flag;
uint32 Output,in1,in2,in3,in4;
uint8 threshold=200;
// Stepper Motor timing for track movement
uint32 down_high=460;
uint32 down_level=145;
uint32 high_down=540;
uint32 high_level=380;
uint32 level_high=380;
uint32 level_down=145;
uint8 preState=0;


// Stepper Motor Sequence with up and down for both motors
void down1(uint32 temp){
    uint32 count=0;
    while(count<temp){
    Stepper1_Write(0b0001);
    CyDelay(3);
    Stepper1_Write(0b0010);
    CyDelay(3);
    Stepper1_Write(0b0100);
    CyDelay(3);
    Stepper1_Write(0b1000);
    CyDelay(3);
    count++;
    }
}

void up1(uint32 temp){
    uint32 count=0;
    while(count<temp){
    Stepper1_Write(0b1000);
    CyDelay(3);
    Stepper1_Write(0b0100);
    CyDelay(3);
    Stepper1_Write(0b0010);
    CyDelay(3);
    Stepper1_Write(0b0001);
    CyDelay(3);
    count++;
    }
}

void up2(uint32 temp){
    uint32 count=0;
    while(count<temp){
    Stepper2_Write(0b0001);
    CyDelay(3);
    Stepper2_Write(0b0010);
    CyDelay(3);
    Stepper2_Write(0b0100);
    CyDelay(3);
    Stepper2_Write(0b1000);
    CyDelay(3);
    count++;
    }
}

void down2(uint32 temp){
    uint32 count=0;
    while(count<temp){
    Stepper2_Write(0b1000);
    CyDelay(3);
    Stepper2_Write(0b0100);
    CyDelay(3);
    Stepper2_Write(0b0010);
    CyDelay(3);
    Stepper2_Write(0b0001);
    CyDelay(3);
    count++;
    }
}

// Interrupt Service Routine: when vakue high, it will output the voltage
CY_ISR(isr_1)
{
    interrupt_flag = 1;
    Output = ADC_SAR_1_CountsTo_mVolts(ADC_SAR_1_GetResult16());
}

// Activate ADC
// in1,in2,in3,in4 = sensor values
void read(uint8 temp){
    ADC_SAR_1_StartConvert();
    while(interrupt_flag==0){
        //UART_1_PutString("reading\n");
    }
    if(temp==0)
        in1=Output;
    else if(temp==1)
        in2=Output;
    else if(temp==2)
        in3=Output;
    else
        in4=Output;
    ADC_SAR_1_StopConvert();
}

int main()
{
    uint8 Ch;
    uint8 flag,count;
    uint8 channel,state=0;
    UART_1_Start();
    ADC_SAR_1_Start();
    ADC_SAR_1_IRQ_Enable();
    AMux_Start();
    isr_1_StartEx(isr_1);
    CyGlobalIntEnable;
    //UART_1_PutString("COM Port Open");
    char TransmitBuffer[TRANSMIT_BUFFER_SIZE];
    
    //Starting at a level position
    up1(down_level);
    up2(down_level);

    for(;;){
        //Tera Term: Pressing "R" or "r" to start running our project, and "S" or "s" to stop running 
        Ch = UART_1_GetChar();
    
        switch(Ch){
                case 0:
                /* No new data was recieved */
                break;
            case 'R':
            case 'r':
                flag=1;
                break;
            case 'S':
            case 's':
                flag=0;
                break;
        }
        //Multiplexer:
    //LED turns solid when reading a value on the Photoresistors
    if(flag==1){
    LED_Write(1);
        AMux_Select(0);
        CyDelay(100);
        read(0);
        interrupt_flag=0;
        CyDelay(100);
        read(0);
        interrupt_flag=0;
        AMux_Select(1);
        CyDelay(100);
        read(1);
        interrupt_flag=0;
        CyDelay(100);
        read(1);
        interrupt_flag=0;
        AMux_Select(2);
        CyDelay(100);
        read(2);
        interrupt_flag=0;
        CyDelay(100);
        read(2);
        interrupt_flag=0;
        AMux_Select(3);
        CyDelay(100);
        read(3);
        interrupt_flag=0;
        CyDelay(100);
        read(3);
        interrupt_flag=0;
    LED_Write(0);
    
    //Displays Values for all 4 Photoresistors at the same time on Tera Term
    //threshold=200;
    sprintf(TransmitBuffer, "Sample 1: %lu mV\r\n", in1);
    UART_1_PutString(TransmitBuffer);
    sprintf(TransmitBuffer, "Sample 2: %lu mV\r\n", in2);
    UART_1_PutString(TransmitBuffer);
    sprintf(TransmitBuffer, "Sample 3: %lu mV\r\n", in3);
    UART_1_PutString(TransmitBuffer);
    sprintf(TransmitBuffer, "Sample 4: %lu mV\r\n", in4);
    UART_1_PutString(TransmitBuffer);
  
    
    // Setting our states, compares reading values and sets the states
    if(in1<threshold && in2>threshold && in3>threshold && in4>threshold)
        state=1;
    else if(in1<threshold && in2<threshold && in3>threshold && in4>threshold)
        state=2;
    else if(in2<threshold && in1>threshold && in3>threshold && in4>threshold)
        state=3;
    else if(in2<threshold && in3<threshold && in1>threshold && in4>threshold)
        state=4;
    else if(in3<threshold && in1>threshold && in2>threshold && in4>threshold)
        state=5;
    else if(in3<threshold && in4<threshold && in1>threshold && in2>threshold)
        state=6;
    else if(in4<threshold && in1>threshold && in2>threshold && in3>threshold)
        state=7;
    else if(in4<threshold && in1<threshold && in2>threshold && in3>threshold)
        state=8;
    else
        state=0;
    
    //Displays PreState and NextState
    sprintf(TransmitBuffer, "preState: %d \r\n", preState);
    UART_1_PutString(TransmitBuffer);
    sprintf(TransmitBuffer, "State: %d \r\n", state);
    UART_1_PutString(TransmitBuffer);
   
    // If-else statements: 9 states with 9 different possibilities, 81 scenearios total 
    if(state==0){
        if(preState==0){
            
           //If prestate and current state same, LED keeps blinking (3 times)
            for(count=0;count<3;count++){
                LED_Write(1);
                CyDelay(300);
                LED_Write(0);
                CyDelay(300);
            }
        }
        else if(preState==1){
            down1(high_level);
        }
        else if(preState==2){
            down1(high_level);
            up2(down_level);
        }
        else if(preState==3){
            up2(down_level);
        }
        else if(preState==4){
            up1(down_level);
            up2(down_level);
        }
        else if(preState==5){
            up1(down_level);
        }
        else if(preState==6){
            up1(down_level);
            down2(high_level);
        }
        else if(preState==7){
            down2(high_level);
        }
        else{
            down1(high_level);
            down2(high_level);
        }
    }
    else if(state==1){
        if(preState==0){
            up1(level_high);
        }
        else if(preState==1){
            for(count=0;count<3;count++){
                LED_Write(1);
                CyDelay(300);
                LED_Write(0);
                CyDelay(300);
            }
        }
        else if(preState==2){
            up2(down_level);
        }
        else if(preState==3){
            up1(level_high);
            up2(down_level);
        }
        else if(preState==4){
            down2(high_level);
        }
        else if(preState==5){
            up1(down_high);
        }
        else if(preState==6){
            up1(down_high);
            down2(high_level);
        }
        else if(preState==7){
            up1(level_high);
            up2(high_level);
        }
        else{
            down2(high_level);
        }
    }
    else if(state==2){
        if(preState==0){
            up1(level_high);
            down2(level_down);
        }
        else if(preState==2){
            for(count=0;count<3;count++){
                LED_Write(1);
                CyDelay(300);
                LED_Write(0);
                CyDelay(300);
            }
        }
        else if(preState==3){
            up1(level_high);
        }
        else if(preState==4){
            up1(down_high);
        }
        else if(preState==5){
            up1(down_high);
            down2(level_down);
        }
        else if(preState==6){
            up1(down_high);
            down2(high_down);
        }
        else if(preState==7){
            up1(level_high);
            down2(high_down);
        }
        else{
            down2(high_down);
        }
    }
    else if(state==3){
        if(preState==0){
            down2(level_down);
        }
        else if(preState==1){
            down1(high_level);
            down2(level_down);
        }
        else if(preState==2){
            down1(high_level);
        }
        else if(preState==3){
            for(count=0;count<3;count++){
                LED_Write(1);
                CyDelay(300);
                LED_Write(0);
                CyDelay(300);
            }
        }
        else if(preState==4){
            up1(down_level);
        }
        else if(preState==5){
            up1(down_level);
            down2(level_down);
        }
        else if(preState==6){
            up1(down_level);
            down2(high_down);
        }
        else if(preState==7){
            down2(high_down);
        }
        else{
            down1(high_level);
            down2(high_down);
        }
    }
    else if(state==4){
        if(preState==0){
            down1(level_down);
            down2(level_down);
        }
        else if(preState==1){
            down1(high_down);
            down2(level_down);
        }
        else if(preState==2){
            down1(high_down);
        }
        else if(preState==3){
            down1(level_down);
        }
        else if(preState==4){
            for(count=0;count<3;count++){
                LED_Write(1);
                CyDelay(300);
                LED_Write(0);
                CyDelay(300);
            }
        }
        else if(preState==5){
            down2(level_down);
        }
        else if(preState==6){
            down2(high_down);
        }
        else if(preState==7){
            down1(level_down);
            down2(high_down);
        }
        else{
            down1(high_down);
            down2(high_down);
        }
    }
    else if(state==5){
        if(preState==0){
            down1(level_down);
        }
        else if(preState==1){
            down1(high_down);
        }
        else if(preState==2){
            down1(high_down);
            up2(down_level);
        }
        else if(preState==3){
            down1(level_down);
            up2(down_level);
        }
        else if(preState==4){
            up2(down_level);
        }
        else if(preState==5){
            for(count=0;count<3;count++){
                LED_Write(1);
                CyDelay(300);
                LED_Write(0);
                CyDelay(300);
            }
        }
        else if(preState==6){
            down2(high_level);
        }
        else if(preState==7){
            down1(level_down);
            down2(high_level);
        }
        else{
            down1(high_down);
            down2(high_level);
        }
    }
    else if(state==6){
        if(preState==0){
            down1(level_down);
            up2(level_high);
        }
        else if(preState==1){
            down1(high_down);
            up2(level_high);
        }
        else if(preState==2){
            down1(high_down);
            up2(down_high);
        }
        else if(preState==3){
            down1(level_down);
            up2(down_high);
        }
        else if(preState==4){
            up2(down_high);
        }
        else if(preState==5){
            up2(level_high);
        }
        else if(preState==6){
            for(count=0;count<3;count++){
                LED_Write(1);
                CyDelay(300);
                LED_Write(0);
                CyDelay(300);
            }
        }
        else if(preState==7){
            down1(level_down);
        }
        else{
            down1(high_down);
        }
    }
    else if(state==7){
        if(preState==0){
            up2(level_high);
        }
        else if(preState==1){
            down1(high_level);
            up2(level_high);
        }
        else if(preState==2){
            down1(high_level);
            up2(down_high);
        }
        else if(preState==3){
            up2(down_high);
        }
        else if(preState==4){
            up1(down_level);
            up2(down_high);
        }
        else if(preState==5){
            up1(down_level);
            up2(level_high);
        }
        else if(preState==6){
            up1(down_level);
        }
        else if(preState==7){
            for(count=0;count<3;count++){
                LED_Write(1);
                CyDelay(300);
                LED_Write(0);
                CyDelay(300);
            }
        }
        else{
            down1(high_level);
        }
    }
    else{
        if(preState==0){
            up1(level_high);
            up2(level_high);
        }
        else if(preState==1){
            up2(level_high);
        }
        else if(preState==2){
            up2(down_high);
        }
        else if(preState==3){
            up1(level_high);
            up2(down_high);
        }
        else if(preState==4){
            up1(down_high);
            up2(down_high);
        }
        else if(preState==5){
            up1(down_high);
            up2(level_high);
        }
        else if(preState==6){
            up1(down_high);
        }
        else if(preState==7){
            up1(level_high);
        }
        else{
            for(count=0;count<3;count++){
                LED_Write(1);
                CyDelay(300);
                LED_Write(0);
                CyDelay(300);
            }
        }
    }
    preState=state;
    flag=1;
    }
    else{
        //Do  nothing
    }
    }
}
    


/* [] END OF FILE */
