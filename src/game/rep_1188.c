#include "game/rep_1188.h"
#include "header_rep_data.h"

#include "game/UnknownHomes.h"
extern u8 aILevel[4];
// .text:0x0006D6D4 size:0x290 mapped:0x806AC768
void fn_3_6D6D4(void) {
    return;
}

// .text:0x0006D964 size:0x4FC mapped:0x806AC9F8
void fn_3_6D964(void) {
    return;
}

// .text:0x0006DE60 size:0x374 mapped:0x806ACEF4
void setInMemBatterConstants(int rosterID)
{
    int battingOrderCounter;
    int index;
    int batterID;

    CharacterStats *char_stats = &inMemRoster[gameControls.teamBatting][rosterID];
    ChallengeTrackingStruct *starMissions = starMissionCompletionTracker; 
    inMemBatter.easyBatting = 0;

    if (gameInitVariables.GameModeSelected == GAME_TYPE_PRACTICE) {
        if (practiceStruct.practiceType_2 == 0 || practiceStruct.practiceType_2 == 1 || practiceStruct.practiceType_2 == 2 || practiceStruct.practiceType_2 == 3) {
            char_stats = inMemRoster[gameControls.teamBatting];
        }
    } else if (gameInitVariables.minigamesEnabled) {
        rosterID = minigameStruct.minigameControlStruct[0].characterIndex[minigameStruct.minigamesRosterIDOfCurrentPlayer];
        char_stats = &inMemRoster[0][rosterID];
    } else {
        inMemBatter.easyBatting = gameInitOptions.controlOptions[gameInitVariables.PlayerPorts[gameControls.teamBatting] % 4].easyBatting;
    }
    
    inMemBatter.rosterID = rosterID;
    inMemBatter.charID = char_stats->stats.CharID;
    inMemBatter.slapContactSize_raw = char_stats->stats.SlapContactSize;
    inMemBatter.chargeContactSize_raw = char_stats->stats.ChargeContactSize;
    inMemBatter.slapHitPower_raw = char_stats->stats.SlapHitPower;
    inMemBatter.chargeHitPower_raw = char_stats->stats.ChargeHitPower;
    inMemBatter.buntingContactSize = char_stats->stats.BuntingContactSize;
    inMemBatter.trajectoryPushPull = char_stats->stats.HitTrajectoryPushPull;
    inMemBatter.trajectoryHighLow = char_stats->stats.HitTrajectoryHighLow;
    inMemBatter.batterHand = char_stats->stats.BattingStance;
    inMemBatter.characterClass = char_stats->stats.CharacterClass;
    inMemBatter.trimmedBat = BatterHitbox[inMemBatter.charID].TrimmedBat;
    inMemBatter.chemLinksOnBase = 0;
    inMemBatter.aiLevel = gameControls.AIDifficulty0Special3Weak[gameControls.homeTeamBattingInd_fieldingTeam];
    
    if (gameInitVariables.minigamesEnabled) {
        inMemBatter.captainStarHitPitch = 0;
        inMemBatter.noncaptainStarSwing = 0;
    } else {
        inMemBatter.captainStarHitPitch = (char_stats->stats).CaptainStarHitPitch;
        inMemBatter.noncaptainStarSwing = (char_stats->stats).NonCaptainStarSwing;
    }
    
    if (!gameInitVariables.exhibitionMatchInd) {
        inMemBatter.charIDForScoutFlagMission = -1;
        for (index = 0; index < 54; index++) {
            if ((inMemBatter.charID == index) && (starMissions[index].variantClassification <= 3)) {
                inMemBatter.charIDForScoutFlagMission = index;
                break;
            }
        }
    }
    
    if (gameInitVariables.minigamesEnabled) {       
        s8 bVar1;
        // This should match target's struct access pattern
        if (minigameStruct.battingHandedness[rosterID] & 1) {
            inMemBatter.batterHand = 1;
        } else {
            inMemBatter.batterHand = 0;
        }

        bVar1 = minigameStruct.minigameControlStruct[0].characterIndex[rosterID];
        
        if (minigameStruct.minigameControlStruct[0].battingHandedness[bVar1] != 0) {
            inMemPitcher.aiLevel = aILevel[minigameStruct.minigameControlStruct[0].aIStrength[bVar1]];
        }
    }
}

// .text:0x0006E1D4 size:0x78 mapped:0x806AD268
void fn_3_6E1D4(void) {
    return;
}

// .text:0x0006E24C size:0x968 mapped:0x806AD2E0
void fn_3_6E24C(void) {
    return;
}

// .text:0x0006EBB4 size:0x368 mapped:0x806ADC48
void fn_3_6EBB4(void) {
    return;
}

// .text:0x0006EF1C size:0x5CC mapped:0x806ADFB0
void fn_3_6EF1C(void) {
    return;
}
