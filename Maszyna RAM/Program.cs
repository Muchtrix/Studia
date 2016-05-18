//---------------------------------------------------------------------------//
// Wiktor Adamski                                                            //
// Nr indeksu 272220                                                         //
// Systemy operacyjne - zadanie dodatkowe                                    //
// Maszyna RAM                                                               //
//---------------------------------------------------------------------------//


using System;
using static System.Console;

namespace Maszyna_RAM {
    class Program {
        static void Main(string[] args) {
            if (args.Length >= 1) {

                foreach(string s in args) {
                    WriteLine($"Program {s}");
                    Maszyna m = null;
                    try {
                        m = Parser.CzytajProgram(s);
                    }
                    catch (Exception e) {
                        WriteLine(e.Message);
                        continue;
                    }

                    if (m.Uruchom()) {
                        Write("Taśma wejściowa: ");
                        foreach (int i in m.TasmaWejsciowa)
                            Write($"{i}, ");
                        WriteLine();

                        Write("Taśma wyjściowa: ");
                        foreach (int i in m.TasmaWyjsciowa)
                            Write($"{i}, ");
                        WriteLine();
                    }
                    
                    WriteLine();
                }
            }
            else {
                WriteLine("Symulator maszyny RAM");
                WriteLine($"  {AppDomain.CurrentDomain.FriendlyName} [pliki]");
                WriteLine();
                WriteLine("  pliki\tLista plików z programami maszyny RAM");


                Maszyna m = null;
                try {
                    m = Parser.CzytajProgram("Bledny.txt");
                }
                catch (Exception e) {
                    WriteLine(e.Message);
                }

                m.Uruchom();

                Write("Taśma wejściowa: ");
                foreach (int i in m.TasmaWejsciowa)
                    Write($"{i}, ");
                WriteLine();

                Write("Taśma wyjściowa: ");
                foreach (int i in m.TasmaWyjsciowa)
                    Write($"{i}, ");
                WriteLine();
                WriteLine();
            }
        }
    }
}
