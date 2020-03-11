//
// Created by Cristian on 10/03/20.
//

#ifndef PROYECTO1EDD_CASILLA_H
#define PROYECTO1EDD_CASILLA_H


class Casilla {
public:
    Casilla();
    void setX(int arg1);
    void setY(int arg1);
    void setMultiplicador(int arg1);
    int getX();
    int getY();
    int getMultiplicador();
private:
    int x;
    int y;
    int multiplicador;

};

Casilla::Casilla() {

}

void Casilla::setX(int arg1) {
    this->x=arg1;
}

void Casilla::setY(int arg1) {
    this->y=arg1;
}

void Casilla::setMultiplicador(int arg1) {
    this->multiplicador=arg1;
}

int Casilla::getX() {
    return this->x;
}

int Casilla::getY() {
    return this->y;
}

int Casilla::getMultiplicador() {
    return this->multiplicador;
}


#endif //PROYECTO1EDD_CASILLA_H
