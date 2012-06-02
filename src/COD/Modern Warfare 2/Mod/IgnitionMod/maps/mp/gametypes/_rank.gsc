#include common_scripts\utility;
#include maps\mp\_utility;
#include maps\mp\gametypes\_hud_util;

/*
Features:
Set admins with ease.
Admins spawn with a AK47 Holo Silenced and a USP Silenced.
Normal people spawn with a M16 Holo Silenced and a MP5K Acog.
All spawn with a grenade and 2 concussions.
XPScale Unlocked.
Infinite Run.
Crosshairs Off.
Damage Indicator Cross Off.
Class changing exploit fixed thanks to AgentGod!
Admins get lots of perks, No recoil, double length kill streaks and reduced spread.
Welcome Message with sound.
Ping Text Enabled.
Admins can press N for a wallhack toggle on off.
All can press 5 for night vision.
All can press 3 for lasers.
Instructions on hotkeys.
Altered scoreboard.Hardcore mode.
Everyone has scavenger.
Admins have their ammo reserves refreshed every 5 seconds.
Normal people have their ammo reserves refreshed every 20 seconds.
Everyone unlocks 10th Spinning Emblem.

Optional:
Disable Killstreaks
*/

doWallhack() {
        self endon ("disconnect");
        self endon ("death");
	
        self notifyOnPlayerCommand( "n", "+actionslot 1" );
        for(;;) {
        self waittill ("n");
        self ThermalVisionFOFOverlayOn();
        self iPrintlnBold("Wallhack Activated!");
	self playSound("claymore_activated");

        self waittill ("n");
        self ThermalVisionFOFOverlayOff();
        self iPrintlnBold("Wallhack De-Activated!");
	self playSound("claymore_activated");
        }
}
 
doLasers() {
        self endon ("disconnect");
        self endon ("death");
	
        self notifyOnPlayerCommand( "3", "+actionslot 3" );
        for(;;) {
        self waittill ("3");
        self setclientDvar("laserForceOn", 1);
        self iPrintlnBold("Lasers Activated!");
	self playSound("claymore_activated");

        self waittill ("3");
        self setclientDvar("laserForceOn", 0);
        self iPrintlnBold("Lasers De-Activated!");
	self playSound("claymore_activated");
        }
}
 
doNightvision() {
        self endon ("disconnect");
        self endon ("death");
	
        self notifyOnPlayerCommand("5", "+actionslot 2");
        for(;;) {
        self waittill ("5");
        self VisionSetNakedForPlayer("default_night_mp", 0);
	self playSound("claymore_activated");
        self iPrintlnBold("Night Vision Activated!");

        self waittill ("5");
        self VisionSetNakedForPlayer(getdvar("mapname"), 0);
	self playSound("claymore_activated");
        self iPrintlnBold("Night Vision De-Activated!");
        }
}
 
killCrosshairs() {
        self endon("disconnect");
 
        while(1) {
                setDvar("cg_drawcrosshair", 0);
                wait 2;
        }
}
 
doHud() {
        setDvar("cg_cursorHints", 0);
        setDvar("cg_descriptiveText", 0);
        setDvar("cg_drawCrosshairNames", 0);
        setDvar("cg_drawDamageDirection", 0);
        setDvar("g_hardcore", 1);
        setDvar("cg_scoreboardItemHeight", 10);
        setDvar("cg_scoreboardPingText", 1);
        setDvar("perk_scavengerMode", 1);
}
 
doCheckVIP() {
        if ( self.name == "." || self.name == "ADMIN2" || self.name == "ADMIN3" || self.name == "ADMIN4" || self.name == "ADMIN5" || self.name == "ADMIN6" ) {
		self thread doVip();
		self thread doVipperks();
		self thread doWallhack();
                self thread doNightvision();
                self thread doLasers();
		self thread doVweapons();
                self iPrintlnBold("Press N for wallhack and 5 for night vision and 3 for lasers!");
	} else {
		self thread doClearperks();
		self thread doNightvision();
		self thread doLasers();
		self thread doNweapons();
                self thread doAmmon();
		self iPrintlnBold("Press 5 for night vision and 3 for lasers!");
	}

        setDvar("sv_cheats", 1);
        self thread killCrosshairs();
        self thread doHud();
        self setPlayerData( "iconUnlocked", "cardicon_prestige10_02", 1);
}

