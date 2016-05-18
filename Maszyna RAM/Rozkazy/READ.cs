using System;

namespace Maszyna_RAM.Rozkazy {
    class READ : Rozkaz {
        Argument arg;

        public READ(Maszyna wyk, Argument arg) : base(wyk) {
            this.arg = arg;
        }

        public override bool Wykonaj() {

            try {
                switch (arg.rodzaj) {
                    case ArgTyp.adresBezp:
                        if (!Wykonawca.Pobierz(out Wykonawca
                                      .KomorkiPamieci[arg.wartosc]))
                            return false;
                        break;
                    case ArgTyp.adresPosr:
                        if (!Wykonawca.Pobierz(out Wykonawca
                                      .KomorkiPamieci[Wykonawca
                                      .KomorkiPamieci[arg.wartosc]]))
                            return false;
                        break;
                    case ArgTyp.stala:
                        return false;
                }
            }
            catch (IndexOutOfRangeException) {
                return false;
            }
            return true;
        }

        public override string ToString() {
            return "READ " + arg.ToString();
        }
    }
}
