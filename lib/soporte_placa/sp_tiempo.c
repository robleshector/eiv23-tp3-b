#include <soporte_placa/sp_tiempo.h>
#include <stdbool.h> // bool, true, false
#include <stdint.h>  // uint32_t
#include <stddef.h>  // size_t
#include <stm32f1xx.h> // __WFI

/* Temporización */

/**
 * @brief Variable actualizada una vez por milisegundo en el handler
 * de interrupción del timer del sistema (SysTick)
 * 
 */
static uint32_t volatile ticks;

void SP_delay(uint32_t tiempo){
    uint32_t const ticks_inicial = ticks;
    uint32_t tiempo_transcurrido = ticks - ticks_inicial;
    while(tiempo_transcurrido < tiempo){
        // https://arm-software.github.io/CMSIS_5/Core/html/group__intrinsic__CPU__gr.html#gaed91dfbf3d7d7b7fba8d912fcbeaad88
        __WFI();
        tiempo_transcurrido = ticks - ticks_inicial;
    }

}

#ifndef SP_MAX_TIMEOUTS
#define SP_MAX_TIMEOUTS 4
#endif

typedef struct SP_TimeoutDescriptor{
    uint32_t volatile tiempo;
    SP_TimeoutHandler volatile handler;
    void *volatile  param;
} SP_TimeoutDescriptor;

SP_TimeoutDescriptor timeoutDescriptors[SP_MAX_TIMEOUTS];


bool SP_Timeout(uint32_t const tiempo,SP_TimeoutHandler const handler,void *const param){
    bool hecho = false;
    __disable_irq();
    for(size_t i=0;i<SP_MAX_TIMEOUTS;++i){
        SP_TimeoutDescriptor * const td = timeoutDescriptors + i;
        if (td->tiempo) continue;
        td->tiempo = tiempo;
        td->handler = handler;
        td->param = param;
        hecho = true;
        break;
    }
    __enable_irq();
    return hecho;
}

void SysTick_Handler(void){
    ++ticks;
    for (size_t i=0;i<SP_MAX_TIMEOUTS;++i){
        SP_TimeoutDescriptor *const td = timeoutDescriptors + i;
        if (td->tiempo){
            const uint32_t tiempo_restante = --td->tiempo;
            if(!tiempo_restante && td->handler){
                td->handler(td->param);
            }
        } 
    }
}