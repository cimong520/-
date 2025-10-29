@echo off
chcp 65001 >nul
echo ========================================
echo   STM32项目冗余文件清理工具
echo ========================================
echo.
echo 本脚本将删除以下冗余内容：
echo   1. 独立测试文件夹（土壤湿度传感器、继电器等）
echo   2. 多余的Keil配置文件
echo   3. 编译输出文件（Objects、Listings）
echo.
echo ⚠️  警告：此操作不可恢复！
echo.
pause

echo.
echo [1/4] 删除独立测试文件夹...
if exist "土壤湿度传感器" (
    rmdir /s /q "土壤湿度传感器"
    echo   ✓ 已删除：土壤湿度传感器
)
if exist "继电器" (
    rmdir /s /q "继电器"
    echo   ✓ 已删除：继电器
)
if exist "蜂鸣器" (
    rmdir /s /q "蜂鸣器"
    echo   ✓ 已删除：蜂鸣器
)
if exist "风速传感器" (
    rmdir /s /q "风速传感器"
    echo   ✓ 已删除：风速传感器
)
if exist "BH1750光照" (
    rmdir /s /q "BH1750光照"
    echo   ✓ 已删除：BH1750光照
)
if exist "dht11温湿度" (
    rmdir /s /q "dht11温湿度"
    echo   ✓ 已删除：dht11温湿度
)

echo.
echo [2/4] 清理编译输出文件...
if exist "Objects" (
    rmdir /s /q "Objects"
    echo   ✓ 已删除：Objects文件夹
)
if exist "Listings" (
    rmdir /s /q "Listings"
    echo   ✓ 已删除：Listings文件夹
)

echo.
echo [3/4] 删除多余的Keil配置文件...
if exist "project.uvguix.35366" (
    del /q "project.uvguix.35366"
    echo   ✓ 已删除：project.uvguix.35366
)
if exist "project.uvguix.A11" (
    del /q "project.uvguix.A11"
    echo   ✓ 已删除：project.uvguix.A11
)
if exist "project.uvguix.PC" (
    del /q "project.uvguix.PC"
    echo   ✓ 已删除：project.uvguix.PC
)
if exist "project.uvguix.song" (
    del /q "project.uvguix.song"
    echo   ✓ 已删除：project.uvguix.song
)

echo.
echo [4/4] 整理文档文件...
if not exist "Docs" mkdir Docs
if exist "任务调度器解耦原理.md" move "任务调度器解耦原理.md" "Docs\" >nul
if exist "云端控制架构说明.md" move "云端控制架构说明.md" "Docs\" >nul
if exist "Test_ESP8266使用说明.md" move "Test_ESP8266使用说明.md" "Docs\" >nul
if exist "引脚冲突修复报告.md" move "引脚冲突修复报告.md" "Docs\" >nul
if exist "土壤传感器引脚修改完成报告.md" move "土壤传感器引脚修改完成报告.md" "Docs\" >nul
if exist "硬件接线图.md" move "硬件接线图.md" "Docs\" >nul
if exist "项目冗余清理方案.md" move "项目冗余清理方案.md" "Docs\" >nul
echo   ✓ 文档已移动到Docs文件夹

echo.
echo ========================================
echo   清理完成！
echo ========================================
echo.
echo 清理结果：
echo   ✓ 删除了6个测试文件夹
echo   ✓ 删除了编译输出文件
echo   ✓ 删除了多余的配置文件
echo   ✓ 整理了文档文件
echo.
echo 项目现在更加精简，可以正常编译运行！
echo.
pause
