#include <getopt.h>
#include "disease.cpp"
#include "death_events.cpp"
#include "bank.cpp"
#include "demo_spalovna.cpp"
#include "demo_lakovna.cpp"
#include "../lib/sim.h"



#define REQUIRED_ARGUMENT 1
#define NO_ARGUMENT 0

class Arguments;
Arguments *arguments;
/**
 * Class which processes arguments
 */
class Arguments
{
    public:
        std::string example;
        std::string distribution;
        static Arguments* parse_arguments(int argc, char **argv);

    Arguments()
    {
        //init setting
        this->example = "";
        this->distribution = "";
    }

    /**
     * Help
     */
    void print_help()
    {
        printf("SIM - DISCRETE SIMULATION LIBRARY\n\n");
        printf("Arguments:\n");
        printf("-h/--help: help\n");
        printf("-e/--example <example>: example to run\n");
        printf("-d/--distribution <dist>: asci art (exp/norm/rand/uniform) \n");
        exit(0);
    }
};

Arguments* Arguments::parse_arguments(int argc, char **argv){
    Arguments *arguments = new Arguments();

    bool wasServer = false;
    char option;

    const struct option longopts[] =
    {
        {"help", NO_ARGUMENT, 0, 'h'},
        {"example", REQUIRED_ARGUMENT, 0, 'e'},
        {"distribution", REQUIRED_ARGUMENT, 0, 'd'}
    };

    int index;
    while((option = getopt_long(argc, argv, "he:d:", longopts,&index)) != -1){
        
        switch(option){
            case 'h':
                arguments->print_help();
                break;
            case 'e':
                arguments->example = optarg;
                break;
            case 'd':
                arguments->distribution = optarg;
                break;
            default:
                arguments->print_help();;
        }
    }

    if (arguments->example == "" && arguments->distribution == ""){
        std::cerr << "example or distribution has to be set\n";
        exit(1);
    }
    
    return arguments;
}

int main(int argc, char *argv[]){
     arguments = Arguments::parse_arguments(argc, argv);
    //BankSimulator().start();
    if((arguments->distribution).compare("exp")==0){
        sim::Random_g generato;
        generato.distributions();
    }

    if((arguments->example).compare("death")==0){
        DeathSimulator sim;
        sim.start();
    }
    else if((arguments->example).compare("disease")==0){
        DiseaseSimulator sim;
        sim.start();
    }
    else if ((arguments->example).compare("bank")==0){
        BankSimulator_v sim;
        sim.start();
    }
    else if ((arguments->example).compare("spalovna")==0){
       Spalovna sim;
        sim.start();
    }
    else{
        Lakovna_sim sim;
        sim.start();
    }
    return 0;
};