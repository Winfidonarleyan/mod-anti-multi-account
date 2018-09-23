#include "ScriptMgr.h"
#include "Configuration/Config.h"
#include "AMAS.h"
#include "DBCStores.h"
#include "AccountMgr.h"
#include "AMAS_Config.h"
#include "AMAS_Random_Ban.h"

bool Prev(const std::pair<uint64, float> &a, const std::pair<uint64, float> &b)
{
    return a.second > b.second;
}

AMAS::AMAS()
{
    _sessionTimeStore.clear();
}

AMAS::~AMAS()
{
    while (!_sessionTimeStore.empty())
    {
        _sessionTimeStore.erase(_sessionTimeStore.begin());
    }
}

float AMAS::GetWPTotalTimeAccount(uint32 TotalTimeAccount)
{
    if (!CONF_BOOL(conf::AMAS_ENABLE))
        return 0.0f;
	
	TotalTimeAccount += 1;

    uint32 MinTimeAccount = CONF_INT(conf::AMAS_MIN_TOTAL_TIME_ACC);
    uint32 WP = CONF_INT(conf::AMAS_MIN_TOTAL_TIME_ACC_POINT);

    float Total = float(WP) / float(MinTimeAccount) / float(TotalTimeAccount) * DAY * IN_MILLISECONDS;

    return std::max<float>(0.1f, Total);
}

float AMAS::GetWPAverageItemLevel(float AverageItemLevel)
{
    if (!CONF_BOOL(conf::AMAS_ENABLE))
        return 0.0f;
	
	uint32 MinAVGILvl = CONF_INT(conf::AMAS_MIN_AVG_ILVL);
    uint32 WP = CONF_INT(conf::AMAS_MIN_AVG_ILVL_POINT);

    if (AverageItemLevel < float(MinAVGILvl))
        return float(WP);

    return 0.0f;
}

float AMAS::GetWPFreeTalent(uint32 FreeTalent)
{
    if (!CONF_BOOL(conf::AMAS_ENABLE))
        return 0.0f;
	
	uint32 WP = CONF_INT(conf::AMAS_FREE_TALENT_POINT);

    if (FreeTalent > 0)
        return float(WP);

    return 0.0f;
}

float AMAS::GetWPCompletedQuestCount(uint32 CompleteQuest)
{
    if (!CONF_BOOL(conf::AMAS_ENABLE))
        return 0.0f;
	
	uint32 MinQuestCount = CONF_INT(conf::AMAS_MIN_COUNT_REWARDED_QUEST);
    uint32 WP = CONF_INT(conf::AMAS_MIN_COUNT_REWARDED_QUEST_POINT);

    if (CompleteQuest < MinQuestCount)
        return float(WP);

    return 0.0f;
}

float AMAS::GetWPFriend(uint32 FriendCount)
{
    if (!CONF_BOOL(conf::AMAS_ENABLE))
        return 0.0f;
	
	uint32 MinFriendCount = CONF_INT(conf::AMAS_MIN_COUNT_FRIEND);
    uint32 WP = CONF_INT(conf::AMAS_MIN_COUNT_FRIEND_POINT);

    if (FriendCount < MinFriendCount)
        return float(WP);

    return 0.0f;
}

float AMAS::GetWPMoney(uint32 Money)
{
    if (!CONF_BOOL(conf::AMAS_ENABLE))
        return 0.0f;
	
	uint32 MaxMoneyCount = CONF_INT(conf::AMAS_MAX_COUNT_MONEY) * GOLD;
    uint32 WP = CONF_INT(conf::AMAS_MAX_COUNT_MONEY_POINT);

    if (Money > MaxMoneyCount)
        return float(WP);

    return 0.0f;
}

float AMAS::GetWPHonorAndKills(uint32 Honor, uint32 Kills)
{
    if (!CONF_BOOL(conf::AMAS_ENABLE))
        return 0.0f;
	
	uint32 WP = CONF_INT(conf::AMAS_NULL_HONOR_AND_KILLS);

    if (!Honor && !Kills)
        return float(WP);

    return 0.0f;
}

