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

    int Random_g::distributions(int type){
        const int nrolls=100000;  // number of experiments
        const int nstars=100;    // maximum number of stars to distribute
        const int nintervals=10; // number of intervals
        int p[nintervals]={};
        std::map<int, int> result_set;
        
        if (type==1)
        {
            std::cout << "exponential distribution exp(1)" << std::endl;
            for (int i=0; i<nrolls; ++i) {
                double n =exp(3.5);
                if (n<1.0) ++p[int(nintervals*n)];
            }

            for (int i=0; i<nintervals; ++i) {
                printf("%.2f - %.2f: ",float(i)/nintervals,float(i+1)/nintervals);
                std::cout << std::string(p[i]*nstars/nrolls,'*') << std::endl;
            }
        }
        else if (type==2)
        {
            std::cout << "normal distribution normal(50,2)" << std::endl;
             std::map<int, int> hist{};
            for (int i=0; i<nrolls; ++i) {
                ++hist[std::round(normal(5,2.5))];
            }
            for(auto p : hist) {
            std::cout << std::setw(2)
            << p.first << ' ' << std::string(p.second/300, '*') << '\n';
            }
            
        }
        else if (type==3)
        {
            std::cout << "uniform distribution uniform(0,10)" << std::endl;
            std::map<int, int> hist{};
            for (int i=0; i<nrolls; ++i) {
                ++hist[uniform(1,10)];
            }
            for(auto p : hist) {
            std::cout << std::setw(2)
            << p.first << ' ' << std::string(p.second/300, '*') << '\n';
            }
            
        }
        else if (type==4)
        {
            
            std::cout << "random distribution rand()" << std::endl;
            for (int i=0; i<nrolls; ++i) {
                double n =rand();
                if (n<1.0) ++p[int(nintervals*n)];
            }

            for (int i=0.0; i<nintervals; ++i) {
                printf("%.2f - %.2f: ",float(i)/nintervals,float(i+1)/nintervals);
                std::cout << std::string(p[i]*nstars/nrolls,'*') << std::endl;
            }
        }
        else
        {
            exit(1);
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
            if(end_time <= time){
                exit(0);
            }
            Event *tmp = events.elements.front();
            events.elements.erase(events.elements.begin());
            tmp->execute(this);
        }
    }
}