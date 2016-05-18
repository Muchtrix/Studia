#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "punkty.h"
#include "obsluga.h"
#include "plik.h"

GtkWidget* Okno;
GtkWidget* PasekStanu;
GtkWidget* Rysunek;
GtkWidget* OknoInfo = NULL;
punkt* sciezki = NULL;
int ilePlikow;
waypoint przeciecia = NULL;
mapa teren;

void rysowanie(GtkWidget *widget, cairo_t *cr, gpointer data){
    punkt _lista = NULL;
    waypoint _prz = przeciecia;
    int nrPunktu = 1;
    char tekst[5];

    /** Ustawienie tła **/
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    /** Ustawienia lini **/
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
    cairo_set_line_width(cr, 3);
    cairo_set_source_rgb(cr, 1, 0.6, 0);

    /** Rysowanie ścieżki **/
    for(int i = 0; i < ilePlikow; ++i){
        _lista = sciezki[i];
        while (_lista != NULL){
            cairo_line_to(cr, _lista-> dlugosc, _lista->szerokosc);
            _lista = _lista -> nastepny;
        }
        cairo_stroke(cr);
    }

    /** Rysowanie grafu połączeń **/
    cairo_set_source_rgb(cr, 0, 0.6, 1);
    if(przeciecia != NULL)
    for(waypoint tmp1 = przeciecia; tmp1->nastepny != NULL; tmp1 = tmp1->nastepny){
        if(tmp1->s1 == tmp1->nastepny->s1){
            punkt a = znajdzNajblizszy(tmp1, sciezki[tmp1->s1]);
            int ia = znajdzIndeks(sciezki[tmp1->s1], a);
            punkt b = znajdzNajblizszy(tmp1->nastepny, sciezki[tmp1->s1]);
            int ib = znajdzIndeks(sciezki[tmp1->s1], b);
            if(ia >ib){
                punkt c = a;
                a = b;
                b = c;
            }
            if(tmp1->s2 == tmp1->nastepny->s2){
                punkt a1 = znajdzNajblizszy(tmp1, sciezki[tmp1->s2]);
                int ia1 = znajdzIndeks(sciezki[tmp1->s2], a1);
                punkt b1 = znajdzNajblizszy(tmp1->nastepny, sciezki[tmp1->s2]);
                int ib1 = znajdzIndeks(sciezki[tmp1->s2], b1);
                if(ia1 >ib1){
                    punkt c1 = a1;
                    a1 = b1;
                    b1 = c1;
                }
                if(liczDroge(a1,b1) < liczDroge(a,b)){
                    a = a1;
                    b = b1;
                }
            }
            while (a != b){
                cairo_line_to(cr, a-> dlugosc, a->szerokosc);
                a = a -> nastepny;
            }
            cairo_stroke(cr);
        }
    }

    /** Rysowanie waypointów **/
    while (_prz != NULL){
        sprintf(tekst, "%d", nrPunktu);
        cairo_set_source_rgb(cr, 1, 0.85, 0);
        cairo_set_line_width(cr, 2);
        cairo_rectangle(cr, _prz->dlugosc - 10, _prz->szerokosc - 5,
                        10, 10);
        cairo_stroke_preserve(cr);
        cairo_set_source_rgb(cr, 1, 1, 0);
        cairo_fill(cr);

        cairo_set_source_rgb(cr, 1, 0, 0);
        cairo_move_to(cr, _prz->dlugosc - 10, _prz->szerokosc + 3);
        cairo_text_path(cr, tekst);
        cairo_fill(cr);

        ++nrPunktu;
        _prz = _prz -> nastepny;
    }
}


void zakonczenie(){
    skasujWaypoint(przeciecia);
    free(sciezki);
}

