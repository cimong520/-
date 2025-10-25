#ifndef __TASK_H
#define __TASK_H

#include "../Start/stm32f10x.h"


// 声明全局变量（在task.c中定义）
extern volatile uint32_t system_time;   // 系统时间计数器（单位：毫秒）
extern uint16_t task_ready_bitmap;      // 任务就绪位图（每位对应一个任务）
extern uint16_t task_active_bitmap;     // 任务激活位图（每位对应一个任务）

// 任务ID类型定义（0-9为有效ID，0xFF表示无效ID）
typedef uint8_t TaskID;
#define INVALID_TASK_ID 0xFF

// 任务优先级定义（从高到低）
typedef enum {
    PRIORITY_CRITICAL = 0,  // 最高优先级（实时性要求高的任务）
    PRIORITY_HIGH,          // 高优先级（重要任务）
    PRIORITY_NORMAL,        // 普通优先级（一般任务）
    PRIORITY_LOW,           // 低优先级（后台任务）
    PRIORITY_COUNT          // 优先级总数（内部使用）
} TaskPriority;

// 任务状态定义
typedef enum {
    TASK_READY,     // 任务就绪（等待执行）
    TASK_RUNNING,   // 任务正在执行中
    TASK_SUSPENDED  // 任务挂起（不参与调度）
} TaskState;

// 任务控制块 (TCB) - 存储任务的所有信息
typedef struct {
    void (*task_func)(void);  // 任务函数指针（无参数无返回值）
    uint32_t interval;        // 执行间隔（毫秒），0表示每次循环都执行
    uint32_t last_run;        // 上次执行时间（系统时间戳）
    uint32_t max_exec_time;   // 最大执行时间（用于性能监控）
    TaskState state;          // 当前任务状态
    const char *name;         // 任务名称（调试用）
    TaskPriority priority;    // 任务优先级
} Task_t;

/********************* 任务管理接口 *********************/

/**
 * @brief 初始化任务系统
 * @note 必须在添加任何任务前调用
 */
void Task_Init(void);

/**
 * @brief 添加新任务
 * @param task_func 任务函数指针（void func(void)形式）
 * @param interval 执行间隔（毫秒），0表示每次调度循环都执行
 * @param priority 任务优先级
 * @param name 任务名称（用于调试）
 * @return TaskID 分配的任务ID（INVALID_TASK_ID表示添加失败）
 * 
 * @example 
 * TaskID myTask = Task_Add(MyTaskFunc, 100, PRIORITY_NORMAL, "MyTask");
 */
TaskID Task_Add(void (*task_func)(void), 
               uint32_t interval, 
               TaskPriority priority,
               const char *name);

/**
 * @brief 删除任务
 * @param id 要删除的任务ID
 * 
 * @note 删除后任务不再执行，ID可被新任务重用
 */
void Task_Remove(TaskID id);

/**
 * @brief 挂起任务
 * @param id 要挂起的任务ID
 * 
 * @note 挂起的任务保留状态，可通过Task_Resume恢复
 */
void Task_Suspend(TaskID id);

/**
 * @brief 恢复挂起的任务
 * @param id 要恢复的任务ID
 * 
 * @note 恢复后任务会立即参与调度（重置last_run时间）
 */
void Task_Resume(TaskID id);

/**
 * @brief 修改任务执行间隔
 * @param id 任务ID
 * @param new_interval 新的执行间隔（毫秒）
 */
void Task_ChangeInterval(TaskID id, uint32_t new_interval);

/**
 * @brief 修改任务优先级
 * @param id 任务ID
 * @param new_priority 新的优先级
 */
void Task_ChangePriority(TaskID id, TaskPriority new_priority);

/**
 * @brief 运行任务调度器
 * @note 在主循环中不断调用此函数
 */
void Task_RunScheduler(void);

/**
 * @brief 获取当前系统时间
 * @return uint32_t 系统时间（毫秒）
 */
static inline uint32_t Task_GetSystemTime(void) {
    return system_time;
}

/**
 * @brief 检查任务是否就绪
 * @param task_id 任务ID
 * @return uint8_t 1=就绪，0=未就绪
 * 
 * @note 内部使用，通常不需要直接调用
 */
static inline uint8_t Task_IsReady(uint8_t task_id) {
    return (task_ready_bitmap & (1 << task_id)) != 0;
}

/**
 * @brief 清除任务就绪标志
 * @param task_id 任务ID
 * 
 * @note 内部使用，任务执行后自动调用
 */
static inline void Task_ClearReadyFlag(uint8_t task_id) {
    task_ready_bitmap &= ~(1 << task_id);
}

/**
 * @brief 系统心跳更新（由SysTick中断调用）
 * @note 每毫秒调用一次，更新系统时间和任务状态
 */
void Task_UpdateTick(void);

#endif
