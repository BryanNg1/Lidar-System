// 2DX4StudioW20E1_ADCDemo
// This program illustrates the use of the ADC in the C language.
// Note the library headers asscoaited are PLL.h and SysTick.h,
// which define functions and variables used in PLL.c and SysTick.c.
// This program uses code directly from your course textbook.
//
// This example will be extended for in W21E0 and W21E1.
//
//  Written by Bryan Ng
//  ngb7 400181785 
//  24MHz Bus speed, PN0, PL4 for distance status and displacement status ports
//  March, 2 2020

#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "PLL.h"
#include "SysTick.h"

void PortL_Init(void){
	//external LED
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R10;	//activate clock
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R10) == 0){};	
	GPIO_PORTL_DIR_R = 0b00010000;  //set PL4 as an output     								    								
  GPIO_PORTL_DEN_R = 0b00010000;  //enable PN4      								    									
	return;
}

void PortN_Init(void){
	//onboard LED	
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12;				//activate clock
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R12) == 0){};
	GPIO_PORTN_DIR_R = 0b00000001;  //set PN0 as output      								   								
  GPIO_PORTN_DEN_R = 0b00000001;  //enable PN0      								
	return;
}

void PortM_Init(void){
	//motor registers and button
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R11;	 //activate clock
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R11) == 0){};	
	GPIO_PORTM_DIR_R = 0b00111100;  							//set PM2-PM5 as output to motor
  GPIO_PORTM_DEN_R = 0b00111101;						//enable PM2-PM5 and also PM0 as an input button press
	return;
}
		

int main(){
	SysTick_Init();					// Initialize SysTick
	PLL_Init();						  // initialize PLL
	PortL_Init();						// Initialize Port L 
	PortM_Init();					  // Initialize Port M 
	PortN_Init();						// Initialize Port N 
	int count;              // count definition
	
	while(1){ //while loop to keep program running 
		if((GPIO_PORTM_DATA_R & 0b00000001) == 0b00000000){ //checks to see if portm is equal to 0 when "and" with 1
			GPIO_PORTL_DATA_R = 0b00000000; //reset portL to 0
			for(count = 1; count <= 512; count++){ //loop to count 512 times (for motor rotation (from studios))
				if(count%64 == 0){ //take modulus of count by 64 (1/8 of 512) as the LED commanded by portN needs to blink every 45 degrees / 360 degrees
					GPIO_PORTN_DATA_R = 0b00000001; // if the count is equal to a multiple of 64, portn data set equal to 1 and thus LED will blink
				}else{ 
					GPIO_PORTN_DATA_R = 0b00000000; // turns light off otherwise
				}
				SysTick_Wait10ms(5); //calls systick wait to delay motor (modified based on student number)
				GPIO_PORTM_DATA_R = 0b00100100;
				SysTick_Wait10ms(5);
				GPIO_PORTM_DATA_R = 0b00110000;
				SysTick_Wait10ms(5);
				GPIO_PORTM_DATA_R = 0b00011000;
				SysTick_Wait10ms(5);
				GPIO_PORTM_DATA_R = 0b00001100;
				
			}
		}else{
			GPIO_PORTN_DATA_R = 0b00000000; //ensure the onboard LED is off
			GPIO_PORTL_DATA_R = 0b00010000; //ensure the external LED is ON when the motor stops running 
		}
	}
}

