#include "bot.h"
#include "game.h"
#include "empty.h"

const unsigned short Bot::genomSize = 100;
const float Bot::energyMax = 100;

uchar Bot::tolerance = 2;

float Bot::photoSugarMax = 5;
float Bot::photoSugarMin = 0;
float Bot::photoSugarBorder = 256;

float Bot::sugarToEnergyKof = 1;
float Bot::mineralsToEnergyKof = 1;
float Bot::tallowToEnergyKof = 1;
float Bot::tallowToHealthKof = 1;

float Bot::everyTurnCost = 0.1;
uint Bot::botsliveTime = 100;

float Bot::oldCost = 0.5;

float Bot::defenceCoolDown = 0.1;
float Bot::longLiveCoolDown = 0.1;

float Bot::healthMax = 100;

float Bot::tallowFromDead = 15;

float Bot::moveCost = 1;
float Bot::attackCost = 5;
float Bot::photoCost = 0.2;
float Bot::damage = 100;
float Bot::minEnergyToClone = 50;
float Bot::cloneCost = 45;
float Bot::startBotEnergy = 30;
float Bot::genomMutationChance = 0.2;
float Bot::parametrsMutationChance = 0.2;
float Bot::mutationSpeedOfParamets = 0.12;

bool Bot::captureAttack = false;
bool Bot::effectiveEat = true;

float Bot::defenceKof = 1;
float Bot::longLiveKof = 10;

bool Bot::rottingDead = false;
float Bot::rottingTurns = 1000;
bool Bot::rottingTallow = false;


uchar* Bot::tempDirections = new uchar[8];


