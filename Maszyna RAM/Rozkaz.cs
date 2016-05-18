namespace Maszyna_RAM {
    internal abstract class Rozkaz {
        protected Maszyna Wykonawca;

        public Rozkaz(Maszyna wyk) {
            Wykonawca = wyk;
        }

        public abstract bool Wykonaj();
    }
}