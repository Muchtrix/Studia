#ifndef PUNKTY_H_INCLUDED
#define PUNKTY_H_INCLUDED

/** Typ punktu œcie¿ki **/
typedef struct _pkt* punkt;
typedef struct _pkt {
    double szerokosc;
    double dlugosc;
    double wysokosc;
    int czas;
    punkt nastepny;
} pkt;

/** Typ waypoint **/
typedef struct _wpkt* waypoint;
typedef struct _wpkt {
    double szerokosc;
    double dlugosc;
    int s1;
    int s2;
    waypoint nastepny;
} wpt;

/** Typ zawierajacy graniczne wspó³rzêdne terenu **/
typedef struct _mapa {
    double minSzer;
    double maxSzer;
    double minDlug;
    double maxDlug;
} mapa;

/** Operacje na punktach **/
punkt dodajPunkt(punkt lista, double sz, double dl, double ws, int cz);
void skasujListe(punkt lista);
void mapuj(punkt lista, mapa* teren, int szer);
double liczDroge(punkt pocz, punkt koniec);
int znajdzIndeks(punkt pocz, punkt koniec);
int szukajPrzeciecia(punkt f1, punkt f2,double *dl,double *sz);

/** Operacje na waypointach **/
waypoint dodajWaypoint(waypoint lista, double sz, double dl, int je, int dw);
void skasujWaypoint(waypoint lista);
void mapujWaypoint(waypoint lista, mapa* teren, int szer);
double wOdl(waypoint w1, waypoint w2);
punkt znajdzNajblizszy(waypoint szukany, punkt lista);

/** Operacje na danych obszaru (mapie) **/
mapa inicjujMape();
void daneMapy(mapa* teren, waypoint lista);
void daneMapyS(mapa* teren, punkt lista);
void marginesMapy(mapa* teren);
double proporcje(mapa* teren);

#endif
