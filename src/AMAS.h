#ifndef _AMAS_H_
#define _AMAS_H_

#include "Common.h"
#include <ace/Singleton.h>

namespace amas
{
    struct WarningType
    {
        float TimeAccount;
        float AverageIlvl;
        float FreeTalent;
        float CompletedQuest;
        float Friend;
        float Money;
        float HonorAndKills;
        float TrainerSpells;
        float Ip;
        float WarningZone;
    };

    enum CheckType
    {
        TIME_ACCOUNT,
        AVERAGE_ITEM_LEVEL,
        FREE_TALENT,
        COMPLETED_QUEST,
        FRIEND,
        MONEY,
        HONOR_AND_KILLS,
        TRAINER_SPELLS,
        IP,
        WARNING_ZONE,

        MAX_CHECK_TYPE
    };
}

class AMAS
{
    friend class ACE_Singleton<AMAS, ACE_Null_Mutex>;

public:
    AMAS();
    ~AMAS();

    float GetWarningPoint(Player* player, amas::CheckType TypeCheck);
    float GetAllWarningPoint(Player* player);
    void AddWarningPoint(Player* player, amas::CheckType TypeCheck, float SetPointWarning);
    void ClearWarningPoint(Player* player);
    void StartCheck(Player* player);
    void LogoutPlayer(Player* player);
    void LoadWarningZone();

private:
    typedef UNORDERED_MAP<uint64, amas::WarningType> PlayerWarningPointContainer;
    typedef std::vector<uint32> WarningZoneContainer;

    PlayerWarningPointContainer _playerWarningPointStore;
    WarningZoneContainer _warningZoneStore;

    void CheckTotalTimeAccount(Player* player);
    void CheckAverageItemLevel(Player* player);
    void CheckFreeTalent(Player* player);
    void CheckCompletedQuestCount(Player* player);
    void CheckFriend(Player* player);
    void CheckMoney(Player* player);
    void CheckHonorAndKills(Player* player);
    void CheckIP(Player* player);
    void CheckTrainerSpells(Player* player);
    void CheckWarningZone(Player* player);
    bool IsWarningZone(uint32 ZoneID);
};

#define sAMAS ACE_Singleton<AMAS, ACE_Null_Mutex>::instance()

#endif // _AMAS_H_