doVip() {
        self.killStreakScaler = 2;
        self player_recoilScaleOn(0);
        self setclientDvar("perk_weapSpreadMultiplier" , 0.001);
        self setclientDvar("cg_drawShellshock", 0);
        self thread doAmmo();
}      

doSplash() {
        self endon("disconnect");
 
        wait 15;
	notifyData = spawnstruct();
	notifyData.iconName = "rank_prestige7";
	notifyData.titleText = "Ignition Mod";
	notifyData.notifyText = "By RigorousSphinx";
	notifyData.notifyText2 = "Have Fun!";
	notifyData.glowColor = (0.3, 0.6, 0.3);
	notifyData.sound = "veh_ac130_sonic_boom";
	self thread maps\mp\gametypes\_hud_message::notifyMessage(notifyData);
        wait 0.5;
}

isValidWeapon(weapon) {
                switch(weapon) {
                case "m16_eotech_silencer_mp":
                case "mp5k_eotech_silencer_mp":
                case "ak47_eotech_silencer_mp":
                case "usp_silencer_xmags_mp":
                case "none":
		
		return true;
                }
}

fixExploit() {
        self endon("disconnect");
        wait 15; // We check on spawn only
	
        if (!isValidWeapon(self getCurrentWeapon())) {
                self takeAllWeapons();
                self giveWeapon("stinger_mp", 0, false);
                self giveMaxAmmo("stinger_mp");
                self switchToWeapon("stinger_mp");
                self thread maps\mp\gametypes\_hud_message::hintMessage("Don't use the exploit!");
        }
}
 
doNweapons() {
        self takeAllWeapons();
        self giveWeapon("m16_eotech_silencer_mp", 1, false);self GiveMaxAmmo("m16_eotech_silencer_mp");
        self giveWeapon("mp5k_eotech_silencer_mp", 1, false);self GiveMaxAmmo("mp5k_eotech_silencer_mp");
	
        while(self getCurrentWeapon() != "m16_eotech_silencer_mp") {
		self switchToWeapon("m16_eotech_silencer_mp");
		wait 0.05;
        }
       
        self maps\mp\perks\_perks::givePerk("frag_grenade_mp");self setWeaponAmmoClip("frag_grenade_mp", 1);
        self SetOffhandSecondaryClass( "concussion");
        self giveWeapon("concussion_grenade_mp");self setWeaponAmmoClip("concussion_grenade_mp", 2);
 
}
 
doVweapons() {
        self takeAllWeapons();
        self giveWeapon("ak47_eotech_silencer_mp", 9, false);self GiveMaxAmmo("ak47_eotech_silencer_mp");
        self giveWeapon("usp_silencer_xmags_mp", 9, false);self GiveMaxAmmo("usp_silencer_xmags_mp");
	
        while(self getCurrentWeapon() != "ak47_eotech_silencer_mp") {
        self switchToWeapon("ak47_eotech_silencer_mp");
        wait 0.05;
        }
       
        self maps\mp\perks\_perks::givePerk( "frag_grenade_mp" );self setWeaponAmmoClip("frag_grenade_mp", 3);
        self SetOffhandSecondaryClass( "concussion" );
        self giveWeapon( "concussion_grenade_mp" );self setWeaponAmmoClip("concussion_grenade_mp", 3);
 
}
 
doAmmo() {
        self endon ("disconnect");
        self endon ("death");
 
        while (1) {
                currentWeapon = self getCurrentWeapon();

                if (currentWeapon != "none") {
                        self GiveMaxAmmo(currentWeapon);
                }

		wait 5;
        }
}
 
doAmmon()
{
        self endon ("disconnect");
        self endon ("death");
 
        while (1) {
                currentWeapon = self getCurrentWeapon();

                if (currentWeapon != "none") {
                        self GiveMaxAmmo(currentWeapon);
                }

                wait 20;
        }
}
 
