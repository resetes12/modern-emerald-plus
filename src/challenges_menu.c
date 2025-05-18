#include "global.h"
#include "challenges_menu.h"
#include "bg.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "main.h"
#include "menu.h"
#include "palette.h"
#include "scanline_effect.h"
#include "sprite.h"
#include "strings.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "window.h"
#include "gba/m4a_internal.h"
#include "constants/rgb.h"
#include "string_util.h"

#define tMenuSelection   data[0]
//NEW OPTIONS MENU
//Page 1: Options
#define tTextSpeed       data[1]
#define tFollower        data[2]
#define tBigFollower     data[3]
#define tButtonMode      data[4]
#define tMatchCalls      data[5]
#define tFastHeal        data[6]
#define tWindowFrameType data[7] //default 6

//Page 2: Options 2
#define tAutoRun         data[8]
#define tAutoRunSurf     data[9]
#define tAutoRunDive     data[10]
#define tUnitSystem      data[11]
#define tSkipIntro       data[12]

//Page 3: Battle
#define tFastBattles       data[13] //Intro slide
#define tBattleScene       data[14] //Battle Anims
#define tDifficulty        data[15]
#define tBattleStyle       data[16]
#define tBallPrompt        data[17]
#define tFastRun           data[18]
#define tFastRunPrompt     data[19]

//Page 4: Sound
#define tSound             data[20] //default 4
#define tMusic             data[21]
#define tBikeSurfMusic     data[22]
#define tWildMusic         data[23]
#define tTrainerMusic      data[24]
#define tFrontierMusic     data[25]
#define tSoundEffects      data[26]

// Menu items Pg1: Options 1
enum
{
    MENUITEM_TEXTSPEED,
    MENUITEM_BUTTONMODE,
    MENUITEM_FOLLOWER,
    MENUITEM_BIG_FOLLOWER,
    MENUITEM_MATCH_CALLS,
    MENUITEM_FAST_HEAL,
    MENUITEM_FRAMETYPE,
    MENUITEM_COUNT,
};

// Menu items Pg2: Options 1
enum
{
    MENUITEM_AUTORUN,
    MENUITEM_AUTORUN_SURF,
    MENUITEM_AUTORUN_DIVE,
    MENUITEM_UNIT_SYSTEM,
    MENUITEM_SKIP_INTRO,
    MENUITEM_COUNT_PG2,
};

// Menu items Pg3: Battles
enum
{
    MENUITEM_DIFFICULTY,
    MENUITEM_BATTLESTYLE,
    MENUITEM_FAST_BATTLES,
    MENUITEM_BATTLESCENE,
    MENUITEM_BALL_PROMPT,
    MENUITEM_FAST_RUN,
    MENUITEM_FAST_RUN_PROMPT,
    MENUITEM_COUNT_PG3,
};

// Menu items Pg4: Sound
enum
{
    MENUITEM_SOUND,
    MENUITEM_MUSIC,
    MENUITEM_BIKE_SURF_MUSIC,
    MENUITEM_WILD_MUSIC,
    MENUITEM_TRAINER_MUSIC,
    MENUITEM_FRONTIER_MUSIC,
    MENUITEM_SOUND_EFFECTS,
    MENUITEM_COUNT_PG4,
};

enum
{
    WIN_HEADER,
    WIN_OPTIONS
};

//Pg1
#define YPOS_TEXTSPEED            (MENUITEM_TEXTSPEED * 16)
#define YPOS_BUTTONMODE           (MENUITEM_BUTTONMODE * 16)
#define YPOS_FOLLOWER             (MENUITEM_FOLLOWER * 16)
#define YPOS_BIG_FOLLOWER         (MENUITEM_BIG_FOLLOWER * 16)
#define YPOS_FAST_HEAL            (MENUITEM_FAST_HEAL * 16)
#define YPOS_FRAMETYPE            (MENUITEM_FRAMETYPE * 16)
#define YPOS_MATCH_CALLS          (MENUITEM_MATCH_CALLS * 16)
//Pg2
#define YPOS_AUTORUN              (MENUITEM_AUTORUN * 16)
#define YPOS_AUTORUN_SURF         (MENUITEM_AUTORUN_SURF * 16)
#define YPOS_AUTORUN_DIVE         (MENUITEM_AUTORUN_DIVE * 16)
#define YPOS_UNIT_SYSTEM          (MENUITEM_UNIT_SYSTEM * 16)
#define YPOS_SKIP_INTRO           (MENUITEM_SKIP_INTRO * 16)
//Pg3
#define YPOS_DIFFICULTY           (MENUITEM_DIFFICULTY * 16)
#define YPOS_BATTLESTYLE          (MENUITEM_BATTLESTYLE * 16)
#define YPOS_FAST_BATTLES         (MENUITEM_FAST_BATTLES * 16)
#define YPOS_BATTLESCENE          (MENUITEM_BATTLESCENE * 16)
#define YPOS_BALL_PROMPT          (MENUITEM_BALL_PROMPT * 16)
#define YPOS_FAST_RUN             (MENUITEM_FAST_RUN * 16)
#define YPOS_FAST_RUN_PROMPT      (MENUITEM_FAST_RUN_PROMPT * 16)

//Pg4
#define YPOS_SOUND                (MENUITEM_SOUND * 16)
#define YPOS_MUSIC                (MENUITEM_MUSIC * 16)
#define YPOS_BIKE_SURF_MUSIC      (MENUITEM_BIKE_SURF_MUSIC * 16)
#define YPOS_WILD_MUSIC           (MENUITEM_WILD_MUSIC * 16)
#define YPOS_TRAINER_MUSIC        (MENUITEM_TRAINER_MUSIC * 16)
#define YPOS_FRONTIER_MUSIC       (MENUITEM_FRONTIER_MUSIC * 16)
#define YPOS_SOUND_EFFECTS        (MENUITEM_SOUND_EFFECTS * 16)

#define PAGE_COUNT  4

static void Task_OptionMenuFadeIn(u8 taskId);
static void Task_OptionMenuProcessInput(u8 taskId);
static void Task_OptionMenuSave(u8 taskId);
static void Task_OptionMenuFadeOut(u8 taskId);
static void HighlightOptionMenuItem(u8 selection);
static u8 TextSpeed_ProcessInput(u8 selection);
static void TextSpeed_DrawChoices(u8 selection);
static u8 BattleScene_ProcessInput(u8 selection);
static void BattleScene_DrawChoices(u8 selection);
static u8 BattleStyle_ProcessInput(u8 selection);
static void BattleStyle_DrawChoices(u8 selection);
static u8 Sound_ProcessInput(u8 selection);
static void Sound_DrawChoices(u8 selection);
static u8 FrameType_ProcessInput(u8 selection);
static void FrameType_DrawChoices(u8 selection);
static u8 ButtonMode_ProcessInput(u8 selection);
static void ButtonMode_DrawChoices(u8 selection);
static void DrawHeaderText(void);
static void DrawOptionMenuTexts(void);
static void DrawBgWindowFrames(void);
//NEW OPTIONS MENU
static void Task_OptionMenuSaveNoFade(u8 taskId);
static void Task_OptionMenuFadeIn_Pg2(u8 taskId);
static void Task_OptionMenuProcessInput_Pg2(u8 taskId);
static void Task_OptionMenuFadeIn_Pg3(u8 taskId);
static void Task_OptionMenuProcessInput_Pg3(u8 taskId);
static void Task_OptionMenuFadeIn_Pg4(u8 taskId);
static void Task_OptionMenuProcessInput_Pg4(u8 taskId);
static int GetMiddleX(const u8 *txt1, const u8 *txt2, const u8 *txt3);
//Difficulty
static u8   Difficulty_ProcessInput(u8 selection);
static void Difficulty_DrawChoices(u8 selection);
//Follower
static u8   Follower_ProcessInput(u8 selection);
static void Follower_DrawChoices(u8 selection);
//Big Follower
static u8   BigFollower_ProcessInput(u8 selection);
static void BigFollower_DrawChoices(u8 selection);
//Autorun
static u8   Autorun_ProcessInput(u8 selection);
static void Autorun_DrawChoices(u8 selection);
//Autorun surf
static u8   AutorunSurf_ProcessInput(u8 selection);
static void AutorunSurf_DrawChoices(u8 selection);
//Autorun dive
static u8   AutorunDive_ProcessInput(u8 selection);
static void AutorunDive_DrawChoices(u8 selection);
//Unit System
static u8   UnitSystem_ProcessInput(u8 selection);
static void UnitSystem_DrawChoices(u8 selection);
//Match Calls
static u8   MatchCalls_ProcessInput(u8 selection);
static void MatchCalls_DrawChoices(u8 selection);
//Skip Intro
static u8   SkipIntro_ProcessInput(u8 selection);
static void SkipIntro_DrawChoices(u8 selection);
//Fast Battles
static u8   FastBattles_ProcessInput(u8 selection);
static void FastBattles_DrawChoices(u8 selection);
//Ball prompt
static u8   BallPrompt_ProcessInput(u8 selection);
static void BallPrompt_DrawChoices(u8 selection);
//Fast Run
static u8   FastRun_ProcessInput(u8 selection);
static void FastRun_DrawChoices(u8 selection);
//Fast Run Prompt
static u8   FastRunPrompt_ProcessInput(u8 selection);
static void FastRunPrompt_DrawChoices(u8 selection);
//Fast Heal
static u8   FastHeal_ProcessInput(u8 selection);
static void FastHeal_DrawChoices(u8 selection);
//Music
static u8   Music_ProcessInput(u8 selection);
static void Music_DrawChoices(u8 selection);
//Bike Surf Music
static u8   BikeSurfMusic_ProcessInput(u8 selection);
static void BikeSurfMusic_DrawChoices(u8 selection);
//Wild Music
static u8   WildMusic_ProcessInput(u8 selection);
static void WildMusic_DrawChoices(int selection, int y);
static void WildMusic_DrawChoices_Six(const u8 *const *const strings, int selection, int y);
//Trainer Music
static u8   TrainerMusic_ProcessInput(u8 selection);
static void TrainerMusic_DrawChoices(u8 selection);
static void TrainerMusic_DrawChoices_Six(const u8 *const *const strings, int selection, int y);
//Frontier Music
static u8   FrontierMusic_ProcessInput(u8 selection);
static void FrontierMusic_DrawChoices(u8 selection);
static void FrontierMusic_DrawChoices_Six(const u8 *const *const strings, int selection, int y);
//Sound Effects
static u8   SoundEffects_ProcessInput(u8 selection);
static void SoundEffects_DrawChoices(u8 selection);


