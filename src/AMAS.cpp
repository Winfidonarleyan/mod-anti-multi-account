#include "ScriptMgr.h"
#include "Configuration/Config.h"
#include "AMAS.h"
#include "DBCStores.h"
#include "AccountMgr.h"
#include "AMAS_Config.h"

bool Prev(const std::pair<uint64, float> &a, const std::pair<uint64, float> &b)
{
    return a.second > b.second;
}

float AMAS::GetWPTotalTimeAccount(uint32 TotalTimeAccount)
{
    TotalTimeAccount += 1;

    uint32 MinTimeAccount = CONF_INT(conf::AMAS_MIN_TOTAL_TIME_ACC);
    uint32 WP = CONF_INT(conf::AMAS_MIN_TOTAL_TIME_ACC_POINT);

    float Total = float(WP) / float(MinTimeAccount) / float(TotalTimeAccount) * DAY * IN_MILLISECONDS;

    return std::max<float>(0.1f, Total);
}

float AMAS::GetWPAverageItemLevel(float AverageItemLevel)
{
    uint32 MinAVGILvl = CONF_INT(conf::AMAS_MIN_AVG_ILVL);
    uint32 WP = CONF_INT(conf::AMAS_MIN_AVG_ILVL_POINT);

    if (AverageItemLevel < float(MinAVGILvl))
        return float(WP);

    return 0.0f;
}

float AMAS::GetWPFreeTalent(uint32 FreeTalent)
{
    uint32 WP = CONF_INT(conf::AMAS_FREE_TALENT_POINT);

    if (FreeTalent > 0)
        return float(WP);

    return 0.0f;
}

float AMAS::GetWPCompletedQuestCount(uint32 CompleteQuest)
{
    uint32 MinQuestCount = CONF_INT(conf::AMAS_MIN_COUNT_REWARDED_QUEST);
    uint32 WP = CONF_INT(conf::AMAS_MIN_COUNT_REWARDED_QUEST_POINT);

    if (CompleteQuest < MinQuestCount)
        return float(WP);

    return 0.0f;
}

float AMAS::GetWPFriend(uint32 FriendCount)
{
    uint32 MinFriendCount = CONF_INT(conf::AMAS_MIN_COUNT_FRIEND);
    uint32 WP = CONF_INT(conf::AMAS_MIN_COUNT_FRIEND_POINT);

    if (FriendCount < MinFriendCount)
        return float(WP);

    return 0.0f;
}

float AMAS::GetWPMoney(uint32 Money)
{
    uint32 MaxMoneyCount = CONF_INT(conf::AMAS_MAX_COUNT_MONEY) * GOLD;
    uint32 WP = CONF_INT(conf::AMAS_MAX_COUNT_MONEY_POINT);

    if (Money > MaxMoneyCount)
        return float(WP);

    return 0.0f;
}

float AMAS::GetWPHonorAndKills(uint32 Honor, uint32 Kills)
{
    uint32 WP = CONF_INT(conf::AMAS_NULL_HONOR_AND_KILLS);

    if (!Honor && !Kills)
        return float(WP);

    return 0.0f;
}

float AMAS::GetWPIP(std::string IP)
{
    int8 IPCount = 0;

    if (CONF_BOOL(conf::AMAS_FULL_IP_CHECK_ENABLE))
        IPCount = sAMAS->GetFullIPCount(IP);
    else
        IPCount = sAMAS->GetOnlineIPCount(IP);
	
    uint32 WP = CONF_INT(conf::AMAS_MORE_IP_POINT);    

    if (IPCount > 1)
        return float(WP);

    return 0.0f;
}

float AMAS::GetWPTrainerSpells(uint32 MissingTrainerSpells)
{
    uint32 WP = CONF_INT(conf::AMAS_MISSING_TRAINER_SPELL_POINT);
    uint32 MinMissingSpells = CONF_INT(conf::AMAS_MIN_TRAINER_SPELL_MISSING);

    if (MissingTrainerSpells > MinMissingSpells)
        return float(WP);

    return 0.0f;
}

