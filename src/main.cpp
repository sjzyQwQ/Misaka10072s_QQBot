#define _HAS_STD_BYTE 0

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <mirai.h>
#include "messages.hpp"
#include "hitokoto.hpp"

using namespace std;
using namespace Cyan;
using namespace mis1072;
#undef SendMessage

int main(int argc, char *argv[])
{
#if defined(WIN32) || defined(_WIN32)
	system("chcp 65001");
#endif
	system("cls");
	ifstream ifs;
	MiraiBot bot;
	SessionOptions opts;
	ifs.open("C:\\ProgramData\\Misaka10072s_QQBot\\bot.conf");
	if (!ifs.good())
	{
		ofstream ofs;
		cout << "配置文件不存在！" << endl;
		ofs.open("C:\\ProgramData\\Misaka10072s_QQBot\\bot.conf");
		if (!ofs.good())
		{
			system("mkdir C:\\ProgramData\\Misaka10072s_QQBot");
			ofs.open("C:\\ProgramData\\Misaka10072s_QQBot\\bot.conf");
		}
		ofs << "# 与 mirai-api-http 通信的端口(Http 适配器)，默认值为 8080" << endl << "HttpPort=8080" << endl
		    << "# 与 mirai-api-http 通信的端口(WebSocket 适配器)，默认值为 8080" << endl << "WebSocketPort=8080" << endl
		    << "# mirai-api-http 历史消息缓存容量, 过小可能导致撤回消息失败, 默认值为 4096" << endl << "CacheSize=4096" << endl
		    << "# mirai-cpp 线程池的容量, 建议设置为 CPU 核心数 * 3" << endl << "ThreadPoolSize=6" << endl
		    << "# 机器人的QQ号" << endl << "BotQQ=" << endl
		    << "# Hostname (Http 适配器), 默认值为 localhost" << endl << "HttpHostname=localhost" << endl
		    << "# Hostname (WebScoket 适配器), 默认值为 localhost" << endl << "WebSocketHostname=localhost" << endl
		    << "# 认证流程需要的密钥" << endl << "VerifyKey=" << endl
		    << "# 迎新词（当Welcom=false时迎新词功能关闭）" << endl << "Welcome=false" << endl
		    << "# 成员离开提醒（不含管理员移出，true/false）" << endl << "Leave=false" << endl;

		ofs.close();
		cout << "配置文件已生成在C:\\ProgramData\\Misaka10072s_QQBot\\bot.conf，请编辑后重启Bot" << endl;
		return 1;
	}
	vector<string> conf;
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
	ifs.open("C:\\ProgramData\\Misaka10072s_QQBot\\forward.conf");
	if (!ifs.good())
	{
		ofstream ofs;
		cout << "转发规则不存在！" << endl;
		ofs.open("C:\\ProgramData\\Misaka10072s_QQBot\\forward.conf");
		ofs << endl;
		ofs.close();
		cout << "转发规则已生成在C:\\ProgramData\\Misaka10072s_QQBot\\forward.conf，可根据需要编辑后重启Bot（格式：源QQ群号->目标QQ群号）" << endl;
	}
	map<GID_t, vector<GID_t>> fwd;
	while (ifs >> temp)
	{
		for (auto i = temp.begin(); i != temp.end(); ++i)
		{
			if (*i == '-')
			{
				fwd[GID_t(stoll(string(temp.begin(), i)))].push_back(GID_t(stoll(string(i + 2, temp.end()))));
			}
		}
	}
	ifs.close();
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
		catch (const exception &ex)
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
	cout << "连接成功！" << endl
	     << "Tip：关闭本程序请使用 stop 命令，否则会导致Mirai HTTP API内存泄漏" << endl;
	map<GID_t, map<MessageId_t, vector<int64_t>>> fwd_id;
	string help = "Misaka10072's QQBot\nVer.23.1.20.dev\n项目地址：https://github.com/sjzyQwQ/Misaka10072s_QQBot\nMirai：https://github.com/mamoe/mirai\nMirai-CPP：https://github.com/cyanray/mirai-cpp";
	bot.On<FriendMessage>([&](FriendMessage m)
	                      {
		                      ReceiveMessage(bot, m, fwd_id);
		                      cout << endl << "\tMessageId: " << m.MessageId() << endl;
		                      if (strstr(m.MessageChain.GetPlainText().c_str(), "帮助"))bot.SendMessage(m.Sender.QQ, MessageChain().Plain(help));
		                      else if (strstr(m.MessageChain.GetPlainText().c_str(), "一言"))request_hitokoto(bot, m);
	                      });
	bot.On<GroupMessage>([&](GroupMessage m)
	                     {
		                     ReceiveMessage(bot, m, fwd_id);
		                     cout << endl << "\tMessageId: " << m.MessageId() << endl;
		                     if (m.AtMe())
		                     {
			                     if (strstr(m.MessageChain.GetPlainText().c_str(), "帮助"))bot.SendMessage(m.Sender.Group.GID, MessageChain().Plain(help), m.MessageId());
			                     else if (strstr(m.MessageChain.GetPlainText().c_str(), "一言"))request_hitokoto(bot, m);
		                     }
		                     mis1072::ForwardMessage(bot, fwd, fwd_id, m);
	                     });
	bot.On<TempMessage>([&](TempMessage m)
	                    {
		                    cout << endl << "\t" << "MessageId: " << m.MessageId() << " 该用户通过 " << m.Sender.Group.Name << "(" << m.Sender.Group.GID.ToInt64() << ") 向您的Bot发起临时会话" << endl;
		                    if (strstr(m.MessageChain.GetPlainText().c_str(), "帮助"))bot.SendMessage(m.Sender.Group.GID, m.Sender.QQ, MessageChain().Plain(help));
		                    else if (strstr(m.MessageChain.GetPlainText().c_str(), "一言"))request_hitokoto(bot, m);
	                    });
	bot.On<GroupRecallEvent>([&](GroupRecallEvent e)
	                         {
		                         cout << "[" << e.Group.Name << "(" << e.Group.GID.ToInt64() << ")]" << string(e.ToJson().at("operator").at("memberName")) << "(" << e.AuthorQQ << ") 撤回了一条消息：" << bot.GetGroupMessageFromId(e.MessageId, e.Group.GID).MessageChain.GetPlainText() << endl;
		                         for (int i = 0; i < fwd_id[e.Group.GID][e.MessageId].size() / 2; ++i)
		                         {
			                         bot.Recall(MessageId_t(fwd_id[e.Group.GID][e.MessageId][i * 2]), GID_t(fwd_id[e.Group.GID][e.MessageId][i * 2 + 1]));
			                         fwd_id[e.Group.GID][e.MessageId].clear();
		                         }
	                         });
	bot.On<BotMuteEvent>([&](BotMuteEvent e)
	                     {
		                     cout << "[被禁言]您的Bot在 " << e.Operator.Group.Name << "(" << e.Operator.Group.GID.ToInt64() << ") 被 " << e.Operator.MemberName << "(" << e.Operator.QQ.ToInt64() << ") 禁言";
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
		                       cout << "[被解除禁言]您的Bot在 " << e.Operator.Group.Name << "(" << e.Operator.Group.GID.ToInt64() << ") 被 " << e.Operator.MemberName << "(" << e.Operator.QQ.ToInt64() << ") 解除禁言" << endl;
	                       });
	bot.On<GroupNameChangeEvent>([&](GroupNameChangeEvent e)
	                             {
		                             cout << "[群名称改变]群聊 " << e.OriginName << "(" << e.Group.GID.ToInt64() << ") 更名为 " << e.CurrentName << endl;
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
	bot.On<MemberJoinEvent>([&](MemberJoinEvent e)
	                        {
		                        cout << "[新人入群]" << e.NewMember.MemberName << "(" << e.NewMember.QQ.ToInt64() << ") 加入了 " << e.NewMember.Group.Name << "(" << e.NewMember.Group.GID.ToInt64() << ")" << endl;
		                        if (conf[8] != "false")
		                        {
			                        bot.SendMessage(e.NewMember.Group.GID, MessageChain().At(e.NewMember.QQ).Plain(" " + conf[8]));
		                        }
	                        });
	bot.On<MemberLeaveEventQuit>([&](MemberLeaveEventQuit e)
	                             {
		                             cout << "[成员离开]" << e.Member.Group.Name << "(" << e.Member.Group.GID.ToInt64() << ") 的 " << e.Member.MemberName << "(" << e.Member.QQ.ToInt64() << ") 离开了" << endl;
		                             if (conf[9] == "true")
		                             {
			                             bot.SendMessage(e.Member.Group.GID, MessageChain().Plain(e.Member.MemberName + " 离开了我们"));
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
			cout << help << endl;
		}
		else if (s == "bot")
		{
			cout << "Bot登录信息：" << bot.GetBotProfile().NickName << "(" << bot.GetBotQQ().ToInt64() << ")" << endl;
		}
		else if (s == "forward")
		{
			vector<Group_t> temp = bot.GetGroupList();
			for (int i = 0; i < temp.size(); ++i)
			{
				for (int j = 0; j < fwd[temp[i].GID].size(); ++i)
				{
					cout << temp[i].GID.ToInt64() << "->" << fwd[temp[i].GID][j].ToInt64() << endl;
				}
			}
		}
		else if (s == "friend")
		{
			vector<Friend_t> temp = bot.GetFriendList();
			for (int i = 0; i < temp.size(); ++i)
			{
				cout << temp[i].NickName << "(" << temp[i].QQ.ToInt64() << ")" << endl;
			}
		}
		else if (s == "group")
		{
			vector<Group_t> temp = bot.GetGroupList();
			for (int i = 0; i < temp.size(); ++i)
			{
				cout << temp[i].Name << "(" << temp[i].GID.ToInt64() << ")" << endl;
			}
		}
		else if (s == "help")
		{
			cout << "about\t输出关于信息" << endl
			     << "bot\t输出Bot登录信息" << endl
			     << "forward\t输出转发规则" << endl
			     << "friend\t输出Bot好友列表" << endl
			     << "group\t输出Bot群组列表" << endl
			     << "help\t输出本信息" << endl
			     << "recall\t撤回一条消息" << endl
			     << "stop\t断开连接并关闭程序" << endl;
		}
		else if (s == "recall")
		{
			cout << "请按照 QQ(群)号 消息ID qq/gid 的格式输入（qq表示QQ号，gid表示QQ群号）" << endl;
			int64_t temp[2];
			string temp_s;
			cin >> temp[0] >> temp[1] >> temp_s;
			if (temp_s == "qq")
			{
				bot.Recall(MessageId_t(temp[1]), QQ_t(temp[0]));
			}
			else if (temp_s == "gid")
			{
				bot.Recall(MessageId_t(temp[1]), GID_t(temp[0]));
			}
			else
			{
				cout << "号码类型不明确" << endl;
			}
		}
		else if (s == "stop")
		{
			break;
		}
		else
		{
			cout << "未知的命令" << endl;
		}
	}
	bot.Disconnect();
	return 0;
}