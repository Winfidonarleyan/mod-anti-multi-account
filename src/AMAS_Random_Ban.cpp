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

void AMASRandomBan::AddRandomBan(uint32 AccountID, uint32 StartBanDate, std::string Duration, std::string BanReason, std::string BanAuthor)
{
    // Check if accounts exists
    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_BY_ID);
    stmt->setUInt32(0, AccountID);
    PreparedQueryResult Presult = LoginDatabase.Query(stmt);

    if (!Presult)
        return;

    QueryResult result = LoginDatabase.PQuery("SELECT * FROM `account_rnd_ban` WHERE `AccountID` = %u", AccountID);
    if (result)
        return;

    LoginDatabase.PExecute("INSERT INTO `account_rnd_ban`(`AccountID`, `Date`, `Duration`, `BanReason`, `Author`) VALUES (%u, FROM_UNIXTIME(%u), '%s', '%s', '%s')", AccountID, StartBanDate, Duration.c_str(), BanReason.c_str(), BanAuthor.c_str());
}