#pragma once

#include "SDK.h"

/*===================================================================================
//Weapon ID Define List
//
//http://www.tf2items.com/allitems.php?d=1
//===================================================================================*/

enum globalweapons {
	TF_WEAPON_NONE = 0, //0x00
	TF_WEAPON_BAT = 1, //0x01
	TF_WEAPON_BAT_WOOD = 2, //0x02
	TF_WEAPON_BOTTLE = 3, //0x03
	TF_WEAPON_FIREAXE = 4, //0x04
	TF_WEAPON_CLUB = 5, //0x05
	TF_WEAPON_CROWBAR = 6, //0x06
	TF_WEAPON_KNIFE = 7, //0x07
	TF_WEAPON_FISTS = 8, //0x08
	TF_WEAPON_SHOVEL = 9, //0x09
	TF_WEAPON_WRENCH = 10, //0x010
	TF_WEAPON_BONESAW = 11, //0x011
	TF_WEAPON_SHOTGUN_PRIMARY = 12, //0x012
	TF_WEAPON_SHOTGUN_SOLDIER = 13, //0x013
	TF_WEAPON_SHOTGUN_HWG = 14, //0x014
	TF_WEAPON_SHOTGUN_PYRO = 15, //0x015
	TF_WEAPON_SCATTERGUN = 16, //0x016
	TF_WEAPON_SNIPERRIFLE = 17, //0x017
	TF_WEAPON_MINIGUN = 18, //0x018
	TF_WEAPON_SMG = 19, //0x019
	TF_WEAPON_SYRINGEGUN_MEDIC = 20, //0x020
	TF_WEAPON_TRANQ = 21, //0x021
	TF_WEAPON_ROCKETLAUNCHER = 22, //0x022
	TF_WEAPON_GRENADELAUNCHER = 23, //0x023
	TF_WEAPON_PIPEBOMBLAUNCHER = 24, //0x024
	TF_WEAPON_FLAMETHROWER = 25, //0x025
	TF_WEAPON_GRENADE_NORMAL = 26, //0x026
	TF_WEAPON_GRENADE_CONCUSSION = 27, //0x027
	TF_WEAPON_GRENADE_NAIL = 28, //0x028
	TF_WEAPON_GRENADE_MIRV = 29, //0x029
	TF_WEAPON_GRENADE_MIRV_DEMOMAN = 30, //0x030
	TF_WEAPON_GRENADE_NAPALM = 31, //0x031
	TF_WEAPON_GRENADE_GAS = 32, //0x032
	TF_WEAPON_GRENADE_EMP = 33, //0x033
	TF_WEAPON_GRENADE_CALTROP = 34, //0x034
	TF_WEAPON_GRENADE_PIPEBOMB = 35, //0x035
	TF_WEAPON_GRENADE_SMOKE_BOMB = 36, //0x036
	TF_WEAPON_GRENADE_HEAL = 37, //0x037
	TF_WEAPON_GRENADE_STUNBALL = 38, //0x038
	TF_WEAPON_GRENADE_JAR = 39, //0x039
	TF_WEAPON_GRENADE_JAR_MILK = 40, //0x040
	TF_WEAPON_PISTOL = 41, //0x041
	TF_WEAPON_PISTOL_SCOUT = 42, //0x042
	TF_WEAPON_REVOLVER = 43, //0x043
	TF_WEAPON_NAILGUN = 44, //0x044
	TF_WEAPON_PDA = 45, //0x045
	TF_WEAPON_PDA_ENGINEER_BUILD = 46, //0x046
	TF_WEAPON_PDA_ENGINEER_DESTROY = 47, //0x047
	TF_WEAPON_PDA_SPY = 48, //0x048
	TF_WEAPON_BUILDER = 49, //0x049
	TF_WEAPON_MEDIGUN = 50, //0x050
	TF_WEAPON_GRENADE_MIRVBOMB = 51, //0x051
	TF_WEAPON_FLAMETHROWER_ROCKET = 52, //0x052
	TF_WEAPON_GRENADE_DEMOMAN = 53, //0x053
	TF_WEAPON_SENTRY_BULLET = 54, //0x054
	TF_WEAPON_SENTRY_ROCKET = 55, //0x055
	TF_WEAPON_DISPENSER = 56, //0x056
	TF_WEAPON_INVIS = 57, //0x057
	TF_WEAPON_FLAREGUN = 58, //0x058
	TF_WEAPON_LUNCHBOX = 59, //0x059
	TF_WEAPON_JAR = 60, //0x060
	TF_WEAPON_COMPOUND_BOW = 61, //0x061
	TF_WEAPON_BUFF_ITEM = 62, //0x062
	TF_WEAPON_PUMPKIN_BOMB = 63, //0x063
	TF_WEAPON_SWORD = 64, //0x064
	TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT = 65, //0x065
	TF_WEAPON_LIFELINE = 66, //0x066
	TF_WEAPON_LASER_POINTER = 67, //0x067
	TF_WEAPON_DISPENSER_GUN = 68, //0x068
	TF_WEAPON_SENTRY_REVENGE = 69, //0x069
	TF_WEAPON_JAR_MILK = 70, //0x070
	TF_WEAPON_HANDGUN_SCOUT_PRIMARY = 71, //0x071
	TF_WEAPON_BAT_FISH = 72, //0x072
	TF_WEAPON_CROSSBOW = 73, //0x073
	TF_WEAPON_STICKBOMB = 74, //0x074
	TF_WEAPON_HANDGUN_SCOUT_SECONDARY = 75, //0x075
	TF_WEAPON_SODA_POPPER = 76, //0x076
	TF_WEAPON_SNIPERRIFLE_DECAP = 77, //0x077
	TF_WEAPON_RAYGUN = 78, //0x078
	TF_WEAPON_PARTICLE_CANNON = 79, //0x079
	TF_WEAPON_MECHANICAL_ARM = 80, //0x080
	TF_WEAPON_DRG_POMSON = 81, //0x081
	TF_WEAPON_BAT_GIFTWRAP = 82, //0x082
	TF_WEAPON_GRENADE_ORNAMENT_BALL = 83, //0x083
	TF_WEAPON_FLAREGUN_REVENGE = 84, //0x084
	TF_WEAPON_PEP_BRAWLER_BLASTER = 85, //0x085
	TF_WEAPON_CLEAVER = 86, //0x086
	TF_WEAPON_GRENADE_CLEAVER = 87, //0x087
	TF_WEAPON_STICKY_BALL_LAUNCHER = 88, //0x088
	TF_WEAPON_GRENADE_STICKY_BALL = 89, //0x089
	TF_WEAPON_SHOTGUN_BUILDING_RESCUE = 90, //0x090
	TF_WEAPON_CANNON = 91, //0x091
	TF_WEAPON_THROWABLE = 92, //0x092
	TF_WEAPON_GRENADE_THROWABLE = 93, //0x093
	TF_WEAPON_PDA_SPY_BUILD = 94, //0x094
	TF_WEAPON_GRENADE_WATERBALLOON = 95, //0x095
	TF_WEAPON_HARVESTER_SAW = 96, //0x096
	TF_WEAPON_SPELLBOOK = 97, //0x097
	TF_WEAPON_SPELLBOOK_PROJECTILE = 98, //0x098
	TF_WEAPON_SNIPERRIFLE_CLASSIC = 99, //0x099
	TF_WEAPON_PARACHUTE = 100, //0x0100
	TF_WEAPON_GRAPPLINGHOOK = 101, //0x0101
	TF_WEAPON_PASSTIME_GUN = 102, //0x0102
	TF_WEAPON_CHARGED_SMG = 103, //0x0103
	TF_WEAPON_BREAKABLE_SIGN = 104, //0x0104
	TF_WEAPON_ROCKETPACK = 105, //0x0105
	TF_WEAPON_SLAP = 106, //0x0106
	TF_WEAPON_JAR_GAS = 107, //0x0107
	TF_WEAPON_GRENADE_JAR_GAS = 108, //0x0108
	TF_WEPON_FLAME_BALL = 109, //0x0109

}; //Size: 0x0110

