/*

* Author: Suraj,Amit,Akshat

* Both part included in this code 

* Description: This code demonstartes how to use Interrputs and Timers in TivaC board and using this we will check for switch pressed 
               and will also increment switch 2 value and shown in debugger

* Filename: lab2.c 

* Functions: setup(), led_pin_config(), main()  

* Global Variables: uint8_t sw2_pressed,led_value,sw1_pressed,sw1_no;

*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"

uint8_t sw2_pressed=0,led_value=1,sw1_pressed=0,sw1_no=0;

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
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);
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

* Function Name: switch_1_ip()

* Input: none

* Output: none

* Description: Setting PORTF pin 0 as input pin 

* Example Call: switch_1_ip();

*/
void switch_1_ip(void)
{
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)=GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR)|=0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0;
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE ,GPIO_PIN_0);
}
/*

* Function Name: enable_timer_interrupt()

* Input: none

* Output: none

* Description: Enable TIMERA interrupt 

* Example Call: enable_timer_interrupt();

*/
void enable_timer_interrupt()
{
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntMasterEnable();
}

/*

* Function Name: enable_timer()

* Input: none

* Output: none

* Description: Enable TIMERA at 1/10th of System Clock to set Interrput 

* Example Call: enable_timer();

*/

void enable_timer()
{
    uint32_t ui32Period = (SysCtlClockGet() / 10) / 2;
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);
    TimerEnable(TIMER0_BASE, TIMER_A);
}
/*

* Function Name: Timer0IntHandler()

* Input: none

* Output: none

* Description: Called when timer 0 interrupt occurs

* Example Call: implicitlly called by system during interrupt of timers

*/
void Timer0IntHandler(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);  // clear interrupt at timer A
    if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)) // Read Pin 4 of PortF to check if switch oressed or not
    {
        //sw1_pressed=0;
      sw2_pressed+=1;   

      if(sw2_pressed==2)
      {
          if(led_value==8)
          {
              led_value=2;  // As the input recieved changing LED va
          }
          else
          {
              led_value*=2;  // As the input recieved changing LED value
          }
          GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, led_value); // writing LED value to PORTF 
      }
    }
    else
    {
        sw2_pressed=0;

    }

    if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0))
    {
       //sw2_pressed=0;
       sw1_pressed+=1;
       if(sw1_pressed==2)
             {
               sw1_no+=1;  //incrementing switch 2 value and showed in debugger
             }
    }
    else
    {
        sw1_pressed=0;
    }

}
int main(void)
{

    setup(); // calling setup() function to setup crystal frequency
    led_pin_config(); // calling led_pin_config() to config PORTF pins for LED functionality
    switch_1_ip(); // calling switch_1_ip() to set PIN0 of PORTF as Input
    //printf(SysCtlClockGet());
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE ,GPIO_PIN_4|GPIO_INT_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_4|GPIO_PIN_0,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU); // Padding Pins of PORTF
    enable_timer_interrupt(); // enable timer interrupt by calling enable_timer_interrupt()
    enable_timer();  // enable timer by calling enable_timer()

    //GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 2);
    while(1)
    {
        //waiting here so that it does not end
    }
}
