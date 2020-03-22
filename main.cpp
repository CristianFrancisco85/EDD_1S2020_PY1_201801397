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
//MUESTRA MENU DE SELECCION DE JUGADORES
Jugador* showJugadores(WINDOW * win,string jugador);
//COMIENZA JUEGO
void comenzarJuego(WINDOW * win,Jugador *Jugador1 , Jugador *Jugador2);
//GRAFICA COLA DE FICHAS
void graphFichas();
//VALIDA LA PALABRA CON EL DICCIONARIO
bool validarPalabra(DoubleLinkedList<Ficha> *TempFichas,int direccion,int coorX,int coorY,int *ActualPuntaje);
//INTERCAMBIA LAS FICHAS
void intercambiarFichas(DoubleLinkedList<Ficha> *TempFichas,DoubleLinkedList<Ficha> *FichasJugador);
//GRAFICA TABLERO DE JUEGO
void graphTablero();
//REGRESA EL MULTIPLICADOR DE UNA CASILLA
int puntajeFicha(int x,int y);
//GRAFICA LAS FICHAS DE UN JUGADOR
void graphFichasJugador(DoubleLinkedList<Ficha> *TempFichas,Jugador *TempJugador);
//GRAFICA EL PUNTAJE DE UN JUGADOR
void graphPuntajesJugador(Jugador *TempJugador);
//GRAFICA SCOREBOARD
void graphScoreBoard();

