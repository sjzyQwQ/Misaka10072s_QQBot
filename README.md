# Misaka10072's QQBot

 一个基于Mirai-CPP的QQ机器人

## 使用方法

```bash
git clone https://github.com/sjzyQwQ/Misaka10072s_QQBot.git
cd Misaka10072s_QQBot
# 更新 mirai-cpp 到最新
git submodule update --init --force --remote
```

## 配置文件

位于 C:\ProgramData\Misaka10072s_QQBot\bot.conf

注意：配置中项目顺序不能调换

## 支持的功能

1. 群聊消息互通

```
转发规则位于C:\ProgramData\Misaka10072s_QQBot\forward.conf
请按照 源QQ群号->目标QQ群号 格式编写
```

2. 一言 (hitokoto.cn)

```
私信Bot或@Bot发送 一言 即可触发
```

3. 暂时没有