float AMAS::GetWPWarningZone(uint32 ZoneID)
{
    uint32 WP = CONF_INT(conf::AMAS_WARNING_ZONE_POINT);

    if (this->IsWarningZone(ZoneID))
        return float(WP);

    return 0.0f;
}

float AMAS::GetWPProfession(uint32 ProfCount)
{
    uint32 WP = CONF_INT(conf::AMAS_PROFESSION_POINT);
    uint32 MinProf = CONF_INT(conf::AMAS_MIN_PROFESSION);

    if (ProfCount < MinProf)
        return float(WP);

    return 0.0f;
}

float AMAS::GetWPJoinAccount(uint32 DateUnix)
{
    uint32 MinDiff = CONF_INT(conf::AMAS_DIFF_ACC_CREATE);
    uint32 WP = CONF_INT(conf::AMAS_DIFF_ACC_CREATE_POINT);

    if (DateUnix < MinDiff)
        return float(WP);

    return 0.0f;
}

float AMAS::GetWPJoinCharacter(uint32 DateUnix)
{
    uint32 MinDiff = CONF_INT(conf::AMAS_DIFF_CHAR_CREATE);
    uint32 WP = CONF_INT(conf::AMAS_DIFF_CHAR_CREATE_POINT);

    if (DateUnix < MinDiff)
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
        sLog->outString(">> Loaded zones %i за %u мс", count, GetMSTimeDiffToNow(oldMSTime));
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

    return WPTimeAcc + WPAverageIlvl + WPFreeTalent + WPCompletedQuest + WPFriend + WPMoney + WPHonorAndKills + WPTrainerSpells + WPWarningZone + WPProfession + WPIp + WPJoinAcc + WPJoinChar;
}

void AMAS::LogoutPlayer(Player * player)
{
    if (!CONF_BOOL(conf::AMAS_ENABLE))
        return;

    this->PushDBPlayerInfo(player);
}

void AMAS::PushDBPlayerInfo(Player* player)
{
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
        FriendCount = result->Fetch()->GetUInt32();

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
        DateUnix = result->Fetch()->GetUInt32();

    return DateUnix;
}

uint32 AMAS::GetDateUnixJoinCharacter(uint32 PlayerGuid)
{
    uint32 DateUnix = 0;

    QueryResult result = CharacterDatabase.PQuery("SELECT UNIX_TIMESTAMP(create_date) FROM characters WHERE guid = %u", PlayerGuid);
    if (result)
        DateUnix = result->Fetch()->GetUInt32();

    return DateUnix;
}


void AMAS::GetTopWPOnlineList(ChatHandler * handler)
{
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

int8 AMAS::GetCommentCount(uint64 PlayerGuid)
{
    QueryResult result = CharacterDatabase.PQuery("SELECT COUNT(*) FROM `amas_player_comment` WHERE `PlayerGuid` = %u", PlayerGuid);
    if (result)
        return result->Fetch()->GetInt8();

    return 0;
}

int8 AMAS::GetFullIPCount(std::string IP)
{
    QueryResult result = LoginDatabase.PQuery("SELECT COUNT(*) FROM `account` WHERE `last_ip` LIKE '%%%s%%'", IP.c_str());
    if (result)
        return result->Fetch()->GetInt8();

    return 0;
}

int8 AMAS::GetOnlineIPCount(std::string IP)
{
    int8 IPCount = 0;

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

std::string AMAS::GetAccountNameByLastIp(std::string IP, uint32 SkipAccount)
{
    QueryResult result = LoginDatabase.PQuery("SELECT username FROM `account` WHERE `last_ip` LIKE '%%%s%%' AND NOT id = %u", IP.c_str(), SkipAccount);
    if (result)
        return result->Fetch()->GetString();

    return "";
}
