#pragma once

#include <iostream>
#include <mirai.h>
#include <cpr/cpr.h>
#include <mirai/third-party/nlohmann/json.hpp>

#undef SendMessage

namespace mis1072
{
	class htkt
	{
	public:
		htkt() {}

		htkt(json temp)
		{
			success = true;
			id = temp["id"];
			uuid = temp["uuid"];
			hitokoto = temp["hitokoto"];
			type = temp["type"];
			from = temp["from"];
			if (!temp["from_who"].is_null())
			{
				from_who = temp["from_who"];
			}
			creator = temp["creator"];
			creator_uid = temp["creator_uid"];
			reviewer = temp["reviewer"];
			commit_from = temp["commit_from"];
			created_at = temp["created_at"];
			length = temp["length"];
		}

		bool success = false;
		int id;
		string uuid;
		string hitokoto;
		string type;
		string from;
		string from_who = "null";
		string creator;
		int creator_uid;
		int reviewer;
		string commit_from;
		string created_at;
		unsigned int length;

		void operator()(json temp)
		{
			success = true;
			id = temp["id"];
			uuid = temp["uuid"];
			hitokoto = temp["hitokoto"];
			type = temp["type"];
			from = temp["from"];
			if (!temp["from_who"].is_null())
			{
				from_who = temp["from_who"];
			}
			creator = temp["creator"];
			creator_uid = temp["creator_uid"];
			reviewer = temp["reviewer"];
			commit_from = temp["commit_from"];
			created_at = temp["created_at"];
			length = temp["length"];
		}
	};

	void request_hitokoto(MiraiBot &bot, FriendMessage m)
	{
		cpr::Response res = cpr::Get(cpr::Url("https://v1.hitokoto.cn"));
		htkt temp;
		FriendImage img;
		MessageChain c;
		if (res.status_code == 200)
		{
			temp(json::parse(res.text));
		}
		res = cpr::Get(cpr::Url("https://api.vvhan.com/api/acgimg?type=json"));
		if (res.status_code == 200)
		{
			img.Url = json::parse(res.text)["imgurl"];
		}
		if (temp.success == true)
		{
			c.Plain(temp.hitokoto).Plain("\n\n类型：");
			if (temp.type == "a")c.Plain("动画");
			else if (temp.type == "b")c.Plain("漫画");
			else if (temp.type == "c")c.Plain("游戏");
			else if (temp.type == "d")c.Plain("文学");
			else if (temp.type == "e")c.Plain("原创");
			else if (temp.type == "f")c.Plain("来自网络");
			else if (temp.type == "g")c.Plain("其他");
			else if (temp.type == "h")c.Plain("影视");
			else if (temp.type == "i")c.Plain("诗词");
			else if (temp.type == "j")c.Plain("网易云");
			else if (temp.type == "k")c.Plain("哲学");
			else if (temp.type == "l")c.Plain("抖机灵");
			c.Plain("\n出处：").Plain(temp.from);
			if (temp.from_who != "null")c.Plain("\n作者：").Plain(temp.from_who);
			c.Plain("\n来源：hitokoto.cn").Image(img);
			cout << "[一言-好友]" << m.Sender.NickName << "(" << m.Sender.QQ.ToInt64() << ")" << endl
			     << "\tMessageId: " << bot.SendMessage(m.Sender.QQ, c) << endl;
		}
	}

	void request_hitokoto(MiraiBot &bot, GroupMessage m)
	{
		cpr::Response res = cpr::Get(cpr::Url("https://v1.hitokoto.cn"));
		htkt temp;
		GroupImage img;
		MessageChain c;
		if (res.status_code == 200)
		{
			temp(json::parse(res.text));
		}
		res = cpr::Get(cpr::Url("https://api.vvhan.com/api/acgimg?type=json"));
		if (res.status_code == 200)
		{
			img.Url = json::parse(res.text)["imgurl"];
		}
		if (temp.success == true)
		{
			c.Plain(temp.hitokoto).Plain("\n\n类型：");
			if (temp.type == "a")c.Plain("动画");
			else if (temp.type == "b")c.Plain("漫画");
			else if (temp.type == "c")c.Plain("游戏");
			else if (temp.type == "d")c.Plain("文学");
			else if (temp.type == "e")c.Plain("原创");
			else if (temp.type == "f")c.Plain("来自网络");
			else if (temp.type == "g")c.Plain("其他");
			else if (temp.type == "h")c.Plain("影视");
			else if (temp.type == "i")c.Plain("诗词");
			else if (temp.type == "j")c.Plain("网易云");
			else if (temp.type == "k")c.Plain("哲学");
			else if (temp.type == "l")c.Plain("抖机灵");
			c.Plain("\n出处：").Plain(temp.from);
			if (temp.from_who != "null")c.Plain("\n作者：").Plain(temp.from_who);
			c.Plain("\n来源：hitokoto.cn").Image(img);
			cout << "[一言-群组]" << m.Sender.Group.Name << "(" << m.Sender.Group.GID.ToInt64() << ")" << endl
			     << "\tMessageId: " << bot.SendMessage(m.Sender.Group.GID, c, m.MessageId()) << endl;
		}
	}

	void request_hitokoto(MiraiBot &bot, TempMessage m)
	{
		cpr::Response res = cpr::Get(cpr::Url("https://v1.hitokoto.cn"));
		htkt temp;
		GroupImage img;
		MessageChain c;
		if (res.status_code == 200)
		{
			temp(json::parse(res.text));
		}
		res = cpr::Get(cpr::Url("https://api.vvhan.com/api/acgimg?type=json"));
		if (res.status_code == 200)
		{
			img.Url = json::parse(res.text)["imgurl"];
		}
		if (temp.success == true)
		{
			c.Plain(temp.hitokoto).Plain("\n\n类型：");
			if (temp.type == "a")c.Plain("动画");
			else if (temp.type == "b")c.Plain("漫画");
			else if (temp.type == "c")c.Plain("游戏");
			else if (temp.type == "d")c.Plain("文学");
			else if (temp.type == "e")c.Plain("原创");
			else if (temp.type == "f")c.Plain("来自网络");
			else if (temp.type == "g")c.Plain("其他");
			else if (temp.type == "h")c.Plain("影视");
			else if (temp.type == "i")c.Plain("诗词");
			else if (temp.type == "j")c.Plain("网易云");
			else if (temp.type == "k")c.Plain("哲学");
			else if (temp.type == "l")c.Plain("抖机灵");
			c.Plain("\n出处：").Plain(temp.from);
			if (temp.from_who != "null")c.Plain("\n作者：").Plain(temp.from_who);
			c.Plain("\n来源：hitokoto.cn").Image(img);
			cout << "[一言-临时]" << m.Sender.MemberName << "(" << m.Sender.QQ.ToInt64() << ")" << endl
			     << "\tFrom: " << m.Sender.Group.Name << "(" << m.Sender.Group.GID.ToInt64() << ") MessageId: " << bot.SendMessage(m.Sender.Group.GID, m.Sender.QQ, c) << endl;
		}
	}
}