QString Bot::genomCommandsToString(Bot::GenomCommands value)
{
    switch (value) {
        case STAND                 : return "СТОЙ";
        case GO                    : return "ИДИ";
        case CHANGEDIRPOSITIVE     : return "ВЕРТИСЬ ПРОТИВ";
        case CHANGEDIRNEGAITIVE    : return "ВЕРТИСЬ ПО";
        case SUICIDE               : return "САМОУБИЙСТВО";
        case PHOTO                 : return "ФОТО";
        case ATTACK                : return "АТАКА";
        case EATSUGAR              : return "ЕШЬ САХАР";
        case EATMINERALS           : return "ЕШЬ МИНЕРАЛЫ";
        case EATTALLOW             : return "ЕШЬ ЖИР";
        case SHARESUGAR            : return "ОТДАЙ САХАР";
        case SHAREMINERALS         : return "ОТДАЙ МИНЕРАЛЫ";
        case SHARETALLOW           : return "ОТДАЙ ЖИР";
        case USESUGAR              : return "ИСПОЛЬЗУЙ САХАР";
        case USEMINERALS           : return "ИСПОЛЬЗУЙ МИНЕРАЛЫ";
        case USETALLOW             : return "ИСПОЛЬЗУЙ ЖИР";
        case LOOKENERGY            : return "ПРОВЕРЬ ЭНЕРГИЮ";
        case LOOKUP                : return "ПОСМОТРИ ВПЕРЕД";
        case CLONE                 : return "ОТПОЧКОВАТЬ";
        case MOVEINDEX1            : return "ПЕРЕХОД НА 1";
        case MOVEINDEX2            : return "ПЕРЕХОД НА 2";
        case MOVEINDEX3            : return "ПЕРЕХОД НА 3";
        case MOVEINDEX4            : return "ПЕРЕХОД НА 4";
        case MOVEINDEX5            : return "ПЕРЕХОД НА 5";
        case MOVEINDEX6            : return "ПЕРЕХОД НА 6";
        case MOVEINDEX7            : return "ПЕРЕХОД НА 7";
        case MOVEINDEX8            : return "ПЕРЕХОД НА 8";
        case MOVEINDEX9            : return "ПЕРЕХОД НА 9";
        case MOVEINDEX10           : return "ПЕРЕХОД НА 10";
        case MOVEINDEX11           : return "ПЕРЕХОД НА 11";
        case MOVEINDEX12           : return "ПЕРЕХОД НА 12";
        case MOVEINDEX13           : return "ПЕРЕХОД НА 13";
        case MOVEINDEX14           : return "ПЕРЕХОД НА 14";
        case MOVEINDEX15           : return "ПЕРЕХОД НА 15";
        case MOVEINDEX16           : return "ПЕРЕХОД НА 16";
        case MOVEINDEX17           : return "ПЕРЕХОД НА 17";
        case MOVEINDEX18           : return "ПЕРЕХОД НА 18";
        case MOVEINDEX19           : return "ПЕРЕХОД НА 19";
        case MOVEINDEX20           : return "ПЕРЕХОД НА 20";
        case MOVEINDEX21           : return "ПЕРЕХОД НА 21";
        case MOVEINDEX22           : return "ПЕРЕХОД НА 22";
        case MOVEINDEX23           : return "ПЕРЕХОД НА 23";
        case MOVEINDEX24           : return "ПЕРЕХОД НА 24";
        case MOVEINDEX25           : return "ПЕРЕХОД НА 25";
        case MOVEINDEX26           : return "ПЕРЕХОД НА 26";
        case MOVEINDEX27           : return "ПЕРЕХОД НА 27";
        case MOVEINDEX28           : return "ПЕРЕХОД НА 28";
        case MOVEINDEX29           : return "ПЕРЕХОД НА 29";
        case MOVEINDEX30           : return "ПЕРЕХОД НА 30";
        case MOVEINDEX31           : return "ПЕРЕХОД НА 31";
        case MOVEINDEX32           : return "ПЕРЕХОД НА 32";
        case MOVEINDEX33           : return "ПЕРЕХОД НА 33";
        case MOVEINDEX34           : return "ПЕРЕХОД НА 34";
        case MOVEINDEX35           : return "ПЕРЕХОД НА 35";
        case MOVEINDEX36           : return "ПЕРЕХОД НА 36";
        case MOVEINDEX37           : return "ПЕРЕХОД НА 37";
        case MOVEINDEX38           : return "ПЕРЕХОД НА 38";
        case MOVEINDEX39           : return "ПЕРЕХОД НА 39";
        case MOVEINDEX40           : return "ПЕРЕХОД НА 40";
        case MOVEINDEX41           : return "ПЕРЕХОД НА 41";
        case MOVEINDEX42           : return "ПЕРЕХОД НА 42";
        case MOVEINDEX43           : return "ПЕРЕХОД НА 43";
        case MOVEINDEX44           : return "ПЕРЕХОД НА 44";
        case MOVEINDEX45           : return "ПЕРЕХОД НА 45";
        case MOVEINDEX46           : return "ПЕРЕХОД НА 46";
        case MOVEINDEX47           : return "ПЕРЕХОД НА 47";
        case MOVEINDEX48           : return "ПЕРЕХОД НА 48";
        case MOVEINDEX49           : return "ПЕРЕХОД НА 49";
        case MOVEINDEX50           : return "ПЕРЕХОД НА 50";
        default: return QString();
    }
}

float Bot::defenceFromDefenceMinerals(float minerals)
{
    return minerals/(defenceKof+minerals);
}

float Bot::longLiveValueFromSugar(float sugar)
{
    return sugar/(longLiveKof+sugar);
}

Bot::Bot() : Cell()
{
    childType = Cell::bot;
    genom = new unsigned short[genomSize];
    for(unsigned short i = 0; i < genomSize; i++){
        genom[i] = STAND;
    }
}

Bot::Bot(unsigned short x, unsigned short y): Cell(x,y)
{
    childType = Cell::bot;
    genom = new unsigned short[genomSize];
    for(unsigned short i = 0; i < genomSize; i++){
        genom[i] = STAND;
    }
}

