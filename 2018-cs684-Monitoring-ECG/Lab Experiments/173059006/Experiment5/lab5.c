/*

* Author: Suraj,Amit,Akshat

* All parts included in this single file

* Description: This lab set LCD and draw MICKey,control square position  using joystic and controlling speed of panther on LCD using joystic 

* Filename: lab3.c 

* Functions: setup(), main() ,drawSquare(),drawcSquare(),drawPanther(),displayPantherAnimation(),configureADC(),enableADC(),glcd_cmd(),glcd_init(),glcd_data()
			glcd_setPage(),glcd_setColumn(),glcd_cleardisplay(),setOutput(),setInput()
* Global Variables: delay

*/

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "mickey.h"

#include "Panther/one.h"
#include "Panther/two.h"
#include "Panther/three.h"
#include "Panther/four.h"
#include "Panther/five.h"
#include "Panther/six.h"
#include "Panther/seven.h"
#include "Panther/eight.h"




 uint16_t delay=1;
/*

* Function Name: setup()

* Input: none

* Output: none

* Description: Set crystal frequency and enable GPIO Peripherals,ADC0 and UART0

* Example Call: setup();

*/

void setup(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

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

* Function Name: glcd_cmd()

* Input: command

* Output: none

* Description: setting GLCD cmnd function to send commands to glcd

* Example Call: glcd_cmd(cmd);

*/
void glcd_cmd(unsigned char cmd)
{
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,0x00);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,0x00);

	/* RS = 0 */
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6,0x00);

	/* Put command on data lines */
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,cmd);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,cmd);

	/* Generate a high to low pulse on enable */
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0x01);
	SysCtlDelay(1340);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0x00);

}

/* void glcd_data(data)
 * This function sends data to the GLCD.
 * Value of RS is 1
 * Data is written on data lines.
 * Enable is made 1 for a short duration.
* Input: data

* Output: none
  Description: setting GLCD data function to send data to glcd

* Example Call: glcd_data(data);
 */
void glcd_data(unsigned char data)
{
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,0x00);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,0x00);

	/* RS = 1 */
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6,0x40);

	/* Put data on data lines */
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,data);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,data);

	/* Generate a high to low pulse on enable */
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0x01);
	SysCtlDelay(1340);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0,0x00);
}



/* void glcd_init()
 * This function initializes the GLCD.
 * Always call this function at the beginning of main program after configuring the port pins.

 * Input: none

* Output: none
  Description: initializing GLCD

* Example Call: glcd_cmd(cmd);
 */
void glcd_init()
{
	SysCtlDelay(6700000/50);                            // creates ~10ms delay - TivaWare fxn
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0x00);    //cbi(GPORTC_2,GLCD_RST);
	SysCtlDelay(6700000/50);
	/* Set RST */
	GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_5,0x20);

	/* Set CS1 (CS1=1 and CS2=0) The right side is selected(column>64) */
	GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x00);

	/* Select the start line */
	glcd_cmd(0xC0);
	//SysCtlDelay(6700);

	/* Send the page */
	glcd_cmd(0xB8);
	//  SysCtlDelay(6700);

	/*Send the column */
	glcd_cmd(0x40);
	//SysCtlDelay(6700);

	/* Send glcd on command */
	glcd_cmd(0x3F);


	/* Initialize the right side of GLCD */
	/* Set CS2 (CS2=1 and CS1=0) The right side is selected(column>64) */
	GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x08);

	/* Select the start line */
	glcd_cmd(0xC0);
	SysCtlDelay(6700);

	/* Send the page */
	glcd_cmd(0xB8);
	//  SysCtlDelay(6700);

	/*Send the column */
	glcd_cmd(0x40);
	//  SysCtlDelay(6700);

	/* Send glcd on command */
	glcd_cmd(0x3F);
	//  SysCtlDelay(6700);
}




/* void glcd_setpage(page)
 * This function selects page number on GLCD.
 * Depending on the value of column number CS1 or CS2 is made high.
 * Input: page
 Description: setting page for lcd

* Example Call: glcd_setpage(cmd);
* Output: none
 */
void glcd_setpage(unsigned char page)
{
	/* Set CS1 (CS1=1 and CS2=0) The right side is selected(column>64) */
	GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x00);
	glcd_cmd(0xB8 | page);
	//SysCtlDelay(100);

	/* Set CS2 (CS2=1 and CS1=0) The right side is selected(column>64) */
	GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x08);
	glcd_cmd(0xB8 | page);
	//SysCtlDelay(100);
}



/* void glcd_setcolumn(column)
 * This function selects column number on GLCD.
 * Depending on the value of column number CS1 or CS2 is made high.

 Input: column
* Output: none
 Description: setting page for lcd

* Example Call: glcd_setcolumn(cmd);
 */
