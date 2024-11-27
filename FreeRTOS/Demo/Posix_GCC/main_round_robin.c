#define configUSE_PREEMPTION            0  // 禁用抢占式调度
#define configUSE_TIME_SLICING          1  // 启用时间片轮转
#define configTICK_RATE_HZ    100          // 定义时间片轮转频率为100Hz


#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"

// 任务a函数（输出1）
void rbTask1(void *pvParameters) {
    while (1) {
        printf("Running Task1\n");   // 输出1
    }
    vTaskDelete(NULL);  // 任务结束，删除自己
}

// 任务b函数（输出2）
void rbTask2(void *pvParameters) {
    while (1) {
        printf("Runing Task2\n");   // 输出2
    }
    vTaskDelete(NULL);  // 任务结束，删除自己
}

int main(void) {
    // 创建任务a，优先级为2
    xTaskCreate(rbTask1, "rbTask1", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    
    // 创建任务b，优先级为2
    xTaskCreate(rbTask2, "rbTask2", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

    // 启动调度器
    vTaskStartScheduler();

    // 正常情况下不会运行到这里
    for (;;);
    return 0;
}
