#include "ScriptMgr.h"
#include "AccountMgr.h"
#include "AMAS_Random_Ban.h"

void AMASRandomBan::Start()
{
    QueryResult result = LoginDatabase.Query("SELECT AccountID, UNIX_TIMESTAMP(Date), Duration, BanReason, Author FROM `account_rnd_ban`");
	if (!result)
		return;

	do
	{
		Field *field = result->Fetch();

		uint32 AccountID = field[0].GetUInt32();
		uint32 Time = field[1].GetUInt32();
		std::string DurationStr = field[2].GetString();
		std::string BanReason = field[3].GetString();
		std::string Author = field[4].GetString();

		if (DurationStr.empty())
			DurationStr = "0s";

		if (BanReason.empty())
			BanReason = "Cheat";

		if (Author.empty())
			Author = "System";

		uint32 TimeNow = uint32(time(NULL));

		std::string AccountName;
		AccountMgr::GetName(AccountID, AccountName);

		if (TimeNow > Time)
			sWorld->BanAccount(BAN_ACCOUNT, AccountName, DurationStr, BanReason, Author);

	} while (result->NextRow());

	LoginDatabase.Execute("DELETE FROM `account_rnd_ban` WHERE UNIX_TIMESTAMP(Date) <= UNIX_TIMESTAMP()");
}