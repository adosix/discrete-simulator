//
// Created by hater on 7. 12. 2020.
//
#include "../lib/sim.h"
class Poziadavka: virtual public sim::Event{

public:
    static int pocet_vytvorenych_produktov;
    static int pocet_zlozenych_prodkutov;
    static int pocet_nalakovanych_prodkuktov;
    static int pocet_vyhodenych_produktov;

    static const int vyrobena = 0;
    static const int namontovana = 1;
    int state=vyrobena;
    sim::Random_g generator;

    virtual void execute(sim::Simulator *simulate) ;
};

class Generator : virtual public sim::Event{
public:

    sim::Random_g generator;
    double length=generator.exp_m(30);
    virtual void execute(sim::Simulator *simulate) ;
};
class Montovna : virtual public sim::Facility<Poziadavka> {
public:
    static int max_montovna;
    bool closed=false;
};
class Lakovna : virtual public sim::Facility<Poziadavka> {
public:

};
Montovna *montovna = new Montovna();
Lakovna *lakovna = new Lakovna();
int Poziadavka::pocet_vytvorenych_produktov=0;
int Poziadavka::pocet_zlozenych_prodkutov=0;
int Poziadavka::pocet_nalakovanych_prodkuktov=0;
int Poziadavka::pocet_vyhodenych_produktov=0;
int Montovna::max_montovna=0;
void Generator::execute(sim::Simulator *simulate) {
    Poziadavka::pocet_vytvorenych_produktov++;
    Poziadavka *poziadavka = new Poziadavka();
    sim::Random_g generator;
    poziadavka->length = generator.uniform(25,35);
    montovna->Seize(simulate,poziadavka);
    if(Montovna::max_montovna <montovna->queue_f.size() ) Montovna::max_montovna = montovna->queue_f.size();
    time =  ((sim::Simulator*)simulate)->now() +generator.exp_m(30);
    simulate->events.insert(this);
}
void Poziadavka::execute(sim::Simulator *simulate) {
    sim::Random_g generator;
    if(this->state == vyrobena){
        if(montovna->closed){
            Poziadavka::pocet_vyhodenych_produktov++;
            montovna->Seize(simulate, this);
            if(Montovna::max_montovna < (int)(montovna->queue_f.size()) ) Montovna::max_montovna = montovna->queue_f.size();
            return;
        }
        this->state = namontovana;
        this->length =  generator.uniform(30,40);
        Poziadavka::pocet_zlozenych_prodkutov++;
        montovna->Release(simulate);
        lakovna->Seize(simulate,this);
        if(lakovna->queue_f.size() == 5){
            montovna->closed=true;
        }
    }
    else if(this->state == namontovana){
        Poziadavka::pocet_nalakovanych_prodkuktov++;
        lakovna->Release(simulate);
        if(montovna->closed && lakovna->queue_f.size() == 0){
            montovna->closed=false;
            montovna->Release(simulate);
        }
    }
}

class Lakovna_sim : virtual public sim::Simulator {
public:
    void start() {
        sim::ListQueue events_q;
        end_time=24*60;

        Generator *gen = new Generator();
        gen->time=gen->length;
        events_q.insert(gen);

        events=events_q;
        doAllEvents();
        printf("Pocet vytvorenych produktov  %d\n", Poziadavka::pocet_vytvorenych_produktov);
        printf("Pocet zlozenych produktov %d\n", Poziadavka::pocet_zlozenych_prodkutov);
        printf("Pocet nalakovanych produktov %d\n", Poziadavka::pocet_nalakovanych_prodkuktov);
        printf("Pocet vyhodenych produktov %d\n", Poziadavka::pocet_vyhodenych_produktov);
        printf("dlzka rady %d\n", Montovna::max_montovna);
        printf("aktualny pocet v rade na lakovanie %ld\n", lakovna->queue_f.size());
        printf("aktualny pocet v rade na montovanie %ld\n", montovna->queue_f.size());
        printf("Ked prevadzka bezi s frekvenciou prichadzajucich poziadavkoch ako exp_m(30)\n" );
        printf("Ked prevadzka bezi s dlzkou montovania ako normal(25,35)\n" );
        printf("Ked prevadzka bezi s dlzkou lakovania ako normal(30,40)\n" );
        printf("Ked prevadzka bezi s radou 5\n" );
    }

};