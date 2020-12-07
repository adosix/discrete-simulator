
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
class Event  {
public:
    double time=0.0;
    int priority=0;
    double length=0;
    bool lessThan(double y);
    bool greaterThan(int x);
    bool equal(double y);
    virtual void execute(Simulator *simulate) {};
};




class ListQueue{
    public: 
    std::vector<Event *> elements;

    void insert(Event *e);
    void printlist();
};

class Simulator{
public:
    ListQueue events;
    double time=0.0;//same name with event, take care
    double now();
    void doAllEvents() ;
    };
    
class Random_g{
    static const std::mt19937 gen(std::random_device());

    public:
        int distributions(int type);
        double exp(double m);
        double normal(double m, double q);
        double uniform(double a, double b);
        double rand();

};
template<class T>
class Queue_t{

public:


    std::vector<T*> objects;
    void insert( T *obj );

    T* remove();
    int size();
};
template<class T>
class Facility{

public:
    bool busy_flag=false;
    Queue_t<T> queue;
    bool Busy();
    virtual void Seize(sim::Simulator *simulate, T *obj );
    virtual void Seize(sim::Simulator *simulate, T *obj , float time_v);
    virtual void Release(sim::Simulator *simulate);
};
template<class T>
class Store{
    public:
        int capacity;
        Store(int capacity_v);
        bool Busy();
        void Seize();
        void Release();
    };
}
#endif