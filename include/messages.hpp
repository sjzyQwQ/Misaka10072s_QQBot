#pragma once

#include<iostream>
#include<algorithm>
#include<mirai.h>

using namespace std;
using namespace Cyan;

namespace mis1072
{
	void ReceiveMessage(MiraiBot &bot, FriendMessage &m, const map<GID_t, map<MessageId_t, vector<int64_t>>> &fwd_id, bool is_Quote = false)
	{
		if (!is_Quote)
		{
			cout << "[好友]" << m.Sender.NickName << "(" << m.Sender.QQ.ToInt64() << ")：";
		}
		for (int i = 0; i < m.MessageChain.ToJson().size(); ++i)
		{
			if (m.MessageChain.ToJson()[i]["type"] == "Face")
			{
				cout << "[" << string(m.MessageChain.ToJson()[i]["name"]) << "]";
			}
			else if (m.MessageChain.ToJson()[i]["type"] == "Image")
			{
				cout << "[图片]" << string(m.MessageChain.ToJson()[i]["url"]) << " ";
			}
			else if (m.MessageChain.ToJson()[i]["type"] == "Plain")
			{
				cout << string(m.MessageChain.ToJson()[i]["text"]);
			}
			else if (m.MessageChain.ToJson()[i]["type"] == "Quote")
			{
				if (!is_Quote)
				{
					FriendMessage Quote_m = bot.GetFriendMessageFromId(MessageId_t(m.MessageChain.ToJson()[i]["id"]), m.Sender.QQ);
					cout << "回复 " << Quote_m.Sender.NickName << " ";
					ReceiveMessage(bot, Quote_m, fwd_id, true);
				}
			}
			else
			{
				cout << m.MessageChain.ToJson()[i];
			}
		}
	}

	void ReceiveMessage(MiraiBot &bot, GroupMessage &m, const map<GID_t, map<MessageId_t, vector<int64_t>>> &fwd_id, bool is_Quote = false)
	{
		if (!is_Quote)
		{
			cout << "[" << m.Sender.Group.Name << "(" << m.Sender.Group.GID.ToInt64() << ")]" << m.Sender.MemberName << "(" << m.Sender.QQ.ToInt64() << ")：";
		}
		for (int i = 0; i < m.MessageChain.ToJson().size(); ++i)
		{
			if (m.MessageChain.ToJson()[i]["type"] == "At")
			{
				GroupMember gm = bot.GetGroupMemberInfo(m.Sender.Group.GID, QQ_t(m.MessageChain.ToJson()[i]["target"]));
				cout << "@" << gm.MemberName;
			}
			else if (m.MessageChain.ToJson()[i]["type"] == "Face")
			{
				cout << "[" << string(m.MessageChain.ToJson()[i]["name"]) << "]";
			}
			else if (m.MessageChain.ToJson()[i]["type"] == "Image")
			{
				cout << "[图片]" << string(m.MessageChain.ToJson()[i]["url"]) << " ";
			}
			else if (m.MessageChain.ToJson()[i]["type"] == "Plain")
			{
				cout << string(m.MessageChain.ToJson()[i]["text"]);
			}
			else if (m.MessageChain.ToJson()[i]["type"] == "Quote")
			{
				if (!is_Quote)
				{
					GroupMessage Quote_m = bot.GetGroupMessageFromId(MessageId_t(m.MessageChain.ToJson()[i]["id"]), m.Sender.Group.GID);
					cout << "回复 " << Quote_m.Sender.MemberName << " ";
					ReceiveMessage(bot, Quote_m, fwd_id, true);
					cout << "：";
				}
			}
			else
			{
				cout << m.MessageChain.ToJson()[i];
			}
		}
	}

	void ReceiveMessage(MiraiBot &bot, TempMessage &m, const map<GID_t, map<MessageId_t, vector<int64_t>>> &fwd_id)
	{
		for (int i = 0; i < m.MessageChain.ToJson().size(); ++i)
		{
			if (m.MessageChain.ToJson()[i]["type"] == "Face")
			{
				cout << "[" << string(m.MessageChain.ToJson()[i]["name"]) << "]";
			}
			else if (m.MessageChain.ToJson()[i]["type"] == "Image")
			{
				cout << "[图片]" << string(m.MessageChain.ToJson()[i]["url"]) << " ";
			}
			else if (m.MessageChain.ToJson()[i]["type"] == "Plain")
			{
				cout << string(m.MessageChain.ToJson()[i]["text"]);
			}
			else
			{
				cout << m.MessageChain.ToJson()[i];
			}
		}
	}

