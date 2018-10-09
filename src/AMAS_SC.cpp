#include "ScriptMgr.h"
#include "Configuration/Config.h"
#include "AMAS.h"
#include "DBCStores.h"
#include "Language.h"
#include "AMAS_Config.h"
#include "AccountMgr.h"
#include "AMAS_Random_Ban.h"

// AMAS SC
class AMAS_SC : public PlayerScript
{
public:
    AMAS_SC() : PlayerScript("AMAS_SC") { }    

    void OnLogin(Player* player) override
    {
        sAMAS->LoginPlayer(player);
    }

    void OnLogout(Player* player) override
    {
        sAMAS->LogoutPlayer(player);
    }
};

class AMAS_SC_World : public WorldScript
{
public:
    AMAS_SC_World() : WorldScript("AMAS_SC_World") { }

    void OnLoadCustomDatabaseTable() override
    {
        if (!CONF_BOOL(conf::AMAS_ENABLE))
            return;

        sLog->outString("Loading warning zone for AMAS");
        sAMAS->LoadWarningZone();
    }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sLog->outString("Loading AMAS configuration...");
        sAMASConfig->LoadConfig();
    }
	
	void OnUpdate(uint32 Diff) override
    {
        if (!CONF_BOOL(conf::AMAS_ENABLE))
            return;

        UpdateTimer.Update(Diff);

        if (UpdateTimer.Passed())
        {
            sAMASRandomBan->Start();
            UpdateTimer.Reset();
        }
    }
	
private:
	IntervalTimer& UpdateTimer = sAMASConfig->GetUpdateTimerRandomBan();
};

class CS_AMAS : public CommandScript
{
public:
    CS_AMAS() : CommandScript("CS_AMAS") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> TableCommandAmasZone = // .amas zone
        {
            { "list",	    	    SEC_ADMINISTRATOR,  	true, &HandleAMASZoneList, 		   	                "" },
            { "add",	    	    SEC_ADMINISTRATOR,  	true, &HandleAMASZoneAdd, 		   	                "" },
            { "delete",	    	    SEC_ADMINISTRATOR,  	true, &HandleAMASZoneDelete, 		   	            "" },
            { "reload",	    	    SEC_ADMINISTRATOR,  	true, &HandleAMASZoneRelaod, 		   	            "" }
        };        

        static std::vector<ChatCommand> TableCommandAmasComment = // .amas comment
        {
            { "add",	            SEC_ADMINISTRATOR,  	true, &HandleAMASCommentAdd, 		   	            "" },
            { "delete",	    	    SEC_ADMINISTRATOR,  	true, &HandleAMASCommentDelete, 		   	        "" },
            { "edit",	    	    SEC_ADMINISTRATOR,  	true, &HandleAMASCommentEdit, 		   	            "" },
			{ "list",	    	    SEC_ADMINISTRATOR,  	true, &HandleAMASCommentList, 		   	            "" }
        };

        static std::vector<ChatCommand> TableCommandAmasList = // .amas list
        {
            { "all",	            SEC_ADMINISTRATOR,  	true, &HandleAMASListAll, 		   	                "" },
            { "online",	    	    SEC_ADMINISTRATOR,  	true, &HandleAMASListOnline, 		   	            "" },
            { "offline",	    	SEC_ADMINISTRATOR,  	true, &HandleAMASListOffline, 		   	            "" }
        };

        static std::vector<ChatCommand> TableCommandAmas = // .amas
        {
            { "info",				SEC_ADMINISTRATOR, 		true, &HandleAMASInfo,             	   			    "" },
			{ "mail",				SEC_ADMINISTRATOR, 		true, &HandleAMASMail,             	   			    "" },
			{ "zone",				SEC_ADMINISTRATOR, 		true, nullptr,             	   					    "", TableCommandAmasZone },            
            { "comment",			SEC_ADMINISTRATOR, 		true, nullptr,             	   						"", TableCommandAmasComment },
            { "list",			    SEC_ADMINISTRATOR, 		true, nullptr,             	   						"", TableCommandAmasList }
        };

        static std::vector<ChatCommand> commandTable =
        {  
            { "amas",				SEC_ADMINISTRATOR, 		true, nullptr,             	   				        "", TableCommandAmas }
        };

