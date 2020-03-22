//
// Created by Cristian on 10/03/20.
//

#ifndef PROYECTO1EDD_JUGADOR_H
#define PROYECTO1EDD_JUGADOR_H

#include <iostream>
#include "LinkedList.h"

using namespace std;

class Jugador {
public:
    Jugador();
    void setNombre(string arg1);
    string getNombre();
    void addpuntaje(int arg1);
    LinkedList<int>* getPuntajes();
    void setIzquierda(Jugador *arg1);
    void setDerecha(Jugador *arg1);
    Jugador* getIzquierda();
    Jugador* getDerecha();
private:
    Jugador *izquierda;
    Jugador *derecha;
    string nombre;
    LinkedList<int> *puntajes;
};

Jugador::Jugador(){
    izquierda=NULL;
    derecha=NULL;
    puntajes= new LinkedList<int>();
}

void Jugador::setNombre(string arg1) {
    this->nombre=arg1;
}

void Jugador::addpuntaje(int arg1) {
    if(puntajes->getSize()==0){
        puntajes->addBegin(arg1);
    }
    else{
        int SizeLista=puntajes->getSize();
        for(int i=0; i<SizeLista;i++){
            if(arg1>puntajes->getXNode(i)){
                puntajes->addX(arg1,i);
                break;
            }
            else if(i==puntajes->getSize()-1){
                puntajes->addEnd(arg1);
                break;
            }
        }
    }
}

LinkedList<int>* Jugador::getPuntajes() {
    return this->puntajes;
}

string Jugador::getNombre() {
    return this->nombre;
}

void Jugador::setIzquierda(Jugador *arg1) {
    this->izquierda=arg1;
}

void Jugador::setDerecha(Jugador *arg1) {
    this->derecha=arg1;
}

Jugador* Jugador::getIzquierda() {
    return this->izquierda;
}

Jugador* Jugador::getDerecha() {
    return this->derecha;
}


#endif //PROYECTO1EDD_JUGADOR_H
