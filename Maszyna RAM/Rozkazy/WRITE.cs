using System;

namespace Maszyna_RAM.Rozkazy {
    class WRITE : Rozkaz {
        Argument arg;

        public WRITE(Maszyna wyk, Argument arg) : base(wyk) {
            this.arg = arg;
        }

        public override bool Wykonaj() {
            int wart = Wykonawca.KomorkiPamieci[0];

            try {
                switch (arg.rodzaj) {
                    case ArgTyp.adresBezp:
                        Wykonawca.TasmaWyjsciowa.Add(Wykonawca
                                                .KomorkiPamieci[arg.wartosc]);
                        break;
                    case ArgTyp.adresPosr:
                        Wykonawca.TasmaWyjsciowa.Add(Wykonawca
                                                .KomorkiPamieci[Wykonawca
                                                .KomorkiPamieci[arg.wartosc]]);
                        break;
                    case ArgTyp.stala:
                        Wykonawca.TasmaWyjsciowa.Add(arg.wartosc);
                        break;
                }
            }
            catch (IndexOutOfRangeException) { return false; }
            catch (InvalidOperationException) { return false; }
            return true;
        }

        public override string ToString() {
            return "WRITE " + arg.ToString();
        }
    }
}
