#ifndef BOT_H
#define BOT_H

#include "cell.h"


class Bot : public Cell
{
public:
    enum GenomCommands{
        STAND,
        GO,

        CHANGEDIRPOSITIVE,
        CHANGEDIRNEGAITIVE,

        SUICIDE,

        PHOTO,

        ATTACK,

        EATSUGAR,
        EATMINERALS,
        EATTALLOW,

        SHARESUGAR,
        SHAREMINERALS,
        SHARETALLOW,

        USESUGAR,
        USEMINERALS,
        USETALLOW,

        LOOKENERGY,
        LOOKUP,

        CLONE,

        MOVEINDEX1,
        MOVEINDEX2,
        MOVEINDEX3,
        MOVEINDEX4,
        MOVEINDEX5,
        MOVEINDEX6,
        MOVEINDEX7,
        MOVEINDEX8,
        MOVEINDEX9,
        MOVEINDEX10,
        MOVEINDEX11,
        MOVEINDEX12,
        MOVEINDEX13,
        MOVEINDEX14,
        MOVEINDEX15,
        MOVEINDEX16,
        MOVEINDEX17,
        MOVEINDEX18,
        MOVEINDEX19,
        MOVEINDEX20,
        MOVEINDEX21,
        MOVEINDEX22,
        MOVEINDEX23,
        MOVEINDEX24,
        MOVEINDEX25,
        MOVEINDEX26,
        MOVEINDEX27,
        MOVEINDEX28,
        MOVEINDEX29,
        MOVEINDEX30,
        MOVEINDEX31,
        MOVEINDEX32,
        MOVEINDEX33,
        MOVEINDEX34,
        MOVEINDEX35,
        MOVEINDEX36,
        MOVEINDEX37,
        MOVEINDEX38,
        MOVEINDEX39,
        MOVEINDEX40,
        MOVEINDEX41,
        MOVEINDEX42,
        MOVEINDEX43,
        MOVEINDEX44,
        MOVEINDEX45,
        MOVEINDEX46,
        MOVEINDEX47,
        MOVEINDEX48,
        MOVEINDEX49,
        MOVEINDEX50
    };
    static QString genomCommandsToString(Bot::GenomCommands value);
    static float defenceFromDefenceMinerals(float minerals);
    static float longLiveValueFromSugar(float sugar);

    enum Directions{
        RIGHT,
        UPRIGHT,
        UP,
        UPLEFT,
        LEFT,
        DOWNLEFT,
        DOWN,
        DOWNRIGHT
    };
    static const unsigned short genomSize;
    static const float energyMax;
    static uchar tolerance;

    static float photoSugarMax;
    static float photoSugarMin;
    static float photoSugarBorder;

    static float sugarToEnergyKof;
    static float mineralsToEnergyKof;
    static float tallowToEnergyKof;
    static float tallowToHealthKof;

    static uint botsliveTime;
    static float everyTurnCost;
    static float oldCost;

    static float defenceCoolDown;
    static float longLiveCoolDown;

    static float healthMax;
    static float tallowFromDead;

    static float moveCost;
    static float attackCost;
    static float photoCost;
    static float cloneCost;
    static float damage;

    static float minEnergyToClone;
    static float startBotEnergy;

    static float genomMutationChance;
    static float parametrsMutationChance;
    static float mutationSpeedOfParamets;

    static uchar* tempDirections;
    static bool captureAttack;
    static bool effectiveEat;

    static float defenceKof;
    static float longLiveKof;

    Bot(unsigned short x,unsigned short y);

    ~Bot();
    void clearBot();
    uchar recurtionCounter = 0;
    void readNextCommand();

    void doSimpleIntention();
    void doMoveIntention();
    void doAttackIntention();
    void doCloneIntention();

    void directionToXY(unsigned short &x,unsigned short &y);
    void directionToXY(Directions dir,unsigned short &x,unsigned short &y);
    uchar genomDifferenceFrom(Bot *otherBot);

    unsigned short* genom = nullptr;
    unsigned short genomIndex = 0;

    uchar intentionCommand = 0;
    uchar direction = 0;



    float health = 0;
    float energy = 0;

    float carrySugar = 0;
    float eatSugarKof = 1;
    float shareSugarKof = 1;
    float useSugarKof = 1;
    float longLiveSugar = 0;

    float carryMinerals = 0;
    float eatMineralsKof = 1;
    float shareMineralsKof = 1;
    float useMineralsKof = 1;
    float defenceMinerals = 0;

    float carryTallow = 0;
    float eatTallowKof = 1;
    float shareTallowKof = 1;
    float useTallowKof = 1;

    float photoUser = 0;
    float mineralsUser = 0;
    float tallowUser = 0;

    uint turnCount = 0;
    uint cloneCount = 0;
    uint killCount = 0;




    void payForLive();
    void payForMove();
    void payForAttack();
    void payForClone();
    void payForPhoto();
    void calculateLifeStyle();
private:
    Bot(const Bot &other);
    Bot();

};

#endif // BOT_H
