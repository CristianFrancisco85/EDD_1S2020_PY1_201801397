
#ifndef PROYECTO1EDD_DOUBLECIRCULARLINKEDLIST_H
#define PROYECTO1EDD_DOUBLECIRCULARLINKEDLIST_H

#include "Nodo.h"


template <class T>
class DoubleCircularLinkedList {
public:
    DoubleCircularLinkedList();
    void addBegin(T Value);
    void addEnd(T Value);
    void addX(T Value , int x);
    T getFirst();
    T getLast();
    int getSize();
    T getXNode(int x);
    void deleteXNode(int x);

private:
    Nodo<T> *Head;
    Nodo<T> *Iterador;
    int Size;
};

template <class T>
DoubleCircularLinkedList<T>::DoubleCircularLinkedList(){
    this->Head=NULL;
    this->Iterador=NULL;
    this->Size=0;
}

template <class T>
T DoubleCircularLinkedList<T>::getFirst() {

    try {
        if(this->Head == NULL){
            throw -1;
        }
        else{
            this->Iterador=this->Head;
            return this->Iterador->getNodoValue();
        }
    }
    catch (int x){
        Nodo<T> NullNodo;
        std::cout<<"ERROR -- LA LISTA ESTA VACIA , NO ES POSIBLE REALIZAR getFirst()";
        return NullNodo.getNodoValue();
    }

}

template <class T>
T DoubleCircularLinkedList<T>::getLast() {

    try {
        if (this->Head == NULL) {
            throw -1;
        } else {
            this->Iterador = this->Head;
            while (this->Iterador->getNextNodo() != this->Head) {
                this->Iterador = this->Iterador->getNextNodo();
            }
            return this->Iterador->getNodoValue();
        }
    }
    catch (int x){
        Nodo<T> NullNodo;
        std::cout<<"ERROR -- LA LISTA ESTA VACIA , NO ES POSIBLE REALIZAR getLast()";
        return NullNodo.getNodoValue();
    }
}

template <class T>
T DoubleCircularLinkedList<T>::getXNode(int x ){

    try {
        if (this->Head == NULL) {
            throw -1;
        } else {
            this->Iterador = this->Head;
            try {
                for (int i = 0; i < x; i++) {
                    if (this->Iterador->getNextNodo() == this->Head) {
                        throw -1;
                    } else {
                        this->Iterador = this->Iterador->getNextNodo();
                    }
                }
                return this->Iterador->getNodoValue();
            }
            catch (int x1) {
                Nodo<T> NullNodo;
                std::cout << "ERROR -- NO EXISTE NODO EN LA POSICION " <<x;
                return NullNodo.getNodoValue();
            }
        }
    }
    catch (int x){
        Nodo<T> NullNodo;
        std::cout<<"ERROR -- LA LISTA ESTA VACIA , NO ES POSIBLE REALIZAR getXNode()";
        return NullNodo.getNodoValue();
        //return NULL;
    }
}


template <class T>
void DoubleCircularLinkedList<T>::addBegin(T Value) {
    Nodo<T> *NewNode = new Nodo<T>;
    NewNode->setNodoValue(Value);
    if(this->Head==NULL){
        this->Head=NewNode;
        this->Head->setNextNodo(NewNode);
        this->Head->setPrevNodo(NewNode);
    }
    else {
        NewNode->setPrevNodo(this->Head->getPrevNodo());
        this->Head->getPrevNodo()->setNextNodo(NewNode);
        this->Head->setPrevNodo(NewNode);
        NewNode->setNextNodo(this->Head);
        this->Head = NewNode;
    }
    this->Size++;
}

template <class T>
void DoubleCircularLinkedList<T>::addEnd(T Value) {

    Nodo<T> *NewNode = new Nodo<T>;
    NewNode->setNodoValue(Value);
    if(this->Head==NULL){
        this->Head=NewNode;
        this->Head->setNextNodo(NewNode);
        this->Head->setPrevNodo(NewNode);
    }
    else{
        this->Iterador=this->Head;
        while(this->Iterador->getNextNodo()!= this->Head){
            this->Iterador = this->Iterador->getNextNodo();
        }
        this->Iterador->setNextNodo(NewNode);
        NewNode->setPrevNodo(this->Iterador);
        NewNode->setNextNodo(this->Head);
        this->Head->setPrevNodo(NewNode);
    }
    this->Size++;
}

template <class T>
void DoubleCircularLinkedList<T>::addX(T Value , int x ) {
    if (this->Head == NULL) {
        addBegin(Value);
    }
    else if(x<=0 ){
        addBegin(Value);
    }
    else if(x>=(this->Size)-1){
        addEnd(Value);
    }
    else {
        Nodo<T> *NewNode = new Nodo<T>;
        NewNode->setNodoValue(Value);
        this->Iterador = this->Head;
        try {
            for (int i = 0; i < x; i++) {
                if (this->Iterador->getNextNodo() == NULL) {
                    throw -1;
                } else {
                    this->Iterador = this->Iterador->getNextNodo();
                }
            }

            this->Iterador->getPrevNodo()->setNextNodo(NewNode);
            NewNode->setPrevNodo(this->Iterador->getPrevNodo());
            this->Iterador->setPrevNodo(NewNode);
            NewNode->setNextNodo(this->Iterador);
            this->Size++;
        }
        catch (int x1) {
            std::cout << "ERROR -- NO EXISTE NODO EN LA POSICION " << x <<" PARA PODER INSERTAR"<<std::endl;
        }
    }
}

template <class T>
void DoubleCircularLinkedList<T>::deleteXNode(int x) {

    try {

        if (this->Head == NULL) {
            throw -1;
        }
        else {
            this->Iterador = this->Head;
            try {
                for (int i = 0; i < x; i++) {
                    if (this->Iterador->getNextNodo() == this->Head) {
                        throw -1;
                    } else {
                        this->Iterador = this->Iterador->getNextNodo();
                    }
                }
                if(this->Iterador->getPrevNodo() == this->Head && this->Iterador->getNextNodo() == this->Head){
                    this->Head=NULL;
                }
                else if(this->Iterador->getPrevNodo() == this->Head->getPrevNodo() ){
                    this->Iterador->getNextNodo()->setPrevNodo(this->Head->getPrevNodo());
                    this->Head->getPrevNodo()->setNextNodo(this->Iterador->getNextNodo());
                    this->Head=this->Iterador->getNextNodo();
                }
                else if(this->Iterador->getNextNodo() == this->Head){
                    this->Iterador->getPrevNodo()->setNextNodo(this->Head);
                    this->Head->setPrevNodo(this->Iterador->getPrevNodo());
                }
                else {
                    this->Iterador->getPrevNodo()->setNextNodo(this->Iterador->getNextNodo());
                    this->Iterador->getNextNodo()->setPrevNodo(this->Iterador->getPrevNodo());
                }
                this -> Size--;
                delete this->Iterador;
                this->Iterador = NULL;
            }
            catch (int x1) {
                std::cout << "ERROR -- NO EXISTE NODO EN LA POSICION " << x;
            }
        }
    }
    catch (int x){
        std::cout<<"ERROR -- LA LISTA ESTA VACIA , NO ES POSIBLE REALIZAR deleteXNode()";
    }
}

template <class T>
int DoubleCircularLinkedList<T>::getSize() {
    return this->Size;
}


#endif //PROYECTO1EDD_DOUBLECIRCULARLINKEDLIST_H
