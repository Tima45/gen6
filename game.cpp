#include "game.h"


Game* Game::singleGame = nullptr;

uchar Game::turnsToUpdateInSkipMode = 64;
const unsigned short Game::worldWidth = 512;
const unsigned short Game::worldHeight = 512;

Game::Game(QObject *parent) : QObject(parent)
{
    /*
    botHell.reserve(worldWidth*worldHeight);
    emptyHell.reserve(worldWidth*worldHeight);
    */

    if(singleGame == nullptr){
        singleGame = this;
    }

    botsToDoSimpleThings = new Bot*[worldWidth*worldHeight];
    botsToMove = new Bot*[worldWidth*worldHeight];
    botsToAttack = new Bot*[worldWidth*worldHeight];
    botsToClone = new Bot*[worldWidth*worldHeight];

    for(int i = 0; i < worldWidth*worldHeight; i++){
        botHell.append(new Bot(0,0));
        emptyHell.append(new Empty(0,0));
    }
    world = new Cell**[worldHeight];
    for(unsigned short y = 0; y < worldHeight; y++){
        world[y] = new Cell*[worldWidth];
        for(unsigned short x = 0; x < worldWidth; x++){
            Empty *cell = emptyHell.takeLast();
            cell->minerals = 0;
            cell->setCoords(x,y);
            world[y][x] = cell;
        }
    }
    displayMode = Style;
}

Game::~Game()
{
    delete[] botsToDoSimpleThings;
    delete[] botsToMove;
    delete[] botsToAttack;
    delete[] botsToClone;
    singleGame = nullptr;
}

void Game::turn()
{

    locker.lockForWrite();
    inTurn = true;
    locker.unlock();

    aliveBotsCount = 0;
    deadBotsCount = 0;

    botsToDoSimpleThingsCount = 0;
    botsToMoveCount = 0;
    botsToAttackCount = 0;
    botsToCloneCount = 0;
    //-------------------------------------------
    for(unsigned short y = 0; y < worldHeight; y++){
        for(unsigned short x = 0; x < worldWidth;x++){
            if(world[y][x]->childType == Cell::empty){
                Empty *cell = (Empty*)world[y][x];
                cell->mineralsGrowUp();
            }else if(world[y][x]->childType == Cell::bot){
                Bot* bot = (Bot*)world[y][x];
                if(bot->health > 0){
                    aliveBotsCount++;
                    bot->readNextCommand();
                    if((bot->intentionCommand != Bot::GO) && (bot->intentionCommand != Bot::ATTACK) && (bot->intentionCommand != Bot::CLONE)){
                        botsToDoSimpleThings[botsToDoSimpleThingsCount] = bot;
                        botsToDoSimpleThingsCount++;
                    }else{
                        if(bot->intentionCommand == Bot::GO){
                            botsToMove[botsToMoveCount] = bot;
                            botsToMoveCount++;
                        }else if(bot->intentionCommand == Bot::ATTACK){
                            botsToAttack[botsToAttackCount] = bot;
                            botsToAttackCount++;
                        }else{
                            botsToClone[botsToCloneCount] = bot;
                            botsToCloneCount++;
                        }
                    }
                }else{
                    deadBotsCount++;
                }
            }
        }
    }
    for(uint i = 0; i < botsToDoSimpleThingsCount; i++){
        botsToDoSimpleThings[i]->doSimpleIntention();
    }
    for(uint i = 0; i < botsToMoveCount; i++){
        botsToMove[i]->doMoveIntention();
    }
    for(uint i = 0; i < botsToAttackCount; i++){
        botsToAttack[i]->doAttackIntention();
    }
    for(uint i = 0; i < botsToCloneCount; i++){
        botsToClone[i]->doCloneIntention();
    }
/*
    for(unsigned short y = 0; y < worldHeight; y++){
        for(unsigned short x = 0; x < worldWidth;x++){
            world[y][x]->botToMoveOn = nullptr;
        }
    }*/
    //-------------------------------------------
    locker.lockForWrite();
    currentTurn++;
    locker.unlock();

    locker.lockForRead();
    if(skipDisplay){
        locker.unlock();
        if(currentTurn%turnsToUpdateInSkipMode == 0){
            emit updateLabels(currentTurn,aliveBotsCount,deadBotsCount);
        }
    }else{
        locker.unlock();
        emit updateLabels(currentTurn,aliveBotsCount,deadBotsCount);
        drawWorld();
        emit emitReplotWorld(QCustomPlot::rpQueuedReplot);
    }
    locker.lockForWrite();
    inTurn = false;
    locker.unlock();
}


