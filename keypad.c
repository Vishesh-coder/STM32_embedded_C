#include "stm32g071xx.h"

void GPIO_init(void);
void LCD_init(void);
void LCD_cmd(uint8_t comd);
void LCD_disp(uint8_t data);
void msdelay(uint32_t nCount);
char Keypad_scan(void);

const char keypad_map[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

int main(void) {
    char key;

    GPIO_init();
    LCD_init();

    LCD_cmd(0x80); // 1st line[cite: 5]
    LCD_disp('K'); 
    LCD_disp('e'); 
    LCD_disp('y'); 
    LCD_disp(':');

    while (1) {
        key = Keypad_scan();
        if (key != 0) {
            LCD_cmd(0xC0); // 2nd line[cite: 5]
            LCD_disp(key);
            msdelay(250);
        }
    }
}

void msdelay(uint32_t nCount) {
    for (uint32_t i = 0; i < nCount * 1000; i++) {
        __NOP();
    }
}

void GPIO_init(void) {
    // Enable GPIOA, GPIOB, GPIOC clocks
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOBEN | RCC_IOPENR_GPIOCEN;

    // PB0-PB7: LCD Data (Outputs)[cite: 5]
    GPIOB->MODER &= ~(0xFFFF);
    GPIOB->MODER |= 0x5555;

    // PC0 (EN), PC1 (RS): LCD Controls (Outputs)[cite: 5]
    GPIOC->MODER &= ~(0x0F);
    GPIOC->MODER |= 0x05;

    // PA8-PA11: Keypad Rows R1-R4 (Outputs)[cite: 4]
    GPIOA->MODER &= ~(0xFF << 16);
    GPIOA->MODER |= (0x55 << 16);
    GPIOA->ODR |= (0x0F << 8);

    // PC4-PC7: Keypad Columns C1-C4 (Inputs + Pull-Up)[cite: 4]
    GPIOC->MODER &= ~(0xFF << 8);
    GPIOC->PUPDR &= ~(0xFF << 8);
    GPIOC->PUPDR |= (0x55 << 8);
}

void LCD_cmd(uint8_t comd) {
    GPIOB->ODR = (GPIOB->ODR & ~0xFF) | comd; // PB0-PB7[cite: 5]
    GPIOC->ODR &= ~(1 << 1);                  // RS = 0[cite: 5]
    GPIOC->ODR |= (1 << 0);                   // EN = 1[cite: 5]
    msdelay(2);
    GPIOC->ODR &= ~(1 << 0);                  // EN = 0[cite: 5]
    msdelay(2);
}

void LCD_disp(uint8_t data) {
    GPIOB->ODR = (GPIOB->ODR & ~0xFF) | data; // PB0-PB7[cite: 5]
    GPIOC->ODR |= (1 << 1);                   // RS = 1[cite: 5]
    GPIOC->ODR |= (1 << 0);                   // EN = 1[cite: 5]
    msdelay(2);
    GPIOC->ODR &= ~(1 << 0);                  // EN = 0[cite: 5]
    msdelay(2);
}

void LCD_init(void) {
    msdelay(50);
    LCD_cmd(0x38); // 8-bit mode, 2 lines, 5x7 matrix[cite: 5]
    LCD_cmd(0x0C); // Display ON, cursor OFF[cite: 5]
    LCD_cmd(0x01); // Clear screen[cite: 5]
    msdelay(5);
    LCD_cmd(0x06); // Auto-increment cursor[cite: 5]
}

char Keypad_scan(void) {
    for (int r = 0; r < 4; r++) {
        GPIOA->ODR |= (0x0F << 8);          // Set PA8-PA11 HIGH[cite: 4]
        GPIOA->ODR &= ~(1 << (8 + r));      // Set active row LOW[cite: 4]
        msdelay(2);

        uint32_t col = (GPIOC->IDR >> 4) & 0x0F; // Read PC4-PC7[cite: 4]

        if (col != 0x0F) {
            if ((col & 0x01) == 0) return keypad_map[r][0]; // C1 (PC4)[cite: 4]
            if ((col & 0x02) == 0) return keypad_map[r][1]; // C2 (PC5)[cite: 4]
            if ((col & 0x04) == 0) return keypad_map[r][2]; // C3 (PC6)[cite: 4]
            if ((col & 0x08) == 0) return keypad_map[r][3]; // C4 (PC7)[cite: 4]
        }
    }
    return 0;
}