float AMAS::GetWPIP(std::string IP)
{
    if (!CONF_BOOL(conf::AMAS_ENABLE))
        return 0.0f;

    int8 IPCount = this->GetIPCount(IP);
    int8 SameFirstByteIpCount = this->GetIPCount(IP, true);
    
    uint32 WPSameIpFull = CONF_INT(conf::AMAS_MORE_IP_POINT_FULL);
    uint32 WPSameIpFirsByte = CONF_INT(conf::AMAS_MORE_IP_POINT_FIRST_BYTE);

    if (IPCount > 1)
        return float(WPSameIpFull);
    else if (SameFirstByteIpCount > 1)
        return float(WPSameIpFirsByte);

    return 0.0f;
}

float AMAS::GetWPTrainerSpells(uint32 MissingTrainerSpells)
{
    if (!CONF_BOOL(conf::AMAS_ENABLE))
        return 0.0f;
	
	uint32 WP = CONF_INT(conf::AMAS_MISSING_TRAINER_SPELL_POINT);
    uint32 MinMissingSpells = CONF_INT(conf::AMAS_MIN_TRAINER_SPELL_MISSING);

    if (MissingTrainerSpells > MinMissingSpells)
        return float(WP);

    return 0.0f;
}

float AMAS::GetWPWarningZone(uint32 ZoneID)
{
    if (!CONF_BOOL(conf::AMAS_ENABLE))
        return 0.0f;
	
	uint32 WP = CONF_INT(conf::AMAS_WARNING_ZONE_POINT);

    if (this->IsWarningZone(ZoneID))
        return float(WP);

    return 0.0f;
}

float AMAS::GetWPProfession(uint32 ProfCount)
{
    if (!CONF_BOOL(conf::AMAS_ENABLE))
        return 0.0f;
	
	uint32 WP = CONF_INT(conf::AMAS_PROFESSION_POINT);
    uint32 MinProf = CONF_INT(conf::AMAS_MIN_PROFESSION);

    if (ProfCount < MinProf)
        return float(WP);

    return 0.0f;
}

float AMAS::GetWPJoinAccount(uint32 DateUnix)
{
    if (!CONF_BOOL(conf::AMAS_ENABLE))
        return 0.0f;
	
	uint32 MinDiff = CONF_INT(conf::AMAS_DIFF_ACC_CREATE);
    uint32 WP = CONF_INT(conf::AMAS_DIFF_ACC_CREATE_POINT);

    if (DateUnix < MinDiff)
        return float(WP);

    return 0.0f;
}

float AMAS::GetWPJoinCharacter(uint32 DateUnix)
{
    if (!CONF_BOOL(conf::AMAS_ENABLE))
        return 0.0f;
	
	uint32 MinDiff = CONF_INT(conf::AMAS_DIFF_CHAR_CREATE);
    uint32 WP = CONF_INT(conf::AMAS_DIFF_CHAR_CREATE_POINT);

    if (DateUnix < MinDiff)
        return float(WP);

    return 0.0f;
}

float AMAS::GetWPAverageSessionTime(uint32 Time)
{
    if (!CONF_BOOL(conf::AMAS_ENABLE))
        return 0.0f;

    uint32 MitTime = CONF_INT(conf::AMAS_MIN_TIME_AVG_SESSION);
    uint32 WP = CONF_INT(conf::AMAS_MIN_TIME_AVG_SESSION_POINT);

    if (Time < MitTime)
        return float(WP);

    return 0.0f;
}

