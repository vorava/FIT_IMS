// IMS projekt
// Vojtech Orava (xorava02)
#include <iostream>
#include "simlib.h"
#include <string>
#include <math.h>
#include "arg_parser.h"

using namespace std;

/**** STREDOVEK ***/
// Pesak
#define INFANTRY_POWER 22
#define INFANTRY_HP 150
#define INFANTRY_TR 10
// Rytir
#define KNIGHT_POWER 105
#define KNIGHT_HP 380
#define KNIGHT_TR 15
// Jezdec
#define CATAPULT_POWER 150
#define CATAPULT_HP 400
#define CATAPULT_TR 20

/***** SOUCASNOST ***/
// Vojak
#define SOLDIER_POWER 60
#define SOLDIER_HP 300
#define SOLDIER_TR 15
// Odstrelovac
#define SNIPER_POWER 115
#define SNIPER_HP 250
#define SNIPER_TR 25
// Tank
#define TANK_POWER 200
#define TANK_HP 800
#define TANK_TR 30

/**** BUDOUCNOST ***/
// Supervojak
#define SUPERSOLDIER_POWER 70
#define SUPERSOLDIER_HP 400
#define SUPERSOLDIER_TR 30
// Droid
#define DROID_POWER 120
#define DROID_HP 500
#define DROID_TR 35
// Autonomni tank
#define AUTONOMOUSTANK_POWER 300
#define AUTONOMOUSTANK_HP 1000
#define AUTONOMOUSTANK_TR 40

#define TRAINING_CENTRE1_CAPACITY 1000
#define TRAINING_CENTRE2_CAPACITY 1000

Store Training1("Vycvikove centrum", TRAINING_CENTRE1_CAPACITY);
Store Training2("Vycvikove centrum", TRAINING_CENTRE2_CAPACITY);
Histogram trainTime1("Doba vycviceni jednotky - armada 1", 0, 100, 100);
Histogram trainTime2("Doba vycviceni jednotky - armada 2", 0, 100, 100);

Histogram fallenUnits1("Padle jednotky - armada 1", 0, 1, 9);
Histogram fallenUnits2("Padle jednotky - armada 2", 0, 1, 9);

Histogram takenHpDisease1("Odebrane zivoty vlivem nemoci - armada 1", 0, 20, 20);
Histogram takenHpDisease2("Odebrane zivoty vlivem nemoci - armada 2", 0, 20, 20);

Histogram takenHpWeather1("Odebrane zivoty vlivem pocasi - armada 1", 0, 20, 20);
Histogram takenHpWeather2("Odebrane zivoty vlivem pocasi - armada 2", 0, 20, 20);

Histogram supply1("Zvyseni narocnosti vycviku - armada 1", 0, 20, 20);
Histogram supply2("Zvyseni narocnosti vycviku - armada 2", 0, 20, 20);

ulong realisedAttacks = 0;

Histogram startFight("Kdo zacal utocit", 1, 1, 2);

int hpBase1 = 10000;
int hpBase2 = 10000;

Queue f1;
Queue f2;
int order1 = 1;
int order2 = 1;
int killed1 = 0;
int killed2 = 0;
int count1 = 0;
int count2 = 0;

bool winDecided = false;
bool silent = false;

