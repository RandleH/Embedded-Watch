
project(Embedded-Watch)


set( PROJECT_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/src)
set( MCU_FAMILY      STM32F4xx)
set( MCU_MODEL       STM32F411xx)
set( CPU_PARAMETERS  -mcpu=cortex-m4
                     -mthumb
                     -mfpu=fpv4-sp-d16
                     -mfloat-abi=hard)

set( STARTUP_SCRIPT  
     ${CMAKE_CURRENT_SOURCE_DIR}/framework/framework-cmsis-stm32f4/Source/gcc/startup_stm32f411xe.s)


set(EXECUTABLE ${CMAKE_PROJECT_NAME})

enable_language( C CXX ASM)
set( CMAKE_C_STANDARD              99)
set( CMAKE_C_STANDARD_REQUIRED     ON)
set( CMAKE_C_EXTENSIONS            ON)
set( CMAKE_CXX_STANDARD            99)
set( CMAKE_CXX_STANDARD_REQUIRED   ON)
set( CMAKE_CXX_EXTENSIONS          ON)

set( STM32CUBEMX_INCLUDE_DIRECTORIES
     ${CMAKE_CURRENT_SOURCE_DIR}/framework/framework-stm32cubef4/Drivers/${MCU_FAMILY}_HAL_Driver/Inc
     ${CMAKE_CURRENT_SOURCE_DIR}/framework/framework-stm32cubef4/Drivers/${MCU_FAMILY}_HAL_Driver/Inc/Legacy
     ${CMAKE_CURRENT_SOURCE_DIR}/framework/framework-stm32cubef4/Drivers/CMSIS/Core/Include
     ${CMAKE_CURRENT_SOURCE_DIR}/framework/framework-stm32cubef4/Drivers/CMSIS/Device/ST/${MCU_FAMILY}/Include
     ${CMAKE_CURRENT_SOURCE_DIR}/framework/framework-stm32cubef4/Drivers/CMSIS/Include)

set( PROJECT_INCLUDE_DIRECTORIES
     ${CMAKE_CURRENT_SOURCE_DIR}/src  )

file( GLOB_RECURSE STM32CUBEMX_SOURCES   
      ${CMAKE_CURRENT_SOURCE_DIR}/??? )     

file( GLOB_RECURSE PROJECT_SOURCES FOLLOW_SYMLINK
      ${CMAKE_CURRENT_SOURCE_DIR}/??? )

add_excutable( ${EXCUTABLE} 
               ${STM32CUBEMX_SOURCES} 
               ${PROJECT_SOURCES} 
               ${STARTUP_SCRIPT})

target_compile_definitions( ${EXCUTABLE} PRIVATE $<$<CONFIG:Debug>:DEBUG> 
                            ${MCU_MODEL}
                            USE_HAL_DRIVER)               

target_include_directories( ${EXCUTABLE} PRIVATE
                            ${STM32CUBEMX_INCLUDE_DIRECTORIES}
                            ${PROJECT_INCLUDE_DIRECTORIES})                           

                             