EWRAM_DATA static bool8 sArrowPressed = FALSE;
EWRAM_DATA static u8 sCurrPage = 0;

static const u16 sOptionMenuText_Pal[] = INCBIN_U16("graphics/interface/option_menu_text.gbapal");
// note: this is only used in the Japanese release
static const u8 sEqualSignGfx[] = INCBIN_U8("graphics/interface/option_menu_equals_sign.4bpp");

static const u8 *const sOptionMenuItemsNames[MENUITEM_COUNT] =
{
    [MENUITEM_TEXTSPEED]       = gText_TextSpeed,
    [MENUITEM_BUTTONMODE]      = gText_ButtonMode,
    [MENUITEM_FOLLOWER]        = gText_Follower,
    [MENUITEM_BIG_FOLLOWER]    = gText_Big_Follower,
    [MENUITEM_MATCH_CALLS]     = gText_MatchCalls,
    [MENUITEM_FAST_HEAL]       = gText_FastHeal,
    [MENUITEM_FRAMETYPE]       = gText_Frame,
};

static const u8 *const sOptionMenuItemsNames_Pg2[MENUITEM_COUNT_PG2] =
{

    [MENUITEM_AUTORUN]         = gText_Autorun,
    [MENUITEM_AUTORUN_SURF]    = gText_AutorunSurf,
    [MENUITEM_AUTORUN_DIVE]    = gText_AutorunDive,
    [MENUITEM_UNIT_SYSTEM]     = gText_UnitSystem,
    [MENUITEM_SKIP_INTRO]      = gText_SkipIntro,
};

static const u8 *const sOptionMenuItemsNames_Pg3[MENUITEM_COUNT_PG3] =
{
    [MENUITEM_DIFFICULTY]      = gText_Difficulty,
    [MENUITEM_BATTLESTYLE]     = gText_BattleStyle,
    [MENUITEM_FAST_BATTLES]    = gText_FastBattles,
    [MENUITEM_BATTLESCENE]     = gText_BattleScene,
    [MENUITEM_BALL_PROMPT]     = gText_BallPrompt,
    [MENUITEM_FAST_RUN]        = gText_FastRun,
    [MENUITEM_FAST_RUN_PROMPT] = gText_FastRunPrompt,
};

static const u8 *const sOptionMenuItemsNames_Pg4[MENUITEM_COUNT_PG4] =
{
    [MENUITEM_SOUND]                   = gText_Sound,
    [MENUITEM_MUSIC]                   = gText_Music,
    [MENUITEM_BIKE_SURF_MUSIC]         = gText_SurfBikeMusic,
    [MENUITEM_WILD_MUSIC]              = gText_WildMusic,
    [MENUITEM_TRAINER_MUSIC]           = gText_TrainerMusic,
    [MENUITEM_FRONTIER_MUSIC]          = gText_FrontierMusic,
    [MENUITEM_SOUND_EFFECTS]           = gText_SoundEffects,
};


static const struct WindowTemplate sOptionMenuWinTemplates[] =
{
    [WIN_HEADER] = {
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 1,
        .width = 26,
        .height = 2,
        .paletteNum = 1,
        .baseBlock = 2
    },
    [WIN_OPTIONS] = {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 5,
        .width = 26,
        .height = 14,
        .paletteNum = 1,
        .baseBlock = 0x36
    },
    DUMMY_WIN_TEMPLATE
};

static const struct BgTemplate sOptionMenuBgTemplates[] =
{
    {
        .bg = 1,
        .charBaseIndex = 1,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = 0,
        .charBaseIndex = 1,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0
    }
};

static const u16 sOptionMenuBg_Pal[] = {RGB(17, 18, 31)};

static void MainCB2(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}
//New options menu
static void ReadAllCurrentSettings(u8 taskId)
{
    gTasks[taskId].tMenuSelection = 0;
    //pg1
    gTasks[taskId].tTextSpeed = gSaveBlock2Ptr->optionsTextSpeed;
    gTasks[taskId].tButtonMode = gSaveBlock2Ptr->optionsButtonMode;
    gTasks[taskId].tFollower = gSaveBlock2Ptr->optionsFollower;
    gTasks[taskId].tBigFollower = gSaveBlock2Ptr->optionsBigFollower;
    gTasks[taskId].tMatchCalls = gSaveBlock2Ptr->optionsMatchCalls;
    gTasks[taskId].tFastHeal = gSaveBlock2Ptr->optionsFastHeal;
    gTasks[taskId].tWindowFrameType = gSaveBlock2Ptr->optionsWindowFrameType;
    //pg2
    gTasks[taskId].tAutoRun = gSaveBlock2Ptr->optionsAutorun;
    gTasks[taskId].tAutoRunSurf = gSaveBlock2Ptr->optionsAutorunSurf;
    gTasks[taskId].tAutoRunDive = gSaveBlock2Ptr->optionsAutorunDive;
    gTasks[taskId].tUnitSystem = gSaveBlock2Ptr->optionsUnitSystem;
    gTasks[taskId].tSkipIntro = gSaveBlock2Ptr->optionsSkipIntro;
    //pg3
    gTasks[taskId].tBattleScene = gSaveBlock2Ptr->optionsBattleScene;
    gTasks[taskId].tBattleStyle = gSaveBlock2Ptr->optionsBattleStyle;
    //gTasks[taskId].tDifficulty = gSaveBlock2Ptr->;
    gTasks[taskId].tFastBattles = gSaveBlock2Ptr->optionsFastBattles;
    gTasks[taskId].tBallPrompt = gSaveBlock2Ptr->optionsBallPrompt;
    gTasks[taskId].tFastRun = gSaveBlock2Ptr->optionsFastRun;
    gTasks[taskId].tFastRunPrompt = gSaveBlock2Ptr->optionsFastRunPrompt;
    //pg4
    gTasks[taskId].tSound = gSaveBlock2Ptr->optionsSound;
    gTasks[taskId].tMusic = gSaveBlock2Ptr->optionsMusic;
    gTasks[taskId].tBikeSurfMusic = gSaveBlock2Ptr->optionsBikeSurfMusic;
    gTasks[taskId].tWildMusic = gSaveBlock2Ptr->optionsWildMusic;
    gTasks[taskId].tFrontierMusic = gSaveBlock2Ptr->optionsFrontierMusic;
    gTasks[taskId].tTrainerMusic = gSaveBlock2Ptr->optionsTrainerMusic;
    gTasks[taskId].tSoundEffects = gSaveBlock2Ptr->optionsSoundEffects;
}

static void DrawOptionsPg1(u8 taskId)
{  
    ReadAllCurrentSettings(taskId);

    TextSpeed_DrawChoices(gTasks[taskId].tTextSpeed);
    ButtonMode_DrawChoices(gTasks[taskId].tButtonMode);
    Follower_DrawChoices(gTasks[taskId].tFollower);
    BigFollower_DrawChoices(gTasks[taskId].tBigFollower);
    MatchCalls_DrawChoices(gTasks[taskId].tMatchCalls);
    FastHeal_DrawChoices(gTasks[taskId].tFastHeal);
    FrameType_DrawChoices(gTasks[taskId].tWindowFrameType);

    HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_FULL);
}

static void DrawOptionsPg2(u8 taskId)
{
    ReadAllCurrentSettings(taskId);

    Autorun_DrawChoices(gTasks[taskId].tAutoRun);
    AutorunSurf_DrawChoices(gTasks[taskId].tAutoRunSurf);
    AutorunDive_DrawChoices(gTasks[taskId].tAutoRunDive);
    UnitSystem_DrawChoices(gTasks[taskId].tUnitSystem);
    SkipIntro_DrawChoices(gTasks[taskId].tSkipIntro);
    
    HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_FULL);
}

static void DrawOptionsPg3(u8 taskId)
{
    ReadAllCurrentSettings(taskId);

    BattleScene_DrawChoices(gTasks[taskId].tBattleScene);
    BattleStyle_DrawChoices(gTasks[taskId].tBattleStyle);
    Difficulty_DrawChoices(gTasks[taskId].tDifficulty);
    FastBattles_DrawChoices(gTasks[taskId].tFastBattles);
    BallPrompt_DrawChoices(gTasks[taskId].tBallPrompt);
    FastRun_DrawChoices(gTasks[taskId].tFastRun);
    FastRunPrompt_DrawChoices(gTasks[taskId].tFastRunPrompt);

    HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_FULL);
}

static void DrawOptionsPg4(u8 taskId)
{
    ReadAllCurrentSettings(taskId);

    Sound_DrawChoices(gTasks[taskId].tSound);
    Music_DrawChoices(gTasks[taskId].tMusic);
    BikeSurfMusic_DrawChoices(gTasks[taskId].tBikeSurfMusic);
    WildMusic_DrawChoices(gTasks[taskId].tWildMusic, YPOS_WILD_MUSIC);
    TrainerMusic_DrawChoices(gTasks[taskId].tTrainerMusic);
    FrontierMusic_DrawChoices(gTasks[taskId].tFrontierMusic);
    SoundEffects_DrawChoices(gTasks[taskId].tSoundEffects);

    HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_FULL);
    
}

