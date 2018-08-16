#ifndef _AMAS_H_
#define _AMAS_H_

#include "Common.h"
#include <ace/Singleton.h>

class AMAS
{
    friend class ACE_Singleton<AMAS, ACE_Null_Mutex>;

public:
    AMAS();
    ~AMAS();

    uint32 GetWarningPoint(Player* player);
    void AddWarningPoint(Player* player, uint32 SetPointWarning);
    void ClearWarningPoint(Player* player);
    void StartCheck(Player* player);
	void LoadWarningZone();

private:
    typedef UNORDERED_MAP<uint64, uint32> WarningPointContainer;
	typedef std::vector<uint32> WarningZoneContainer;
    WarningPointContainer _warningPointStore;
	WarningZoneContainer _warningZoneStore;

    void CheckTotalTimeAccount(Player* player);
    void CheckAverageItemLevel(Player* player);
    void CheckFreeTalent(Player* player);
    void CheckRewardedQuestCount(Player* player);
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