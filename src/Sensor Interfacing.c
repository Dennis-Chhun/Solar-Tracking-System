/* ========================================
 * Project Name		: Sensor Interfacing
 * File Name	    : main.c
 * Version 			: **
 * Device Used		: CY8C5888LTI-LP097
 * Software Used    : PSoC Creator 3.1 SP2
 * Compiler    		: ARM GCC 4.8.4, ARM RVDS Generic, ARM MDK Generic
 * Related Hardware	: CY8CKIT059 PSoC 5 LP Prototyping Kit 
 * Owner			: Dennis Chhun
 *
 * Summary:
 *  main() performs following functions:
 *  1: Starts the ADC and UART components.
 *  2: Checks for ADC end of conversion.  Stores latest result in output
 *     if conversion complete.
 *  3: Checks for UART input.
 *     On 'C' or 'c' received: transmits the last sample via the UART.
 *     On 'S' or 's' received: continuously transmits samples as they are completed.
 *     On 'X' or 'x' received: stops continuously transmitting samples.
 *     On 'E' or 'e' received: transmits a dummy byte of data.
 *
 * ========================================
*/

#include <device.h>
#include "stdio.h"

/* Project Defines */
#define FALSE  0
#define TRUE   1
#define TRANSMIT_BUFFER_SIZE  16

int main()
{
    /* Variable to store ADC result */
    uint32 Output;
    /* Variable to store UART received character */
    uint8 Ch;
    /* Variable used to send emulated data */
    uint8 EmulatedData;
    /* Flags used to store transmit data commands */
    uint8 ContinuouslySendData;
    uint8 SendSingleByte;
    uint8 SendEmulatedData;
    /* Transmit Buffer */
    char TransmitBuffer[TRANSMIT_BUFFER_SIZE];
    
    /* Start the components */
    UART_1_Start();
    ADC_SAR_1_Start();
    ADC_SAR_2_Start();
    
    /* Initialize Variables */
    ContinuouslySendData = FALSE;
    SendSingleByte = FALSE;
    SendEmulatedData = FALSE;
    EmulatedData = 0;
    
    /* Start the ADC conversion */
    ADC_SAR_1_StartConvert();
    ADC_SAR_2_StartConvert();
    
    /* Send message to verify COM port is connected properly */
    UART_1_PutString("COM Port Open");
    
    for(;;)
    {        
        /* Non-blocking call to get the latest data recieved  */
        Ch = UART_1_GetChar();
        
        /* Set flags based on UART command */
        switch(Ch)
        {
            case 0:
                /* No new data was recieved */
                break;
            case 'C':
            case 'c':
                SendSingleByte = TRUE;
                break;
            case 'S':
            case 's':
                ContinuouslySendData = TRUE;
                break;
            case 'X':
            case 'x':
                ContinuouslySendData = FALSE;
                break;
            case 'E':
            case 'e':
                SendEmulatedData = TRUE;
                break;
            default:
                /* Place error handling code here */
                break;    
        }
        
        /* Check to see if an ADC conversion has completed */
        if(ADC_SAR_1_IsEndConversion(ADC_SAR_1_RETURN_STATUS))
        {
            /* Use the GetResult16 API to get an 8 bit unsigned result in
             * single ended mode.  The API CountsTo_mVolts is then used
             * to convert the ADC counts into mV before transmitting via
             * the UART.  See the datasheet API description for more
             * details */
            Output = ADC_SAR_1_CountsTo_mVolts(ADC_SAR_1_GetResult16());
            
            /* Send data based on last UART command */
            if(SendSingleByte || ContinuouslySendData)
            {
                /* Format ADC result for transmition */
                sprintf(TransmitBuffer, "Sample1: %lu mV\r\n", Output);
                /* Send out the data */
                UART_1_PutString(TransmitBuffer);
                /* Reset the send once flag */
                SendSingleByte = FALSE;
            }
            else if(SendEmulatedData)
            {
                /* Format ADC result for transmition */
                sprintf(TransmitBuffer, "Emulated Data: %x \r\n", EmulatedData);
                /* Send out the data */
                UART_1_PutString(TransmitBuffer);
                EmulatedData++;
                /* Reset the send once flag */
                SendEmulatedData = FALSE;   
            }
        }
        if(ADC_SAR_2_IsEndConversion(ADC_SAR_2_RETURN_STATUS))
        {
            /* Use the GetResult16 API to get an 8 bit unsigned result in
             * single ended mode.  The API CountsTo_mVolts is then used
             * to convert the ADC counts into mV before transmitting via
             * the UART.  See the datasheet API description for more
             * details */
            Output = ADC_SAR_2_CountsTo_mVolts(ADC_SAR_2_GetResult16());
            
            /* Send data based on last UART command */
            if(SendSingleByte || ContinuouslySendData)
            {
                /* Format ADC result for transmition */
                sprintf(TransmitBuffer, "Sample2: %lu mV\r\n", Output);
                /* Send out the data */
                UART_1_PutString(TransmitBuffer);
                /* Reset the send once flag */
                SendSingleByte = FALSE;
            }
            else if(SendEmulatedData)
            {
                /* Format ADC result for transmition */
                sprintf(TransmitBuffer, "Emulated Data: %x \r\n", EmulatedData);
                /* Send out the data */
                UART_1_PutString(TransmitBuffer);
                EmulatedData++;
                /* Reset the send once flag */
                SendEmulatedData = FALSE;   
            }
        }
    }
}


/* [] END OF FILE */
