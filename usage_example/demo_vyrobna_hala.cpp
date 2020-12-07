//
// Created by hater on 7. 12. 2020.
//
#include "../lib/sim.h"
class Poziadavka: virtual public sim::Event{
public:
    static const int vyrobena = 0;
    static const int namontovana = 1;
    int state=vyrobena;
    sim::Random_g generator;

    virtual void execute(sim::Simulator *simulate) ;
};

class Generator : virtual public sim::Event{
public:
    sim::Random_g generator;
    double length=30+generator.exp_m(1);
    virtual void execute(sim::Simulator *simulate) ;
};
class Montovna : virtual public sim::Facility<Poziadavka> {
public:
    bool closed=false;
};
class Lakovna : virtual public sim::Facility<Poziadavka> {
public:
};
Montovna *montovna = new Montovna();
Lakovna *lakovna = new Lakovna();
void Generator::execute(sim::Simulator *simulate) {
    printf("bola vyrobena poziadavka %f\n",((sim::Simulator*)simulate)->now() );

    Poziadavka *poziadavka = new Poziadavka();
    sim::Random_g generator;
    poziadavka->length = generator.uniform(30,40);
    printf("cas montovania %f\n", poziadavka->length);
    montovna->Seize(simulate,poziadavka);
    printf("montovna rada %ld\n", montovna->queue_f.size());
    time =  ((sim::Simulator*)simulate)->now() +30+generator.exp_m(1);
    simulate->events.insert(this);
}
void Poziadavka::execute(sim::Simulator *simulate) {
    sim::Random_g generator;
    if(this->state == vyrobena){
        if(montovna->closed){
            printf("produkt co vyhodime %f cas %f\n",this->length, ((sim::Simulator*)simulate)->now() );
            montovna->Seize(simulate, this);
            return;
        }
        this->state = namontovana;
        this->length =  generator.uniform(40,50);
        printf("produkt namontovany\n cas lakovania %f\n", this->length);
        montovna->Release(simulate);
        lakovna->Seize(simulate,this);
        if(lakovna->queue_f.size() == 3){
            printf("ZATVARAME\n");
            montovna->closed=true;
        }
        printf("lakovacia rada %ld\n", lakovna->queue_f.size());
    }
    else if(this->state == namontovana){
        printf("produkt nalakovany\n");
        lakovna->Release(simulate);
        if(montovna->closed && lakovna->queue_f.size() == 0){
            printf("OTVARAME\n");
            montovna->closed=false;
            printf("toto je cas montovania %f\n", montovna->queue_f.front()->length);
            montovna->Release(simulate);
        }
    }
}

class Lakovna_sim : virtual public sim::Simulator {
public:
    void start() {
        sim::ListQueue events_q;
        end_time=1000;

        Generator *gen = new Generator();
        gen->time=gen->length;
        events_q.insert(gen);

        events=events_q;
        doAllEvents();
    }

};