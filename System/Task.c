#include "task.h"
#include <string.h>

// 最大任务数量（根据需求调整）
#define MAX_TASKS 10

// 任务数组（存储所有任务信息）
Task_t tasks[MAX_TASKS];

// 全局系统时间（每毫秒递增）
volatile uint32_t system_time = 0;

// 任务位图（高效管理任务状态）
static uint16_t task_ready_bitmap = 0;   // 就绪位图（1=需要执行）
static uint16_t task_active_bitmap = 0;  // 激活位图（1=任务存在）

/**
 * @brief 初始化任务系统
 * 
 * 功能：
 * 1. 清零任务数组
 * 2. 重置系统时间
 * 3. 清空任务位图
 * 
 * 注意：必须在添加任务前调用
 */
void Task_Init(void) {
    memset(tasks, 0, sizeof(tasks));
    system_time = 0;
    task_ready_bitmap = 0;
    task_active_bitmap = 0;
}

/**
 * @brief 添加新任务
 * 
 * @param task_func 任务函数指针
 * @param interval 执行间隔（毫秒）
 * @param priority 任务优先级
 * @param name 任务名称
 * @return TaskID 分配的任务ID（0-9）或INVALID_TASK_ID（失败）
 * 
 * 工作流程：
 * 1. 查找第一个空闲任务槽（未激活的任务）
 * 2. 初始化任务控制块
 * 3. 设置任务激活位
 * 4. 返回任务ID
 */
TaskID Task_Add(void (*task_func)(void), 
                uint32_t interval, 
                TaskPriority priority,
                const char *name) 
{
    // 遍历所有任务槽（0到MAX_TASKS-1）
    for (TaskID i = 0; i < MAX_TASKS; i++) {
        // 检查任务槽是否空闲（未激活）
        if (!(task_active_bitmap & (1 << i))) {
            // 初始化任务控制块
            tasks[i] = (Task_t){
                .task_func = task_func,
                .interval = interval,
                .last_run = system_time,  // 记录当前时间
                .max_exec_time = 0,       // 初始最大执行时间
                .state = TASK_READY,      // 初始状态为就绪
                .priority = priority,
                .name = name
            };
            
            // 设置任务激活位
            task_active_bitmap |= (1 << i);
            
            // 返回任务ID
            return i;
        }
    }
    return INVALID_TASK_ID; // 无可用槽位
}

/**
 * @brief 删除任务
 * @param id 要删除的任务ID
 * 
 * 功能：
 * 1. 清除任务激活位
 * 2. 清除任务就绪位
 * 3. 设置任务状态为挂起
 */
void Task_Remove(TaskID id) {
    if (id >= MAX_TASKS) return; // 检查ID有效性
    
    // 清除激活位（任务不再参与调度）
    task_active_bitmap &= ~(1 << id);
    
    // 清除就绪位（如果任务在就绪队列）
    task_ready_bitmap &= ~(1 << id);
    
    // 设置任务状态为挂起
    tasks[id].state = TASK_SUSPENDED;
}

/**
 * @brief 挂起任务
 * @param id 要挂起的任务ID
 * 
 * 功能：
 * 1. 设置任务状态为挂起
 * 2. 清除任务就绪位
 */
void Task_Suspend(TaskID id) {
    if (id >= MAX_TASKS) return;
    
    tasks[id].state = TASK_SUSPENDED;
    task_ready_bitmap &= ~(1 << id);
}

/**
 * @brief 恢复挂起的任务
 * @param id 要恢复的任务ID
 * 
 * 功能：
 * 1. 设置任务状态为就绪
 * 2. 重置上次执行时间（避免立即执行）
 */
void Task_Resume(TaskID id) {
    if (id >= MAX_TASKS) return;
    
    tasks[id].state = TASK_READY;
    tasks[id].last_run = system_time; // 重置时间，避免立即执行
}