void AMAS::LoadWarningZone()
{
    if (!CONF_BOOL(conf::AMAS_ENABLE))
        return;

    uint32 oldMSTime = getMSTime();

    _warningZoneStore.clear();

    QueryResult result = CharacterDatabase.Query("SELECT ZoneID FROM `amas_warning_zone`");
    if (!result)
    {
        sLog->outString(">> Loaded 0 zones. Table `amas_warning_zone` is empty.");
        sLog->outString();
        return;
    }

    int32 count = 0;

    do
    {
        uint32 ZoneID = result->Fetch()->GetUInt32();

        AreaTableEntry const* zone = sAreaTableStore.LookupEntry(ZoneID);
        if (!zone)
        {
            sLog->outErrorDb(">> Not found zone (%u)", ZoneID);
            continue;
        }

        _warningZoneStore.push_back(ZoneID);
        count++;

    } while (result->NextRow());

    if (_warningZoneStore.size())
    {
        sLog->outString(">> Loaded zones %i за %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        sLog->outString();
    }
    else
    {
        sLog->outString(">> Loaded 0 zones. In DB table `amas_warning_zone` not found correct zone");
        sLog->outString();
    }
}

bool AMAS::IsWarningZoneExist()
{
    return _warningZoneStore.size();
}

bool AMAS::IsWarningZone(uint32 ZoneID)
{
    WarningZoneContainer& itr = _warningZoneStore;
    for (auto VectorZone : itr)
    {
        if (VectorZone == ZoneID)
            return true;
    }

    return false;
}

void AMAS::AddWarningZone(uint32 ZoneID, bool IsDB)
{
    if (!CONF_BOOL(conf::AMAS_ENABLE))
        return;
	
	_warningZoneStore.push_back(ZoneID);

    if (!IsDB)
        return;

    std::string ZoneName = "";

    AreaTableEntry const* zone = sAreaTableStore.LookupEntry(ZoneID);
    if (zone)
        ZoneName = zone->area_name[1];

    std::replace(ZoneName.begin(), ZoneName.end(), '\'', ' ');

    CharacterDatabase.PExecute("INSERT INTO `amas_warning_zone`(`ZoneID`, `Comment`) VALUES (%u, '%s')", ZoneID, ZoneName.c_str());
}

void AMAS::DeleteWarningZone(uint32 ZoneID, bool IsDB)
{
    if (!CONF_BOOL(conf::AMAS_ENABLE))
        return;
	
	_warningZoneStore.erase(std::remove(_warningZoneStore.begin(), _warningZoneStore.end(), ZoneID), _warningZoneStore.end());

    if (!IsDB)
        return;

    CharacterDatabase.PExecute("DELETE FROM `amas_warning_zone` WHERE `ZoneID` = %u", ZoneID);
}

float AMAS::GetAllWarningPoint(Player * player)
{
    if (!CONF_BOOL(conf::AMAS_ENABLE))
        return 0.0f;

    float WPTimeAcc = this->GetWPTotalTimeAccount(player->GetSession()->GetTotalTime());
    float WPAverageIlvl = this->GetWPAverageItemLevel(this->GetAverageItemLevel(player));
    float WPFreeTalent = this->GetWPFreeTalent(player->GetFreeTalentPoints());
    float WPCompletedQuest = this->GetWPCompletedQuestCount(player->GetRewardedQuestCount());
    float WPFriend = this->GetWPFriend(this->GetFriendCount(player));
    float WPMoney = this->GetWPMoney(player->GetMoney());
    float WPHonorAndKills = this->GetWPHonorAndKills(player->GetHonorPoints(), player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS));
    float WPTrainerSpells = this->GetWPTrainerSpells(this->GetMissingTrainerSpells(player));
    float WPWarningZone = this->GetWPWarningZone(player->GetZoneId());
    float WPProfession = this->GetWPProfession(this->GetProfessionCount(player));
    float WPIp = this->GetWPIP(player->GetSession()->GetRemoteAddress());
    float WPJoinAcc = this->GetWPJoinAccount(this->GetDateUnixJoinAccount(player->GetSession()->GetAccountId()));
    float WPJoinChar = this->GetWPJoinCharacter(this->GetDateUnixJoinCharacter(player->GetGUID()));
	float WPAVGSessionTime = this->GetWPAverageSessionTime(this->GetAverageSessionTime(player->GetGUID()));

    return WPTimeAcc + WPAverageIlvl + WPFreeTalent + WPCompletedQuest + WPFriend + WPMoney + WPHonorAndKills + WPTrainerSpells + WPWarningZone + WPProfession + WPIp + WPJoinAcc + WPJoinChar + WPAVGSessionTime;
}

void AMAS::LogoutPlayer(Player * player)
{
    if (!player || !CONF_BOOL(conf::AMAS_ENABLE))
        return;

    if (!CONF_BOOL(conf::AMAS_GM_CHECK_ENABLE) && sAccountMgr->IsGMAccount(player->GetSession()->GetSecurity()))
        return;
	
    this->PushDBPlayerInfo(player);
	this->AddAverageSessionTime(player->GetGUID());

    _sessionTimeStore.erase(player->GetGUID());
}

