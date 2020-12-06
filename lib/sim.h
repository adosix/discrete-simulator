
#ifndef SIM_H
#define SIM_H

#include <list>
#include <vector>
#include <queue>
#include <functional>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <random>
#include <math.h>
#include <queue>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>

#include <queue>
#include <algorithm>
#include <vector>

namespace sim {

    class Simulator;

    class Event {
    public:
        double time = 0.0;
        int priority = 0;
        double length = 0;

        bool lessThan(double y);

        bool greaterThan(int x);

        bool equal(double y);

        virtual void execute(Simulator *simulate) {};
    };


    class ListQueue {
    public:
        std::vector<Event *> elements;

        void insert(Event *e);

        void printlist();
    };

    class Simulator {
    public:
        ListQueue events;
        double time = 0.0;//same name with event, take care
        double end_time=0.0;
        double now();
        void doAllEvents();
    };

    class Random_g {
        static const std::mt19937 gen(std::random_device());

    public:
        int distributions();

        double exp(double m);

        double normal(double m, double q);

        double uniform(double a, double b);

        double rand();

    };

    template<class T>
    class Queue_t {

    public:


        std::vector<T *> objects;

        void insert(T *obj);

        T *remove();

        int size();
    };

    template<class T>
    class Facility {

    public:
        bool busy_flag = false;
        Queue_t<T> queue;
        void Seize(sim::Simulator *simulate, T *obj);
        void Release(sim::Simulator *simulate);
    };
    template<class T>
    struct Compare {
        bool operator()(T  *p1, T *p2)
        {
            // return "true" if "p1" is ordered
            // before "p2", for example:
            return p1->priority < p2->priority;
        }
    };
    template<class T>
    class Storage {
    public:
        std::priority_queue<T *, std::vector<T *>,Compare<T>> queue;
        int capacity = 1;
        int current_capacity = 0;



        bool Full(T *obj);
        void Enter(sim::Simulator *simulate, T *obj);

        void Leave(sim::Simulator *simulate, T *obj);
    };
}
#include "sim.tcc"
#endif