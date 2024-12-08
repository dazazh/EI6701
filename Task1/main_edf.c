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

void mdelay(uint32_t ms)
{
    uint32_t i;
    for(i=0;i<ms*1000000;i++)
    {
        asm volatile("nop");
    }
}   
// 低优先级任务
// void edf_Task1(void *pvParameters) {
//     const TickType_t xFrequency = pdMS_TO_TICKS(333); // 每333ms运行一次
//     while (1) {
//         TickType_t xStartTime = xTaskGetTickCount();
//         //printf("Task 1 running at tick: %lu\n", xTaskGetTickCount());
//         mdelay(20000);
//         //printf("Task 1 runned: %lu tick and finished at %lu\n", xTaskGetTickCount()-xStartTime, xTaskGetTickCount());
//         //vTaskDelay(pdMS_TO_TICKS(1000));
//         // t++;
//         // printf("Task 1 running at tick: %lu\n", xTaskGetTickCount());
//         // vTaskDelayUntil(&xLastWakeTime, xFrequency);
//         // if(t==1000000000){ //过程中并未主动交出cpu，若任务输出了task2则证明task1被抢占
//         //     // printf("Task1 (Low Priority) running\n");
            
//         //     t=0;
//         // } 
//         break;
//     }
//     printf("Task1 完成\n");
//     vTaskDelete(NULL);
// }

// // 高优先级任务
// void edf_Task2(void *pvParameters) {
//    vTaskDelay(pdMS_TO_TICKS(200));

//     // 初始化 xLastWakeTime 为当前时间
//     while (1) {
//         TickType_t xStartTime = xTaskGetTickCount();
//         // printf("Task2 (High Priority) running\n");
//         //printf("Task 2 running at tick: %lu\n", xTaskGetTickCount());
//         mdelay(20000);
//         //printf("Task 2 runned: %lu tick and finished at %lu\n", xTaskGetTickCount()-xStartTime, xTaskGetTickCount());
//         //vTaskDelay(pdMS_TO_TICKS(100));  // 每 1000 毫秒延时一次，释放 CPU
//         break;
//     }
//     printf("Task2 完成\n");
//     vTaskDelete(NULL);
// }

// 定义任务句柄
TaskHandle_t Task1Handle, Task2Handle, Task3Handle;

// 模拟数据
int deviceStatus = 0;       // 模拟设备状态值
int safetyThreshold = 10;   // 安全临界值

// 任务1：低优先级 - 日志记录
void Log(void *pvParameters) {
    printf("Logging started.\n");
    for (;;) {
        printf("Logging device data...\n");
        mdelay(500); // 执行500单位
        break;
    }
    printf("Task:Log 完成\n");
    vTaskDelete(NULL);
}

// 任务2：中优先级 - 状态检查
void CheckStatus(void *pvParameters) {
    printf("Status Checking started.\n");
    for (;;) {
        printf("Checking device status... Current status: %d\n", deviceStatus);
        mdelay(200); // 执行200单位
        break;
    }
    printf("Task:CheckStatus 完成\n");
    vTaskDelete(NULL);    
}

// 任务3：高优先级 - 安全监控
void Monitor(void *pvParameters) {
    printf("Monitoring started.\n");
    for (;;) {
        if (deviceStatus >= safetyThreshold) {
            printf("Warning! Device status (%d) exceeds safety threshold (%d)!\n", deviceStatus, safetyThreshold);
        }
        mdelay(50); // 执行50单位
        break;
    }
    printf("Task:Monitor 完成\n");
    vTaskDelete(NULL);
}

void ChangeStatus(void *pvParameters){
    for(;;){
        deviceStatus = (deviceStatus + 1) % 15; // 模拟设备状态的变化
        printf("系统状态转为：%d\n",deviceStatus);
        vTaskDelay(pdMS_TO_TICKS(1000)); 
    }
}

void main_edf( void ) {
    xTaskCreateWithDeadline(ChangeStatus, "ChangeStatus", configMINIMAL_STACK_SIZE, NULL, 1, &Task3Handle,50);
    xTaskCreateWithDeadline(Log, "Log", configMINIMAL_STACK_SIZE, NULL, 4, &Task1Handle,40);
    xTaskCreateWithDeadline(CheckStatus, "CheckStatus", configMINIMAL_STACK_SIZE, NULL, 3, &Task2Handle,20);
    xTaskCreateWithDeadline(Monitor, "Monitor", configMINIMAL_STACK_SIZE, NULL, 2, &Task3Handle,10);

    vTaskStartScheduler();
    for (;;);
}