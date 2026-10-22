#include "stm32g071xx.h"

void msdelay (uint32_t nCount);
uint8_t count;
uint32_t keypress;

int main (void){
	RCC->IOPENR |= 0x00000005;
	GPIOA->MODER &= 0xFFFFF3FF;
	GPIOA->MODER |= 0x00000400;
	GPIOA->OTYPER &= 0xFFFFFFDF;
	GPIOA->PUPDR &= 0xFFFFF3FF;
	
	GPIOC->MODER &= 0xF3FFFFFF;
	GPIOC->PUPDR &= 0xF3FFFFFF;

	while(1){
		keypress = GPIOC->IDR & 0x00002000;
		while (keypress!=0){
			keypress = GPIOC->IDR & 0x00002000;
		}
		count ++;
		if(count%2 != 0){
			for(int i=0; i<=5; i++){
				GPIOA->BSRR = 0x00000020;
				msdelay(500);
				GPIOA->BSRR = 0x00200000;
				msdelay(500);
			}
		}
		else{
			for(int i=0; i<=7; i++){
				GPIOA->BSRR = 0x00000020;
				msdelay(250);
				GPIOA->BSRR = 0x00200000;
				msdelay(250);
			}
		}	
	}
}

void msdelay (uint32_t nCount)
{
	long int loops;
	loops = 850 *nCount*4;
	while(loops--) {}
}