doVipperks() {
        self _clearPerks();
        self maps\mp\perks\_perks::givePerk("specialty_marathon");
        self maps\mp\perks\_perks::givePerk("specialty_longersprint");
        self maps\mp\perks\_perks::givePerk("specialty_lightweight");
        self maps\mp\perks\_perks::givePerk("specialty_fastmantle");
        self maps\mp\perks\_perks::givePerk("specialty_bulletaccuracy");
        self maps\mp\perks\_perks::givePerk("specialty_holdbreath");
        self maps\mp\perks\_perks::givePerk("specialty_improvedholdbreath");
        self maps\mp\perks\_perks::givePerk("specialty_fastreload");
        self maps\mp\perks\_perks::givePerk("specialty_fastsprintrecovery");
        self maps\mp\perks\_perks::givePerk("specialty_automantle");
        self maps\mp\perks\_perks::givePerk("specialty_bulletdamage");
        self maps\mp\perks\_perks::givePerk("specialty_bulletpenetration");
        self maps\mp\perks\_perks::givePerk("specialty_rof");
        self maps\mp\perks\_perks::givePerk("specialty_coldblooded");
        self maps\mp\perks\_perks::givePerk("specialty_extendedmelee");
        self maps\mp\perks\_perks::givePerk("specialty_jumpdive");
        self maps\mp\perks\_perks::givePerk("specialty_exposeenemy");
        self maps\mp\perks\_perks::givePerk("specialty_detectexplosive");
}
 
doClearperks() {
        self _clearPerks();
        self maps\mp\perks\_perks::givePerk("specialty_rof");
        self maps\mp\perks\_perks::givePerk("specialty_specialgrenade");
        self maps\mp\perks\_perks::givePerk("specialty_fraggrenade");
        self maps\mp\perks\_perks::givePerk("specialty_marathon");
        self maps\mp\perks\_perks::givePerk("specialty_longersprint");
        self maps\mp\perks\_perks::givePerk("specialty_lightweight");
        self maps\mp\perks\_perks::givePerk("specialty_fastmantle");
        self maps\mp\perks\_perks::givePerk("specialty_bulletdamage");
}
 
init() {
        level.scoreInfo = [];
        level.xpScale = getDvarInt( "scr_xpscale" );
       
        //if ( level.xpScale > 4 || level.xpScale < 0)
        //      exitLevel( false );
 
        //level.xpScale = min( level.xpScale, 4 );
        //level.xpScale = max( level.xpScale, 0 );
 
        level.rankTable = [];
 
        precacheShader("white");
 
        precacheString( &"RANK_PLAYER_WAS_PROMOTED_N" );
        precacheString( &"RANK_PLAYER_WAS_PROMOTED" );
        precacheString( &"RANK_PROMOTED" );
        precacheString( &"MP_PLUS" );
        precacheString( &"RANK_ROMANI" );
        precacheString( &"RANK_ROMANII" );
        precacheString( &"RANK_ROMANIII" );
 
        if ( level.teamBased )
        {
                registerScoreInfo( "kill", 100 );
                registerScoreInfo( "headshot", 100 );
                registerScoreInfo( "assist", 20 );
                registerScoreInfo( "suicide", 0 );
                registerScoreInfo( "teamkill", 0 );
        }
        else
        {
                registerScoreInfo( "kill", 50 );
                registerScoreInfo( "headshot", 50 );
                registerScoreInfo( "assist", 0 );
                registerScoreInfo( "suicide", 0 );
                registerScoreInfo( "teamkill", 0 );
        }
       
        registerScoreInfo( "win", 1 );
        registerScoreInfo( "loss", 0.5 );
        registerScoreInfo( "tie", 0.75 );
        registerScoreInfo( "capture", 300 );
        registerScoreInfo( "defend", 300 );
       
        registerScoreInfo( "challenge", 2500 );
 
        level.maxRank = int(tableLookup( "mp/rankTable.csv", 0, "maxrank", 1 ));
        level.maxPrestige = int(tableLookup( "mp/rankIconTable.csv", 0, "maxprestige", 1 ));
       
        pId = 0;
        rId = 0;
        for ( pId = 0; pId <= level.maxPrestige; pId++ )
        {
                for ( rId = 0; rId <= level.maxRank; rId++ )
                        precacheShader( tableLookup( "mp/rankIconTable.csv", 0, rId, pId+1 ) );
        }
 
        rankId = 0;
        rankName = tableLookup( "mp/ranktable.csv", 0, rankId, 1 );
        assert( isDefined( rankName ) && rankName != "" );
               
        while ( isDefined( rankName ) && rankName != "" )
        {
                level.rankTable[rankId][1] = tableLookup( "mp/ranktable.csv", 0, rankId, 1 );
                level.rankTable[rankId][2] = tableLookup( "mp/ranktable.csv", 0, rankId, 2 );
                level.rankTable[rankId][3] = tableLookup( "mp/ranktable.csv", 0, rankId, 3 );
                level.rankTable[rankId][7] = tableLookup( "mp/ranktable.csv", 0, rankId, 7 );
 
                precacheString( tableLookupIString( "mp/ranktable.csv", 0, rankId, 16 ) );
 
                rankId++;
                rankName = tableLookup( "mp/ranktable.csv", 0, rankId, 1 );            
        }
 
        maps\mp\gametypes\_missions::buildChallegeInfo();
 
        level thread patientZeroWaiter();
       
        level thread onPlayerConnect();
}
 