void dzialanie(){

    /** Utworzenie okienka z informacjami **/
    if(OknoInfo != NULL){
        gtk_widget_destroy(OknoInfo);
        OknoInfo = NULL;
    }
    OknoInfo = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GdkPixbuf *Ikona = gdk_pixbuf_new_from_file("dane/ikona.png", NULL);
    gtk_window_set_icon(GTK_WINDOW(Okno), Ikona);
    gtk_window_set_title(GTK_WINDOW(OknoInfo), "Ścieżki");
    gint posx, posy;
    gtk_window_get_position(GTK_WINDOW(Okno), &posx, &posy);
    gtk_window_move(GTK_WINDOW(OknoInfo), posx-235, posy+5);
    gtk_window_set_resizable(GTK_WINDOW(OknoInfo), FALSE);
    gtk_widget_set_size_request(OknoInfo, 215,300);

    GtkWidget* Tekst = gtk_text_view_new();
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(Tekst), FALSE);
    GtkWidget* pasekPrzew = gtk_scrolled_window_new(NULL,NULL);
    gtk_container_add(GTK_CONTAINER(OknoInfo), pasekPrzew);
    gtk_container_add(GTK_CONTAINER(pasekPrzew), Tekst);
    GtkTextBuffer *bufor = gtk_text_view_get_buffer(GTK_TEXT_VIEW(Tekst));
    GtkTextIter iter; gtk_text_buffer_get_iter_at_offset(bufor, &iter, 0);
    gtk_text_buffer_insert(bufor, &iter, "Ścieżka\t\tOdległość (km)\n", -1);



    gtk_widget_show_all(OknoInfo);
    /** Szukanie przecięć **/
    double x,y;
    for(int i = 0; i < ilePlikow; ++i){
        for(int j = i+1; j < ilePlikow; ++j){
            punkt tmp = sciezki[i];
            while(tmp->nastepny != NULL){
                punkt tmp2 = sciezki[j];
                while(tmp2->nastepny != NULL){
                    if(szukajPrzeciecia(tmp, tmp2, &x, &y))
                        przeciecia = dodajWaypoint(przeciecia, y,x,i,j);
                    tmp2 = tmp2->nastepny;
                }
                tmp = tmp -> nastepny;
            }
        }
    }

    /** Redukcja przecięć zbyt blisko siebie, między tymi samymi ścieżkami **/
    if(przeciecia != NULL)
    for(waypoint tmp = przeciecia; tmp->nastepny != NULL;){
        if(tmp->s1 == tmp->nastepny->s1 && tmp->s2 == tmp->nastepny->s2 &&
           wOdl(tmp, tmp->nastepny) < 0.01){
            waypoint a = tmp->nastepny->nastepny;
            free(tmp->nastepny);
            tmp->nastepny = a;
           }
        else tmp = tmp->nastepny;
    }

    /** Liczenie odległości między przecięciami **/
    int nrPrz = 1;
    if(przeciecia != NULL)
    for(waypoint tmp = przeciecia; tmp->nastepny != NULL; tmp = tmp->nastepny){
        int nrPrz2 = nrPrz+1;
        for(waypoint tmp2 = tmp->nastepny; tmp2 != NULL; tmp2 = tmp2->nastepny){
            double droga = 40000000.0;
            if(tmp->s1 == tmp2->s1 || tmp->s1 == tmp2->s2){
                punkt a = znajdzNajblizszy(tmp, sciezki[tmp->s1]);
                int ia = znajdzIndeks(sciezki[tmp->s1], a);
                punkt b = znajdzNajblizszy(tmp2, sciezki[tmp->s1]);
                int ib = znajdzIndeks(sciezki[tmp->s1], b);
                if(ia >ib){
                    punkt c = a;
                    a = b;
                    b = c;
                }
                droga = liczDroge(a,b);
            }else if(tmp->s2 == tmp2->s1 || tmp->s2 == tmp2->s2){
                punkt a = znajdzNajblizszy(tmp, sciezki[tmp->s2]);
                int ia = znajdzIndeks(sciezki[tmp->s2], a);
                punkt b = znajdzNajblizszy(tmp2, sciezki[tmp->s2]);
                int ib = znajdzIndeks(sciezki[tmp->s2], b);
                if(ia >ib){
                    punkt c = a;
                    a = b;
                    b = c;
                }
                if (droga > liczDroge(a,b)) droga = liczDroge(a,b);
            }
            char linijka[25];
            sprintf(linijka,"%3d --> %3d\t%.3f\n", nrPrz, nrPrz2, droga);
            if (droga != 40000000.0)
                gtk_text_buffer_insert(bufor, &iter, linijka, -1);
            ++nrPrz2;
        }
        ++nrPrz;
    }

    /** Ustalenie granic rysunku **/
    teren = inicjujMape();
    if (przeciecia != NULL) daneMapy(&teren, przeciecia);
    else for(int i = 0; i < ilePlikow; ++i) daneMapyS(&teren, sciezki[i]);
    marginesMapy(&teren);

    /** Przekształcenie wsp geogr. na rysunku **/
    for(int i = 0; i < ilePlikow; ++i)
        mapuj(sciezki[i], &teren, SZEROKOSC);
    mapujWaypoint(przeciecia, &teren, SZEROKOSC);

    /** Rysowanie **/
    g_signal_connect(Rysunek, "draw", G_CALLBACK(rysowanie),NULL);
    gtk_widget_set_size_request(Rysunek, SZEROKOSC, 1.1*SZEROKOSC*proporcje(&teren));
}

