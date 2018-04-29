#include "worldparametersdialog.h"
#include "ui_worldparametersdialog.h"

WorldParametersDialog::WorldParametersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorldParametersDialog)
{
    ui->setupUi(this);
    ui->toleranceBox->setValue(Bot::tolerance);
    ui->photoSugarMaxBox->setValue(Bot::photoSugarMax);
    ui->photoSugarMinBox->setValue(Bot::photoSugarMin);
    ui->photoSugarBorderBox->setValue(Bot::photoSugarBorder);
    ui->sugarToEnergyKofBox->setValue(Bot::sugarToEnergyKof);
    ui->mineralsToEnergyKofBox->setValue(Bot::mineralsToEnergyKof);
    ui->tallowToEnergyKofBox->setValue(Bot::tallowToEnergyKof);
    ui->tallowToHealthKofBox->setValue(Bot::tallowToHealthKof);
    ui->everyTernCostBox->setValue(Bot::everyTernCost);
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


}

WorldParametersDialog::~WorldParametersDialog()
{
    delete ui;
}

void WorldParametersDialog::on_buttonBox_accepted()
{
    Bot::tolerance = ui->toleranceBox->value();
    Bot::photoSugarMax = ui->photoSugarMaxBox->value()           ;
    Bot::photoSugarMin = ui->photoSugarMinBox->value();
    Bot::photoSugarBorder = ui->photoSugarBorderBox->value();
    Bot::sugarToEnergyKof = ui->sugarToEnergyKofBox->value();
    Bot::mineralsToEnergyKof = ui->mineralsToEnergyKofBox->value();
    Bot::tallowToEnergyKof = ui->tallowToEnergyKofBox->value();
    Bot::tallowToHealthKof = ui->tallowToHealthKofBox->value();
    Bot::everyTernCost = ui->everyTernCostBox->value();
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
    Empty::mineralsMax = ui->mineralsMaxBox->value();
    Empty::mineralsMin = ui->mineralsMinBox->value();
    Empty::mineralsGrowSpeedMax = ui->mineralsGrowSpeedMaxBox->value();
    Empty::mineralsGrowSpeedMin = ui->mineralsGrowSpeedMinBox->value();
    Empty::produciveProbabilityMax = ui->produciveProbabilityMaxBox->value();
    Empty::produciveProbabilityMin = ui->produciveProbabilityMinBox->value();
    Empty::mineralsGrowBorder = ui->mineralsGrowBorderBox->value();

}
