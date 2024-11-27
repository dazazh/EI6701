#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

// 低优先级任务
void Task1(void *pvParameters) {
    int t=0;
    while (1) {
        t++;
        if(t==1000000000){ //过程中并未主动交出cpu，若任务输出了task2则证明task1被抢占
            printf("Task1 (Low Priority) running\n");
            t=0;
        } 
    }
}

// 高优先级任务
void Task2(void *pvParameters) {
    while (1) {
        printf("Task2 (High Priority) running\n");
        vTaskDelay(pdMS_TO_TICKS(1000));  // 每 1000 毫秒延时一次，释放 CPU
    }
}

int main_rank(void) {
    // 创建任务，低优先级任务优先级设置为 1
    xTaskCreate(Task1, "Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    // 创建任务，高优先级任务优先级设置为 2
    xTaskCreate(Task2, "Task2", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

    // 启动调度器
    vTaskStartScheduler();

    // 正常情况下不会运行到这里
    for (;;);
}