void AMAS::LoginPlayer(Player * player)
{
    if (!player || !CONF_BOOL(conf::AMAS_ENABLE))
        return;

    if (!CONF_BOOL(conf::AMAS_GM_CHECK_ENABLE) && sAccountMgr->IsGMAccount(player->GetSession()->GetSecurity()))
        return;

    uint32 MinWP = CONF_INT(conf::AMAS_SUSPICIOUS_ACCOUNT_MIN_POINT);    
    float PlayerWP = sAMAS->GetAllWarningPoint(player);

    if (PlayerWP > float(MinWP))
        sWorld->SendGMText(amas::AMAS_ANNOUNCE_GM, player->GetName().c_str(), PlayerWP);

    this->CheckConfirmed(player);
	
	uint32 TimeNow = uint32(time(NULL));
    _sessionTimeStore[player->GetGUID()] = TimeNow;
}

void AMAS::CheckConfirmed(Player* player)
{
    if (!player || !CONF_BOOL(conf::AMAS_ENABLE))
        return;
    
    uint32 MinWPConfirmed = CONF_INT(conf::AMAS_CONFIRMED_MIN_POINT);
    float PlayerWP = sAMAS->GetAllWarningPoint(player);

    if (PlayerWP < MinWPConfirmed)
        return;

    ConfirmedAction AMASConfirmedAction = ConfirmedAction(CONF_INT(conf::AMAS_CONFIRMED_ACTION));
    uint32 RandomIntervalMin = CONF_INT(conf::AMAS_CONFIRMED_BAN_INTERVAL_MIN);
    uint32 RandomIntervalMax = CONF_INT(conf::AMAS_CONFIRMED_BAN_INTERVAL_MAX);
    std::string Reason = CONF_STR(conf::AMAS_BAN_REASON);
    uint32 TimeNow = uint32(time(NULL));
    uint32 RandomInterval = urand(RandomIntervalMin, RandomIntervalMax);
    uint32 StartBanDate = TimeNow + RandomInterval;

    switch (AMASConfirmedAction)
    {
    case ACTION_BAN:
        sAMASRandomBan->AddRandomBan(player->GetSession()->GetAccountId(), StartBanDate, "0s", Reason, "System");
        break;
    case ACTION_GM_MESSAGE:
        sWorld->SendGMText(amas::AMAS_GM_ANNOUNCE_CONFIRMED, player->GetName().c_str(), PlayerWP, MinWPConfirmed);
        break;
    case ACTION_ALL:
        sWorld->SendGMText(amas::AMAS_GM_ANNOUNCE_CONFIRMED, player->GetName().c_str(), PlayerWP, MinWPConfirmed);
        sAMASRandomBan->AddRandomBan(player->GetSession()->GetAccountId(), StartBanDate, "0s", Reason, "System");
        break;
    default:
        break;
    }
}

void AMAS::PushDBPlayerInfo(Player* player)
{
    if (!CONF_BOOL(conf::AMAS_ENABLE))
        return;
	
	uint64 PlayerGUID = player->GetGUID();
    uint32 TotalTimeAccount = player->GetSession()->GetTotalTime();
    uint32 AVGILvl = this->GetAverageItemLevel(player);
    uint32 FreeTalent = player->GetFreeTalentPoints();
    uint32 TotalRewardQuest = player->GetRewardedQuestCount();
    uint32 TotalTimePlayed = player->GetTotalPlayedTime();
    uint32 FriendCount = this->GetFriendCount(player);;
    uint32 TotalMoney = player->GetMoney();
    uint32 TotalHonorPoint = player->GetHonorPoints();
    uint32 TotalKill = player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS);
    std::string PlayerIP = player->GetSession()->GetRemoteAddress();
    uint32 MissingTrainerSpells = this->GetMissingTrainerSpells(player);
    uint32 CurrentZone = player->GetZoneId();
    uint32 ProfCount = this->GetProfessionCount(player);
    float WPAll = this->GetAllWarningPoint(player);
   
    CharacterDatabase.PExecute("REPLACE INTO `amas_player_info`(`PlayerGUID`, `TotalWarningPoint`, `TotalTimeAccount`, `AverageItemLevel`, `IP`, `FriendCount`, `Money`, `CompletedQuests`, `TotalTimePlayed`, `Honor`, `Kills`, `CurrentZone`, `MissingSpells`, `ProfessionLearned`, `FreeTalent`) VALUES (%u, %f, %u, %u, '%s', %u, %u, %u, %u, %u, %u, %u, %u, %u, %u)",
        PlayerGUID, WPAll, TotalTimeAccount, AVGILvl, PlayerIP.c_str(), FriendCount, TotalMoney, TotalRewardQuest, TotalTimePlayed, TotalHonorPoint, TotalKill, CurrentZone, MissingTrainerSpells, ProfCount, FreeTalent);
}

