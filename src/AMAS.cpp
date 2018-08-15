/**
    This plugin can be used for common player customizations
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Configuration/Config.h"
#include "AMAS.h"

#define KARGATUM_LANG_ANNOUNCE_GLOBAL_GM_PLAYER_WARNING 40037

KargatumAMAS::KargatumAMAS()
{
    _warningPointStore.clear();
}

KargatumAMAS::~KargatumAMAS()
{
    while (!_warningPointStore.empty())
    {
        _warningPointStore.erase(_warningPointStore.begin());
    }
}

void KargatumAMAS::CheckTotalTimeAccount(Player * player)
{
    uint32 TotalTimeAccount = player->GetSession()->GetTotalTime();
    uint32 MinTimeAccount = sConfigMgr->GetIntDefault("AMAS.Min.Total.Time.Account", DAY);
    uint32 PointWarning = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Total.Time.Account", 10);

    if (TotalTimeAccount < MinTimeAccount)
        this->AddWarningPoint(player, PointWarning);
}

void KargatumAMAS::CheckAverageItemLevel(Player * player)
{
    uint32 AVGILvl = player->GetAverageItemLevel();
    uint32 MinAVGILvl = sConfigMgr->GetIntDefault("AMAS.Min.Average.Ilvl", 50);
    uint32 PointWarning = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Average.Ilvl", 10);

    if (AVGILvl < MinAVGILvl)
        this->AddWarningPoint(player, PointWarning);
}

void KargatumAMAS::CheckFreeTalent(Player * player)
{
    uint32 FreeTalent = player->GetFreeTalentPoints();
    uint32 PointWarning = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Free.Talent", 5);

    if (FreeTalent > 0)
        this->AddWarningPoint(player, PointWarning);
}

void KargatumAMAS::CheckRewardedQuestCount(Player * player)
{
    uint32 TotalRewardQuest = player->GetRewardedQuestCount();
    uint32 MinQuestCount = sConfigMgr->GetIntDefault("AMAS.Min.Quest.Rewarded.Count", 20);
    uint32 PointWarning = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Quest.Rewarded.Count", 10);

    if (TotalRewardQuest < MinQuestCount)
        this->AddWarningPoint(player, PointWarning);
}

void KargatumAMAS::CheckFriend(Player * player)
{
    uint32 FriendCount = 0;

    QueryResult result = CharacterDatabase.PQuery("SELECT COUNT(*) FROM character_social JOIN characters ON characters.guid = character_social.friend WHERE character_social.guid = %u AND deleteinfos_name IS NULL LIMIT 255;", player->GetGUID());
    if (result)
        FriendCount = result->Fetch()->GetUInt32();

    uint32 MinFriendCount = sConfigMgr->GetIntDefault("AMAS.Min.Friend.Count", 2);
    uint32 PointWarning = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Min.Friend", 5);

    if (FriendCount < MinFriendCount)
        this->AddWarningPoint(player, PointWarning);
}

void KargatumAMAS::CheckMoney(Player * player)
{
    uint32 TotalMoney = player->GetMoney();
    uint32 MaxMoneyCount = sConfigMgr->GetIntDefault("AMAS.Max.Count.Money", 100000) * GOLD;
    uint32 PointWarning = sConfigMgr->GetIntDefault("AMAS.Warning.Point.Max.Count.Money", 5);

    if (TotalMoney > MaxMoneyCount)
        this->AddWarningPoint(player, PointWarning);
}

void KargatumAMAS::CheckHonorAndKills(Player * player)
{
    uint32 TotalHonorPoint = player->GetHonorPoints();
    uint32 TotalKill = player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS);
    uint32 PointWarning = sConfigMgr->GetIntDefault("AMAS.Warning.Point.NULL.Honor.And.Kills", 10);

    if (!TotalHonorPoint && !TotalKill)
        this->AddWarningPoint(player, PointWarning);
}

void KargatumAMAS::CheckIP(Player * player)
{
    uint8 IPCount = 1;
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
        this->AddWarningPoint(player, PointWarning);
}

void KargatumAMAS::StartCheck(Player * player)
{
    if (!sConfigMgr->GetBoolDefault("AMAS.Enable", true))
        return;

    this->CheckTotalTimeAccount(player);
    this->CheckAverageItemLevel(player);
    this->CheckFreeTalent(player);
    this->CheckRewardedQuestCount(player);
    this->CheckFriend(player);
    this->CheckMoney(player);
    this->CheckHonorAndKills(player);
    this->CheckIP(player);
}

uint32 KargatumAMAS::GetWarningPoint(Player * player)
{
    WarningPointContainer::const_iterator itr = _warningPointStore.find(player->GetGUID());
    if (itr != _warningPointStore.end())
        return itr->second;

    return 0;
}

void KargatumAMAS::AddWarningPoint(Player * player, uint32 SetPointWarning)
{
    _warningPointStore[player->GetGUID()] = _warningPointStore[player->GetGUID()] + SetPointWarning;
}

void KargatumAMAS::ClearWarningPoint(Player * player)
{
    _warningPointStore.erase(player->GetGUID());
}

// AMAS SC
class Anti_Multi_Account_System : public PlayerScript
{
public:
    Anti_Multi_Account_System() : PlayerScript("Anti_Multi_Account_System") { }    

    void OnLogin(Player* player) override
    {
        if (!sConfigMgr->GetBoolDefault("AMAS.Enable", true))
            return;

        sAMAS->StartCheck(player);

        uint32 MinWaringPoint = sConfigMgr->GetIntDefault("AMAS.Min.Point.For.Warning.Account", 50);
        uint32 PlayerWarningPoint = sAMAS->GetWarningPoint(player);

        if (PlayerWarningPoint > MinWaringPoint)
            sWorld->SendGMText(KARGATUM_LANG_ANNOUNCE_GLOBAL_GM_PLAYER_WARNING, player->GetName().c_str(), PlayerWarningPoint);
    }

    void OnLogout(Player* player) override
    {
        if (!sConfigMgr->GetBoolDefault("AMAS.Enable", true))
            return;

        sAMAS->ClearWarningPoint(player);
    }
};

void AddAMASScripts() 
{
    new Anti_Multi_Account_System();
}
