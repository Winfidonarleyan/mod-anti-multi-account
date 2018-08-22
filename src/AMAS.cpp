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

void AMAS::CheckTotalTimeAccount(Player * player)
{
    float TotalTimeAccount = player->GetSession()->GetTotalTime();
    float MinTimeAccount = sConfigMgr->GetIntDefault("AMAS.Min.Total.Time.Account", DAY);
    float PointWarning = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Total.Time.Account", 10);

    float TotalPointWarning = (PointWarning / MinTimeAccount) / (TotalTimeAccount / DAY);

    this->AddWarningPoint(player, amas::TIME_ACCOUNT, TotalPointWarning);
}

void AMAS::CheckAverageItemLevel(Player * player)
{
    uint32 AVGILvl = player->GetAverageItemLevel();
    uint32 MinAVGILvl = sConfigMgr->GetIntDefault("AMAS.Min.Average.Ilvl", 50);
    uint32 PointWarning = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Average.Ilvl", 10);

    if (AVGILvl < MinAVGILvl)
        this->AddWarningPoint(player, amas::AVERAGE_ITEM_LEVEL, float(PointWarning));
}

void AMAS::CheckFreeTalent(Player * player)
{
    uint32 FreeTalent = player->GetFreeTalentPoints();
    uint32 PointWarning = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Free.Talent", 5);

    if (FreeTalent > 0)
        this->AddWarningPoint(player, amas::FREE_TALENT, float(PointWarning));
}

void AMAS::CheckCompletedQuestCount(Player * player)
{
    uint32 TotalRewardQuest = player->GetRewardedQuestCount();
    uint32 MinQuestCount = sConfigMgr->GetIntDefault("AMAS.Min.Quest.Rewarded.Count", 20);
    uint32 PointWarning = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Quest.Rewarded.Count", 10);

    if (TotalRewardQuest < MinQuestCount)
        this->AddWarningPoint(player, amas::COMPLETED_QUEST, float(PointWarning));
}

void AMAS::CheckFriend(Player * player)
{
    uint32 FriendCount = 0;

    QueryResult result = CharacterDatabase.PQuery("SELECT COUNT(*) FROM character_social JOIN characters ON characters.guid = character_social.friend WHERE character_social.guid = %u AND deleteinfos_name IS NULL LIMIT 255;", player->GetGUID());
    if (result)
        FriendCount = result->Fetch()->GetUInt32();

    uint32 MinFriendCount = sConfigMgr->GetIntDefault("AMAS.Min.Friend.Count", 2);
    uint32 PointWarning = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Min.Friend", 5);

    if (FriendCount < MinFriendCount)
        this->AddWarningPoint(player, amas::FRIEND, float(PointWarning));
}

void AMAS::CheckMoney(Player * player)
{
    uint32 TotalMoney = player->GetMoney();
    uint32 MaxMoneyCount = sConfigMgr->GetIntDefault("AMAS.Max.Count.Money", 100000) * GOLD;
    uint32 PointWarning = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Max.Count.Money", 5);

    if (TotalMoney > MaxMoneyCount)
        this->AddWarningPoint(player, amas::MONEY, float(PointWarning));
}

void AMAS::CheckHonorAndKills(Player * player)
{
    uint32 TotalHonorPoint = player->GetHonorPoints();
    uint32 TotalKill = player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS);
    uint32 PointWarning = sConfigMgr->GetIntDefault("AMAS.Warning.Point.NULL.Honor.And.Kills", 10);

    if (!TotalHonorPoint && !TotalKill)
        this->AddWarningPoint(player, amas::HONOR_AND_KILLS, float(PointWarning));
}

