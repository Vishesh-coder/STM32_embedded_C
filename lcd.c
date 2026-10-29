#include "stm32g071xx.h"

void GPIO_init(void);
void LCD_init(void);
void LCD_cmd(uint8_t comd);
void LCD_disp(uint8_t data);
void msdelay(uint32_t nCount);

int main(void) {
    char msg[] = "DoECE SVNIT";

    GPIO_init();
    LCD_init();

    LCD_cmd(0xC0); // Move cursor to line 2

    for (int i = 0; msg[i] != '\0'; i++) {
        LCD_disp(msg[i]);
    }

    while (1);
}

void GPIO_init(void) {
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN | RCC_IOPENR_GPIOCEN; // Enable GPIOB/C clock

    // Configure PB0-PB7 as Outputs
    GPIOB->MODER &= ~0x0000FFFF;
    GPIOB->MODER |=  0x00005555;

    // Configure PC0 (EN) & PC1 (RS) as Outputs
    GPIOC->MODER &= ~0x0000000F;
    GPIOC->MODER |=  0x00000005;
}

void LCD_init(void) {
    msdelay(20);    // Power-on delay
    LCD_cmd(0x38);  // 8-bit mode, 2 lines, 5x7 matrix
    LCD_cmd(0x0E);  // Display ON, cursor ON
    LCD_cmd(0x01);  // Clear display
    msdelay(2);
    LCD_cmd(0x06);  // Auto-increment cursor
}

void LCD_cmd(uint8_t comd) {
    GPIOB->ODR = (GPIOB->ODR & ~0xFF) | comd;
    GPIOC->ODR = (GPIOC->ODR & ~0x03) | 0x01; // RS=0, EN=1
    msdelay(1);
    GPIOC->ODR &= ~0x01;                      // EN=0
    msdelay(1);
}

void LCD_disp(uint8_t data) {
    GPIOB->ODR = (GPIOB->ODR & ~0xFF) | data;
    GPIOC->ODR = (GPIOC->ODR & ~0x03) | 0x03; // RS=1, EN=1
    msdelay(1);
    GPIOC->ODR &= ~0x01;                      // EN=0
    msdelay(1);
}

void msdelay (uint32_t nCount) {
    long int loops;
    loops = 850 * nCount * 4;
    while (loops--) {}
}