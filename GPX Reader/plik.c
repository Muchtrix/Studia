#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "plik.h"

//extern punkt lista;

struct line_reader {
	FILE	*f;
	char	*buf;
	size_t	 siz;
};

/*
 * Inicjalizacja line reader _lr_ dla strumienia _f_.
 */
void lr_init(struct line_reader *lr, FILE *f){
	lr->f = f;
	lr->buf = NULL;
	lr->siz = 0;
}

char *next_line(struct line_reader *lr, size_t *len){
	size_t newsiz;
	int c;
	char *newbuf;

	*len = 0;
	while(1) {
		c = fgetc(lr->f);
		if (ferror(lr->f))
			return NULL;

		if (c == EOF) {
			if (*len == 0)
				return NULL;
			else
				return lr->buf;
		} else {
			if (*len == lr->siz) {
				newsiz = lr->siz + 4096;
				newbuf = realloc(lr->buf, newsiz);
				if (newbuf == NULL)
					return NULL;
				lr->buf = newbuf;
				lr->siz = newsiz;
			}
			lr->buf[(*len)++] = c;
			if (c == '\n'){
                lr->buf[--(*len)] = '\0';
				return lr->buf;
			}
		}
	}
}

/*
 * uwalnia pamiec zajmowana przez _lr_
 */
void lr_free(struct line_reader *lr){
	free(lr->buf);
	lr->buf = NULL;
	lr->siz = 0;
}

double szukajLiczby(char* napis){
    double wynik = 0.0, mnoznik = 0.1;
    char *akt = napis;
    while(!isdigit(*akt)) ++akt; //do liczby
    while(isdigit(*akt)){ //czesc calkowita
        int tmp = *akt - '0';
        wynik *= 10;
        wynik += tmp; ++akt;
    }
    if(*akt == '.'){
        ++akt;
        while(isdigit(*akt)){ //czesc dziesietna
            int tmp = *akt - '0';
            wynik += tmp*mnoznik; ++akt;
            mnoznik *=0.1;
        }
    }
    return wynik;
}

int szukajCzasu(char*napis){
    int wynik = 0, tmp = 0;
    char *akt = napis;
    while(*akt != 'T') ++akt;
    ++akt;
    for(int i = 0; i < 3; ++i){
        while(isdigit(*akt)){
            tmp *= 10;
            tmp += *akt - '0';
            ++akt;
        }
        wynik *= 60;
        wynik += tmp;
        tmp = 0; ++akt;
    }
    return wynik;
}

void szukajOpisu(char* napis, char* res){
    char *akt = napis;
    while(*akt != '>') ++akt;
    ++akt;
    char *koniec = akt;
    while (*koniec != '<') ++koniec;
    strncpy(res, akt, koniec-akt);
}

/* Na podstawie:
 * Read a file line by line.
 * http://rosettacode.org/wiki/Read_a_file_line_by_line
 */
void odczyt(char* sciezka, punkt* lista){
	struct line_reader lr;
	FILE *f;
	size_t len;
	char *line;

	f = fopen(sciezka, "r");
	if (f == NULL) {
		printf("BLAD OTWARCIA PLIKU");
		return;
	}

	lr_init(&lr, f);
	while ((line = next_line(&lr, &len))) {
		if(strstr(line, "<rtept") != NULL ||
           strstr(line, "<trkpt") != NULL ){
            double sz = szukajLiczby(line);
            double dl = szukajLiczby(strstr(line,"lon"));
            double ws = 0.0;
            int czas = 0;
            while(strstr(line, "</rtept>") == NULL &&
                  strstr(line, "</trkpt>") == NULL ){
                if(strstr(line, "<ele>") != NULL)
                    ws = szukajLiczby(line);
                if(strstr(line, "<time>") != NULL)
                    czas = szukajCzasu(line);
                line = next_line(&lr,&len);
            }
            *lista = dodajPunkt(*lista, -sz, dl, ws, czas);
		}
	}
	lr_free(&lr);

	return;
}