void AMAS::CheckIP(Player * player)
{
    uint8 IPCount = 0;
    std::string PlayerIP = player->GetSession()->GetRemoteAddress();
    uint32 PointWarning = sConfigMgr->GetIntDefault("AMAS.Warning.Point.More.IP", 20);

    SessionMap::iterator itr;
    SessionMap m_sessions = sWorld->GetAllSessions();
    for (itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
    {
        if (!itr->second && !itr->second->GetPlayer() && !itr->second->GetPlayer()->IsInWorld())
            continue;

        if (itr->second->GetRemoteAddress() == PlayerIP)
            IPCount++;
    }

    if (IPCount > 1)
        this->AddWarningPoint(player, amas::IP, float(PointWarning));
}

void AMAS::CheckTrainerSpells(Player * player)
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
        0, // UNKNOWN class in 3.3.5a
        3033, // Druid
    };

    uint8 PlayerClass = player->getClass();
    int32 Count = 0;

    if (auto spells = sObjectMgr->GetNpcTrainerSpells(NpcTrainers[PlayerClass]))
    {
        for (auto itr = spells->spellList.begin(); itr != spells->spellList.end(); itr++)
        {
            auto spell = &itr->second;

            if (spell->spell > 0 && player->GetTrainerSpellState(spell) == TRAINER_SPELL_GREEN && !player->HasSpell(spell->spell))
                Count++;
        }
    }

    uint32 PointWarning = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Missing.One.Trainer.Spell", 5);
    uint32 MinMissingSpells = sConfigMgr->GetIntDefault("AMAS.Min.Missing.Trainer.Spells", 5);

    if (Count > MinMissingSpells)
        this->AddWarningPoint(player, amas::TRAINER_SPELLS, float(PointWarning * Count));
}

void AMAS::CheckWarningZone(Player * player)
{
    uint32 PointWarning = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Warning.Zone", 20);

    if (this->IsWarningZone(player->GetZoneId()))
        this->AddWarningPoint(player, amas::WARNING_ZONE, float(PointWarning));
}

