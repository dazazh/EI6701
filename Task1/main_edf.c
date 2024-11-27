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

void PrintTaskStatus(void)
{
    /* 假设最多有 10 个任务 */
    const UBaseType_t uxArraySize = 10;
    TaskStatus_t pxTaskStatusArray[uxArraySize];
    UBaseType_t uxTaskCount;
    configRUN_TIME_COUNTER_TYPE ulTotalRunTime;

    /* 获取任务状态 */
    uxTaskCount = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, &ulTotalRunTime);

    /* 打印任务状态信息 */
    printf("Task Name      State   Priority   Stack Remaining   Runtime\r\n");
    printf("-----------------------------------------------------------\r\n");

    for (UBaseType_t i = 0; i < uxTaskCount; i++)
    {
        printf("%-15s %-7d %-10u %-15u %-10u\r\n",
               pxTaskStatusArray[i].pcTaskName,
               pxTaskStatusArray[i].eCurrentState,
               pxTaskStatusArray[i].uxCurrentPriority,
               pxTaskStatusArray[i].usStackHighWaterMark,
               pxTaskStatusArray[i].ulRunTimeCounter);
    }

    /* 如果启用了运行时间统计 */
    if (ulTotalRunTime > 0)
    {
        printf("Total Run Time: %u ticks\r\n", (unsigned int)ulTotalRunTime);
    }
    else
    {
        printf("Run time statistics not enabled.\r\n");
    }
}

void vTask1(void *pvParameters) {
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(100); // 每100ms运行一次
    int i = 0;
    // 初始化 xLastWakeTime 为当前时间
    xLastWakeTime = xTaskGetTickCount();
    for (;;) {
        i++;
        if (i == 1000000){
            i = 0;
             printf("Task 1 running at tick: %lu\n", xTaskGetTickCount());
        }
       
        // vTaskDelayUntil(&xLastWakeTime, xFrequency);

    }
}

void vTask2(void *pvParameters) {
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(30); // 每30ms运行一次

    // 初始化 xLastWakeTime 为当前时间
    xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        printf("Task 2 running at tick: %lu\n", xTaskGetTickCount());
        // 延时到下一个周期
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void vTask3(void *pvParameters) {
    for (;;) {
    // printf("Task3 is running\n");
    // vTaskSetDeadline(NULL, xTaskGetTickCount() + 100);
    // 模拟任务执行
    vTaskDelay(50);
    }
}

void vTask4(void *pvParameters) {
for (;;) {
    // printf("Task4 is running\n");
    // vTaskSetDeadline(NULL, xTaskGetTickCount() + 100);
    // 模拟任务执行
    vTaskDelay(50);
}
    
}

void main_edf( void ) {
    printf("ddl\n");
    // xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    // xTaskCreate(vTask2, "Task2", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
    xTaskCreateWithDeadline(vTask1, "Task1", configMINIMAL_STACK_SIZE, NULL, 2, NULL, 18);
    xTaskCreateWithDeadline(vTask2, "Task2", configMINIMAL_STACK_SIZE, NULL, 2, NULL, 5);
    vTaskStartScheduler();
    
    // If the scheduler starts successfully, this point should never be reached
    for (;;);
}