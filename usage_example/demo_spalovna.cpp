//
// Created by hater on 6. 12. 2020.
//
#include "../lib/sim.h"


class Package: virtual public sim::Event{
public:

    static int count;
    static double vynimocny;
    static double start_tmp;
    static double end_tmp;
    sim::Random_g generator;
    double length=generator.exp_m(2);
    double value=1;
    int priority = 0;
    virtual void execute(sim::Simulator *simulate) ;
};

class Car: virtual public sim::Event{
public:
    int id;
    sim::Random_g generator;
    double length=generator.normal(1.2,0.1);
    virtual void execute(sim::Simulator *simulate) ;
};
class Comb_chamber : virtual public sim::Storage<Package> {
public:
    bool special=false;
    std::vector<Car*> cars;
};
Comb_chamber *comb_chamber = new Comb_chamber();
void Car::execute(sim::Simulator *simulate) {

    Package *package = new Package();
    sim::Random_g generator;
    comb_chamber->Enter(simulate, package);
    if(comb_chamber->queue.size()> 0 ){
        if (Package::start_tmp == 0)Package::start_tmp = ((sim::Simulator*)simulate)->now();
        comb_chamber->special=true;
    }
    if(comb_chamber->special){
        comb_chamber->cars.push_back(this);
    }
    else{
        time = ((sim::Simulator*)simulate)->now() + generator.normal(1.2,0.1);

        simulate->events.insert(this);
    }

}
void Package::execute(sim::Simulator *simulate) {
    Package::count++;
    comb_chamber->Leave(simulate,this);
    if(comb_chamber->special && comb_chamber->current_capacity==0){
        while(comb_chamber->cars.size() > 0){
            Car *tmp = comb_chamber->cars.front();
            tmp->time = ((sim::Simulator*)simulate)->now() + generator.normal(1.2,0.1);
            comb_chamber->cars.erase(comb_chamber->cars.begin());
            simulate->events.insert(tmp);
            comb_chamber->special= false;
            Package::end_tmp= ((sim::Simulator*)simulate)->now();
            if (Package::start_tmp != 0){

                Package::vynimocny += (Package::end_tmp-Package::start_tmp);
                Package::start_tmp = 0;
            }
        }


    }
}

class Spalovna : public sim::Simulator {
public:

    void start() {
        sim::ListQueue events_q;
        end_time=168;
        comb_chamber->capacity=7;
        Car *car1 = new Car();
        Car *car2 = new Car();
        Car *car3 = new Car();
        Car *car4 = new Car();
        Car *car5 = new Car();

        car1->time=car1->length;
        car2->time=car2->length;
        car3->time=car3->length;
        car4->time=car4->length;
        car5->time=car5->length;
        car1->id = 0;
        car2->id=1;
        car3->id=2;
        car4->id=3;
        car5->id=4;
        events_q.insert(car1);
        events_q.insert(car2);
        events_q.insert(car3);
        events_q.insert(car4);
      //  events_q.insert(car5);
        events=events_q;
        doAllEvents();

        printf(" vynimocny cas %f\n",Package::vynimocny);
        printf(" count  %d\n",Package::count);
    }

};
int Package::count=0;
double Package::vynimocny=0;
double Package::start_tmp=0;
double Package::end_tmp=0;