void CB2_InitChallengesMenu(void)
{
    u8 taskId;
    switch (gMain.state)
    {
    default:
    case 0:
        SetVBlankCallback(NULL);
        gMain.state++;
        break;
    case 1:
        DmaClearLarge16(3, (void *)(VRAM), VRAM_SIZE, 0x1000);
        DmaClear32(3, OAM, OAM_SIZE);
        DmaClear16(3, PLTT, PLTT_SIZE);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sOptionMenuBgTemplates, ARRAY_COUNT(sOptionMenuBgTemplates));
        ChangeBgX(0, 0, BG_COORD_SET);
        ChangeBgY(0, 0, BG_COORD_SET);
        ChangeBgX(1, 0, BG_COORD_SET);
        ChangeBgY(1, 0, BG_COORD_SET);
        ChangeBgX(2, 0, BG_COORD_SET);
        ChangeBgY(2, 0, BG_COORD_SET);
        ChangeBgX(3, 0, BG_COORD_SET);
        ChangeBgY(3, 0, BG_COORD_SET);
        InitWindows(sOptionMenuWinTemplates);
        DeactivateAllTextPrinters();
        SetGpuReg(REG_OFFSET_WIN0H, 0);
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG0);
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_CLR);
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_DARKEN);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 4);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
        ShowBg(0);
        ShowBg(1);
        gMain.state++;
        break;
    case 2:
        ResetPaletteFade();
        ScanlineEffect_Stop();
        ResetTasks();
        ResetSpriteData();
        gMain.state++;
        break;
    case 3:
        LoadBgTiles(1, GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->tiles, 0x120, 0x1A2);
        gMain.state++;
        break;
    case 4:
        LoadPalette(sOptionMenuBg_Pal, BG_PLTT_ID(0), sizeof(sOptionMenuBg_Pal));
        LoadPalette(GetWindowFrameTilesPal(gSaveBlock2Ptr->optionsWindowFrameType)->pal, BG_PLTT_ID(7), PLTT_SIZE_4BPP);
        gMain.state++;
        break;
    case 5:
        LoadPalette(sOptionMenuText_Pal, BG_PLTT_ID(1), sizeof(sOptionMenuText_Pal));
        gMain.state++;
        break;
    case 6:
        PutWindowTilemap(WIN_HEADER);
        DrawHeaderText();
        gMain.state++;
        break;
    case 7:
        gMain.state++;
        break;
    case 8:
        PutWindowTilemap(WIN_OPTIONS);
        DrawOptionMenuTexts();
        gMain.state++;
    case 9:
        DrawBgWindowFrames();
        gMain.state++;
        break;
    case 10:
    {
        switch(sCurrPage)
        {
        case 0:
            taskId = CreateTask(Task_OptionMenuFadeIn, 0);
            DrawOptionsPg1(taskId);
            break;
        case 1:
            taskId = CreateTask(Task_OptionMenuFadeIn_Pg2, 0);
            DrawOptionsPg2(taskId);
            break;
        case 2:
            taskId = CreateTask(Task_OptionMenuFadeIn_Pg3, 0);
            DrawOptionsPg3(taskId);
            break;  
        case 3:
            taskId = CreateTask(Task_OptionMenuFadeIn_Pg4, 0);
            DrawOptionsPg4(taskId);
            break;              
        }
    }
    case 11:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB);
        SetMainCallback2(MainCB2);
        return;
    }
}

static u8 Process_ChangePage(u8 CurrentPage)
{
    if (JOY_NEW(R_BUTTON))
    {
        if (CurrentPage < PAGE_COUNT - 1)
            CurrentPage++;
        else
            CurrentPage = 0;
    }
    if (JOY_NEW(L_BUTTON))
    {
        if (CurrentPage != 0)
            CurrentPage--;
        else
            CurrentPage = PAGE_COUNT - 1;
    }
    return CurrentPage;
}

static void Task_ChangePage(u8 taskId)
{
    DrawHeaderText();
    PutWindowTilemap(1);
    DrawOptionMenuTexts();
    switch(sCurrPage)
    {
    case 0:
        DrawOptionsPg1(taskId);
        gTasks[taskId].func = Task_OptionMenuFadeIn;
        break;
    case 1:
        DrawOptionsPg2(taskId);
        gTasks[taskId].func = Task_OptionMenuFadeIn_Pg2;
        break;
    case 2:
        DrawOptionsPg3(taskId);
        gTasks[taskId].func = Task_OptionMenuFadeIn_Pg3;
        break;
    case 3:
        DrawOptionsPg4(taskId);
        gTasks[taskId].func = Task_OptionMenuFadeIn_Pg4;
        break;
    }
}


static void Task_OptionMenuFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        gTasks[taskId].func = Task_OptionMenuProcessInput;
    }
}

static void Task_OptionMenuProcessInput(u8 taskId)
{
    if (JOY_NEW(L_BUTTON) || JOY_NEW(R_BUTTON))
    {
        FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(1));
        ClearStdWindowAndFrame(WIN_OPTIONS, FALSE);
        sCurrPage = Process_ChangePage(sCurrPage);
        gTasks[taskId].func = Task_OptionMenuSaveNoFade;
    }
    else if (JOY_NEW(B_BUTTON) || JOY_NEW(A_BUTTON))
    {
        gTasks[taskId].func = Task_OptionMenuSave;
    }
    else if (JOY_NEW(DPAD_UP))
    {
        if (gTasks[taskId].tMenuSelection > 0)
            gTasks[taskId].tMenuSelection--;
        else
            gTasks[taskId].tMenuSelection = MENUITEM_FRAMETYPE;
        HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        if (gTasks[taskId].tMenuSelection < MENUITEM_FRAMETYPE)
            gTasks[taskId].tMenuSelection++;
        else
            gTasks[taskId].tMenuSelection = 0;
        HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
    }
    else
    {
        u8 previousOption;

        switch (gTasks[taskId].tMenuSelection)
        {
        case MENUITEM_TEXTSPEED:
            previousOption = gTasks[taskId].tTextSpeed;
            gTasks[taskId].tTextSpeed = TextSpeed_ProcessInput(gTasks[taskId].tTextSpeed);

            if (previousOption != gTasks[taskId].tTextSpeed)
                TextSpeed_DrawChoices(gTasks[taskId].tTextSpeed);
            break;
        case MENUITEM_FOLLOWER:
            previousOption = gTasks[taskId].tFollower;
            gTasks[taskId].tFollower = Follower_ProcessInput(gTasks[taskId].tFollower);

            if (previousOption != gTasks[taskId].tFollower)
                Follower_DrawChoices(gTasks[taskId].tFollower);
            break;
        case MENUITEM_BIG_FOLLOWER:
            previousOption = gTasks[taskId].tBigFollower;
            gTasks[taskId].tBigFollower = BigFollower_ProcessInput(gTasks[taskId].tBigFollower);

            if (previousOption != gTasks[taskId].tBigFollower)
                BigFollower_DrawChoices(gTasks[taskId].tBigFollower);
            break;
        case MENUITEM_BUTTONMODE:
            previousOption = gTasks[taskId].tButtonMode;
            gTasks[taskId].tButtonMode = ButtonMode_ProcessInput(gTasks[taskId].tButtonMode);

            if (previousOption != gTasks[taskId].tButtonMode)
                ButtonMode_DrawChoices(gTasks[taskId].tButtonMode);
            break;
        case MENUITEM_MATCH_CALLS:
            previousOption = gTasks[taskId].tMatchCalls;
            gTasks[taskId].tMatchCalls = MatchCalls_ProcessInput(gTasks[taskId].tMatchCalls);

            if (previousOption != gTasks[taskId].tMatchCalls)
                MatchCalls_DrawChoices(gTasks[taskId].tMatchCalls);
            break;
        case MENUITEM_FAST_HEAL:
            previousOption = gTasks[taskId].tFastHeal;
            gTasks[taskId].tFastHeal = FastHeal_ProcessInput(gTasks[taskId].tFastHeal);
            
            if (previousOption != gTasks[taskId].tFastHeal)
                FastHeal_DrawChoices(gTasks[taskId].tFastHeal);
            break;
        case MENUITEM_FRAMETYPE:
            previousOption = gTasks[taskId].tWindowFrameType;
            gTasks[taskId].tWindowFrameType = FrameType_ProcessInput(gTasks[taskId].tWindowFrameType);

            if (previousOption != gTasks[taskId].tWindowFrameType)
                FrameType_DrawChoices(gTasks[taskId].tWindowFrameType);
            break;
        default:
            return;
        }

        if (sArrowPressed)
        {
            sArrowPressed = FALSE;
            CopyWindowToVram(WIN_OPTIONS, COPYWIN_GFX);
        }
    }
}

static void Task_OptionMenuFadeIn_Pg2(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_OptionMenuProcessInput_Pg2;
}

