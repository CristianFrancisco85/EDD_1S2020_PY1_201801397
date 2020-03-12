#include <iostream>
#include <fstream>
#include <curses.h>
#include <json/json.h>
#include <cstdlib>
#include <ctime>

#include "Casilla.h"
#include "DoubleCircularLinkedList.h"
#include "DoubleLinkedList.h"
#include "LinkedList.h"
#include "MatrizDispersa.h"
#include "Queue.h"
#include "Ficha.h"
#include "Jugador.h"
#include "ABB.h"

using namespace std;

//MUESTRA MENU
void showMenu(WINDOW * win);
// LIMPIA VENTANA DE TEXTO
void clearWin(WINDOW * win);
//LEE STRING CON FORMATO JSON
void readJSON(string Texto);
//CREA LAS FICHAS Y LAS INGRESA DE MANERA ALEATORIA A LA COLA
void createFichas();
//GRAFICA REPORTES DEL DICCIONARIO
void graphDiccionario();
//MUESTRA MENU DE REPORTES
void showMenuReportes(WINDOW * win);
//GRAFICA ABB DE JUGADORES
void graphArbol();
//GRAFICA ABB
void graphPreOrden();
//GRAFICA ABB
void graphInOrden();
//GRAFICA ABB
void graphPostOrden();

//TAMAÑO DEL TABLERO
int dimensionTablero;
//LISTA DE CASILLAS DOBLES Y TRIPLES
LinkedList<Casilla> *CasillasList = new LinkedList<Casilla>();
//DICCIONARIO DE PALABRAS
DoubleCircularLinkedList<string> *Diccionario = new DoubleCircularLinkedList<string>();
//COLA DE FICHAS
Queue<Ficha> *ColaFichas = new Queue<Ficha>();
//ARBOL BINARIO DE BUSQEDAD PARA JUAGORES
ABB *ArbolJugadores = new ABB();


//VARIABLES PARA ENUMERAR REPORTES
int reporteDiccionario=0;
int reporteArbol=0;
int reporteArbolPre=0;
int reporteArbolIn=0;
int reporteArbolPost=0;

