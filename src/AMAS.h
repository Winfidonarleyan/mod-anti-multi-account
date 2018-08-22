#ifndef _AMAS_H_
#define _AMAS_H_

#include "Common.h"
#include "UnorderedMap.h"
#include <vector>
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
        float Profession;
        float JoinAccount;
        float JoinCharacter;
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
        PROFESSION,
        JOIN_ACC,
        JOIN_CHAR,

        MAX_CHECK_TYPE
    };

    enum Language
    {
        LANG_ANNOUNCE_GM = 40037,
        LANG_AMAS_PLAYER_OFFLINE,
        LANG_AMAS_INFO_PLAYER_WARNING,
        LANG_AMAS_WARNING_ZONE_LIST,
        LANG_AMAS_WARNING_ZONE_ENTER_ZONEID,
        LANG_AMAS_WARNING_ZONE_NOT_CORRECT_ZONEID,
        LANG_AMAS_WARNING_ZONE_ADD,
        LANG_AMAS_WARNING_ZONE_NOT_FOUND,
        LANG_AMAS_WARNING_ZONE_DELETE,
        LANG_AMAS_WARNING_ZONE_EXIST
    };
}

class AMAS
{
    friend class ACE_Singleton<AMAS, ACE_Null_Mutex>;

public:
    AMAS();
    ~AMAS();

    typedef UNORDERED_MAP<uint64, amas::WarningType> PlayerWarningPointContainer;
    typedef std::vector<uint32> WarningZoneContainer;

    PlayerWarningPointContainer _playerWarningPointStore;
    WarningZoneContainer _warningZoneStore;

    float GetWarningPoint(Player* player, amas::CheckType TypeCheck);
    float GetAllWarningPoint(Player* player);
    void AddWarningPoint(Player* player, amas::CheckType TypeCheck, float SetPointWarning);
    void ClearWarningPoint(Player* player);
    void StartCheck(Player* player);
    void LogoutPlayer(Player* player);
    void LoadWarningZone();
    bool IsWarningZone(uint32 ZoneID);
    void AddWarningZone(uint32 ZoneID, bool IsDB);
    void DeleteWarningZone(uint32 ZoneID, bool IsDB);

    WarningZoneContainer &GetWarningZone() { return _warningZoneStore; }

private:
    
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
    void CheckProfession(Player * player);
    void CheckJoinAccount(Player* player);
    void CheckJoinCharacter(Player* player);
    bool IsValidTime(Player* player);
	void PushDBPlayerInfo(Player* player);
};

#define sAMAS ACE_Singleton<AMAS, ACE_Null_Mutex>::instance()

#endif // _AMAS_H_
