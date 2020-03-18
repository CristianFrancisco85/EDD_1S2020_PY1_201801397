#ifndef PROYECTO1EDD_MATRIZDISPERSA_H
#define PROYECTO1EDD_MATRIZDISPERSA_H

#include "NodoOrtogonal.h"

template <class T>
class MatrizDispersa {

public:
    MatrizDispersa();
    void addInXY(int x,int y,T Value);
    void deleteInXY(int x,int y);
    NodoOrtogonal<T>* getInXY(int x,int y);

private:
    NodoOrtogonal<T> *Head;
    NodoOrtogonal<T> *Iterador;
};

template <class T>
MatrizDispersa<T>::MatrizDispersa(){
    this->Head=new NodoOrtogonal<T>();
}

template <class T>
void MatrizDispersa<T>::addInXY(int x, int y, T Value) {

    NodoOrtogonal<T> *NewNodo = new NodoOrtogonal<T>;
    NewNodo->setNodoValue(Value);
    NewNodo->setX(x);
    NewNodo->setY(y);

    // SI LOS INDICES DE COLUMNAS Y FILAS ESTAN VACIOS
    if(this->Head->getRightNodo()==NULL && this->Head->getDownNodo()==NULL){

        //SE CREA NUEVO INDICE DE COLUMNA
        NodoOrtogonal<T> *NewIndiceX = new NodoOrtogonal<T>;
        NewIndiceX->setIndice(x);
        NewIndiceX->setLeftNodo(this->Head);
        this->Head->setRightNodo(NewIndiceX);

        //SE CREA NUEVO INDICE DE FILA
        NodoOrtogonal<T> *NewIndiceY = new NodoOrtogonal<T>;
        NewIndiceY->setIndice(y);
        NewIndiceY->setUpNodo(this->Head);
        this->Head->setDownNodo(NewIndiceY);

    }

    //SE BUSCA INDICE DE COLUMNA
    this->Iterador=this->Head->getRightNodo();
    //MIENTRAS QUE EL INDICE SEA DIFERENTE DE X
    while(this->Iterador->getIndice()!= x){
        this->Iterador=this->Iterador->getRightNodo();//AQUI ESTA EL ERROR
        //SI EL INDICE ES MAYOR QUE X
        if(this->Iterador->getIndice() > x){
            //INSERTAR NUEVO INDICE A LA IZQUIERDA DEL ITERADOR
            NodoOrtogonal<T> *NewIndice = new NodoOrtogonal<T>;
            NewIndice->setIndice(x);
            this->Iterador->getLeftNodo()->setRightNodo(NewIndice);
            NewIndice->setLeftNodo(this->Iterador->getLeftNodo());
            this->Iterador->setLeftNodo(NewIndice);
            NewIndice->setRightNodo(this->Iterador);
            //ACTUALIZAR ITERADOR
            this->Iterador=NewIndice;
        }
        //SI TODOS LOS INDICES SON MENORES
        else if(this->Iterador->getRightNodo()==NULL && this->Iterador->getX() != x){
            //INSERTAR NUEVO INDICE A LA DERECHA DEL ITERADOR
            NodoOrtogonal<T> *NewIndice = new NodoOrtogonal<T>;
            NewIndice->setIndice(x);
            this->Iterador->setRightNodo(NewIndice);
            NewIndice->setLeftNodo(this->Iterador);
            //ACTUALIZAR ITERADOR
            this->Iterador=NewIndice;
        }
    }

    //SI NO HAY NODOS HACIA ABAJO
    if(this->Iterador->getDownNodo()==NULL){
        this->Iterador->setDownNodo(NewNodo);
        NewNodo->setUpNodo(this->Iterador);
    }
    else{
        //INSERTAR HACIA ABAJO DEL INDICE EN Y (Iterador)
        do{
            this->Iterador=this->Iterador->getDownNodo();
            //SI EL INDICE DEL NODO ES MAYOR QUE Y
            if(this->Iterador->getDownNodo()->getY()> y){
                //INSERTAR NUEVO NODO ARRIBA DEL ITERADOR
                this->Iterador->getUpNodo()->setDownNodo(NewNodo);
                NewNodo->setUpNodo(this->Iterador->getUpNodo());
                this->Iterador->setUpNodo(NewNodo);
                NewNodo->setDownNodo(this->Iterador);
                this->Iterador=NewNodo;
            }
            //SI YA NO HAY NODOS HACIA ABAJO
            else if(this->Iterador->getDownNodo()==NULL){
                this->Iterador->setDownNodo(NewNodo);
                NewNodo->setUpNodo(this->Iterador);
                this->Iterador=NewNodo;
            }
            else if(this->Iterador->getY() == y){
                this->Iterador->setNodoValue(NewNodo->getNodoValue());
            }
        }while(this->Iterador->getY()!= y);

    }


    //SE BUSCA INDICE FILA
    this->Iterador=this->Head->getDownNodo();

    //MIENTRAS QUE EL INDICE SEA DIFERENTE DE Y
    while(this->Iterador->getIndice()!= y){

        this->Iterador=this->Iterador->getDownNodo();
        //SI EL INDICE ES MAYOR QUE Y
        if(this->Iterador->getIndice() > y){
            //INSERTAR NUEVO INDICE A LA ARRIBA DEL ITERADOR
            NodoOrtogonal<T> *NewIndice = new NodoOrtogonal<T>;
            NewIndice->setIndice(y);
            this->Iterador->getUpNodo()->setDownNodo(NewIndice);
            NewIndice->setUpNodo(this->Iterador->getUpNodo());
            this->Iterador->setUpNodo(NewIndice);
            NewIndice->setDownNodo(this->Iterador);
            //ACTUALIZAR ITERADOR
            this->Iterador=NewIndice;
        }
        //SI TODOS LOS INDICES SON MENORES
        else if(this->Iterador->getDownNodo()==NULL && this->Iterador->getY()!= y){
            //INSERTAR NUEVO INDICE ABAJO DEL ITERADOR
            NodoOrtogonal<T> *NewIndice = new NodoOrtogonal<T>;
            NewIndice->setIndice(y);
            this->Iterador->setDownNodo(NewIndice);
            NewIndice->setUpNodo(this->Iterador);
            //ACTUALIZAR ITERADOR
            this->Iterador=NewIndice;
        }
    }

    //SI NO HAY NODOS HACIA LA DERECHA
    if(this->Iterador->getRightNodo()==NULL){
        this->Iterador->setRightNodo(NewNodo);
        NewNodo->setLeftNodo(this->Iterador);
    }
    else{
        //INSERTAR HACIA LA DERECHA EN X (Iterador)
        do{
            this->Iterador=this->Iterador->getDownNodo();
            //SI EL INDICE DEL NODO ES MAYOR QUE X
            if(this->Iterador->getDownNodo()->getX()> x){
                //INSERTAR NUEVO NODO LA IZQUIERDA DEL ITERADOR
                this->Iterador->getLeftNodo()->setRightNodo(NewNodo);
                NewNodo->setLeftNodo(this->Iterador->getLeftNodo());
                this->Iterador->setLeftNodo(NewNodo);
                NewNodo->setRightNodo(this->Iterador);
                this->Iterador=NewNodo;
            }
            //SI YA NO HAY NODOS HACIA LA DERECHA
            else if(this->Iterador->getDownNodo()==NULL){
                this->Iterador->setRightNodo(NewNodo);
                NewNodo->setLeftNodo(this->Iterador);
                this->Iterador=NewNodo;
            }
            else if(this->Iterador->getX() == x){
                this->Iterador->setNodoValue(NewNodo->getNodoValue());
            }
        }while(this->Iterador->getX()!= x);
    }


}

