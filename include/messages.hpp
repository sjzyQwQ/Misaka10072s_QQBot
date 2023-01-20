#pragma once

#include<iostream>
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
			if (m.MessageChain.ToJson()[i].at("type") == "Face")
			{
				cout << "[" << string(m.MessageChain.ToJson()[i].at("name")) << "]";
			}
			else if (m.MessageChain.ToJson()[i].at("type") == "Image")
			{
				cout << "[图片]" << string(m.MessageChain.ToJson()[i].at("url")) << " ";
			}
			else if (m.MessageChain.ToJson()[i].at("type") == "Plain")
			{
				cout << string(m.MessageChain.ToJson()[i].at("text"));
			}
			else if (m.MessageChain.ToJson()[i].at("type") == "Quote")
			{
				if (!is_Quote)
				{
					FriendMessage Quote_m = bot.GetFriendMessageFromId(MessageId_t(m.MessageChain.ToJson()[i].at("id")), m.Sender.QQ);
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
			if (m.MessageChain.ToJson()[i].at("type") == "At")
			{
				GroupMember gm = bot.GetGroupMemberInfo(m.Sender.Group.GID, QQ_t(m.MessageChain.ToJson()[i].at("target")));
				cout << "@" << gm.MemberName;
			}
			else if (m.MessageChain.ToJson()[i].at("type") == "Face")
			{
				cout << "[" << string(m.MessageChain.ToJson()[i].at("name")) << "]";
			}
			else if (m.MessageChain.ToJson()[i].at("type") == "Image")
			{
				cout << "[图片]" << string(m.MessageChain.ToJson()[i].at("url")) << " ";
			}
			else if (m.MessageChain.ToJson()[i].at("type") == "Plain")
			{
				cout << string(m.MessageChain.ToJson()[i].at("text"));
			}
			else if (m.MessageChain.ToJson()[i].at("type") == "Quote")
			{
				if (!is_Quote)
				{
					GroupMessage Quote_m = bot.GetGroupMessageFromId(MessageId_t(m.MessageChain.ToJson()[i].at("id")), m.Sender.Group.GID);
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
			if (m.MessageChain.ToJson()[i].at("type") == "Face")
			{
				cout << "[" << string(m.MessageChain.ToJson()[i].at("name")) << "]";
			}
			else if (m.MessageChain.ToJson()[i].at("type") == "Image")
			{
				cout << "[图片]" << string(m.MessageChain.ToJson()[i].at("url")) << " ";
			}
			else if (m.MessageChain.ToJson()[i].at("type") == "Plain")
			{
				cout << string(m.MessageChain.ToJson()[i].at("text"));
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
				if (m.MessageChain.ToJson()[0].at("type") == "Quote")
				{
					GroupMessage temp = bot.GetGroupMessageFromId(m.MessageChain.ToJson()[0].at("id"), m.Sender.Group.GID);
					c.Plain("回复 ").Plain(temp.Sender.MemberName).Plain(" ");
					if (temp.MessageChain.ToJson()[0].at("type") == "Quote")
					{
						for (int i = 1; i < temp.MessageChain.ToJson().size(); ++i)
						{
							if (temp.MessageChain.ToJson()[i].at("type") == "At")
							{
								GroupMember gm = bot.GetGroupMemberInfo(m.Sender.Group.GID, QQ_t(temp.MessageChain.ToJson()[i].at("target")));
								c.Plain("@").Plain(gm.MemberName);
							}
							else if (temp.MessageChain.ToJson()[i].at("type") == "Face")
							{
								c.Face(int(temp.MessageChain.ToJson()[i].at("faceId")));
							}
							else if (temp.MessageChain.ToJson()[i].at("type") == "Image")
							{
								GroupImage img;
								img.Id = temp.MessageChain.ToJson()[i].at("imageId");
								c.Image(img);
							}
							else if (temp.MessageChain.ToJson()[i].at("type") == "Plain")
							{
								c.Plain(string(temp.MessageChain.ToJson()[i].at("text")));
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
						if (m.MessageChain.ToJson()[i].at("type") == "At")
						{
							GroupMember gm = bot.GetGroupMemberInfo(m.Sender.Group.GID, QQ_t(m.MessageChain.ToJson()[i].at("target")));
							c.Plain("@").Plain(gm.MemberName);
						}
						else if (m.MessageChain.ToJson()[i].at("type") == "Face")
						{
							c.Face(int(m.MessageChain.ToJson()[i].at("faceId")));
						}
						else if (m.MessageChain.ToJson()[i].at("type") == "Image")
						{
							GroupImage img;
							img.Id = m.MessageChain.ToJson()[i].at("imageId");
							c.Image(img);
						}
						else if (m.MessageChain.ToJson()[i].at("type") == "Plain")
						{
							c.Plain(string(m.MessageChain.ToJson()[i].at("text")));
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
}
