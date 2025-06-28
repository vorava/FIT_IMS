// IMS projekt
// Vojtech Orava (xorava02)
#ifndef __ARG_PARSER_H__
#define __ARG_PARSER_H__

#include <iostream>
#include <getopt.h>
using namespace std;

class ArgParser{

public:
    ArgParser(int argc, char* argv[]);
    int infantry[2] = {0,0};
    int knights[2] = {0, 0};
    int catapults[2] = {0, 0};

    int soldiers[2] = {0, 0};
    int snipers[2] = {0, 0};
    int tanks[2] = {0, 0};

    int supersoldiers[2] = {0, 0};
    int droids[2] = {0, 0};
    int autonomoustanks[2] = {0, 0};

    bool silent = false;
    bool externalIncidents = false;
    bool stats = false;

    void setUnits(int *u1, int *u2);

private:
    void checkIndexes(int *indexes);

};


#endif
