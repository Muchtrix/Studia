#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "punkty.h"

#define PROMIEN 6371
#define DEG2RAD 0.0174532925199433d

/*****************************************************************************/
/**                       Operacje na punktach                              **/
/*****************************************************************************/
punkt dodajPunkt(punkt lista, double sz, double dl, double ws, int cz){
    if(lista == NULL){
        lista = malloc(sizeof(pkt));
        if(lista == NULL){
           //printf("Brak pamieci");
            return NULL;
        }
        lista -> szerokosc = sz;
        lista -> dlugosc = dl;
        lista -> wysokosc = ws;
        lista -> czas = cz;
        lista -> nastepny = NULL;
    } else lista -> nastepny = dodajPunkt(lista->nastepny, sz, dl, ws, cz);
    return lista;
}

void skasujListe(punkt lista){
    if(lista != NULL){
        skasujListe(lista -> nastepny);
        free(lista);
    }
}

void mapuj(punkt lista, mapa*teren, int szer){
    double hskala = szer/(teren -> maxDlug - teren -> minDlug)*1.0;
    double vskala = hskala*proporcje(teren);
    while(lista != NULL){
        lista -> szerokosc = (lista -> szerokosc - teren -> minSzer)* vskala;
        lista -> dlugosc = (lista -> dlugosc - teren -> minDlug)* hskala;
        lista = lista -> nastepny;
    }
}

int znajdzIndeks(punkt pocz, punkt koniec){
    int wyn = 0;
    for(;pocz != koniec; pocz = pocz->nastepny) ++wyn;
    return wyn;
}

double liczDroge(punkt pocz, punkt koniec){
    double wynik = 0.0;
    for(punkt akt = pocz; akt->nastepny != NULL && akt != koniec; akt = akt->nastepny){
        double ddlu = (akt->dlugosc - akt->nastepny->dlugosc) * DEG2RAD;
        double dszer = (akt->szerokosc - akt->nastepny->szerokosc) * DEG2RAD;

        double a = pow(sin(dszer/2),2) + cos(akt->szerokosc *DEG2RAD) *
                   cos(akt->szerokosc *DEG2RAD) * pow(sin(ddlu/2), 2);
        double poziom = 2*PROMIEN*atan2(sqrt(a), sqrt(1-a));
        wynik += poziom; //sqrt(pow(poziom,2) + pow(pocz->wysokosc - pocz->nastepny->wysokosc,2));
    }
    return wynik;
}

int szukajPrzeciecia(punkt f1, punkt f2,double *dl,double *sz){
    double p0_x = f1->dlugosc, p0_y = f1->szerokosc;
    double p1_x = f1->nastepny->dlugosc, p1_y = f1->nastepny->szerokosc;
    double p2_x = f2->dlugosc, p2_y = f2->szerokosc;
    double p3_x = f2->nastepny->dlugosc, p3_y = f2->nastepny->szerokosc;

    double s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    double s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        if (dl != NULL)
            *dl = p0_x + (t * s1_x);
        if (sz != NULL)
            *sz = p0_y + (t * s1_y);
        return 1;
    }

    return 0;
}

/*****************************************************************************/
/**                  Operacje na punktach przeciecia                        **/
/*****************************************************************************/
waypoint dodajWaypoint(waypoint lista, double sz, double dl, int je, int dw){
    if(lista == NULL){
        lista = malloc(sizeof(wpt));
        if(lista == NULL){
           // printf("Brak pamieci");
            return NULL;
        }
        lista -> szerokosc = sz;
        lista -> dlugosc = dl;
        lista -> s1 = je; lista -> s2 = dw;
        lista -> nastepny = NULL;
    } else lista -> nastepny = dodajWaypoint(lista->nastepny, sz, dl, je, dw);
    return lista;
}

void skasujWaypoint(waypoint lista){
    if(lista != NULL){
        skasujWaypoint(lista -> nastepny);
        free(lista);
    }
}

