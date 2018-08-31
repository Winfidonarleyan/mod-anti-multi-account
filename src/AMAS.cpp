/**
    This plugin can be used for common player customizations
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Configuration/Config.h"
#include "AMAS.h"
#include "Chat.h"
#include "DBCStores.h"
#include "AccountMgr.h"

AMAS::AMAS()
{
    _playerWarningPointStore.clear();
}

AMAS::~AMAS()
{
    while (!_playerWarningPointStore.empty())
    {
        _playerWarningPointStore.erase(_playerWarningPointStore.begin());
    }
}

void AMASConfig::LoadConfig()
{
    // AMAS general
    m_bool[conf::AMAS_ENABLE]                           = sConfigMgr->GetBoolDefault("AMAS.Enable", true);
    m_int[conf::AMAS_MIN_TOTAL_TIME_ACC]                = sConfigMgr->GetIntDefault("AMAS.Min.Total.Time.Account", DAY);
    m_int[conf::AMAS_MIN_TOTAL_TIME_ACC_POINT]          = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Total.Time.Account", 10);
    m_int[conf::AMAS_MIN_AVG_ILVL]                      = sConfigMgr->GetIntDefault("AMAS.Min.Average.Ilvl", 50);
    m_int[conf::AMAS_MIN_AVG_ILVL_POINT]                = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Average.Ilvl", 10);
    m_int[conf::AMAS_FREE_TALENT_POINT]                 = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Free.Talent", 5);
    m_int[conf::AMAS_MIN_COUNT_REWARDED_QUEST]          = sConfigMgr->GetIntDefault("AMAS.Min.Completed.Quest.Count", 20);
    m_int[conf::AMAS_MIN_COUNT_REWARDED_QUEST_POINT]    = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Completed.Quest.Count", 10);
    m_int[conf::AMAS_MIN_COUNT_FRIEND]                  = sConfigMgr->GetIntDefault("AMAS.Min.Friend.Count", 2);
    m_int[conf::AMAS_MIN_COUNT_FRIEND_POINT]            = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Min.Friend", 5);
    m_int[conf::AMAS_MAX_COUNT_MONEY]                   = sConfigMgr->GetIntDefault("AMAS.Max.Count.Money", 2) * GOLD;
    m_int[conf::AMAS_MAX_COUNT_MONEY_POINT]             = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Max.Count.Money", 5);
    m_int[conf::AMAS_NULL_HONOR_AND_KILLS]              = sConfigMgr->GetIntDefault("AMAS.Warning.Point.NULL.Honor.And.Kills", 10);
    m_int[conf::AMAS_MORE_IP_POINT]                     = sConfigMgr->GetIntDefault("AMAS.Warning.Point.More.IP", 20);
    m_int[conf::AMAS_MIN_TRAINER_SPELL_MISSING]         = sConfigMgr->GetIntDefault("AMAS.Min.Missing.Trainer.Spells", 1);
    m_int[conf::AMAS_MISSING_TRAINER_SPELL_POINT]       = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Missing.One.Trainer.Spell", 20);
    m_int[conf::AMAS_WARNING_ZONE_POINT]                = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Warning.Zone", 20);
    m_int[conf::AMAS_MIN_PROFESSION]                    = sConfigMgr->GetIntDefault("AMAS.Min.Profession", 1);
    m_int[conf::AMAS_PROFESSION_POINT]                  = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Professions", 20);
    m_int[conf::AMAS_DIFF_ACC_CREATE]                   = sConfigMgr->GetIntDefault("AMAS.Min.Diff.Account.Create", DAY);
    m_int[conf::AMAS_DIFF_CHAR_CREATE]                  = sConfigMgr->GetIntDefault("AMAS.Min.Diff.Character.Create", DAY);
    m_int[conf::AMAS_DIFF_ACC_CREATE_POINT]             = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Diff.Account", 10);
    m_int[conf::AMAS_DIFF_CHAR_CREATE_POINT]            = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Diff.Character", 5);
    m_int[conf::AMAS_MIN_AVERAGE_SESSION]               = sConfigMgr->GetIntDefault("AMAS.Min.Average.Session", 5 * IN_MILLISECONDS * 60);
    m_int[conf::AMAS_MIN_AVERAGE_SESSION_POINT]         = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Min.Average.Session", 10);

    // Amas confirmed
    m_int[conf::AMAS_CONFIRMED_MIN_POINT]               = sConfigMgr->GetIntDefault("AMAS.Confirmed.Min.Points", 20);
    m_int[conf::AMAS_CONFIRMED_ACTION]                  = sConfigMgr->GetIntDefault("AMAS.Confirmed.Action", 20);
    m_int[conf::AMAS_CONFIRMED_BAN_INTERVAL_MIN]        = sConfigMgr->GetIntDefault("AMAS.Confirmed.Ban.RandomInterval.Min", 20);
    m_int[conf::AMAS_CONFIRMED_BAN_INTERVAL_MAX]        = sConfigMgr->GetIntDefault("AMAS.Confirmed.Ban.RandomInterval.Max", 20);
    m_string[conf::AMAS_BAN_REASON]                     = sConfigMgr->GetStringDefault("AMAS.Confirmed.Ban.Reason", "This account has been banned for using multiple accounts");

    // Other amas option
    m_int[conf::AMAS_SUSPICIOUS_ACCOUNT_MIN_POINT]      = sConfigMgr->GetIntDefault("AMAS.Suspicious.Account.Min.Points", 50);
    m_int[conf::AMAS_LOW_GMLEVEL]                       = sConfigMgr->GetIntDefault("AMAS.Low.GMLevel", 20);
    m_int[conf::AMAS_HIGH_GMLEVEL]                      = sConfigMgr->GetIntDefault("AMAS.High.GMLevel", 20);
    m_int[conf::AMAS_WARNING_INTERVAL]                  = sConfigMgr->GetIntDefault("AMAS.Warning.Interval", 20);
    m_int[conf::AMAS_MIN_TIME_TO_DB_HISTORY]            = sConfigMgr->GetIntDefault("AMAS.Min.Time.For.DB.History", DAY / 2);
    m_bool[conf::AMAS_GM_CHECK_ENABLE]                  = sConfigMgr->GetBoolDefault("AMAS.GM.Check.Enable", false);
}
float AMAS::GetWPTotalTimeAccount(uint32 TotalTimeAccount)
{
    if (TotalTimeAccount == 0)
        TotalTimeAccount = 60;

    uint32 MinTimeAccount = CONF_INT(conf::AMAS_MIN_TOTAL_TIME_ACC);
    uint32 PointWarning = CONF_INT(conf::AMAS_MIN_TOTAL_TIME_ACC_POINT);

    return float(PointWarning) / float(MinTimeAccount) / float(TotalTimeAccount) * DAY * IN_MILLISECONDS;
}

float AMAS::GetWPAverageItemLevel(float AverageItemLevel)
{
    uint32 MinAVGILvl = CONF_INT(conf::AMAS_MIN_AVG_ILVL);
    uint32 PointWarning = CONF_INT(conf::AMAS_MIN_AVG_ILVL_POINT);

    if (AverageItemLevel < float(MinAVGILvl))
        return float(PointWarning);

    return 0.0f;
}

float AMAS::GetWPFreeTalent(uint32 FreeTalent)
{
    uint32 PointWarning = CONF_INT(conf::AMAS_FREE_TALENT_POINT);

    if (FreeTalent > 0)
        return float(PointWarning);

    return 0.0f;
}

float AMAS::GetWPCompletedQuestCount(uint32 CompleteQuest)
{
    uint32 MinQuestCount = CONF_INT(conf::AMAS_MIN_COUNT_REWARDED_QUEST);
    uint32 PointWarning = CONF_INT(conf::AMAS_MIN_COUNT_REWARDED_QUEST_POINT);

    if (CompleteQuest < MinQuestCount)
        return float(PointWarning);

    return 0.0f;
}

float AMAS::GetWPFriend(uint32 FriendCount)
{
    uint32 MinFriendCount = CONF_INT(conf::AMAS_MIN_COUNT_FRIEND);
    uint32 PointWarning = CONF_INT(conf::AMAS_MIN_COUNT_FRIEND_POINT);

    if (FriendCount < MinFriendCount)
        return float(PointWarning);

    return 0.0f;
}

float AMAS::GetWPMoney(uint32 Money)
{
    uint32 MaxMoneyCount = CONF_INT(conf::AMAS_MAX_COUNT_MONEY) * GOLD;
    uint32 PointWarning = CONF_INT(conf::AMAS_MAX_COUNT_MONEY_POINT);

    if (Money > MaxMoneyCount)
        return float(PointWarning);

    return 0.0f;
}

float AMAS::GetWPHonorAndKills(uint32 Honor, uint32 Kills)
{
    uint32 PointWarning = CONF_INT(conf::AMAS_NULL_HONOR_AND_KILLS);

    if (!Honor && !Kills)
        return float(PointWarning);

    return 0.0f;
}

float AMAS::GetWPIP(std::string IP)
{
    uint8 IPCount = 0;
    uint32 PointWarning = CONF_INT(conf::AMAS_MORE_IP_POINT);

    SessionMap::iterator itr;
    SessionMap m_sessions = sWorld->GetAllSessions();
    for (itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
    {
        if (!itr->second && !itr->second->GetPlayer() && !itr->second->GetPlayer()->IsInWorld())
            continue;

        if (itr->second->GetRemoteAddress() == IP)
            IPCount++;
    }

    if (IPCount > 1)
        return float(PointWarning);

    return 0.0f;
}

float AMAS::GetWPTrainerSpells(uint32 MissingTrainerSpells)
{
    uint32 PointWarning = CONF_INT(conf::AMAS_MISSING_TRAINER_SPELL_POINT);
    uint32 MinMissingSpells = CONF_INT(conf::AMAS_MIN_TRAINER_SPELL_MISSING);

    if (MissingTrainerSpells > MinMissingSpells)
        return float(PointWarning);

    return 0.0f;
}

float AMAS::GetWPWarningZone(uint32 ZoneID)
{
    uint32 PointWarning = CONF_INT(conf::AMAS_WARNING_ZONE_POINT);

    if (this->IsWarningZone(ZoneID))
        return float(PointWarning);

    return 0.0f;
}

float AMAS::GetWPProfession(uint32 ProfCount)
{
    uint32 PointWarning = CONF_INT(conf::AMAS_PROFESSION_POINT);
    uint32 MinProf = CONF_INT(conf::AMAS_MIN_PROFESSION);

    if (ProfCount < MinProf)
        return float(PointWarning);

    return 0.0f;
}

float AMAS::GetWPJoinAccount(uint32 DateUnix)
{
    uint32 MinDiff = CONF_INT(conf::AMAS_DIFF_ACC_CREATE);
    uint32 PointWarning = CONF_INT(conf::AMAS_DIFF_ACC_CREATE_POINT);

    if (DateUnix < MinDiff)
        return float(PointWarning);

    return 0.0f;
}

float AMAS::GetWPJoinCharacter(uint32 DateUnix)
{
    uint32 MinDiff = CONF_INT(conf::AMAS_DIFF_CHAR_CREATE);
    uint32 PointWarning = CONF_INT(conf::AMAS_DIFF_CHAR_CREATE_POINT);

    if (DateUnix < MinDiff)
        return float(PointWarning);

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

    CharacterDatabase.PExecute("DELETE FROM `amas_player_info` WHERE `PlayerGUID` = %u", PlayerGUID);
    CharacterDatabase.PExecute("INSERT INTO `amas_player_info`(`PlayerGUID`, `TotalTimeAccount`, `AverageItemLevel`, `IP`, `FriendCount`, `Money`, `CompletedQuests`, `TotalTimePlayed`, `Honor`, `Kills`, `CurrentZone`, `MissingSpells`, `ProfessionLearned`, `FreeTalent`, `DateCheck`) VALUES (%u, %u, %u, '%s', %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, NOW())",
        PlayerGUID, TotalTimeAccount, AVGILvl, PlayerIP.c_str(), FriendCount, TotalMoney, TotalRewardQuest, TotalTimePlayed, TotalHonorPoint, TotalKill, CurrentZone, MissingTrainerSpells, ProfCount, FreeTalent);
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

// AMAS SC
class AMAS_SC : public PlayerScript
{
public:
    AMAS_SC() : PlayerScript("AMAS_SC") { }    

    void OnLogin(Player* player) override
    {
        if (!CONF_BOOL(conf::AMAS_ENABLE))
            return;

        if (!(CONF_BOOL(conf::AMAS_GM_CHECK_ENABLE) && !AccountMgr::IsPlayerAccount(player->GetSession()->GetSecurity())))
            return;

        uint32 MinWaringPoint = CONF_INT(conf::AMAS_SUSPICIOUS_ACCOUNT_MIN_POINT);
        float PlayerWarningPoint = sAMAS->GetAllWarningPoint(player);

        if (PlayerWarningPoint > float(MinWaringPoint))
            sWorld->SendGMText(amas::AMAS_ANNOUNCE_GM, player->GetName().c_str(), PlayerWarningPoint);
    }

    void OnLogout(Player* player) override
    {
        if (!CONF_BOOL(conf::AMAS_ENABLE))
            return;

        if (!(CONF_BOOL(conf::AMAS_GM_CHECK_ENABLE) && !AccountMgr::IsPlayerAccount(player->GetSession()->GetSecurity())))
            return;

        sAMAS->LogoutPlayer(player);
    }
};

class AMAS_SC_World : public WorldScript
{
public:
    AMAS_SC_World() : WorldScript("AMAS_SC_World") { }

    void OnLoadCustomDatabaseTable() override
    {
        if (!CONF_BOOL(conf::AMAS_ENABLE))
            return;

        sLog->outString("Loading warning zone for AMAS");
        sAMAS->LoadWarningZone();
    }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        if (!CONF_BOOL(conf::AMAS_ENABLE))
            return;

        sLog->outString("Loading AMAS configuration...");
        sAMASConfig->LoadConfig();
    }
};

class CS_AMAS : public CommandScript
{
public:
    CS_AMAS() : CommandScript("CS_AMAS") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> TableCommandAmasZone = // .amas zone
        {
            { "list",	    	    SEC_ADMINISTRATOR,  	true, &HandleAMASZoneList, 		   	                "" },
            { "add",	    	    SEC_ADMINISTRATOR,  	true, &HandleAMASZoneAdd, 		   	                "" },
            { "delete",	    	    SEC_ADMINISTRATOR,  	true, &HandleAMASZoneDelete, 		   	            "" },
            { "reload",	    	    SEC_ADMINISTRATOR,  	true, &HandleAMASZoneRelaod, 		   	            "" }
        };        

        static std::vector<ChatCommand> TableCommandAmasComment = // .amas comment (not work)
        {
            /*{ "add",	            SEC_ADMINISTRATOR,  	true, &HandleAMASCommentAdd, 		   	            "" },
            { "delete",	    	    SEC_ADMINISTRATOR,  	true, &HandleAMASCommentDelete, 		   	        "" },
            { "edit",	    	    SEC_ADMINISTRATOR,  	true, &HandleAMASCommentEdit, 		   	            "" }*/
        };

        static std::vector<ChatCommand> TableCommandAmasList = // .amas list (not work)
        {
            /*{ "all",	            SEC_ADMINISTRATOR,  	true, &HandleAMASListAll, 		   	                "" },
            { "online",	    	    SEC_ADMINISTRATOR,  	true, &HandleAMASListOnline, 		   	            "" },
            { "offline",	    	SEC_ADMINISTRATOR,  	true, &HandleAMASListOffline, 		   	            "" }*/
        };

        static std::vector<ChatCommand> TableCommandAmas = // .amas
        {
            { "zone",				SEC_ADMINISTRATOR, 		true, nullptr,             	   					    "", TableCommandAmasZone },
            { "info",				SEC_ADMINISTRATOR, 		true, &HandleAMASInfo,             	   			    "" },
            { "comment",			SEC_ADMINISTRATOR, 		true, nullptr,             	   						"", TableCommandAmasComment },
            { "list",			    SEC_ADMINISTRATOR, 		true, nullptr,             	   						"", TableCommandAmasList }
        };

        static std::vector<ChatCommand> commandTable =
        {  
            { "amas",				SEC_ADMINISTRATOR, 		true, nullptr,             	   				        "", TableCommandAmas }
        };

        return commandTable;
    }
    /*
    static bool HandleKargatumTest(ChatHandler *handler, const char *args)
    {
        return true;
    }

    static bool HandleAMASListAll(ChatHandler *handler, const char *args)
    {
        return true;
    }

    static bool HandleAMASListOnline(ChatHandler *handler, const char *args)
    {
        return true;
    }

    static bool HandleAMASListOffline(ChatHandler *handler, const char *args)
    {
        return true;
    }

    static bool HandleAMASCommentAdd(ChatHandler *handler, const char *args)
    {
        return true;
    }

    static bool HandleAMASCommentDelete(ChatHandler *handler, const char *args)
    {
        return true;
    }

    static bool HandleAMASCommentEdit(ChatHandler *handler, const char *args)
    {
        return true;
    }

    static bool HandleAMASInfoAccount(ChatHandler *handler, const char *args)
    {
        return true;
    }

    static bool HandleAMASInfoPlayer(ChatHandler *handler, const char *args)
    {
        return true;
    }
    */
    static bool HandleAMASZoneList(ChatHandler *handler, const char* /*args*/)
    {
        if (!sAMAS->IsWarningZoneExist())
        {
            handler->PSendSysMessage(amas::LANG_AMAS_WARNING_ZONE_NOT_LOADED);
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		handler->SendSysMessage(amas::LANG_AMAS_WARNING_ZONE_LIST);
        uint32 Count = 1;
        int8 locale_index = handler->GetSessionDbLocaleIndex();
        std::string ZoneName = "";

        AMAS::WarningZoneContainer& _WarningZone = sAMAS->GetWarningZone();
        for (auto i : _WarningZone)
        {
            if (Count == 50)
                break;

            AreaTableEntry const* zone = sAreaTableStore.LookupEntry(i);
            if (!zone)
                continue;

            ZoneName = zone->area_name[locale_index];

            if (ZoneName.empty())
                continue;

            handler->PSendSysMessage("%u. %u - %s", Count, i, ZoneName.c_str());

            Count++;
        }

        return true;
    }

    static bool HandleAMASZoneAdd(ChatHandler *handler, const char *args)
    {
        uint32 ZoneID = 0;

        if (!*args)
            ZoneID = handler->GetSession()->GetPlayer()->GetZoneId();
        else
            ZoneID = (uint32)atoi((char *)args);

        std::string ZoneName = "";

        AreaTableEntry const* zone = sAreaTableStore.LookupEntry(ZoneID);
        if (!zone)
        {
            handler->PSendSysMessage(amas::AMAS_ZONE_INVALID, ZoneID);
            handler->SetSentErrorMessage(true);
            return false;
        }
        else
            ZoneName = zone->area_name[handler->GetSessionDbLocaleIndex()];

        if (sAMAS->IsWarningZone(ZoneID))
        {
            handler->PSendSysMessage(amas::AMAS_WARNING_ZONE_EXIST, ZoneID, ZoneName.c_str());
            handler->SetSentErrorMessage(true);
            return false;
        }

        sAMAS->AddWarningZone(ZoneID, true);

        handler->PSendSysMessage(amas::AMAS_WARNING_ZONE_ADD, ZoneID, ZoneName.c_str());

        return true;
    }

    static bool HandleAMASZoneDelete(ChatHandler *handler, const char *args)
    {
        uint32 ZoneID = 0;

        if (!*args)
            ZoneID = handler->GetSession()->GetPlayer()->GetZoneId();
        else
            ZoneID = (uint32)atoi((char *)args);

        std::string ZoneName = "";

        AreaTableEntry const* zone = sAreaTableStore.LookupEntry(ZoneID);
        if (!zone)
        {
            handler->PSendSysMessage(amas::AMAS_ZONE_INVALID, ZoneID);
            handler->SetSentErrorMessage(true);
            return false;
        }
        else
            ZoneName = zone->area_name[handler->GetSessionDbLocaleIndex()];

        if (!sAMAS->IsWarningZone(ZoneID))
        {
            handler->PSendSysMessage(amas::AMAS_WARNING_ZONE_NOT_FOUND, ZoneID, ZoneName.c_str());
            handler->SetSentErrorMessage(true);
            return false;
        }

        sAMAS->DeleteWarningZone(ZoneID, true);

        handler->PSendSysMessage(amas::AMAS_WARNING_ZONE_DELETE, ZoneID, ZoneName.c_str());

        return true;
    }

    static bool HandleAMASZoneRelaod(ChatHandler *handler, const char* /*args*/)
    {
        sLog->outString("Reload warning zone for AMAS...");
        sAMAS->LoadWarningZone();
        handler->SendGlobalGMSysMessage("|cff6C8CD5#|cFFFF0000 DB table|r `amas_warning_zone` |cFFFF0000reloading|r");
        return true;
    }
    
    static bool HandleAMASInfo(ChatHandler *handler, const char *args)
    {
        Player* player;
        uint64 playerGUID;
        std::string PlayerName;
        if (!handler->extractPlayerTarget((char*)args, &player, &playerGUID, &PlayerName))
            return false;

        uint32 TotalTimeAccount, AVGILvl, FreeTalent, TotalRewardQuest, TotalTimePlayed, FriendCount, TotalMoney, TotalHonorPoint, TotalKill, MissingTrainerSpells, CurrentZone, ProfCount;
        std::string PlayerIP;

        if (player)
        {
            TotalTimeAccount = player->GetSession()->GetTotalTime();
            AVGILvl = sAMAS->GetAverageItemLevel(player);
            FreeTalent = player->GetFreeTalentPoints();
            TotalRewardQuest = player->GetRewardedQuestCount();
            TotalTimePlayed = player->GetTotalPlayedTime();
            FriendCount = sAMAS->GetFriendCount(player);
            TotalMoney = player->GetMoney();
            TotalHonorPoint = player->GetHonorPoints();
            TotalKill = player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS);
            PlayerIP = player->GetSession()->GetRemoteAddress();
            MissingTrainerSpells = sAMAS->GetMissingTrainerSpells(player);
            CurrentZone = player->GetZoneId();
            ProfCount = sAMAS->GetProfessionCount(player);
        }
        else
        {                                                    //         0                  1           2       3         4           5                6           7      8         9            10                11             12    
            QueryResult result = CharacterDatabase.PQuery("SELECT TotalTimeAccount, AverageItemLevel, IP, FriendCount, Money, CompletedQuests, TotalTimePlayed, Honor, Kills, CurrentZone, MissingSpells, ProfessionLearned, FreeTalent FROM `amas_player_info` WHERE PlayerGUID = %u", playerGUID);
            if (!result)
            {
                handler->PSendSysMessage(amas::AMAS_PLAYER_NOT_SAVED_DB, PlayerName.c_str());
                handler->SetSentErrorMessage(true);
                return false;
            }

            Field* field = result->Fetch();
            TotalTimeAccount = field[0].GetUInt32();
            AVGILvl = field[1].GetUInt32();
            FreeTalent = field[12].GetUInt32();
            TotalRewardQuest = field[5].GetUInt32();
            TotalTimePlayed = field[6].GetUInt32();
            FriendCount = field[3].GetUInt32();
            TotalMoney = field[4].GetUInt32();
            TotalHonorPoint = field[7].GetUInt32();
            TotalKill = field[8].GetUInt32();
            PlayerIP = field[2].GetString();
            MissingTrainerSpells = field[10].GetUInt32();
            CurrentZone = field[9].GetUInt32();
            ProfCount = field[11].GetUInt32();
        }

        uint32 gold = TotalMoney / GOLD;
        uint32 silv = (TotalMoney % GOLD) / SILVER;
        uint32 copp = (TotalMoney % GOLD) % SILVER;

        std::string TotalTimeAccountStr = secsToTimeString(TotalTimeAccount);
        std::string MoneyStr = std::to_string(gold) + "|TInterface\\MoneyFrame\\UI-GoldIcon:0:0:2:0|t " + std::to_string(silv) + "|TInterface\\MoneyFrame\\UI-SilverIcon:0:0:2:0|t " + std::to_string(copp) + "|TInterface\\MoneyFrame\\UI-CopperIcon:0:0:2:0|t ";
        std::string TotalTimePlayedStr = secsToTimeString(TotalTimePlayed);
        std::string IsWarningZone;
        std::string ZoneName;

        AreaTableEntry const* zone = sAreaTableStore.LookupEntry(CurrentZone);
        if (zone)
            ZoneName = zone->area_name[handler->GetSessionDbLocaleIndex()];

        if (sAMAS->IsWarningZone(CurrentZone))
            IsWarningZone = handler->GetTrinityString(amas::AMAS_IS_WARNING_ZONE);

        float WPTimeAcc = sAMAS->GetWPTotalTimeAccount(TotalTimeAccount);
        float WPAverageIlvl = sAMAS->GetWPAverageItemLevel(AVGILvl);
        float WPFreeTalent = sAMAS->GetWPFreeTalent(FreeTalent);
        float WPCompletedQuest = sAMAS->GetWPCompletedQuestCount(TotalRewardQuest);
        float WPFriend = sAMAS->GetWPFriend(FriendCount);
        float WPMoney = sAMAS->GetWPMoney(TotalMoney);
        float WPHonorAndKills = sAMAS->GetWPHonorAndKills(TotalHonorPoint, TotalKill);
        float WPTrainerSpells = sAMAS->GetWPTrainerSpells(MissingTrainerSpells);
        float WPWarningZone = sAMAS->GetWPWarningZone(CurrentZone);
        float WPProfession = sAMAS->GetWPProfession(ProfCount);
        float WPIp = sAMAS->GetWPIP(PlayerIP);

        float WPAll = WPTimeAcc + WPAverageIlvl + WPFreeTalent + WPCompletedQuest + WPFriend + WPMoney + WPHonorAndKills + WPTrainerSpells + WPWarningZone + WPProfession + WPIp;
        //float WPJoinAcc = sAMAS->GetWPJoinAccount(sAMAS->GetDateUnixJoinAccount(player->GetSession()->GetAccountId()));
        //float WPJoinChar = sAMAS->GetWPJoinCharacter(sAMAS->GetDateUnixJoinCharacter(playerGUID));

        handler->PSendSysMessage(amas::AMAS_INFO,
            PlayerName.c_str(), WPAll,
            WPTimeAcc, TotalTimeAccountStr.c_str(),
            WPAverageIlvl, AVGILvl,
            WPFreeTalent, FreeTalent,
            WPCompletedQuest, TotalRewardQuest,
            TotalTimePlayedStr.c_str(),
            WPFriend, FriendCount,
            WPMoney, MoneyStr.c_str(),
            WPHonorAndKills, TotalHonorPoint, TotalKill,
            WPIp, PlayerIP.c_str(),
            WPTrainerSpells, MissingTrainerSpells,
            WPWarningZone, CurrentZone, ZoneName.c_str(), IsWarningZone.c_str(),
            WPProfession, ProfCount);

        return true;
    }
};

void AddAMASScripts() 
{
    new AMAS_SC();
    new AMAS_SC_World();
    new CS_AMAS();
}