void Game::resetWorld()
{
    locker.lockForRead();
    if(!inTurn && !isPlaying){
        locker.unlock();
        currentTurn = 0;
        for(unsigned short y = 0; y < worldHeight; y++){
            for(unsigned short x = 0; x < worldWidth; x++){
                if(world[y][x]->childType == Cell::empty){
                    Empty *cell = (Empty*)world[y][x];
                    cell->minerals = 0;
                    cell->setCoords(x,y);
                    cell->recalculateProductivable();
                }else if(world[y][x]->childType == Cell::bot){
                    botHell.append((Bot*)world[y][x]);

                    Empty *cell = emptyHell.takeLast();
                    cell->minerals = 0;
                    cell->setCoords(x,y);
                    world[y][x] = cell;
                }
            }
        }


        if(true){
            Bot* firstBot = botHell.takeLast();
            for(unsigned short i = 0; i < Bot::genomSize; i++){
                firstBot->genom[i] = Bot::STAND;
            }

            for(unsigned short i = 0; i < Bot::genomSize; i+=3){
                firstBot->genom[i] = Bot::PHOTO;
                firstBot->genom[i+1] = Bot::EATSUGAR;
            }




            firstBot->genomIndex = 0;
            firstBot->turnCount = 0;

            firstBot->health = 100;
            firstBot->energy = 80;

            firstBot->carryMinerals = 0;
            firstBot->carrySugar = 0;
            firstBot->carryTallow = 0;

            firstBot->defenceMinerals = 0;
            firstBot->longLiveSugar = 0;
            firstBot->direction = rand()%8;

            firstBot->eatMineralsKof = 1;
            firstBot->eatSugarKof = 1;
            firstBot->eatTallowKof = 1;

            firstBot->useMineralsKof = 1;
            firstBot->useSugarKof = 1;
            firstBot->useTallowKof = 1;

            firstBot->shareMineralsKof = 1;
            firstBot->shareSugarKof = 1;
            firstBot->shareTallowKof = 1;


            firstBot->photoUser = 0;
            firstBot->mineralsUser = 0;
            firstBot->tallowUser = 0;
            firstBot->cloneCount = 0;
            firstBot->calculateLifeStyle();

            firstBot->setCoords(worldHeight/2,worldWidth/2);
            emptyHell.append((Empty*)Game::world[worldWidth/2][worldHeight/2]);
            Game::world[worldWidth/2][worldHeight/2] = firstBot;
        }




        /*{
            Bot* firstBot = botHell.takeLast();
            for(unsigned short i = 0; i < Bot::genomSize; i++){
                firstBot->genom[i] = Bot::GO;
            }

            for(unsigned short i = 0; i < Bot::genomSize; i+=3){
                firstBot->genom[i] = Bot::EATMINERALS;
            }



            firstBot->genomIndex = 0;
            firstBot->turnCount = 0;

            firstBot->health = 100;
            firstBot->energy = 80;

            firstBot->carryMinerals = 0;
            firstBot->carrySugar = 0;
            firstBot->carryTallow = 0;

            firstBot->defenceMinerals = 0;
            firstBot->longLiveSugar = 0;
            firstBot->direction = 0;

            firstBot->eatMineralsKof = 1;
            firstBot->eatSugarKof = 1;
            firstBot->eatTallowKof = 1;

            firstBot->useMineralsKof = 1;
            firstBot->useSugarKof = 1;
            firstBot->useTallowKof = 1;

            firstBot->shareMineralsKof = 1;
            firstBot->shareSugarKof = 1;
            firstBot->shareTallowKof = 1;

            firstBot->genomDifference = 0;

            firstBot->photoUser = 0;
            firstBot->mineralsUser = 0;
            firstBot->tallowUser = 0;
            firstBot->calculateLifeStyle();

            firstBot->setCoords(400,400);
            emptyHell.append((Empty*)Game::world[400][400]);
            Game::world[400][400] = firstBot;
        }*/

    }else{
        locker.unlock();
    }
}

