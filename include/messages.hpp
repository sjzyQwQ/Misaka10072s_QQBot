#pragma once
#include<iostream>
#include<mirai.h>
using namespace std;
using namespace Cyan;

void ReceiveMessage(MiraiBot& bot, FriendMessage& m, const map<GID_t, map<MessageId_t, vector<int64_t>>>fwd_id, bool is_Quote = false)
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
			cout << "[图片]" << string(m.MessageChain.ToJson()[0].at("url")) << " ";
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

void ReceiveMessage(MiraiBot& bot, GroupMessage& m, const map<GID_t, map<MessageId_t, vector<int64_t>>>fwd_id, bool is_Quote = false)
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
			cout << "[图片]" << string(m.MessageChain.ToJson()[0].at("url")) << " ";
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

void ReceiveMessage(MiraiBot& bot, TempMessage& m, const map<GID_t, map<MessageId_t, vector<int64_t>>>fwd_id, bool is_Quote = false)
{
	if (!is_Quote)
	{
		cout << "[临时]" << m.Sender.MemberName << "(" << m.Sender.QQ.ToInt64() << ")：";
	}
	for (int i = 0; i < m.MessageChain.ToJson().size(); ++i)
	{
		if (m.MessageChain.ToJson()[i].at("type") == "Face")
		{
			cout << "[" << string(m.MessageChain.ToJson()[i].at("name")) << "]";
		}
		else if (m.MessageChain.ToJson()[i].at("type") == "Image")
		{
			cout << "[图片]" << string(m.MessageChain.ToJson()[0].at("url")) << " ";
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