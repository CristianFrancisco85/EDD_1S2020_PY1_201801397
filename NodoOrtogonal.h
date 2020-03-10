#ifndef PROYECTO1EDD_NODOORTOGONAL_H
#define PROYECTO1EDD_NODOORTOGONAL_H

#include <iostream>

template <class T>
class NodoOrtogonal {

public:
    NodoOrtogonal();
    T getNodoValue();
    int getIndice();
    void setIndice(int arg1);
    int getX();
    void setX(int arg1);
    int getY();
    void setY(int arg1);
    void setNodoValue(T TempValor);
    void setLeftNodo(NodoOrtogonal<T>* TempNodo);
    void setRightNodo(NodoOrtogonal<T>* TempNodo);
    void setUpNodo(NodoOrtogonal<T>* TempNodo);
    void setDownNodo(NodoOrtogonal<T>* TempNodo);
    NodoOrtogonal<T>* getLeftNodo();
    NodoOrtogonal<T>* getRightNodo();
    NodoOrtogonal<T>* getUpNodo();
    NodoOrtogonal<T>* getDownNodo();

private:
    //Apuntadores
    NodoOrtogonal<T> *LeftNodo;
    NodoOrtogonal<T> *RightNodo;
    NodoOrtogonal<T> *UpNodo;
    NodoOrtogonal<T> *DownNodo;
    T Valor;
    int Indice;
    int coorX;
    int coorY;
};


//Constructor
template <class T>
NodoOrtogonal<T>::NodoOrtogonal() {
    this->LeftNodo=NULL;
    this->RightNodo=NULL;
    this->UpNodo=NULL;
    this->DownNodo=NULL;
    this->Indice=-1;
}

//Recibe un Apuntador Nodo
template <class T>
void NodoOrtogonal<T>::setLeftNodo(NodoOrtogonal* TempNodo) {
    this->LeftNodo = TempNodo;
}

//Recibe un Apuntador Nodo
template <class T>
void NodoOrtogonal<T>::setRightNodo(NodoOrtogonal* TempNodo) {
    this->RightNodo = TempNodo;
}

//Recibe un Apuntador Nodo
template <class T>
void NodoOrtogonal<T>::setUpNodo(NodoOrtogonal* TempNodo) {
    this->UpNodo = TempNodo;
}

//Recibe un Apuntador Nodo
template <class T>
void NodoOrtogonal<T>::setDownNodo(NodoOrtogonal* TempNodo) {
    this->DownNodo = TempNodo;
}

//Recibe un Objeto T
template <class T>
void NodoOrtogonal<T>::setNodoValue(T TempValor) {
    this->Valor = TempValor;
}

//Retorna un Objeto T
template <class T>
T NodoOrtogonal<T>::getNodoValue() {
    return this->Valor;
}

// Retorna un Apuntador Nodo
template <class T>
NodoOrtogonal<T>* NodoOrtogonal<T>::getLeftNodo() {
    return this->LeftNodo;
}

// Retorna un Apuntador Nodo
template <class T>
NodoOrtogonal<T>* NodoOrtogonal<T>::getRightNodo() {
    return this->RightNodo;
}

// Retorna un Apuntador Nodo
template <class T>
NodoOrtogonal<T>* NodoOrtogonal<T>::getUpNodo() {
    return this->UpNodo;
}

// Retorna un Apuntador Nodo
template <class T>
NodoOrtogonal<T>* NodoOrtogonal<T>::getDownNodo() {
    return this->DownNodo;
}

template <class T>
int NodoOrtogonal<T>::getIndice() {
    return this->Indice;
}

template <class T>
void NodoOrtogonal<T>::setIndice(int arg1) {
    this->Indice=arg1;
}

template <class T>
int NodoOrtogonal<T>::getX() {
    return this->coorX;
}

template <class T>
void NodoOrtogonal<T>::setX(int arg1) {
    this->coorX=arg1;
}

template <class T>
int NodoOrtogonal<T>::getY() {
    return this->coorY;
}

template <class T>
void NodoOrtogonal<T>::setY(int arg1) {
    this->coorY=arg1;
}


#endif //PROYECTO1EDD_NODOORTOGONAL_H
