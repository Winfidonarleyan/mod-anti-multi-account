#ifndef _AMAS_RANDOM_BAN_H_
#define _AMAS_RANDOM_BAN_H_

#include "Common.h"
#include <ace/Singleton.h>

class AMASRandomBan
{
    friend class ACE_Singleton<AMASRandomBan, ACE_Null_Mutex>;

public:
    
    void Start();
	void AddRandomBan(uint32 AccountID, uint32 StartBanDate, std::string Duration, std::string BanReason, std::string BanAuthor);

private:
    
};

#define sAMASRandomBan ACE_Singleton<AMASRandomBan, ACE_Null_Mutex>::instance()

#endif // _AMAS_RANDOM_BAN_H_