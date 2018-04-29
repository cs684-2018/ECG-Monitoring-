
/*

* Author: Suraj,Amit,Akshat

* 

* Description: This lab code runs servo motor using PWM inTIVAC

* Filename: lab3.c 

* Functions: setup(), led_pin_config(), main()  

* Global Variables: none

*/

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"
#define PWM_FREQUENCY 55

/*

* Function Name: main()

* Input: none

* Output: none

* Description: This lab code runs servo motor using PWM inTIVAC. 
*/
int main(void)
{
volatile uint32_t ui32Load;  
volatile uint32_t ui32PWMClock;
volatile uint8_t ui8Adjust; //used for setting PWM pulse Width
ui8Adjust = 83;
ROM_SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);  //using ROM based APIs to set CLock System Apis
ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_64);  // using ROM based APIs to set PWM CLock 
ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);  //using ROM based APIs to set PWM1 perihpheral  enable
ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);   //using ROM based APIs to set GPIO port D perihpheral  enable
ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //using ROM based APIs to set GPIO port F perihpheral  enable
ROM_GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0); // using ROM based APis to set GPIO PIN 0 PWM
ROM_GPIOPinConfigure(GPIO_PD0_M1PWM0);
HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;   //using  PIN0 of PortF pin to input by locking it
HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
ROM_GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);  // using ROM based APis to set PORTF 4 pin as Output
ROM_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
ui32PWMClock = SysCtlClockGet() / 64;  // setting PWM clock to 1/64th of SysCtlClockGet()
ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;
PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN); //configuring PWM 
PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, ui32Load);
ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 1000); //ROM based APi to set PWM lpulse width
ROM_PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);	
ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_0);
while(1)
{
	// this portion will move the accutator on basis of switch pressed
if(ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0x00) //checking PIN4 if pressed
{
ui8Adjust--;  //moving actuator
if (ui8Adjust < 74) 
{
ui8Adjust = 74;
}
ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 1000);  
}
if(ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0x00) // checking PIN0 if pressed 
{
ui8Adjust++;  //moving actuator
if (ui8Adjust > 90)
{
ui8Adjust = 90;
}
ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 1000);
}
ROM_SysCtlDelay(100000);
}
}