template <class T>
void MatrizDispersa<T>::deleteInXY(int x, int y) {

    this->Iterador=getInXY(x,y);
    if(this->Iterador->getDownNodo()!=NULL){
        this->Iterador->getUpNodo()->setDownNodo(this->Iterador->getDownNodo());
        this->Iterador->getDownNodo()->setUpNodo(this->Iterador->getUpNodo());
    }
    else {
        this->Iterador->getUpNodo()->setDownNodo(NULL);
    }

    if(this->Iterador->getRightNodo()!=NULL){
        this->Iterador->getLeftNodo()->setRightNodo(this->Iterador->getRightNodo());
        this->Iterador->getRightNodo()->setLeftNodo(this->Iterador->getLeftNodo());
    }
    else{
        this->Iterador->getLeftNodo()->setRightNodo(NULL);
    }

    delete this->Iterador;

}

template <class T>
NodoOrtogonal<T>* MatrizDispersa<T>::getInXY(int x, int y) {

    //SE BUSCA INDICE DE COLUMNA
    this->Iterador=this->Head;

    //MIENTRAS QUE EL INDICE SEA DIFERENTE DE X
    do{
        //SI NO EXISTE EL INDICE
        if(this->Iterador->getRightNodo()==NULL && this->Iterador->getIndice()!= x){
            NodoOrtogonal<T> *NullNodo= new NodoOrtogonal<T>;
            NullNodo->setIndice(-100);
            return NullNodo;
        }
        else{
            if(this->Iterador->getIndice()!= x){
                this->Iterador=this->Iterador->getRightNodo();
            }
        }
    }while(this->Iterador->getIndice()!= x);

    //SE BUSCA NODO HACIA ABAJO CON COORDENADA Y
    do{
        //SI NO EXISTE EL INDICE
        if(this->Iterador->getDownNodo()==NULL && this->Iterador->getIndice()!= y){
            NodoOrtogonal<T> *NullNodo= new NodoOrtogonal<T>;
            NullNodo->setIndice(-100);
            return NullNodo;
        }
        else{
            if(this->Iterador->getIndice()!= y){
                this->Iterador=this->Iterador->getDownNodo();
            }
        }
    }while(this->Iterador->getY()!=y);

    return this->Iterador;



}




#endif //PROYECTO1EDD_MATRIZDISPERSA_H
