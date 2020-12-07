//
// Created by hater on 6. 12. 2020.
//

#ifndef SIM_H
#endif
namespace sim {

    template <class T>
    int Queue_t<T>::size(){

        return objects.size();

    };
    template <class T>
    T* sim::Queue_t<T>::remove(){

        T *obj = objects.front();
        objects.erase(objects.begin());
        return obj;

    };
    template <class T>
    void sim::Queue_t<T>::insert(T *obj){

        objects.push_back(obj);

    };

    template <class T>
    void sim::Facility<T>::Release(sim::Simulator *simulate) {

        busy_flag=false;
        if (queue.size()>0) {
            T *obj= queue.remove();
            this->Seize(simulate, obj);
        }
    };


    template <class T>
    void sim::Facility<T>::Seize(sim::Simulator *simulate, T *obj) {
        if(busy_flag){
            queue.insert(obj);
        }
        else{
            busy_flag=true;

            obj->time = ((sim::Simulator*)simulate)->now() + obj->length;
            simulate->events.insert(obj);
        }
    };
    template <class T>
    bool sim::Storage<T>::Full(T *obj){

        if (capacity < (current_capacity+obj->value)){
            return true;
        }
        else{
            return false;
        }

    }
    template <class T>
    void sim::Storage<T>::Enter(sim::Simulator *simulate, T *obj){
        if(Full(obj)){

            queue.push(obj);
        }
        else{
            current_capacity += obj->value;

            obj->time = ((sim::Simulator*)simulate)->now() + obj->length;
            simulate->events.insert(obj);
        }


    };
    template <class T>
    void sim::Storage<T>::Leave(sim::Simulator *simulate, T *obj){
        current_capacity  -= obj->value;
        if (queue.size()>0) {
            T *obj= queue.top();
            if(!Full(obj)){
                queue.pop();
                this->Enter(simulate, obj);
            }
        }

    };
}
 //DISCRETE_SIMULATOR_SIM_TCC