void glcd_setcolumn(unsigned char column)
{
	if(column < 64)
	{
		/* Set CS1 (CS1=1 and CS2=0) The right side is selected(column>64) */
		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x00);
		glcd_cmd(0x40 | column);
		//SysCtlDelay(6700);
	}
	else
	{
		/* Set CS2 (CS2=1 and CS1=0) The right side is selected(column>64) */
		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3,0x08);
		glcd_cmd(0x40 | (column-64));
		//SysCtlDelay(6700);
	}

}



/* void glcd_cleardisplay()
 * This function clears the data on GLCD by writing 0 on all pixels.

*Input: column
* Output: none
* Description: setting page for lcd

* Example Call: glcd_setcolumn(cmd);
 */
void glcd_cleardisplay()
{
	unsigned char i,j;
	for(i=0;i<8;i++)
	{
		glcd_setpage(i);
		for(j=0;j<128;j++)
		{
			glcd_setcolumn(j);
			glcd_data(0x00);
		}
	}
}

/* function:-- void setOutput()
 * Description :-- This function set all the Output pins which are needed to be set .

*Input: none
* Output: none
* 

* Example Call: setOutput();
 */
void setOutput()
{
	/* Configure PE5 (RST), PE0 to PE3 (D0 to D3) and PB4 to PB7(D4 to D7) as output pins */
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_5|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

	/* Configure RS as output */
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_3);

	/*This ensures buzzer remains OFF, since PC4 when logic 0 turns ON buzzer */
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_6);


	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_3);
}
/*

* Function Name: buzzerOff()

* Input: none

* Output: none

* Description: Setting buzzer off for GameConsole 

* Example Call: buzzerOff();

*/

void buzzerOff()
{
	GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4,16);
}
/*

* Function Name: setInput()

* Input: none

* Output: none

* Description: Setting PORTF pin 0 as input pin 

* Example Call: setInput();
*/
void setInput()
{
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY;    // unlocking sw2 switch
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0;
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_0);
}
/*

* Function Name: drawMickey()

* Input: none

* Output: none

* Description: this function draws mickey on GLCD screen

* Example Call: drawMickey();
*/
void drawMickey(void)
{
	unsigned char i,j;
	uint32_t mickey_counter;
	for(i=0;i<8;i++)  // for all 8 pages in screen
	{
		glcd_setpage(i);  //setting page for glcd
		for(j=0;j<128;j++)  // for all 128 columns in glcd
		{
			glcd_setcolumn(j); //setting column in GLCD
			if(mickey_counter<1024) 
			{
				glcd_data(one[mickey_counter]); // drawing mickey
				mickey_counter++;
			}
			else
			{
				break;
			}
		}
	}
}
/*

* Function Name: drawPanther()

* Input: panther bitmap file

* Output: none

* Description: this function plays panther animation and controls its speed using Joystics

* Example Call: drawPanther();
*/
void drawPanther(unsigned char (panther)[1024]){
		unsigned char i,j;
		uint32_t panther_counter = 0;
		for(i=0;i<8;i++) // running for all 8 pages 
		{
			glcd_setpage(i); //setting page for glcd
			for(j=0;j<128;j++) // running for all 128 columns in a page
			{
				glcd_setcolumn(j); //setting column for glcd
				if(panther_counter<1024)
				{
					glcd_data(panther[panther_counter]); // drawing panther
					panther_counter++;
				}
				else
				{
					break;
				}
			}
		}
}
/*

* Function Name: drawSquare()

* Input: none

* Output: none

* Description: this function draws square on GLCD

* Example Call: drawSquare();
*/
void drawSquare(void)
{
	unsigned char j;
	glcd_setpage(4);
	for(j=60;j<68;j++)
	{
		glcd_setcolumn(j);
		glcd_data(0xff);
	}
}
/*

* Function Name: drawcSquare()

* Input: r,c (r-page number,c-- column no)

* Output: none

* Description: this function draws square on GLCD

* Example Call: drawcSquare();
*/
void drawcSquare(unsigned char r,unsigned char c)
{
	glcd_setpage(r);
	uint32_t column;
	for(column=c;column<c+8;column++)
	{
		glcd_setcolumn(column);
		glcd_data(0xff);
	}
}

/*

* Function Name: configureADC()

* Input: none

* Output: none

* Description: this function used to configure ADC 

* Example Call: configureADC();
*/
void configureAdc(void)
{
	ADCSequenceDisable(ADC0_BASE, 0);
	ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH7| ADC_CTL_IE);
	ADCSequenceStepConfigure(ADC0_BASE,0, 1, ADC_CTL_CH6 | ADC_CTL_IE |ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 0);
}
void ADC0IntHandler(void)
{
    uint32_t ui32ADC0Value[8];
     //clear interrupt flag
     ADCIntClear(ADC0_BASE, 0);

     ADCSequenceDataGet(ADC0_BASE,0,ui32ADC0Value);


}

