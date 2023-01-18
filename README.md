# Misaka10072's QQBot

 一个基于Mirai-CPP的QQ机器人

## 使用方法

```bash
git clone https://github.com/sjzyQwQ/Misaka10072s_QQBot.git
cd Misaka10072s_QQBot
# 更新 mirai-cpp 到最新
git submodule update --init --force --remote
```

然后使用 Visual Studio 或 Visual Studio Code 直接打开整个文件夹即可。

## 配置文件

位于 C:\ProgramData\Misaka10072s_QQBot\bot.conf

注意：配置中项目顺序不能调换

## 支持的功能

1.群聊消息互通

```
转发规则位于C:\ProgramData\Misaka10072s_QQBot\forward.conf
请按照 源QQ群号->目标QQ群号 格式编写
```

2.暂时没有