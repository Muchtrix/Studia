//---------------------------------------------------------------------------//
// Wiktor Adamski                                                             //
// Nr indeksu 27220                                                           //
// Systemy operacyjne - Pamięć stosowa                                        //
//---------------------------------------------------------------------------//

using System;

namespace PamiecStosowa
{
    class Program {
        static void Main(string[] args) {
            Pamiec p = new Pamiec(80, 8);
            Console.WriteLine(p);
            Console.WriteLine();

            bool petla = true;
            while (petla) {
                var a = Console.ReadKey(true).Key;
                switch (a)
                {
                    case ConsoleKey.X:
                        petla = false;
                        break;
                    case ConsoleKey.P:
                        for (int i = 0; i < p.dane.Length; ++i)
                        {
                            Console.Write(p.dane[i].ToString() + " ");
                            if (i % 10 == 9) Console.WriteLine();
                        }
                        Console.WriteLine();
                        break;
                    default:
                        if (!p.DodajDana())
                        {
                            Console.WriteLine("Pamięć zapełniona!");
                            Console.ReadKey();
                            petla = false;
                        }
                        Console.WriteLine(p);
                        Console.WriteLine();
                        break;
                }
            }
        }
    }
}
