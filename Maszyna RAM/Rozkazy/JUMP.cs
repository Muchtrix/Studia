using System;

namespace Maszyna_RAM.Rozkazy {
    class JUMP : Rozkaz {
        string arg;

        public JUMP(Maszyna wyk, string arg) : base(wyk) {
            this.arg = arg;
        }

        public override bool Wykonaj() {
            int instrukcja = 0;
            if (Wykonawca.RejestrEtykiet.TryGetValue(arg,out instrukcja)) {
                Wykonawca.LicznikRozkazow = instrukcja -1;
                return true;
            }
            return false;
        }

        public override string ToString() {
            return "JUMP " + arg;
        }
    }
}
