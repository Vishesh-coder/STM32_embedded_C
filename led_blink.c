#include "stm32g071xx.h"

void msdelay (uint32_t nCount);

int main (void){
	RCC->IOPENR = RCC->IOPENR | 0x00000001;
	GPIOA->MODER &= 0xFFFFF3FF;
	GPIOA->MODER |= 0x00000400;
	GPIOA->OTYPER &= 0xFFFFFDF;
	GPIOA->PUPDR &= 0xFFFFF3FF;
	
	while(1){
		for(int i=0; i<=5; i++){
			GPIOA->BSRR = 0x00000020;
			msdelay(278);
			GPIOA->BSRR = 0x00200000;
			msdelay(278);
		}
		msdelay(1000);
		for(int i=0; i<=5; i++){
			GPIOA->BSRR = 0x00000020;
			msdelay(143);
			GPIOA->BSRR = 0x00200000;
			msdelay(143);
		}
		msdelay(1000);
	}	
}

void msdelay (uint32_t nCount)
{
	long int loops;
	loops = 850 *nCount*4;
	while(loops--) {}
}