void pokazInfo(GtkWidget *widget, gpointer data){
    GdkPixbuf *_Obraz = gdk_pixbuf_new_from_file("dane/ikona.png", NULL);
    GdkPixbuf *Obraz = gdk_pixbuf_scale_simple(_Obraz, 64, 64, GDK_INTERP_BILINEAR);
    g_object_unref(_Obraz);

    GtkWidget *Okno = gtk_about_dialog_new();
    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(Okno), "GiPeX");
    gtk_window_set_icon(GTK_WINDOW(Okno), Obraz);
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(Okno), WERSJA);
    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(Okno), "(c) Wiktor Adamski");
    gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(Okno), Obraz);

    g_object_unref(Obraz);
    gtk_dialog_run(GTK_DIALOG(Okno));
    gtk_widget_destroy(Okno);
}

void otwieraniePliku(GtkWidget *widget, gpointer data){
    /** Filtr wyszukiwania plików **/
    GtkFileFilter *filtr = gtk_file_filter_new();
    gtk_file_filter_set_name(filtr, "GPX (*.gpx)");
    gtk_file_filter_add_pattern(filtr, "*.gpx");

    /** Okno wyszukiwania **/
    GtkWidget *OknoWyboru = gtk_file_chooser_dialog_new("Otwórz plik", NULL, GTK_FILE_CHOOSER_ACTION_OPEN,
                                                        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                                        GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);
    gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(OknoWyboru), TRUE);
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(OknoWyboru), filtr);

    if(gtk_dialog_run(GTK_DIALOG(OknoWyboru)) == GTK_RESPONSE_ACCEPT){
        GSList* pliki = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(OknoWyboru));
        if(sciezki != NULL){
            for(int i = 0; i < ilePlikow; ++i)
                skasujListe(sciezki[i]);
            free(sciezki); sciezki = NULL;
            skasujWaypoint(przeciecia); przeciecia = NULL;
        }
        ilePlikow = 0;
        for(GSList* tmp = pliki; tmp != NULL; tmp = tmp->next)
            ++ilePlikow;
        sciezki = malloc(sizeof(punkt)*ilePlikow);
        for (int i = 0; i < ilePlikow; ++i)
            sciezki[i] = NULL;

        int i = 0;
        for(GSList* tmp = pliki; tmp != NULL; tmp = tmp->next){
            odczyt((char*) tmp->data, &sciezki[i]);
            ++i;

        }
        g_slist_free(pliki);
        dzialanie();

    }
    int prz = 0;
    for(waypoint tmp = przeciecia; tmp != NULL; tmp = tmp-> nastepny)
        ++prz;
    char tp[57];
    sprintf(tp,"Wczytano plików: %d, punktów przecięcia: %d", ilePlikow, prz);
    gtk_statusbar_push(GTK_STATUSBAR(PasekStanu), 1, tp);
    gtk_widget_destroy(OknoWyboru);
}