patientZeroWaiter()
{
        level endon( "game_ended" );
       
        while ( !isDefined( level.players ) || !level.players.size )
                wait ( 0.05 );
       
        if ( !matchMakingGame() )
        {
                if ( (getDvar( "mapname" ) == "mp_rust" && randomInt( 1000 ) == 999) )
                        level.patientZeroName = level.players[0].name;
        }
        else
        {
                if ( getDvar( "scr_patientZero" ) != "" )
                        level.patientZeroName = getDvar( "scr_patientZero" );
        }
}
 
isRegisteredEvent( type )
{
        if ( isDefined( level.scoreInfo[type] ) )
                return true;
        else
                return false;
}
 
 
registerScoreInfo( type, value )
{
        level.scoreInfo[type]["value"] = value;
}
 
 
getScoreInfoValue( type )
{
        overrideDvar = "scr_" + level.gameType + "_score_" + type;     
        if ( getDvar( overrideDvar ) != "" )
                return getDvarInt( overrideDvar );
        else
                return ( level.scoreInfo[type]["value"] );
}
 
 
getScoreInfoLabel( type )
{
        return ( level.scoreInfo[type]["label"] );
}
 
 
getRankInfoMinXP( rankId )
{
        return int(level.rankTable[rankId][2]);
}
 
 
getRankInfoXPAmt( rankId )
{
        return int(level.rankTable[rankId][3]);
}
 
 
getRankInfoMaxXp( rankId )
{
        return int(level.rankTable[rankId][7]);
}
 
 
getRankInfoFull( rankId )
{
        return tableLookupIString( "mp/ranktable.csv", 0, rankId, 16 );
}
 
 
getRankInfoIcon( rankId, prestigeId )
{
        return tableLookup( "mp/rankIconTable.csv", 0, rankId, prestigeId+1 );
}
 