Bot::Bot(const Bot &other)
{
    childType = Cell::bot;
    genom = new unsigned short[genomSize];
    for(unsigned short i = 0; i < genomSize; i++){
        genom[i] = other.genom[i];
    }
    genomIndex = other.genomIndex;
    turnCount = other.turnCount;
    health = other.health;
    energy = other.energy;
    carryMinerals = other.carryMinerals;
    carrySugar = other.carrySugar;
    carryTallow = other.carryTallow;
    defenceMinerals = other.defenceMinerals;
    longLiveSugar = other.longLiveSugar;
    direction = other.direction;
    eatMineralsKof = other.eatMineralsKof;
    eatSugarKof = other.eatSugarKof;
    eatTallowKof = other.eatTallowKof;
    useMineralsKof = other.useMineralsKof;
    useSugarKof = other.useSugarKof;
    useTallowKof = other.useTallowKof;
    shareMineralsKof = other.shareMineralsKof;
    shareSugarKof = other.shareSugarKof;
    shareTallowKof = other.shareTallowKof;
    photoUser = other.photoUser;
    mineralsUser = other.mineralsUser;
    tallowUser = other.tallowUser;
    cloneCount = other.cloneCount;
}

Bot::~Bot()
{
    delete[] genom;
}


void Bot::clearBot()
{

}

void Bot::readNextCommand()
{
    recurtionCounter++;
    if(recurtionCounter == genomSize){
        intentionCommand = STAND;
        recurtionCounter = 0;
        return;
    }
    if(energy >= (energyMax-everyTurnCost)){
        intentionCommand = CLONE;

        energy = energyMax;

        recurtionCounter = 0;
        return;
    }

    genomIndex++;
    genomIndex = genomIndex % genomSize;

    switch (genom[genomIndex]){
        case GO:{
            intentionCommand = GO;
            unsigned short moveX = x;
            unsigned short moveY = y;
            directionToXY(moveX,moveY);
            if( Game::singleGame().world[moveY][moveX]->botToMoveOn == nullptr){
                 Game::singleGame().world[moveY][moveX]->botToMoveOn = this;
            }else{
                Bot *otherBot = (Bot*) Game::singleGame().world[moveY][moveX]->botToMoveOn;
                if(this->energy > otherBot->energy){
                    otherBot->intentionCommand = STAND;
                    otherBot->payForMove();
                     Game::singleGame().world[moveY][moveX]->botToMoveOn = this;
                }else{
                    payForMove();
                    intentionCommand = STAND;
                }
            }
            break;
        }
        case CHANGEDIRPOSITIVE:{
            direction != DOWNRIGHT ? direction++ : direction = RIGHT;
            readNextCommand();
            break;
        }
        case CHANGEDIRNEGAITIVE:{
            direction != RIGHT ? direction-- : direction = DOWNRIGHT;
            readNextCommand();
            break;
        }

        case LOOKENERGY:{
            if(energy > energyMax*0.5){
                genomIndex += 2;
            }else if(energy > energyMax*0.2){
                genomIndex += 1;
            }
            readNextCommand();
            break;
        }

        case LOOKUP:{

            unsigned short lookX = x;
            unsigned short lookY = y;
            directionToXY(lookX,lookY);

            if(Game::singleGame().world[lookY][lookX]->childType == Cell::bot){
                Bot *bot = (Bot*)Game::singleGame().world[lookY][lookX];
                if(bot->health < 0){
                    genomIndex += 1;
                }else {
                    if(genomDifferenceFrom(bot) <= tolerance){
                        //friendly bot
                        genomIndex += 2;
                    }else{
                        //not friendly bot
                        genomIndex += 3;
                    }
                }
            }// else if empty, readNextCommand
            readNextCommand();
            break;
        }
        case MOVEINDEX1:
        case MOVEINDEX2:
        case MOVEINDEX3:
        case MOVEINDEX4:
        case MOVEINDEX5:
        case MOVEINDEX6:
        case MOVEINDEX7:
        case MOVEINDEX8:
        case MOVEINDEX9:
        case MOVEINDEX10:
        case MOVEINDEX11:
        case MOVEINDEX12:
        case MOVEINDEX13:
        case MOVEINDEX14:
        case MOVEINDEX15:
        case MOVEINDEX16:
        case MOVEINDEX17:
        case MOVEINDEX18:
        case MOVEINDEX19:
        case MOVEINDEX20:
        case MOVEINDEX21:
        case MOVEINDEX22:
        case MOVEINDEX23:
        case MOVEINDEX24:
        case MOVEINDEX25:
        case MOVEINDEX26:
        case MOVEINDEX27:
        case MOVEINDEX28:
        case MOVEINDEX29:
        case MOVEINDEX30:
        case MOVEINDEX31:
        case MOVEINDEX32:
        case MOVEINDEX33:
        case MOVEINDEX34:
        case MOVEINDEX35:
        case MOVEINDEX36:
        case MOVEINDEX37:
        case MOVEINDEX38:
        case MOVEINDEX39:
        case MOVEINDEX40:
        case MOVEINDEX41:
        case MOVEINDEX42:
        case MOVEINDEX43:
        case MOVEINDEX44:
        case MOVEINDEX45:
        case MOVEINDEX46:
        case MOVEINDEX47:
        case MOVEINDEX48:
        case MOVEINDEX49:
        case MOVEINDEX50:{
            unsigned short indexToMove = (unsigned short)genom[genomIndex]-(unsigned short)MOVEINDEX1;
            genomIndex += indexToMove;
            readNextCommand();
            break;
        }
        default:{
            intentionCommand = genom[genomIndex];
            break;
        }
    }
    recurtionCounter--;
}