static void Task_OptionMenuProcessInput_Pg2(u8 taskId)
{
    if (JOY_NEW(L_BUTTON) || JOY_NEW(R_BUTTON))
    {
        FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(1));
        ClearStdWindowAndFrame(WIN_OPTIONS, FALSE);
        sCurrPage = Process_ChangePage(sCurrPage);
        gTasks[taskId].func = Task_OptionMenuSaveNoFade;
    }
    else if (JOY_NEW(B_BUTTON) || JOY_NEW(A_BUTTON))
    {
        gTasks[taskId].func = Task_OptionMenuSave;
    }
    else if (JOY_NEW(DPAD_UP))
    {
        if (gTasks[taskId].tMenuSelection > 0)
            gTasks[taskId].tMenuSelection--;
        else
            gTasks[taskId].tMenuSelection = MENUITEM_SKIP_INTRO;
        HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        if (gTasks[taskId].tMenuSelection < MENUITEM_SKIP_INTRO)
            gTasks[taskId].tMenuSelection++;
        else
            gTasks[taskId].tMenuSelection = 0;
        HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
    }
    else
    {
        u8 previousOption;

        switch (gTasks[taskId].tMenuSelection)
        {

        case MENUITEM_AUTORUN:
            previousOption = gTasks[taskId].tAutoRun;
            gTasks[taskId].tAutoRun = Autorun_ProcessInput(gTasks[taskId].tAutoRun);

            if (previousOption != gTasks[taskId].tAutoRun)
                Autorun_DrawChoices(gTasks[taskId].tAutoRun);
            break;
        case MENUITEM_AUTORUN_SURF:
            previousOption = gTasks[taskId].tAutoRunSurf;
            gTasks[taskId].tAutoRunSurf = AutorunSurf_ProcessInput(gTasks[taskId].tAutoRunSurf);

            if (previousOption != gTasks[taskId].tAutoRunSurf)
                AutorunSurf_DrawChoices(gTasks[taskId].tAutoRunSurf);
            break;
        case MENUITEM_AUTORUN_DIVE:
            previousOption = gTasks[taskId].tAutoRunDive;
            gTasks[taskId].tAutoRunDive = AutorunDive_ProcessInput(gTasks[taskId].tAutoRunDive);

            if (previousOption != gTasks[taskId].tAutoRunDive)
                AutorunDive_DrawChoices(gTasks[taskId].tAutoRunDive);
            break;
        case MENUITEM_UNIT_SYSTEM:
            previousOption = gTasks[taskId].tUnitSystem;
            gTasks[taskId].tUnitSystem = UnitSystem_ProcessInput(gTasks[taskId].tUnitSystem);
            
            if (previousOption != gTasks[taskId].tUnitSystem)
                UnitSystem_DrawChoices(gTasks[taskId].tUnitSystem);
            break;
        case MENUITEM_SKIP_INTRO:
            previousOption = gTasks[taskId].tSkipIntro;
            gTasks[taskId].tSkipIntro = SkipIntro_ProcessInput(gTasks[taskId].tSkipIntro);
            
            if (previousOption != gTasks[taskId].tSkipIntro)
                SkipIntro_DrawChoices(gTasks[taskId].tSkipIntro);
            break;
        default:
            return;
        }

        if (sArrowPressed)
        {
            sArrowPressed = FALSE;
            CopyWindowToVram(WIN_OPTIONS, COPYWIN_GFX);
        }
    }
}

static void Task_OptionMenuFadeIn_Pg3(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_OptionMenuProcessInput_Pg3;
}

static void Task_OptionMenuProcessInput_Pg3(u8 taskId)
{
    if (JOY_NEW(L_BUTTON) || JOY_NEW(R_BUTTON))
    {
        FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(1));
        ClearStdWindowAndFrame(WIN_OPTIONS, FALSE);
        sCurrPage = Process_ChangePage(sCurrPage);
        gTasks[taskId].func = Task_OptionMenuSaveNoFade;
    }
    else if (JOY_NEW(B_BUTTON) || JOY_NEW(A_BUTTON))
    {
        gTasks[taskId].func = Task_OptionMenuSave;
    }
    else if (JOY_NEW(DPAD_UP))
    {
        if (gTasks[taskId].tMenuSelection > 0)
            gTasks[taskId].tMenuSelection--;
        else
            gTasks[taskId].tMenuSelection = MENUITEM_FAST_RUN_PROMPT;
        HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        if (gTasks[taskId].tMenuSelection < MENUITEM_FAST_RUN_PROMPT)
            gTasks[taskId].tMenuSelection++;
        else
            gTasks[taskId].tMenuSelection = 0;
        HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
    }
    else
    {
        u8 previousOption;

        switch (gTasks[taskId].tMenuSelection)
        {
        case MENUITEM_BATTLESCENE:
            previousOption = gTasks[taskId].tBattleScene;
            gTasks[taskId].tBattleScene = BattleScene_ProcessInput(gTasks[taskId].tBattleScene);

            if (previousOption != gTasks[taskId].tBattleScene)
                BattleScene_DrawChoices(gTasks[taskId].tBattleScene);
            break;
        case MENUITEM_BATTLESTYLE:
            previousOption = gTasks[taskId].tBattleStyle;
            gTasks[taskId].tBattleStyle = BattleStyle_ProcessInput(gTasks[taskId].tBattleStyle);

            if (previousOption != gTasks[taskId].tBattleStyle)
                BattleStyle_DrawChoices(gTasks[taskId].tBattleStyle);
            break;
        case MENUITEM_DIFFICULTY:
            previousOption = gTasks[taskId].tDifficulty;
            gTasks[taskId].tDifficulty = Difficulty_ProcessInput(gTasks[taskId].tDifficulty);

            if (previousOption != gTasks[taskId].tDifficulty)
                Difficulty_DrawChoices(gTasks[taskId].tDifficulty);
            break;

        case MENUITEM_FAST_BATTLES:
            previousOption = gTasks[taskId].tFastBattles;
            gTasks[taskId].tFastBattles = FastBattles_ProcessInput(gTasks[taskId].tFastBattles);
            
            if (previousOption != gTasks[taskId].tFastBattles)
                FastBattles_DrawChoices(gTasks[taskId].tFastBattles);
            break;
        case MENUITEM_BALL_PROMPT:
            previousOption = gTasks[taskId].tBallPrompt;
            gTasks[taskId].tBallPrompt = BallPrompt_ProcessInput(gTasks[taskId].tBallPrompt);
            
            if (previousOption != gTasks[taskId].tBallPrompt)
                BallPrompt_DrawChoices(gTasks[taskId].tBallPrompt);
            break;
        case MENUITEM_FAST_RUN:
            previousOption = gTasks[taskId].tFastRun;
            gTasks[taskId].tFastRun = FastRun_ProcessInput(gTasks[taskId].tFastRun);
            
            if (previousOption != gTasks[taskId].tFastRun)
                FastRun_DrawChoices(gTasks[taskId].tFastRun);
            break;
        case MENUITEM_FAST_RUN_PROMPT:
            previousOption = gTasks[taskId].tFastRunPrompt;
            gTasks[taskId].tFastRunPrompt = FastRunPrompt_ProcessInput(gTasks[taskId].tFastRunPrompt);
            
            if (previousOption != gTasks[taskId].tFastRunPrompt)
                FastRunPrompt_DrawChoices(gTasks[taskId].tFastRunPrompt);
            break;
        default:
            return;
        }

        if (sArrowPressed)
        {
            sArrowPressed = FALSE;
            CopyWindowToVram(WIN_OPTIONS, COPYWIN_GFX);
        }
    }
}

static void Task_OptionMenuFadeIn_Pg4(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_OptionMenuProcessInput_Pg4;
}

static void Task_OptionMenuProcessInput_Pg4(u8 taskId)
{
    if (JOY_NEW(L_BUTTON) || JOY_NEW(R_BUTTON))
    {
        FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(1));
        ClearStdWindowAndFrame(WIN_OPTIONS, FALSE);
        sCurrPage = Process_ChangePage(sCurrPage);
        gTasks[taskId].func = Task_OptionMenuSaveNoFade;
    }
    else if (JOY_NEW(B_BUTTON) || JOY_NEW(A_BUTTON))
    {
        gTasks[taskId].func = Task_OptionMenuSave;
    }
    else if (JOY_NEW(DPAD_UP))
    {
        if (gTasks[taskId].tMenuSelection > 0)
            gTasks[taskId].tMenuSelection--;
        else
            gTasks[taskId].tMenuSelection = MENUITEM_SOUND_EFFECTS;
        HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        if (gTasks[taskId].tMenuSelection < MENUITEM_SOUND_EFFECTS)
            gTasks[taskId].tMenuSelection++;
        else
            gTasks[taskId].tMenuSelection = 0;
        HighlightOptionMenuItem(gTasks[taskId].tMenuSelection);
    }
    else
    {
        u8 previousOption;

        switch (gTasks[taskId].tMenuSelection)
        {
        case MENUITEM_SOUND:
            previousOption = gTasks[taskId].tSound;
            gTasks[taskId].tSound = Sound_ProcessInput(gTasks[taskId].tSound);
            
            if (previousOption != gTasks[taskId].tSound)
                Sound_DrawChoices(gTasks[taskId].tSound);
            break;
        case MENUITEM_MUSIC:
            previousOption = gTasks[taskId].tMusic;
            gTasks[taskId].tMusic = Music_ProcessInput(gTasks[taskId].tMusic);
            
            if (previousOption != gTasks[taskId].tMusic)
                Music_DrawChoices(gTasks[taskId].tMusic);
            break;
        case MENUITEM_BIKE_SURF_MUSIC:
            previousOption = gTasks[taskId].tBikeSurfMusic;
            gTasks[taskId].tBikeSurfMusic = BikeSurfMusic_ProcessInput(gTasks[taskId].tBikeSurfMusic);
            
            if (previousOption != gTasks[taskId].tBikeSurfMusic)
                BikeSurfMusic_DrawChoices(gTasks[taskId].tBikeSurfMusic);
            break;
        case MENUITEM_WILD_MUSIC:
            previousOption = gTasks[taskId].tWildMusic;
            gTasks[taskId].tWildMusic = WildMusic_ProcessInput(gTasks[taskId].tWildMusic);
            
            if (previousOption != gTasks[taskId].tWildMusic)
                WildMusic_DrawChoices(gTasks[taskId].tWildMusic, YPOS_WILD_MUSIC);
            break;
        case MENUITEM_TRAINER_MUSIC:
            previousOption = gTasks[taskId].tTrainerMusic;
            gTasks[taskId].tTrainerMusic = TrainerMusic_ProcessInput(gTasks[taskId].tTrainerMusic);
            
            if (previousOption != gTasks[taskId].tTrainerMusic)
            TrainerMusic_DrawChoices(gTasks[taskId].tTrainerMusic);
            break;
        case MENUITEM_FRONTIER_MUSIC:
            previousOption = gTasks[taskId].tFrontierMusic;
            gTasks[taskId].tFrontierMusic = FrontierMusic_ProcessInput(gTasks[taskId].tFrontierMusic);
            
            if (previousOption != gTasks[taskId].tFrontierMusic)
            FrontierMusic_DrawChoices(gTasks[taskId].tFrontierMusic);
            break;
        case MENUITEM_SOUND_EFFECTS:
            previousOption = gTasks[taskId].tSoundEffects;
            gTasks[taskId].tSoundEffects = SoundEffects_ProcessInput(gTasks[taskId].tSoundEffects);
            
            if (previousOption != gTasks[taskId].tSoundEffects)
                SoundEffects_DrawChoices(gTasks[taskId].tSoundEffects);
            break;
        default:
            return;
        }

        if (sArrowPressed)
        {
            sArrowPressed = FALSE;
            CopyWindowToVram(WIN_OPTIONS, COPYWIN_GFX);
        }
    }
}



