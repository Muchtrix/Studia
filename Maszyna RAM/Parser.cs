using Maszyna_RAM.Rozkazy;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace Maszyna_RAM {
    internal static class Parser {
        static public Maszyna CzytajProgram(string nazwaPliku) {
            StreamReader plik = null;
            try {
                plik = new StreamReader(nazwaPliku);
            }
            catch (Exception) {

                throw new Exception("Błąd odczytu pliku.");
            }


            int rozmiar;
            List<int> tasma;
            try {
                // Odczytanie danych poczatkowych
                rozmiar = CzytajRozmiar(plik);
                tasma = CzytajTasme(plik);
            }
            catch (IOException) {
                throw;
            }


            Maszyna masz = new Maszyna(rozmiar, tasma);
            int wiersz = 0;
            string linijka;

            try {
                while ((linijka = plik.ReadLine()) != null) {
                    linijka = linijka.Trim();
                    if (linijka == "") continue;
                    string[] komentarze = linijka.Split(new char[] { ' ' , '\t'},
                                        StringSplitOptions.RemoveEmptyEntries);

                    string[] slowa = komentarze.TakeWhile(s => s[0] != '/')
                                                                    .ToArray();

                    if (slowa.Length == 0) continue;
                    // Analiza pojedynczego rozkazu
                    if (slowa[0][0] == ':') {
                        if (slowa.Length >= 3) {
                            masz.RejestrEtykiet.Add(slowa[0].Substring(1),
                                                    wiersz);
                            CzytajRozkaz(masz, slowa[1].ToUpper(), slowa[2]);
                        }
                        else if (slowa[1].ToUpper() == "STOP") {
                            masz.RejestrEtykiet.Add(slowa[0].Substring(1),
                                                    wiersz);
                            masz.ListaRozkazow.Add(new STOP(masz));
                        }
                        else throw new IOException("Nie podano argumentu.");
                    }
                    else {
                        if (slowa.Length >= 2) {
                            CzytajRozkaz(masz, slowa[0].ToUpper(), slowa[1]);
                        }
                        else if (slowa[0].ToUpper() == "STOP") {
                            masz.ListaRozkazow.Add(new STOP(masz));
                        } else throw new IOException("Nie podano argumentu.");
                    }
                    ++wiersz;
                }
            }
            catch (IOException e) {
                throw new IOException($"Błąd w linijce {wiersz}: {e.Message}");
            }

            plik.Close();
            return masz;
        }

        static int CzytajRozmiar(StreamReader plik) {
            string linijka = plik.ReadLine();
            if (linijka == null)
                throw new IOException("Błąd odczytu rozmiaru pamięci.");
            int rozmiar = 0;
            if (!int.TryParse(linijka, out rozmiar))
                throw new IOException("Błąd odczytu rozmiaru pamięci.");
            return rozmiar;
        }

        static List<int> CzytajTasme(StreamReader plik) {
            string linijka = plik.ReadLine();
            if (linijka == null)
                throw new IOException("Błąd odczytu taśmy wejściowej.");
            string[] sep = linijka.Split(new char[] {' '}, 
                                        StringSplitOptions.RemoveEmptyEntries);
            List<int> wyn = new List<int>();
            int licz = 0;
            foreach(string s in sep) {
                if(!int.TryParse(s, out licz))
                    throw new IOException("Błąd odczytu taśmy wejściowej.");
                wyn.Add(licz);
            }
            return wyn;
        }

        static Argument CzytajArg(string napis) {
            ArgTyp rodz = ArgTyp.adresBezp;
            int wart = 0;
            switch (napis[0]) {
                case '=':
                    rodz = ArgTyp.stala;
                    if (!int.TryParse(napis.Substring(1), out wart))
                        throw new IOException("Błąd odczytu argumentu");
                    break;
                case '^':
                    rodz = ArgTyp.adresPosr;
                    if (!int.TryParse(napis.Substring(1), out wart))
                        throw new IOException("Błąd odczzytu argumentu");
                    break;
                default:
                    if (!int.TryParse(napis, out wart))
                        throw new IOException("Błąd odczytu argumentu");
                    break;
            }

            return new Argument(wart, rodz);
        }

        static void CzytajRozkaz(Maszyna m, string rozk, string argS) {
            Argument arg;
            switch (rozk) {
                case "JGTZ":
                    m.ListaRozkazow.Add(new JGTZ(m, argS));
                    return;
                case "JLTZ":
                    m.ListaRozkazow.Add(new JLTZ(m, argS));
                    return;
                case "JUMP":
                    m.ListaRozkazow.Add(new JUMP(m, argS));
                    return;
                case "JZERO":
                    m.ListaRozkazow.Add(new JZERO(m, argS));
                    return;
                case "ADD":
                    arg = CzytajArg(argS);
                    m.ListaRozkazow.Add(new ADD(m, arg));
                    return;
                case "DIV":
                    arg = CzytajArg(argS);
                    m.ListaRozkazow.Add(new DIV(m, arg));
                    return;
                case "LOAD":
                    arg = CzytajArg(argS);
                    m.ListaRozkazow.Add(new LOAD(m, arg));
                    return;
                case "MOD":
                    arg = CzytajArg(argS);
                    m.ListaRozkazow.Add(new MOD(m, arg));
                    return;
                case "MULT":
                    arg = CzytajArg(argS);
                    m.ListaRozkazow.Add(new MULT(m, arg));
                    return;
                case "READ":
                    arg = CzytajArg(argS);
                    m.ListaRozkazow.Add(new READ(m, arg));
                    return;
                case "STORE":
                    arg = CzytajArg(argS);
                    m.ListaRozkazow.Add(new STORE(m, arg));
                    return;
                case "SUB":
                    arg = CzytajArg(argS);
                    m.ListaRozkazow.Add(new SUB(m, arg));
                    return;
                case "WRITE":
                    arg = CzytajArg(argS);
                    m.ListaRozkazow.Add(new WRITE(m, arg));
                    return;
                default:
                    throw new IOException("Niedozwolona instrukcja");
            }
        }
    }
}