int main() {
    int height,width,startx,starty;

    height = 26,
            width= 81;
    startx = 2;
    starty = 2;

    // CONFIGURACIONES INICIALES
    initscr();
    cbreak();
    noecho();
    raw();
    keypad(stdscr, TRUE);
    //IMPRIMIR ENCABEZADO y VENTANA
    attron(A_REVERSE);
    move(0,2);
    printw("             PROYECTO 1 - CRISTIAN FRANCISCO MEONO CANEL -  201801397             ");
    attroff(A_REVERSE);
    WINDOW * borderwin = newwin(height+2,width+2,1,1);
    WINDOW * win = newwin(height,width,starty,startx);
    refresh();
    box (borderwin,124,45);
    wrefresh(borderwin);

    // MOSTRAR MENU
    showMenu(win);

    bool Control = true;

    while(Control){
        int ch = getch();

        switch(ch){
            //OPCION 1 -- Abrir Archivo JSON
            case 49: {

                string Ruta="";
                string textJSON="";

                mvwprintw(win, 18, 6, "Escriba Ruta de Archivo: ");
                wrefresh(win);
                bool option1Control = true;

                while (option1Control)
                {
                    ch = getch();
                    if (ch >= 32 && ch <= 126) {
                        Ruta.push_back(ch);
                        wprintw(win, "%c", ch);
                        wrefresh(win);
                    }
                    else if(ch==127 && Ruta.length()>0){
                        Ruta.pop_back();
                        clearWin(win);
                        showMenu(win);
                        mvwprintw(win, 18, 6, "Escriba Ruta de Archivo: ");
                        wprintw(win, Ruta.c_str());
                        wrefresh(win);
                    }
                    else if(ch==27 ){
                        option1Control = false;
                        clearWin(win);
                        showMenu(win);
                    }
                    else if (ch == 10) {
                        string aux = Ruta;
                        //SE VERIFICA QUE EL FICHERO EXISTA Y SE LEE
                        ifstream fichero;
                        fichero.open(Ruta);
                        if (!fichero.fail()) {
                            char letra;
                            fichero.get(letra);

                            while(!fichero.eof())
                            {
                            textJSON.push_back(letra);
                            fichero.get(letra);
                            }
                            fichero.close();
                            readJSON(textJSON);
                            //graphDiccionario();
                            option1Control = false;
                            clearWin(win);
                            showMenu(win);
                        }
                        else {
                            clearWin(win);
                            showMenu(win);
                            mvwprintw(win, 20, 6, "NO EXISTE ARCHIVO EN LA RUTA INDICADA");
                            mvwprintw(win, 18, 6, "Escriba Ruta de Archivo: ");
                            wrefresh(win);
                        }
                        Ruta = "";
                    }
                }
            }
                break;
            //OPCION 2  -- Jugar
            case 50:
                break;
            //OPCION 3 -- Reportes
            case 51:
                showMenuReportes(win);
                break;
            //OPCION 4 -- Agregar Nuevo Jugador
            case 52:{
                string Nombre="";
                mvwprintw(win, 18, 6, "Escriba Nombre de Nuevo Jugador: ");
                wrefresh(win);
                bool option4Control = true;
                while (option4Control)
                {
                    ch = getch();
                    if (ch >= 32 && ch <= 126) {
                        Nombre.push_back(ch);
                        wprintw(win, "%c", ch);
                        wrefresh(win);
                    }
                    else if(ch==127 && Nombre.length()>0){
                        Nombre.pop_back();
                        clearWin(win);
                        showMenu(win);
                        mvwprintw(win, 18, 6, "Escriba Nombre de Nuevo Jugador: ");
                        wprintw(win, Nombre.c_str());
                        wrefresh(win);
                    }
                    else if(ch==27 ){
                        option4Control = false;
                        clearWin(win);
                        showMenu(win);
                        wrefresh(win);
                    }
                    else if (ch == 10) {
                        Jugador *TempJugador = new Jugador();
                        TempJugador->setNombre(Nombre);

                        if (ArbolJugadores->addJugador(*TempJugador)) {
                            delete TempJugador;
                            clearWin(win);
                            showMenu(win);
                            mvwprintw(win, 20, 6, "JUGADOR AGREGADO");
                            mvwprintw(win, 18, 6, "Presione cualquir tecla para continuar...");
                            wrefresh(win);
                            getch();
                            option4Control = false;
                            clearWin(win);
                            showMenu(win);
                            wrefresh(win);
                        }
                        else {
                            clearWin(win);
                            showMenu(win);
                            mvwprintw(win, 20, 6, "YA EXISTE EL NOMBRE DEL JUGADOR");
                            mvwprintw(win, 18, 6, "Escriba Nombre de Nuevo Jugador: ");
                            wrefresh(win);
                        }
                        Nombre = "";
                    }
                }
            }
                break;
            //OPCION 5 -- Salir
            case 53:
                clearWin(win);
                mvwprintw(win,0,0,"Hasta la proxima -- Presione una tecla para continuar...");
                wrefresh(win);
                getch();
                clearWin(win);
                endwin();
                Control = false;
                break;
        }
    }


    return 0;
}


//MANEJO DE INTERFAZ EN CONSOLA

void clearWin(WINDOW * win){
    werase(win);
    wrefresh(win);
}

void showMenu(WINDOW * win){
    mvwprintw(win,2,5,"UNIVERSIDAD DE SAN CARLOS DE GUATEMALA");
    mvwprintw(win,3,5,"FACULTAD DE INGENIERIA");
    mvwprintw(win,4,5,"ESTRUCTURAS DE DATOS");

    mvwprintw(win,6,5,"MENU:");
    mvwprintw(win,8,6,"1.Abrir Archivo JSON de Configuracion");
    mvwprintw(win,9,6,"2.Jugar");
    mvwprintw(win,10,6,"3.Reportes");
    mvwprintw(win,11,6,"4.Crear Nuevo Jugador");
    mvwprintw(win,12,6,"5.Salir");
    wrefresh(win);

}