uint32 AMAS::GetFriendCount(Player* player)
{
    uint32 FriendCount = 0;

    QueryResult result = CharacterDatabase.PQuery("SELECT COUNT(*) FROM character_social JOIN characters ON characters.guid = character_social.friend WHERE character_social.guid = %u AND deleteinfos_name IS NULL LIMIT 255;", player->GetGUID());
    if (result)
        FriendCount = (uint32)result->Fetch()->GetUInt64();

    return FriendCount;
}

uint32 AMAS::GetMissingTrainerSpells(Player* player)
{
    std::vector<uint32> NpcTrainers =
    {
        0, // None
        985, // Warrior
        927, // Paladin
        987, // Hunter
        917, // Rogue
        376, // Priest
        28472, // Death knight
        986, // Shaman
        328, // Mage
        906, // Warlock
        0, // Monk (5.x)
        3033 // Druid
    };

    uint8 PlayerClass = player->getClass();
    uint32 MissingTrainerSpells = 0;

    if (auto spells = sObjectMgr->GetNpcTrainerSpells(NpcTrainers[PlayerClass]))
    {
        for (auto itr = spells->spellList.begin(); itr != spells->spellList.end(); itr++)
        {
            auto spell = &itr->second;

            if (spell->spell > 0 && player->GetTrainerSpellState(spell) == TRAINER_SPELL_GREEN && !player->HasSpell(spell->spell))
                MissingTrainerSpells++;
        }
    }

    return MissingTrainerSpells;
}

uint32 AMAS::GetProfessionCount(Player* player)
{
    uint32 ProfCount = 0;

    if (player->HasSkill(SKILL_MINING))
        ProfCount++;
    if (player->HasSkill(SKILL_SKINNING))
        ProfCount++;
    if (player->HasSkill(SKILL_HERBALISM))
        ProfCount++;

    for (uint32 i = 1; i < sSkillLineStore.GetNumRows(); ++i)
    {
        SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(i);
        if (!SkillInfo)
            continue;

        if (SkillInfo->categoryId == SKILL_CATEGORY_SECONDARY)
            continue;

        if ((SkillInfo->categoryId != SKILL_CATEGORY_PROFESSION) || !SkillInfo->canLink)
            continue;

        const uint32 skillID = SkillInfo->id;
        if (player->HasSkill(skillID))
            ProfCount++;
    }

    return ProfCount;
}

float AMAS::GetAverageItemLevel(Player* player)
{
    float sum = 0;
    uint32 count = 0;

    for (uint8 Slot = EQUIPMENT_SLOT_START; Slot < EQUIPMENT_SLOT_END; Slot++)
    {
        Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, Slot);
        if (!item || !item->GetTemplate())
            continue;

        if (Slot == EQUIPMENT_SLOT_TABARD || Slot == EQUIPMENT_SLOT_OFFHAND || Slot == EQUIPMENT_SLOT_BODY)
            continue;

        sum += item->GetTemplate()->ItemLevel;

        ++count;
    }

    return std::max<float>(0.0f, sum / (float)count);
}

uint32 AMAS::GetDateUnixJoinAccount(uint32 AccountID)
{
    uint32 DateUnix = 0;

    QueryResult result = LoginDatabase.PQuery("SELECT UNIX_TIMESTAMP(joindate) FROM account WHERE id = %u", AccountID);
    if (result)
        DateUnix = (uint32)result->Fetch()->GetUInt64();

    return DateUnix;
}

uint32 AMAS::GetDateUnixJoinCharacter(uint32 PlayerGuid)
{
    uint32 DateUnix = 0;

    QueryResult result = CharacterDatabase.PQuery("SELECT UNIX_TIMESTAMP(create_date) FROM characters WHERE guid = %u", PlayerGuid);
    if (result)
        DateUnix = (uint32)result->Fetch()->GetUInt64();

    return DateUnix;
}

