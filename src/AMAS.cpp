/**
    This plugin can be used for common player customizations
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Configuration/Config.h"
#include "AMAS.h"

#define LANG_ANNOUNCE_GLOBAL_GM_PLAYER_WARNING 40037

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

    uint32 PointWarning = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Trainer.Spells", 20);
    if (Count > 1)
        this->AddWarningPoint(player, amas::TRAINER_SPELLS, float(PointWarning));
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
        _playerWarningPointStore[player->GetGUID()].WarningZone;
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

    CharacterDatabase.PExecute("INSERT INTO `amas_player_rating_histoty`(`PlayerGUID`, `WarningPointAll`, `WarningPointTimeAcc`, `WarningPointAverageIlvl`, `WarningPointFreeTalent`, `WarningPointCompletedQuest`, `WarningPointFriend`, `WarningPointMoney`, `WarningPointHonorAndKills`, `WarningPointTrainerSpells`, `WarningPointIp`, `WarningPointWarningZone`, `Date`) VALUES (%u, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, NOW())",
        PlayerGUID, AllWarningPoint, WPTimeAcc, WPAverageIlvl, WPFreeTalent, WPCompletedQuest, WPFriend, WPMoney, WPHonorAndKills, WPTrainerSpells, WPIp, WPWarningZone);

    this->ClearWarningPoint(player);
}

void AMAS::ClearWarningPoint(Player * player)
{
    _playerWarningPointStore.erase(player->GetGUID());
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

        sAMAS->StartCheck(player);

        uint32 MinWaringPoint = sConfigMgr->GetIntDefault("AMAS.Min.Point.For.Warning.Account", 50);
        float PlayerWarningPoint = sAMAS->GetAllWarningPoint(player);

        if (PlayerWarningPoint > float(MinWaringPoint))
            sWorld->SendGMText(LANG_ANNOUNCE_GLOBAL_GM_PLAYER_WARNING, player->GetName().c_str(), PlayerWarningPoint);
    }

    void OnLogout(Player* player) override
    {
        if (!sConfigMgr->GetBoolDefault("AMAS.Enable", true))
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

void AddAMASScripts() 
{
    new AMAS_SC();
    new AMAS_SC_World();
}
