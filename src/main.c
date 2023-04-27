#include <stm32f1xx.h>


enum {LUZ_ON=0,LUZ_OFF=1,PULSADOR_ACTIVO=0,PULSADOR_NORMAL=1};

int main(void){
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN;
    // A15 pulsador, con resistencia pull hacia el nivel normal
    GPIOA->BSRR = 1 << ((PULSADOR_NORMAL)? 15:15+16);
    GPIOA->CRH =   (GPIOA->CRH & ~(GPIO_CRH_MODE15 | GPIO_CRH_CNF15))
                 | GPIO_CRH_CNF15_1;
    // C13 luz, inicialmente apagada
    GPIOC->BSRR = 1 << ((LUZ_OFF)? 13:13+16);
    GPIOC->CRH =   (GPIOC->CRH & ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13))
                 | GPIO_CRH_MODE13_1;
    for(;;){
        while((GPIOA->IDR & (1<<15)) != (PULSADOR_ACTIVO << 15));
        GPIOC->BSRR = 1 << ((LUZ_ON)? 13:13+16);
        for(int j=0;j<60000;++j){
            for(int volatile i=0;i<(8000000/(1000*13));++i);
        }
        GPIOC->BSRR = 1 << ((LUZ_OFF)? 13:13+16);
    }
    return 0;
}