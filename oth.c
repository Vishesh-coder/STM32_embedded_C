#include <ARMCM0plus.h>

int main(void){
	uint8_t binum = 0x052;
	uint8_t temp = 0;
	volatile uint8_t hundred, tens, ones;
	
	temp = binum;
	if(binum >= 100){
		hundred = temp/100;
		temp = temp % 100;
	}
	else 
		hundred = 0;
	
	if(binum >= 10){
		tens = temp/10;
		temp = temp%10;
	}
	else 
		tens = 0;
	
	ones = temp;
	while(1){}
}