enum scoutweapons
{
	//Primary
	WPN_Scattergun = 13,
	WPN_NewScattergun = 200,
	WPN_FAN = 45,
	WPN_Shortstop = 220,
	WPN_SodaPopper = 448,
	WPN_BabyFaceBlaster = 772,
	WPN_BotScattergunS = 799,
	WPN_BotScattergunG = 808,
	WPN_BotScattergunR = 888,
	WPN_BotScattergunB = 897,
	WPN_BotScattergunC = 906,
	WPN_BotScattergunD = 915,
	WPN_BotScattergunES = 964,
	WPN_BotScattergunEG = 973,
	//Secondary
	WPN_ScoutPistol = 23,
	WPN_NewPistol = 209,
	WPN_Bonk = 46,
	WPN_CritCola = 163,
	WPN_Lugermorph = 160,
	WPN_Milk = 222,
	WPN_Lugermorph2 = 294,
	WPN_Winger = 449,
	WPN_PocketPistol = 773,
	//Melee
	WPN_Bat = 0,
	WPN_NewBat = 190,
	WPN_Sandman = 44,
	WPN_Fish = 221,
	WPN_Cane = 317,
	WPN_BostonBasher = 325,
	WPN_SunStick = 349,
	WPN_FanOWar = 355,
	WPN_RuneBlade = 452,
	WPN_Saxxy = 423,
	WPN_Atomizer = 450,
	WPN_ConscientiousObjector = 474,
	WPN_UnarmedCombat = 572,
	WPN_WrapAssassin = 648,
	WPN_FestiveBat = 660,
	WPN_FestiveScattergun = 669,
	WPN_FlyingGuillotine1 = 812,
	WPN_FlyingGuillotine2 = 833,
	WPN_FreedomStaff = 880,
	WPN_BatOuttaHell = 939,
	WPN_FestiveFish = 999,
};
enum soldierweapons
{
	//Primary
	WPN_RocketLauncher = 18,
	WPN_NewRocketLauncher = 205,
	WPN_DirectHit = 127,
	WPN_BlackBox = 228,
	WPN_RocketJump = 237,
	WPN_LibertyLauncher = 414,
	WPN_CowMangler = 441,
	WPN_Original = 513,
	WPN_FestiveRocketLauncher = 658,
	WPN_BeggersBazooka = 730,
	WPN_BotRocketlauncherS = 800,
	WPN_BotRocketlauncherG = 809,
	WPN_BotRocketlauncherR = 889,
	WPN_BotRocketlauncherB = 898,
	WPN_BotRocketlauncherC = 907,
	WPN_BotRocketlauncherD = 916,
	WPN_BotRocketlauncherES = 965,
	WPN_BotRocketlauncherEG = 974,
	//Secondary
	WPN_SoldierShotgun = 10,
	WPN_NewShotgun = 199,
	WPN_BuffBanner = 129,
	WPN_BattalionBackup = 226,
	WPN_Concheror = 354,
	WPN_ReserveShooter = 415,
	WPN_RighteousBison = 442,
	WPN_FestiveBuffBanner = 1001,
	//Melee
	WPN_Shovel = 6,
	WPN_NewShovel = 196,
	WPN_Equalizer = 128,
	WPN_PainTrain = 154,
	WPN_Katana = 357,
	WPN_MarketGardener = 416,
	WPN_DisciplinaryAction = 447,
	WPN_EscapePlan = 775,
};
enum pyroweapons
{
	//Primary
	WPN_Flamethrower = 21,
	WPN_NewFlamethrower = 208,
	WPN_Backburner = 40,
	WPN_Degreaser = 215,
	WPN_FestiveFlamethrower = 659,
	WPN_Phlogistinator = 594,
	WPN_Rainblower = 741,
	WPN_BotFlamethrowerS = 798,
	WPN_BotFlamethrowerG = 807,
	WPN_BotFlamethrowerR = 887,
	WPN_BotFlamethrowerB = 896,
	WPN_BotFlamethrowerC = 905,
	WPN_BotFlamethrowerD = 914,
	WPN_BotFlamethrowerES = 963,
	WPN_BotFlamethrowerEG = 972,
	//Secondary
	WPN_PyroShotgun = 12,
	WPN_Flaregun = 39,
	WPN_Detonator = 351,
	WPN_ManMelter = 595,
	WPN_ScorchShot = 740,
	//Melee
	WPN_Fireaxe = 2,
	WPN_NewAxe = 192,
	WPN_Axtingusher = 38,
	WPN_HomeWrecker = 153,
	WPN_PowerJack = 214,
	WPN_Backscratcher = 326,
	WPN_VolcanoFragment = 348,
	WPN_Maul = 466,
	WPN_Mailbox = 457,
	WPN_ThirdDegree = 593,
	WPN_Lollychop = 739,
	WPN_NeonAnnihilator1 = 813,
	WPN_NeonAnnihilator2 = 834,
	WPN_FestiveAxtwingisher = 1000,
};
enum demomanweapons
{
	//Primary
	WPN_GrenadeLauncher = 19,
	WPN_NewGrenadeLauncher = 206,
	WPN_LochNLoad = 308,
	WPM_LoooseCannon = 996,
	WPN_FestiveGrenadeLauncher = 1007,
	//Secondary
	WPN_StickyLauncher = 20,
	WPN_NewStickyLauncher = 207,
	WPN_ScottishResistance = 130,
	WPN_StickyJumper = 265,
	WPN_FestiveStickyLauncher = 661,
	WPN_BotStickyS = 797,
	WPN_BotStickyG = 806,
	WPN_BotStickyR = 886,
	WPN_BotStickyB = 895,
	WPN_BotStickyC = 904,
	WPN_BotStickyD = 913,
	WPN_BotStickyES = 962,
	WPN_BotStickyEG = 971,
	//Melee
	WPN_Bottle = 1,
	WPN_NewBottle = 191,
	WPN_Sword = 132,
	WPN_ScottsSkullctter = 172,
	WPN_Fryingpan = 264,
	WPN_Headless = 266,
	WPN_Ullapool = 307,
	WPN_Claidheamhmor = 327,
	WPN_PersainPersuader = 404,
	WPN_Golfclub = 482,
	WPN_ScottishHandshake = 609,
};
enum heavyweapons
{
	//Primary
	WPN_Minigun = 15,
	WPN_NewMinigun = 202,
	WPN_Natascha =  41,
	WPN_IronCurtain = 298,
	WPN_BrassBeast = 312,
	WPN_Tomislav = 424,
	WPN_FestiveMinigun = 654,
	WPN_HuoLongHeatmaker1 = 811,
	WPN_HuoLongHeatmaker2 = 832,
	WPN_BotMinigunS = 793,
	WPN_BotMinigunG = 802,
	WPN_BotMinigunR = 882,
	WPN_BotMinigunB = 891,
	WPN_BotMinigunC = 900,
	WPN_BotMinigunD = 909,
	WPN_Deflector = 850,
	WPN_BotMinigunES = 958,
	WPN_BotMinigunEG = 967,
	//Secondary
	WPN_HeavyShotgun = 11,
	WPN_Sandvich = 42,
	WPN_CandyBar = 159,
	WPN_Steak = 311,
	WPN_Fishcake = 433,
	WPN_FamilyBuisness = 425, //GIVE 'EM THE BUISSNESS, TONY!
	WPN_RobotSandvich = 863,
	WPN_FestiveSandvich = 1002,
	//Melee
	WPN_Fists = 5,
	WPN_NewFists = 195,
	WPN_KGB = 43,
	WPN_GRU = 239,
	WPN_WarriorSpirit = 310,
	WPN_FistsOfSteel = 331,
	WPN_EvictionNotice = 426,
	WPN_ApocoFists = 587,
	WPN_HolidayPunch = 656,
};
enum engineerweapons
{
	//Primary
	WPN_EngineerShotgun = 9,
	WPN_FrontierJustice = 141,
	WPN_Widowmaker = 527,
	WPN_Pomson = 588,
	WPN_RescueRanger = 997,
	WPN_FestiveFrontierJustice = 1004,
	//Secondary
	WPN_EngineerPistol = 22,
	WPN_Wrangler = 140,
	WPN_ShortCircut = 528,
	//Melee
	WPN_Wrench = 7,
	WPN_NewWrench = 197,
	WPN_Goldenwrench = 169,
	WPN_SouthernHospitality = 155,
	WPN_Gunslinger = 142,
	WPN_Jag = 329,
	WPN_FestiveWrench = 662,
	WPN_EurekaEffect = 589,
	WPN_BotWrenchS = 795,
	WPN_BotWrenchG = 804,
	WPN_BotWrenchR = 884,
	WPN_BotWrenchB = 893,
	WPN_BotWrenchC = 902,
	WPN_BotWrenchD = 911,
	WPN_BotWrenchES = 960,
	WPN_BotWrenchEG = 969,
	//Misc
	WPN_Builder = 25,
	WPN_Builder2 = 737,
	WPN_Destructor = 26,
	WPN_Toolbox = 28,
};
enum medicweapons
{
	//Primary
	WPN_SyringeGun = 17,
	WPN_NewSyringeGun = 204,
	WPN_Blutsauger = 36,
	WPN_Crossbow = 305,
	WPN_Overdose = 412,
	//Secondary
	WPN_Medigun = 29,
	WPN_NewMedigun = 211,
	WPN_Kritzkrieg = 35,
	WPN_QuickFix = 411,
	WPN_FestiveMedigun = 663,
	WPN_MedigunS = 796,
	WPN_MedigunG = 805,
	WPN_MedigunR = 885,
	WPN_MedigunB = 894,
	WPN_MedigunC = 903,
	WPN_MedigunD = 912,
	WPN_Vaccinator = 998,
	WPN_MedigunES = 961,
	WPN_MedigunEG = 970,
	//Melee
	WPN_Bonesaw = 8,
	WPN_NewBonesaw = 198,
	WPN_Ubersaw = 37,
	WPN_Vitasaw = 173,
	WPN_Amputator = 304,
	WPN_Solemnvow = 413,
	WPN_FestiveUbersaw = 1003,
};
enum sniperweapons
{
	//Primary
	WPN_SniperRifle = 14,
	WPN_NewSniperRifle = 201,
	WPN_Huntsman = 56,
	WPN_SydneySleeper = 230,
	WPN_Bazaarbargain = 402,
	WPN_Machina = 526,
	WPN_FestiveSniperRifle = 664,
	WPN_HitmanHeatmaker = 752,
	WPN_BotSniperRifleS = 792,
	WPN_BotSniperRifleG = 801,
	WPN_BotSniperRifleR = 881,
	WPN_BotSniperRifleB = 890,
	WPN_BotSniperRifleC = 899,
	WPN_BotSniperRifleD = 908,
	WPN_AWP = 851,
	WPN_BotSniperRifleES = 957,
	WPN_BotSniperRifleEG = 966,
	WPN_FestiveHuntsman = 1005,
	//Secondary
	WPN_SMG = 16,
	WPN_NewSMG = 203,
	WPN_Jarate = 58,
	WPN_DarwinDangerShield = 231,
	WPN_CleanersCarbine = 751,
	//Melee
	WPN_Kukri = 3,
	WPN_NewKukri = 193,
	WPN_TribalmansShiv = 171,
	WPN_Bushwacka = 232,
	WPN_Shahanshah = 401,
	//Hats
	HAT_Snaggletooth = 229,
};
enum spyweapons
{
	//Primary
	WPN_Revolver = 24,
	WPN_NewRevolver = 210,
	WPN_Ambassador =  61,
	WPN_BigKill = 161,
	WPN_Letranger = 224,
	WPN_Enforcer = 460,
	WPN_Diamondback = 525,
	WPN_FestiveAmbassador = 1006,
	//Melee
	WPN_Knife = 4,
	WPN_NewKnife = 194,
	WPN_EternalReward = 225,
	WPN_DisguiseKit = 27,
	WPN_Kunai = 356,
	WPN_BigEarner = 461,
	WPN_WangaPrick = 574,
	WPN_SharpDresser = 638,
	WPN_Spycicle = 649,
	WPN_FestiveKnife = 665,
	WPN_BlackRose = 727,
	WPN_BotKnifeS = 794,
	WPN_BotKnifeG = 803,
	WPN_BotKnifeR = 883,
	WPN_BotKnifeB = 892,
	WPN_BotKnifeC = 901,
	WPN_BotKnifeD = 910,
	WPN_BotKnifeES = 959,
	WPN_BotKnifeEG = 968,
	//Watches
	WPN_InivisWatch = 30,
	WPN_DeadRinger = 59,
	WPN_CloakAndDagger = 60,
	WPN_TimePiece = 297,
	//Sapper
	WPN_Sapper = 735,
	WPN_Sapper2 = 736,
	WPN_RedTape1 = 810,
	WPN_RedTape2 = 831,
};