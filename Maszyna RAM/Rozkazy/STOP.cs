using System;

namespace Maszyna_RAM.Rozkazy {
    class STOP : Rozkaz {
        public STOP(Maszyna wyk) : base(wyk) { }

        public override bool Wykonaj() {

            Wykonawca.LicznikRozkazow = -2;
            return true;
        }

        public override string ToString() {
            return "STOP";
        }
    }
}