/**
 * @brief 修改任务执行间隔
 * @param id 任务ID
 * @param new_interval 新的执行间隔（毫秒）
 * 
 * 功能：
 * 1. 更新任务间隔
 * 2. 重置上次执行时间（确保新间隔立即生效）
 */
void Task_ChangeInterval(TaskID id, uint32_t new_interval) {
    if (id >= MAX_TASKS) return;
    
    tasks[id].interval = new_interval;
    tasks[id].last_run = system_time; // 重置时间，新间隔立即生效
}

/**
 * @brief 修改任务优先级
 * @param id 任务ID
 * @param new_priority 新的优先级
 */
void Task_ChangePriority(TaskID id, TaskPriority new_priority) {
    if (id >= MAX_TASKS) return;
    tasks[id].priority = new_priority;
}

/**
 * @brief 运行任务调度器（主循环中调用）
 * 
 * 工作流程：
 * 1. 如果没有就绪任务，直接返回
 * 2. 按优先级从高到低遍历
 * 3. 在每个优先级组内，遍历所有任务
 * 4. 执行符合条件的任务：
 *    - 任务已激活
 *    - 任务已就绪
 *    - 任务优先级匹配当前组
 *    - 任务状态为就绪
 * 5. 执行任务并记录执行时间
 */
void Task_RunScheduler(void) {
    // 如果没有就绪任务，直接返回
    if (task_ready_bitmap == 0) return;
    
    // 按优先级分组执行（从高到低）
    for (TaskPriority prio = PRIORITY_CRITICAL; prio < PRIORITY_COUNT; prio++) {
        // 遍历所有任务槽
        for (TaskID i = 0; i < MAX_TASKS; i++) {
            // 检查任务是否：激活、就绪、优先级匹配、状态就绪
            if ((task_active_bitmap & (1 << i)) && 
                (task_ready_bitmap & (1 << i)) &&
                tasks[i].priority == prio &&
                tasks[i].state == TASK_READY) 
            {
                // 清除就绪标志（防止重复执行）
                Task_ClearReadyFlag(i);
                
                // 设置任务状态为运行中
                tasks[i].state = TASK_RUNNING;
                
                // 记录任务开始时间
                uint32_t start_time = system_time;
                
                // 执行任务函数
                tasks[i].task_func();
                
                // 计算并更新最大执行时间
                uint32_t exec_time = system_time - start_time;
                if (exec_time > tasks[i].max_exec_time) {
                    tasks[i].max_exec_time = exec_time;
                }
                
                // 更新任务状态和时间
                tasks[i].last_run = system_time;
                tasks[i].state = TASK_READY;
            }
        }
    }
}

/**
 * @brief 系统心跳更新（每毫秒调用一次）
 * 
 * 工作流程：
 * 1. 更新系统时间（处理32位溢出）
 * 2. 遍历所有激活的任务
 * 3. 计算距离上次执行的时间
 * 4. 如果时间间隔已到且任务就绪，设置就绪位
 */
void Task_UpdateTick(void) {
    // 更新系统时间（安全处理32位溢出）
    if (system_time < 0xFFFFFFFF) {
        system_time++;
    } else {
        system_time = 0; // 处理计数器回绕
    }

    // 遍历所有任务槽
    for (TaskID i = 0; i < MAX_TASKS; i++) {
        // 只处理激活的任务
        if (task_active_bitmap & (1 << i)) {
            // 计算距离上次执行的时间（安全处理时间回绕）
            uint32_t elapsed;
            if (system_time >= tasks[i].last_run) {
                elapsed = system_time - tasks[i].last_run;
            } else {
                // 处理计数器回绕的情况
                elapsed = 0xFFFFFFFF - tasks[i].last_run + system_time + 1;
            }
            
            // 检查是否到达执行间隔且任务状态为就绪
            if (elapsed >= tasks[i].interval && tasks[i].state == TASK_READY) {
                // 设置任务就绪位
                task_ready_bitmap |= (1 << i);
            }
        }
    }
}
