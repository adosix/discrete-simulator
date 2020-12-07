//
// Created by hater on 6. 12. 2020.
//
#include "../lib/sim.h"
class Package: virtual public sim::Event{
public:


    sim::Random_g generator;
    double length=2+generator.exp_m(1);
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

    printf("prislo auto %d,%f\n",this->id,((sim::Simulator*)simulate)->now() );
    Package *package = new Package();
    sim::Random_g generator;
    comb_chamber->Enter(simulate, package);
    if(comb_chamber->queue.size()> 0 ){
        printf("vynimocny cas\n");
        comb_chamber->special=true;
    }
    if(comb_chamber->special){
        comb_chamber->cars.push_back(this);
    }
    else{
        time = ((sim::Simulator*)simulate)->now() + generator.normal(1.2,0.1);

        printf("zhori %f, trvanie horenia %f\n",package->time, package->length );
        printf("nove auto %d,%f\n",this->id,time );
        printf("storage capacita %d\n",comb_chamber->current_capacity );
        printf("queue kapacita %ld\n",comb_chamber->queue.size() );
        simulate->events.insert(this);
    }

}
void Package::execute(sim::Simulator *simulate) {
    printf("spalil som sa\n");

    comb_chamber->Leave(simulate,this);
    printf("storage capacita %d\n",comb_chamber->current_capacity );
    if(comb_chamber->special && comb_chamber->current_capacity==0){
        while(comb_chamber->cars.size() > 0){
            Car *tmp = comb_chamber->cars.front();
            tmp->time = ((sim::Simulator*)simulate)->now() + generator.normal(1.2,0.1);
            comb_chamber->cars.erase(comb_chamber->cars.begin());
            simulate->events.insert(tmp);
            printf("nove auto %d,%f\n",tmp->id,tmp->time );
            comb_chamber->special= false;
        }


    }
}

class Spalovna : public sim::Simulator {
public:
    void start() {
        sim::ListQueue events_q;
        end_time=12;
        comb_chamber->capacity=5;
        Car *car1 = new Car();
        Car *car2 = new Car();
        Car *car3 = new Car();

        car1->time=car1->length;
        car2->time=car2->length;
        car3->time=car3->length;

        car1->id = 0;
        car2->id=1;
        car3->id=2;
        events_q.insert(car1);
        events_q.insert(car2);
        events_q.insert(car3);

        events=events_q;
        doAllEvents();
    }

};