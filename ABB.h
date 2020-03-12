//
// Created by Cristian on 11/03/20.
//

#ifndef PROYECTO1EDD_ABB_H
#define PROYECTO1EDD_ABB_H

#include "Jugador.h"
#include "Queue.h"

class ABB {
public:
    ABB();
    Jugador* getRaiz();
    bool addJugador(Jugador arg1);
    void preOrden(Jugador *Raiz,Queue<Jugador*> *Cola);
    void inOrden(Jugador *Raiz,Queue<Jugador*> *Cola);
    void postOrden(Jugador *Raiz,Queue<Jugador*> *Cola);

private:
    Jugador *raiz;
    Jugador *Iterador;
};

ABB::ABB(){
    raiz=NULL;
    Iterador=NULL;
}

Jugador* ABB::getRaiz() {
    return this->raiz;
}

bool ABB::addJugador(Jugador arg1) {

    Jugador *NewJugador = new Jugador();
    NewJugador->setNombre(arg1.getNombre());

    if(raiz==NULL){
        raiz=NewJugador;
        return true;
    }
    else{
        Iterador=raiz;
        while(Iterador!=NULL){
            if(arg1.getNombre().compare(Iterador->getNombre())==0){
                return false;
            }
            else if(arg1.getNombre().compare(Iterador->getNombre())<0){
                if(Iterador->getIzquierda()==NULL){
                    Iterador->setIzquierda(NewJugador);
                    return true;
                }
                else{
                    Iterador=Iterador->getIzquierda();
                }
            }
            else if(arg1.getNombre().compare(Iterador->getNombre())>0){
                if(Iterador->getDerecha()==NULL){
                    Iterador->setDerecha(NewJugador);
                    return true;
                }
                else{
                    Iterador=Iterador->getDerecha();
                }
            }
        }

    }
}

void ABB::preOrden(Jugador *Raiz, Queue<Jugador*> *Cola) {
    if(Raiz!=NULL){
        Cola->push(Raiz);
        preOrden(Raiz->getIzquierda(),Cola);
        preOrden(Raiz->getDerecha(),Cola);
    }
}

void ABB::inOrden(Jugador *Raiz, Queue<Jugador*> *Cola) {
    if(Raiz!=NULL){
        inOrden(Raiz->getIzquierda(),Cola);
        Cola->push(Raiz);
        inOrden(Raiz->getDerecha(),Cola);
    }
}

void ABB::postOrden(Jugador *Raiz, Queue<Jugador*> *Cola) {
    if(Raiz!=NULL){
        postOrden(Raiz->getIzquierda(),Cola);
        postOrden(Raiz->getDerecha(),Cola);
        Cola->push(Raiz);
    }
}



#endif //PROYECTO1EDD_ABB_H