static void Task_OptionMenuSave(u8 taskId)
{
    //pg1
    gSaveBlock2Ptr->optionsTextSpeed = gTasks[taskId].tTextSpeed;
    gSaveBlock2Ptr->optionsFollower = gTasks[taskId].tFollower;
    gSaveBlock2Ptr->optionsBigFollower = gTasks[taskId].tBigFollower; 
    gSaveBlock2Ptr->optionsButtonMode = gTasks[taskId].tButtonMode;
    gSaveBlock2Ptr->optionsFastHeal = gTasks[taskId].tFastHeal;
    gSaveBlock2Ptr->optionsWindowFrameType = gTasks[taskId].tWindowFrameType;
    gSaveBlock2Ptr->optionsMatchCalls = gTasks[taskId].tMatchCalls;   
    /*gTasks[taskId].tFollower == 0 ? FlagClear(FLAG_POKEMON_FOLLOWERS) : FlagSet(FLAG_POKEMON_FOLLOWERS);
        VarSet(VAR_DIFFICULTY, gTasks[taskId].tDifficulty);*/
    //pg2
    gSaveBlock2Ptr->optionsAutorun = gTasks[taskId].tAutoRun;
    gSaveBlock2Ptr->optionsAutorunSurf = gTasks[taskId].tAutoRunSurf;
    gSaveBlock2Ptr->optionsAutorunDive = gTasks[taskId].tAutoRunDive;
    gSaveBlock2Ptr->optionsUnitSystem = gTasks[taskId].tUnitSystem;
    gSaveBlock2Ptr->optionsSkipIntro = gTasks[taskId].tSkipIntro;
    //pg3
    gSaveBlock2Ptr->optionsFastBattles = gTasks[taskId].tFastBattles;
    gSaveBlock2Ptr->optionsBallPrompt = gTasks[taskId].tBallPrompt;
    gSaveBlock2Ptr->optionsFastRun = gTasks[taskId].tFastRun;
    gSaveBlock2Ptr->optionsFastRunPrompt = gTasks[taskId].tFastRunPrompt;
    gSaveBlock2Ptr->optionsBattleScene = gTasks[taskId].tBattleScene;
    gSaveBlock2Ptr->optionsBattleStyle = gTasks[taskId].tBattleStyle;
    //pg4
    gSaveBlock2Ptr->optionsMusic = gTasks[taskId].tMusic;
    gSaveBlock2Ptr->optionsSound = gTasks[taskId].tSound;
    gSaveBlock2Ptr->optionsBikeSurfMusic = gTasks[taskId].tBikeSurfMusic;
    gSaveBlock2Ptr->optionsWildMusic = gTasks[taskId].tWildMusic;
    gSaveBlock2Ptr->optionsFrontierMusic = gTasks[taskId].tFrontierMusic;
    gSaveBlock2Ptr->optionsTrainerMusic = gTasks[taskId].tTrainerMusic;
    gSaveBlock2Ptr->optionsSoundEffects = gTasks[taskId].tSoundEffects;

    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    gTasks[taskId].func = Task_OptionMenuFadeOut;
}

static void Task_OptionMenuSaveNoFade(u8 taskId)
{
    //pg1
    gSaveBlock2Ptr->optionsTextSpeed = gTasks[taskId].tTextSpeed;
    gSaveBlock2Ptr->optionsFollower = gTasks[taskId].tFollower;
    gSaveBlock2Ptr->optionsBigFollower = gTasks[taskId].tBigFollower; 
    gSaveBlock2Ptr->optionsButtonMode = gTasks[taskId].tButtonMode;
    gSaveBlock2Ptr->optionsFastHeal = gTasks[taskId].tFastHeal;
    gSaveBlock2Ptr->optionsWindowFrameType = gTasks[taskId].tWindowFrameType;
    gSaveBlock2Ptr->optionsMatchCalls = gTasks[taskId].tMatchCalls;   
    /*gTasks[taskId].tFollower == 0 ? FlagClear(FLAG_POKEMON_FOLLOWERS) : FlagSet(FLAG_POKEMON_FOLLOWERS);
        VarSet(VAR_DIFFICULTY, gTasks[taskId].tDifficulty);*/
    //pg2
    gSaveBlock2Ptr->optionsAutorun = gTasks[taskId].tAutoRun;
    gSaveBlock2Ptr->optionsAutorunSurf = gTasks[taskId].tAutoRunSurf;
    gSaveBlock2Ptr->optionsAutorunDive = gTasks[taskId].tAutoRunDive;
    gSaveBlock2Ptr->optionsUnitSystem = gTasks[taskId].tUnitSystem;
    gSaveBlock2Ptr->optionsSkipIntro = gTasks[taskId].tSkipIntro;
    //pg3
    gSaveBlock2Ptr->optionsFastBattles = gTasks[taskId].tFastBattles;
    gSaveBlock2Ptr->optionsBallPrompt = gTasks[taskId].tBallPrompt;
    gSaveBlock2Ptr->optionsFastRun = gTasks[taskId].tFastRun;
    gSaveBlock2Ptr->optionsFastRunPrompt = gTasks[taskId].tFastRunPrompt;
    gSaveBlock2Ptr->optionsBattleScene = gTasks[taskId].tBattleScene;
    gSaveBlock2Ptr->optionsBattleStyle = gTasks[taskId].tBattleStyle;
    //pg4
    gSaveBlock2Ptr->optionsMusic = gTasks[taskId].tMusic;
    gSaveBlock2Ptr->optionsSound = gTasks[taskId].tSound;
    gSaveBlock2Ptr->optionsBikeSurfMusic = gTasks[taskId].tBikeSurfMusic;
    gSaveBlock2Ptr->optionsWildMusic = gTasks[taskId].tWildMusic;
    gSaveBlock2Ptr->optionsFrontierMusic = gTasks[taskId].tFrontierMusic;
    gSaveBlock2Ptr->optionsTrainerMusic = gTasks[taskId].tTrainerMusic;
    gSaveBlock2Ptr->optionsSoundEffects = gTasks[taskId].tSoundEffects;
    gTasks[taskId].func = Task_ChangePage;
}

static void Task_OptionMenuFadeOut(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        DestroyTask(taskId);
        FreeAllWindowBuffers();
        SetMainCallback2(gMain.savedCallback);
    }
}

static void HighlightOptionMenuItem(u8 index)
{
    SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(16, DISPLAY_WIDTH - 16));
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(index * 16 + 40, index * 16 + 56));
}

static void DrawOptionMenuChoice(const u8 *text, u8 x, u8 y, u8 style)
{
    u8 dst[16];
    u16 i;

    for (i = 0; *text != EOS && i < ARRAY_COUNT(dst) - 1; i++)
        dst[i] = *(text++);

    if (style != 0)
    {
        dst[2] = TEXT_COLOR_RED;
        dst[5] = TEXT_COLOR_LIGHT_RED;
    }

    dst[i] = EOS;
    AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, dst, x, y + 1, TEXT_SKIP_DRAW, NULL);
}

static int GetMiddleX(const u8 *txt1, const u8 *txt2, const u8 *txt3)
{
    int xMid;
    int widthLeft = GetStringWidth(1, txt1, 0);
    int widthMid = GetStringWidth(1, txt2, 0);
    int widthRight = GetStringWidth(1, txt3, 0);

    widthMid -= (198 - 104);
    xMid = (widthLeft - widthMid - widthRight) / 2 + 104;
    return xMid;
}

static u8 Follower_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void Follower_DrawChoices(u8 selection)
{
    u8 styles[2];
    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;
    DrawOptionMenuChoice(gText_FollowerOff, 104, YPOS_FOLLOWER, styles[0]);
    DrawOptionMenuChoice(gText_FollowerOn, GetStringRightAlignXOffset(FONT_NORMAL, gText_FollowerOn, 198), YPOS_FOLLOWER, styles[1]);
}


static u8 BigFollower_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void BigFollower_DrawChoices(u8 selection)
{
    u8 styles[2];
    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;
    DrawOptionMenuChoice(gText_FollowerOff, 104, YPOS_BIG_FOLLOWER, styles[0]);
    DrawOptionMenuChoice(gText_FollowerOn, GetStringRightAlignXOffset(FONT_NORMAL, gText_FollowerOn, 198), YPOS_BIG_FOLLOWER, styles[1]);
}

static u8 Autorun_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void Autorun_DrawChoices(u8 selection)
{
    u8 styles[2];
    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;
    DrawOptionMenuChoice(gText_BattleSceneOff, 104, YPOS_AUTORUN, styles[0]);
    DrawOptionMenuChoice(gText_BattleSceneOn, GetStringRightAlignXOffset(FONT_NORMAL, gText_BattleSceneOn, 198), YPOS_AUTORUN, styles[1]);
}


