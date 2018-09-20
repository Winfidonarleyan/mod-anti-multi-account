#ifndef _AMAS_RANDOM_BAN_H_
#define _AMAS_RANDOM_BAN_H_

#include "Common.h"
#include <ace/Singleton.h>

class AMASRandomBan
{
    friend class ACE_Singleton<AMASRandomBan, ACE_Null_Mutex>;

public:
    
    void Start();

private:
    
};

#define sAMASRandomBan ACE_Singleton<AMASRandomBan, ACE_Null_Mutex>::instance()

#endif // _AMAS_RANDOM_BAN_H_