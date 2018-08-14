#ifndef _KARGATUM_AMAS_H_
#define _KARGATUM_AMAS_H_

#include "Common.h"
#include <ace/Singleton.h>

class KargatumAMAS
{
    friend class ACE_Singleton<KargatumAMAS, ACE_Null_Mutex>;

public:
    KargatumAMAS();
    ~KargatumAMAS();

    uint32 GetWarningPoint(Player* player);
    void AddWarningPoint(Player* player, uint32 SetPointWarning);
    void ClearWarningPoint(Player* player);
    void StartCheck(Player * player);

private:
    uint32 WarningPoint;
    typedef UNORDERED_MAP<uint64, uint32> WarningPointContainer;
    WarningPointContainer _warningPointStore;

    void CheckTotalTimeAccount(Player* player);
    void CheckAverageItemLevel(Player* player);
    void CheckFreeTalent(Player* player);
    void CheckRewardedQuestCount(Player* player);
    void CheckFriend(Player* player);
    void CheckMoney(Player* player);
    void CheckHonorAndKills(Player* player);
    void CheckIP(Player* player);    
};

#define sAMAS ACE_Singleton<KargatumAMAS, ACE_Null_Mutex>::instance()

#endif // _KARGATUM_AMAS_H_