void Bot::doSimpleIntention()
{
    if(health > 0) {
        switch(intentionCommand){
            case STAND:{
                break;
            }
            case PHOTO:{
                float awayFromCenter = sqrt(powf(x-Game::worldWidth/2.0,2.0)+powf(y-Game::worldHeight/2.0,2.0));
                if(awayFromCenter < photoSugarBorder){
                    float newSugar = (photoSugarMax-photoSugarMin)*((photoSugarBorder-awayFromCenter)/photoSugarBorder) + photoSugarMin;
                    carrySugar += newSugar;
                    payForPhoto();
                }
                break;
            }
            case EATSUGAR:{
                if(effectiveEat){
                    float energyNeed = energyMax-energy;
                    if(carrySugar*eatSugarKof*sugarToEnergyKof <= energyNeed){
                        float shugarToEat = eatSugarKof*carrySugar;
                        carrySugar -= shugarToEat;
                        energy += shugarToEat*sugarToEnergyKof;
                    }else{
                        carrySugar -= (energyNeed/sugarToEnergyKof)/eatSugarKof;
                        energy += energyNeed;
                    }
                }else{
                    float shugarToEat = eatSugarKof*carrySugar;
                    carrySugar -= shugarToEat;
                    energy += shugarToEat*sugarToEnergyKof;
                    if(energy > energyMax){
                        energy = energyMax;
                    }
                }
                break;
            }
            case EATMINERALS:{
                if(effectiveEat){
                    float energyNeed = energyMax-energy;
                    if(carryMinerals*eatMineralsKof*mineralsToEnergyKof <= energyNeed){
                        float mineralsToEat = eatSugarKof*carryMinerals;
                        carryMinerals -= mineralsToEat;
                        energy += mineralsToEat*mineralsToEnergyKof;
                    }else{
                        carryMinerals -= (energyNeed/mineralsToEnergyKof)/eatMineralsKof;
                        energy += energyNeed;
                    }
                }else{
                    float mineralsToEat = eatSugarKof*carryMinerals;
                    carryMinerals -= mineralsToEat;
                    energy += mineralsToEat*mineralsToEnergyKof;
                    if(energy > energyMax){
                        energy = energyMax;
                    }
                }
                break;
            }
            case EATTALLOW:{
                if(effectiveEat){
                    float energyNeed = energyMax-energy;
                    if(carryTallow*eatTallowKof*tallowToEnergyKof <= energyNeed){
                        float tallowToEat = eatTallowKof*carryTallow;
                        carryTallow -= tallowToEat;
                        energy += tallowToEat*tallowToEnergyKof;
                    }else{
                        carryTallow -= (energyNeed/tallowToEnergyKof)/eatTallowKof;
                        energy += energyNeed;
                    }
                }else{
                    float tallowToEat = eatTallowKof*carryTallow;
                    carryTallow -= tallowToEat;
                    energy += tallowToEat*tallowToEnergyKof;
                    if(energy > energyMax){
                        energy = energyMax;
                    }
                }
                break;
            }
            case SHARESUGAR:{
                unsigned short lookX = x;
                unsigned short lookY = y;
                directionToXY(lookX,lookY);
                if( Game::singleGame().world[lookY][lookX]->childType == Cell::bot){
                    float sugarToShare = shareSugarKof*carrySugar;
                    carrySugar -= sugarToShare;
                    ((Bot*) Game::singleGame().world[lookY][lookX])->carrySugar += sugarToShare;
                }
                break;
            }
            case SHAREMINERALS:{
                unsigned short lookX = x;
                unsigned short lookY = y;
                directionToXY(lookX,lookY);
                if( Game::singleGame().world[lookY][lookX]->childType == Cell::bot){
                    float mineralsToShare = shareMineralsKof*carryMinerals;
                    carryMinerals -= mineralsToShare;
                    ((Bot*) Game::singleGame().world[lookY][lookX])->carryMinerals += mineralsToShare;
                }
                break;
            }
            case SHARETALLOW:{
                unsigned short lookX = x;
                unsigned short lookY = y;
                directionToXY(lookX,lookY);
                if( Game::singleGame().world[lookY][lookX]->childType == Cell::bot){
                    float tallowToShare = shareTallowKof*carryTallow;
                    carryTallow -= tallowToShare;
                    ((Bot*) Game::singleGame().world[lookY][lookX])->carryTallow += tallowToShare;
                }
                break;
            }
            case USESUGAR:{
                float sugarToLongLive = useSugarKof*carrySugar;
                carrySugar -= sugarToLongLive;
                longLiveSugar += sugarToLongLive;
                break;
            }
            case USEMINERALS:{
                float mineralsToDefence = useMineralsKof*carryMinerals;
                carryMinerals -= mineralsToDefence;
                defenceMinerals += mineralsToDefence;
                break;
            }
            case USETALLOW:{
                float tallowToHealth = useTallowKof*carryTallow;
                carryTallow -= tallowToHealth;
                health += tallowToHealth*tallowToHealthKof;
                if(health > healthMax){
                    float extraTallow = (health-healthMax)/tallowToHealthKof;
                    carryTallow += extraTallow;
                    health = healthMax;
                }
                break;
            }
            case SUICIDE:{
                health = -1;
                break;
            }
        }
        payForLive();
    }
}