void showMenuReportes(WINDOW * win){
    clearWin(win);
    mvwprintw(win,3,5,"          REPORTES");
    mvwprintw(win,5,6,"1.Diccionario");
    mvwprintw(win,6,6,"2.ABB de Jugadores");
    mvwprintw(win,7,6,"3.Recorrido PreOrden del ABB de Jugadores");
    mvwprintw(win,8,6,"4.Recorrido InOrden del ABB de Jugadores");
    mvwprintw(win,9,6,"5.Recorrido PostOrden del ABB de Jugadores");
    mvwprintw(win,10,6,"6.Historial de Puntajes de un Jugador");
    mvwprintw(win,11,6,"7.ScoreBoard");
    mvwprintw(win,12,6,"8.Salir");
    wrefresh(win);

    bool Control = true;

    while(Control){
        int ch = getch();

        switch(ch){
            //OPCION 1 -- DICCIONARIO
            case 49:
                graphDiccionario();
                break;
            //OPCION 2 -- ABB JUGADORES
            case 50:
                graphArbol();
                break;
            //OPCION 3 -- PREORDEN ABB
            case 51:
                graphPreOrden();
                break;
            //OPCION 4 -- INORDEN ABB
            case 52:
                graphInOrden();
                break;
            //OPCION 5 -- POSTORDENABB
            case 53:
                graphPostOrden();
                break;
            //OPCION 6 -- PUNTAJES JUAGOR
            case 54:
                break;
            //OPCION 7 -- SCOREBOARD
            case 55:
                break;
            //OPCION 8 -- SALIR
            case 56: {
                clearWin(win);
                mvwprintw(win, 0, 0, "Presione una tecla para salir...");
                wrefresh(win);
                getch();
                clearWin(win);
                showMenu(win);
                wrefresh(win);
                Control = false;
                break;
            }
        }
    }

}

//LECTURA DE JSON Y CREACION DE OBJETOS

void readJSON(string Texto){
    Json::Reader reader;
    Json::Value obj;
    Casilla *TempCasilla = new Casilla();;
    if(reader.parse(Texto, obj)) {
        //LEE DIMENSION DEL TABLERO
        dimensionTablero= (int)obj["dimension"].asInt();
        //LEE LAS CASILLAS DOBLES
        for(int i = 0 ; i< obj["casillas"]["dobles"].size();i++){
            TempCasilla->setX(obj["casillas"]["dobles"][i]["x"].asInt());
            TempCasilla->setY(obj["casillas"]["dobles"][i]["y"].asInt());
            TempCasilla->setMultiplicador(2);
            CasillasList->addEnd(*TempCasilla);
        }
        //LEE LAS CASILLAS TRIPLES
        for(int i = 0 ; i< obj["casillas"]["triples"].size();i++){
            TempCasilla->setX(obj["casillas"]["triples"][i]["x"].asInt());
            TempCasilla->setY(obj["casillas"]["triples"][i]["y"].asInt());
            TempCasilla->setMultiplicador(3);
            CasillasList->addEnd(*TempCasilla);
        }
        //LEE PALABRAS PARA EL DICCIONARIO
        for(int i = 0 ; i< obj["diccionario"].size();i++){
            Diccionario->addEnd(obj["diccionario"][i]["palabra"].asString());
        }
    }
    else{
        //cout<<Texto;
        std::cout << reader.getFormattedErrorMessages() << std::endl;
    }

}

//CREACION DE FICHAS

