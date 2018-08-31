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
        LANG_AMAS_ANNOUNCE_GM = 40037,
        LANG_AMAS_PLAYER_NOT_SAVED_DB,
        LANG_AMAS_INFO_PLAYER_WARNING_DETAIL,
        LANG_AMAS_INFO_PLAYER_WARNING,
        LANG_AMAS_WARNING_ZONE_LIST,
        LANG_AMAS_WARNING_ZONE_ENTER_ZONEID,
        LANG_AMAS_IS_LOW_GMLEVEL,
        LANG_AMAS_WARNING_ZONE_ADD,
        LANG_AMAS_WARNING_ZONE_NOT_FOUND,
        LANG_AMAS_WARNING_ZONE_DELETE,
        LANG_AMAS_WARNING_ZONE_EXIST,
		LANG_AMAS_WARNING_ZONE_NOT_LOADED,
        LANG_AMAS_ZONE_INVALID,
        LANG_AMAS_INFO_PLAYER_DETAIL,
        LANG_AMAS_IS_WARNING_ZONE
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
	
	uint32 GetFriendCount(Player* player);
    uint32 GetMissingTrainerSpells(Player* player);
    uint32 GetProfessionCount(Player* player);
	
	float GetAverageItemLevel(Player* player);
	
	bool IsWarningZoneExist();

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

namespace conf
{
    enum BoolConfigs
    {
        AMAS_ENABLE,
        AMAS_GM_CHECK_ENABLE,

        BOOL_VALUE_COUNT
    };

    enum FloatConfigs
    {
        AMAS_TEST, // not error :D

        FLOAT_VALUE_COUNT
    };

    enum IntConfigs
    {
        AMAS_MIN_TOTAL_TIME_ACC,
        AMAS_MIN_TOTAL_TIME_ACC_POINT,
        AMAS_MIN_AVG_ILVL,
        AMAS_MIN_AVG_ILVL_POINT,
        AMAS_FREE_TALENT_POINT,
        AMAS_MIN_COUNT_REWARDED_QUEST,
        AMAS_MIN_COUNT_REWARDED_QUEST_POINT,
        AMAS_MIN_COUNT_FRIEND,
        AMAS_MIN_COUNT_FRIEND_POINT,
        AMAS_MAX_COUNT_MONEY,
        AMAS_MAX_COUNT_MONEY_POINT,
        AMAS_NULL_HONOR_AND_KILLS,
        AMAS_MORE_IP_POINT,
        AMAS_MIN_TRAINER_SPELL_MISSING,
        AMAS_MISSING_TRAINER_SPELL_POINT,
        AMAS_WARNING_ZONE_POINT,
        AMAS_MIN_PROFESSION,
        AMAS_PROFESSION_POINT,
        AMAS_DIFF_ACC_CREATE,
        AMAS_DIFF_CHAR_CREATE,
        AMAS_DIFF_ACC_CREATE_POINT,
        AMAS_DIFF_CHAR_CREATE_POINT,
        AMAS_MIN_AVERAGE_SESSION,
        AMAS_MIN_AVERAGE_SESSION_POINT,

        // Amas Confirmed
        AMAS_CONFIRMED_MIN_POINT,
        AMAS_CONFIRMED_ACTION,
        AMAS_CONFIRMED_BAN_INTERVAL_MIN,
        AMAS_CONFIRMED_BAN_INTERVAL_MAX,

        // AMAS Other
        AMAS_SUSPICIOUS_ACCOUNT_MIN_POINT,
        AMAS_LOW_GMLEVEL,
        AMAS_HIGH_GMLEVEL,
        AMAS_WARNING_INTERVAL,
        AMAS_MIN_TIME_TO_DB_HISTORY,

        INT_VALUE_COUNT
    };

    enum StringConfigs
    {
        AMAS_BAN_REASON,

        STRING_VALUE_COUNT
    };
} // conf

class AMASConfig
{
    friend class ACE_Singleton<AMASConfig, ACE_Null_Mutex>;

public:

    bool GetBool(conf::BoolConfigs index) const
    {
        return index < conf::BOOL_VALUE_COUNT ? m_bool[index] : 0;
    }

    float GetFloat(conf::FloatConfigs index) const
    {
        return index < conf::FLOAT_VALUE_COUNT ? m_float[index] : 0;
    }

    uint32 GetInt(conf::IntConfigs index) const
    {
        return index < conf::INT_VALUE_COUNT ? m_int[index] : 0;
    }

    std::string GetString(conf::StringConfigs index) const
    {
        return index < conf::STRING_VALUE_COUNT ? m_string[index] : 0;
    }

    void LoadConfig();

private:
    bool m_bool[conf::BOOL_VALUE_COUNT];
    float m_float[conf::FLOAT_VALUE_COUNT];
    uint32 m_int[conf::INT_VALUE_COUNT];
    std::string m_string[conf::STRING_VALUE_COUNT];
};

#define sAMASConfig ACE_Singleton<AMASConfig, ACE_Null_Mutex>::instance()

#define CONF_BOOL sAMASConfig->GetBool
#define CONF_INT sAMASConfig->GetInt
#define CONF_STR sAMASConfig->GetString
#define CONF_FLOAT sAMASConfig->GetFloat

#endif // _AMAS_H_
