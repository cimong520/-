# Test_ESP8266() 监控任务使用说明

## 📋 功能说明

`Test_ESP8266()` 是一个可选的监控任务，用于定期显示系统状态信息。

### **特点：**
- ✅ 完全解耦，不干扰业务逻辑
- ✅ 只显示状态，不处理数据
- ✅ 可以随时启用或禁用
- ✅ 不影响云端控制功能

## 🔧 如何启用

### **方法1：在任务调度器中启用（推荐）**

在 `User/main.c` 的 `Task_Initialization()` 函数中，取消注释这一行：

```c
void Task_Initialization(void){
    // ... 其他任务 ...
    
    // 启用ESP8266监控任务
    monitorTaskID = Task_Add(Test_ESP8266, 10000, PRIORITY_NORMAL, "ESP8266Monitor");
    //                                      ↑
    //                                   每10秒执行一次
}
```

### **方法2：在主循环中调用（不推荐）**

如果你想在主循环中调用（虽然不推荐），可以这样：

```c
while(1) {
    // ... 其他代码 ...
    
    Test_ESP8266();  // 每次循环都会执行
    
    Task_RunScheduler();
}
```

**注意：** 这种方式会导致每次循环都输出状态信息，可能会刷屏。

## 📊 输出示例

启用后，每10秒会输出一次系统状态：

```
=== 系统状态 ===
WiFi: FZKJ (172.168.5.138)
云端: 已连接
温度: 25°C  湿度: 60%
继电器: 开
================
```

## ⚙️ 自定义配置

### **修改输出周期：**

```c
// 每5秒输出一次
monitorTaskID = Task_Add(Test_ESP8266, 5000, PRIORITY_NORMAL, "ESP8266Monitor");

// 每30秒输出一次
monitorTaskID = Task_Add(Test_ESP8266, 30000, PRIORITY_NORMAL, "ESP8266Monitor");
```

### **修改输出内容：**

在 `Test_ESP8266()` 函数中添加或删除你需要的信息：

```c
void Test_ESP8266(void)
{
    Serial_SendString("\r\n=== 系统状态 ===\r\n");
    
    // 添加更多信息
    Serial_SendString("LED: ");
    Serial_SendString(data.flag.led ? "开" : "关");
    Serial_SendString("\r\n");
    
    Serial_SendString("温度阈值: ");
    Serial_SendNumber((uint32_t)data.Threshold.Temp, 2);
    Serial_SendString("°C\r\n");
    
    // ... 其他信息 ...
}
```

## 🎯 使用场景

### **开发调试阶段：**
- ✅ 启用监控任务
- 实时查看系统状态
- 方便排查问题

### **生产部署阶段：**
- ❌ 禁用监控任务
- 减少串口输出
- 节省系统资源

## 🔄 启用/禁用方法

### **启用：**
```c
// 在 Task_Initialization() 中
monitorTaskID = Task_Add(Test_ESP8266, 10000, PRIORITY_NORMAL, "ESP8266Monitor");
```

### **禁用：**
```c
// 在 Task_Initialization() 中注释掉
// monitorTaskID = Task_Add(Test_ESP8266, 10000, PRIORITY_NORMAL, "ESP8266Monitor");
```

## ⚠️ 注意事项

1. **不要在 Test_ESP8266() 中处理 wifi.rxover**
   - 所有数据处理由 `DataAnylize()` 完成
   - `Test_ESP8266()` 只负责显示状态

2. **不要在 Test_ESP8266() 中调用 Clear_BuffData()**
   - 会干扰数据处理流程
   - 导致云端控制失效

3. **合理设置输出周期**
   - 太频繁会刷屏
   - 太慢可能错过重要信息
   - 推荐：10秒

## 📝 完整示例

```c
// 任务初始化
void Task_Initialization(void){
    // 关键任务
    keyTaskID = Task_Add(Menu_key_set, 10, PRIORITY_CRITICAL, "KeyScan");
    
    // 业务任务
    dataTaskID = Task_Add(GetData, 50, PRIORITY_NORMAL, "GetData");
    alarmTaskID = Task_Add(Alarm, 100, PRIORITY_HIGH, "Alarm");
    IWDG_ID = Task_Add(IWDGADD, 1000, PRIORITY_HIGH, "IWDG_ID");
    wifiID = Task_Add(WIFI_Init, 5000, PRIORITY_NORMAL, "WIFI_Init");
    uploadID = Task_Add(upload_data, 30000, PRIORITY_NORMAL, "uploadID");
    
    // 控制任务
    TaskID controlTaskID = Task_Add(Process_Cloud_Control, 100, PRIORITY_HIGH, "CloudControl");
    
    // 监控任务（可选）
    monitorTaskID = Task_Add(Test_ESP8266, 10000, PRIORITY_NORMAL, "ESP8266Monitor");
}

// 主循环
int main(void) {
    // 初始化...
    
    while(1) {
        // 其他代码...
        
        Task_RunScheduler();  // 统一由任务调度器管理
    }
}
```

## 🎉 总结

- `Test_ESP8266()` 是一个可选的调试工具
- 完全解耦，不影响业务逻辑
- 可以随时启用或禁用
- 推荐在开发阶段使用，生产环境可以禁用
