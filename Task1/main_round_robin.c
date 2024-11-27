#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"

// 任务a函数（输出1）
void rbTask1(void *pvParameters) {
    int t = 0;
    while (1) {
        t++;
        if(t==1000000000){
            printf("Task1 running\n");
            t=0;
        } 
    }
    vTaskDelete(NULL);  // 任务结束，删除自己
}

// 任务b函数（输出2）
void rbTask2(void *pvParameters) {
    int t = 0;
     while (1) {
        t++;
        if(t==1000000000){ 
            printf("Task2 running\n");
            t=0;
        } 
    }
    vTaskDelete(NULL);  // 任务结束，删除自己
}

int main_round_robin(void) {
    // 创建任务a，优先级为2
    
    xTaskCreate(rbTask1, "rbTask1", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(rbTask2, "rbTask2", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    // 创建任务b，优先级为2
   

    // 启动调度器
    vTaskStartScheduler();

    // 正常情况下不会运行到这里
    for (;;);
    return 0;
}