void mapujWaypoint(waypoint lista, mapa* teren, int szer){
    double hskala = szer/(teren -> maxDlug - teren -> minDlug)*1.0;
    double vskala = hskala*proporcje(teren);
    while(lista != NULL){
        lista -> szerokosc = (lista -> szerokosc - teren -> minSzer)* vskala;
        lista -> dlugosc = (lista -> dlugosc - teren -> minDlug)* hskala;
        lista = lista -> nastepny;
    }
}

double wOdl(waypoint w1, waypoint w2){
    double y1 = w1->szerokosc;
    double x1 = w1->dlugosc;
    double y2 = w2->szerokosc;
    double x2 = w2->dlugosc;

    double ddlu = (x1 - x2) * DEG2RAD;
    double dszer = (y1 - y2) * DEG2RAD;

    double a = pow(sin(dszer/2),2) + cos(y1 *DEG2RAD) *
                   cos(y1 *DEG2RAD) * pow(sin(ddlu/2), 2);
    return 2*PROMIEN*atan2(sqrt(a), sqrt(1-a));
}

punkt znajdzNajblizszy(waypoint szukany, punkt lista){
    double y1 = szukany->szerokosc * DEG2RAD;
    double x1 = szukany->dlugosc * DEG2RAD;
    punkt bliski = NULL;
    double min = 40000000.0;
    for(punkt akt = lista; akt != NULL; akt = akt->nastepny){
        double y2 = akt->szerokosc * DEG2RAD;
        double x2 = akt->dlugosc * DEG2RAD;
        double a = pow(sin((y2-y1)/2.0),2);
        double b = pow(sin((x2-x1)/2.0),2);
        double c = sqrt(a + cos(y2)*cos(y1)*b);
        double poziom = 2*asin(c)*PROMIEN;
        if(poziom < min){
            min = poziom;
            bliski = akt;
        }
    }
    return bliski;
}

/*****************************************************************************/
/**                          Opreacje na obszarze                           **/
/*****************************************************************************/
mapa inicjujMape(){
    mapa teren;
    teren.minSzer = 90;
    teren.maxSzer = -90;
    teren.minDlug = 180;
    teren.maxDlug = -180;
    return teren;
}

void daneMapy(mapa* teren, waypoint lista){
    while(lista != NULL){
        if(lista -> szerokosc < teren -> minSzer) teren -> minSzer = lista -> szerokosc;
        if(lista -> szerokosc > teren -> maxSzer) teren -> maxSzer = lista -> szerokosc;
        if(lista -> dlugosc < teren -> minDlug) teren -> minDlug = lista -> dlugosc;
        if(lista -> dlugosc > teren -> maxDlug) teren -> maxDlug = lista -> dlugosc;
        lista =  lista -> nastepny;
    }
}

void daneMapyS(mapa* teren, punkt lista){
    while(lista != NULL){
        if(lista -> szerokosc < teren -> minSzer) teren -> minSzer = lista -> szerokosc;
        if(lista -> szerokosc > teren -> maxSzer) teren -> maxSzer = lista -> szerokosc;
        if(lista -> dlugosc < teren -> minDlug) teren -> minDlug = lista -> dlugosc;
        if(lista -> dlugosc > teren -> maxDlug) teren -> maxDlug = lista -> dlugosc;
        lista =  lista -> nastepny;
    }
}

void marginesMapy(mapa* teren){
    double pionowy = (teren -> maxSzer - teren -> minSzer) * 0.1;
    double poziomy = (teren -> maxDlug - teren -> minDlug) * 0.1;
    teren -> maxSzer += pionowy;
    teren -> minSzer -= pionowy;
    teren -> maxDlug += poziomy;
    teren -> minDlug -= poziomy;
}

double proporcje(mapa* teren){
    double pion = (teren ->maxSzer - teren ->minSzer);
    double poziom = (teren ->maxDlug - teren ->minDlug);
    if (pion/poziom > 0) return pion/poziom;
    else return 1;
}