void AMAS::LoadWarningZone()
{
    if (!sConfigMgr->GetBoolDefault("AMAS.Enable", true))
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

    sLog->outString(">> Loaded warning zones %i in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
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

void AMAS::CheckProfession(Player * player)
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

    uint32 PointWarning = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Professions", 10);
    uint32 MinProf = sConfigMgr->GetIntDefault("AMAS.Min.Profession", 1);

    if (ProfCount < MinProf)
        this->AddWarningPoint(player, amas::PROFESSION, float(PointWarning));
}

void AMAS::CheckJoinAccount(Player * player)
{
    uint32 AccoutCreateDateUnix = 0;

    QueryResult result = LoginDatabase.PQuery("SELECT UNIX_TIMESTAMP(joindate) FROM account WHERE id = %u", player->GetSession()->GetAccountId());
    if (result)
        AccoutCreateDateUnix = result->Fetch()->GetUInt32();

    uint32 MinDiff = sConfigMgr->GetIntDefault("AMAS.Min.Diff.Account.Create", DAY);
    uint32 PointWarning = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Diff.Account", 10);

    if (AccoutCreateDateUnix < MinDiff)
        this->AddWarningPoint(player, amas::JOIN_ACC, float(PointWarning));
}

void AMAS::CheckJoinCharacter(Player * player)
{
    uint32 CharacterCreateDateUnix = 0;

    QueryResult result = CharacterDatabase.PQuery("SELECT UNIX_TIMESTAMP(create_date) FROM characters WHERE guid = %u", player->GetGUID());
    if (result)
        CharacterCreateDateUnix = result->Fetch()->GetUInt32();

    uint32 MinDiff = sConfigMgr->GetIntDefault("AMAS.Min.Diff.Character.Create", DAY);
    uint32 PointWarning = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Diff.Character", 10);

    if (CharacterCreateDateUnix < MinDiff)
        this->AddWarningPoint(player, amas::JOIN_CHAR, float(PointWarning));
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

void AMAS::StartCheck(Player * player)
{
    if (!sConfigMgr->GetBoolDefault("AMAS.Enable", true))
        return;

    this->CheckTotalTimeAccount(player);
    this->CheckAverageItemLevel(player);
    this->CheckFreeTalent(player);
    this->CheckCompletedQuestCount(player);
    this->CheckFriend(player);
    this->CheckMoney(player);
    this->CheckHonorAndKills(player);
    this->CheckIP(player);
    this->CheckTrainerSpells(player);
    this->CheckWarningZone(player);
    this->CheckProfession(player);
    this->CheckJoinAccount(player);
    this->CheckJoinCharacter(player);
}

float AMAS::GetWarningPoint(Player * player, amas::CheckType TypeCheck)
{
    switch (TypeCheck)
    {
    case amas::TIME_ACCOUNT:
        return _playerWarningPointStore[player->GetGUID()].TimeAccount;
        break;
    case amas::AVERAGE_ITEM_LEVEL:
        return _playerWarningPointStore[player->GetGUID()].AverageIlvl;
        break;
    case amas::FREE_TALENT:
        return _playerWarningPointStore[player->GetGUID()].FreeTalent;
        break;
    case amas::COMPLETED_QUEST:
        return _playerWarningPointStore[player->GetGUID()].CompletedQuest;
        break;
    case amas::FRIEND:
        return _playerWarningPointStore[player->GetGUID()].Friend;
        break;
    case amas::MONEY:
        return _playerWarningPointStore[player->GetGUID()].Money;
        break;
    case amas::HONOR_AND_KILLS:
        return _playerWarningPointStore[player->GetGUID()].HonorAndKills;
        break;
    case amas::TRAINER_SPELLS:
        return _playerWarningPointStore[player->GetGUID()].TrainerSpells;
        break;
    case amas::IP:
        return _playerWarningPointStore[player->GetGUID()].Ip;
        break;
    case amas::WARNING_ZONE:
        return _playerWarningPointStore[player->GetGUID()].WarningZone;
        break;
    case amas::PROFESSION:
        return _playerWarningPointStore[player->GetGUID()].Profession;
        break;
    case amas::JOIN_ACC:
        return _playerWarningPointStore[player->GetGUID()].JoinAccount;
        break;
    case amas::JOIN_CHAR:
        return _playerWarningPointStore[player->GetGUID()].JoinCharacter;
        break;
    default:
        return 0.0f;
        break;
    }
}

float AMAS::GetAllWarningPoint(Player * player)
{
    return  _playerWarningPointStore[player->GetGUID()].TimeAccount +
        _playerWarningPointStore[player->GetGUID()].AverageIlvl +
        _playerWarningPointStore[player->GetGUID()].FreeTalent +
        _playerWarningPointStore[player->GetGUID()].CompletedQuest +
        _playerWarningPointStore[player->GetGUID()].Friend +
        _playerWarningPointStore[player->GetGUID()].Money +
        _playerWarningPointStore[player->GetGUID()].HonorAndKills +
        _playerWarningPointStore[player->GetGUID()].TrainerSpells +
        _playerWarningPointStore[player->GetGUID()].Ip +
        _playerWarningPointStore[player->GetGUID()].WarningZone +
        _playerWarningPointStore[player->GetGUID()].Profession +
        _playerWarningPointStore[player->GetGUID()].JoinAccount +
        _playerWarningPointStore[player->GetGUID()].JoinCharacter;
}

void AMAS::AddWarningPoint(Player * player, amas::CheckType TypeCheck, float SetPointWarning)
{
    if (TypeCheck > amas::CheckType::MAX_CHECK_TYPE)
        return;

    switch (TypeCheck)
    {
    case amas::TIME_ACCOUNT:
        _playerWarningPointStore[player->GetGUID()].TimeAccount = _playerWarningPointStore[player->GetGUID()].TimeAccount + SetPointWarning;
        break;
    case amas::AVERAGE_ITEM_LEVEL:
        _playerWarningPointStore[player->GetGUID()].AverageIlvl = _playerWarningPointStore[player->GetGUID()].AverageIlvl + SetPointWarning;
        break;
    case amas::FREE_TALENT:
        _playerWarningPointStore[player->GetGUID()].FreeTalent = _playerWarningPointStore[player->GetGUID()].FreeTalent + SetPointWarning;
        break;
    case amas::COMPLETED_QUEST:
        _playerWarningPointStore[player->GetGUID()].CompletedQuest = _playerWarningPointStore[player->GetGUID()].CompletedQuest + SetPointWarning;
        break;
    case amas::FRIEND:
        _playerWarningPointStore[player->GetGUID()].Friend = _playerWarningPointStore[player->GetGUID()].Friend + SetPointWarning;
        break;
    case amas::MONEY:
        _playerWarningPointStore[player->GetGUID()].Money = _playerWarningPointStore[player->GetGUID()].Money + SetPointWarning;
        break;
    case amas::HONOR_AND_KILLS:
        _playerWarningPointStore[player->GetGUID()].HonorAndKills = _playerWarningPointStore[player->GetGUID()].HonorAndKills + SetPointWarning;
        break;
    case amas::TRAINER_SPELLS:
        _playerWarningPointStore[player->GetGUID()].TrainerSpells = _playerWarningPointStore[player->GetGUID()].TrainerSpells + SetPointWarning;
        break;
    case amas::IP:
        _playerWarningPointStore[player->GetGUID()].Ip = _playerWarningPointStore[player->GetGUID()].Ip + SetPointWarning;
        break;
    case amas::WARNING_ZONE:
        _playerWarningPointStore[player->GetGUID()].WarningZone = _playerWarningPointStore[player->GetGUID()].WarningZone + SetPointWarning;
        break;
    case amas::PROFESSION:
        _playerWarningPointStore[player->GetGUID()].Profession = _playerWarningPointStore[player->GetGUID()].Profession + SetPointWarning;
        break;
    case amas::JOIN_ACC:
        _playerWarningPointStore[player->GetGUID()].JoinAccount = _playerWarningPointStore[player->GetGUID()].JoinAccount + SetPointWarning;
        break;
    case amas::JOIN_CHAR:
        _playerWarningPointStore[player->GetGUID()].JoinCharacter = _playerWarningPointStore[player->GetGUID()].JoinCharacter + SetPointWarning;
        break;
    default:
        break;
    }
}

void AMAS::LogoutPlayer(Player * player)
{
    uint64 PlayerGUID = player->GetGUID();
    float AllWarningPoint = GetAllWarningPoint(player);
    float WPTimeAcc = this->GetWarningPoint(player, amas::TIME_ACCOUNT);
    float WPAverageIlvl = this->GetWarningPoint(player, amas::AVERAGE_ITEM_LEVEL);
    float WPFreeTalent = this->GetWarningPoint(player, amas::FREE_TALENT);
    float WPCompletedQuest = this->GetWarningPoint(player, amas::COMPLETED_QUEST);
    float WPFriend = this->GetWarningPoint(player, amas::FRIEND);
    float WPMoney = this->GetWarningPoint(player, amas::MONEY);
    float WPHonorAndKills = this->GetWarningPoint(player, amas::HONOR_AND_KILLS);
    float WPTrainerSpells = this->GetWarningPoint(player, amas::TRAINER_SPELLS);
    float WPIp = this->GetWarningPoint(player, amas::IP);
    float WPWarningZone = this->GetWarningPoint(player, amas::WARNING_ZONE);
    float WPProfession = this->GetWarningPoint(player, amas::PROFESSION);
    float WPJoinAcc = this->GetWarningPoint(player, amas::JOIN_ACC);
    float WPJoinChar = this->GetWarningPoint(player, amas::JOIN_CHAR);

    this->ClearWarningPoint(player);

    if (AllWarningPoint < sConfigMgr->GetIntDefault("AMAS.Suspicious.Account.Min.Points", 15))
        return;

    if (!this->IsValidTime(player))
        return;

    CharacterDatabase.PExecute("INSERT INTO `amas_player_rating_history`(`PlayerGUID`, `WarningPointAll`, `WarningPointTimeAcc`, `WarningPointAverageIlvl`, `WarningPointFreeTalent`, `WarningPointCompletedQuest`, `WarningPointFriend`, `WarningPointMoney`, `WarningPointHonorAndKills`, `WarningPointTrainerSpells`, `WarningPointIp`, `WarningPointWarningZone`, `WarningPointProfession`, `WarningPointJoinAccount`, `WarningPointJoinCharacter`, `Date`) VALUES (%u, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, NOW())",
        PlayerGUID, AllWarningPoint, WPTimeAcc, WPAverageIlvl, WPFreeTalent, WPCompletedQuest, WPFriend, WPMoney, WPHonorAndKills, WPTrainerSpells, WPIp, WPWarningZone, WPProfession, WPJoinAcc, WPJoinChar);
}

bool AMAS::IsValidTime(Player* player)
{
    uint32 LastDate = 0;

    QueryResult result = CharacterDatabase.PQuery("SELECT UNIX_TIMESTAMP(Date) FROM `amas_player_rating_history` WHERE PlayerGUID = %u ORDER BY `Date` DESC LIMIT 0, 1", player->GetGUID());
    if (result)
        LastDate = result->Fetch()->GetUInt32();

    uint32 TimeNow = uint32(time(NULL));
    uint32 Diff = TimeNow - LastDate;
    uint32 MinDiff = sConfigMgr->GetIntDefault("AMAS.Min.Time.For.DB.Histoty", DAY / 2);

    if (Diff > MinDiff)
        return true;

    return false;
}

void AMAS::ClearWarningPoint(Player * player)
{
    _playerWarningPointStore.erase(player->GetGUID());
}

void AMAS::PushDBPlayerInfo(Player* player)
{
    uint64 PlayerGUID = player->GetGUID();
    uint32 TotalTimeAccount = player->GetSession()->GetTotalTime();
    uint32 AVGILvl = player->GetAverageItemLevel();
    uint32 FreeTalent = player->GetFreeTalentPoints();
    uint32 TotalRewardQuest = player->GetRewardedQuestCount();
    uint32 TotalTimePlayed = player->GetTotalPlayedTime();

    uint32 FriendCount = 0;

    QueryResult result = CharacterDatabase.PQuery("SELECT COUNT(*) FROM character_social JOIN characters ON characters.guid = character_social.friend WHERE character_social.guid = %u AND deleteinfos_name IS NULL LIMIT 255", player->GetGUID());
    if (result)
        FriendCount = result->Fetch()->GetUInt32();

    uint32 TotalMoney = player->GetMoney();
    uint32 TotalHonorPoint = player->GetHonorPoints();
    uint32 TotalKill = player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS);
    std::string PlayerIP = player->GetSession()->GetRemoteAddress();

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

    uint32 CurrentZone = player->GetZoneId();
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

    CharacterDatabase.PExecute("DELETE FROM `amas_player_info` WHERE `PlayerGUID` = %u", PlayerGUID);
    CharacterDatabase.PExecute("INSERT INTO `amas_player_info`(`PlayerGUID`, `TotalTimeAccount`, `AverageItemLevel`, `IP`, `FriendCount`, `Money`, `CompletedQuests`, `TotalTimePlayed`, `Honor`, `Kills`, `CurrentZone`, `MissingSpells`, `ProfessionLearned`, `FreeTalent`, `DateCheck`) VALUES (%u, %u, %u, '%s', %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, NOW())",
        PlayerGUID, TotalTimeAccount, AVGILvl, PlayerIP.c_str(), FriendCount, TotalMoney, TotalRewardQuest, TotalTimePlayed, TotalHonorPoint, TotalKill, CurrentZone, MissingTrainerSpells, ProfCount, FreeTalent);
}

// AMAS SC
class AMAS_SC : public PlayerScript
{
public:
    AMAS_SC() : PlayerScript("AMAS_SC") { }    

    void OnLogin(Player* player) override
    {
        if (!sConfigMgr->GetBoolDefault("AMAS.Enable", true))
            return;

        if (!(sConfigMgr->GetBoolDefault("AMAS.GM.Check.Enable", true) && !AccountMgr::IsPlayerAccount(player->GetSession()->GetSecurity())))
            return;

        sAMAS->StartCheck(player);

        uint32 MinWaringPoint = sConfigMgr->GetIntDefault("AMAS.Min.Point.For.Warning.Account", 50);
        float PlayerWarningPoint = sAMAS->GetAllWarningPoint(player);

        if (PlayerWarningPoint > float(MinWaringPoint))
            sWorld->SendGMText(amas::LANG_ANNOUNCE_GM, player->GetName().c_str(), PlayerWarningPoint);
    }

    void OnLogout(Player* player) override
    {
        if (!sConfigMgr->GetBoolDefault("AMAS.Enable", true))
            return;

        if (!(sConfigMgr->GetBoolDefault("AMAS.GM.Check.Enable", true) && !AccountMgr::IsPlayerAccount(player->GetSession()->GetSecurity())))
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
        if (!sConfigMgr->GetBoolDefault("AMAS.Enable", true))
            return;

        sLog->outString("Loading warning zone for AMAS");
        sAMAS->LoadWarningZone();
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

        static std::vector<ChatCommand> TableCommandAmasInfo = // .amas info (not work)
        {
            /*{ "account",	        SEC_ADMINISTRATOR,  	true, &HandleAMASInfoAccount, 		   	            "" },*/
            { "player",	    	    SEC_ADMINISTRATOR,  	true, &HandleAMASInfoPlayer, 		   	            "" }
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
            { "zone",				SEC_ADMINISTRATOR, 		true, nullptr,             	   						"", TableCommandAmasZone },
            { "info",				SEC_ADMINISTRATOR, 		true, nullptr,             	   						"", TableCommandAmasInfo },
            { "comment",			SEC_ADMINISTRATOR, 		true, nullptr,             	   						"", TableCommandAmasComment },
            { "list",			    SEC_ADMINISTRATOR, 		true, nullptr,             	   						"", TableCommandAmasList }
        };

        static std::vector<ChatCommand> commandTable =
        {  
            { "amas",				SEC_ADMINISTRATOR, 		true, nullptr,             	   				    "", TableCommandAmas }
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

        uint32 ZoneID = (uint32)atoi((char *)args);
        std::string ZoneName = "";

        AreaTableEntry const* zone = sAreaTableStore.LookupEntry(ZoneID);
        if (!zone)
        {
            handler->PSendSysMessage(amas::LANG_AMAS_WARNING_ZONE_NOT_CORRECT_ZONEID, ZoneID);
            handler->SetSentErrorMessage(true);
            return false;
        }
        else
            ZoneName = zone->area_name[handler->GetSessionDbLocaleIndex()];

        if (sAMAS->IsWarningZone(ZoneID))
        {
            handler->PSendSysMessage(amas::LANG_AMAS_WARNING_ZONE_EXIST, ZoneID, ZoneName.c_str());
            handler->SetSentErrorMessage(true);
            return false;
        }

        sAMAS->AddWarningZone(ZoneID, true);

        handler->PSendSysMessage(amas::LANG_AMAS_WARNING_ZONE_ADD, ZoneID, ZoneName.c_str());

        return true;
    }

    static bool HandleAMASZoneDelete(ChatHandler *handler, const char *args)
    {
        uint32 ZoneID = 0;

        if (!*args)
            ZoneID = handler->GetSession()->GetPlayer()->GetZoneId();
        else
            ZoneID = (uint32)atoi((char *)args);

        uint32 ZoneID = (uint32)atoi((char *)args);
        std::string ZoneName = "";

        AreaTableEntry const* zone = sAreaTableStore.LookupEntry(ZoneID);
        if (!zone)
        {
            handler->PSendSysMessage(amas::LANG_AMAS_WARNING_ZONE_NOT_CORRECT_ZONEID, ZoneID);
            handler->SetSentErrorMessage(true);
            return false;
        }
        else
            ZoneName = zone->area_name[handler->GetSessionDbLocaleIndex()];

        if (!sAMAS->IsWarningZone(ZoneID))
        {
            handler->PSendSysMessage(amas::LANG_AMAS_WARNING_ZONE_NOT_FOUND, ZoneID, ZoneName.c_str());
            handler->SetSentErrorMessage(true);
            return false;
        }

        sAMAS->DeleteWarningZone(ZoneID, true);

        handler->PSendSysMessage(amas::LANG_AMAS_WARNING_ZONE_DELETE, ZoneID, ZoneName.c_str());

        return true;
    }

    static bool HandleAMASZoneRelaod(ChatHandler *handler, const char* /*args*/)
    {
        sLog->outString("Reload warning zone for AMAS...");
        sAMAS->LoadWarningZone();
        handler->SendGlobalGMSysMessage("|cff6C8CD5#|cFFFF0000 DB table|r `amas_warning_zone` |cFFFF0000reloading|r");
        return true;
    }

    static bool HandleAMASInfoPlayer(ChatHandler *handler, const char *args)
    {
        Player* player;
        uint64 playerGUID;
        std::string PlayerName;
        if (!handler->extractPlayerTarget((char*)args, &player, &playerGUID, &PlayerName))
            return false;

        float AllWarningPoint, WPTimeAcc, WPAverageIlvl, WPFreeTalent, WPCompletedQuest, WPFriend, WPMoney, WPHonorAndKills, WPTrainerSpells, WPWarningZone, WPProfession, WPIp, WPJoinAcc, WPJoinChar;

        if (player)
        {
            AllWarningPoint = sAMAS->GetAllWarningPoint(player);
            WPTimeAcc = sAMAS->GetWarningPoint(player, amas::TIME_ACCOUNT);
            WPAverageIlvl = sAMAS->GetWarningPoint(player, amas::AVERAGE_ITEM_LEVEL);
            WPFreeTalent = sAMAS->GetWarningPoint(player, amas::FREE_TALENT);
            WPCompletedQuest = sAMAS->GetWarningPoint(player, amas::COMPLETED_QUEST);
            WPFriend = sAMAS->GetWarningPoint(player, amas::FRIEND);
            WPMoney = sAMAS->GetWarningPoint(player, amas::MONEY);
            WPHonorAndKills = sAMAS->GetWarningPoint(player, amas::HONOR_AND_KILLS);
            WPTrainerSpells = sAMAS->GetWarningPoint(player, amas::TRAINER_SPELLS);
            WPWarningZone = sAMAS->GetWarningPoint(player, amas::WARNING_ZONE);
            WPProfession = sAMAS->GetWarningPoint(player, amas::PROFESSION);
            WPIp = sAMAS->GetWarningPoint(player, amas::IP);
            WPJoinAcc = sAMAS->GetWarningPoint(player, amas::JOIN_ACC);
            WPJoinChar = sAMAS->GetWarningPoint(player, amas::JOIN_CHAR);
        }
        else
        {                                                    //         0                    1                     2                        3                          4                       5                  6                     7                            8                    9                  10                       11                      12                        13                              
            QueryResult result = CharacterDatabase.PQuery("SELECT WarningPointAll, WarningPointTimeAcc, WarningPointAverageIlvl, WarningPointFreeTalent, WarningPointCompletedQuest, WarningPointFriend, WarningPointMoney, WarningPointHonorAndKills, WarningPointTrainerSpells, WarningPointIp, WarningPointWarningZone, WarningPointProfession, WarningPointJoinAccount, WarningPointJoinCharacter FROM `amas_player_rating_history` WHERE PlayerGUID = %u ORDER BY `Date` DESC LIMIT 0, 1", playerGUID);
            if (!result)
            {
                handler->PSendSysMessage(amas::LANG_AMAS_PLAYER_OFFLINE, PlayerName.c_str());
                handler->SetSentErrorMessage(true);
                return false;
            }

            Field* field = result->Fetch();
            AllWarningPoint = field[0].GetFloat();
            WPTimeAcc = field[1].GetFloat();
            WPAverageIlvl = field[2].GetFloat();
            WPFreeTalent = field[3].GetFloat();
            WPCompletedQuest = field[4].GetFloat();
            WPFriend = field[5].GetFloat();
            WPMoney = field[6].GetFloat();
            WPHonorAndKills = field[7].GetFloat();
            WPTrainerSpells = field[8].GetFloat();
            WPIp = field[9].GetFloat();
            WPWarningZone = field[10].GetFloat();
            WPProfession = field[11].GetFloat();
            WPJoinAcc = field[12].GetFloat();
            WPJoinChar = field[13].GetFloat();
        }

        handler->PSendSysMessage(amas::LANG_AMAS_INFO_PLAYER_WARNING,
            PlayerName.c_str(), AllWarningPoint, WPTimeAcc, WPAverageIlvl, WPFreeTalent, WPCompletedQuest, WPFriend, WPMoney, WPHonorAndKills, WPTrainerSpells, WPWarningZone, WPIp, WPProfession, WPJoinAcc, WPJoinChar);

        return true;
    }
};

void AddAMASScripts() 
{
    new AMAS_SC();
    new AMAS_SC_World();
    new CS_AMAS();
}
