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

//TAMAÑO DEL TABLERO
int dimensionTablero;
//LISTA DE CASILLAS DOBLES Y TRIPLES
LinkedList<Casilla> *CasillasList = new LinkedList<Casilla>();
//DICCIONARIO DE PALABRAS
DoubleCircularLinkedList<string> *Diccionario = new DoubleCircularLinkedList<string>();
//COLA DE FICHAS
Queue<Ficha> *ColaFichas = new Queue<Ficha>();

int reporteDiccionario=0;

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
    string Ruta="";
    string textJSON="";
    bool Control = true;

    while(Control){
        int ch = getch();

        switch(ch){
            //OPCION 1 -- Abrir Archivo JSON
            case 49: {
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
                    if (ch == 10) {
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
                            graphDiccionario();
                            option1Control = false;
                            clearWin(win);
                            showMenu(win);
                        } else {
                            clearWin(win);
                            showMenu(win);
                            mvwprintw(win, 20, 6, "NO EXISTE ARCHIVO EN LA RUTA INDICADA");
                            mvwprintw(win, 18, 6, "Escriba Ruta de Archivo: ");
                            wrefresh(win);
                        }
                        Ruta = "";
                    }
                }
                //SE PARSEA JSON
                readJSON(textJSON);
            }
                break;
            //OPCION 2  -- Jugar
            case 50:
                break;
            //OPCION 3 -- Reportes
            case 51:
                break;
            //OPCION 4 -- Salir
            case 52:
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
    mvwprintw(win,11,6,"4.Salir");
    wrefresh(win);

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
    string aux = "";
    ofstream file;
    int ListaSize;
    string TempPalabra;

    ListaSize=Diccionario->getSize();
    if(ListaSize>0){
        file.open("./Diccionario" + to_string(reporteDiccionario) + ".dot", fstream::in | fstream::out | fstream::trunc);
        file << "digraph {";
        file << "node [shape=box];"<<endl;
        file << "rankdir=RL;"<<endl;

        TempPalabra = Diccionario->getFirst();
        file<< "\""+TempPalabra+"\""<<endl;
        file<<"->"<<endl;
        TempPalabra = Diccionario->getXNode(1);
        file<< "\""+TempPalabra+"\""<<endl;

        TempPalabra = Diccionario->getFirst();
        file<< "\""+TempPalabra+"\""<<endl;
        file<<"->"<<endl;
        TempPalabra = Diccionario->getLast();
        file<< "\""+TempPalabra+"\""<<endl;


        for(int i =1;i<ListaSize-1;i++){

            TempPalabra = Diccionario->getXNode(i);
            file<< "\""+TempPalabra+"\""<<endl;
            file<<"->"<<endl;
            TempPalabra = Diccionario->getXNode(i+1);
            file<< "\""+TempPalabra+"\""<<endl;

            TempPalabra = Diccionario->getXNode(i+1);
            file<< "\""+TempPalabra+"\""<<endl;
            file<<"->"<<endl;
            TempPalabra = Diccionario->getXNode(i);
            file<< "\""+TempPalabra+"\""<<endl;

        }
        TempPalabra = Diccionario->getLast();
        file<< "\""+TempPalabra+"\""<<endl;
        file<<"->"<<endl;
        TempPalabra = Diccionario->getFirst();
        file<< "\""+TempPalabra+"\""<<endl;
    }
    else{
        file.open("./Malo" + to_string(reporteDiccionario) + ".dot", fstream::in | fstream::out | fstream::trunc);
    }
    file << "}";
    file.close();
    command = "dot -Tpng ./Diccionario" + to_string(reporteDiccionario) + ".dot -o Diccionario" + to_string(reporteDiccionario) + ".png >>/dev/null 2>>/dev/null";
    system(command.c_str());

    reporteDiccionario++;

}