void Game::drawWorld()
{
    for(unsigned short y = 0; y < worldHeight; y++){
        for(unsigned short x = 0; x < worldWidth;x++){
            if(world[y][x]->childType == Cell::empty){
                Empty *cell = (Empty*)world[y][x];
                if(cell->minerals < Empty::mineralsMax){
                    colorMap->data()->setCell(x,y, (cell->minerals/Empty::mineralsMax)*(DoubleColors::MineralsMaxColor - DoubleColors::MineralsMinColor) + DoubleColors::MineralsMinColor);
                }else{
                    colorMap->data()->setCell(x,y,DoubleColors::MineralsMaxColor);
                }
            } else if(world[y][x]->childType == Cell::bot){
                Bot *bot = (Bot*)world[y][x];
                if(bot->health <= 0){
                    colorMap->data()->setCell(x,y,DoubleColors::DeadBotColor);
                }else{
                    locker.lockForRead();
                    switch (displayMode) {
                        case Style:{
                            if((bot->photoUser == bot->mineralsUser) && (bot->photoUser == bot->tallowUser)){
                                colorMap->data()->setCell(x,y,DoubleColors::StandingColor);
                            }else{
                                if((bot->photoUser >= bot->mineralsUser) && (bot->photoUser >=  bot->tallowUser)){
                                    colorMap->data()->setCell(x,y,(bot->photoUser/(bot->photoUser+bot->mineralsUser+bot->tallowUser))*(DoubleColors::PhotoUserMaxColor - DoubleColors::PhotoUserMinColor) + DoubleColors::PhotoUserMinColor);
                                }
                                if((bot->mineralsUser >= bot->photoUser) && (bot->mineralsUser >=  bot->tallowUser)){
                                    colorMap->data()->setCell(x,y,(bot->mineralsUser/(bot->photoUser+bot->mineralsUser+bot->tallowUser))*(DoubleColors::MineralsUserMaxColor - DoubleColors::MineralsUserMinColor) + DoubleColors::MineralsUserMinColor);
                                }
                                if((bot->tallowUser >= bot->photoUser) && (bot->tallowUser >=  bot->photoUser)){
                                    colorMap->data()->setCell(x,y,(bot->tallowUser/(bot->photoUser+bot->mineralsUser+bot->tallowUser))*(DoubleColors::TallowUserMaxColor - DoubleColors::TallowUserMinColor) + DoubleColors::TallowUserMinColor);
                                }
                            }
                            break;
                        }
                        case Energy:{
                            if(bot->energy > 0){
                                colorMap->data()->setCell(x,y,(bot->energy/Bot::energyMax)*(DoubleColors::BotEnergyMaxColor - DoubleColors::BotEnergyMinColor) + DoubleColors::BotEnergyMinColor);
                            }else{
                                colorMap->data()->setCell(x,y,DoubleColors::BotEnergyMinColor);
                            }
                            break;
                        }
                        case KillCount:{
                            if(bot->killCount == 0){
                                colorMap->data()->setCell(x,y,DoubleColors::NoKillColor);
                            } else if(bot->killCount < 20){
                                colorMap->data()->setCell(x,y,(bot->killCount/20.0)*(DoubleColors::KillMaxColor - DoubleColors::KillMinColor) + DoubleColors::KillMinColor);
                            }else{
                                colorMap->data()->setCell(x,y,DoubleColors::KillMaxColor);
                            }
                            break;
                        }
                        case CloneCount:{
                            if(bot->cloneCount < 10){
                                colorMap->data()->setCell(x,y,(bot->cloneCount/10.0)*(DoubleColors::CloneMaxColor - DoubleColors::CloneMinColor) + DoubleColors::CloneMinColor);
                            }else{
                                colorMap->data()->setCell(x,y,DoubleColors::CloneMaxColor);
                            }
                            break;
                        }
                        case DefenceAmount:{
                             colorMap->data()->setCell(x,y,(Bot::defenceFromDefenceMinerals(bot->defenceMinerals))*(DoubleColors::DefenceMaxColor - DoubleColors::DefenceMinColor) + DoubleColors::DefenceMinColor);
                            break;
                        }
                        case LongLiveAmount:{
                             colorMap->data()->setCell(x,y,(Bot::longLiveValueFromSugar(bot->longLiveSugar))*(DoubleColors::LongLiveMaxColor - DoubleColors::LongLiveMinColor) + DoubleColors::LongLiveMinColor);
                            break;
                        }
                        case SugarAmount:{
                            if(bot->carrySugar < 100){
                                colorMap->data()->setCell(x,y,(bot->carrySugar/100.0)*(DoubleColors::AmountMaxColor - DoubleColors::AmountMinColor) + DoubleColors::AmountMinColor);
                            }else{
                                colorMap->data()->setCell(x,y,DoubleColors::AmountMaxColor);
                            }
                            break;
                        }
                        case MineralsAmount:{
                            if(bot->carryMinerals < 100){
                                colorMap->data()->setCell(x,y,(bot->carryMinerals/100.0)*(DoubleColors::AmountMaxColor - DoubleColors::AmountMinColor) + DoubleColors::AmountMinColor);
                            }else{
                                colorMap->data()->setCell(x,y,DoubleColors::AmountMaxColor);
                            }
                            break;
                        }
                        case TallowAmount:{
                            if(bot->carryTallow < 100){
                                colorMap->data()->setCell(x,y,(bot->carryTallow/100.0)*(DoubleColors::AmountMaxColor - DoubleColors::AmountMinColor) + DoubleColors::AmountMinColor);
                            }else{
                                colorMap->data()->setCell(x,y,DoubleColors::AmountMaxColor);
                            }
                            break;
                        }
                    }
                    locker.unlock();
                }
            }
        }
    }
}