void Bot::doMoveIntention()
{
    if(health > 0){
        unsigned short toMoveX = x;
        unsigned short toMoveY = y;
        directionToXY(toMoveX,toMoveY);
        unsigned short lastX = x;
        unsigned short lastY = y;
        Game::singleGame().world[toMoveY][toMoveX]->botToMoveOn = nullptr;
        if(Game::singleGame().world[toMoveY][toMoveX]->childType == Cell::empty){
            Empty* cell = (Empty*) Game::singleGame().world[toMoveY][toMoveX];

            carryMinerals += cell->minerals;
            cell->minerals = 0;
            cell->setCoords(lastX,lastY);
            this->setCoords(toMoveX,toMoveY);
            Game::singleGame().world[toMoveY][toMoveX] = this;
            Game::singleGame().world[lastY][lastX] = cell;
            if(cell->tracking){
                cell->tracking = false;
            }
        }else if(Game::singleGame().world[toMoveY][toMoveX]->childType == Cell::bot){
            Bot* otherBot = (Bot*)Game::singleGame().world[toMoveY][toMoveX];
            if(otherBot->health <= 0){

                if(Bot::rottingTallow && Bot::rottingDead){
                    float rotKof = Bot::rottingTurns-(otherBot->rottingTurnsCount);
                    if(rotKof > 0){
                        carryTallow += tallowFromDead/rotKof;
                    }
                }else{
                    carryTallow += tallowFromDead;
                }

                carryTallow += otherBot->carryTallow;
                carrySugar += otherBot->carrySugar;
                carryMinerals += otherBot->carryMinerals;
                if(otherBot->tracking){
                    otherBot->tracking = false;
                }
                Game::singleGame().botHell.append(otherBot);
                Empty* newEmpty = Game::singleGame().emptyHell.takeLast();
                this->setCoords(toMoveX,toMoveY);
                newEmpty->minerals = 0;
                newEmpty->botToMoveOn = nullptr;
                newEmpty->setCoords(lastX,lastY);
                newEmpty->recalculateProductivable();
                Game::singleGame().world[toMoveY][toMoveX] = this;
                Game::singleGame().world[lastY][lastX] = newEmpty;
            }
        }
        payForMove();
        payForLive();
    }
}

