#include "worldparametersdialog.h"
#include "ui_worldparametersdialog.h"

WorldParametersDialog::WorldParametersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorldParametersDialog)
{
    ui->setupUi(this);
}

WorldParametersDialog::~WorldParametersDialog()
{
    delete ui;
}

void WorldParametersDialog::loadParametres()
{
    ui->toleranceBox->setValue(Bot::tolerance);
    ui->photoSugarMaxBox->setValue(Bot::photoSugarMax);
    ui->photoSugarMinBox->setValue(Bot::photoSugarMin);
    ui->photoSugarBorderBox->setValue(Bot::photoSugarBorder);
    ui->sugarToEnergyKofBox->setValue(Bot::sugarToEnergyKof);
    ui->mineralsToEnergyKofBox->setValue(Bot::mineralsToEnergyKof);
    ui->tallowToEnergyKofBox->setValue(Bot::tallowToEnergyKof);
    ui->tallowToHealthKofBox->setValue(Bot::tallowToHealthKof);
    ui->everyTurnCostBox->setValue(Bot::everyTurnCost);
    ui->botsliveTimeBox->setValue(Bot::botsliveTime);
    ui->oldCostBox->setValue(Bot::oldCost);
    ui->defenceCoolDownBox->setValue(Bot::defenceCoolDown);
    ui->longLiveCoolDownBox->setValue(Bot::longLiveCoolDown);
    ui->healthMaxBox->setValue(Bot::healthMax);
    ui->tallowFromDeadBox->setValue(Bot::tallowFromDead);
    ui->moveCostBox->setValue(Bot::moveCost);
    ui->attackCostBox->setValue(Bot::attackCost);
    ui->photoCostBox->setValue(Bot::photoCost);
    ui->damageBox->setValue(Bot::damage);
    ui->minEnergyToCloneBox->setValue(Bot::minEnergyToClone);
    ui->cloneCostBox->setValue(Bot::cloneCost);
    ui->startBotEnergyBox->setValue(Bot::startBotEnergy);
    ui->genomMutationChanceBox->setValue(Bot::genomMutationChance);
    ui->parametrsMutationChanceBox->setValue(Bot::parametrsMutationChance);
    ui->mutationSpeedOfParametsBox->setValue(Bot::mutationSpeedOfParamets);
    ui->mineralsMaxBox->setValue(Empty::mineralsMax);
    ui->mineralsMinBox->setValue(Empty::mineralsMin);
    ui->mineralsGrowSpeedMaxBox->setValue(Empty::mineralsGrowSpeedMax);
    ui->mineralsGrowSpeedMinBox->setValue(Empty::mineralsGrowSpeedMin);
    ui->produciveProbabilityMaxBox->setValue(Empty::produciveProbabilityMax);
    ui->produciveProbabilityMinBox->setValue(Empty::produciveProbabilityMin);
    ui->mineralsGrowBorderBox->setValue(Empty::mineralsGrowBorder);

    ui->effectiveEatBox->setChecked(Bot::effectiveEat);
    ui->captureAttackBox->setChecked(Bot::captureAttack);

    ui->defenceKofBox->setValue(Bot::defenceKof);
    ui->longLiveKofBox->setValue(Bot::longLiveKof);


    ui->rottingTallowBox->setChecked(Bot::rottingTallow);
    ui->rottingTurnsBox->setValue(qRound(Bot::rottingTurns));

    ui->rottingDeadBox->setChecked(Bot::rottingDead);

    on_rottingDeadBox_toggled(Bot::rottingDead);

    ui->worldSizeBox->setValue(Game::worldWidth);

}

