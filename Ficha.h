//
// Created by Cristian on 10/03/20.
//

#ifndef PROYECTO1EDD_FICHA_H
#define PROYECTO1EDD_FICHA_H


class Ficha {
public:
    Ficha();
    Ficha(char arg1 , int arg2);
    void setLetra(char arg1);
    void setPuntos(int arg1);
    char getLetra();
    int getPuntos();
private:
    char letra;
    int puntos;
};

Ficha::Ficha() {

}

Ficha::Ficha(char arg1 , int arg2) {
    this->letra=arg1;
    this->puntos=arg2;
}

void Ficha::setLetra(char arg1) {
    this->letra=arg1;
}

void Ficha::setPuntos(int arg1) {
    this->puntos=arg1;
}

char Ficha::getLetra() {
    return this->letra;
}

int Ficha::getPuntos() {
    return  this->puntos;
}



#endif //PROYECTO1EDD_FICHA_H