void Bot::doAttackIntention()
{
    if(health > 0){
        unsigned short toHitX = x;
        unsigned short toHitY = y;
        directionToXY(toHitX,toHitY);
        if(Game::singleGame().world[toHitY][toHitX]->childType == Cell::bot){
            Bot* otherBot = (Bot*)Game::singleGame().world[toHitY][toHitX];
            float healthBefore = otherBot->health;
            otherBot->health -= damage*(1.0-(defenceFromDefenceMinerals(otherBot->defenceMinerals)));
            if(healthBefore > 0 && otherBot->health <= 0){
                killCount++;
            }
            if(captureAttack){
                if(otherBot->health <= 0){
                    unsigned short lastX = this->x;
                    unsigned short lastY = this->y;

                    if(otherBot->tracking){
                        otherBot->tracking = false;
                    }

                    if(Bot::rottingTallow && Bot::rottingDead){
                        float rotKof = Bot::rottingTurns-(otherBot->rottingTurnsCount);
                        if(rotKof > 0){
                            carryTallow += tallowFromDead/rotKof;
                        }
                    }else{
                        carryTallow += tallowFromDead;
                    }

                    carryTallow += otherBot->carryTallow;
                    carrySugar += otherBot->carrySugar;
                    carryMinerals += otherBot->carryMinerals;

                    Game::singleGame().botHell.append(otherBot);
                    Empty* newEmpty = Game::singleGame().emptyHell.takeLast();
                    newEmpty->minerals = 0;
                    newEmpty->botToMoveOn = nullptr;
                    this->setCoords(toHitX,toHitY);
                    newEmpty->setCoords(lastX,lastY);
                    newEmpty->recalculateProductivable();
                    Game::singleGame().world[toHitY][toHitX] = this;
                    Game::singleGame().world[lastY][lastX] = newEmpty;
                }
            }
        }
        payForAttack();
        payForLive();
    }
}

