#include <stm32f1xx.h>


enum {LUZ_ON=0,LUZ_OFF=1,PULSADOR_ACTIVO=0,PULSADOR_NORMAL=1};

int main(void){
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN;
    // B9 pulsador, con resistencia pull hacia el nivel normal
    GPIOB->BSRR = 1 << (9+((PULSADOR_NORMAL)? 0:16));
    GPIOB->CRH =   (GPIOB->CRH & ~(GPIO_CRH_MODE9 | GPIO_CRH_CNF9))
                 | GPIO_CRH_CNF9_1;
    // C13 luz, inicialmente apagada
    GPIOC->BSRR = 1 << (13+((LUZ_OFF)? 0:16));
    GPIOC->CRH =   (GPIOC->CRH & ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13))
                 | GPIO_CRH_MODE13_1;
    for(;;){
        while((GPIOB->IDR & (1<<9)) != (PULSADOR_ACTIVO << 9));
        GPIOC->BSRR = 1 << (13+((LUZ_ON)? 0:16));
        for(int j=0;j<60000;++j){
            for(int volatile i=0;i<(8000000/(1000*13));++i);
        }
        GPIOC->BSRR = 1 << (13+((LUZ_OFF)? 0:16));
    }
    return 0;
}