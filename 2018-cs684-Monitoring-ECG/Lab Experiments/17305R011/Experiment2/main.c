/*

* Author: Amit Vhatkar

* Both part included in this code 

* Description: This code demonstartes how to use Interrputs and Timers in TivaC board and using this we will check for switch pressed 
               and will also increment switch 2 value and shown in debugger

* Filename: lab2.c 

* Functions: setup(), led_pin_config(), main()  

* Global Variables: uint8_t, sw2Status, debounceDelay, currentState, lastState;

*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"

uint8_t ui8LED = 2;
uint8_t lastState = 0;
uint8_t currentState = 0;
uint8_t debounceDelay = 10;
uint8_t sw2Status = 1;
/*

* Function Name: setup()

* Input: none

* Output: none

* Description: Set crystal frequency and enable GPIO Peripherals

* Example Call: setup();

*/
void setup(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
}

/*

* Function Name: led_pin_config()

* Input: none

* Output: none

* Description: Set PORTF Pin 1, Pin 2, Pin 3 as output. On this pin Red, Blue and Green LEDs are connected.

* Example Call: led_pin_config();

*/

void pin_config(void)
{
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_INT_PIN_0|GPIO_PIN_4); // setting pfo and pf1 for input connected to switch1 and switch2
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
}

uint8_t detectKeyPress(void){

	uint8_t result = 0;

	SysCtlDelay(6700000*2);

	//still pressed
	if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_INT_PIN_0) == 0){

		if(lastState != currentState && currentState == 1){
			lastState = !currentState;
			result = 1;
		}
	}

	return result;

}

void unlock_pf0(void)
{
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
}

int main(void)
{

	setup();
	unlock_pf0();
	pin_config();

	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4|								GPIO_INT_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);  // enable pull up register

	while(1){

		//switch pressed
		if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_INT_PIN_0) == 0){

				//go to press state
				currentState = 1;

				//True press: go to release state
				if(detectKeyPress() == 1){

					//Wait Till switch is presses
					while(GPIOPinRead(GPIO_PORTF_BASE, GPIO_INT_PIN_0) == 0);

					if(ui8LED==8)
					{
						ui8LED=2;
					}
					else {
						ui8LED*=2;
					}
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8LED);
					sw2Status++;
				}
				//False press: Go to ideal state
				else{
					lastState = 0;
					currentState = 0;
				}
		}
		//switch released: go to ideal state
		if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_INT_PIN_0) == 1){
			lastState = 0;
			currentState = 0;
		}

	}
}
