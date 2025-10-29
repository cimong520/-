#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
修复menu.c中的乱码注释
"""

import re

# 乱码到正确中文的映射
replacements = {
    # 常见乱码模式
    '��ʼ����ʾ': '初始化显示',
    '��ʼ����': '初始高亮',
    '��������': '按键处理',
    'ֻ��ѡ��ı�ʱ������ʾ': '只在选项改变时更新显示',
    '��ֹ������������': '防止按键抖动',
    '�˵�ѡ������': '菜单选项数组',
    'ѭ�������һ��': '循环到最后一项',
    'ѭ������һ��': '循环到第一项',
    'ȷ��ѡ��': '确认选择',
    '������ʾƫ����': '计算显示偏移量',
    'ѡ����ǰ4�����Ҫ����': '选项在前4项，不需要滚动',
    'ѡ�񳬹�4���Ҫ����': '选项超过4项，需要滚动',
    'ֻ��ѡ��ı��ƫ�Ƹı�ʱ������ʾ': '只在选项改变或偏移改变时更新显示',
    '��ʾ��ͷ': '显示箭头',
    '��ʾ�˵���': '显示菜单项',
    '�����ͷλ��': '反转箭头位置',
    'ǿ��ˢ����ʾ': '强制刷新显示',
    '����ѡ��': '重置选项',
    '�¶���ֵ����': '温度阈值设置',
    'ʪ����ֵ����': '湿度阈值设置',
    '�ֶ�ģʽ': '手动模式',
    '�Զ�����': '自动控制',
    '�˳��˵�': '退出菜单',
    '�¶��������': '温度下限设置',
    '�¶��������': '温度上限设置',
    'ʪ���������': '湿度下限设置',
    'ʪ���������': '湿度上限设置',
    'Ѫ����ֵ���': '血氧阈值下限',
    'Ѫ����ֵ���': '血氧阈值上限',
    '��ǰѡ���ѡ��': '当前选中选项',
    'ȷ��ѡ���ѡ��': '确认选中选项',
    '��ʾƫ����': '显示偏移量',
    '�ϴε�flagֵ': '上次的flag值',
    '�ϴε�ƫ����': '上次的偏移量',
    '��ʼ��Ϊ������ֵ': '初始化为无效值',
    '��ѡ����': '总选项数',
    '�ɼ�ѡ����': '可见选项数',
    '����': '上移',
    '����': '下移',
}

def fix_file(filename):
    """修复文件中的乱码"""
    try:
        # 尝试用不同编码读取
        encodings = ['utf-8', 'gbk', 'gb2312', 'gb18030']
        content = None
        
        for encoding in encodings:
            try:
                with open(filename, 'r', encoding=encoding) as f:
                    content = f.read()
                print(f"成功使用 {encoding} 编码读取文件")
                break
            except:
                continue
        
        if content is None:
            print("无法读取文件")
            return False
        
        # 替换所有乱码
        modified = False
        for old, new in replacements.items():
            if old in content:
                content = content.replace(old, new)
                modified = True
                print(f"替换: {old} -> {new}")
        
        if modified:
            # 保存为UTF-8编码
            with open(filename, 'w', encoding='utf-8') as f:
                f.write(content)
            print(f"\n文件已修复并保存为UTF-8编码")
            return True
        else:
            print("未发现需要替换的乱码")
            return False
            
    except Exception as e:
        print(f"错误: {e}")
        return False

if __name__ == '__main__':
    fix_file('User/menu.c')
