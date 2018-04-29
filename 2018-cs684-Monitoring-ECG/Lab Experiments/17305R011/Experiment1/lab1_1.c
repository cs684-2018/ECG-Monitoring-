/*

* Author: Suraj,Amit,Akshat

* 

* Description: This code will turn RED LED on when switch pressed for the first time,Green On second and BLUE on third time  

* Filename: lab-1.c 

* Functions: setup(), led_pin_config(), main()  

* Global Variables: none

*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

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

void led_pin_config(void)
{
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
}
/*

* Function Name: main()

* Input: none

* Output: none

* Description: This code will turn RED LED on when switch pressed for the first time,Green On second and BLUE on third time. 
*/

int main(void)
{
    uint8_t led_val=2;  // local variable to decide which LED should be ON 2 for Red,4 for Green,8 for BLUE
    setup();  // calling setup() function to setup crystal frequency
    led_pin_config();  // calling led_pin_config() to config PORTF pins for LED functionality
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE ,GPIO_PIN_4);  // Setting PIN4 of PORTF for Input
    //GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU); // Padding Pins of PORTF
    //GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0);
    while(1)
    {
        // Turn on the LED
        while(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)!=0); // CHecking for input
        if(led_val==8)
        {
            led_val=2; // As the input recieved changing LED value
        }
        else
        {
            led_val*=2;  // As the input recieved changing LED value
        }
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, led_val);  // writing LED value into PORTF to glow colour LED
        while(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)==0); // waiting to release switch
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0); // switching off LED as the switch turned off
    }
}
