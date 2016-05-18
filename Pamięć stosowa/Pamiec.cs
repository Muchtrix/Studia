//---------------------------------------------------------------------------//
// Wiktor Adamski                                                             //
// Nr indeksu 27220                                                           //
// Systemy operacyjne - Pamięć stosowa                                        //
//---------------------------------------------------------------------------//

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PamiecStosowa {
    class Pamiec {
        List<Stos> stosy = new List<Stos>();
        public int[] dane;
        int niePrzydzielono;
        Random gen = new Random();

        /// <summary>
        /// Tworzy nową pamięć.
        /// </summary>
        /// <param name="rozmiar">Liczba komórek pamięci</param>
        /// <param name="ileStosow">
        /// Liczba stosów na które pamięć jest podzielona.
        /// </param>
        public Pamiec(int rozmiar, int ileStosow) {
            //Inicjacja pamięci
            dane = new int[rozmiar];

            //Utworzenie stosów
            for (int i = 0; i < rozmiar; ++i) dane[i] = 0;
            int rozmiarStosu = rozmiar / ileStosow;
            for(int i = 0; i < ileStosow; ++i)
                stosy.Add(new Stos(i * rozmiarStosu, rozmiarStosu));

            //Obliczenie nieprzydzielonej pamięci
            niePrzydzielono = rozmiar - rozmiarStosu * ileStosow;

        }

        /// <summary>
        /// Dodaje nową losową danę do losowo wybranego stosu.
        /// </summary>
        /// <returns></returns>
        public bool DodajDana() {
            int dana = gen.Next(9)+1;
            int rozmiar = gen.Next(3) + 1;
            int st = gen.Next(stosy.Count);
            return DodajKonkretDana(dana, rozmiar, st);
        }

        /// <summary>
        /// Dodaje daną do określonego stosu.
        /// </summary>
        /// <param name="dana">Liczba do zapamiętania</param>
        /// <param name="rozmiar">Liczba komórek pamięci do zajęcia</param>
        /// <param name="st">Numer stosu do którego dodajemy danę.</param>
        /// <returns></returns>
        public bool DodajKonkretDana(int dana, int rozmiar, int st) {
            while (true)
            {
                int adr = stosy[st].Dodaj(rozmiar);
                if (adr >= 0)
                {
                    for (int i = 0; i < rozmiar; ++i)
                    {
                        dane[i + adr] = dana;
                    }
                    return true;
                }
                else if (!Reorganizacja(rozmiar, st))
                    return false;
            }
        }

        /// <summary>
        /// Reorganizuje pamięć w celu umożliwienia dalszej eksploatacji.
        /// </summary>
        /// <param name="wymagane">
        /// Liczba komórek pamięci wymaganych w stosie nrStosu.</param>
        /// <param name="nrStosu">Numer stosu w którym wystąpił brak miejsca.
        /// </param>
        /// <returns></returns>
        private bool Reorganizacja(int wymagane, int nrStosu) {
            Console.WriteLine("Reorganizacja pamięci!");
            int pozostalaPamiec = (from s in stosy
                             select s.pozostalo).Sum() + niePrzydzielono;
            double calkowityPrzyrost = (from s in stosy
                                        select s.przyrost).Sum();

            if (wymagane > pozostalaPamiec) return false;

            int poczatek = 0;
            int[,] noweIndeksy = new int[stosy.Count,2];
            pozostalaPamiec -= wymagane;
            noweIndeksy[nrStosu, 1] = wymagane;

            //Wyliczenie nowych wskaźników
            for (int i = 0; i < stosy.Count; ++i)
            {
                int wielkosc = (int)(stosy[i].przyrost * 
                                          pozostalaPamiec / calkowityPrzyrost);
                noweIndeksy[i, 0] = poczatek;
                noweIndeksy[i, 1] += wielkosc+stosy[i].rozmiar;
                poczatek += noweIndeksy[i, 1];
            }

            //Sprawdzenie czy nowy przydział jest możliwy
            niePrzydzielono = dane.Length - (noweIndeksy[stosy.Count-1, 0] +
                                             noweIndeksy[stosy.Count-1, 1]);
            if (niePrzydzielono < 0) return false;

            //Przeniesienie zawartości pamięci
            int ostatnia = dane.Length;
            for (int i = stosy.Count-1; i > 0; --i)
            {
                for(int j = 1; j<=stosy[i].rozmiar; ++j)
                {
                    dane[--ostatnia] = dane[stosy[i].poczatek +
                                            stosy[i].rozmiar - j];
                }
            }
            for (int i = 1; i < stosy.Count; ++i)
            {
                for(int j = 0; j <stosy[i].rozmiar; ++j)
                {
                    dane[noweIndeksy[i, 0] + j] = dane[ostatnia++];
                }
            }

            //zmiana indeksów stosów
            for (int i = 0; i < stosy.Count; ++i)
            {
                stosy[i].Przenies(noweIndeksy[i, 0], noweIndeksy[i, 1]);
            }
            return true;
        }

        /// <summary>
        /// Zwraca komunikat o zawartości stosów.
        /// </summary>
        /// <returns></returns>
        public override string ToString() {
            StringBuilder wyn = new StringBuilder();
            for (int i = 0; i < stosy.Count; ++i) {
                wyn.Append($"Stos nr {i,3}:");
                for(int j = 0; j<stosy[i].rozmiar; ++j)
                {
                    wyn.Append(dane[stosy[i].poczatek + j]);
                }
                wyn.Append('_', stosy[i].pozostalo);
                wyn.Append('\n');
            }
            wyn.Append($"Nie przydzielono: {niePrzydzielono}");

            return wyn.ToString();
        }
    }
}