void Game::infinitGamePlaying()
{
    locker.lockForRead();
    while(isPlaying){
        locker.unlock();
        turn();
        locker.lockForRead();
    }
    locker.unlock();
}

void Game::playOneTurn()
{
    isPlaying = true;
    turn();
    isPlaying = false;
}

void Game::recalculateMineralsProductivable()
{
    for(unsigned short y = 0; y < worldHeight; y++){
        for(unsigned short x = 0; x < worldWidth;x++){
            if(world[y][x]->childType == Cell::empty){
                Empty *cell = (Empty*)world[y][x];
                cell->recalculateProductivable();
            }
        }
    }
}

void Game::saveWorld(QDataStream &str)
{
    str << Bot::tolerance;
    str << Bot::photoSugarMax;
    str << Bot::photoSugarMin;
    str << Bot::photoSugarBorder;
    str << Bot::sugarToEnergyKof;
    str << Bot::mineralsToEnergyKof;
    str << Bot::tallowToEnergyKof;
    str << Bot::tallowToHealthKof;
    str << Bot::everyTurnCost;
    str << Bot::botsliveTime;
    str << Bot::oldCost;
    str << Bot::defenceCoolDown;
    str << Bot::longLiveCoolDown;
    str << Bot::healthMax;
    str << Bot::tallowFromDead;
    str << Bot::moveCost;
    str << Bot::attackCost;
    str << Bot::photoCost;
    str << Bot::damage;
    str << Bot::minEnergyToClone;
    str << Bot::cloneCost;
    str << Bot::startBotEnergy;
    str << Bot::genomMutationChance;
    str << Bot::parametrsMutationChance;
    str << Bot::mutationSpeedOfParamets;
    str << Bot::captureAttack;
    str << Bot::effectiveEat;
    str << Bot::defenceKof;
    str << Bot::longLiveKof;
    str << Empty::mineralsMax;
    str << Empty::mineralsMin;
    str << Empty::mineralsGrowSpeedMax;
    str << Empty::mineralsGrowSpeedMin;
    str << Empty::produciveProbabilityMax;
    str << Empty::produciveProbabilityMin;
    str << Empty::mineralsGrowBorder;

    str << currentTurn;
    str << turnsPerSecond;
    str << aliveBotsCount;
    str << deadBotsCount;

    QVector<Bot*> tmpBots;
    QVector<Empty*> tmpEmpty;
    for(unsigned short y = 0; y < worldHeight; y++){
        for(unsigned short x = 0; x < worldWidth;x++){
            if(world[y][x]->childType == Cell::empty){
                tmpEmpty.append((Empty*)world[y][x]);
            }else if(world[y][x]->childType == Cell::bot){
                tmpBots.append((Bot*)world[y][x]);
            }
        }
    }
    str << tmpBots.count();
    for(unsigned int i = 0; i < tmpBots.count(); i++){
        Bot* bot = tmpBots.at(i);
        for(unsigned short j = 0; j < Bot::genomSize; j++){
            str << bot->genom[j];
        }
        str << bot->x;
        str << bot->y;
        str << bot->genomIndex;
        str << bot->turnCount;
        str << bot->health;
        str << bot->energy;
        str << bot->carryMinerals;
        str << bot->carrySugar;
        str << bot->carryTallow;
        str << bot->defenceMinerals;
        str << bot->longLiveSugar;
        str << bot->direction;
        str << bot->eatMineralsKof;
        str << bot->eatSugarKof;
        str << bot->eatTallowKof;
        str << bot->useMineralsKof;
        str << bot->useSugarKof;
        str << bot->useTallowKof;
        str << bot->shareMineralsKof;
        str << bot->shareSugarKof;
        str << bot->shareTallowKof;
        str << bot->cloneCount;
        str << bot->killCount;
    }
    str << tmpEmpty.count();
    for(unsigned int i = 0; i < tmpEmpty.count(); i++){
        Empty *cell = tmpEmpty.at(i);
        str << cell->x;
        str << cell->y;
        str << cell->producive;
        str << cell->minerals;
        str << cell->localMineralsMax;
        str << cell->mineralsGrowSpeed;
    }
}