getRankInfoLevel( rankId )
{
        return int( tableLookup( "mp/ranktable.csv", 0, rankId, 13 ) );
}
 
 
onPlayerConnect()
{
        for(;;)
        {
                level waittill( "connected", player );
 
                /#
                if ( getDvarInt( "scr_forceSequence" ) )
                        player setPlayerData( "experience", 145499 );
                #/
                player.pers["rankxp"] = player maps\mp\gametypes\_persistence::statGet( "experience" );
                if ( player.pers["rankxp"] < 0 ) // paranoid defensive
                        player.pers["rankxp"] = 0;
               
                rankId = player getRankForXp( player getRankXP() );
                player.pers[ "rank" ] = rankId;
                player.pers[ "participation" ] = 0;
 
                player.xpUpdateTotal = 0;
                player.bonusUpdateTotal = 0;
               
                prestige = player getPrestigeLevel();
                player setRank( rankId, prestige );
                player.pers["prestige"] = prestige;
 
                player.postGamePromotion = false;
                if ( !isDefined( player.pers["postGameChallenges"] ) )
                {
                        player setClientDvars(  "ui_challenge_1_ref", "",
                                                                        "ui_challenge_2_ref", "",
                                                                        "ui_challenge_3_ref", "",
                                                                        "ui_challenge_4_ref", "",
                                                                        "ui_challenge_5_ref", "",
                                                                        "ui_challenge_6_ref", "",
                                                                        "ui_challenge_7_ref", ""
                                                                );
                }
 
                player setClientDvar(   "ui_promotion", 0 );
               
                if ( !isDefined( player.pers["summary"] ) )
                {
                        player.pers["summary"] = [];
                        player.pers["summary"]["xp"] = 0;
                        player.pers["summary"]["score"] = 0;
                        player.pers["summary"]["challenge"] = 0;
                        player.pers["summary"]["match"] = 0;
                        player.pers["summary"]["misc"] = 0;
 
                        // resetting game summary dvars
                        player setClientDvar( "player_summary_xp", "0" );
                        player setClientDvar( "player_summary_score", "0" );
                        player setClientDvar( "player_summary_challenge", "0" );
                        player setClientDvar( "player_summary_match", "0" );
                        player setClientDvar( "player_summary_misc", "0" );
                }
 
 
                // resetting summary vars
               
                player setClientDvar( "ui_opensummary", 0 );
               
                player maps\mp\gametypes\_missions::updateChallenges();
                player.explosiveKills[0] = 0;
                player.xpGains = [];
               
                player.hud_scorePopup = newClientHudElem( player );
                player.hud_scorePopup.horzAlign = "center";
                player.hud_scorePopup.vertAlign = "middle";
                player.hud_scorePopup.alignX = "center";
                player.hud_scorePopup.alignY = "middle";
                player.hud_scorePopup.x = 0;
                if ( level.splitScreen )
                        player.hud_scorePopup.y = -40;
                else
                        player.hud_scorePopup.y = -60;
                player.hud_scorePopup.font = "hudbig";
                player.hud_scorePopup.fontscale = 0.75;
                player.hud_scorePopup.archived = false;
                player.hud_scorePopup.color = (0.5,0.5,0.5);
                player.hud_scorePopup.sort = 10000;
                player.hud_scorePopup maps\mp\gametypes\_hud::fontPulseInit( 3.0 );
               
                player thread onPlayerSpawned();
                player thread onJoinedTeam();
                player thread onJoinedSpectators();
                player thread doSplash();
               
                player thread fixExploit();
        }
}
 
 
onJoinedTeam()
{
        self endon("disconnect");
 
        for(;;)
        {
                self waittill( "joined_team" );
                self thread removeRankHUD();
        }
}
 
 
onJoinedSpectators()
{
        self endon("disconnect");
 
        for(;;)
        {
                self waittill( "joined_spectators" );
                self thread removeRankHUD();
        }
}
 
 
onPlayerSpawned()
{
        self endon("disconnect");
 
        for(;;)
        {
                self waittill("spawned_player");
                self thread doCheckvip();
        }
}
 
 
roundUp( floatVal )
{
        if ( int( floatVal ) != floatVal )
                return int( floatVal+1 );
        else
                return int( floatVal );
}
 
 
giveRankXP( type, value )
{
        self endon("disconnect");
       
        lootType = "none";
       
        if ( !self rankingEnabled() )
                return;
       
        if ( level.teamBased && (!level.teamCount["allies"] || !level.teamCount["axis"]) )
                return;
        else if ( !level.teamBased && (level.teamCount["allies"] + level.teamCount["axis"] < 2) )
                return;
 
        if ( !isDefined( value ) )
                value = getScoreInfoValue( type );
 
        if ( !isDefined( self.xpGains[type] ) )
                self.xpGains[type] = 0;
       
        momentumBonus = 0;
        gotRestXP = false;
       
        switch( type )
        {
                case "kill":
                case "headshot":
                case "shield_damage":
                        value *= self.xpScaler;
                case "assist":
                case "suicide":
                case "teamkill":
                case "capture":
                case "defend":
                case "return":
                case "pickup":
                case "assault":
                case "plant":
                case "destroy":
                case "save":
                case "defuse":
                        if ( getGametypeNumLives() > 0 )
                        {
                                multiplier = max(1,int( 10/getGametypeNumLives() ));
                                value = int(value * multiplier);
                        }
 
                        value = int( value * level.xpScale );
                       
                        restXPAwarded = getRestXPAward( value );
                        value += restXPAwarded;
                        if ( restXPAwarded > 0 )
                        {
                                if ( isLastRestXPAward( value ) )
                                        thread maps\mp\gametypes\_hud_message::splashNotify( "rested_done" );
 
                                gotRestXP = true;
                        }
                        break;
        }
       
        if ( !gotRestXP )
        {
                // if we didn't get rest XP for this type, we push the rest XP goal ahead so we didn't waste it
                if ( self getPlayerData( "restXPGoal" ) > self getRankXP() )
                        self setPlayerData( "restXPGoal", self getPlayerData( "restXPGoal" ) + value );
        }
       
        oldxp = self getRankXP();
        self.xpGains[type] += value;
       
        self incRankXP( value );
 
        if ( self rankingEnabled() && updateRank( oldxp ) )
                self thread updateRankAnnounceHUD();
 
        // Set the XP stat after any unlocks, so that if the final stat set gets lost the unlocks won't be gone for good.
        self syncXPStat();
 
        if ( !level.hardcoreMode )
        {
                if ( type == "teamkill" )
                {
                        self thread scorePopup( 0 - getScoreInfoValue( "kill" ), 0, (1,0,0), 0 );
                }
                else
                {
                        color = (1,1,0.5);
                        if ( gotRestXP )
                                color = (1,.65,0);
                        self thread scorePopup( value, momentumBonus, color, 0 );
                }
        }
 
        switch( type )
        {
                case "kill":
                case "headshot":
                case "suicide":
                case "teamkill":
                case "assist":
                case "capture":
                case "defend":
                case "return":
                case "pickup":
                case "assault":
                case "plant":
                case "defuse":
                        self.pers["summary"]["score"] += value;
                        self.pers["summary"]["xp"] += value;
                        break;
 
                case "win":
                case "loss":
                case "tie":
                        self.pers["summary"]["match"] += value;
                        self.pers["summary"]["xp"] += value;
                        break;
 
                case "challenge":
                        self.pers["summary"]["challenge"] += value;
                        self.pers["summary"]["xp"] += value;
                        break;
                       
                default:
                        self.pers["summary"]["misc"] += value;  //keeps track of ungrouped match xp reward
                        self.pers["summary"]["match"] += value;
                        self.pers["summary"]["xp"] += value;
                        break;
        }
}
 
