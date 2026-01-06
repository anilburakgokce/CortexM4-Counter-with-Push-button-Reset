#include "stm32f4xx.h"

// Push button and LEDs
void init_config_GPIO_A_D(){
    // Enable Clock for GPIOA and GPIOD (Bits 0 and 3 of AHB1ENR)
    RCC->AHB1ENR |= 0x9;

    // configure PA0 for data input
    GPIOA->MODER &= ~0x3;
    GPIOA->PUPDR &= ~0x3;

    // configure PD12-15 for data output
    GPIOD->MODER &= ~(0xff<<24);
    GPIOD->MODER |= (0x55<<24);
    GPIOD->OTYPER &= ~(0xf<<12);
    GPIOD->PUPDR &= ~(0xff<<24);
}

void init_config_TIM6(){
    // Enable Clock for TIM6 (Bit 4 of APB1ENR)
    RCC->APB1ENR |= (1u<<4);

    // timer6 configure
    TIM6->ARR = 0x8000;
    TIM6->PSC = 32;
    TIM6->CR1 = 0x8; // one-pulse mode enabled
    TIM6->DIER |= 0x1; // interrupt enable
    TIM6->SR &= ~(1u);// clear the interrupt bit
}

// light the LEDs such that value % 16 = blue*8 + red*4 + orange*2 + green
void updateLEDs(int value){
    // we have only 4 LEDs
    value &= 0xf;

    // reset the LEDs
    GPIOD->BSRR = (0xf << 28);

    // set value to LEDs
    GPIOD->BSRR = (value << 12);
}

int main(){

    // init GPIOs and TIM6
    init_config_GPIO_A_D();
    init_config_TIM6();

    int value = 0;

    // init the LED and start the timer
    updateLEDs(value);
    TIM6->CR1 |= 1u;

    while(1){
        // poll the push-button (PA0)
        if(GPIOA->IDR & 0x1){
            // reset the value and counter
            value = 0;
            TIM6 ->CNT = 0;

            // clear the interrupt bit in case it is set
            TIM6->SR &= ~(1u);
        }

        // poll the interrupt bit
        else if(TIM6->SR & 0x1){
            // clear the interrupt bit
            TIM6->SR &= ~(1u);

            // update the value and LEDs
            value++;
            updateLEDs(value);

            //restart the timer
            TIM6->CR1 |= 1u;
        }
    }

    return 0;
}