void Game::loadWorld(QDataStream &str)
{
    for(unsigned short y = 0; y < worldHeight; y++){
        for(unsigned short x = 0; x < worldWidth;x++){
            if(world[y][x]->childType == Cell::empty){
                emptyHell.append((Empty*)world[y][x]);
            }else if(world[y][x]->childType == Cell::bot){
                botHell.append((Bot*)world[y][x]);
            }
        }
    }

    str >> Bot::tolerance;
    str >> Bot::photoSugarMax;
    str >> Bot::photoSugarMin;
    str >> Bot::photoSugarBorder;
    str >> Bot::sugarToEnergyKof;
    str >> Bot::mineralsToEnergyKof;
    str >> Bot::tallowToEnergyKof;
    str >> Bot::tallowToHealthKof;
    str >> Bot::everyTurnCost;
    str >> Bot::botsliveTime;
    str >> Bot::oldCost;
    str >> Bot::defenceCoolDown;
    str >> Bot::longLiveCoolDown;
    str >> Bot::healthMax;
    str >> Bot::tallowFromDead;
    str >> Bot::moveCost;
    str >> Bot::attackCost;
    str >> Bot::photoCost;
    str >> Bot::damage;
    str >> Bot::minEnergyToClone;
    str >> Bot::cloneCost;
    str >> Bot::startBotEnergy;
    str >> Bot::genomMutationChance;
    str >> Bot::parametrsMutationChance;
    str >> Bot::mutationSpeedOfParamets;
    str >> Bot::captureAttack;
    str >> Bot::effectiveEat;
    str >> Bot::defenceKof;
    str >> Bot::longLiveKof;
    str >> Empty::mineralsMax;
    str >> Empty::mineralsMin;
    str >> Empty::mineralsGrowSpeedMax;
    str >> Empty::mineralsGrowSpeedMin;
    str >> Empty::produciveProbabilityMax;
    str >> Empty::produciveProbabilityMin;
    str >> Empty::mineralsGrowBorder;

    str >> currentTurn;
    str >> turnsPerSecond;
    str >> aliveBotsCount;
    str >> deadBotsCount;

    int botsCount = 0;
    str >> botsCount;

    for(unsigned int i = 0; i < botsCount; i++){
        Bot* bot = botHell.takeLast();
        for(unsigned short j = 0; j < Bot::genomSize; j++){
            str >> bot->genom[j];
        }
        bot->calculateLifeStyle();
        str >> bot->x;
        str >> bot->y;
        str >> bot->genomIndex;
        str >> bot->turnCount;
        str >> bot->health;
        str >> bot->energy;
        str >> bot->carryMinerals;
        str >> bot->carrySugar;
        str >> bot->carryTallow;
        str >> bot->defenceMinerals;
        str >> bot->longLiveSugar;
        str >> bot->direction;
        str >> bot->eatMineralsKof;
        str >> bot->eatSugarKof;
        str >> bot->eatTallowKof;
        str >> bot->useMineralsKof;
        str >> bot->useSugarKof;
        str >> bot->useTallowKof;
        str >> bot->shareMineralsKof;
        str >> bot->shareSugarKof;
        str >> bot->shareTallowKof;
        str >> bot->cloneCount;
        str >> bot->killCount;
        world[bot->y][bot->x] = bot;
    }

    int emptyCount = 0;
    str >> emptyCount;

    for(unsigned int i = 0; i < emptyCount; i++){
        Empty *cell = emptyHell.takeLast();
        str >> cell->x;
        str >> cell->y;
        str >> cell->producive;
        str >> cell->minerals;
        str >> cell->localMineralsMax;
        str >> cell->mineralsGrowSpeed;
        world[cell->y][cell->x] = cell;
    }
}
