//
// Created by hater on 6. 12. 2020.
//

#ifndef SIM_H
#endif
namespace sim {


    template <class T>
    void sim::Facility<T>::Release(sim::Simulator *simulate) {

        busy_flag=false;
        if (queue_f.size()>0) {
            T *obj= queue_f.front();
            queue_f.pop();
            this->Seize(simulate, obj);
        }
    };


    template <class T>
    void sim::Facility<T>::Seize(sim::Simulator *simulate, T *obj) {
        if(busy_flag){
            queue_f.push(obj);
        }
        else{
            busy_flag=true;

            obj->time = ((sim::Simulator*)simulate)->now() + obj->length;
            simulate->events.insert(obj);
        }
    };
    template <class T>
    bool sim::Storage<T>::Full(T *obj){

        if (capacity < (current_capacity+obj->value) || queue.size()>0){
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
            T *tmp= queue.top();
            if(!(capacity < (current_capacity+tmp->value))){
                queue.pop();
                current_capacity += tmp->value;

                tmp->time = ((sim::Simulator*)simulate)->now() + tmp->length;
                simulate->events.insert(tmp);
            }
        }

    };
}
 //DISCRETE_SIMULATOR_SIM_TCC
