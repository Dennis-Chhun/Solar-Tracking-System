/* ========================================
 *
 * Project Name		: Stepper Motor Interfacing
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
#include <project.h>

void delay_custom()
{
	unsigned char i,j,k;
	for(i=0;i<6;i++) {
		for(j=0;j<255;j++)
			for(k=0;k<255;k++);
	}
}
int main()
{
while(1){
    //Clockwise
    /***Stepper_Write(0b1010);
    CyDelay(3);
    Stepper_Write(0b0110);
    CyDelay(3);
    Stepper_Write(0b0101);
    CyDelay(3);
    Stepper_Write(0b1001);
    CyDelay(3);**/
    //CounterClockwise
    Stepper_Write(0b0001);
    CyDelay(3);
    Stepper_Write(0b0010);
    CyDelay(3);
    Stepper_Write(0b0100);
    CyDelay(3);
    Stepper_Write(0b1000);
    CyDelay(3);
}
}

/* [] END OF FILE */