void Bot::doCloneIntention()
{
    if(energy > minEnergyToClone){

        char freeDirectionsCounter = 0;

        unsigned short lookX = x;
        unsigned short lookY = y;
        for(uchar dir = 0; dir < 8; dir++){
            lookX = x;
            lookY = y;
            directionToXY((Directions)dir,lookX,lookY);
            if(Game::singleGame().world[lookY][lookX]->childType == Cell::empty){
                tempDirections[freeDirectionsCounter] = dir;
                freeDirectionsCounter++;
            }
        }

        if(freeDirectionsCounter > 0){

            uchar toCloneDir = rand()%freeDirectionsCounter;
            cloneCount++;
            lookX = x;
            lookY = y;
            directionToXY((Directions)tempDirections[toCloneDir],lookX,lookY);

            Game::singleGame().world[lookY][lookX]->tracking = false;

            Bot* newBot = Game::singleGame().botHell.takeLast();
            newBot->carryMinerals = 0;
            newBot->carryMinerals += ((Empty*)Game::singleGame().world[lookY][lookX])->minerals;

            Game::singleGame().emptyHell.append((Empty*)Game::singleGame().world[lookY][lookX]);
            Game::singleGame().world[lookY][lookX] = newBot;

            newBot->setCoords(lookX,lookY);

            newBot->intentionCommand = 0;
            newBot->genomIndex = 0;
            newBot->turnCount = 0;
            newBot->rottingTurnsCount = 0;

            newBot->health = 100;
            newBot->energy = startBotEnergy;

            newBot->carrySugar = 0;
            newBot->carryTallow = 0;

            newBot->defenceMinerals = 0;
            newBot->longLiveSugar = 0;
            newBot->direction = rand()%8;

            newBot->eatMineralsKof = eatMineralsKof;
            newBot->eatSugarKof = eatSugarKof;
            newBot->eatTallowKof = eatTallowKof;

            newBot->useMineralsKof = useMineralsKof;
            newBot->useSugarKof = useSugarKof;
            newBot->useTallowKof = useTallowKof;

            newBot->shareMineralsKof = shareMineralsKof;
            newBot->shareSugarKof = shareSugarKof;
            newBot->shareTallowKof = shareTallowKof;

            newBot->photoUser = photoUser;
            newBot->mineralsUser = mineralsUser;
            newBot->tallowUser = tallowUser;
            newBot->cloneCount = 0;
            newBot->killCount = 0;


            for(unsigned short i = 0; i < genomSize; i++){
                newBot->genom[i] = genom[i];
            }
            if(rand()/(float)RAND_MAX < genomMutationChance){
                uchar randomCommand = rand() % MOVEINDEX2;
                if(randomCommand == MOVEINDEX1){
                    randomCommand += rand()%50;
                }
                unsigned short randomPosition = rand()%genomSize;
                newBot->genom[randomPosition] = (GenomCommands)randomCommand;
            }
            newBot->calculateLifeStyle();
            if(rand()/(float)RAND_MAX < parametrsMutationChance){
                uchar randomParametr = rand()%9;
                float mutation = mutationSpeedOfParamets*(powf(-1,rand()%2));
                switch(randomParametr){
                    case 0:{
                        eatMineralsKof += mutation;
                        if(eatMineralsKof < 0) eatMineralsKof = 0;
                        if(eatMineralsKof > 1) eatMineralsKof = 1;
                        break;
                    }
                    case 1:{
                        eatSugarKof += mutation;
                        if(eatSugarKof < 0) eatSugarKof = 0;
                        if(eatSugarKof > 1) eatSugarKof = 1;
                        break;
                    }
                    case 2:{
                        eatTallowKof += mutation;
                        if(eatTallowKof < 0) eatTallowKof = 0;
                        if(eatTallowKof > 1) eatTallowKof = 1;
                        break;
                    }
                    case 3:{
                        useMineralsKof += mutation;
                        if(useMineralsKof < 0) useMineralsKof = 0;
                        if(useMineralsKof > 1) useMineralsKof = 1;
                        break;
                    }
                    case 4:{
                        useSugarKof += mutation;
                        if(useSugarKof < 0) useSugarKof = 0;
                        if(useSugarKof > 1) useSugarKof = 1;
                        break;
                    }
                    case 5:{
                        useTallowKof += mutation;
                        if(useTallowKof < 0) useTallowKof = 0;
                        if(useTallowKof > 1) useTallowKof = 1;
                        break;
                    }
                    case 6:{
                        shareMineralsKof += mutation;
                        if(shareMineralsKof < 0) shareMineralsKof = 0;
                        if(shareMineralsKof > 1) shareMineralsKof = 1;
                        break;
                    }
                    case 7:{
                        shareSugarKof += mutation;
                        if(shareSugarKof < 0) shareSugarKof = 0;
                        if(shareSugarKof > 1) shareSugarKof = 1;
                        break;
                    }
                    case 8:{
                        shareTallowKof += mutation;
                        if(shareTallowKof < 0) shareTallowKof = 0;
                        if(shareTallowKof > 1) shareTallowKof = 1;
                        break;
                    }
                    default:;
                }
            }   
        }
        payForClone();
    }
    payForLive();
}