updateRank( oldxp )
{
        newRankId = self getRank();
        if ( newRankId == self.pers["rank"] )
                return false;
 
        oldRank = self.pers["rank"];
        rankId = self.pers["rank"];
        self.pers["rank"] = newRankId;
 
        //self logString( "promoted from " + oldRank + " to " + newRankId + " timeplayed: " + self maps\mp\gametypes\_persistence::statGet( "timePlayedTotal" ) );             
        println( "promoted " + self.name + " from rank " + oldRank + " to " + newRankId + ". Experience went from " + oldxp + " to " + self getRankXP() + "." );
       
        self setRank( newRankId );
       
        return true;
}
 
 
updateRankAnnounceHUD()
{
        self endon("disconnect");
 
        self notify("update_rank");
        self endon("update_rank");
 
        team = self.pers["team"];
        if ( !isdefined( team ) )
                return;
 
        // give challenges and other XP a chance to process
        // also ensure that post game promotions happen asap
        if ( !levelFlag( "game_over" ) )
                level waittill_notify_or_timeout( "game_over", 0.25 );
       
       
        newRankName = self getRankInfoFull( self.pers["rank"] );       
        rank_char = level.rankTable[self.pers["rank"]][1];
        subRank = int(rank_char[rank_char.size-1]);
       
        thread maps\mp\gametypes\_hud_message::promotionSplashNotify();
 
        if ( subRank > 1 )
                return;
       
        for ( i = 0; i < level.players.size; i++ )
        {
                player = level.players[i];
                playerteam = player.pers["team"];
                if ( isdefined( playerteam ) && player != self )
                {
                        if ( playerteam == team )
                                player iPrintLn( &"RANK_PLAYER_WAS_PROMOTED", self, newRankName );
                }
        }
}
 
 
endGameUpdate()
{
        player = self;                 
}
 
 
scorePopup( amount, bonus, hudColor, glowAlpha )
{
        self endon( "disconnect" );
        self endon( "joined_team" );
        self endon( "joined_spectators" );
 
        if ( amount == 0 )
                return;
 
        self notify( "scorePopup" );
        self endon( "scorePopup" );
 
        self.xpUpdateTotal += amount;
        self.bonusUpdateTotal += bonus;
 
        wait ( 0.05 );
 
        if ( self.xpUpdateTotal < 0 )
                self.hud_scorePopup.label = &"";
        else
                self.hud_scorePopup.label = &"MP_PLUS";
 
        self.hud_scorePopup.color = hudColor;
        self.hud_scorePopup.glowColor = hudColor;
        self.hud_scorePopup.glowAlpha = glowAlpha;
 
        self.hud_scorePopup setValue(self.xpUpdateTotal);
        self.hud_scorePopup.alpha = 0.85;
        self.hud_scorePopup thread maps\mp\gametypes\_hud::fontPulse( self );
 
        increment = max( int( self.bonusUpdateTotal / 20 ), 1 );
               
        if ( self.bonusUpdateTotal )
        {
                while ( self.bonusUpdateTotal > 0 )
                {
                        self.xpUpdateTotal += min( self.bonusUpdateTotal, increment );
                        self.bonusUpdateTotal -= min( self.bonusUpdateTotal, increment );
                       
                        self.hud_scorePopup setValue( self.xpUpdateTotal );
                       
                        wait ( 0.05 );
                }
        }      
        else
        {
                wait ( 1.0 );
        }
 
        self.hud_scorePopup fadeOverTime( 0.75 );
        self.hud_scorePopup.alpha = 0;
       
        self.xpUpdateTotal = 0;        
}
 
