using System;
using System.Collections.Generic;

namespace Maszyna_RAM {
    internal class Maszyna {
        public List<Rozkaz> ListaRozkazow = new List<Rozkaz>();
        public int LicznikRozkazow = 0;
        public int[] KomorkiPamieci;
        public Dictionary<string, int> RejestrEtykiet
                                        = new Dictionary<string, int>();
        public List<int> TasmaWejsciowa = new List<int>();
        int aktDana = 0;
        public List<int> TasmaWyjsciowa = new List<int>();

        public Maszyna(int RozmiarPamieci, List<int> Tasma) {
            KomorkiPamieci = new int[RozmiarPamieci];
            TasmaWejsciowa = Tasma;
        }

        // Pobranie danej z tasmy wejsciowej
        public bool Pobierz(out int wynik) {
            try {
                wynik = TasmaWejsciowa[aktDana++];
            }
            catch (IndexOutOfRangeException) {
                wynik = 0;
                return false;
            }
            return true;
        }

        public bool Uruchom() {
            try {
                do {
                    if (!ListaRozkazow[LicznikRozkazow].Wykonaj()) {
                        Console.WriteLine("Błąd w instrukcji {0}: {1}",
                                          LicznikRozkazow,
                                          ListaRozkazow[LicznikRozkazow]);
                        return false;
                    }
                } while (++LicznikRozkazow >= 0);
            }
            catch (IndexOutOfRangeException) { return true; }
            return true;
        }
    }

    struct Argument {
        public int wartosc;
        public ArgTyp rodzaj;
        public Argument(int w, ArgTyp r) {
            wartosc = w;
            rodzaj = r;
        }

        public override string ToString() {
            switch (rodzaj) {
                case ArgTyp.adresBezp:
                    return wartosc.ToString();
                case ArgTyp.adresPosr:
                    return "^" + wartosc.ToString();
                case ArgTyp.stala:
                    return "=" + wartosc.ToString();
            }
            return null;
        }
    }

    enum ArgTyp {
        adresBezp,
        adresPosr,
        stala
    }
}
