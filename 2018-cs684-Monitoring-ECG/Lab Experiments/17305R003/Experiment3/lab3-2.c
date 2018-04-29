
/*

* Author: Suraj,Amit,Akshat

* 

* Description: This lab code runs rainbow using PWM 

* Filename: lab3.c 

* Functions: setup(), led_pin_config(), main()  

* Global Variables: sw2_pressed=0,led_value=0,sw1_pressed=0,delay=1,intensity=100,sw2_p1=0,sw2_p2=0,sw2_p3=0
                    mode,led_vals[]

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
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#define PWM_FREQUENCY 55
#define DELAY_MS(x) (SysCtlDelay(6700000/500*x))

uint16_t sw2_pressed=0,led_value=0,sw1_pressed=0,delay=1,intensity=100,sw2_p1=0,sw2_p2=0,sw2_p3=0;
uint8_t mode=0;// 0-- for auto mode , 1--manual mode
uint8_t led_vals[]={2,10,8,12,4,6};

/*

* Function Name: Timer0IntHandler()

* Input: none

* Output: none

* Description: Called when timer 0 interrupt occurs

* Example Call: Timer0IntHandler();

*/

void Timer0IntHandler(void)
{
  ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);  // Clear the timer interrupt
  if(!mode)
  {
      if(!ROM_GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
      {
          //sw1_pressed=0;
          sw2_pressed+=1;

          if(sw2_pressed==2)
          {
              if(delay>1)
              {
                  delay-=1;
              }
          }
      }
      else
      {
          sw2_pressed=0;

      }

      if(!ROM_GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0))
      {
          //sw2_pressed=0;
          sw1_pressed+=1;
          if(sw1_pressed==2)
          {
              delay+=1;
          }
      }
      else
      {
          sw1_pressed=0;
      }

      if(!ROM_GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) && !ROM_GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
      {
          mode=1;
          sw2_pressed=0;sw1_pressed=0;
          DELAY_MS(100);
      }
  }
  else
  {
      if(!ROM_GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0))
      {
          sw1_pressed+=1;
          if(sw1_pressed>2)
          {
              if(intensity>12)
              {
              intensity-=10;
              }
              if(!ROM_GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
              {   sw2_p1+=1;
              if(sw2_p1>=2)
              {
                  mode=2;
                  if(!ROM_GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
                  {
                  sw2_p2+=1;
                  if(sw2_p2>=2)
                  {
                      mode=3;
                      if(!ROM_GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
                      {
                      sw2_p3+=1;
                      if(sw2_p3>2)
                      {

                         mode=3;
                      }
                      }
                  }
                  }
              }

              }
          }

      }
      else
      {
          sw1_pressed=0;
      }
      if(!ROM_GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
      {
          sw2_pressed+=1;
                    if(sw2_pressed>2)
                    {
                        intensity+=10;

                    }
      }
      else
      {
          sw2_pressed=0;
      }
  }

}
void initTimer()
{
  uint32_t ulPeriod = (SysCtlClockGet() / 10)/ 2;
  ROM_TimerLoadSet(TIMER0_BASE, TIMER_A,ulPeriod -1);
  ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);   // 32 bits Timer
  //TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0IntHandler);    // Registering  isr
  ROM_TimerEnable(TIMER0_BASE, TIMER_A);
  ROM_IntEnable(INT_TIMER0A);
  ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}

int main(void)
{
    uint16_t i=0;
    volatile uint32_t ui32Load,ulPeriod;
    volatile uint32_t ui32PWMClock;
    volatile uint8_t ui8Adjust;  //used for setting PWM pulse Width
    ui8Adjust = 83;
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ); //using ROM based APIs to set CLock System Apis
    ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_64);  // using ROM based APIs to set PWM CLock 
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);  //using ROM based APIs to set PWM1 perihpheral  enable
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);   //using ROM based APIs to set GPIO port D perihpheral  enable
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //using ROM based APIs to set GPIO port F perihpheral  enable
    ROM_GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); // using ROM based APis to set GPIO PIN 0 PWM
    ROM_GPIOPinConfigure(GPIO_PF1_M1PWM5);    // setting PortF1 pin PWM
    ROM_GPIOPinConfigure(GPIO_PF2_M1PWM6);    // setting PortF2 pin PWM
    ROM_GPIOPinConfigure(GPIO_PF3_M1PWM7);    // setting PortF3 pin PWM
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;   //using  PIN0 of PortF pin to input by locking it
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
    ROM_GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);
    ROM_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    ui32PWMClock = SysCtlClockGet() / 64;
    ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;
    ROM_IntMasterEnable();
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);  // using ROM based APIs to enable Timer 0
    ROM_TimerEnable(TIMER0_BASE, TIMER_A);  //enable TimerA using ROM based API
    initTimer();   

        PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN); // configuring 
        PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, ui32Load);
        ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 1 * ui32Load / 1000);
        ROM_PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);
        ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_2);

        // Pin G
        PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN);
        PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, ui32Load);
        ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 1 * ui32Load / 1000);
        ROM_PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
        ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_3);


        // Pin B
        ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 1 * ui32Load / 1000);
        ROM_PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);

        while(1)
        {
           //this portion of code runs rainbow
            if(!mode)
            {
            for(i=1;i<=999;i++){
                // Increase G
                ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, i * ui32Load / 1000);
                DELAY_MS(delay);
            }
            }
            if(!mode)
            {
            for(i=999;i>=1;i--){
                // Decrease R
                ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, i * ui32Load / 1000);
                DELAY_MS(delay);
            }
            }
            if(!mode)
            {
            for(i=1;i<=999;i++){
                // Increase B
                ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, i * ui32Load / 1000);
                DELAY_MS(delay);
            }
            }
            if(!mode)
            {
            for(i=999;i>=1;i--){
                // Decrease G
                ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, i * ui32Load / 1000);
                DELAY_MS(delay);
            }
            }
            if(!mode)
            {
            for(i=1;i<=999;i++){
                // Increase R
                ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, i * ui32Load / 1000);
                DELAY_MS(delay);
            }
            }
            if(!mode)
            {
            for(i=999;i>=1;i--){
                // Decrease B
                ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, i * ui32Load / 1000);
                DELAY_MS(delay);
            }
            }
            if(mode==1)
            {
                ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 1);
                ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 1);
                ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 1);
            }
            if(mode==2)
            {

                ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, intensity * ui32Load / 1000);
                ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 1);
                ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 1);
            }
            if(mode==3)
            {
                ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, intensity * ui32Load / 1000);
                                ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 1);
                                ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 1);
            }
            if(mode==4)
            {
                ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, intensity * ui32Load / 1000);
                                ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 1);
                                ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 1);
            }


    }
}

