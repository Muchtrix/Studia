using System;

namespace Maszyna_RAM.Rozkazy {
    class LOAD : Rozkaz {
        Argument arg;

        public LOAD(Maszyna wyk, Argument arg) : base(wyk) {
            this.arg = arg;
        }

        public override bool Wykonaj() {
            int wart = 0;

            try {
                switch (arg.rodzaj) {
                    case ArgTyp.adresBezp:
                        wart = Wykonawca.KomorkiPamieci[arg.wartosc];
                        break;
                    case ArgTyp.adresPosr:
                        wart = Wykonawca.KomorkiPamieci[Wykonawca
                                        .KomorkiPamieci[arg.wartosc]];
                        break;
                    case ArgTyp.stala:
                        wart = arg.wartosc;
                        break;
                }
            }
            catch (IndexOutOfRangeException) { return false; }

            Wykonawca.KomorkiPamieci[0] = wart;
            return true;
        }

        public override string ToString() {
            return "LOAD " + arg.ToString();
        }
    }
}
