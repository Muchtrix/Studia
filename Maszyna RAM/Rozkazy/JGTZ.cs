﻿using System;

namespace Maszyna_RAM.Rozkazy {
    class JGTZ : Rozkaz {
        string arg;

        public JGTZ(Maszyna wyk, string arg) : base(wyk) {
            this.arg = arg;
        }

        public override bool Wykonaj() {

            if (Wykonawca.KomorkiPamieci[0] <= 0) return true;

            int instrukcja = 0;
            if (Wykonawca.RejestrEtykiet.TryGetValue(arg,out instrukcja)) {
                Wykonawca.LicznikRozkazow = instrukcja -1;
                return true;
            }
            return false;
        }

        public override string ToString() {
            return "JGTZ " + arg;
        }
    }
}
