#include "sim.h"

namespace sim {
    bool Event::lessThan(double y) {
        return time <= y;
    }

    bool Event::equal(double y) {
        return time == y;
    }

    bool Event::greaterThan(int x) {
        return priority >= x;
    }


    double Random_g::exp(double m){
        std::random_device rd;
        std::mt19937 gen(rd());
        //parameter is lambda
        std::exponential_distribution<> d(m);
        
        return d(gen);
    }

    double Random_g::normal(double m, double q){
        std::random_device rd;
        std::mt19937 gen(rd());
        // values near the mean are the most likely
        // standard deviation affects the dispersion of generated values from the mean
        std::normal_distribution<> d{m,q};

        return d(gen);
    }

    double Random_g::rand(){
        std::random_device rd;
        std::mt19937 gen(rd());
        //uniform between 0-1 is random
        std::uniform_real_distribution<> d(0.0, 1.0);

        return d(gen);
    }

    double Random_g::uniform(double a, double b){
        std::random_device rd;
        std::mt19937 gen(rd());        
        std::uniform_real_distribution<> d(a, b);
        return d(gen);
    }

    int Random_g::distributions(){
        int s = 100;
            
        std::map<int, int> result_set;
        
        for (int i =0; i < 100000; ++i)
            ++result_set[exp(1)]; 

        for (auto& v : result_set) {
            std::cout << std::setprecision (1) << std::fixed;
            std::cout << v.first/1.f << " - " << (v.first+1)/1.f << " -> ";
            std::cout << std::string (v.second/100, '.') << std::endl;
        }
    }

    void ListQueue::insert(Event *e) {
        std::vector<Event *>::iterator it = elements.begin();

        if (elements.size() == 0) {
            elements.insert(it, e);
            return;
        }
        int i = 0;
        int elemests_size = elements.size();
        for (; i < elemests_size; i++) {

            if ((*it)->equal(e->time) && !(*it)->greaterThan(e->priority)) {
                elements.insert(it, e);
                break;
            }
            if (!(*it)->lessThan(e->time)) {
                elements.insert(it, e);
                break;
            }
            ++it;
            if ((1 + i) == elements.size()) {
                elements.insert(it, e);
                break;
            }
        }

    }

    void ListQueue::printlist() {
        std::vector<sim::Event *>::iterator it = elements.begin();
        for (it = elements.begin(); it != elements.end(); ++it)
            printf("%f %d\n", ((*it)->time), ((*it)->priority));
    }

    double Simulator::now() {
        return time;
    }

    void Simulator::doAllEvents() {
        while (events.elements.size() > 0) {
            time = events.elements.front()->time;
            Event *tmp = events.elements.front();
            events.elements.erase(events.elements.begin());
            tmp->execute(this);
        }
    }
}