int unitsT1[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int unitsT2[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int unitsPower[9] = {INFANTRY_POWER, KNIGHT_POWER, CATAPULT_POWER,
                     SOLDIER_POWER, SNIPER_POWER, TANK_POWER,
                     SUPERSOLDIER_POWER, DROID_POWER, AUTONOMOUSTANK_POWER};
int unitsHp[9] = {INFANTRY_HP, KNIGHT_HP, CATAPULT_HP,
                  SOLDIER_HP, SNIPER_HP, TANK_HP,
                  SUPERSOLDIER_HP, DROID_HP, AUTONOMOUSTANK_HP};
string unitsName[9] = {"Pesak", "Rytir", "Katapult",
                       "Vojak", "Odstrelovac", "Tank",
                       "Supervojak", "Droid", "Autonomni tank"};
int unitsT1TrainReq[9] = {INFANTRY_TR, KNIGHT_TR, CATAPULT_TR,
                             SOLDIER_TR, SNIPER_TR, TANK_TR,
                             SUPERSOLDIER_TR, DROID_TR, AUTONOMOUSTANK_TR};
int unitsT2TrainReq[9] = {INFANTRY_TR, KNIGHT_TR, CATAPULT_TR,
                             SOLDIER_TR, SNIPER_TR, TANK_TR,
                             SUPERSOLDIER_TR, DROID_TR, AUTONOMOUSTANK_TR};
int unitsInitTrainReq[9] = {INFANTRY_TR, KNIGHT_TR, CATAPULT_TR,
                          SOLDIER_TR, SNIPER_TR, TANK_TR,
                          SUPERSOLDIER_TR, DROID_TR, AUTONOMOUSTANK_TR};

// prevzato z https://stackoverflow.com/questions/1657883/variable-number-of-arguments-in-c
template<class... Args>
void print(Args... args){
    if(!silent)
        (std::cout << ... << args);
}

class Unit : public Process{
public:
    int power;
    int hp;
    int number;
    int trainReq;
    string name;
    int team;
    int id;

    Unit(int power, int hp, int trainReq, string name, int team, int id) {
        this->power = power;
        this->hp = hp;
        this->trainReq = trainReq;
        this->number = 0;
        this->name = name;
        this->team = team;
        this->id = id;
    }

    void Behavior(){
        if(team == 1){
            this->number = order1++;
            int income = Time;
            Enter(Training1, trainReq);
            Wait(Uniform(15, 25));
            Leave(Training1, trainReq);
            trainTime1(Time - income);
            f1.Insert(this);
        }
        else{
            this->number = order2++;
            int income = Time;
            Enter(Training2, trainReq);
            Wait(Uniform(15, 25));
            Leave(Training2, trainReq);
            trainTime2(Time - income);
            f2.Insert(this);
        }
        Passivate();
    }
};



class UnitSpawner : public Event {
    void Behavior(){
        for (int i = 0; i < 9; i++) {
            for(int q = 0; q < unitsT1[i]; q++){
                count1++;
                (new Unit(unitsPower[i], unitsHp[i],
                    unitsT1TrainReq[i], unitsName[i], 1, i))->Activate();
            }
            for(int q = 0; q < unitsT2[i]; q++){
                count2++;
                (new Unit(unitsPower[i], unitsHp[i],
                    unitsT2TrainReq[i], unitsName[i], 2, i))->Activate();
            }
        }
    }
};


class Fight : public Process {

    void Behavior(){
        Unit *unit1;
        Unit *unit2;
        Queue *q1 = &f1;
        Queue *q2 = &f2;
        int *s1 = &killed1;
        int *s2 = &killed2;

        bool switched = false;
        // vyber prvniho tahu
        if(Random()<0.5f){
            unit2 = (Unit*)f1.GetFirst();
            unit1 = (Unit*)f2.GetFirst();
            q1 = &f2;
            q2 = &f1;
            s2 = &killed1;
            s1 = &killed2;
            switched = true;
            startFight(2);
        }
        else{
            unit1 = (Unit*)f1.GetFirst();
            unit2 = (Unit*)f2.GetFirst();
            startFight(1);
        }
        print(unit1->team , ". " , unit1->name , "(" , unit1->number , ")"
        , " *" , unit1->hp , "* vs " , unit2->team , ". "
        , unit2->name , "(" , unit2->number , ")"
        , " *" , unit2->hp , "*");

        //utok 1->2
        unit2->hp -= unit1->power;
        // prezity utok 1->2
        if(unit2->hp > 0){
            // utok 2->1
            unit1->hp -= unit2->power;
            // prezity utok 2->1
            if(unit1->hp > 0){
                q1->InsFirst(unit1);
                print(" => Remiza\n");
            }
            else{ //smrt z utoku 2->1
                if(switched)
                    fallenUnits2(unit1->id);
                else
                    fallenUnits1(unit1->id);
                ((Process*)unit1)->Terminate();
                (*s1)++;
                print(" => " , unit2->team , ". " , unit2->name , "("
                , unit2->number , ") wins\n");
            }
            q2->InsFirst(unit2);
        }
        else{ // smrt z utoku 1->2
            if(switched)
                fallenUnits1(unit2->id);
            else
                fallenUnits2(unit2->id);
            ((Process*)unit2)->Terminate();
            q1->InsFirst(unit1);
            (*s2)++;
            print(" => " , unit1->team , ". " , unit1->name , "("
            , unit1->number , ") wins\n");
        }

        print("pocet bojovniku A1: " , f1.Length() , " | pocet bojovniku A2: " , f2.Length(), "\n");
        print("----------------------------------------------------------\n");
    }
};

class FightEvent : public Event {
    void Behavior(){
        if(!f1.Empty() && !f2.Empty()){
            (new Fight)->Activate();
        }
        else if(f1.Empty() && !f2.Empty()){
            print("Utok na zakladnu armady 1\n");
            Unit * unit = (Unit*)f2.GetFirst();
            hpBase1 -= unit->power;
            f2.InsFirst(unit);
        }
        else if(!f1.Empty() && f2.Empty()){
            print("Utok na zakladnu armady 2\n");
            Unit * unit = (Unit*)f1.GetFirst();
            hpBase2 -= unit->power;
            f1.InsFirst(unit);
        }

        if(killed1 == count1){
            cout << "\nArmada 2 vitezi! (zabiti vsichni nepratele)\n";
            winDecided = true;
        }
        else if(killed2 == count2){
            cout << "\nArmada 1 vitezi! (zabiti vsichni nepratele)\n";
            winDecided = true;
        }
        else{
            if (hpBase1 > 0 && hpBase2 > 0 && !winDecided){
                realisedAttacks++;
                Activate(Time + 10);
            }
            else if(hpBase1 <= 0){
                cout << "\nArmada 2 vitezi (znicena zakladna nepritele)\n";
                winDecided = true;
            }
            else if(hpBase2 <= 0){
                winDecided = true;
                cout << "\nArmada 1 vitezi (znicena zakladna nepritele)\n";
            }
        }
    }
};

class Incident1Remove : public Process {
    void Behavior(){
        if(!winDecided){
            for(uint i = 0; i < 9; i++){
                unitsT1TrainReq[i] = unitsInitTrainReq[i];
            }
            print("Zasobovani armady 1 obnoveno\n");
        }
    }
};

class Incident2Remove : public Process {
    void Behavior(){
        if(!winDecided){
            for(uint i = 0; i < 9; i++){
                unitsT2TrainReq[i] = unitsInitTrainReq[i];
            }
            print("Zasobovani armady 2 obnoveno\n");
        }
    }
};

class Incident : public Process {
    void Behavior(){
        if(!winDecided){
            double r = Random();
            int hpTemp = 0;
            Unit *unit;
            if(r <= 0.33f){ //nemoc
                if(Random() >= 0.5){ //team1
                    for(uint i = 0; i < f1.Length(); i++){
                        unit = (Unit*)f1.GetFirst();
                        hpTemp = unit->hp;
                        unit->hp = (int)round(Uniform(0.7f, 0.9f) * unit->hp);
                        takenHpDisease1(hpTemp - unit->hp);
                        f1.InsLast(unit);
                    }
                    print("***Armada 1 byla zasazena nemoci***\n");
                }
                else{ //team2
                    for(uint i = 0; i < f2.Length(); i++){
                        unit = (Unit*)f2.GetFirst();
                        hpTemp = unit->hp;
                        unit->hp = (int)round(Uniform(0.7f, 0.9f) * unit->hp);
                        takenHpDisease2(hpTemp - unit->hp);
                        f2.InsLast(unit);
                    }
                    print("***Armada 2 byla zasazena nemoci***\n");
                }
            }
            else if(r > 0.33f && r <= 0.66f){   // pocasi
                for(uint i = 0; i < f1.Length(); i++){
                    unit = (Unit*)f1.GetFirst();
                    hpTemp = unit->hp;
                    unit->hp = (int)round(0.7f * unit->hp);
                    takenHpWeather1(hpTemp - unit->hp);
                    f1.InsLast(unit);
                }

                for(uint i = 0; i < f2.Length(); i++){
                    unit = (Unit*)f2.GetFirst();
                    hpTemp = unit->hp;
                    unit->hp = (int)round(0.7f * unit->hp);
                    takenHpWeather2(hpTemp - unit->hp);
                    f2.InsLast(unit);
                }
                print(">>>Neprizen pocasi ztizila bojove podminky<<<\n");
            }
            else{   // zasobovani - zpomaleni vycviku
                int reqTemp = 0;
                if(Random() >= 0.5){ //team1
                    for(uint i = 0; i < 9; i++){
                        reqTemp = unitsT1TrainReq[i];
                        unitsT1TrainReq[i] = (int)round(unitsT1TrainReq[i] * Uniform(1.5f, 2.5f));
                        supply1(unitsT1TrainReq[i] - reqTemp);
                    }
                    print("!!!Vypadek zasobovani snizil kapacitu vycviku armady 1!!!\n");
                    (new Incident1Remove)->Activate(Time + 200);
                }
                else{ //team2
                    for(uint i = 0; i < 9; i++){
                        reqTemp = unitsT2TrainReq[i];
                        unitsT2TrainReq[i] = (int)round(unitsT2TrainReq[i] * Uniform(1.5f, 2.5f));
                        supply2(unitsT2TrainReq[i] - reqTemp);
                    }
                    print("!!!Vypadek zasobovani snizil kapacitu vycviku armady 2!!!\n");
                    (new Incident2Remove)->Activate(Time + 200);
                }
            }
        }
    }
};

class IncidentEvent : public Event {
    void Behavior(){
        (new Incident)->Activate();
        if(!winDecided)
            Activate(Time + Exponential(75));
    }
};

int main(int argc, char *argv[]) {
    ArgParser argParser(argc, argv);
    argParser.setUnits(unitsT1, unitsT2);
    silent = argParser.silent;

    Init(0);
    (new UnitSpawner)->Activate();
    (new FightEvent)->Activate();
    if(argParser.externalIncidents)
        (new IncidentEvent)->Activate();
    Run();

    if(!winDecided){
        if(hpBase1 > hpBase2){
            cout << "\nArmada 1 vitezi (mene poskozena zakladna)" << endl;
        }
        else if(hpBase2 > hpBase1){
            cout << "\nArmada 2 vitezi (mene poskozena zakladna)" << endl;
        }
        else{
            cout << "\nRemiza" << endl;
        }
    }

    cout << "\n----------- STATISTIKY BOJE ----------------" << endl;
    cout << "Bojovniku armady 1: " << count1 << " | zabito: " << killed1 << endl;
    cout << "Bojovniku armady 2: " << count2 << " | zabito: " << killed2 << endl;
    cout << "Zivoty zaklady armady 1: " << hpBase1 << endl;
    cout << "Zivoty zaklady armady 2: " << hpBase2 << endl;
    cout << "Celkovy pocet souboju: " << realisedAttacks << endl;
    fallenUnits1.Output();
    fallenUnits2.Output();

    if(argParser.externalIncidents){
        takenHpDisease1.Output();
        takenHpDisease2.Output();
        takenHpWeather1.Output();
        takenHpWeather2.Output();
        supply1.Output();
        supply2.Output();
    }

    if(argParser.stats){
        trainTime1.Output();
        trainTime2.Output();
        startFight.Output();
    }
    //trainTime1.Output();
    return 0;
}
