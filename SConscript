# RT-Thread building script for bridge
from building import *

cwd = GetCurrentDir()

# 获取源文件
src = Glob('src/*.c')
src += Glob('util/*.c')

# 包含路径
CPPPATH = [cwd + '/inc', cwd + '/util']

# 检查是否定义了 USING_IKUNLED_DEMO 宏
# 使用 GetDepend 而不是 GetDefine
if GetDepend('USING_IKUNLED_DEMO'):
    src += Glob('example/*.c')
    # 添加示例代码的头文件路径
    CPPPATH += [cwd + '/example']

# 创建组件组
group = DefineGroup('ikunLed', src, depend=['PKG_USING_IKUNLED'], CPPPATH=CPPPATH)

Return('group')