/*

* Function Name: displayPantherAnimation()

* Input: none

* Output: none

* Description: this function plays panther animation and controls its speed using Joystics

* Example Call: displayPantherAnimation();
*/
void displayPantherAnimation(){

	uint8_t count = 1;
	uint32_t ui32ADC0Value[8];
	while(1){

			glcd_cleardisplay();  // first clear glcd display
			switch(count){  //selecting which panther pitcure to show on GLCD
				case 1:
					drawPanther(one);
					break;
				case 2:
					drawPanther(two);
					break;
				case 3:
					drawPanther(three);
					break;
				case 4:
					drawPanther(four);
					break;
				case 5:
					drawPanther(five);
					break;
				case 6:
					drawPanther(six);
					break;
				case 7:
					drawPanther(seven);
					break;
				case 8:
					drawPanther(eight);
					break;
			}

			count++;

			if(count > 8){
				count = 1;
			}
			SysCtlDelay(67000*delay);  //applying delay

			ADCIntClear(ADC0_BASE, 0);  //clear adc
			            ADCProcessorTrigger(ADC0_BASE, 0);
			            while(!ADCIntStatus(ADC0_BASE, 0, false))
			            {
			            }
			            ADCSequenceDataGet(ADC0_BASE, 0, ui32ADC0Value);  //getting ADC value for joystic and controlling animation
			            if((ui32ADC0Value[0]>600 && ui32ADC0Value[0]<3000) && (ui32ADC0Value[1]<3000 && ui32ADC0Value[1]>600))
			                        {
			                           delay=30;  //setting delay to 30*0.5  ms

			                        }
			            else if(ui32ADC0Value[1]<30 && (ui32ADC0Value[0]<3000 && ui32ADC0Value[0]>600 ))
			                     {
			                         delay=1; //setting delay to 0.5 ms
			                      }
			                 else if(ui32ADC0Value[1]>3000 && (ui32ADC0Value[0]<3000 && ui32ADC0Value[0]>600 ))
			                 {
			                     delay=100; //setting delay to 100*0.5 ms
			                 }
	}
	//	SysCtlDelay(67000);
}

/*

* Function Name: squareControl()

* Input: none

* Output: none

* Description: this function control square position

* Example Call: squareControl();
*/
void squareControl(void)
{
	uint32_t ui32ADC0Value[8];
		uint32_t d0,d1;
		char pageNumber=4,prevPage=40;
		uint32_t column=60,prevCol=60;
	    drawSquare();  //drawing square on glcd scrren


		/* Select a page and display lines on it from column 0 to 127 */
		while(1)
		{
			ADCIntClear(ADC0_BASE, 0);
			ADCProcessorTrigger(ADC0_BASE, 0);
			while(!ADCIntStatus(ADC0_BASE, 0, false))
			{
			}
			ADCSequenceDataGet(ADC0_BASE, 0, ui32ADC0Value);  //getting ADC value based on joystic function
			if((ui32ADC0Value[0]>600 && ui32ADC0Value[0]<3000) && (ui32ADC0Value[1]<3000 && ui32ADC0Value[1]>600))
			{// for normal position
				prevPage=pageNumber; 
				prevCol=column;

			}
			else if(ui32ADC0Value[0]<30 && (ui32ADC0Value[1]<3000 && ui32ADC0Value[1]>600 ))
			{
				if(pageNumber<7)
				{
				pageNumber+=1; // for down position incrementing page number 
				}
			}
			else if(ui32ADC0Value[1]<30 && (ui32ADC0Value[0]<3000 && ui32ADC0Value[0]>600 ))
			{
				if(column<127)
				{
					column+=8;  // for right position incrementing column number
				}
			}
			else if(ui32ADC0Value[0]>3000 && (ui32ADC0Value[1]<3000 && ui32ADC0Value[1]>600 ))
			{
				if(pageNumber>0)
				{
				pageNumber-=1; // for up position decrementing page number
				}

			}else if(ui32ADC0Value[1]>3000 && (ui32ADC0Value[0]<3000 && ui32ADC0Value[0]>600 ))
			{
				if(column>0)
				{
				column-=8;   // for left position decrementing page numbe
				}
			}

			if(pageNumber!=prevPage || column!=prevCol)
			{
            glcd_cleardisplay(); 
            drawcSquare(pageNumber,column); // drawing square based on page number and column
			} 

		}
}
/*

* Function Name: main()

* Input:mone

* Output: none

* Description: this function draws square on GLCD
 * called by system

*/
int main()
{
	setup();//setting up peripherals
	enable_ADC(); /enable ADC
	setOutput(); // set Output for pins
	buzzerOff();  //buzzer off
	setInput(); //setting input for pins
	configureAdc(); //configuring ADC
	/*initialize glcd*/
	glcd_init(); //initialize glcd
	glcd_cleardisplay(); //clear display for glcd
	//uncomment the function u want to execute
	displayPantherAnimation(); 
	//squareCntrol()
	//drawMickey()
}
