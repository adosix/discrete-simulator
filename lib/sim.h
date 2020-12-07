
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
    /*! \brief Class Event describes an event in module.
     */
    class Event {
    public:
        /*!
         * \param time double setting when is the event finished
         * \param priority int setting the priority of the event
         * \param length double setting the length of the event; needed to initialize when the event goes to facility or storage
         * \param  value int how much capacity in store does event take
        */
        double time = 0.0;
        int priority = 0;
        double length = 0;
        int value = 0;
        /*! \brief Compares time with double y. Used in Simulator to sort events by time
        * \param y double time to be compared
         * \returns true if time is less than y
       */
        bool lessThan(double y);
        /*! \brief Compares priority with in x. Used in Simulator to sort events by priority when time is the same.
        * \param x int priority to be compared
         * \returns true if priority is greater than x
       */
        bool greaterThan(int x);
        /*! \brief Compares time with double y. Used in Simulator to sort events by time
        * \param y double time to be compared
         * \returns true if time is equal to y
       */
        bool equal(double y);
        /*! \brief Behaviour of the event. Every child class of the class Event has to implement its own execute.
        * \param *simulate  Simulator simulator that simulates an example
       */
        virtual void execute(Simulator *simulate) {};
    };

    /*! \brief Class ListQueue that stores events in simulator in the vector of <Event *>
     */
    class ListQueue {
    public:
        /*!
         * \param elements std::vector<Event *> stores events of simulator in ascending order  by their time.
        */
        std::vector<Event *> elements;
        /*! \brief insert Event *e in the std::vector<Event *> elements
         * \param *e Event to be inserted into elements
        */
        void insert(Event *e);

    };
    /*! \brief Class Simulator that is used to simulate whole experiment
     *
     *  Simulation ends when its time reaches end_time or when all the events are finished.
     *  Simulation time is time of the last executed event
     */
    class Simulator {
    public:
        /*!
         * \param events ListQueue to store events of simulator.
         *  \param time  double time of the simulation.
         *  \param end_time  double is final time of the simulation.
        */
        ListQueue events;
        double time = 0.0;
        double end_time=0.0;
        /*! \brief returns time of the Simulator
         * \returns  time of the Simulator
        */
        double now();
        /*! \brief execute all the events in Simulator
         *  Sets time of Simulator to the time of he last executed event.
         *  Program ends when the time is greater or equal to the end_time.
         *  Executes event at the front of ListQueue::elements and erase this event from ListQueue::elements.
        */
        void doAllEvents();
        /*! \brief start of the Simulation
        * Initialize sim::ListQueue events; sets end_time(MAX=double_max)
         * Insert into events at least one Event
         * Call doAllEvents()
       */
        virtual void start(){};
    };
    /*! \brief Class Random_g generates random values
     * Can generate exponential normal, uniform distribution and random distribution in rage [0,1]
   *
   */
    class Random_g {
        static const std::mt19937 gen(std::random_device());

    public:
        int distributions();

        double exp(double m);

        double normal(double m, double q);

        double uniform(double a, double b);

        double rand();

    };
    /*! \brief Class Facility that represents device in a simulation that its capacity is one event
   *
   */
    template<class T>
    class Facility {

    public:
        /*!
         * \param bool busy_flag it is set to True when the device is taken
         *  \param queue_f std::queue<T *> represents queue in front of device
        */
        bool busy_flag = false;
        std::queue<T *> queue_f;
        /*! \brief *obj T occupy Facility
        * \param *obj T that will occupy Facility
         * \param *simulate  Simulator simulator that simulates an example
         *
         * if the busy flag is false, then *obj will occupy Facility and the busy_flag is set to true and the time of obj is set to simulate::time + obj::length
         *  otherwise push *obj to queue_f
         */
        void Seize(sim::Simulator *simulate, T *obj);
        /*! \brief release the Facility
        * \param *simulate  Simulator simulator that simulates an example
        * bus_flag is set to false
         * if queue_f has size()>0 , get first element as T *obj, erase this element from queue_f and the function Seize(sim::Simulator *simulate, T *obj) is called
        */

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
        /*!
        * \param queue std::priority_queue<T *, std::vector<T *>,Compare<T>> represents priority queue of device based on its priority
        *  \param capacity int capacity of device
         *  \param surrent_capacity int how much capacity is taken
       */
        std::priority_queue<T *, std::vector<T *>,Compare<T>> queue;
        int capacity = 1;
        int current_capacity = 0;

        /*! \brief Checks if *obj can fit in storage
        * \param *obj T that will occupy Storage
         *
         * \returns true if *obj can not be fit in Storage or there is other *obj in queue, otherwise false
         */

        bool Full(T *obj);
        /*! \brief *obj T enters Storage
        * \param *obj T that will enter Storage
         * \param *simulate  Simulator simulator that simulates an example
         *
         * if the *obj can be fit in storage, Storage::current_capacity is incremented bu *obj::vaulue
         * obj::time is set to Simulate::time+obj::length
         * and obj is inserted into simulate::events
         * otherwise *obj is pushed to the queue
         */
        void Enter(sim::Simulator *simulate, T *obj);
        /*! \brief *obj leaves from storage
        * \param *simulate  Simulator simulator that simulates an example
         * \param *obj T that leaves Storage
         *
         * Storage::current_capacity is decremented by *obj::value
         * if the queue is non_empty then object from the top of the queue enters if it can fit
        */
        void Leave(sim::Simulator *simulate, T *obj);
    };
}
#include "sim.tcc"
#endif