	void ForwardMessage(MiraiBot &bot, map<GID_t, vector<GID_t>> &fwd, map<GID_t, map<MessageId_t, vector<int64_t>>> &fwd_id, GroupMessage &m)
	{
		if (fwd[m.Sender.Group.GID].size())
		{
			for (int i = 0; i < fwd[m.Sender.Group.GID].size(); ++i)
			{
				MessageChain c;
				c.Plain("[消息互通-" + m.Sender.Group.Name + "]\n" + m.Sender.MemberName + "：");
				if (m.MessageChain.ToJson()[0]["type"] == "Quote")
				{
					GroupMessage temp = bot.GetGroupMessageFromId(m.MessageChain.ToJson()[0]["id"], m.Sender.Group.GID);
					c.Plain("回复 ").Plain(temp.Sender.MemberName).Plain(" ");
					if (temp.MessageChain.ToJson()[0]["type"] == "Quote")
					{
						for (int i = 1; i < temp.MessageChain.ToJson().size(); ++i)
						{
							if (temp.MessageChain.ToJson()[i]["type"] == "At")
							{
								GroupMember gm = bot.GetGroupMemberInfo(m.Sender.Group.GID, QQ_t(temp.MessageChain.ToJson()[i]["target"]));
								c.Plain("@").Plain(gm.MemberName);
							}
							else if (temp.MessageChain.ToJson()[i]["type"] == "Face")
							{
								c.Face(int(temp.MessageChain.ToJson()[i]["faceId"]));
							}
							else if (temp.MessageChain.ToJson()[i]["type"] == "Image")
							{
								GroupImage img;
								img.Id = temp.MessageChain.ToJson()[i]["imageId"];
								c.Image(img);
							}
							else if (temp.MessageChain.ToJson()[i]["type"] == "Plain")
							{
								c.Plain(string(temp.MessageChain.ToJson()[i]["text"]));
							}
						}
					}
					else
					{
						c = c + temp.MessageChain;
					}
					c.Plain("：");
					for (int i = 1; i < m.MessageChain.ToJson().size(); ++i)
					{
						if (m.MessageChain.ToJson()[i]["type"] == "At")
						{
							GroupMember gm = bot.GetGroupMemberInfo(m.Sender.Group.GID, QQ_t(m.MessageChain.ToJson()[i]["target"]));
							c.Plain("@").Plain(gm.MemberName);
						}
						else if (m.MessageChain.ToJson()[i]["type"] == "Face")
						{
							c.Face(int(m.MessageChain.ToJson()[i]["faceId"]));
						}
						else if (m.MessageChain.ToJson()[i]["type"] == "Image")
						{
							GroupImage img;
							img.Id = m.MessageChain.ToJson()[i]["imageId"];
							c.Image(img);
						}
						else if (m.MessageChain.ToJson()[i]["type"] == "Plain")
						{
							c.Plain(string(m.MessageChain.ToJson()[i]["text"]));
						}
					}
				}
				else
				{
					c = c + m.MessageChain;
				}
				fwd_id[m.Sender.Group.GID][m.MessageId()].push_back(int64_t(bot.SendMessage(fwd[m.Sender.Group.GID][i], c)));
				fwd_id[m.Sender.Group.GID][m.MessageId()].push_back(fwd[m.Sender.Group.GID][i].ToInt64());
				cout << "\tForwardInfo: GID(" << fwd_id[m.Sender.Group.GID][m.MessageId()][i * 2 + 1] << ") MessageId(" << fwd_id[m.Sender.Group.GID][m.MessageId()][i * 2] << ")" << endl;
			}
		}
	}

