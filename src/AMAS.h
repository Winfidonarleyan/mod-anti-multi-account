#ifndef _AMAS_H_
#define _AMAS_H_

#include "Common.h"
#include "Player.h"
#include "Chat.h"
#include <ace/Singleton.h>

namespace amas
{
    enum Language
    {
        // AMAS
        AMAS_ANNOUNCE_GM = 40037,
        AMAS_PLAYER_NOT_SAVED_DB,
        AMAS_INFO_PLAYER_WARNING_DETAIL,
        AMAS_INFO_PLAYER_WARNING,
        AMAS_WARNING_ZONE_LIST,
        AMAS_WARNING_ZONE_ENTER_ZONEID,
        AMAS_IS_LOW_GMLEVEL,
        AMAS_WARNING_ZONE_ADD,
        AMAS_WARNING_ZONE_NOT_FOUND,
        AMAS_WARNING_ZONE_DELETE,
        AMAS_WARNING_ZONE_EXIST,
        AMAS_WARNING_ZONE_NOT_LOADED,
        AMAS_ZONE_INVALID,
        AMAS_INFO,
        AMAS_IS_WARNING_ZONE,
		AMAS_LIST_ONLINE_PLAYER,
		AMAS_LIST_ONLINE_PLAYER_NOT_FOUND,
		AMAS_LIST_OFFLINE_PLAYER,
		AMAS_LIST_OFFLINE_PLAYER_NOT_FOUND,
		AMAS_COMMENT_NEED_ENTER_PLAYENAME_AND_COMMENT,
		AMAS_COMMENT_NEED_ENTER_COMMENT,
		AMAS_COMMENT_NEED_ENTER_COMMENTID,
		AMAS_COMMENT_NEED_COMMENTID_AND_COMMENT,
		AMAS_COMMENT_NEED_ENTER_PLAYERNAME,
		AMAS_COMMENT_NOT_FOUND_COMMENTID,
		AMAS_COMMENT_NOT_FOUND_COMMENTS_FOR_PLAYER,
		AMAS_COMMENT_INVALID_COMMENTID,
		AMAS_COMMENT_ADDED,
		AMAS_COMMENT_DELETED,
		AMAS_COMMENT_EDITED,
		AMAS_COMMENT_LIST,
		AMAS_COMMENT_LIST_FOR
    };
}

class AMAS
{
    friend class ACE_Singleton<AMAS, ACE_Null_Mutex>;

public:
    AMAS() {}
    ~AMAS() {}

    typedef std::vector<uint32> WarningZoneContainer;
    WarningZoneContainer _warningZoneStore;

    float GetAllWarningPoint(Player* player);
    void LogoutPlayer(Player* player);
    void LoadWarningZone();
    void AddWarningZone(uint32 ZoneID, bool IsDB);
    void DeleteWarningZone(uint32 ZoneID, bool IsDB);
    /*void AddComment(std::string Text, uint64 PlayerGuid);
    void DeleteComment(std::string Text, uint64 PlayerGuid);
    void EditComment(std::string Text, uint64 PlayerGuid);*/
	void GetTopWPOnlineList(ChatHandler* handler);
    void GetTopWPOfflineList(ChatHandler* handler);

    uint32 GetFriendCount(Player* player);
    uint32 GetMissingTrainerSpells(Player* player);
    uint32 GetProfessionCount(Player* player);
    uint32 GetDateUnixJoinAccount(uint32 AccountID);
    uint32 GetDateUnixJoinCharacter(uint32 PlayerGuid);

    float GetAverageItemLevel(Player* player);

    float GetWPTotalTimeAccount(uint32 TotalTimeAccount);
    float GetWPAverageItemLevel(float AverageItemLevel);
    float GetWPFreeTalent(uint32 FreeTalent);
    float GetWPCompletedQuestCount(uint32 CompleteQuest);
    float GetWPFriend(uint32 FriendCount);
    float GetWPMoney(uint32 Money);
    float GetWPHonorAndKills(uint32 Honor, uint32 Kills);
    float GetWPIP(std::string IP);
    float GetWPTrainerSpells(uint32 MissingTrainerSpells);
    float GetWPWarningZone(uint32 ZoneID);
    float GetWPProfession(uint32 ProffCount);
    float GetWPJoinAccount(uint32 DateUnix);
    float GetWPJoinCharacter(uint32 DateUnix);
    //float GetWPAverageSessionTime(Player* player);

    //std::string GetComment(Player* player);

    bool IsWarningZoneExist();
    bool IsWarningZone(uint32 ZoneID);

    WarningZoneContainer &GetWarningZone() { return _warningZoneStore; }

private:
    
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
