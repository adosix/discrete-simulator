#include "../lib/sim.h"

class Customer_v : virtual public sim::Event{
public:
    sim::Facility<Customer_v> *fac;
    int som=1;
    Customer_v(int som){
        som = som;
    }
    sim::Random_g generato;
    double length=generato.rand();
    double value=2;
    int priority = (int)(generato.rand()*10);
    virtual void execute(sim::Simulator *simulate) ;
};

class Generator_v : virtual public sim::Event {
public:
    sim::Facility<Customer_v> *fac;
    Generator_v(sim::Facility<Customer_v> *facility){
        fac=facility;
    }
    virtual void execute(sim::Simulator *simulate) ;
};

class Server_s : virtual public sim::Facility<Customer_v> {
public:
   sim::Queue_t<Customer_v> queue_v;

/**
* The customer's service is completed so print a message.
* If the queue is not empty, get the next customer.
*/
    void Seize(sim::Simulator *simulate, Customer_v customer) {

        busy_flag=true;

        sim::Random_g generato;
        double serviceTime = generato.exp(1.0);
        customer.time = ((sim::Simulator*)simulate)->now() + serviceTime;
        simulate->events.insert(&customer);
    }
    void Release(sim::Simulator *simulate) {

        busy_flag=false;
        if (queue.size()>0) {

            Customer_v *customer= queue.remove();
            simulate->events.insert( customer);
        }

    }
};

void Generator_v::execute(sim::Simulator *simulate) {
    Customer_v *customer =  new Customer_v(0);
    customer->fac=fac;
    sim::Random_g generato;
    time = ((sim::Simulator*)simulate)->now()+ generato.exp(8.0);
    fac->Seize(simulate,customer);


    if (time < 10.0) simulate->events.insert(this);

};
void Customer_v::execute(sim::Simulator *simulate){
printf("Finished serving %d at time %f.\n" , this->som , time);
fac->Release(simulate);
}
class BankSimulator_v : public sim::Simulator {
public:
    void start() {
        sim::ListQueue events_q;

        /* Create the generator, queue, and simulator */

        sim::Facility<Customer_v> *fac = new sim::Facility<Customer_v>();
        Generator_v generator = Generator_v(fac);
        /* Connect them together. */


        /* Start the generator by creating one customer immediately */
        generator.time = 0.0;
        events_q.insert(&generator);
        events=events_q;
       // doAllEvents();

    }
};