void Bot::doRot()
{
    if(health <= 0){
        rottingTurnsCount++;
        if(rottingTurnsCount >= Bot::rottingTurns){
            Game::singleGame().botHell.append(this);
            this->tracking = false;
            Empty* newEmpty = Game::singleGame().emptyHell.takeLast();
            newEmpty->minerals = 0;
            newEmpty->botToMoveOn = nullptr;
            newEmpty->setCoords(x,y);
            newEmpty->recalculateProductivable();
            Game::singleGame().world[y][x] = newEmpty;
        }
    }
}

void Bot::directionToXY(unsigned short &x, unsigned short &y)
{
    if(direction == RIGHT || direction == UPRIGHT || direction == DOWNRIGHT){
        x = this->x+1;
    }else if(direction == LEFT || direction == UPLEFT || direction == DOWNLEFT){
        x = this->x-1;
    }

    if(direction == UP || direction == UPRIGHT || direction == UPLEFT){
        y = this->y+1;
    }else if(direction == DOWN || direction == DOWNRIGHT || direction == DOWNLEFT){
        y = this->y-1;
    }
    if(x == Game::worldWidth) x = 0; else
    if(x == 65535) x = Game::worldWidth-1;

    if(y == Game::worldHeight) y = 0; else
    if(y == 65535) y = Game::worldHeight-1;
}

void Bot::directionToXY(Bot::Directions dir, unsigned short &x, unsigned short &y)
{
    if(dir == RIGHT || dir == UPRIGHT || dir == DOWNRIGHT){
        x = this->x+1;
    }else if(dir == LEFT || dir == UPLEFT || dir == DOWNLEFT){
        x = this->x-1;
    }

    if(dir == UP || dir == UPRIGHT || dir == UPLEFT){
        y = this->y+1;
    }else if(dir == DOWN || dir == DOWNRIGHT || dir == DOWNLEFT){
        y = this->y-1;
    }
    if(x == Game::worldWidth) x = 0; else
    if(x == 65535) x = Game::worldWidth-1;

    if(y == Game::worldHeight) y = 0; else
    if(y == 65535) y = Game::worldHeight-1;
}

uchar Bot::genomDifferenceFrom(Bot *otherBot)
{
    uchar diff = 0;
    for(unsigned short i = 0; i < genomSize; i++){
        if(this->genom[i] !=  otherBot->genom[i]){
            diff++;
        }
    }
    return diff;
}

void Bot::payForLive()
{
    if(health > 0){
        defenceMinerals -= defenceCoolDown;
        if(defenceMinerals < 0){
            defenceMinerals = 0;
        }

        longLiveSugar -= longLiveCoolDown;
        if(longLiveSugar < 0){
            longLiveSugar = 0;
        }

        energy -= everyTurnCost;
        turnCount++;
        if(turnCount >= botsliveTime){
            energy -= oldCost*(turnCount-botsliveTime)*(1-(longLiveValueFromSugar(longLiveSugar)));
        }
        if(energy <= 0){
            health += (energy*(1-(longLiveValueFromSugar(longLiveSugar))));
        }
    }
}

void Bot::payForMove()
{
    energy -= moveCost;
}

void Bot::payForAttack()
{
    energy -= attackCost;
}

void Bot::payForClone()
{
    energy -= cloneCost;
}

void Bot::payForPhoto()
{
    energy -= photoCost;
}

void Bot::calculateLifeStyle()
{
    //-------
    mineralsUser = 0;
    photoUser = 0;
    tallowUser = 0;
    for(unsigned short i = 0; i < genomSize; i++){
        switch(genom[i]){
            case EATMINERALS:
            case GO:{
                mineralsUser++;
                break;
            }
            case EATSUGAR:
            case PHOTO:{
                photoUser++;
                break;
            }
            case EATTALLOW:
            case ATTACK:{
                tallowUser++;
                break;
            }
            default:;
        }
    }
}