//TAMAÑO DEL TABLERO
int dimensionTablero;
//TABLERO DEL JUEGO
MatrizDispersa<Ficha> *Tablero = new MatrizDispersa<Ficha>;
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
int reporteFichas=0;
int reporteFichasJugador=0;

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
    int ch;
    while(Control){
        ch = getch();

        switch(ch) {
            //OPCION 1 -- Abrir Archivo JSON
            case 49: {

                string Ruta = "";
                string textJSON = "";

                mvwprintw(win, 18, 6, "Escriba Ruta de Archivo: ");
                wrefresh(win);
                bool option1Control = true;

                while (option1Control) {
                    ch = getch();
                    if (ch >= 32 && ch <= 126) {
                        Ruta.push_back(ch);
                        wprintw(win, "%c", ch);
                        wrefresh(win);
                    } else if (ch == 127 && Ruta.length() > 0) {
                        Ruta.pop_back();
                        clearWin(win);
                        showMenu(win);
                        mvwprintw(win, 18, 6, "Escriba Ruta de Archivo: ");
                        wprintw(win, Ruta.c_str());
                        wrefresh(win);
                    } else if (ch == 27) {
                        option1Control = false;
                        clearWin(win);
                        showMenu(win);
                    } else if (ch == 10) {
                        string aux = Ruta;
                        //SE VERIFICA QUE EL FICHERO EXISTA Y SE LEE
                        ifstream fichero;
                        fichero.open(Ruta);
                        if (!fichero.fail()) {
                            char letra;
                            fichero.get(letra);

                            while (!fichero.eof()) {
                                textJSON.push_back(letra);
                                fichero.get(letra);
                            }
                            fichero.close();
                            readJSON(textJSON);
                            //graphDiccionario();
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
            }
                break;
                //OPCION 2  -- Jugar
            case 50:{
                Jugador *jugador1;
                Jugador *jugador2;
                jugador1=showJugadores(win,"1");
                jugador2=showJugadores(win,"2");
                comenzarJuego(win,jugador1,jugador2);
                clearWin(win);
                showMenu(win);
                break;
            }
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
    mvwprintw(win,2,5,"REPORTES");
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
    int ch;
    while(Control){
        ch = getch();

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
            //OPCION 6 -- PUNTAJES JUGADOR
            case 54:
                graphPuntajesJugador(showJugadores(win,"."));
                clearWin(win);
                mvwprintw(win,2,5,"REPORTES");
                mvwprintw(win,5,6,"1.Diccionario");
                mvwprintw(win,6,6,"2.ABB de Jugadores");
                mvwprintw(win,7,6,"3.Recorrido PreOrden del ABB de Jugadores");
                mvwprintw(win,8,6,"4.Recorrido InOrden del ABB de Jugadores");
                mvwprintw(win,9,6,"5.Recorrido PostOrden del ABB de Jugadores");
                mvwprintw(win,10,6,"6.Historial de Puntajes de un Jugador");
                mvwprintw(win,11,6,"7.ScoreBoard");
                mvwprintw(win,12,6,"8.Salir");
                wrefresh(win);
                break;
            //OPCION 7 -- SCOREBOARD
            case 55:
                graphScoreBoard();
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

Jugador* showJugadores(WINDOW * win,string jugador){
    clearWin(win);
    mvwprintw(win,3,5,"ELIJA AL JUGADOR ");
    wprintw(win,jugador.c_str());
    Queue<Jugador*> *AuxCola = new Queue<Jugador*>();
    string auxilar;

    ArbolJugadores->preOrden(ArbolJugadores->getRaiz(),AuxCola);
    int SizeCola = AuxCola->getSize();
    if(SizeCola>0){

        for(int i=0;i<SizeCola;i++){
            auxilar=to_string(i)+". "+AuxCola->pop()->getNombre();
            mvwprintw(win,i+6,5,auxilar.c_str());
        }

    }
    wrefresh(win);
    ArbolJugadores->preOrden(ArbolJugadores->getRaiz(),AuxCola);
    char ch;
    int eleccion;
    bool control=true;

    while(control){
        ch = getch();
        eleccion = ch - '0';
        if(eleccion<AuxCola->getSize()){
            Jugador *auxJugador;
            for(int i=0;i<=eleccion;i++){
                auxJugador=AuxCola->pop();
            }
            return auxJugador;
        }
    }

}

void comenzarJuego(WINDOW * win,Jugador *Jugador1 , Jugador *Jugador2){

    int puntajeJugador1=0;
    int puntajeJugador2=0;
    dimensionTablero=50;
    DoubleLinkedList<Ficha> *FichasJugador1 = new DoubleLinkedList<Ficha>();
    DoubleLinkedList<Ficha> *FichasJugador2 = new DoubleLinkedList<Ficha>() ;
    createFichas();
    //SE REPARTEN LAS 7 FICHAS A CADA JUGADOR
    for(int i=0;i<7;i++){
        FichasJugador1->addEnd(ColaFichas->pop());
        FichasJugador2->addEnd(ColaFichas->pop());
    }
    clearWin(win);

    bool runtime =true;
    int ch;

    Jugador *JugadorActual;
    DoubleLinkedList<Ficha> *FichasActuales;
    int *PuntajeActual;

    srand((unsigned) time(0));
    int apuntadorJugador = rand() % 2;

    if(apuntadorJugador==0){
        JugadorActual=Jugador1;
        FichasActuales=FichasJugador1;
        PuntajeActual=&puntajeJugador1;
    }
    else{
        JugadorActual=Jugador2;
        FichasActuales=FichasJugador2;
        PuntajeActual=&puntajeJugador2;
    }

    while(runtime){

        //SI EL JUGADOR YA NO TIENE FICHAS
        if(FichasActuales->getSize()==0){
            for(int i=0;i<7;i++){
                FichasActuales->addEnd(ColaFichas->pop());
            }
        }

        wmove(win,0,0); wclrtoeol(win);
        wprintw(win,"ES EL TURNO DE: ");
        wprintw(win,JugadorActual->getNombre().c_str());
        wprintw(win,"     PUNTAJE: ");
        wprintw(win,"%d",*PuntajeActual);
        mvwprintw(win,15,0,"Ctrl+W para graficar fichas y Ctrl+Q para terminar partida");
        mvwprintw(win,16,0,"Si no presiona cualquier tecla para comenzar tu turno");
        wrefresh(win);
        ch = getch();
        if(ch==23){
            graphFichas();
            graphFichasJugador(FichasJugador1,Jugador1);
            graphFichasJugador(FichasJugador2,Jugador2);
        }
        else if(ch==17){
            break;
        }

        wmove(win,15,0); wclrtoeol(win);
        wmove(win,16,0); wclrtoeol(win);
        int sizeFichas=FichasActuales->getSize();
        for(int i=0;i<sizeFichas;i++){
            mvwprintw(win,3,2+2*i,"%c",FichasActuales->getXNode(i).getLetra());
        }
        mvwprintw(win,4,2,"^");
        mvwprintw(win,5,2,"|");
        wrefresh(win);

        //SE ELIGE LA DIRECCION
        int direccion; //0=ABAJO 1=DERECHA
        mvwprintw(win,10,0,"¿Hacia donde quieres escribir? Utiliza las teclas de direccion, solo se permite DERECHA y ABAJO");
        wrefresh(win);
        bool elegirDireccion=true;
        while(elegirDireccion){
            ch = getch();
                 if(ch==KEY_DOWN) { direccion=0; elegirDireccion=false; }
            else if(ch==KEY_RIGHT){ direccion=1; elegirDireccion=false; }
        }

        //SE ELIGE LA POSICION DE INICIO
        wmove(win,10,0); wclrtoeol(win); wrefresh(win);
        mvwprintw(win,10,0,"¿Donde quieres comenzar? Ingresa coordenada X , Y");
        wrefresh(win);
        string coordenada="";
        bool boolcoordenadas;
        int coorX,coorY;

        //COORDENADA EN X
        boolcoordenadas=true;
        mvwprintw(win,11,0,"Coordenada en X: ");
        wrefresh(win);
        while(boolcoordenadas){
            ch = getch();
            if(ch >=48 && ch<=57){
                coordenada=coordenada.append(to_string(ch-48));
                wprintw(win,"%c",ch); wrefresh(win);
            }
            else if(ch==10){
                coorX=stoi(coordenada);
                if(coorX<dimensionTablero){
                    boolcoordenadas=false;
                    wmove(win,13,0); wclrtoeol(win);
                }
                else{
                    wmove(win,11,0); wclrtoeol(win);
                    mvwprintw(win,13,0,"Coordenada en X no valida,intente de nuevo ");
                    mvwprintw(win,11,0,"Coordenada en X: ");
                    wrefresh(win);
                    coordenada="";
                }
            }
        }

        //COORDENADA EN Y
        boolcoordenadas=true;
        wmove(win,11,0); wclrtoeol(win);
        mvwprintw(win,11,0,"Coordenada en Y: ");
        wrefresh(win);
        coordenada="";
        while(boolcoordenadas){
            ch = getch();
            if(ch >=48 && ch<=57){
                coordenada=coordenada.append(to_string(ch-48));
                wprintw(win,"%c",ch); wrefresh(win);
            }
            else if(ch==10){
                coorY=stoi(coordenada);
                if(coorY<dimensionTablero){
                    boolcoordenadas=false;
                    wmove(win,13,0); wclrtoeol(win);
                }
                else{
                    wmove(win,11,0); wclrtoeol(win);
                    mvwprintw(win,13,0,"Coordenada en Y no valida,intente de nuevo ");
                    mvwprintw(win,11,0,"Coordenada en Y: ");
                    wrefresh(win);
                    coordenada="";
                }
            }
        }

        //SE SELECCIONAN FICHAS A PONER EN EL TABLERO
        wmove(win,11,0); wclrtoeol(win);
        mvwprintw(win,10,0,"Para intercambiar fichas presiona Ctrl+S sino presiona cualquier tecla");
        wrefresh(win);

        bool elegirFichas=true;
        int punteroLetra=0;
        DoubleLinkedList<Ficha> *TempFichas = new DoubleLinkedList<Ficha>();

        ch=getch();
        if(ch==19){
            elegirFichas=false;
            punteroLetra=0;
            wmove(win,4,0); wclrtoeol(win);
            wmove(win,5,0); wclrtoeol(win);
            mvwprintw(win,4,2,"^");
            mvwprintw(win,5,2,"|");
            wrefresh(win);
        }
        else{
            wmove(win,10,0); wclrtoeol(win);
            wprintw(win,"Selecciona en orden las fichas pulsando Enter , cuando termines presiona Ctrl+E");
        }

        while(elegirFichas){
            ch = getch();
            if(ch==KEY_LEFT && punteroLetra>0){
                punteroLetra--;
                wmove(win,4,0); wclrtoeol(win);
                wmove(win,5,0); wclrtoeol(win);
                mvwprintw(win,4,2+2*punteroLetra,"^");
                mvwprintw(win,5,2+2*punteroLetra,"|");
                wrefresh(win);
            }
            else if(ch==KEY_RIGHT && punteroLetra<sizeFichas-1){
                punteroLetra++;
                wmove(win,4,0); wclrtoeol(win);
                wmove(win,5,0); wclrtoeol(win);
                mvwprintw(win,4,2+2*punteroLetra,"^");
                mvwprintw(win,5,2+2*punteroLetra,"|");
                wrefresh(win);
            }
            else if(ch==10){
                TempFichas->addEnd(FichasActuales->getXNode(punteroLetra));
                FichasActuales->deleteXNode(punteroLetra);
                sizeFichas=FichasActuales->getSize();
                wmove(win,3,0); wclrtoeol(win);
                for(int i=0;i<sizeFichas;i++){
                    mvwprintw(win,3,2+2*i,"%c",FichasActuales->getXNode(i).getLetra());
                }
                wmove(win,4,0); wclrtoeol(win);
                wmove(win,5,0); wclrtoeol(win);
                mvwprintw(win,4,2,"^");
                mvwprintw(win,5,2,"|");
                wrefresh(win);
                punteroLetra=0;
            }
            else if(ch==5){
                elegirFichas=false;
                wmove(win,4,0); wclrtoeol(win);
                wmove(win,5,0); wclrtoeol(win);
            }
        }

        //SE VALIDA LA PALABRA FORMADA Y SU UBICACION EN EL TABLERO
        if(ch==5){
            if(validarPalabra(TempFichas,direccion,coorX,coorY,PuntajeActual)){
                graphTablero();
            }
            else{
                wmove(win,10,0); wclrtoeol(win);
                mvwprintw(win,10,0,"Palabra no Valida, presione cualquier tecla.");
                wrefresh(win);
                //SE REGRESAN FICHAS
                for(int i=0;i<TempFichas->getSize();i++){
                    FichasActuales->addEnd(TempFichas->getXNode(i));
                }
                ch=getch();
            }
        }

        //SE SELECCIONAN FICHAS A INTERCAMBIAR
        else if(ch==19){
            wmove(win,10,0); wclrtoeol(win);
            wmove(win,12,0); wclrtoeol(win);
            mvwprintw(win,10,0,"Selecciona las fichas pulsando Enter, cuando termines presiona Ctrl+E");
            wrefresh(win);
            elegirFichas=true;
            while(elegirFichas){

                ch = getch();
                if(ch==KEY_LEFT && punteroLetra>0){
                    punteroLetra--;
                    wmove(win,4,0); wclrtoeol(win);
                    wmove(win,5,0); wclrtoeol(win);
                    mvwprintw(win,4,2+2*punteroLetra,"^");
                    mvwprintw(win,5,2+2*punteroLetra,"|");
                    wrefresh(win);
                }
                else if(ch==KEY_RIGHT && punteroLetra<sizeFichas-1){
                    punteroLetra++;
                    wmove(win,4,0); wclrtoeol(win);
                    wmove(win,5,0); wclrtoeol(win);
                    mvwprintw(win,4,2+2*punteroLetra,"^");
                    mvwprintw(win,5,2+2*punteroLetra,"|");
                    wrefresh(win);
                }
                else if(ch==10){
                    TempFichas->addEnd(FichasActuales->getXNode(punteroLetra));
                    FichasActuales->deleteXNode(punteroLetra);
                    sizeFichas=FichasActuales->getSize();
                    wmove(win,3,0); wclrtoeol(win);
                    for(int i=0;i<sizeFichas;i++){
                        mvwprintw(win,3,2+2*i,"%c",FichasActuales->getXNode(i).getLetra());
                    }
                    wmove(win,4,0); wclrtoeol(win);
                    wmove(win,5,0); wclrtoeol(win);
                    mvwprintw(win,4,2,"^");
                    mvwprintw(win,5,2,"|");
                    wrefresh(win);
                    punteroLetra=0;
                }
                else if(ch==5){
                    //SE TERMINA TURNO
                    elegirFichas=false;
                    intercambiarFichas(TempFichas,FichasActuales);
                    wmove(win,4,0); wclrtoeol(win);
                    wmove(win,5,0); wclrtoeol(win);
                }
            }

        }

        //SE INTERCAMBIA EL TURNO DEL JUGADOR
        if(apuntadorJugador==0){
            apuntadorJugador=1;
            JugadorActual=Jugador2;
            FichasActuales=FichasJugador2;
            PuntajeActual=&puntajeJugador2;
        }
        else{
            apuntadorJugador=0;
            JugadorActual=Jugador1;
            FichasActuales=FichasJugador1;
            PuntajeActual=&puntajeJugador1;
        }

        clearWin(win);
    }

    Jugador1->addpuntaje(puntajeJugador1);
    Jugador2->addpuntaje(puntajeJugador2);

    delete Tablero;
    delete ColaFichas;
    Tablero = new MatrizDispersa<Ficha>;
    ColaFichas = new Queue<Ficha>();
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
    //SE VACIA COLA DE FICHAS
    delete ColaFichas;
    ColaFichas = new Queue<Ficha>();
    //LAS FICHAS SON "QUEMADAS"
    DoubleLinkedList<Ficha> *TempFichas = new DoubleLinkedList<Ficha>();
    Ficha TempFicha ;
    //SE CREAN FICHAS DE CADA LETRA
    //A
    for(int i=0;i<12;i++){
        TempFicha.setLetra('A');
        TempFicha.setPuntos(1);
        TempFichas->addEnd(TempFicha);
    }
    //E
    for(int i=0;i<12;i++){
        TempFicha.setLetra('E');
        TempFicha.setPuntos(1);
        TempFichas->addEnd(TempFicha);
    }
    //O
    for(int i=0;i<9;i++){
        TempFicha.setLetra('O');
        TempFicha.setPuntos(1);
        TempFichas->addEnd(TempFicha);
    }
    //I
    for(int i=0;i<6;i++){
        TempFicha.setLetra('I');
        TempFicha.setPuntos(1);
        TempFichas->addEnd(TempFicha);
    }
    //S
    for(int i=0;i<6;i++){
        TempFicha.setLetra('S');
        TempFicha.setPuntos(1);
        TempFichas->addEnd(TempFicha);
    }
    //N
    for(int i=0;i<5;i++){
        TempFicha.setLetra('N');
        TempFicha.setPuntos(1);
        TempFichas->addEnd(TempFicha);
    }
    //L
    for(int i=0;i<4;i++){
        TempFicha.setLetra('L');
        TempFicha.setPuntos(1);
        TempFichas->addEnd(TempFicha);
    }
    //R
    for(int i=0;i<5;i++){
        TempFicha.setLetra('R');
        TempFicha.setPuntos(1);
        TempFichas->addEnd(TempFicha);
    }
    //U
    for(int i=0;i<5;i++){
        TempFicha.setLetra('U');
        TempFicha.setPuntos(1);
        TempFichas->addEnd(TempFicha);
    }
    //T
    for(int i=0;i<4;i++){
        TempFicha.setLetra('T');
        TempFicha.setPuntos(1);
        TempFichas->addEnd(TempFicha);
    }
    //D
    for(int i=0;i<5;i++){
        TempFicha.setLetra('D');
        TempFicha.setPuntos(2);
        TempFichas->addEnd(TempFicha);
    }
    //G
    for(int i=0;i<2;i++){
        TempFicha.setLetra('G');
        TempFicha.setPuntos(2);
        TempFichas->addEnd(TempFicha);
    }
    //C
    for(int i=0;i<4;i++){
        TempFicha.setLetra('C');
        TempFicha.setPuntos(3);
        TempFichas->addEnd(TempFicha);
    }
    //B
    for(int i=0;i<2;i++){
        TempFicha.setLetra('B');
        TempFicha.setPuntos(3);
        TempFichas->addEnd(TempFicha);
    }
    //M
    for(int i=0;i<2;i++){
        TempFicha.setLetra('M');
        TempFicha.setPuntos(3);
        TempFichas->addEnd(TempFicha);
    }
    //P
    for(int i=0;i<2;i++){
        TempFicha.setLetra('P');
        TempFicha.setPuntos(3);
        TempFichas->addEnd(TempFicha);
    }
    //H
    for(int i=0;i<2;i++){
        TempFicha.setLetra('H');
        TempFicha.setPuntos(4);
        TempFichas->addEnd(TempFicha);
    }
    //F
    for(int i=0;i<1;i++){
        TempFicha.setLetra('F');
        TempFicha.setPuntos(4);
        TempFichas->addEnd(TempFicha);
    }
    //V
    for(int i=0;i<1;i++){
        TempFicha.setLetra('V');
        TempFicha.setPuntos(4);
        TempFichas->addEnd(TempFicha);
    }
    //Y
    for(int i=0;i<1;i++){
        TempFicha.setLetra('Y');
        TempFicha.setPuntos(4);
        TempFichas->addEnd(TempFicha);
    }
    //Q
    for(int i=0;i<1;i++){
        TempFicha.setLetra('Q');
        TempFicha.setPuntos(5);
        TempFichas->addEnd(TempFicha);
    }
    //J
    for(int i=0;i<1;i++){
        TempFicha.setLetra('J');
        TempFicha.setPuntos(8);
        TempFichas->addEnd(TempFicha);
    }
    //X
    for(int i=0;i<1;i++){
        TempFicha.setLetra('X');
        TempFicha.setPuntos(8);
        TempFichas->addEnd(TempFicha);
    }
    //Z
    for(int i=0;i<1;i++){
        TempFicha.setLetra('Z');
        TempFicha.setPuntos(10);
        TempFichas->addEnd(TempFicha);
    }

    srand((unsigned) time(0));
    int randomNumber;

    //SE INGRESAN DE MANERA ALEATORIA LAS 95 FICHAS A LA COLA

    for(int i=0;i<94;i++){

        if(TempFichas->getSize()!=0){
            randomNumber = rand() % TempFichas->getSize();
            ColaFichas->push(TempFichas->getXNode(randomNumber));
            TempFichas->deleteXNode(randomNumber);
        }
        else{
            ColaFichas->push(TempFichas->getFirst());
            TempFichas->deleteXNode(0);
            //delete TempFichas;
            //break;
        }
    }


}

//FUNCIONES DEL JUEGO

bool validarPalabra(DoubleLinkedList<Ficha> *TempFichas,int direccion,int coorX,int coorY,int *ActualPuntaje){

    MatrizDispersa<Ficha> TempTablero;
    TempTablero=*Tablero;
    string palabra="";
    int CantidadFichas = TempFichas->getSize();
    NodoOrtogonal<Ficha> *TempFicha = new NodoOrtogonal<Ficha>;
    int TempPuntaje=*ActualPuntaje;

    //SE VALIDA QUE SE PUEDA INSERTAR EN EN TABLERO
    if(direccion==0){
        for(int i=0;i<CantidadFichas;i++){
            TempFicha=TempTablero.getInXY(coorX,coorY);
            if(coorY<dimensionTablero){
                if(TempFicha->getIndice()==-100){
                    TempTablero.addInXY(coorX,coorY,TempFichas->getXNode(i));
                    coorY++;
                    palabra=palabra.append(to_string(TempFichas->getXNode(i).getLetra()));
                }
                else{
                    palabra=palabra.append(to_string(TempFicha->getNodoValue().getLetra()));
                    coorY++;
                    i--;
                }
            }
            else{
                return false;
            }
        }
    }
    else{
        for(int i=0;i<CantidadFichas;i++){
            TempFicha=TempTablero.getInXY(coorX,coorY);
            if((coorX)<dimensionTablero){
                if(TempFicha->getIndice()== -100){
                    TempPuntaje=TempPuntaje+TempFichas->getXNode(i).getPuntos()*puntajeFicha(coorX,coorY);
                    TempTablero.addInXY(coorX,coorY,TempFichas->getXNode(i));
                    coorX++;
                    palabra=palabra.append(to_string(TempFichas->getXNode(i).getLetra()));
                    //AGREGAR CODIGO PARA AGARRAR UNA ULTIMA LETRA
                }
                else{
                    palabra=palabra.append(to_string(TempFicha->getNodoValue().getLetra()));
                    coorX++;
                    i--;
                }
            }
            else{
                return false;
            }
        }
    }


    //bool validacionPalabra=false;
    //for(int i=0;i<Diccionario->getSize();i++){
    //    if(Diccionario->getXNode(i).compare(palabra)==0){
    //        validacionPalabra=true;
    //        *ActualPuntaje=TempPuntaje;
    //    }
    //}
    //if(!validacionPalabra){
      //  return false;
    //}
    *ActualPuntaje=TempPuntaje;
    *Tablero=TempTablero;

    return true;

}

void intercambiarFichas(DoubleLinkedList<Ficha> *TempFichas,DoubleLinkedList<Ficha> *FichasJugador){
    int CantidadFichas = TempFichas->getSize();
    for(int i=0;i<CantidadFichas;i++){
        ColaFichas->push(TempFichas->getXNode(i));
        FichasJugador->addEnd(ColaFichas->pop());
    }
    delete TempFichas;
}

int puntajeFicha(int x,int y){

    for(int i=0;i<CasillasList->getSize();i++){
        if(CasillasList->getXNode(i).getX()==x){
            if(CasillasList->getXNode(i).getY()==y){
                return CasillasList->getXNode(i).getMultiplicador();
            }
        }
    }
    return 1;
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
    command="open ./Diccionario" + to_string(reporteDiccionario) + ".png >>/dev/null 2>>/dev/null";
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
        command="open ./ABB" + to_string(reporteArbol) + ".png >>/dev/null 2>>/dev/null";
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
        command="open ./ABBPreOrden" + to_string(reporteArbolPre) + ".png >>/dev/null 2>>/dev/null";
        system(command.c_str());
        reporteArbolPre++;
    }
}

void graphInOrden(){
    Queue<Jugador*> *AuxCola = new Queue<Jugador*>();
    string command = "";
    ofstream file;
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
        command="open ./ABBInOrden" + to_string(reporteArbolIn) + ".png >>/dev/null 2>>/dev/null";
        system(command.c_str());
        reporteArbolIn++;
    }

}

void graphPostOrden(){
    Queue<Jugador*> *AuxCola = new Queue<Jugador*>();
    string command = "";
    ofstream file;
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
        command="open /.ABBPostOrden" + to_string(reporteArbolPost) + ".png >>/dev/null 2>>/dev/null";
        system(command.c_str());
        reporteArbolPost++;
    }
}

void graphFichas(){
    Queue<Ficha> AuxCola = *ColaFichas;
    string command = "";
    ofstream file;

    int SizeCola = AuxCola.getSize();
    if(SizeCola>0){
        file.open("./Fichas" + to_string(reporteFichas) + ".dot", fstream::in | fstream::out | fstream::trunc);
        file << "digraph {";
        file << "node [shape=record];"<<endl;
        file << "rankdir=LR;"<<endl;
        file<< "struct1 [label=\" "<<endl;
        for(int i = 0 ; i <SizeCola-1;i++){
            file<<"<f"+to_string(i)+">"+ " "+ AuxCola.pick().getLetra()+"x"+to_string(AuxCola.pop().getPuntos())+"pts |"<<endl;
        }
        file<<"<f"+to_string(SizeCola)+">"+ " "+ AuxCola.pick().getLetra()+"x"+to_string(AuxCola.pop().getPuntos())+"pts \"];"<<endl;
        file << "}";
        file.close();
        command = "dot -Tpng ./Fichas" + to_string(reporteFichas) + ".dot -o Fichas" + to_string(reporteFichas) + ".png >>/dev/null 2>>/dev/null";
        system(command.c_str());
        command="open ./Fichas" + to_string(reporteFichas) + ".png >>/dev/null 2>>/dev/null";
        system(command.c_str());
        reporteFichas++;
    }
}

void graphFichasJugador(DoubleLinkedList<Ficha> *TempFichas,Jugador *TempJugador){
    string command = "";
    ofstream file;

    int SizeCola = TempFichas->getSize();
    if(SizeCola>0){
        file.open("./Fichas" + TempJugador->getNombre() + ".dot", fstream::in | fstream::out | fstream::trunc);
        file << "digraph {";
        file << "node [shape=box];"<<endl;
        file << "rankdir=LR;"<<endl;
        for(int i=0;i<SizeCola;i++){
            file<<to_string(i)+"[label = \""+TempFichas->getXNode(i).getLetra()+"\" width = 1.5 ];"<<endl;
        }
        for(int i=0;i<SizeCola;i++){
            if(i+1<SizeCola){
                file<<to_string(i)+"->"+to_string(i+1)+";"<<endl;
            }
            if(i-1>=0){
                file<<to_string(i)+"->"+to_string(i-1)+";"<<endl;
            }
        }
        file << "}";
        file.close();
        command = "dot -Tpng ./Fichas" + TempJugador->getNombre() + ".dot -o Fichas" + TempJugador->getNombre() + ".png >>/dev/null 2>>/dev/null";
        system(command.c_str());
        command="open /.Fichas" + TempJugador->getNombre() + ".png >>/dev/null 2>>/dev/null";
        system(command.c_str());
    }
}

void graphPuntajesJugador(Jugador *TempJugador){
    string command = "";
    ofstream file;
    int SizeCola = TempJugador->getPuntajes()->getSize();
    if(SizeCola>0){
        file.open("./Puntajes"+ TempJugador->getNombre()+".dot", fstream::in | fstream::out | fstream::trunc);
        file << "digraph {";
        file << "node [shape=box];"<<endl;
        file << "rankdir=LR;"<<endl;
        for(int i=0;i<SizeCola;i++){
            file<<to_string(i)+"[label = \""+to_string(TempJugador->getPuntajes()->getXNode(i))+"\" width = 1.5 ];"<<endl;
        }
        for(int i=0;i<SizeCola;i++){
            if(i+1<SizeCola){
                file<<to_string(i)+"->"+to_string(i+1)+";"<<endl;
            }
        }
        file << "}";
        file.close();
        command = "dot -Tpng ./Puntajes"+TempJugador->getNombre()+".dot -o Puntajes"+TempJugador->getNombre()+".png >>/dev/null 2>>/dev/null";
        system(command.c_str());
        command="open /.Puntajes"+TempJugador->getNombre()+".png >>/dev/null 2>>/dev/null";
        system(command.c_str());
    }
}

void graphScoreBoard(){
    Queue<Jugador*> *AuxCola = new Queue<Jugador*>();
    ArbolJugadores->preOrden(ArbolJugadores->getRaiz(),AuxCola);
    DoubleLinkedList<Jugador> *puntajes=new DoubleLinkedList<Jugador>();
    string command = "";
    ofstream file;
    int SizeCola = AuxCola->getSize();
    Jugador *AuxJugador;
    for(int i=0; i<SizeCola;i++){
        AuxJugador=AuxCola->pop();
        if(AuxJugador->getPuntajes()->getSize()>0){
            if(puntajes->getSize()==0){
                puntajes->addEnd(*AuxJugador);
            }
            else{
                int Size=puntajes->getSize();
                for(int i=0; i<Size;i++){
                    if(AuxJugador->getPuntajes()->getFirst()>puntajes->getXNode(i).getPuntajes()->getFirst()){
                        puntajes->addX(*AuxJugador,i);
                        break;
                    }
                    else if(i==puntajes->getSize()-1){
                        puntajes->addEnd(*AuxJugador);
                        break;
                    }
                }
            }
        }
    }

    SizeCola = puntajes->getSize();
    if(SizeCola>0){
        file.open("./ScoreBoard.dot", fstream::in | fstream::out | fstream::trunc);
        file << "digraph {";
        file << "node [shape=box];"<<endl;
        file << "rankdir=LR;"<<endl;
        for(int i=0;i<SizeCola;i++){
            file<<to_string(i)+"[label = \""+puntajes->getXNode(i).getNombre()+","+
            to_string(puntajes->getXNode(i).getPuntajes()->getFirst())+"\" width = 1.5 ];"<<endl;
        }
        for(int i=0;i<SizeCola;i++){
            if(i+1<SizeCola){
                file<<to_string(i)+"->"+to_string(i+1)+";"<<endl;
            }
        }
        file << "}";
        file.close();
        command = "dot -Tpng ./ScoreBoard.dot -o ScoreBoard.png >>/dev/null 2>>/dev/null";
        system(command.c_str());
        command="open /.ScoreBoard.png >>/dev/null 2>>/dev/null";
        system(command.c_str());
    }

}

void graphTablero(){
    string command = "";
    ofstream file;
    NodoOrtogonal<Ficha> *Iterador;
    NodoOrtogonal<Ficha> *Iterador2;
    Ficha TempFicha;
    int cantX=0; //CANTIDAD DE INDICES EN X
    int cantY=0; //CANTIDAD DE INDICES EN Y

    file.open("./Tablero.dot", fstream::in | fstream::out | fstream::trunc);
    file << "digraph {";
    file << "node [shape=box];"<<endl;
    file<<"Main[ label = \"Matriz\", width = 1.5, style = filled, fillcolor = firebrick1, group = 1 ];"<<endl;
    file<<"e0[ shape = point, width = 0 ];"<<endl;
    file<<"e1[ shape = point, width = 0 ];"<<endl;

    Iterador=Tablero->getHead()->getDownNodo();
    //INDICES Y ENLACES DE Y
    while(Iterador!=NULL){
        file<<"Y"+to_string(Iterador->getIndice())+"[label = \""+to_string(Iterador->getIndice())+"\" width = 1.5 style = filled, fillcolor = bisque1, group = 1 ];"<<endl;
        Iterador=Iterador->getDownNodo();
        if(Iterador!=NULL){
            cantY++;
            file<<"Y"+to_string(Iterador->getUpNodo()->getIndice())+"->Y"+to_string(Iterador->getIndice())+";"<<endl;
            file<<"Y"+to_string(Iterador->getIndice())+"->Y"+to_string(Iterador->getUpNodo()->getIndice())+";"<<endl;
        }
    }
    //INDICES Y ENLACES DE X
    Iterador=Tablero->getHead()->getRightNodo();
    while(Iterador!=NULL){
        file<<"X"+to_string(Iterador->getIndice())+"[label = \""+to_string(Iterador->getIndice())+"\" width = 1.5 style = filled, fillcolor = bisque1, group = "+to_string(Iterador->getIndice()+2)+" ];"<<endl;
        Iterador=Iterador->getRightNodo();
        if(Iterador!=NULL){
            cantX++;
            file<<"X"+to_string(Iterador->getLeftNodo()->getIndice())+"->X"+to_string(Iterador->getIndice())+";"<<endl;
            file<<"X"+to_string(Iterador->getIndice())+"->X"+to_string(Iterador->getLeftNodo()->getIndice())+";"<<endl;
        }
    }

    //ENLACES Y ALINEACION CON MAIN
    if(cantX>0||cantY>0) {
        Iterador=Tablero->getHead()->getRightNodo();
        file << "Main->X"+to_string(Iterador->getIndice())+";"<< endl;
        file << "X"+to_string(Iterador->getIndice())+"->Main;"<< endl;
        Iterador=Tablero->getHead()->getDownNodo();
        file << "Main->Y"+to_string(Iterador->getIndice())+";"<< endl;
        file << "Y"+to_string(Iterador->getIndice())+"->Main;"<< endl;
        Iterador=Tablero->getHead();
        file <<"{ rank = same; Main; ";
        for(int i=0;i<=cantX;i++){
            Iterador=Iterador->getRightNodo();
            file<<"X"+to_string(Iterador->getIndice())+"; ";
        }
        file <<"}"<<endl;
    }

    //SE ESCRIBEN TODOS LOS NODOS DE LAS FICHAS
    Iterador=Tablero->getHead()->getDownNodo();
    while(Iterador!=NULL){
        Iterador2 = Iterador->getRightNodo();
        while(Iterador2!=NULL){
            TempFicha=Iterador2->getNodoValue();
            file<<"X"+to_string(Iterador2->getX())+"_Y"+to_string(Iterador2->getY())+
                  "[label = \""+TempFicha.getLetra()+"\" width = 1.5 , group = "+to_string(Iterador2->getX()+2)+" ];"<<endl;
            Iterador2= Iterador2->getRightNodo();
        }
        Iterador=Iterador->getDownNodo();
    }

    //SE CREAN TODOS LOS ENLACES DE LAS FICHAS
    Iterador=Tablero->getHead()->getDownNodo();
    while(Iterador!=NULL){
        Iterador2= Iterador->getRightNodo();
        while(Iterador2!=NULL){
            if(Iterador2->getUpNodo()!=NULL){
                if(Iterador2->getUpNodo()->getIndice()==-1){
                    file<<"X"+to_string(Iterador2->getX())+"_Y"+to_string(Iterador2->getY())+"->"+
                    "X"+to_string(Iterador2->getUpNodo()->getX())+"_Y"+to_string(Iterador2->getUpNodo()->getY())+";"<<endl;
                }
                else{
                    file<<"X"+to_string(Iterador2->getUpNodo()->getIndice())+
                    "->X"+to_string(Iterador2->getX())+"_Y"+to_string(Iterador2->getY())+";"<<endl;
                    file<<"X"+to_string(Iterador2->getX())+"_Y"+to_string(Iterador2->getY())+"->"+
                    "X"+to_string(Iterador2->getUpNodo()->getIndice())+";"<<endl;
                }
            }
            if(Iterador2->getRightNodo()!=NULL){
                file<<"X"+to_string(Iterador2->getX())+"_Y"+to_string(Iterador2->getY())+"->"+
                "X"+to_string(Iterador2->getRightNodo()->getX())+"_Y"+to_string(Iterador2->getRightNodo()->getY())+";"<<endl;;
            }
            if(Iterador2->getDownNodo()!=NULL){
                file<<"X"+to_string(Iterador2->getX())+"_Y"+to_string(Iterador2->getY())+"->"+
                "X"+to_string(Iterador2->getDownNodo()->getX())+"_Y"+to_string(Iterador2->getDownNodo()->getY())+";"<<endl;;
            }
            if(Iterador2->getLeftNodo()!=NULL){
                if(Iterador2->getLeftNodo()->getIndice()==-1){
                    file<<"X"+to_string(Iterador2->getX())+"_Y"+to_string(Iterador2->getY())+"->"+
                    "X"+to_string(Iterador2->getLeftNodo()->getX())+"_Y"+to_string(Iterador2->getLeftNodo()->getY())+";"<<endl;;
                }
                else{
                    file<<"Y"+to_string(Iterador2->getLeftNodo()->getIndice())+
                    "->X"+to_string(Iterador2->getX())+"_Y"+to_string(Iterador2->getY())+";"<<endl;
                    file<<"X"+to_string(Iterador2->getX())+"_Y"+to_string(Iterador2->getY())+"->"+
                    "Y"+to_string(Iterador2->getLeftNodo()->getIndice())+";"<<endl;;
                }
            }
            Iterador2= Iterador2->getRightNodo();
        }
        Iterador=Iterador->getDownNodo();
    }

    //SE ESCRIBEN LAS ETIQUETAS RANKSAME
    Iterador=Tablero->getHead();
    for(int i =0;i<=cantY;i++){
        Iterador=Iterador->getDownNodo();
        Iterador2= Iterador->getRightNodo();
        file <<"{ rank = same; Y"+to_string(Iterador->getIndice())+";";
        while(Iterador2!=NULL){
            file<<"X"+to_string(Iterador2->getX())+"_Y"+to_string(Iterador2->getY())+"; ";
            Iterador2= Iterador2->getRightNodo();
        }
        file <<"}"<<endl;
    }




    file << "}";
    file.close();
    command = "dot -Tpng ./Tablero.dot -o Tablero.png >>/dev/null 2>>/dev/null";
    system(command.c_str());
    command="open ./Tablero.png >>/dev/null 2>>/dev/null";
    system(command.c_str());
}



