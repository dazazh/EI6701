#define configUSE_PREEMPTION            0  // 禁用抢占式调度
#define configUSE_TIME_SLICING          1  // 启用时间片轮转
#define configTICK_RATE_HZ    100          // 定义时间片轮转频率为100Hz
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"

// 任务a函数（输出1）
void rbTask1(void *pvParameters) {
    // int t = 0;
    // while (1) {
    //     // t++;
    //     // if (t == 1000000) {
    //     //     printf("Task 1 running at tick: %lu\n", xTaskGetTickCount());
    //     //     t = 0;
    //     // }
        
    // }
    // vTaskDelete(NULL);  // 任务结束，删除自己
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
    printf("rbTask1 完成\n");
    vTaskDelete(NULL);
}

// 任务b函数（输出2）
void rbTask2(void *pvParameters) {
    // int t = 0;
    // while (1) {
    //     // t++;
    //     // if (t == 1000000) {
    //     //     printf("Task 2 running at tick: %lu\n", xTaskGetTickCount());
    //     //     t = 0;
    //     // }
    // }
    // vTaskDelete(NULL);  // 任务结束，删除自己
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
    printf("rbTask2 完成\n");
    vTaskDelete(NULL);
}

int main_round_robin(void) {
    // 创建任务a，优先级为2
    
    xTaskCreate_ddl(rbTask1, "rbTask1", configMINIMAL_STACK_SIZE, NULL, 2, NULL,70);
    xTaskCreate_ddl(rbTask2, "rbTask2", configMINIMAL_STACK_SIZE, NULL, 2, NULL,70);
    // 创建任务b，优先级为2
   

    // 启动调度器
    vTaskStartScheduler();

    // 正常情况下不会运行到这里
    for (;;);
    return 0;
}

