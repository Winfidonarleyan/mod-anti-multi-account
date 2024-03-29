#ifndef _AMAS_H_
#define _AMAS_H_

#include "Common.h"
#include "Player.h"
#include "Chat.h"
#include <ace/Singleton.h>

struct AMASIP
{
    AMASIP()
        : first(0), second(0), third(0), fourth(0) { }

    AMASIP(uint8 const _first, uint8 const _second, uint8 const _third, uint8 const _fourth)
        : first(_first), second(_second), third(_third), fourth(_fourth) { }

    uint8 first;
    uint8 second;
    uint8 third;
    uint8 fourth;
};

namespace amas
{
    enum Language
    {
        // AMAS general
        AMAS_DISABLED = 40037,

        // AMAS announce
        AMAS_ANNOUNCE_GM,

        // AMAS info
        AMAS_INFO,
        AMAS_PLAYER_NOT_SAVED_DB,

        // AMAS Warning zone
        AMAS_WARNING_ZONE_LIST,
        AMAS_WARNING_ZONE_ADD,
        AMAS_WARNING_ZONE_NOT_FOUND,
        AMAS_WARNING_ZONE_DELETE,
        AMAS_WARNING_ZONE_EXIST,
        AMAS_WARNING_ZONE_NOT_LOADED,
        AMAS_IS_WARNING_ZONE,
        AMAS_ZONE_INVALID,

        // AMAS List
        AMAS_LIST_ONLINE_PLAYER,
        AMAS_LIST_ONLINE_PLAYER_NOT_FOUND,
        AMAS_LIST_OFFLINE_PLAYER,
        AMAS_LIST_OFFLINE_PLAYER_NOT_FOUND,

        // AMAS comment
        AMAS_COMMENT_NEED_ENTER_PLAYENAME_AND_COMMENT,
        AMAS_COMMENT_NEED_ENTER_COMMENT,
        AMAS_COMMENT_NEED_ENTER_COMMENTID,
        AMAS_COMMENT_NEED_COMMENTID_AND_COMMENT,
        AMAS_COMMENT_NOT_FOUND_COMMENTID,
        AMAS_COMMENT_NOT_FOUND_COMMENTS_FOR_PLAYER,
        AMAS_COMMENT_INVALID_COMMENTID,
        AMAS_COMMENT_ADDED,
        AMAS_COMMENT_DELETED,
        AMAS_COMMENT_EDITED,
        AMAS_COMMENT_LIST,
        AMAS_COMMENT_LIST_FOR,

        // AMAS same ip
        AMAS_IP_IS_UNIQUE,
        AMAS_IP_IS_PERFECT,
        AMAS_IP_IS_PARTIAL,
		
		// Amas confirmed
        AMAS_GM_ANNOUNCE_CONFIRMED,
		
		// Amas mail
        AMAS_MAIL_INFO
    };
}

enum ConfirmedAction
{
    ACTION_BAN = 1,
    ACTION_GM_MESSAGE,
    ACTION_ALL
};

class AMAS
{
    friend class ACE_Singleton<AMAS, ACE_Null_Mutex>;

public:
	AMAS() {}
    ~AMAS() {}

    typedef std::vector<uint32> WarningZoneContainer;

    float GetAllWarningPoint(Player* player);
    void LogoutPlayer(Player* player);
	void LoginPlayer(Player* player);
    void LoadWarningZone();
    void AddWarningZone(uint32 ZoneID, bool IsDB);
    void DeleteWarningZone(uint32 ZoneID, bool IsDB);
	void GetTopWPOnlineList(ChatHandler* handler);
    void GetTopWPOfflineList(ChatHandler* handler);
	void CheckConfirmed(Player* player);

    uint32 GetFriendCount(uint64 PlayerGuid);
    uint32 GetMissingTrainerSpells(Player* player);
    uint32 GetProfessionCount(Player* player);
    uint32 GetDateUnixJoinAccount(uint32 AccountID);
    uint32 GetDateUnixJoinCharacter(uint32 PlayerGuid);
	uint32 GetCommentCount(uint64 PlayerGuid);
	uint32 GetIPCount(std::string IP, bool IsFirstByte = false);
	uint32 GetAverageSessionTime(uint64 PlayerGuid);
	uint32 GetTotalTimePlayer(uint64 PlayerGuid);
    uint32 GetSessionCount(uint64 PlayerGuid);
	uint32 GetTotalTimeAccount(uint32 AccountID);
    
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
	float GetWPAverageSessionTime(uint32 Time);	
	float GetAverageItemLevel(Player* player);

    bool IsWarningZoneExist();
    bool IsWarningZone(uint32 ZoneID);
	bool IsFirstBytesIPSame(std::string IP1, std::string IP2);
	bool IsFoundInfoInDB(uint64 PlayerGuid);
	
	std::string GetListAccountForIP(std::string IP);
    std::string GetFirstBytesIP(std::string IP);

    AMASIP GetIPFromString(std::string IP);

    WarningZoneContainer &GetWarningZone() { return _warningZoneStore; }

private:

	WarningZoneContainer _warningZoneStore;
    
    void PushDBPlayerInfo(Player* player);
};

#define sAMAS ACE_Singleton<AMAS, ACE_Null_Mutex>::instance()

#endif // _AMAS_H_
