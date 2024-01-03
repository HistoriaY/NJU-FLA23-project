import subprocess
import random
import re

def check_string_pattern(string):
    pattern = r'^a+b+$'
    if re.match(pattern, string):
        return True
    else:
        return False

def count_a_and_b(string):
    count_a = 0
    count_b = 0

    for char in string:
        if char == 'a':
            count_a += 1
        elif char == 'b':
            count_b += 1

    return count_a, count_b

# 调用可执行文件并提供命令行参数
executable = "../bin/turing"  # 可执行文件的路径

for i in range(500):
    length = random.randint(1, 100)  # 随机生成字符串的长度
    characters = ['a', 'b']  # 两种字符
    # 生成随机字符串
    random_string = ''.join(random.choices(characters, k=length))
    
    args = ["../programs/case1.tm", random_string ]  # 命令行参数列表

    # a^ib^j
    satisfy = check_string_pattern(random_string)

    # 执行命令并捕获输出
    output = subprocess.check_output([executable] + args)

    output = output.decode().strip()

    # correct
    if(satisfy):
        i,j = count_a_and_b(random_string)
        str_c = 'c'*(i*j)
        correct = (output=="(ACCEPTED) "+str_c)
    else:
        correct = (output=="(UNACCEPTED) Illegal_Input")

    if(correct):
        print("AC")
    else:
        # 输出结果
        print(random_string)
        print(satisfy)
        print(output)
        break