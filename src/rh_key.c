



#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "rh_key.h"


#ifdef __cplusplus
extern "C"{
#endif




#define PIN_KEY_B              GPIO_PIN_0
#define GPIOx_KEY_B            GPIOA       



static inline void __configXXX(){
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();  

    
    GPIO_Initure.Pin   = PIN_KEY_B;            
    GPIO_Initure.Mode  = GPIO_MODE_IT_FALLING;     
    GPIO_Initure.Pull  = GPIO_NOPULL;        
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;   
    HAL_GPIO_Init( GPIOx_KEY_B, &GPIO_Initure);
    

    HAL_NVIC_SetPriority( EXTI0_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ  ( EXTI0_IRQn);
}


int rh_key__init( void){
    
    __configXXX();


    return 0;
}


int rh_key__read( void){

    if( GPIO_PIN_RESET==HAL_GPIO_ReadPin( GPIOx_KEY_B, PIN_KEY_B) ){
        return RH_KEY__B;
    }

    return -1;
}




#ifdef __cplusplus
}
#endif