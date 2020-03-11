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
private:
    string nombre;
    LinkedList<int> *puntajes;
};

Jugador::Jugador(){

}

void Jugador::setNombre(string arg1) {
    this->nombre=arg1;
}

void Jugador::addpuntaje(int arg1) {
    if(puntajes->getSize()==0){
        puntajes->addBegin(arg1);
    }
    else{
        for(int i=0; i<puntajes->getSize();i++){
            if(puntajes->getXNode(i)>arg1){
                if(i==0){
                    puntajes->addBegin(arg1);
                }
                else{
                    puntajes->addX(arg1,i-1);
                }
            }
            else if(i==puntajes->getSize()-1){
                puntajes->addEnd(arg1);
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


#endif //PROYECTO1EDD_JUGADOR_H