void AMAS::GetTopWPOnlineList(ChatHandler * handler)
{
    if (!CONF_BOOL(conf::AMAS_ENABLE))
        return;
	
	std::map<uint64, float> WPMap;

    int8 Count = 0;

    SessionMap::iterator itr;
    SessionMap m_sessions = sWorld->GetAllSessions();
    for (itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
    {
        if (Count > 20)
            break;

        if (!itr->second && !itr->second->GetPlayer() && !itr->second->GetPlayer()->IsInWorld())
            continue;

        float WPAll = sAMAS->GetAllWarningPoint(itr->second->GetPlayer());

        if (WPAll < float(CONF_INT(conf::AMAS_SUSPICIOUS_ACCOUNT_MIN_POINT)))
            continue;

        WPMap[itr->second->GetPlayer()->GetGUID()] = WPAll;

        Count++;
    }

    if (WPMap.empty())
    {
        handler->PSendSysMessage(amas::AMAS_LIST_ONLINE_PLAYER_NOT_FOUND);
        handler->SetSentErrorMessage(true);
        return;
    }

    std::vector<std::pair<uint64, float>> VectorWP(WPMap.begin(), WPMap.end());
    std::sort(VectorWP.begin(), VectorWP.end(), Prev);

    handler->PSendSysMessage(amas::AMAS_LIST_ONLINE_PLAYER);

    for (auto itr : VectorWP)
    {
        std::string PlayerName;

        sObjectMgr->GetPlayerNameByGUID(itr.first, PlayerName);

        handler->PSendSysMessage("%.2f - %s", itr.second, PlayerName.c_str());
    }
}

void AMAS::GetTopWPOfflineList(ChatHandler * handler)
{
	if (!CONF_BOOL(conf::AMAS_ENABLE))
        return;
	
	QueryResult result = CharacterDatabase.PQuery("SELECT PlayerGUID, TotalWarningPoint FROM `amas_player_info` WHERE TotalWarningPoint > %u ORDER BY `TotalWarningPoint` DESC LIMIT 0, 20", CONF_INT(conf::AMAS_SUSPICIOUS_ACCOUNT_MIN_POINT));
    if (!result)
    {
        handler->PSendSysMessage(amas::AMAS_LIST_OFFLINE_PLAYER_NOT_FOUND);
        handler->SetSentErrorMessage(true);
        return;
    }

    handler->PSendSysMessage(amas::AMAS_LIST_OFFLINE_PLAYER);

    do
    {
        Field* field = result->Fetch();

        uint32 PlayerGuid = field[0].GetUInt32();
        float WPAll = field[1].GetFloat();
        std::string PlayerName;

        sObjectMgr->GetPlayerNameByGUID(PlayerGuid, PlayerName);

        if (PlayerName.empty())
            continue;

        if (ObjectAccessor::FindPlayerByName(PlayerName))
            continue;

        handler->PSendSysMessage("%.2f - %s", WPAll, PlayerName.c_str());

    } while (result->NextRow());
}

uint32 AMAS::GetCommentCount(uint64 PlayerGuid)
{
    if (!CONF_BOOL(conf::AMAS_ENABLE))
        return 0;
	
	QueryResult result = CharacterDatabase.PQuery("SELECT COUNT(*) FROM `amas_player_comment` WHERE `PlayerGuid` = %u", PlayerGuid);
    if (result)
        return (uint32)result->Fetch()->GetUInt64();

    return 0;
}

int8 AMAS::GetFirstByteIP(std::string IP)
{
    std::string Buff;

    for (size_t i = 0; i < IP.size() + 1; i++)
    {
        if (!(IP[i] == '.'))
            Buff += IP[i];
        else
            break;
    }

    return (int8)atoi(Buff.c_str());
}

bool AMAS::IsFirstByteIPSame(std::string IP1, std::string IP2)
{
    int8 FirsByteIP1 = this->GetFirstByteIP(IP1);
    int8 FirsByteIP2 = this->GetFirstByteIP(IP2);

    return FirsByteIP1 == FirsByteIP2;
}

uint32 AMAS::GetIPCount(std::string IP, bool IsFirstByte /*= false*/)
{
    if (IP.empty())
        return 0;

    uint32 IPCount = 0;
    QueryResult result = nullptr;

    if (CONF_BOOL(conf::AMAS_FULL_IP_CHECK_ENABLE) && IsFirstByte)
    {
        result = LoginDatabase.PQuery("SELECT COUNT(*) FROM `account` WHERE `last_ip` LIKE '%i%%'", this->GetFirstByteIP(IP));
        if (result)
            return (uint32)result->Fetch()->GetUInt64();

        return 0;
    }
    else if (CONF_BOOL(conf::AMAS_FULL_IP_CHECK_ENABLE) && !IsFirstByte)
    {
        result = LoginDatabase.PQuery("SELECT COUNT(*) FROM `account` WHERE `last_ip` LIKE '%%%s%%'", IP.c_str());
        if (result)
            return (uint32)result->Fetch()->GetUInt64();

        return 0;
    }
    else if (!CONF_BOOL(conf::AMAS_FULL_IP_CHECK_ENABLE) && IsFirstByte)
    {
        SessionMap::iterator itr;
        SessionMap m_sessions = sWorld->GetAllSessions();
        for (itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
        {
            if (!itr->second && !itr->second->GetPlayer() && !itr->second->GetPlayer()->IsInWorld())
                continue;

            if (this->IsFirstByteIPSame(itr->second->GetRemoteAddress(), IP))
                IPCount++;
        }

        return IPCount;
    }
    else if (!CONF_BOOL(conf::AMAS_FULL_IP_CHECK_ENABLE) && !IsFirstByte)
    {
        SessionMap::iterator itr;
        SessionMap m_sessions = sWorld->GetAllSessions();
        for (itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
        {
            if (!itr->second && !itr->second->GetPlayer() && !itr->second->GetPlayer()->IsInWorld())
                continue;

            if (itr->second->GetRemoteAddress() == IP)
                IPCount++;
        }

        return IPCount;
    }
    else
        return 0;
}

std::string AMAS::GetListAccountForIP(std::string IP)
{
    QueryResult result = LoginDatabase.PQuery("SELECT username FROM `account` WHERE `last_ip` LIKE '%%%s%%'", IP.c_str());
    if (!result)
        return "";

    std::string Buff;

    do 
    {
        if (!Buff.empty())
            Buff += ", ";

        Buff += result->Fetch()->GetString();

    } while (result->NextRow());

    return Buff;
}

uint32 AMAS::GetAverageSessionTime(uint64 PlayerGuid)
{
    if (!CONF_BOOL(conf::AMAS_ENABLE))
        return 0;

    uint32 TimeNow = uint32(time(NULL));
    
    if (!this->IsFoundAVGHistoryInDB(PlayerGuid))
        return TimeNow - _sessionTimeStore[PlayerGuid];

    QueryResult result = CharacterDatabase.PQuery("SELECT SessionDurationAverage FROM `amas_avg_session` WHERE `PlayerGuid` = %u", PlayerGuid);
    if (result)
        return result->Fetch()->GetUInt32();

    return 0;
}

bool AMAS::IsFoundAVGHistoryInDB(uint64 PlayerGuid)
{
    QueryResult result = CharacterDatabase.PQuery("SELECT * FROM `amas_avg_session` WHERE `PlayerGuid` = %u", PlayerGuid);
    if (result)
        return true;

    return false;
}

void AMAS::AddAverageSessionTime(uint64 PlayerGuid)
{
    if (!CONF_BOOL(conf::AMAS_ENABLE))
        return;

    uint32 TimeNow = uint32(time(NULL));
    uint32 SessionDuration = TimeNow - _sessionTimeStore[PlayerGuid];

    if (this->IsFoundAVGHistoryInDB(PlayerGuid))
    {
        QueryResult result = CharacterDatabase.PQuery("SELECT SessionDurationAverage, SessionNumber FROM `amas_avg_session` WHERE `PlayerGuid` = %u", PlayerGuid);
        if (!result)
            return;

        Field* fields = result->Fetch();
        uint32 SessionDurationAverage = fields[0].GetUInt32();
        uint32 SessionNumber = fields[1].GetUInt32();

        if (!SessionNumber)
            SessionNumber = 1; // not crash :D

        uint32 AVG = (SessionDurationAverage * (SessionNumber - 1) + SessionDuration) / SessionNumber;

        SessionNumber++;

        CharacterDatabase.PExecute("UPDATE `amas_avg_session` SET `SessionDurationLast` = %u, `SessionDurationAverage` = %u, `SessionNumber` = `SessionNumber` + 1 WHERE `PlayerGuid` = %u", SessionDuration, AVG, PlayerGuid);

    }
    else
        CharacterDatabase.PExecute("INSERT INTO `amas_avg_session`(`PlayerGuid`, `SessionDurationAverage`, `SessionDurationLast`, `SessionNumber`) VALUES (%u, %u%u, 1)", PlayerGuid, SessionDuration, SessionDuration);
}
