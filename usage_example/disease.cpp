#include "../lib/sim.h"

class Person : virtual public sim::Event {
public:
static const int SUSCEPTIBLE = 0;
static const int INFECTIOUS = 1;
static const int RECOVERED = 2;
static const int nStates = 3;
constexpr static const double meanInfectiousDuration = 10.0;
constexpr static const double meanInterContactInterval = 4.0;;

/**
 *  A vector with every person in the population
*/
static std::vector<Person>  population;
/**
 * A count of the number of people in each state
 */
    static int count[nStates];
    int state;
    int nextState;
    double recoveryTime;

    Person (int initialState) {
        state = initialState;
        population.push_back(*this);
        count[state]++;
    }
    
void execute(sim::Simulator *simulate) {
    if (nextState == INFECTIOUS) { // action is make contact
        makeContact((sim::Simulator*) simulate);
        scheduleNextEvent((sim::Simulator*) simulate);
    } // else action is recover
    count[state]--;
    state = nextState;
    count[state]++;
    display((sim::Simulator*) simulate);
}

/**
* Display the current population distribution
*/
void display(sim::Simulator *simulate) {
    printf("t= %f " , simulate->now());
    for (int i=0; i< nStates; i++) {
        printf(" count [ %d ]= %d",i , count[i]);
    }
   printf("%ld all\n", population.size());
}

/**
* Pick a random member of the population and infect the person if it
* is susceptible.
*/
void makeContact(sim::Simulator *simulate) {
    sim::Random_g generato;
    Person *person = &population.at(
            static_cast<int>(population.size()*generato.rand()));
    if (person->state != SUSCEPTIBLE) return;
    person->recoveryTime = simulate->now() + generato.exp(meanInfectiousDuration);
    person->scheduleNextEvent(simulate);
}
/**
* Schedule the next event, which will be a contact or recovery, whichever
* comes first.
*/
void scheduleNextEvent(sim::Simulator *simulate) {
    sim::Random_g generato;
    double nextContactTime = simulate->now() + generato.exp(meanInterContactInterval);
    if (nextContactTime < recoveryTime) {
        nextState = INFECTIOUS;
        time = nextContactTime;
    } else {
        nextState = RECOVERED;
        time = recoveryTime;
    }
    simulate->events.insert(this);
}
};

class DiseaseSimulator : public sim::Simulator {
public:
void start() { 

    sim::ListQueue events_q;
    sim::Random_g gen;  

    Person *person;
    for(int i = 0; i< person->nStates; i++){
        person->count[i] = 0;
    }
    for (int i = 0; i< 995; i++) {
        person=new Person(0);
    }
    for (int i = 0; i< 5; i++) {
        person = new Person(1);
        person->recoveryTime = gen.exp(person->meanInfectiousDuration);
        person->scheduleNextEvent(this);
    }

    doAllEvents();
    printf("legenda mozu sa nakazit = 0,nakazeni = 1, vylieceni = 2\n");
}
};

    std::vector<Person> Person::population = {0};
    int Person::count[nStates] = {0,1,2};