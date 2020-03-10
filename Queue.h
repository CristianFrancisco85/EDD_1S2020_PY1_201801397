#ifndef PROYECTO1EDD_QUEUE_H
#define PROYECTO1EDD_QUEUE_H


#include "Nodo.h"

template <class T>
class Queue {
public:
    Queue();
    void push(T Value);
    T pop();
    T pick();
    void clearQueue();
    int getSize();
private:
    Nodo<T> *Head;
    int Size;

};


template <class T>
Queue<T>::Queue(){
    this->Head=NULL;
    this->Size=0;
}

template <class T>
void Queue<T>::push(T Value) {

    Nodo<T> *NewNode = new Nodo<T>;
    NewNode->setNodoValue(Value);
    if(this->Head==NULL){
        this->Head=NewNode;
    }
    else{
        this->Iterador = this->Head;
        while (this->Iterador->getNextNodo() != NULL) {
            this->Iterador = this->Iterador->getNextNodo();
        }
        this->Iterador->setNextNodo(NewNode);
    }
    this->Size++;
}

template <class T>
T Queue<T>::pop() {
    try {
        if (this->Head == NULL) {
            throw -1;
        }
        else {
            T Value = this->Head->getNodoValue();
            this->Head=this->Head->getNextNodo();
            this->Size--;
            return Value;
        }
    }
    catch (int x){
        std::cout<<"ERROR -- LA PILA ESTA VACIA , NO ES POSIBLE REALIZAR pop()";
        Nodo<T> NullNodo;
        return NullNodo.getNodoValue();
    }
}

template <class T>
T Queue<T>::pick() {
    Nodo<T> NullNodo;
    try {
        if (this->Head == NULL) {
            throw -1;
        }
        else {
            T Value = this->Head->getNodoValue();
            return Value;
        }
    }
    catch (int x){
        std::cout<<"ERROR -- LA PILA ESTA VACIA , NO ES POSIBLE REALIZAR pick()";
        return NullNodo.getNodoValue();
    }
}

template <class T>
void Queue<T>::clearQueue() {
    this->Head=NULL;
    this->Size=0;
}

template <class T>
int Queue<T>::getSize() {
    return  this->Size;
}


#endif //PROYECTO1EDD_QUEUE_H
