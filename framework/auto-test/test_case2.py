import subprocess
import random

def split_string_at_c(string):
    # 从字符串中找到 "c" 的位置
    c_index = string.find('c')

    # 如果找不到 "c"，返回 None
    if c_index == -1:
        return None

    # 获取 "c" 的前缀子串和后缀子串
    prefix = string[:c_index]
    suffix = string[c_index+1:]

    return prefix, suffix

# 调用可执行文件并提供命令行参数
executable = "../bin/turing"  # 可执行文件的路径

for i in range(1000):
    length = random.randint(1, 100)  # 随机生成字符串的长度
    characters = ['a', 'b', 'c']  # 三种字符
    # 生成随机字符串
    random_string = ''.join(random.choices(characters, k=length))

    
    args = ["../programs/case2.tm", random_string ]  # 命令行参数列表

    stisfy = False
    if (random_string.find('c') == -1):
        pass
    else:
        prefix, suffix = split_string_at_c(random_string)
        satisfy = (prefix==suffix)


    # print("Prefix:", prefix)
    # print("Suffix:", suffix)

    # 执行命令并捕获输出
    output = subprocess.check_output([executable] + args)

    output = output.decode().strip()

    # correct
    if(satisfy):
        correct = (output=="(ACCEPTED) true")
    else:
        correct = (output=="(UNACCEPTED) false")

    if(correct):
        print("AC")
    else:
        # 输出结果
        print(random_string)
        print(satisfy)
        print(output)
        break