static void AutorunSurf_DrawChoices(u8 selection)
{
    u8 styles[2];
    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;
    DrawOptionMenuChoice(gText_BattleSceneOff, 104, YPOS_AUTORUN_SURF, styles[0]);
    DrawOptionMenuChoice(gText_BattleSceneOn, GetStringRightAlignXOffset(FONT_NORMAL, gText_BattleSceneOn, 198), YPOS_AUTORUN_SURF, styles[1]);
}

static u8 AutorunSurf_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void AutorunDive_DrawChoices(u8 selection)
{
    u8 styles[2];
    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;
    DrawOptionMenuChoice(gText_BattleSceneOff, 104, YPOS_AUTORUN_DIVE, styles[0]);
    DrawOptionMenuChoice(gText_BattleSceneOn, GetStringRightAlignXOffset(FONT_NORMAL, gText_BattleSceneOn, 198), YPOS_AUTORUN_DIVE, styles[1]);
}

static u8 AutorunDive_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void MatchCalls_DrawChoices(u8 selection)
{
    u8 styles[2];
    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;
    DrawOptionMenuChoice(gText_BattleSceneOff, 104, YPOS_MATCH_CALLS, styles[0]);
    DrawOptionMenuChoice(gText_BattleSceneOn, GetStringRightAlignXOffset(FONT_NORMAL, gText_BattleSceneOn, 198), YPOS_MATCH_CALLS, styles[1]);
}

static u8 MatchCalls_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static u8 UnitSystem_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void UnitSystem_DrawChoices(u8 selection)
{
    u8 styles[2];
    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;
    DrawOptionMenuChoice(gText_UnitSystemMetric, 104, YPOS_UNIT_SYSTEM, styles[0]);
    DrawOptionMenuChoice(gText_UnitSystemImperial, GetStringRightAlignXOffset(FONT_NORMAL, gText_UnitSystemImperial, 200), YPOS_UNIT_SYSTEM, styles[1]);
}

static void SkipIntro_DrawChoices(u8 selection)
{
    u8 styles[2];
    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;
    DrawOptionMenuChoice(gText_BattleSceneOff, 104, YPOS_SKIP_INTRO, styles[0]);
    DrawOptionMenuChoice(gText_BattleSceneOn, GetStringRightAlignXOffset(FONT_NORMAL, gText_BattleSceneOn, 198), YPOS_SKIP_INTRO, styles[1]);
}

static u8 SkipIntro_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void FastBattles_DrawChoices(u8 selection)
{
    u8 styles[2];
    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;
    DrawOptionMenuChoice(gText_BattleSceneOff, 104, YPOS_FAST_BATTLES, styles[0]);
    DrawOptionMenuChoice(gText_BattleSceneOn, GetStringRightAlignXOffset(FONT_NORMAL, gText_BattleSceneOn, 198), YPOS_FAST_BATTLES, styles[1]);
}

static u8 FastBattles_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void FastRun_DrawChoices(u8 selection)
{
    u8 styles[3] = {0};
    int xMid = GetMiddleX(gText_FastRunLR, gText_FastRunBA, gText_FastRunB);
    styles[selection] = 1;

    /*if (selection == 0)
    {
        gSaveBlock2Ptr->optionsSoundEffects = 0; //Gen 3
    }
    else if (selection == 1)
    {
        gSaveBlock2Ptr->optionsSoundEffects = 1; //DPL
    }
    else
    {
        gSaveBlock2Ptr->optionsSoundEffects = 2; //HGSS
    }*/

    DrawOptionMenuChoice(gText_FastRunLR, 104, YPOS_FAST_RUN, styles[0]);
    DrawOptionMenuChoice(gText_FastRunBA, xMid + 7, YPOS_FAST_RUN, styles[1]);
    DrawOptionMenuChoice(gText_FastRunB, GetStringRightAlignXOffset(1, gText_FastRunBA, 198), YPOS_FAST_RUN, styles[2]);
}

static u8 FastRun_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (selection <= 1)
            selection++;
        else
            selection = 0;

        sArrowPressed = TRUE;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (selection != 0)
            selection--;
        else
            selection = 2;

        sArrowPressed = TRUE;
    }
    return selection;
}

static void FastRunPrompt_DrawChoices(u8 selection)
{
    u8 styles[2];
    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;
    DrawOptionMenuChoice(gText_BattleSceneOff, 104, YPOS_FAST_RUN_PROMPT, styles[0]);
    DrawOptionMenuChoice(gText_BattleSceneOn, GetStringRightAlignXOffset(FONT_NORMAL, gText_BattleSceneOn, 198), YPOS_FAST_RUN_PROMPT, styles[1]);
}

static u8 FastRunPrompt_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void BallPrompt_DrawChoices(u8 selection)
{
    u8 styles[2];
    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;
    DrawOptionMenuChoice(gText_BattleSceneOff, 104, YPOS_BALL_PROMPT, styles[0]);
    DrawOptionMenuChoice(gText_BattleSceneOn, GetStringRightAlignXOffset(FONT_NORMAL, gText_BattleSceneOn, 198), YPOS_BALL_PROMPT, styles[1]);
}

static u8 BallPrompt_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void FastHeal_DrawChoices(u8 selection)
{
    u8 styles[2];
    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;
    DrawOptionMenuChoice(gText_BattleSceneOff, 104, YPOS_FAST_HEAL, styles[0]);
    DrawOptionMenuChoice(gText_BattleSceneOn, GetStringRightAlignXOffset(FONT_NORMAL, gText_BattleSceneOn, 198), YPOS_FAST_HEAL, styles[1]);
}

static u8 FastHeal_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void Music_DrawChoices(u8 selection)
{
    u8 styles[2];
    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;
    DrawOptionMenuChoice(gText_BattleSceneOff, 104, YPOS_MUSIC, styles[0]);
    DrawOptionMenuChoice(gText_BattleSceneOn, GetStringRightAlignXOffset(FONT_NORMAL, gText_BattleSceneOn, 198), YPOS_MUSIC, styles[1]);
}

static u8 Music_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void BikeSurfMusic_DrawChoices(u8 selection)
{
    u8 styles[2];
    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;
    DrawOptionMenuChoice(gText_BattleSceneOff, 104, YPOS_BIKE_SURF_MUSIC, styles[0]);
    DrawOptionMenuChoice(gText_BattleSceneOn, GetStringRightAlignXOffset(FONT_NORMAL, gText_BattleSceneOn, 198), YPOS_BIKE_SURF_MUSIC, styles[1]);
}

static u8 BikeSurfMusic_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static u8 WildMusic_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (selection == 0)
        {
            selection = 1;
            sArrowPressed = TRUE;
        }
        else if (selection == 1)
        {
            selection = 2;
            sArrowPressed = TRUE;
        }
        else if (selection == 2)
        {
            selection = 3;
            sArrowPressed = TRUE;
        }
        else if (selection == 3)
        {
            selection = 4;
            sArrowPressed = TRUE;
        }
        else if (selection == 4)
        {
            selection = 5;
            sArrowPressed = TRUE;
        }
        else if (selection == 5)
        {
            selection = 0;
            sArrowPressed = TRUE;
        }
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (selection == 0)
        {
            selection = 5;
            sArrowPressed = TRUE;
        }
        else if (selection == 1)
        {
            selection = 0;
            sArrowPressed = TRUE;
        }
        else if (selection == 2)
        {
            selection = 1;
            sArrowPressed = TRUE;
        }
        else if (selection == 3)
        {
            selection = 2;
            sArrowPressed = TRUE;
        }
        else if (selection == 4)
        {
            selection = 3;
            sArrowPressed = TRUE;
        }
        else if (selection == 5)
        {
            selection = 4;
            sArrowPressed = TRUE;
        }
    }
    return selection;
}

static const u8 *const sText_Sound_BattleMusic_Strings[] = 
{
    gText_MusicHoenn,  
    gText_MusicKanto1,  
    gText_MusicSinnoh,  
    gText_MusicJohto,   
    gText_MusicKanto2,   
    gText_MusicRandom,
};

static void WildMusic_DrawChoices_Six(const u8 *const *const strings, int selection, int y)
{
    static const u8 choiceOrders[][2] =
    {
        {0, 1},
        {1, 2},
        {2, 3},
        {3, 4},
        {4, 5},
        {5, 0},
    };
    u8 styles[6] = {0};
    const u8 *order = choiceOrders[selection];
    styles[selection] = 1;

    DrawOptionMenuChoice(strings[order[0]], 104, YPOS_WILD_MUSIC, styles[order[0]]);
    DrawOptionMenuChoice(strings[order[1]], GetStringRightAlignXOffset(FONT_NORMAL, strings[order[1]], 198), YPOS_WILD_MUSIC, styles[order[1]]);
}

static void WildMusic_DrawChoices(int selection, int y)
{
    WildMusic_DrawChoices_Six(sText_Sound_BattleMusic_Strings, selection, YPOS_WILD_MUSIC);
    if (selection == 0)
    {
        gSaveBlock2Ptr->optionsWildMusic = 0; // Hoenn
    }
    else if (selection == 1)
    {
        gSaveBlock2Ptr->optionsWildMusic = 1; // Kanto1
    }
    else if (selection == 2)
    {
        gSaveBlock2Ptr->optionsWildMusic = 2; // Sinnoh
    }
    else if (selection == 3)
    {
        gSaveBlock2Ptr->optionsWildMusic = 3; // Johto
    }
    else if (selection == 4)
    {
        gSaveBlock2Ptr->optionsWildMusic = 4; // Kanto 2
    }
    else //(selection == 5)
    {
        gSaveBlock2Ptr->optionsWildMusic = 5; // Random
    }
}