void createFichas(){
    //LAS FICHAS SON "QUEMADAS"
    LinkedList<Ficha> *TempFichas = new LinkedList<Ficha>();
    Ficha *TempFicha = new Ficha();
    //SE CREAN FICHAS DE CADA LETRA
    //A
    for(int i=0;i<12;i++){
        TempFicha->setLetra('A');
        TempFicha->setPuntos(1);
        TempFichas->addEnd(*TempFicha);
    }
    //E
    for(int i=0;i<12;i++){
        TempFicha->setLetra('E');
        TempFicha->setPuntos(1);
        TempFichas->addEnd(*TempFicha);
    }
    //O
    for(int i=0;i<9;i++){
        TempFicha->setLetra('O');
        TempFicha->setPuntos(1);
        TempFichas->addEnd(*TempFicha);
    }
    //I
    for(int i=0;i<6;i++){
        TempFicha->setLetra('I');
        TempFicha->setPuntos(1);
        TempFichas->addEnd(*TempFicha);
    }
    //S
    for(int i=0;i<6;i++){
        TempFicha->setLetra('S');
        TempFicha->setPuntos(1);
        TempFichas->addEnd(*TempFicha);
    }
    //N
    for(int i=0;i<5;i++){
        TempFicha->setLetra('N');
        TempFicha->setPuntos(1);
        TempFichas->addEnd(*TempFicha);
    }
    //L
    for(int i=0;i<4;i++){
        TempFicha->setLetra('L');
        TempFicha->setPuntos(1);
        TempFichas->addEnd(*TempFicha);
    }
    //R
    for(int i=0;i<5;i++){
        TempFicha->setLetra('R');
        TempFicha->setPuntos(1);
        TempFichas->addEnd(*TempFicha);
    }
    //U
    for(int i=0;i<5;i++){
        TempFicha->setLetra('U');
        TempFicha->setPuntos(1);
        TempFichas->addEnd(*TempFicha);
    }
    //T
    for(int i=0;i<4;i++){
        TempFicha->setLetra('T');
        TempFicha->setPuntos(1);
        TempFichas->addEnd(*TempFicha);
    }
    //D
    for(int i=0;i<5;i++){
        TempFicha->setLetra('D');
        TempFicha->setPuntos(2);
        TempFichas->addEnd(*TempFicha);
    }
    //G
    for(int i=0;i<2;i++){
        TempFicha->setLetra('G');
        TempFicha->setPuntos(2);
        TempFichas->addEnd(*TempFicha);
    }
    //C
    for(int i=0;i<4;i++){
        TempFicha->setLetra('C');
        TempFicha->setPuntos(3);
        TempFichas->addEnd(*TempFicha);
    }
    //B
    for(int i=0;i<2;i++){
        TempFicha->setLetra('B');
        TempFicha->setPuntos(3);
        TempFichas->addEnd(*TempFicha);
    }
    //M
    for(int i=0;i<2;i++){
        TempFicha->setLetra('M');
        TempFicha->setPuntos(3);
        TempFichas->addEnd(*TempFicha);
    }
    //P
    for(int i=0;i<2;i++){
        TempFicha->setLetra('P');
        TempFicha->setPuntos(3);
        TempFichas->addEnd(*TempFicha);
    }
    //H
    for(int i=0;i<2;i++){
        TempFicha->setLetra('H');
        TempFicha->setPuntos(4);
        TempFichas->addEnd(*TempFicha);
    }
    //F
    for(int i=0;i<1;i++){
        TempFicha->setLetra('F');
        TempFicha->setPuntos(4);
        TempFichas->addEnd(*TempFicha);
    }
    //V
    for(int i=0;i<1;i++){
        TempFicha->setLetra('V');
        TempFicha->setPuntos(4);
        TempFichas->addEnd(*TempFicha);
    }
    //Y
    for(int i=0;i<1;i++){
        TempFicha->setLetra('Y');
        TempFicha->setPuntos(4);
        TempFichas->addEnd(*TempFicha);
    }
    //Q
    for(int i=0;i<1;i++){
        TempFicha->setLetra('Q');
        TempFicha->setPuntos(5);
        TempFichas->addEnd(*TempFicha);
    }
    //J
    for(int i=0;i<1;i++){
        TempFicha->setLetra('J');
        TempFicha->setPuntos(8);
        TempFichas->addEnd(*TempFicha);
    }
    //X
    for(int i=0;i<1;i++){
        TempFicha->setLetra('X');
        TempFicha->setPuntos(8);
        TempFichas->addEnd(*TempFicha);
    }
    //Z
    for(int i=0;i<1;i++){
        TempFicha->setLetra('J');
        TempFicha->setPuntos(10);
        TempFichas->addEnd(*TempFicha);
    }

    srand((unsigned) time(0));
    int randomNumber;

    //SE INGRESAN DE MANERA ALEATORIA LAS 95 FICHAS A LA COLA

    for(int i=0;i<95;i++){
        if(TempFichas->getSize()!=0){
            randomNumber = (rand() % TempFichas->getSize()-1);
            ColaFichas->push(TempFichas->getXNode(randomNumber));
            TempFichas->deleteXNode(randomNumber);
        }
        else{
            ColaFichas->push(TempFichas->getFirst());
            delete TempFichas;
            break;
        }
    }


}

