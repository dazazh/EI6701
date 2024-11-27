#define configUSE_EDF_SCHEDULER                    1
#include <stdio.h>
#include <pthread.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Local includes. */
#include "console.h"

/* Priorities at which the tasks are created. */
#define mainQUEUE_RECEIVE_TASK_PRIORITY    ( tskIDLE_PRIORITY + 2 )
#define mainQUEUE_SEND_TASK_PRIORITY       ( tskIDLE_PRIORITY + 1 )

/* The rate at which data is sent to the queue.  The times are converted from
 * milliseconds to ticks using the pdMS_TO_TICKS() macro. */
#define mainTASK_SEND_FREQUENCY_MS         pdMS_TO_TICKS( 200UL )
#define mainTIMER_SEND_FREQUENCY_MS        pdMS_TO_TICKS( 2000UL )

/* The number of items the queue can hold at once. */
#define mainQUEUE_LENGTH                   ( 2 )

/* The values sent to the queue receive task from the queue send task and the
 * queue send software timer respectively. */
#define mainVALUE_SENT_FROM_TASK           ( 100UL )
#define mainVALUE_SENT_FROM_TIMER          ( 200UL )

/*-----------------------------------------------------------*/

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvQueueReceiveTask( void * pvParameters );
static void prvQueueSendTask( void * pvParameters );

/*
 * The callback function executed when the software timer expires.
 */
static void prvQueueSendTimerCallback( TimerHandle_t xTimerHandle );

/*-----------------------------------------------------------*/

/* The queue used by both tasks. */
static QueueHandle_t xQueue = NULL;

/* A software timer that is started from the tick hook. */
static TimerHandle_t xTimer = NULL;

/*-----------------------------------------------------------*/

/*** SEE THE COMMENTS AT THE TOP OF THIS FILE ***/
#define configMAX_PRIORITIES      5

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>


void vTask1(void *pvParameters) {
    // low priority (large deadline), high computation
    int i = 0;
    for (;;) {
        i++;
        if (i == 10000000){
            i = 0;
            printf("Task 1 running at tick: %lu\n", xTaskGetTickCount());
        }
    }
}

void vTask2(void *pvParameters) {
    // high priority (small deadline), low computation
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(30); // 每30ms运行一次

    // 初始化 xLastWakeTime 为当前时间
    xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        printf("Task 2 running at tick: %lu\n", xTaskGetTickCount());
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void main_edf( void ) {
    xTaskCreateWithDeadline(vTask1, "Task1", configMINIMAL_STACK_SIZE, NULL, 3, NULL, 18);
    xTaskCreateWithDeadline(vTask2, "Task2", configMINIMAL_STACK_SIZE, NULL, 2, NULL, 5);
    vTaskStartScheduler();
    for (;;);
}