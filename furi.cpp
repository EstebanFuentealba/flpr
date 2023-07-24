#include "furi.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_intr_alloc.h>
// #include "queue.h"


void furi_init() {
    // furi_log_init();
    furi_record_init();
}

// void furi_run() {
//     // Comprobar si el planificador de tareas no ha sido iniciado
//     if(xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
//         // Iniciar el planificador de tareas de FreeRTOS
//         vTaskStartScheduler();
//     }
// }

void furi_run() {
    /* Start the FreeRTOS scheduler */
    // #ifdef RUN_THREAD
    // if(xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED) {
    // vTaskStartScheduler();
    // }
    // #endif
    
}