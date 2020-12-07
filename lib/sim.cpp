#include "sim.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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

    double Random_g::exp(double lambda){
        double u;
        std::random_device rd;
        std::mt19937 gen(rd());
        u = gen() / (std::mt19937::max() + 1.0);
        return -log(1- u) / lambda;
    }

    double Random_g::exp_m(double mean){
        return exp(1/mean);
    }

    double Random_g::normal(double m, double q){
         return (sqrt(-2*log(rand())) * cos(2*M_PI*rand()))*q+m;
    }

    //* uniform distribution, (0..1] */
    double Random_g::rand(){
        std::random_device rd;
        std::mt19937 gen(rd());
        //uniform between 0-1 is random
        return (gen()+1.0)/(std::mt19937::max()+1.0);
    }

    double Random_g::uniform(double a, double b){
        std::random_device rd;
        std::mt19937 gen(rd());        
        std::uniform_real_distribution<> d(a, b);
        return d(gen);
    }

    void Random_g::distributions(int type){
        const int nrolls=10000;  // number of experiments
        const int nstars=100;    // maximum number of stars to distribute
        const int nintervals=10; // number of intervals
        int p[nintervals]={};
        std::map<int, int> result_set;
        
        if (type==1)
        {
            std::cout << "exponential distribution exp(10)" << std::endl;
            std::map<int, int> hist{};
            for (int i=0; i<nrolls; ++i) {
                double n =exp_m(10);
                //printf("%.2f, ", n);
                ++hist[n];
            }
            for(auto p : hist) {
                std::cout << std::setw(2)
                << p.first << ' ' << std::string(p.second/10, '*') << '\n';
            }
        }
        else if (type==2)
        {
            std::cout << "normal distribution normal(50,2)" << std::endl;
            std::map<int, int> hist{};
            for (int i=0; i<nrolls; ++i) {
                float n = normal(10,2.5);
                ++hist[n];
            }
            for(auto p : hist) {
                std::cout << std::setw(2)
                << p.first << ' ' << std::string(p.second/30, '*') << '\n';
            }
            
        }
        else if (type==3)
        {
            std::cout << "uniform distribution uniform(0,10)" << std::endl;
            std::map<int, int> hist{};
            for (int i=0; i<nrolls; ++i) {
                ++hist[uniform(1,10.99)];
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


    double Simulator::now() {
        return time;
    }

    void Simulator::doAllEvents() {
        while (events.elements.size() > 0) {
            time = events.elements.front()->time;
            if(end_time <= time){
                return;
            }
            Event *tmp = events.elements.front();
            events.elements.erase(events.elements.begin());
            tmp->execute(this);
        }
    }
}