void WorldParametersDialog::on_buttonBox_accepted()
{
    Bot::tolerance = ui->toleranceBox->value();
    Bot::photoSugarMax = ui->photoSugarMaxBox->value();
    Bot::photoSugarMin = ui->photoSugarMinBox->value();
    Bot::photoSugarBorder = ui->photoSugarBorderBox->value();
    Bot::sugarToEnergyKof = ui->sugarToEnergyKofBox->value();
    Bot::mineralsToEnergyKof = ui->mineralsToEnergyKofBox->value();
    Bot::tallowToEnergyKof = ui->tallowToEnergyKofBox->value();
    Bot::tallowToHealthKof = ui->tallowToHealthKofBox->value();
    Bot::everyTurnCost = ui->everyTurnCostBox->value();
    Bot::botsliveTime = ui->botsliveTimeBox->value();
    Bot::oldCost = ui->oldCostBox->value();
    Bot::defenceCoolDown = ui->defenceCoolDownBox->value();
    Bot::longLiveCoolDown = ui->longLiveCoolDownBox->value();
    Bot::healthMax = ui->healthMaxBox->value();
    Bot::tallowFromDead = ui->tallowFromDeadBox->value();
    Bot::moveCost = ui->moveCostBox->value();
    Bot::attackCost = ui->attackCostBox->value();
    Bot::photoCost = ui->photoCostBox->value();
    Bot::damage = ui->damageBox->value();
    Bot::minEnergyToClone = ui->minEnergyToCloneBox->value();
    Bot::cloneCost = ui->cloneCostBox->value();
    Bot::startBotEnergy = ui->startBotEnergyBox->value();
    Bot::genomMutationChance = ui->genomMutationChanceBox->value();
    Bot::parametrsMutationChance = ui->parametrsMutationChanceBox->value();
    Bot::mutationSpeedOfParamets = ui->mutationSpeedOfParametsBox->value();


    if(Empty::mineralsMax != ui->mineralsMaxBox->value()||
       Empty::mineralsMin != ui->mineralsMinBox->value()||
       Empty::mineralsGrowSpeedMax != ui->mineralsGrowSpeedMaxBox->value()||
       Empty::mineralsGrowSpeedMin != ui->mineralsGrowSpeedMinBox->value()||
       Empty::produciveProbabilityMax != ui->produciveProbabilityMaxBox->value()||
       Empty::produciveProbabilityMin != ui->produciveProbabilityMinBox->value()||
       Empty::mineralsGrowBorder != ui->mineralsGrowBorderBox->value())
    {
        Empty::mineralsMax = ui->mineralsMaxBox->value();
        Empty::mineralsMin = ui->mineralsMinBox->value();
        Empty::mineralsGrowSpeedMax = ui->mineralsGrowSpeedMaxBox->value();
        Empty::mineralsGrowSpeedMin = ui->mineralsGrowSpeedMinBox->value();
        Empty::produciveProbabilityMax = ui->produciveProbabilityMaxBox->value();
        Empty::produciveProbabilityMin = ui->produciveProbabilityMinBox->value();
        Empty::mineralsGrowBorder = ui->mineralsGrowBorderBox->value();
        Game::singleGame().recalculateMinerals();
    }


    Bot::effectiveEat = ui->effectiveEatBox->isChecked();
    Bot::captureAttack = ui->captureAttackBox->isChecked();
    Bot::defenceKof = ui->defenceKofBox->value();
    Bot::longLiveKof = ui->longLiveKofBox->value();

    Bot::rottingTallow = ui->rottingTallowBox->isChecked();
    Bot::rottingTurns = ui->rottingTurnsBox->value();
    Bot::rottingDead = ui->rottingDeadBox->isChecked();

    if(Game::worldWidth != ui->worldSizeBox->value()){
        if(QMessageBox::question(this,"Внимание","Изменение размера мира влечет удаление текущего мира") == QMessageBox::Yes){
            Game::worldWidth = ui->worldSizeBox->value();
            Game::worldHeight = Game::worldWidth;
            Game::singleGame().resetWorld();
            emit changeWorldSize(Game::worldWidth);
        }
    }
}


void WorldParametersDialog::on_rottingDeadBox_toggled(bool checked)
{
    ui->label_29->setEnabled(checked);
    ui->rottingTurnsBox->setEnabled(checked);
    ui->rottingTallowBox->setEnabled(checked);
    if(!checked){
        ui->rottingTallowBox->setChecked(false);
    }
}

void WorldParametersDialog::on_startBotEnergyBox_valueChanged(double arg1)
{
    ui->everyTurnCostBox->setMaximum(100-arg1);
}
