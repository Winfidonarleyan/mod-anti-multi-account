#ifndef _AMAS_CONFIG_H_
#define _AMAS_CONFIG_H_

#include "Common.h"
#include "Timer.h"
#include <ace/Singleton.h>

namespace conf
{
    enum BoolConfigs
    {
        AMAS_ENABLE,
        AMAS_GM_CHECK_ENABLE,
		AMAS_FULL_IP_CHECK_ENABLE,

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
        AMAS_MORE_IP_POINT_FULL,
		AMAS_MORE_IP_POINT_FIRST_BYTE,
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
		AMAS_TIME_CHECK_RND_BAN,

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
	
	IntervalTimer &GetUpdateTimerRandomBan() { return _UpdateTimerRandomBan; }

private:
    bool m_bool[conf::BOOL_VALUE_COUNT];
    float m_float[conf::FLOAT_VALUE_COUNT];
    uint32 m_int[conf::INT_VALUE_COUNT];
    std::string m_string[conf::STRING_VALUE_COUNT];
	
	IntervalTimer _UpdateTimerRandomBan;
};

#define sAMASConfig ACE_Singleton<AMASConfig, ACE_Null_Mutex>::instance()

#define CONF_BOOL sAMASConfig->GetBool
#define CONF_INT sAMASConfig->GetInt
#define CONF_STR sAMASConfig->GetString
#define CONF_FLOAT sAMASConfig->GetFloat

#endif // _AMAS_CONFIG_H_