static void TrainerMusic_DrawChoices_Six(const u8 *const *const strings, int selection, int y)
{
    static const u8 choiceOrders[][2] =
    {
        {0, 1},
        {1, 2},
        {2, 3},
        {3, 4},
        {4, 5},
        {5, 0},
    };
    u8 styles[6] = {0};
    const u8 *order = choiceOrders[selection];
    styles[selection] = 1;

    DrawOptionMenuChoice(strings[order[0]], 104, YPOS_TRAINER_MUSIC, styles[order[0]]);
    DrawOptionMenuChoice(strings[order[1]], GetStringRightAlignXOffset(FONT_NORMAL, strings[order[1]], 198), YPOS_TRAINER_MUSIC, styles[order[1]]);
}

static void TrainerMusic_DrawChoices(u8 selection)
{
    TrainerMusic_DrawChoices_Six(sText_Sound_BattleMusic_Strings, selection, YPOS_TRAINER_MUSIC);
    /*if (selection == 0)
    {
        gSaveBlock2Ptr->optionsWildMusic = 0; // Hoenn
    }
    else if (selection == 1)
    {
        gSaveBlock2Ptr->optionsWildMusic = 1; // Kanto1
    }
    else if (selection == 2)
    {
        gSaveBlock2Ptr->optionsWildMusic = 2; // Sinnoh
    }
    else if (selection == 3)
    {
        gSaveBlock2Ptr->optionsWildMusic = 3; // Johto
    }
    else if (selection == 4)
    {
        gSaveBlock2Ptr->optionsWildMusic = 4; // Kanto 2
    }
    else //(selection == 5)
    {
        gSaveBlock2Ptr->optionsWildMusic = 5; // Random
    }*/
}

static u8 TrainerMusic_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (selection == 0)
        {
            selection = 1;
            sArrowPressed = TRUE;
        }
        else if (selection == 1)
        {
            selection = 2;
            sArrowPressed = TRUE;
        }
        else if (selection == 2)
        {
            selection = 3;
            sArrowPressed = TRUE;
        }
        else if (selection == 3)
        {
            selection = 4;
            sArrowPressed = TRUE;
        }
        else if (selection == 4)
        {
            selection = 5;
            sArrowPressed = TRUE;
        }
        else if (selection == 5)
        {
            selection = 0;
            sArrowPressed = TRUE;
        }
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (selection == 0)
        {
            selection = 5;
            sArrowPressed = TRUE;
        }
        else if (selection == 1)
        {
            selection = 0;
            sArrowPressed = TRUE;
        }
        else if (selection == 2)
        {
            selection = 1;
            sArrowPressed = TRUE;
        }
        else if (selection == 3)
        {
            selection = 2;
            sArrowPressed = TRUE;
        }
        else if (selection == 4)
        {
            selection = 3;
            sArrowPressed = TRUE;
        }
        else if (selection == 5)
        {
            selection = 4;
            sArrowPressed = TRUE;
        }
    }
    return selection;
}

static void FrontierMusic_DrawChoices_Six(const u8 *const *const strings, int selection, int y)
{
    static const u8 choiceOrders[][2] =
    {
        {0, 1},
        {1, 2},
        {2, 3},
        {3, 4},
        {4, 5},
        {5, 0},
    };
    u8 styles[6] = {0};
    const u8 *order = choiceOrders[selection];
    styles[selection] = 1;

    DrawOptionMenuChoice(strings[order[0]], 104, YPOS_FRONTIER_MUSIC, styles[order[0]]);
    DrawOptionMenuChoice(strings[order[1]], GetStringRightAlignXOffset(FONT_NORMAL, strings[order[1]], 198), YPOS_FRONTIER_MUSIC, styles[order[1]]);
}

static void FrontierMusic_DrawChoices(u8 selection)
{
    FrontierMusic_DrawChoices_Six(sText_Sound_BattleMusic_Strings, selection, YPOS_FRONTIER_MUSIC);
    /*if (selection == 0)
    {
        gSaveBlock2Ptr->optionsWildMusic = 0; // Hoenn
    }
    else if (selection == 1)
    {
        gSaveBlock2Ptr->optionsWildMusic = 1; // Kanto1
    }
    else if (selection == 2)
    {
        gSaveBlock2Ptr->optionsWildMusic = 2; // Sinnoh
    }
    else if (selection == 3)
    {
        gSaveBlock2Ptr->optionsWildMusic = 3; // Johto
    }
    else if (selection == 4)
    {
        gSaveBlock2Ptr->optionsWildMusic = 4; // Kanto 2
    }
    else //(selection == 5)
    {
        gSaveBlock2Ptr->optionsWildMusic = 5; // Random
    }*/
}

static u8 FrontierMusic_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (selection == 0)
        {
            selection = 1;
            sArrowPressed = TRUE;
        }
        else if (selection == 1)
        {
            selection = 2;
            sArrowPressed = TRUE;
        }
        else if (selection == 2)
        {
            selection = 3;
            sArrowPressed = TRUE;
        }
        else if (selection == 3)
        {
            selection = 4;
            sArrowPressed = TRUE;
        }
        else if (selection == 4)
        {
            selection = 5;
            sArrowPressed = TRUE;
        }
        else if (selection == 5)
        {
            selection = 0;
            sArrowPressed = TRUE;
        }
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (selection == 0)
        {
            selection = 5;
            sArrowPressed = TRUE;
        }
        else if (selection == 1)
        {
            selection = 0;
            sArrowPressed = TRUE;
        }
        else if (selection == 2)
        {
            selection = 1;
            sArrowPressed = TRUE;
        }
        else if (selection == 3)
        {
            selection = 2;
            sArrowPressed = TRUE;
        }
        else if (selection == 4)
        {
            selection = 3;
            sArrowPressed = TRUE;
        }
        else if (selection == 5)
        {
            selection = 4;
            sArrowPressed = TRUE;
        }
    }
    return selection;
}

static void SoundEffects_DrawChoices(u8 selection)
{
    u8 styles[3] = {0};
    int xMid = GetMiddleX(gText_SoundEffects_Gen3, gText_SoundEffects_DPPl, gText_SoundEffects_HGSS);
    styles[selection] = 1;

    /*if (selection == 0)
    {
        gSaveBlock2Ptr->optionsSoundEffects = 0; //Gen 3
    }
    else if (selection == 1)
    {
        gSaveBlock2Ptr->optionsSoundEffects = 1; //DPL
    }
    else
    {
        gSaveBlock2Ptr->optionsSoundEffects = 2; //HGSS
    }*/

    DrawOptionMenuChoice(gText_SoundEffects_Gen3, 104, YPOS_SOUND_EFFECTS, styles[0]);
    DrawOptionMenuChoice(gText_SoundEffects_DPPl, xMid, YPOS_SOUND_EFFECTS, styles[1]);
    DrawOptionMenuChoice(gText_SoundEffects_HGSS, GetStringRightAlignXOffset(1, gText_SoundEffects_DPPl, 198), YPOS_SOUND_EFFECTS, styles[2]);
}

static u8 SoundEffects_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (selection <= 1)
            selection++;
        else
            selection = 0;

        sArrowPressed = TRUE;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (selection != 0)
            selection--;
        else
            selection = 2;

        sArrowPressed = TRUE;
    }
    return selection;
}

static u8 Difficulty_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (selection <= 1)
            selection++;
        else
            selection = 0;

        sArrowPressed = TRUE;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (selection != 0)
            selection--;
        else
            selection = 2;

        sArrowPressed = TRUE;
    }
    return selection;
}

static void Difficulty_DrawChoices(u8 selection)
{
    u8 styles[3];
    /* FALSE = Have the middle text be exactly in between where the first text ends and second text begins.
       TRUE = Have the mid text be in the middle of the frame, ignoring the first and last text size. 
    Setting it to FALSE is how vanilla code does it for the TEST SPEED, but the layout looks off-center if there's
    multiple three-item options in one page and the length of characters for the first and last choices
    of one of the options mismatch.*/
    bool8 centerMid = TRUE;
    s32 widthEasy, widthNormal, widthHard, xMid;

    styles[0] = 0;
    styles[1] = 0;
    styles[2] = 0;
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_DifficultyEasy, 104, YPOS_DIFFICULTY, styles[0]);

    widthNormal = GetStringWidth(FONT_NORMAL, gText_DifficultyNormal, 0);
    if (centerMid){
        xMid = (94 - widthNormal) / 2 + 104;
    }
    else{
        widthEasy = GetStringWidth(FONT_NORMAL, gText_DifficultyEasy, 0);
        widthHard = GetStringWidth(FONT_NORMAL, gText_DifficultyHard, 0);
        widthNormal -= 94;
        xMid = (widthEasy - widthNormal - widthHard) / 2 + 104;
    }

    DrawOptionMenuChoice(gText_DifficultyNormal, xMid, YPOS_DIFFICULTY, styles[1]);

    DrawOptionMenuChoice(gText_DifficultyHard, GetStringRightAlignXOffset(FONT_NORMAL, gText_DifficultyHard, 198), YPOS_DIFFICULTY, styles[2]);
}

static u8 TextSpeed_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (selection <= 1)
            selection++;
        else
            selection = 0;

        sArrowPressed = TRUE;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (selection != 0)
            selection--;
        else
            selection = 2;

        sArrowPressed = TRUE;
    }
    return selection;
}

