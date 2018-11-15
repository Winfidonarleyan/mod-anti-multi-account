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

void AddAMASSCScripts() 
{
    new AMAS_SC();
    new AMAS_SC_World();
}