//REPORTES

void graphDiccionario(){

    string command = "";
    ofstream file;
    int ListaSize;
    string TempPalabra;

    ListaSize=Diccionario->getSize();
    if(ListaSize>0){
        file.open("./Diccionario" + to_string(reporteDiccionario) + ".dot", fstream::in | fstream::out | fstream::trunc);
        file << "digraph {";
        file << "node [shape=box];"<<endl;
        file << "rankdir=LR;"<<endl;

        TempPalabra = Diccionario->getFirst();
        file<< "\""+TempPalabra+"\"";
        file<<"->";
        TempPalabra = Diccionario->getXNode(1);
        file<< "\""+TempPalabra+"\";"<<endl;

        TempPalabra = Diccionario->getXNode(1);
        file<< "\""+TempPalabra+"\"";
        file<<"->";
        TempPalabra = Diccionario->getFirst();
        file<< "\""+TempPalabra+"\";"<<endl;

        TempPalabra = Diccionario->getFirst();
        file<< "\""+TempPalabra+"\"";
        file<<"->";
        TempPalabra = Diccionario->getLast();
        file<< "\""+TempPalabra+"\";"<<endl;


        for(int i =1;i<ListaSize-1;i++){

            TempPalabra = Diccionario->getXNode(i);
            file<< "\""+TempPalabra+"\"";
            file<<"->";
            TempPalabra = Diccionario->getXNode(i+1);
            file<< "\""+TempPalabra+"\";"<<endl;

            TempPalabra = Diccionario->getXNode(i+1);
            file<< "\""+TempPalabra+"\"";
            file<<"->";
            TempPalabra = Diccionario->getXNode(i);
            file<< "\""+TempPalabra+"\";"<<endl;

        }
        TempPalabra = Diccionario->getLast();
        file<< "\""+TempPalabra+"\"";
        file<<"->";
        TempPalabra = Diccionario->getFirst();
        file<< "\""+TempPalabra+"\";"<<endl;
    }
    else{
        file.open("./MaloDiccionario" + to_string(reporteDiccionario) + ".dot", fstream::in | fstream::out | fstream::trunc);
    }
    file << "}";
    file.close();
    command = "dot -Tpng ./Diccionario" + to_string(reporteDiccionario) + ".dot -o Diccionario" + to_string(reporteDiccionario) + ".png >>/dev/null 2>>/dev/null";
    system(command.c_str());
    command="open ./Diccionario" + to_string(reporteDiccionario) + ".png ";
    system(command.c_str());
    reporteDiccionario++;

}

void graphArbol(){
    Queue<Jugador*> *AuxCola = new Queue<Jugador*>();
    string command = "";
    ofstream file;
    //Jugador *TempJugador= new Qu
    ArbolJugadores->preOrden(ArbolJugadores->getRaiz(),AuxCola);
    int SizeCola = AuxCola->getSize();
    if(SizeCola>0){
        file.open("./ABB" + to_string(reporteArbol) + ".dot", fstream::in | fstream::out | fstream::trunc);
        file << "digraph {";
        file << "node [shape=circle];"<<endl;
        for(int i=0;i<SizeCola;i++){
            if(AuxCola->pick()->getIzquierda()!=NULL){
                file<< "\""+AuxCola->pick()->getNombre()+"\"";
                file<<"->";
                file<< "\""+AuxCola->pick()->getIzquierda()->getNombre()+"\";"<<endl;
            }
            if(AuxCola->pick()->getDerecha()!=NULL){
                file<< "\""+AuxCola->pick()->getNombre()+"\"";
                file<<"->";
                file<< "\""+AuxCola->pick()->getDerecha()->getNombre()+"\";"<<endl;
            }
            AuxCola->pop();
        }
        file << "}";
        file.close();
        command = "dot -Tpng ./ABB" + to_string(reporteArbol) + ".dot -o ABB" + to_string(reporteArbol) + ".png >>/dev/null 2>>/dev/null";
        system(command.c_str());
        command="open ./ABB" + to_string(reporteArbol) + ".png ";
        system(command.c_str());
        reporteArbol++;
    }
}

