//---------------------------------------------------------------------------//
// Wiktor Adamski                                                             //
// Nr indeksu 27220                                                           //
// Systemy operacyjne - Pamięć stosowa                                        //
//---------------------------------------------------------------------------//

namespace PamiecStosowa
{
    class Stos {
        public int poczatek   { get; private set; }
        public int rozmiarMax { get; private set; }
        public int rozmiar    { get; private set; }
        public int pozostalo => rozmiarMax - rozmiar;
        public double przyrost { get; set; } = 0.0;

        public Stos(int pocz, int miejsce) {
            poczatek = pocz;
            rozmiarMax = miejsce;
            rozmiar = 0;
        }


        /// <summary>
        /// Sprawdza czy dana zmieści się na stosie.
        /// </summary>
        /// <param name="rozmiar">
        /// Rozmiar danej do wprowadzenia na stos.
        /// </param>
        public int Dodaj(int rozmiar) {
            if (rozmiar <= pozostalo) {
                int a = this.rozmiar;
                this.rozmiar += rozmiar;
                przyrost += rozmiar;
                return poczatek + a;
            }
            else return -1;
        }

        /// <summary>
        /// Aktualizuje wskaźniki stosu.
        /// </summary>
        /// <param name="poczatek">Nowy indeks początkowy stosu.</param>
        /// <param name="rozmiar">Nowy rozmiar maksymalny stosu.</param>
        public void Przenies(int poczatek, int rozmiar) {
            this.poczatek = poczatek;
            this.rozmiarMax = rozmiar;
            przyrost *= 0.8;
        }
    }
}
