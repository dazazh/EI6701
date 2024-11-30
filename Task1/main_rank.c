#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

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
void Task1(void *pvParameters) {
    const TickType_t xFrequency = pdMS_TO_TICKS(333); // 每333ms运行一次
    while (1) {
        TickType_t xStartTime = xTaskGetTickCount();
        //printf("Task 1 running at tick: %lu\n", xTaskGetTickCount());
        mdelay(20000);
        //printf("Task 1 runned: %lu tick and finished at %lu\n", xTaskGetTickCount()-xStartTime, xTaskGetTickCount());
        //vTaskDelay(pdMS_TO_TICKS(1000));
        // t++;
        // printf("Task 1 running at tick: %lu\n", xTaskGetTickCount());
        // vTaskDelayUntil(&xLastWakeTime, xFrequency);
        // if(t==1000000000){ //过程中并未主动交出cpu，若任务输出了task2则证明task1被抢占
        //     // printf("Task1 (Low Priority) running\n");
            
        //     t=0;
        // } 
        break;
    }
    printf("Task1 完成\n");
    vTaskDelete(NULL);
}

// 高优先级任务
void Task2(void *pvParameters) {
   vTaskDelay(pdMS_TO_TICKS(200));

    // 初始化 xLastWakeTime 为当前时间
    while (1) {
        TickType_t xStartTime = xTaskGetTickCount();
        // printf("Task2 (High Priority) running\n");
        //printf("Task 2 running at tick: %lu\n", xTaskGetTickCount());
        mdelay(20000);
        //printf("Task 2 runned: %lu tick and finished at %lu\n", xTaskGetTickCount()-xStartTime, xTaskGetTickCount());
        //vTaskDelay(pdMS_TO_TICKS(100));  // 每 1000 毫秒延时一次，释放 CPU
        break;
    }
    printf("Task2 完成\n");
    vTaskDelete(NULL);
}

// void MyTask(void *pvParameters) {
//     TickType_t startTime, endTime;
//     TickType_t executionTime;

//     for (;;) {
//         // 记录开始时间
//         startTime = xTaskGetTickCount();

//         // 执行任务的代码（例如模拟耗时操作）
//         vTaskDelay(pdMS_TO_TICKS(100));  // 模拟100ms的任务执行

//         // 记录结束时间
//         endTime = xTaskGetTickCount();

//         // 计算任务的执行时间（以系统滴答计数为单位）
//         executionTime = endTime - startTime;

//         // 可以将执行时间通过串口输出或保存到日志中
//         printf("Task executed in %d ticks.\n", executionTime);

//         // 执行其他任务逻辑
//     }
// }

int main_rank(void) {
    // 创建任务，低优先级任务优先级设置为 1
    xTaskCreate_ddl(Task1, "Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL,70);

    // 创建任务，高优先级任务优先级设置为 2
    xTaskCreate_ddl(Task2, "Task2", configMINIMAL_STACK_SIZE, NULL, 2, NULL,70);

    // 启动调度器
    vTaskStartScheduler();

    // 正常情况下不会运行到这里
    for (;;);
}