void graphPreOrden(){
    Queue<Jugador*> *AuxCola = new Queue<Jugador*>();
    string command = "";
    ofstream file;
    //Jugador *TempJugador= new Qu
    ArbolJugadores->preOrden(ArbolJugadores->getRaiz(),AuxCola);
    int SizeCola = AuxCola->getSize();
    if(SizeCola>0){
        file.open("./ABBPreOrden" + to_string(reporteArbolPre) + ".dot", fstream::in | fstream::out | fstream::trunc);
        file << "digraph {";
        file << "node [shape=box];"<<endl;
        file << "rankdir=LR;"<<endl;
        for(int i=1;i<SizeCola;i++){
            file<<"\""+AuxCola->pop()->getNombre()+"\"";
            file<<"->";
            file<<"\""+AuxCola->pick()->getNombre()+"\";";
        }
        AuxCola->pop();
        file << "}";
        file.close();
        command = "dot -Tpng ./ABBPreOrden" + to_string(reporteArbolPre) + ".dot -o ABBPreOrden" + to_string(reporteArbolPre) + ".png >>/dev/null 2>>/dev/null";
        system(command.c_str());
        command="open ./ABBPreOrden" + to_string(reporteArbolPre) + ".png ";
        system(command.c_str());
        reporteArbolPre++;
    }
}

void graphInOrden(){
    Queue<Jugador*> *AuxCola = new Queue<Jugador*>();
    string command = "";
    ofstream file;
    //Jugador *TempJugador= new Qu
    ArbolJugadores->inOrden(ArbolJugadores->getRaiz(),AuxCola);
    int SizeCola = AuxCola->getSize();
    if(SizeCola>0){
        file.open("./ABBInOrden" + to_string(reporteArbolIn) + ".dot", fstream::in | fstream::out | fstream::trunc);
        file << "digraph {";
        file << "node [shape=box];"<<endl;
        file << "rankdir=LR;"<<endl;
        for(int i=1;i<SizeCola;i++){
            file<<"\""+AuxCola->pop()->getNombre()+"\"";
            file<<"->";
            file<<"\""+AuxCola->pick()->getNombre()+"\";";
        }
        AuxCola->pop();
        file << "}";
        file.close();
        command = "dot -Tpng ./ABBInOrden" + to_string(reporteArbolIn) + ".dot -o ABBInOrden" + to_string(reporteArbolIn) + ".png >>/dev/null 2>>/dev/null";
        system(command.c_str());
        command="open ./ABBInOrden" + to_string(reporteArbolIn) + ".png ";
        system(command.c_str());
        reporteArbolIn++;
    }

}

void graphPostOrden(){
    Queue<Jugador*> *AuxCola = new Queue<Jugador*>();
    string command = "";
    ofstream file;
    //Jugador *TempJugador= new Qu
    ArbolJugadores->postOrden(ArbolJugadores->getRaiz(),AuxCola);
    int SizeCola = AuxCola->getSize();
    if(SizeCola>0){
        file.open("./ABBPostOrden" + to_string(reporteArbolPost) + ".dot", fstream::in | fstream::out | fstream::trunc);
        file << "digraph {";
        file << "node [shape=box];"<<endl;
        file << "rankdir=LR;"<<endl;
        for(int i=1;i<SizeCola;i++){
            file<<"\""+AuxCola->pop()->getNombre()+"\"";
            file<<"->";
            file<<"\""+AuxCola->pick()->getNombre()+"\";";
        }
        AuxCola->pop();
        file << "}";
        file.close();
        command = "dot -Tpng ./ABBPostOrden" + to_string(reporteArbolPost) + ".dot -o ABBPostOrden" + to_string(reporteArbolPost) + ".png >>/dev/null 2>>/dev/null";
        system(command.c_str());
        command="open /.ABBPostOrden" + to_string(reporteArbolPost) + ".png ";
        system(command.c_str());
        reporteArbolPost++;
    }
}

