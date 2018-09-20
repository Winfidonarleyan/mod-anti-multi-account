#include "ScriptMgr.h"
#include "Configuration/Config.h"
#include "AMAS.h"
#include "DBCStores.h"
#include "AccountMgr.h"
#include "Language.h" 
#include "AMAS_Config.h"

void AMASConfig::LoadConfig()
{
    // AMAS general
    m_bool[conf::AMAS_ENABLE]                           = sConfigMgr->GetBoolDefault("AMAS.Enable", true);
    m_int[conf::AMAS_MIN_TOTAL_TIME_ACC]                = sConfigMgr->GetIntDefault("AMAS.Min.Total.Time.Account", DAY);
    m_int[conf::AMAS_MIN_TOTAL_TIME_ACC_POINT]          = sConfigMgr->GetIntDefault("AMAS.WP.Total.Time.Account", 10);
    m_int[conf::AMAS_MIN_AVG_ILVL]                      = sConfigMgr->GetIntDefault("AMAS.Min.Average.Ilvl", 50);
    m_int[conf::AMAS_MIN_AVG_ILVL_POINT]                = sConfigMgr->GetIntDefault("AMAS.WP.Average.Ilvl", 10);
    m_int[conf::AMAS_FREE_TALENT_POINT]                 = sConfigMgr->GetIntDefault("AMAS.WP.Free.Talent", 5);
    m_int[conf::AMAS_MIN_COUNT_REWARDED_QUEST]          = sConfigMgr->GetIntDefault("AMAS.Min.Completed.Quest.Count", 20);
    m_int[conf::AMAS_MIN_COUNT_REWARDED_QUEST_POINT]    = sConfigMgr->GetIntDefault("AMAS.WP.Completed.Quest.Count", 10);
    m_int[conf::AMAS_MIN_COUNT_FRIEND]                  = sConfigMgr->GetIntDefault("AMAS.Min.Friend.Count", 2);
    m_int[conf::AMAS_MIN_COUNT_FRIEND_POINT]            = sConfigMgr->GetIntDefault("AMAS.WP.Min.Friend", 5);
    m_int[conf::AMAS_MAX_COUNT_MONEY]                   = sConfigMgr->GetIntDefault("AMAS.Max.Count.Money", 2) * GOLD;
    m_int[conf::AMAS_MAX_COUNT_MONEY_POINT]             = sConfigMgr->GetIntDefault("AMAS.WP.Max.Count.Money", 5);
    m_int[conf::AMAS_NULL_HONOR_AND_KILLS]              = sConfigMgr->GetIntDefault("AMAS.WP.NULL.Honor.And.Kills", 10);
    m_int[conf::AMAS_MORE_IP_POINT_FULL]        		= sConfigMgr->GetIntDefault("AMAS.WP.More.IP.Full", 20);
    m_int[conf::AMAS_MORE_IP_POINT_FIRST_BYTE]  		= sConfigMgr->GetIntDefault("AMAS.WP.More.IP.FirstByte", 20);
    m_int[conf::AMAS_MIN_TRAINER_SPELL_MISSING]         = sConfigMgr->GetIntDefault("AMAS.Min.Missing.Trainer.Spells", 1);
    m_int[conf::AMAS_MISSING_TRAINER_SPELL_POINT]       = sConfigMgr->GetIntDefault("AMAS.WP.Missing.One.Trainer.Spell", 20);
    m_int[conf::AMAS_WARNING_ZONE_POINT]                = sConfigMgr->GetIntDefault("AMAS.WP.Warning.Zone", 20);
    m_int[conf::AMAS_MIN_PROFESSION]                    = sConfigMgr->GetIntDefault("AMAS.Min.Profession", 1);
    m_int[conf::AMAS_PROFESSION_POINT]                  = sConfigMgr->GetIntDefault("AMAS.WP.Professions", 20);
    m_int[conf::AMAS_DIFF_ACC_CREATE]                   = sConfigMgr->GetIntDefault("AMAS.Min.Diff.Account.Create", DAY);
    m_int[conf::AMAS_DIFF_CHAR_CREATE]                  = sConfigMgr->GetIntDefault("AMAS.Min.Diff.Character.Create", DAY);
    m_int[conf::AMAS_DIFF_ACC_CREATE_POINT]             = sConfigMgr->GetIntDefault("AMAS.WP.Diff.Account", 10);
    m_int[conf::AMAS_DIFF_CHAR_CREATE_POINT]            = sConfigMgr->GetIntDefault("AMAS.WP.Diff.Character", 5);
    m_int[conf::AMAS_MIN_AVERAGE_SESSION]               = sConfigMgr->GetIntDefault("AMAS.Min.Average.Session", 5 * IN_MILLISECONDS * 60);
    m_int[conf::AMAS_MIN_AVERAGE_SESSION_POINT]         = sConfigMgr->GetIntDefault("AMAS.WP.Min.Average.Session", 10);

    // Amas confirmed
    m_int[conf::AMAS_CONFIRMED_MIN_POINT]               = sConfigMgr->GetIntDefault("AMAS.Confirmed.Min.Points", 20);
    m_int[conf::AMAS_CONFIRMED_ACTION]                  = sConfigMgr->GetIntDefault("AMAS.Confirmed.Action", 20);
    m_int[conf::AMAS_CONFIRMED_BAN_INTERVAL_MIN]        = sConfigMgr->GetIntDefault("AMAS.Confirmed.Ban.RandomInterval.Min", 20);
    m_int[conf::AMAS_CONFIRMED_BAN_INTERVAL_MAX]        = sConfigMgr->GetIntDefault("AMAS.Confirmed.Ban.RandomInterval.Max", 20);
    m_string[conf::AMAS_BAN_REASON]                     = sConfigMgr->GetStringDefault("AMAS.Confirmed.Ban.Reason", "This account has been banned for using multiple accounts");

    // Other amas option
    m_int[conf::AMAS_SUSPICIOUS_ACCOUNT_MIN_POINT]      = sConfigMgr->GetIntDefault("AMAS.Suspicious.Account.Min.Points", 50);
    m_int[conf::AMAS_LOW_GMLEVEL]                       = sConfigMgr->GetIntDefault("AMAS.Low.GMLevel", 20);
    m_int[conf::AMAS_HIGH_GMLEVEL]                      = sConfigMgr->GetIntDefault("AMAS.High.GMLevel", 20);
    m_int[conf::AMAS_WARNING_INTERVAL]                  = sConfigMgr->GetIntDefault("AMAS.Warning.Interval", 20);
    m_int[conf::AMAS_MIN_TIME_TO_DB_HISTORY]            = sConfigMgr->GetIntDefault("AMAS.Min.Time.For.DB.History", DAY / 2);
    m_bool[conf::AMAS_GM_CHECK_ENABLE]                  = sConfigMgr->GetBoolDefault("AMAS.GM.Check.Enable", false);
	m_bool[conf::AMAS_FULL_IP_CHECK_ENABLE]     		= sConfigMgr->GetBoolDefault("AMAS.Full.IP.Check.Enable", false);
	
	m_int[conf::AMAS_TIME_CHECK_RND_BAN]        		= sConfigMgr->GetIntDefault("AMAS.Time.Check.Ban.Random", MINUTE * 5);
    _UpdateTimerRandomBan.SetInterval(m_int[conf::AMAS_TIME_CHECK_RND_BAN] * IN_MILLISECONDS);
	
	if (_UpdateTimerRandomBan.GetInterval() < 1 * IN_MILLISECONDS)
    {
		sLog->outError("Time check random ban (%d) sec, this will freatly load worldserver. Set as default (%d)", _UpdateTimerRandomBan.GetInterval(), MINUTE * 5);
        _UpdateTimerRandomBan.SetInterval(MINUTE * 5);
    }
}