void glowna(int argc, char** argv){
/*****************************************************************************/
/*                            INTERFEJS GTK                                  */
/*****************************************************************************/

    /** Deklaracje i inicjalizacje widgetów **/
    GtkWidget *Pudelko;
    GtkWidget *Pasek;

    GtkWidget *PasekMenu;
    GtkWidget *PlikMenu;
    GtkWidget *InfoMenu;
    GtkWidget *Plik;
    GtkWidget *Informacje;
    GtkWidget *Otworz;
    GtkWidget *Sep;
    GtkWidget *Wyjscie;
    GtkWidget *OProgramie;



    gtk_init(&argc, &argv);
    GdkPixbuf *Ikona = gdk_pixbuf_new_from_file("dane/ikona.png", NULL);

    Okno = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(Okno, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    Pudelko = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
    Pasek = gtk_scrolled_window_new(NULL,NULL);

    PasekMenu = gtk_menu_bar_new();

    PlikMenu = gtk_menu_new();
    Plik = gtk_menu_item_new_with_mnemonic("_Plik");
    Otworz = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, NULL);
    g_signal_connect(Otworz, "activate", G_CALLBACK(otwieraniePliku), NULL);
    Sep = gtk_separator_menu_item_new();
    Wyjscie = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE, NULL);
    g_signal_connect(Wyjscie, "activate", G_CALLBACK(gtk_main_quit), NULL);

    InfoMenu = gtk_menu_new();
    Informacje = gtk_menu_item_new_with_mnemonic("_Informacje");
    OProgramie = gtk_image_menu_item_new_from_stock(GTK_STOCK_ABOUT, NULL);
    g_signal_connect(OProgramie, "activate", G_CALLBACK(pokazInfo), NULL);

    PasekStanu = gtk_statusbar_new();
    gtk_statusbar_push(GTK_STATUSBAR(PasekStanu), 0, "Wybierz plik poleceniem \"Plik->Otwórz\"");

    Rysunek = gtk_drawing_area_new();

    /** Pakowanie widgetów, nadanie parametrów **/
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(Plik), PlikMenu);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(Informacje), InfoMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(PlikMenu), Otworz);
    gtk_menu_shell_append(GTK_MENU_SHELL(PlikMenu), Sep);
    gtk_menu_shell_append(GTK_MENU_SHELL(PlikMenu), Wyjscie);
    gtk_menu_shell_append(GTK_MENU_SHELL(InfoMenu), OProgramie);
    gtk_menu_shell_append(GTK_MENU_SHELL(PasekMenu), Plik);
    gtk_menu_shell_append(GTK_MENU_SHELL(PasekMenu), Informacje);

    gtk_container_add(GTK_CONTAINER(Okno),Pudelko);
    gtk_box_pack_start(GTK_BOX(Pudelko), PasekMenu, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(Pudelko), Pasek, TRUE, TRUE, 0);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(Pasek), Rysunek);
    gtk_box_pack_start(GTK_BOX(Pudelko), PasekStanu, FALSE, FALSE, 0);

    gtk_window_set_default_size(GTK_WINDOW(Okno),(int) SZEROKOSC,(int) SZEROKOSC+120);
    gtk_window_set_position(GTK_WINDOW(Okno), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(Okno), "GiPeX");
    gtk_window_set_icon(GTK_WINDOW(Okno), Ikona);

    gtk_widget_show_all(Okno);
    gtk_main();

    zakonczenie();
}