static void TextSpeed_DrawChoices(u8 selection)
{
    u8 styles[3];
    s32 widthSlow, widthMid, widthFast, xMid;

    styles[0] = 0;
    styles[1] = 0;
    styles[2] = 0;
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_TextSpeedSlow, 104, YPOS_TEXTSPEED, styles[0]);

    widthSlow = GetStringWidth(FONT_NORMAL, gText_TextSpeedSlow, 0);
    widthMid = GetStringWidth(FONT_NORMAL, gText_TextSpeedMid, 0);
    widthFast = GetStringWidth(FONT_NORMAL, gText_TextSpeedFast, 0);

    widthMid -= 94;
    xMid = (widthSlow - widthMid - widthFast) / 2 + 104;
    DrawOptionMenuChoice(gText_TextSpeedMid, xMid, YPOS_TEXTSPEED, styles[1]);

    DrawOptionMenuChoice(gText_TextSpeedFast, GetStringRightAlignXOffset(FONT_NORMAL, gText_TextSpeedFast, 198), YPOS_TEXTSPEED, styles[2]);
}

static u8 BattleScene_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void BattleScene_DrawChoices(u8 selection)
{
    u8 styles[2];

    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_BattleSceneOff, 104, YPOS_BATTLESCENE, styles[0]);
    DrawOptionMenuChoice(gText_BattleSceneOn, GetStringRightAlignXOffset(FONT_NORMAL, gText_BattleSceneOn, 198), YPOS_BATTLESCENE, styles[1]);
}

static u8 BattleStyle_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        sArrowPressed = TRUE;
    }

    return selection;
}

static void BattleStyle_DrawChoices(u8 selection)
{
    u8 styles[2];

    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_BattleStyleShift, 104, YPOS_BATTLESTYLE, styles[0]);
    DrawOptionMenuChoice(gText_BattleStyleSet, GetStringRightAlignXOffset(FONT_NORMAL, gText_BattleStyleSet, 198), YPOS_BATTLESTYLE, styles[1]);
}

static u8 Sound_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_LEFT | DPAD_RIGHT))
    {
        selection ^= 1;
        SetPokemonCryStereo(selection);
        sArrowPressed = TRUE;
    }

    return selection;
}

static void Sound_DrawChoices(u8 selection)
{
    u8 styles[2];

    styles[0] = 0;
    styles[1] = 0;
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_SoundMono, 104, YPOS_SOUND, styles[0]);
    DrawOptionMenuChoice(gText_SoundStereo, GetStringRightAlignXOffset(FONT_NORMAL, gText_SoundStereo, 198), YPOS_SOUND, styles[1]);
}

static u8 FrameType_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (selection < WINDOW_FRAMES_COUNT - 1)
            selection++;
        else
            selection = 0;

        LoadBgTiles(1, GetWindowFrameTilesPal(selection)->tiles, 0x120, 0x1A2);
        LoadPalette(GetWindowFrameTilesPal(selection)->pal, BG_PLTT_ID(7), PLTT_SIZE_4BPP);
        sArrowPressed = TRUE;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (selection != 0)
            selection--;
        else
            selection = WINDOW_FRAMES_COUNT - 1;

        LoadBgTiles(1, GetWindowFrameTilesPal(selection)->tiles, 0x120, 0x1A2);
        LoadPalette(GetWindowFrameTilesPal(selection)->pal, BG_PLTT_ID(7), PLTT_SIZE_4BPP);
        sArrowPressed = TRUE;
    }
    return selection;
}

static void FrameType_DrawChoices(u8 selection)
{
    u8 text[16] = {EOS};
    u8 n = selection + 1;
    u16 i;

    for (i = 0; gText_FrameTypeNumber[i] != EOS && i <= 5; i++)
        text[i] = gText_FrameTypeNumber[i];

    // Convert a number to decimal string
    if (n / 10 != 0)
    {
        text[i] = n / 10 + CHAR_0;
        i++;
        text[i] = n % 10 + CHAR_0;
        i++;
    }
    else
    {
        text[i] = n % 10 + CHAR_0;
        i++;
        text[i] = CHAR_SPACER;
        i++;
    }

    text[i] = EOS;

    DrawOptionMenuChoice(gText_FrameType, 104, YPOS_FRAMETYPE, 0);
    DrawOptionMenuChoice(text, 128, YPOS_FRAMETYPE, 1);
}

static u8 ButtonMode_ProcessInput(u8 selection)
{
    if (JOY_NEW(DPAD_RIGHT))
    {
        if (selection <= 1)
            selection++;
        else
            selection = 0;

        sArrowPressed = TRUE;
    }
    if (JOY_NEW(DPAD_LEFT))
    {
        if (selection != 0)
            selection--;
        else
            selection = 2;

        sArrowPressed = TRUE;
    }
    return selection;
}

static void ButtonMode_DrawChoices(u8 selection)
{
    s32 widthNormal, widthLR, widthLA, xLR;
    u8 styles[3];

    styles[0] = 0;
    styles[1] = 0;
    styles[2] = 0;
    styles[selection] = 1;

    DrawOptionMenuChoice(gText_ButtonTypeNormal, 104, YPOS_BUTTONMODE, styles[0]);

    widthNormal = GetStringWidth(FONT_NORMAL, gText_ButtonTypeNormal, 0);
    widthLR = GetStringWidth(FONT_NORMAL, gText_ButtonTypeLR, 0);
    widthLA = GetStringWidth(FONT_NORMAL, gText_ButtonTypeLEqualsA, 0);

    widthLR -= 94;
    xLR = (widthNormal - widthLR - widthLA) / 2 + 104;
    DrawOptionMenuChoice(gText_ButtonTypeLR, xLR, YPOS_BUTTONMODE, styles[1]);

    DrawOptionMenuChoice(gText_ButtonTypeLEqualsA, GetStringRightAlignXOffset(FONT_NORMAL, gText_ButtonTypeLEqualsA, 198), YPOS_BUTTONMODE, styles[2]);
}

static void DrawHeaderText(void)
{
    u32 i, widthOptions, xMid;
    u8 pageDots[9] = _("");  // Array size should be at least (2 * PAGE_COUNT) -1
    widthOptions = GetStringWidth(FONT_NORMAL, gText_Option, 0);

    for (i = 0; i < PAGE_COUNT; i++)
    {
        if (i == sCurrPage)
            StringAppend(pageDots, gText_LargeDot);
        else
            StringAppend(pageDots, gText_SmallDot);
        if (i < PAGE_COUNT - 1)
            StringAppend(pageDots, gText_Space);            
    }
    xMid = (8 + widthOptions + 5);
    FillWindowPixelBuffer(WIN_HEADER, PIXEL_FILL(1));
    AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_Option, 8, 1, TEXT_SKIP_DRAW, NULL);
    AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, pageDots, xMid, 1, TEXT_SKIP_DRAW, NULL);
    {
    if (sCurrPage == 0)
        AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_PageNav, GetStringRightAlignXOffset(FONT_NORMAL, gText_PageNav, 198), 1, TEXT_SKIP_DRAW, NULL);
    else if (sCurrPage == 1)
        AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_PageNav, GetStringRightAlignXOffset(FONT_NORMAL, gText_PageNav, 198), 1, TEXT_SKIP_DRAW, NULL);         
    else if (sCurrPage == 2)
        AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_PageNav3, GetStringRightAlignXOffset(FONT_NORMAL, gText_PageNav, 201), 1, TEXT_SKIP_DRAW, NULL);
    else if (sCurrPage == 3)
        AddTextPrinterParameterized(WIN_HEADER, FONT_NORMAL, gText_PageNav4, GetStringRightAlignXOffset(FONT_NORMAL, gText_PageNav, 204), 1, TEXT_SKIP_DRAW, NULL);
    }
    CopyWindowToVram(WIN_HEADER, COPYWIN_FULL);
}

static void DrawOptionMenuTexts(void)
{
    u8 i, items;
    items = 0;
    const u8* const* menu = NULL;

    switch (sCurrPage){
    case 0:
        items = MENUITEM_COUNT;
        menu = sOptionMenuItemsNames;
        break;
    case 1:
        items = MENUITEM_COUNT_PG2;
        menu = sOptionMenuItemsNames_Pg2;
        break;    
    case 2:
        items = MENUITEM_COUNT_PG3;
        menu = sOptionMenuItemsNames_Pg3;
        break;
    case 3:
        items = MENUITEM_COUNT_PG4;
        menu = sOptionMenuItemsNames_Pg4;
        break;      
    }

    FillWindowPixelBuffer(WIN_OPTIONS, PIXEL_FILL(1));
    for (i = 0; i < items; i++)
        AddTextPrinterParameterized(WIN_OPTIONS, FONT_NORMAL, menu[i], 8, (i * 16) + 1, TEXT_SKIP_DRAW, NULL);
    CopyWindowToVram(WIN_OPTIONS, COPYWIN_FULL);
}

#define TILE_TOP_CORNER_L 0x1A2
#define TILE_TOP_EDGE     0x1A3
#define TILE_TOP_CORNER_R 0x1A4
#define TILE_LEFT_EDGE    0x1A5
#define TILE_RIGHT_EDGE   0x1A7
#define TILE_BOT_CORNER_L 0x1A8
#define TILE_BOT_EDGE     0x1A9
#define TILE_BOT_CORNER_R 0x1AA

static void DrawBgWindowFrames(void)
{
    //                     bg, tile,              x, y, width, height, palNum
    // Draw title window frame
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  1,  0,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      2,  0, 27,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 28,  0,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     1,  1,  1,  2,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   28,  1,  1,  2,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  1,  3,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      2,  3, 27,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 28,  3,  1,  1,  7);

    // Draw options list window frame
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_L,  1,  4,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_EDGE,      2,  4, 26,  1,  7);
    FillBgTilemapBufferRect(1, TILE_TOP_CORNER_R, 28,  4,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_LEFT_EDGE,     1,  5,  1, 18,  7);
    FillBgTilemapBufferRect(1, TILE_RIGHT_EDGE,   28,  5,  1, 18,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_L,  1, 19,  1,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_EDGE,      2, 19, 26,  1,  7);
    FillBgTilemapBufferRect(1, TILE_BOT_CORNER_R, 28, 19,  1,  1,  7);

    CopyBgTilemapBufferToVram(1);
}