        return commandTable;
    }

	static bool HandleAMASMail(ChatHandler *handler, const char *args)
    {
        Player* player;
        uint64 playerGUID;
        std::string PlayerName;
        if (!handler->extractPlayerTarget((char*)args, &player, &playerGUID, &PlayerName))
            return false;

        uint32 AccountID = 0;
        std::string AccountName;

        player ? AccountID = player->GetSession()->GetAccountId() : AccountID = sObjectMgr->GetPlayerAccountIdByGUID(playerGUID);
                
        AccountMgr::GetName(AccountID, AccountName);
        QueryResult result = nullptr;        
        uint32 MailCountMoney = 0;
        uint32 MailCountItem = 0;
        uint32 MailCountOnlyText = 0;
        uint32 MailCountAuction = 0;

        result = CharacterDatabase.PQuery("SELECT COUNT(*) FROM `mail` WHERE messageType = 0 AND money > 0 AND sender <> receiver AND receiver = %u", playerGUID);
        if (result)
            MailCountMoney = (uint32)result->Fetch()->GetUInt64();

        result = CharacterDatabase.PQuery("SELECT COUNT(*) FROM `mail` WHERE messageType = 0 AND has_items > 0 AND sender <> receiver AND receiver = %u", playerGUID);
        if (result)
            MailCountItem = (uint32)result->Fetch()->GetUInt64();

        result = CharacterDatabase.PQuery("SELECT COUNT(*) FROM `mail` WHERE messageType = 0 AND has_items = 0 AND money = 0 AND sender <> receiver AND receiver = %u", playerGUID);
        if (result)
            MailCountOnlyText = (uint32)result->Fetch()->GetUInt64();

        result = CharacterDatabase.PQuery("SELECT COUNT(*) FROM `mail` WHERE messageType = 2 AND receiver = %u", playerGUID);
        if (result)
            MailCountAuction = (uint32)result->Fetch()->GetUInt64();

        handler->PSendSysMessage(amas::AMAS_MAIL_INFO, PlayerName.c_str(), MailCountMoney, MailCountItem, MailCountOnlyText, MailCountAuction, AccountName.c_str(), AccountID);

        return true;
    }

	static bool HandleAMASListAll(ChatHandler *handler, const char * /*args*/)
    {
        if (!CONF_BOOL(conf::AMAS_ENABLE))
        {
            handler->SendSysMessage(amas::AMAS_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		sAMAS->GetTopWPOnlineList(handler);
        sAMAS->GetTopWPOfflineList(handler);
        return true;
    }
    
    static bool HandleAMASListOnline(ChatHandler *handler, const char * /*args*/)
    {
        if (!CONF_BOOL(conf::AMAS_ENABLE))
        {
            handler->SendSysMessage(amas::AMAS_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		sAMAS->GetTopWPOnlineList(handler);
        return true;
    }

    static bool HandleAMASListOffline(ChatHandler *handler, const char * /*args*/)
    {
        if (!CONF_BOOL(conf::AMAS_ENABLE))
        {
            handler->SendSysMessage(amas::AMAS_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		sAMAS->GetTopWPOfflineList(handler);
        return true;
    }

    static bool HandleAMASCommentAdd(ChatHandler *handler, const char *args)
    {
        if (!CONF_BOOL(conf::AMAS_ENABLE))
        {
            handler->SendSysMessage(amas::AMAS_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if (!*args)
        {
            handler->SendSysMessage(amas::AMAS_COMMENT_NEED_ENTER_PLAYENAME_AND_COMMENT);
            handler->SetSentErrorMessage(true);
            return false;
        }
        
        Player* target = handler->getSelectedPlayer();
        uint64 targetGuid;
        std::string targetName;
        char* Comment;

        if (target && (handler->GetSession() != target->GetSession()))
        {
            target = handler->getSelectedPlayer();
            targetGuid = target->GetGUID();
            targetName = target->GetName();
            Comment = strtok((char*)args, "\r");
        }
        else
        {
            if (!handler->extractPlayerTarget((char*)args, &target, &targetGuid, &targetName))
                return false;

            Comment = strtok(nullptr, "\r");
        }
        
        std::string CommentStr;
        if (Comment == nullptr)
        {
            handler->SendSysMessage(amas::AMAS_COMMENT_NEED_ENTER_COMMENT);
            handler->SetSentErrorMessage(true);
            return false;
        }
        else
            CommentStr = Comment;
        
        uint32 PlayerAccount = sObjectMgr->GetPlayerAccountIdByGUID(targetGuid);
        uint32 CommenterGuid = handler->GetSession()->GetPlayer()->GetGUID();
        uint32 CommenterAccountID = handler->GetSession()->GetAccountId();

        std::replace(CommentStr.begin(), CommentStr.end(), '\'', ' ');
        
        CharacterDatabase.PExecute("INSERT INTO `amas_player_comment`(`PlayerGuid`, `PlayerAccount`, `Comment`, `Date`, `CommenterGuid`, `CommenterAccount`) VALUES (%u, %u, '%s', NOW(), %u, %u)",
            targetGuid, PlayerAccount, CommentStr.c_str(), CommenterGuid, CommenterAccountID);

        handler->PSendSysMessage(amas::AMAS_COMMENT_ADDED, targetName.c_str(), CommentStr.c_str());

        return true;
    }

    static bool HandleAMASCommentDelete(ChatHandler *handler, const char *args)
    {
        if (!CONF_BOOL(conf::AMAS_ENABLE))
        {
            handler->SendSysMessage(amas::AMAS_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if (!*args)
        {
            handler->SendSysMessage(amas::AMAS_COMMENT_NEED_ENTER_COMMENTID);
            handler->SetSentErrorMessage(true);
            return false;
        }

        uint32 CommentID = (uint32)atoi((char *)args);
        std::string Comment;

        QueryResult result = CharacterDatabase.PQuery("SELECT Comment FROM `amas_player_comment` WHERE `CommentID` = %u", CommentID);
        if (!result)
        {
            handler->PSendSysMessage(amas::AMAS_COMMENT_NOT_FOUND_COMMENTID, CommentID);
            handler->SetSentErrorMessage(true);
            return false;
        }
        else
            Comment = result->Fetch()->GetString();

        CharacterDatabase.PExecute("DELETE FROM `amas_player_comment` WHERE `CommentID` = %u", CommentID);

        handler->PSendSysMessage(amas::AMAS_COMMENT_DELETED, CommentID, Comment.c_str());

        return true;
    }

    static bool HandleAMASCommentEdit(ChatHandler *handler, const char *args)
    {
        if (!CONF_BOOL(conf::AMAS_ENABLE))
        {
            handler->SendSysMessage(amas::AMAS_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if (!*args)
        {
            handler->PSendSysMessage(amas::AMAS_COMMENT_NEED_COMMENTID_AND_COMMENT);
            handler->SetSentErrorMessage(true);
            return false;
        }

        char *IDComment = strtok((char *)args, " ");
        if (!IDComment)
        {
            handler->PSendSysMessage(amas::AMAS_COMMENT_INVALID_COMMENTID);
            handler->SetSentErrorMessage(true);
            return false;
        }

        uint32 CommentID = (uint32)atoi(IDComment);

        char* Comment = strtok(nullptr, "\r");
        std::string CommentStr;
        if (Comment == nullptr)
        {
            handler->SendSysMessage(amas::AMAS_COMMENT_NEED_ENTER_COMMENT);
            handler->SetSentErrorMessage(true);
            return false;
        }
        else
            CommentStr = Comment;

        std::replace(CommentStr.begin(), CommentStr.end(), '\'', ' ');

        std::string NewComment = CommentStr;
        std::string OldComment;

        QueryResult result = CharacterDatabase.PQuery("SELECT Comment FROM `amas_player_comment` WHERE `CommentID` = %u", CommentID);
        if (!result)
        {
            handler->PSendSysMessage(amas::AMAS_COMMENT_NOT_FOUND_COMMENTID, CommentID);
            handler->SetSentErrorMessage(true);
            return false;
        }
        else
            OldComment = result->Fetch()->GetString();

        CharacterDatabase.PExecute("UPDATE `amas_player_comment` SET `Comment` = '%s' WHERE `CommentID` = %u", NewComment.c_str(), CommentID);

        handler->PSendSysMessage(amas::AMAS_COMMENT_EDITED, CommentID, OldComment.c_str(), NewComment.c_str());

        return true;
    }

    static bool HandleAMASCommentList(ChatHandler *handler, const char *args)
    {
        if (!CONF_BOOL(conf::AMAS_ENABLE))
        {
            handler->SendSysMessage(amas::AMAS_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		Player* target = handler->getSelectedPlayer();
        uint64 targetGuid;
        std::string targetName;

        if (!args && target)
        {
            target = handler->getSelectedPlayer();
            targetGuid = target->GetGUID();
            targetName = target->GetName();
        }
        else
        {
            if (!handler->extractPlayerTarget((char*)args, &target, &targetGuid, &targetName))
                return false;
        }
                                                     //           0         1      2          3
        QueryResult result = CharacterDatabase.PQuery("SELECT CommentID, Comment, Date, CommenterGuid FROM `amas_player_comment` WHERE `PlayerGuid` = %u", targetGuid);
        if (!result)
        {
            handler->PSendSysMessage(amas::AMAS_COMMENT_NOT_FOUND_COMMENTS_FOR_PLAYER, targetName.c_str());
            handler->SetSentErrorMessage(true);
            return false;
        }

        handler->PSendSysMessage(amas::AMAS_COMMENT_LIST, targetName.c_str());

        do
        {
            Field *field = result->Fetch();
            uint32 CommentID = field[0].GetUInt32();
            std::string Comment = field[1].GetString();
            std::string Date = field[2].GetString();
            uint32 CommenterGuid = field[3].GetUInt32();

            std::string CommenterName;

            if (!sObjectMgr->GetPlayerNameByGUID(CommenterGuid, CommenterName))
                CommenterName = handler->GetTrinityString(LANG_UNKNOWN);                

            handler->PSendSysMessage(amas::AMAS_COMMENT_LIST_FOR, CommentID, CommenterName.c_str(), Date.c_str(), Comment.c_str());

        } while (result->NextRow());

        return true;
    }
 
    static bool HandleAMASZoneList(ChatHandler *handler, const char* /*args*/)
    {
        if (!CONF_BOOL(conf::AMAS_ENABLE))
        {
            handler->SendSysMessage(amas::AMAS_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if (!sAMAS->IsWarningZoneExist())
        {
            handler->PSendSysMessage(amas::AMAS_WARNING_ZONE_NOT_LOADED);
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		handler->SendSysMessage(amas::AMAS_WARNING_ZONE_LIST);
        uint32 Count = 1;
        int8 locale_index = handler->GetSessionDbLocaleIndex();
        std::string ZoneName = "";

        AMAS::WarningZoneContainer& _WarningZone = sAMAS->GetWarningZone();
        for (auto i : _WarningZone)
        {
            if (Count == 50)
                break;

            AreaTableEntry const* zone = sAreaTableStore.LookupEntry(i);
            if (!zone)
                continue;

            ZoneName = zone->area_name[locale_index];

            if (ZoneName.empty())
                continue;

            handler->PSendSysMessage("%u. %u - %s", Count, i, ZoneName.c_str());

            Count++;
        }

        return true;
    }

    static bool HandleAMASZoneAdd(ChatHandler *handler, const char *args)
    {
        if (!CONF_BOOL(conf::AMAS_ENABLE))
        {
            handler->SendSysMessage(amas::AMAS_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		uint32 ZoneID = 0;

        if (!*args)
            ZoneID = handler->GetSession()->GetPlayer()->GetZoneId();
        else
            ZoneID = (uint32)atoi((char *)args);

        std::string ZoneName = "";

        AreaTableEntry const* zone = sAreaTableStore.LookupEntry(ZoneID);
        if (!zone)
        {
            handler->PSendSysMessage(amas::AMAS_ZONE_INVALID, ZoneID);
            handler->SetSentErrorMessage(true);
            return false;
        }
        else
            ZoneName = zone->area_name[handler->GetSessionDbLocaleIndex()];

        if (sAMAS->IsWarningZone(ZoneID))
        {
            handler->PSendSysMessage(amas::AMAS_WARNING_ZONE_EXIST, ZoneID, ZoneName.c_str());
            handler->SetSentErrorMessage(true);
            return false;
        }

        sAMAS->AddWarningZone(ZoneID, true);

        handler->PSendSysMessage(amas::AMAS_WARNING_ZONE_ADD, ZoneID, ZoneName.c_str());

        return true;
    }

    static bool HandleAMASZoneDelete(ChatHandler *handler, const char *args)
    {
        if (!CONF_BOOL(conf::AMAS_ENABLE))
        {
            handler->SendSysMessage(amas::AMAS_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		uint32 ZoneID = 0;

        if (!*args)
            ZoneID = handler->GetSession()->GetPlayer()->GetZoneId();
        else
            ZoneID = (uint32)atoi((char *)args);

        std::string ZoneName = "";

        AreaTableEntry const* zone = sAreaTableStore.LookupEntry(ZoneID);
        if (!zone)
        {
            handler->PSendSysMessage(amas::AMAS_ZONE_INVALID, ZoneID);
            handler->SetSentErrorMessage(true);
            return false;
        }
        else
            ZoneName = zone->area_name[handler->GetSessionDbLocaleIndex()];

        if (!sAMAS->IsWarningZone(ZoneID))
        {
            handler->PSendSysMessage(amas::AMAS_WARNING_ZONE_NOT_FOUND, ZoneID, ZoneName.c_str());
            handler->SetSentErrorMessage(true);
            return false;
        }

        sAMAS->DeleteWarningZone(ZoneID, true);

        handler->PSendSysMessage(amas::AMAS_WARNING_ZONE_DELETE, ZoneID, ZoneName.c_str());

        return true;
    }

    static bool HandleAMASZoneRelaod(ChatHandler *handler, const char* /*args*/)
    {
        if (!CONF_BOOL(conf::AMAS_ENABLE))
        {
            handler->SendSysMessage(amas::AMAS_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		sLog->outString("Reload warning zone for AMAS...");
        sAMAS->LoadWarningZone();
        handler->SendGlobalGMSysMessage("|cff6C8CD5#|cFFFF0000 DB table|r `amas_warning_zone` |cFFFF0000reloading|r");
        return true;
    }
    
    static bool HandleAMASInfo(ChatHandler *handler, const char *args)
    {
        if (!CONF_BOOL(conf::AMAS_ENABLE))
        {
            handler->SendSysMessage(amas::AMAS_DISABLED);
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		Player* player;
        uint64 playerGUID;
        std::string PlayerName;
        if (!handler->extractPlayerTarget((char*)args, &player, &playerGUID, &PlayerName))
            return false;

        uint32 TotalTimeAccount, AVGILvl, FreeTalent, TotalRewardQuest, TotalTimePlayed, FriendCount, TotalMoney, TotalHonorPoint, TotalKill, MissingTrainerSpells, CurrentZone, ProfCount, AVGSessionTime, SessionNumber;
        std::string PlayerIP;

        if (player)
        {
            AVGILvl = sAMAS->GetAverageItemLevel(player);
            FreeTalent = player->GetFreeTalentPoints();
            TotalRewardQuest = player->GetRewardedQuestCount();
            TotalTimePlayed = player->GetTotalPlayedTime();
            TotalMoney = player->GetMoney();
            TotalHonorPoint = player->GetHonorPoints();
            TotalKill = player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS);
            PlayerIP = player->GetSession()->GetRemoteAddress();
            MissingTrainerSpells = sAMAS->GetMissingTrainerSpells(player);
            CurrentZone = player->GetZoneId();
            ProfCount = sAMAS->GetProfessionCount(player);
        }
        else
        {
            //                                                             0           1             2                 3                   4               5           6          7               8                9          10                                                          
            QueryResult result = CharacterDatabase.PQuery("SELECT a.AverageItemLevel, a.IP, a.CompletedQuests, a.MissingSpells, a.ProfessionLearned, a.FreeTalent, c.money, c.totaltime, c.totalHonorPoints, c.totalKills, c.zone FROM amas_player_info a INNER JOIN characters c ON a.PlayerGUID = c.guid WHERE a.PlayerGUID = %u", playerGUID);
            if (!result)
            {
                handler->PSendSysMessage(amas::AMAS_PLAYER_NOT_SAVED_DB, PlayerName.c_str());
                handler->SetSentErrorMessage(true);
                return false;
            }

            Field* field            = result->Fetch();
            AVGILvl                 = (uint32)field[0].GetUInt16();
            PlayerIP                = field[1].GetString();
            TotalRewardQuest        = (uint32)field[2].GetUInt16();
            MissingTrainerSpells    = (uint32)field[3].GetUInt8();
            ProfCount               = (uint32)field[4].GetUInt8();
            FreeTalent              = (uint32)field[5].GetUInt8();
            TotalMoney              = field[6].GetUInt32();
            TotalTimePlayed         = field[7].GetUInt32();
            TotalHonorPoint         = field[8].GetUInt32();
            TotalKill               = field[9].GetUInt32();
            CurrentZone             = (uint32)field[10].GetUInt8();
        }
		
		TotalTimeAccount = sAMAS->GetTotalTimeAccount(sObjectMgr->GetPlayerAccountIdByGUID(playerGUID));
        FriendCount = sAMAS->GetFriendCount(playerGUID);
        SessionNumber = sAMAS->GetSessionCount(playerGUID) + 1;

        uint32 gold = TotalMoney / GOLD;
        uint32 silv = (TotalMoney % GOLD) / SILVER;
        uint32 copp = (TotalMoney % GOLD) % SILVER;

        std::string TotalTimeAccountStr = secsToTimeString(TotalTimeAccount);
        std::string MoneyStr = std::to_string(gold) + "|TInterface\\MoneyFrame\\UI-GoldIcon:0:0:2:0|t " + std::to_string(silv) + "|TInterface\\MoneyFrame\\UI-SilverIcon:0:0:2:0|t " + std::to_string(copp) + "|TInterface\\MoneyFrame\\UI-CopperIcon:0:0:2:0|t ";
        std::string TotalTimePlayedStr = secsToTimeString(TotalTimePlayed);
        std::string IsWarningZone;
        std::string ZoneName;

        AreaTableEntry const* zone = sAreaTableStore.LookupEntry(CurrentZone);
        if (zone)
            ZoneName = zone->area_name[handler->GetSessionDbLocaleIndex()];

        if (sAMAS->IsWarningZone(CurrentZone))
            IsWarningZone = handler->GetTrinityString(amas::AMAS_IS_WARNING_ZONE);
		
		AVGSessionTime = sAMAS->GetAverageSessionTime(playerGUID);
		std::string AVGStr = secsToTimeString(AVGSessionTime);
		
        float WPTimeAcc = sAMAS->GetWPTotalTimeAccount(TotalTimeAccount);
        float WPAverageIlvl = sAMAS->GetWPAverageItemLevel(AVGILvl);
        float WPFreeTalent = sAMAS->GetWPFreeTalent(FreeTalent);
        float WPCompletedQuest = sAMAS->GetWPCompletedQuestCount(TotalRewardQuest);
        float WPFriend = sAMAS->GetWPFriend(FriendCount);
        float WPMoney = sAMAS->GetWPMoney(TotalMoney);
        float WPHonorAndKills = sAMAS->GetWPHonorAndKills(TotalHonorPoint, TotalKill);
        float WPTrainerSpells = sAMAS->GetWPTrainerSpells(MissingTrainerSpells);
        float WPWarningZone = sAMAS->GetWPWarningZone(CurrentZone);
        float WPProfession = sAMAS->GetWPProfession(ProfCount);
        float WPIp = sAMAS->GetWPIP(PlayerIP);
        float WPAVGSessionTime = sAMAS->GetWPAverageSessionTime(AVGSessionTime);
        float WPAll = WPTimeAcc + WPAverageIlvl + WPFreeTalent + WPCompletedQuest + WPFriend + WPMoney + WPHonorAndKills + WPTrainerSpells + WPWarningZone + WPProfession + WPIp + WPAVGSessionTime;
        uint32 CommentCount = sAMAS->GetCommentCount(playerGUID);
		
		uint32 SameIpCountFull = sAMAS->GetIPCount(PlayerIP);
        uint32 SameIpCountFirstByte = sAMAS->GetIPCount(PlayerIP, true);
        std::string IsUniqueIP = handler->GetTrinityString(LANG_YES);
        std::string AccountListSameIP;
        std::string IsSameIp;

        if (SameIpCountFull > 1)
        {
            IsUniqueIP = handler->GetTrinityString(LANG_NO);
            IsSameIp = handler->GetTrinityString(amas::AMAS_IS_SAME_IP_FULL);
            AccountListSameIP = "(" + sAMAS->GetListAccountForIP(PlayerIP) + ")";
        }
        else if (SameIpCountFirstByte > 1)
        {
            IsUniqueIP = handler->GetTrinityString(LANG_NO);
            IsSameIp = handler->GetTrinityString(amas::AMAS_IS_SAME_IP_FIRST);
        }	

        handler->PSendSysMessage(amas::AMAS_INFO,
            PlayerName.c_str(), WPAll,
            WPTimeAcc, TotalTimeAccountStr.c_str(),
            WPAverageIlvl, AVGILvl,
            WPFreeTalent, FreeTalent,
            WPCompletedQuest, TotalRewardQuest,            
            WPFriend, FriendCount,
            WPMoney, MoneyStr.c_str(),
            WPHonorAndKills, TotalHonorPoint, TotalKill,
            WPIp, PlayerIP.c_str(), IsUniqueIP.c_str(), IsSameIp.c_str(), AccountListSameIP.c_str(),
            WPTrainerSpells, MissingTrainerSpells,
            WPWarningZone, CurrentZone, ZoneName.c_str(), IsWarningZone.c_str(),
            WPProfession, ProfCount,
			WPAVGSessionTime, SessionNumber, AVGStr.c_str(),
            TotalTimePlayedStr.c_str(),
            CommentCount);

        return true;
    }
};

void AddAMASScripts() 
{
    new AMAS_SC();
    new AMAS_SC_World();
    new CS_AMAS();
}
