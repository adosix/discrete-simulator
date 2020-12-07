#include "../lib/sim.h"

class Death : virtual public sim::Event {
public: 
    Death(double time_b) {
        time = time_b;
    }
    Death(double time_b, int prio) {
        time = time_b;
        priority=prio;
    }
    virtual void execute(sim::Simulator *simulate) {
        sim::Random_g generator;

        printf("%4.f Time when angel finished his last job.  \n", generator.normal(5,10));
        printf("%4.f Time when person died, human soul has priority %d for angel\n", time,priority);
    }
};

class DeathSimulator : public sim::Simulator {
    public: void start() {
        sim::ListQueue events_q;

        sim::Event *event_ptr = new Death(1,2);
        events_q.insert(event_ptr);
        event_ptr = new Death(2);
        events_q.insert(event_ptr);
        event_ptr = new Death(1);
        events_q.insert(event_ptr);
        event_ptr = new Death(1,1);
        events_q.insert(event_ptr);
        event_ptr = new Death(0);
        events_q.insert(event_ptr);
        event_ptr = new Death(2,1);
        events_q.insert(event_ptr);
        event_ptr = new Death(4,1);
        events_q.insert(event_ptr);
        event_ptr = new Death(2);
        events_q.insert(event_ptr);
        event_ptr = new Death(4,2);
        events_q.insert(event_ptr);

        events = events_q;
        doAllEvents();

    }
};