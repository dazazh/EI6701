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
// void Task1(void *pvParameters) {
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
// void Task2(void *pvParameters) {
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
void Task1(void *pvParameters) {
    printf("Task1 (Logging) started.\n");
    for (;;) {
        printf("Task1: Logging device data...\n");
        mdelay(500); // 执行500单位
        break;
    }
    printf("Task1 完成\n");
    vTaskDelete(NULL);
}

// 任务2：中优先级 - 状态检查
void Task2(void *pvParameters) {
    printf("Task2 (Status Checking) started.\n");
    for (;;) {
        printf("Task2: Checking device status... Current status: %d\n", deviceStatus);
        mdelay(200); // 执行200单位
        break;
    }
    printf("Task2 完成\n");
    vTaskDelete(NULL);    
}

// 任务3：高优先级 - 安全监控
void Task3(void *pvParameters) {
    printf("Task3 (Safety Monitoring) started.\n");
    for (;;) {
        if (deviceStatus >= safetyThreshold) {
            printf("Task3: Warning! Device status (%d) exceeds safety threshold (%d)!\n", deviceStatus, safetyThreshold);
        }
        mdelay(50); // 执行50单位
        break;
    }
    printf("Task3 完成\n");
    vTaskDelete(NULL);
}

void ChangeStatus(void *pvParameters){
    for(;;){
        deviceStatus = (deviceStatus + 1) % 15; // 模拟设备状态的变化
        printf("系统状态转为：%d\n",deviceStatus);
        vTaskDelay(pdMS_TO_TICKS(1000)); 
    }
}
int main_rank(void) {

    xTaskCreate_ddl(ChangeStatus, "ChangeStatus", configMINIMAL_STACK_SIZE, NULL, 1, &Task3Handle,-1);
    xTaskCreate_ddl(Task1, "Task1", configMINIMAL_STACK_SIZE, NULL, 2, &Task1Handle,10);
    xTaskCreate_ddl(Task2, "Task2", configMINIMAL_STACK_SIZE, NULL, 3, &Task2Handle,20);
    xTaskCreate_ddl(Task3, "Task3", configMINIMAL_STACK_SIZE, NULL, 4, &Task3Handle,40);
    
    // 模拟设备状态变化（在实际中由硬件或其他逻辑提供）
    // for (int i = 0; i < 20; i++) {
    //     vTaskDelay(pdMS_TO_TICKS(1000)); // 每隔1秒更新设备状态
    //     deviceStatus = (deviceStatus + 1) % 15; // 模拟设备状态的变化
    // }


    // // 创建任务，低优先级任务优先级设置为 1
    // xTaskCreate_ddl(Task1, "Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL,70);

    // // 创建任务，高优先级任务优先级设置为 2
    // xTaskCreate_ddl(Task2, "Task2", configMINIMAL_STACK_SIZE, NULL, 2, NULL,70);

    // 启动调度器
    vTaskStartScheduler();

    // 正常情况下不会运行到这里
    for (;;);
}

