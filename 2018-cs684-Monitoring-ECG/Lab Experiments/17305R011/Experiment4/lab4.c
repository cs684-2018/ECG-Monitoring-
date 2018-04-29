
/*

* Author: Suraj,Amit,Akshat

* 
* All parts included into single code
* Description: This lab code use ADC and UART to monitor joystic movement in screen 

* Filename: lab3.c 

* Functions: setup(), led_pin_config(), main()  

* Global Variables: none

*/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"

/*

* Function Name: setup()

* Input: none

* Output: none

* Description: Set crystal frequency and enable GPIO Peripherals,ADC0 and UART0

* Example Call: setup();

*/

void setup(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

}
/*

* Function Name: enable_ADC()

* Input: none

* Output: none

* Description: enable ADC for PIN0 and PIN1  

* Example Call: enable_ADC();

*/
void enable_ADC(void)
{
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0);
    GPIOPinTypeADC(GPIO_PORTD_BASE,GPIO_PIN_1);
}
/*

* Function Name: buzzerOff()

* Input: none

* Output: none

* Description: Setting buzzer off for GameConsole 

* Example Call: buzzerOff();

*/
void buzzerOff(void)
{
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4);
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 16);
}
/*

* Function Name: setUART()

* Input: none

* Output: none

* Description: Setting UART 0 with baudrate 115200 with one stop bit and no parity bit

* Example Call: setUART();

*/
void setUART(void)
{
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(),115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}
/*

* Function Name: uArtpinConfigure()

* Input: none

* Output: none

* Description: Setting UART for PA0 and PA1 as reciever and transmitter

* Example Call: uArtpinConfigure();

*/
void uArtpinConfigure(void)
{
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
}
/*

* Function Name: main()

* Input: none

* Output: none

* Description: Setting UART for PA0 and PA1 as reciever and transmitter

* Example Call: main();

*/
int main(void)
{
    uint32_t ui32ADC0Value[8];
    volatile uint32_t vert;  //vertical value
    volatile uint32_t horz;  //horizontal value
    setup();  //setup to set peripherals 
    enable_ADC();  // enable ADC
    buzzerOff();   // buzzerOff 
    uArtpinConfigure();  // setting UART pin values
    setUART();  // initialize UART 
    ADCSequenceDisable(ADC0_BASE, 0);
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH7| ADC_CTL_IE);
    ADCSequenceStepConfigure(ADC0_BASE,0, 1, ADC_CTL_CH6 | ADC_CTL_IE |ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 0);
    while(1)
    {
        // this piece of code read ADC value from joystic and sending it over UART to pc
        ADCIntClear(ADC0_BASE, 0);
        ADCProcessorTrigger(ADC0_BASE, 0);
        while(!ADCIntStatus(ADC0_BASE, 0, false))
        {
        }
        ADCSequenceDataGet(ADC0_BASE, 0, ui32ADC0Value);  //getting ADC value from joystic
        if((ui32ADC0Value[0]>600 && ui32ADC0Value[0]<3000) && (ui32ADC0Value[1]<3000 && ui32ADC0Value[1]>600))
        {
            UARTCharPut(UART0_BASE, 'A');  //sending A when joystic in stable position
        }
        else if(ui32ADC0Value[0]<30 && (ui32ADC0Value[1]<3000 && ui32ADC0Value[1]>600 ))
        {
               UARTCharPut(UART0_BASE, 'B');  //sending B when joystic in down position
        }
        else if(ui32ADC0Value[1]<30 && (ui32ADC0Value[0]<3000 && ui32ADC0Value[0]>600 ))
        {
            UARTCharPut(UART0_BASE, 'C');  //sending C when joystic in right position
        }
        else if(ui32ADC0Value[0]>3000 && (ui32ADC0Value[1]<3000 && ui32ADC0Value[1]>600 ))
        {
            UARTCharPut(UART0_BASE, 'D'); //sending D when joystic in up position

        }else if(ui32ADC0Value[1]>3000 && (ui32ADC0Value[0]<3000 && ui32ADC0Value[0]>600 ))
        {
            UARTCharPut(UART0_BASE, 'E'); //sending E when joystic in left position
        }
    }

}