	void SendMessage(MiraiBot &bot)
	{
		string selection;
		cout << "1.发送好友消息" << endl
		     << "2.发送群组消息" << endl
		     << "3.发送临时消息" << endl
		     << "0.取消" << endl;
		cin >> selection;
		if (selection == "1")
		{
			cout << "正在获取好友列表..." << endl;
			vector<Friend_t> temp = bot.GetFriendList();
			for (int i = 0; i < temp.size(); ++i)cout << temp[i].NickName << "(" << temp[i].QQ.ToInt64() << ")" << endl;
			cout << "请输入欲发送的好友QQ号：";
			long long QQ;
			cin >> QQ;
			for (int i = 0; i < temp.size(); ++i)
			{
				if (QQ == temp[i].QQ.ToInt64())break;
				if (i = temp.size() - 1)
				{
					cout << "输入的QQ号好像不正确，再检查一下？" << endl
					     << "已取消发送" << endl;
					return;
				}
			}
			MessageChain c;
			bool is_Quote = false;
			MessageId_t mid;
			while (true)
			{
				if (!is_Quote)cout << "1.回复" << endl;
				else cout << "1.取消回复" << endl;
				cout << "2.表情" << endl
				     << "3.图片" << endl
				     << "4.文字" << endl
				     << "5.换行" << endl
				     << "9.发送" << endl
				     << "0.取消" << endl;
				cin >> selection;
				if (selection == "1")
				{
					if (!is_Quote)
					{
						cout << "请输入欲回复的MessageId" << endl;
						cin >> mid;
					}
					else cout << "已取消回复" << endl;
					is_Quote = !is_Quote;
				}
				else if (selection == "2")
				{
					cout << "请输入表情faceID：";
					int face;
					cin >> face;
					c.Face(face);
				}
				else if (selection == "3")
				{
					cout << "请输入图片URL：";
					MiraiImage img;
					cin >> img.Url;
					c.Image(img);
				}
				else if (selection == "4")
				{
					cout << "请输入文字：";
					cin >> selection;
					c.Plain(selection);
				}
				else if (selection == "5")c.Plain("\n");
				else if (selection == "9")break;
				else if (selection == "0")return;
				else cout << selection << "是什么意思呀？" << endl;
			}
			try
			{
				if (is_Quote)cout << "消息已发送！MessageId: " << bot.SendMessage(QQ_t(QQ), c, mid) << endl;
				else cout << "消息已发送！MessageId: " << bot.SendMessage(QQ_t(QQ), c) << endl;
			}
			catch (const exception &ex)
			{
				cout << ex.what() << endl;
			}
		}
		else if (selection == "2")
		{
			cout << "正在获取群聊列表..." << endl;
			vector<Group_t> temp = bot.GetGroupList();
			for (int i = 0; i < temp.size(); ++i)cout << temp[i].Name << "(" << temp[i].GID.ToInt64() << ")" << endl;
			cout << "请输入欲发送的群号：";
			long long GID;
			cin >> GID;
			for (int i = 0; i < temp.size(); ++i)
			{
				if (GID == temp[i].GID.ToInt64()) break;
				if (i == temp.size() - 1)
				{
					cout << "输入的群号好像不正确，再检查一下？" << endl
					     << "已取消发送" << endl;
					return;
				}
			}
			MessageChain c;
			bool is_Quote = false;
			MessageId_t mid;
			while (true)
			{
				if (!is_Quote)cout << "1.回复" << endl;
				else cout << "1.取消回复" << endl;
				cout << "2.At" << endl
				     << "3.表情" << endl
				     << "4.图片" << endl
				     << "5.文字" << endl
				     << "6.换行" << endl
				     << "9.发送" << endl
				     << "0.取消" << endl;
				cin >> selection;
				if (selection == "1")
				{
					if (!is_Quote)
					{
						cout << "请输入欲回复的MessageId" << endl;
						cin >> mid;
					}
					else cout << "已取消回复" << endl;
					is_Quote = !is_Quote;
				}
				else if (selection == "2")
				{
					cout << "正在获取成员列表..." << endl;
					vector<GroupMember> GM = bot.GetGroupMembers(GID_t(GID));
					for (int i = 0; i < GM.size(); ++i)cout << GM[i].MemberName << "(" << GM[i].QQ.ToInt64() << ")" << endl;
					cout << "请输入要@的QQ号：";
					long long QQ;
					cin >> QQ;
					for (int i = 0; i < GM.size(); ++i)
					{
						if (QQ == GM[i].QQ.ToInt64())
						{
							c.At(QQ_t(QQ));
							break;
						}
						if (i == GM.size() - 1)
						{
							cout << "输入的QQ号好像不正确，再检查一下？" << endl;
						}
					}
				}
				else if (selection == "3")
				{
					cout << "请输入要表情faceID：";
					int face;
					cin >> face;
					c.Face(face);
				}
				else if (selection == "4")
				{
					cout << "请输入图片URL：";
					MiraiImage img;
					cin >> img.Url;
					c.Image(img);
				}
				else if (selection == "5")
				{
					cout << "请输入文字：";
					cin >> selection;
					c.Plain(selection);
				}
				else if (selection == "6")c.Plain("\n");
				else if (selection == "9")break;
				else if (selection == "0")return;
				else cout << selection << "是什么意思呀？" << endl;
			}
			try
			{
				if (is_Quote)cout << "消息已发送！MessageId: " << bot.SendMessage(GID_t(GID), c, mid) << endl;
				else cout << "消息已发送！MessageId: " << bot.SendMessage(GID_t(GID), c) << endl;
			}
			catch (const exception &ex)
			{
				cout << ex.what() << endl;
			}
		}
		else if (selection == "3")
		{
			cout << "正在获取群聊列表..." << endl;
			vector<Group_t> temp_g = bot.GetGroupList();
			for (int i = 0; i < temp_g.size(); ++i)cout << temp_g[i].Name << "(" << temp_g[i].GID.ToInt64() << ")" << endl;
			cout << "请输入欲发送的QQ所在群号：";
			long long GID;
			cin >> GID;
			for (int i = 0; i < temp_g.size(); ++i)
			{
				if (GID == temp_g[i].GID.ToInt64()) break;
				if (i == temp_g.size() - 1)
				{
					cout << "输入的群号好像不正确，再检查一下？" << endl
					     << "已取消发送" << endl;
					return;
				}
			}
			cout << "正在获取群员列表..." << endl;
			vector<GroupMember> temp_m = bot.GetGroupMembers(GID_t(GID));
			for (int i = 0; i < temp_m.size(); ++i)cout << temp_m[i].MemberName << "(" << temp_m[i].QQ.ToInt64() << ")" << endl;
			cout << "请输入欲发送的QQ号：";
			long long QQ;
			cin >> QQ;
			for (int i = 0; i < temp_m.size(); ++i)
			{
				if (QQ == temp_m[i].QQ.ToInt64())break;
				if (i == temp_m.size() - 1)
				{
					cout << "输入的QQ号好像不正确，再检查一下？" << endl
					     << "已取消发送" << endl;
					return;
				}
			}
			MessageChain c;
			while (true)
			{
				cout << "1.表情" << endl
				     << "2.图片" << endl
				     << "3.文字" << endl
				     << "4.换行" << endl
				     << "9.发送" << endl
				     << "0.取消" << endl;
				cin >> selection;
				if (selection == "1")
				{
					cout << "请输入要表情faceID：";
					int face;
					cin >> face;
					c.Face(face);
				}
				else if (selection == "2")
				{
					cout << "请输入图片URL：";
					MiraiImage img;
					cin >> img.Url;
					c.Image(img);
				}
				else if (selection == "3")
				{
					cout << "请输入文字：";
					cin >> selection;
					c.Plain(selection);
				}
				else if (selection == "4")c.Plain("\n");
				else if (selection == "9")break;
				else if (selection == "0")return;
				else cout << selection << "是什么意思呀？" << endl;
			}
			try
			{
				cout << "消息已发送！MessageId: " << bot.SendMessage(GID_t(GID), QQ_t(QQ), c) << endl;
			}
			catch (const exception &ex)
			{
				cout << ex.what() << endl;
			}
		}
		else cout << selection << "是什么意思呀？" << endl;
	}
}
