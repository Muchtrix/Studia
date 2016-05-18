using System;

namespace Maszyna_RAM.Rozkazy {
    class STORE : Rozkaz {
        Argument arg;

        public STORE(Maszyna wyk, Argument arg) : base(wyk) {
            this.arg = arg;
        }

        public override bool Wykonaj() {
            int wart = Wykonawca.KomorkiPamieci[0];

            try {
                switch (arg.rodzaj) {
                    case ArgTyp.adresBezp:
                        Wykonawca.KomorkiPamieci[arg.wartosc] = wart;
                        break;
                    case ArgTyp.adresPosr:
                        Wykonawca.KomorkiPamieci[Wykonawca
                                 .KomorkiPamieci[arg.wartosc]] = wart;
                        break;
                    case ArgTyp.stala:
                        return false;
                }
            }
            catch (IndexOutOfRangeException) { return false; }
            return true;
        }

        public override string ToString() {
            return "STORE " + arg.ToString();
        }
    }
}
