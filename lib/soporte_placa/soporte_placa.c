#include <soporte_placa.h>
#include <stm32f1xx.h> // SystemCoreClockUpdate, SystemCoreClock, SysTick_Config
#include <stdint.h>    // uint32_t

// Implementación

/* Inicialización general */

void SP_init(void){
    // Ver documentación CMSIS
    // https://arm-software.github.io/CMSIS_5/Core/html/group__system__init__gr.html#gae0c36a9591fe6e9c45ecb21a794f0f0f
    SystemCoreClockUpdate();
    
    uint32_t const frecuencia_hertz = SystemCoreClock;
    uint32_t const cuentas_por_milisgundo = frecuencia_hertz/1000;

    // https://arm-software.github.io/CMSIS_5/Core/html/group__SysTick__gr.html#gabe47de40e9b0ad465b752297a9d9f427
    SysTick_Config(cuentas_por_milisgundo); // Configura SysTick y la interrupción
}
