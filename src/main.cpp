#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<mirai.h>
using namespace std;
using namespace Cyan;

int main(int argc, char* argv[])
{
#if defined(WIN32) || defined(_WIN32)
	system("chcp 65001");
#endif
	system("cls");
	ifstream ifs;
	MiraiBot bot;
	SessionOptions opts;
	ifs.open("C:\\ProgramData\\Misaka10072s_QQBot\\config.conf");
	if (!ifs.good())
	{
		ofstream ofs;
		cout << "配置文件不存在！" << endl;
		ofs.open("C:\\ProgramData\\Misaka10072s_QQBot\\config.conf");
		if (!ofs.good())
		{
			system("mkdir C:\\ProgramData\\Misaka10072s_QQBot");
			ofs.open("C:\\ProgramData\\Misaka10072s_QQBot\\config.conf");
		}
		ofs << "# 与 mirai-api-http 通信的端口(Http 适配器)，默认值为 8080" << endl << "HttpPort=8080" << endl
			<< "# 与 mirai-api-http 通信的端口(WebSocket 适配器)，默认值为 8080" << endl << "WebSocketPort=8080" << endl
			<< "# mirai-api-http 历史消息缓存容量, 过小可能导致撤回消息失败, 默认值为 4096" << endl << "CacheSize=4096" << endl
			<< "# mirai-cpp 线程池的容量, 建议设置为 CPU 核心数 * 3" << endl << "ThreadPoolSize=6" << endl
			<< "# 机器人的QQ号" << endl << "BotQQ=" << endl
			<< "# Hostname (Http 适配器), 默认值为 localhost" << endl << "HttpHostname=localhost" << endl
			<< "# Hostname (WebScoket 适配器), 默认值为 localhost" << endl << "WebSocketHostname=localhost" << endl
			<< "# 认证流程需要的密钥" << endl << "VerifyKey=";
		ofs.close();
		cout << "配置文件已生成在C:\\ProgramData\\Misaka10072s_QQBot\\config.conf下，请编辑后重启Bot" << endl;
		return 1;
	}
	vector<string>conf;
	string temp;
	while (ifs >> temp)
	{
		for (auto i = temp.begin(); i != temp.end(); ++i)
		{
			if (*i == '=')
			{
				conf.push_back(string(i + 1, temp.end()));
			}
		}
	}
	ifs.close();
	opts.HttpPort.Set(stoi(conf[0]));
	opts.WebSocketPort.Set(stoi(conf[1]));
	opts.CacheSize.Set(stoi(conf[2]));
	opts.ThreadPoolSize.Set(stoi(conf[3]));
	opts.BotQQ.Set(QQ_t(stoll(conf[4])));
	opts.HttpHostname.Set(conf[5]);
	opts.WebSocketHostname.Set(conf[6]);
	opts.VerifyKey.Set(conf[7]);
	for (int i = 0; i < 4; ++i)
	{
		try
		{
			if (i == 0)
			{
				cout << "正在连接至Mirai HTTP API……" << endl;
			}
			else
			{
				cout << "正在进行第" << i << "次重连……" << endl;
			}
			bot.Connect(opts);
			break;
		}
		catch (const exception& ex)
		{
			cout << ex.what() << endl;
		}
		MiraiBot::SleepSeconds(1);
		if (i == 3)
		{
			cout << "连接失败！" << endl;
			return 1;
		}
	}
	cout << "连接成功！" << endl;
	ifs.open("C:\\ProgramData\\Misaka10072s_QQBot\interconnection.conf");
	ifs.close();
	bot.On<FriendMessage>([&](FriendMessage m)
		{
			cout << "[好友]" << m.Sender.NickName << "(" << m.Sender.QQ.ToInt64() << ")：" << m.MessageChain.GetPlainText() << endl;
			if (bool(strstr(m.MessageChain.GetPlainText().c_str(), "帮助")))
			{
				bot.SendMessage(m.Sender.QQ, MessageChain().Plain("Misaka10072's QQBot\n项目地址：https://github.com/sjzyQwQ/QQ-message-exchange-robot\nMirai：https://github.com/mamoe/mirai\nMirai-CPP：https://github.com/cyanray/mirai-cpp"));
			}
		});
	bot.On<GroupMessage>([&](GroupMessage m)
		{
			cout << "[" << m.Sender.Group.Name << "(" << m.Sender.Group.GID.ToInt64() << ")]" << m.Sender.MemberName << "(" << m.Sender.QQ.ToInt64() << ")：" << m.MessageChain.GetPlainText() << endl;
			if (m.AtMe())
			{
				if (bool(strstr(m.MessageChain.GetPlainText().c_str(), "帮助")))
				{
					bot.SendMessage(m.Sender.Group.GID, MessageChain().Plain("Misaka10072's QQBot\n项目地址：https://github.com/sjzyQwQ/QQ-message-exchange-robot\nMirai：https://github.com/mamoe/mirai\nMirai-CPP：https://github.com/cyanray/mirai-cpp"), m.MessageId());
				}
			}
		});
	bot.On<TempMessage>([&](TempMessage m)
		{
			cout << "[临时]" << m.Sender.MemberName << "(" << m.Sender.QQ.ToInt64() << ")：" << m.MessageChain.GetPlainText() << endl
				<< "\t该用户通过 " << m.Sender.Group.Name << "(" << m.Sender.Group.GID.ToInt64() << ") 向您的Bot发起临时会话" << endl;
		});
	bot.On<BotMuteEvent>([&](BotMuteEvent e)
		{
			cout << "[被禁言]您的Bot在 " << e.Operator.Group.Name << "(" << e.Operator.Group.GID.ToInt64() << ")" << " 被 " << e.Operator.MemberName << "(" << e.Operator.QQ.ToInt64() << ") 禁言";
			if (e.DurationSeconds < 3600)
			{
				cout << e.DurationSeconds / 60 << "分钟" << endl;
			}
			else if (e.DurationSeconds < 86400)
			{
				cout << e.DurationSeconds / 3600 << "小时" << e.DurationSeconds % 3600 / 60 << "分钟" << endl;
			}
			else
			{
				cout << e.DurationSeconds / 86400 << "天" << e.DurationSeconds % 86400 / 3600 << "小时" << e.DurationSeconds % 3600 / 60 << "分钟" << endl;
			}
		});
	bot.On<BotUnmuteEvent>([&](BotUnmuteEvent e)
		{
			cout << "[被解除禁言]您的Bot在 " << e.Operator.Group.Name << "(" << e.Operator.Group.GID.ToInt64() << ")" << " 被 " << e.Operator.MemberName << "(" << e.Operator.QQ.ToInt64() << ") 解除禁言" << endl;
		});
	bot.On<GroupNameChangeEvent>([&](GroupNameChangeEvent e)
		{
			cout << "[群名称改变]群聊 " << e.OriginName << "(" << e.Group.GID.ToInt64() <<")"<< " 更名为 " << e.CurrentName << endl;
		});
	bot.On<GroupMuteAllEvent>([&](GroupMuteAllEvent e)
		{
			cout << "[群全员禁言]";
			if (e.Current)
			{
				cout << e.Group.Name << "(" << e.Group.GID.ToInt64() << ") 开启了全员禁言" << endl;
			}
			else
			{
				cout << e.Group.Name << "(" << e.Group.GID.ToInt64() << ") 关闭了全员禁言" << endl;
			}
		});
	bot.On<NudgeEvent>([&](NudgeEvent e)
		{
			if (e.Target == bot.GetBotQQ())
			{
				bot.SendMessage(GID_t(e.RawSubjectId), MessageChain().Plain("戳什么戳"));
			}
		});
	while (true)
	{
		string s;
		cin >> s;
		if (s == "about")
		{
			cout << "Misaka10072's QQBot" << endl
				<< "项目地址：https://github.com/sjzyQwQ/Misaka10072s_QQBot" << endl
				<< "Mirai：https://github.com/mamoe/mirai" << endl
				<< "Mirai-CPP：https://github.com/cyanray/mirai-cpp" << endl;
		}
		else if (s == "stop")
		{
			break;
		}
	}
	bot.Disconnect();
	return 0;
}