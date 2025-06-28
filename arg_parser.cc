// IMS projekt
// Vojtech Orava (xorava02)
#include "arg_parser.h"
#include <cstring>

ArgParser::ArgParser(int argc, char* argv[]){
    option longOptions[] = {
                        {"pesak", required_argument, 0, 'p'},
                        {"rytir", required_argument, 0, 'r'},
                        {"katapult", required_argument, 0, 'k'},

                        {"vojak", required_argument, 0, 'v'},
                        {"odstrelovac", required_argument, 0, 'o'},
                        {"tank", required_argument, 0, 't'},

                        {"supervojak", required_argument, 0, 's'},
                        {"droid", required_argument, 0, 'd'},
                        {"autotank", required_argument, 0, 'a'},

                        {"silent", no_argument, 0, 0},
                        {"external", no_argument, 0, 0},
                        {"stats", no_argument, 0, 0},
                        {"help", no_argument, 0, 'h'},

                        {0,0,0,0}};
    int optionIndex = 0;
    int indexes[9] = {0,0,0,0,0,0,0,0,0};
    char c;
    // samotne zpracovani argumentu
    while ((c = getopt_long(argc, argv, "p:r:k:v:o:t:s:d:a:h", longOptions, &optionIndex)) != -1){
        checkIndexes(indexes);
        switch (c){
            case 0:
                if(!strcmp(longOptions[optionIndex].name, "silent")){
                    this->silent = true;
                }

                if(!strcmp(longOptions[optionIndex].name, "external")){
                    this->externalIncidents = true;
                }

                if(!strcmp(longOptions[optionIndex].name, "stats")){
                    this->stats = true;
                }
                break;
            case 'h':
                cout <<
                "Simulator vojenske bitvy pomoci knihovny SIMLIB. Simuluje frontovy stred 2 armad.\n"
                "Pouziti: ./bitva [PARAMETRY]\n"
                "         [PARAMETRY] mohou byt:\n"
                "         -p pocet jednotek Pechota\n"
                "         -r pocet jednotek Rytir\n"
                "         -k pocet jednotek Katapult\n"
                "         -v pocet jednotek Vojak\n"
                "         -o pocet jednotek Odstrelovac\n"
                "         -t pocet jednotek Tank\n"
                "         -s pocet jednotek Supervojak\n"
                "         -d pocet jednotek Droid\n"
                "         -a pocet jednotek Autonomni tank\n"
                "         !!!Prvni vyskyt parametru nastavuje jednotky Armady 1, druhy vyskyt jednotky Armady 2!!!\n\n"
                "         -h --help     vypis teto napovedy\n"
                "         --silent      program nebude vypisovat detailni informace bitvy (konkretni souboje)\n"
                "         --external    boje mohou byt ovlivneny externimi vlivy (pocasi, nemoci, vypadek zasobovani)\n"
                "         --stats       detailni statistiky \n\n"
                "Priklady spusteni:\n"
                "         ./bitva -p 10 -p 15 --external\n"
                "         ./bitva -p 0 -p 15 -v 20 --external --silent\n"
                "         ./bitva -p 10 -p 15 -v 0 -v 30 -a 4 -j 10 -a 7 --silent\n"
                "\nAutor: Vojtech Orava (2022), IMS projekt, Vojenske simulatory"
                << endl;
                exit(0);
                break;
            case 'p':
                this->infantry[indexes[0]++] = stoi(optarg);
                break;
            case 'r':
                this->knights[indexes[1]++] = stoi(optarg);
                break;
            case 'k':
                this->catapults[indexes[2]++] = stoi(optarg);
                break;

            case 'v':
                this->soldiers[indexes[3]++] = stoi(optarg);
                break;
            case 'o':
                this->snipers[indexes[4]++] = stoi(optarg);
                break;
            case 't':
                this->tanks[indexes[5]++] = stoi(optarg);
                break;

            case 's':
                this->supersoldiers[indexes[6]++] = stoi(optarg);
                break;
            case 'd':
                this->droids[indexes[7]++] = stoi(optarg);
                break;
            case 'a':
                this->autonomoustanks[indexes[8]++] = stoi(optarg);
                break;

            default:
                break;
        }
   }
}

void ArgParser::checkIndexes(int *indexes){
    for(int i = 0; i < 9; i++){
        if(indexes[i] > 1){
            indexes[i] = 0;
        }
    }
}

void ArgParser::setUnits(int *u1, int *u2){
    u1[0] = this->infantry[0];
    u1[1] = this->knights[0];
    u1[2] = this->catapults[0];
    u1[3] = this->soldiers[0];
    u1[4] = this->snipers[0];
    u1[5] = this->tanks[0];
    u1[6] = this->supersoldiers[0];
    u1[7] = this->droids[0];
    u1[8] = this->autonomoustanks[0];

    u2[0] = this->infantry[1];
    u2[1] = this->knights[1];
    u2[2] = this->catapults[1];
    u2[3] = this->soldiers[1];
    u2[4] = this->snipers[1];
    u2[5] = this->tanks[1];
    u2[6] = this->supersoldiers[1];
    u2[7] = this->droids[1];
    u2[8] = this->autonomoustanks[1];
}