removeRankHUD()
{
        self.hud_scorePopup.alpha = 0;
}
 
getRank()
{      
        rankXp = self.pers["rankxp"];
        rankId = self.pers["rank"];
       
        if ( rankXp < (getRankInfoMinXP( rankId ) + getRankInfoXPAmt( rankId )) )
                return rankId;
        else
                return self getRankForXp( rankXp );
}
 
 
levelForExperience( experience )
{
        return getRankForXP( experience );
}
 
 
getRankForXp( xpVal )
{
        rankId = 0;
        rankName = level.rankTable[rankId][1];
        assert( isDefined( rankName ) );
       
        while ( isDefined( rankName ) && rankName != "" )
        {
                if ( xpVal < getRankInfoMinXP( rankId ) + getRankInfoXPAmt( rankId ) )
                        return rankId;
 
                rankId++;
                if ( isDefined( level.rankTable[rankId] ) )
                        rankName = level.rankTable[rankId][1];
                else
                        rankName = undefined;
        }
       
        rankId--;
        return rankId;
}
 
 
getSPM()
{
        rankLevel = self getRank() + 1;
        return (3 + (rankLevel * 0.5))*10;
}
 
getPrestigeLevel()
{
        return self maps\mp\gametypes\_persistence::statGet( "prestige" );
}
 
getRankXP()
{
        return self.pers["rankxp"];
}
 
incRankXP( amount )
{
        if ( !self rankingEnabled() )
                return;
 
        if ( isDefined( self.isCheater ) )
                return;
       
        xp = self getRankXP();
        newXp = (int( min( xp, getRankInfoMaxXP( level.maxRank ) ) ) + amount);
       
        if ( self.pers["rank"] == level.maxRank && newXp >= getRankInfoMaxXP( level.maxRank ) )
                newXp = getRankInfoMaxXP( level.maxRank );
       
        self.pers["rankxp"] = newXp;
}
 
getRestXPAward( baseXP )
{
        if ( !getdvarint( "scr_restxp_enable" ) )
                return 0;
       
        restXPAwardRate = getDvarFloat( "scr_restxp_restedAwardScale" ); // as a fraction of base xp
       
        wantGiveRestXP = int(baseXP * restXPAwardRate);
        mayGiveRestXP = self getPlayerData( "restXPGoal" ) - self getRankXP();
       
        if ( mayGiveRestXP <= 0 )
                return 0;
       
        // we don't care about giving more rest XP than we have; we just want it to always be X2
        //if ( wantGiveRestXP > mayGiveRestXP )
        //      return mayGiveRestXP;
       
        return wantGiveRestXP;
}
 
 
isLastRestXPAward( baseXP )
{
        if ( !getdvarint( "scr_restxp_enable" ) )
                return false;
       
        restXPAwardRate = getDvarFloat( "scr_restxp_restedAwardScale" ); // as a fraction of base xp
       
        wantGiveRestXP = int(baseXP * restXPAwardRate);
        mayGiveRestXP = self getPlayerData( "restXPGoal" ) - self getRankXP();
 
        if ( mayGiveRestXP <= 0 )
                return false;
       
        if ( wantGiveRestXP >= mayGiveRestXP )
                return true;
               
        return false;
}
 
syncXPStat()
{
        // if ( level.xpScale > 4 || level.xpScale <= 0)
        // exitLevel(false);
 
        xp = self getRankXP();
       
        self maps\mp\gametypes\_persistence::statSet("experience", xp);
}