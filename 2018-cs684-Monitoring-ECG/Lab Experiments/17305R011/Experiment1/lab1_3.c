/*

* Author: suraj,amit,akshat

* FileName:-- lab1_3.c
* Functions: setup(), pin_config(), main()
*  Global Variables: delay
* Description: This lab contains code to delay LED lightning with switch pressed

*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_ints.h"
#include <time.h>

int64_t delay;
/*

* Function Name: setup()

* Input: none

* Output: none

* Description: Set crystal frequency,enable GPIO Peripherals and unlock Port F pin 0 (PF0)

* Example Call: setup();

*/
void setup(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= 0x01;
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_0);
}

/*

* Function Name: pin_config()

* Input: none

* Output: none

* Description: Set Port F Pin 1, Pin 2, Pin 3 as output. On this pin Red, Blue and Green LEDs are connected.
			   Set Port F Pin 0 and 4 as input, enable pull up on both these pins.

* Example Call: pin_config();

*/

void pin_config(void)
{
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
}


int main(void)
{
	uint8_t ui8LED = 2;
	int32_t sw2monitor,sw1monitor;


    setup();
    pin_config();
    delay = 6700000;  //delay of 0.5 ms
    while(1)
    {
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8LED);
		SysCtlDelay(delay);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
		SysCtlDelay(delay);

    	sw1monitor = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
    	sw2monitor = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);
    	if( (sw2monitor & GPIO_PIN_0)==0)  //sw2monitor to control delay
    	{
    		if(ui8LED == 2) ui8LED = 8;
    		else if(ui8LED == 8) ui8LED = 4;
    		else if(ui8LED == 4) ui8LED = 2;
    	}
    	if((sw1monitor & GPIO_PIN_4)==0) delay = delay*2;
    }
}
