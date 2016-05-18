import Puzzle
import Checker

import Data.Set as Set

--------------------------------------------------------------------------------
-- Wiktor Adamski
-- nr indeksu 272220
-- Zadanie 0: Obserwatorzy
-- Zagadka z magazynu "Wiedza i życie", wydanie wrzesień 2001
--------------------------------------------------------------------------------
-- Idea Rozwiazania
-- 1. Dla kazdego obserwatora generujemy zestaw scian, uwazajac by wszyscy
--    obserwatorzy mieli dobra widocznosc, oraz aby sciany sie nie stykaly.
-- 2. Za pomoca przeszukiwania w glab sprawdzamy spojnosc planszy.
-- 3. Formatujemy wynik do postaci podanej w specyfikacji.
--------------------------------------------------------------------------------
-- Raport z wykonanania testow
-- ======================      <-- Testy wlasne
-- Test: 1/15
-- CPU time:   0.00s
-- Accept!
-- ======================
-- Test: 2/15
-- CPU time:   0.00s
-- Accept!
-- ======================
-- Test: 3/15
-- CPU time:   0.00s
-- Accept!
-- ======================
-- Test: 4/15
-- CPU time:   0.00s
-- Accept!
-- ======================
-- Test: 5/15
-- CPU time:   0.00s
-- Accept!
-- ======================
-- Test: 6/15
-- CPU time:   0.00s
-- Accept!
-- ======================
-- Test: 7/15
-- CPU time:   0.06s
-- Accept!
-- ======================
-- Test: 8/15
-- CPU time:   0.30s
-- Accept!
-- ======================
-- Test: 9/15
-- CPU time:   0.05s
-- Accept!
-- ======================
-- Test: 10/15
-- CPU time:   0.08s
-- Accept!
-- ======================
-- Test: 11/15
-- CPU time:   0.02s
-- Accept!
-- ======================
-- Test: 12/15
-- CPU time:   0.17s
-- Accept!
-- ======================
-- Test: 13/15
-- CPU time:   0.00s
-- Accept!
-- ======================
-- Test: 14/15
-- CPU time:   0.00s
-- Accept!
-- ======================
-- Test: 15/15
-- CPU time:   0.14s
-- Accept!
-- Accepted: 15/15
-- ======================      <-- Testy z KNO
-- Test: 1/2
-- CPU time:   0.00s
-- Accept!
-- ======================
-- Test: 2/2
-- CPU time:   0.00s
-- Accept!
-- Accepted: 2/2
--------------------------------------------------------------------------------
type Sciana = (Int,Int)
type Pole   = (Int,Int)
type Obserwator = (Int,Int,Int)
data Kierunek = Prawo | Lewo | Gora | Dol deriving Eq

-- Glowna funkcja rozwiazujaca. Pierwszy wzorzec chroni program przed podaniem
-- pustej listy scian
solve :: Solver
solve x y [] = [format [] empty x y]
solve x y obs = [format obs wyn x y | wyn <- kandydat x y obs,
                                      czyOdcieto (head obs) wyn x y]

-- Generuje listę kandydatow na ustawienie scian
kandydat :: Int -> Int -> [Obserwator] -> [Set Sciana]
kandydat _ _ [] = [empty]
kandydat x y obs@(o:os) = [sss | s <- kandydat x y os,
                                 ss <- dostawSciany x y o,
                                 sasiedzi ss s,
                                 let sss = ss `union` s,
                                 widWszystkie sss]
                     where widWszystkie scian = and [widOb o scian x y|o <- obs]

-- Generuje liste czworek z ktorych sklada sie widocznosc x
-- (Powiekszona o 3 ze wzgledu na pole obserwatora)
czworki :: Int -> [(Int, Int, Int, Int)]
czworki x = [(a,b,c,d) | a <- [1..x],
                         b <- [1..x], a+b < x+3,
                         c <- [1..x], a+b+c < x+3,
                         let d = (x+3) - (a+b+c)]

-- Generuje liste zbiorow scian w oparciu o liste czworek
dostawSciany :: Int -> Int -> Obserwator -> [Set Sciana]
dostawSciany rox roy (x,y,w) =[aux a b c d x y|(a,b,c,d) <- czworki w,
                                               x+a < rox+2,    y+b < roy+2,
                                               x-c > -1,       y-d > -1]
          where aux a b c d x y = obcinacz rox roy $ fromList [(x+a,y), (x,y+b),
                                                             (x-c,y), (x,y-d)]

-- Usuwa z zestawu sciany bedace poza plansza
obcinacz :: Int -> Int -> Set Sciana -> Set Sciana
obcinacz x y = Set.filter (\(a,b)-> a > 0 && a <= x && b > 0 && b <= y)

-- Sprawdza czy nowe sciany nie sasiaduja z juz istniejacymi
sasiedzi :: Set Sciana -> Set Sciana -> Bool
sasiedzi kand sciany = Set.fold aux True $ Set.map sasiedziSet kand
        where sasiedziSet (x,y) = fromList [(x+1,y), (x-1,y), (x,y+1), (x,y-1)]
              aux x = (&&) (x `intersection` sciany == empty)

-- Sprawdza czy obserwator o ma odpowiednia widocznosc
widOb :: Obserwator -> Set Sciana -> Int -> Int -> Bool
widOb o@(_,_,w) sciany rox roy = w == (1 + sum [liczKier o rox roy sciany kier| kier <- [Prawo,Lewo,Gora,Dol]])

-- Liczy widocznosc obserwatora w danym kierunku
liczKier :: Obserwator -> Int -> Int -> Set Sciana -> Kierunek -> Int
liczKier (x, y, _) rox roy sciany kierunek
            | kierunek == Gora  = aux [(dx, y)| dx <- [(x+1)..rox]]
            | kierunek == Dol   = aux [(dx, y)| dx <- [(x-1), (x-2)..1]]
            | kierunek == Prawo = aux [(x, dy)| dy <- [(y+1)..roy]]
            | kierunek == Lewo  = aux [(x, dy)| dy <- [(y-1), (y-2)..1]]
                where aux = length.takeWhile (`notMember` sciany)

-- Sprawdza czy plansza nie zostala podzielona na czesci przez sciany
czyOdcieto :: Obserwator -> Set Sciana -> Int -> Int -> Bool
czyOdcieto (x, y, _) sciany rox roy = pustych == rox*roy - size sciany
                          where pustych = dfs [(x,y)] sciany 0 empty rox roy

-- Liczy liczbe stykajacych sie z soba wolnych pol
dfs :: [Pole] -> Set Sciana -> Int -> Set Pole -> Int -> Int -> Int
dfs [] _ x _ _ _ = x
dfs ((x,y):xs) sciany n odw rox roy
    | x < 1 || x > rox      = dfs xs sciany n odw rox roy
    | y < 1 || y > roy      = dfs xs sciany n odw rox roy
    | (x,y) `member` sciany = dfs xs sciany n odw rox roy
    | (x,y) `member` odw    = dfs xs sciany n odw rox roy
    | otherwise             = dfs xs' sciany (n+1) ((x,y)`insert`odw) rox roy
        where xs' = (x+1,y):(x-1,y):(x,y+1):(x,y-1):xs

-- Generuje plansze na podstawie listy obserwatorow i zestawu scian
format :: [Obserwator] -> Set Sciana -> Int -> Int -> [[Field]]
format obs sciany rox roy = [wiersz obs sciany dx| dx <- [1..rox]]
      where wiersz obs sciany x = [pole obs sciany x dy| dy <- [1..roy]]
            pole obs sciany x y = case znajdzObs (x,y) obs of
                              Just w  -> N w
                              Nothing -> if (x,y) `member` sciany then B else E

-- Dla pozycji (x,y) zwraca Just w, gdzie w to widocznosc stajacego tam
-- obserwatora, lub Nothing, jesli zaden z obserwatorow tam nie stoi
znajdzObs :: (Int, Int) -> [Obserwator] -> Maybe Int
znajdzObs _ [] = Nothing
znajdzObs (x,y) ((ox,oy,w):obs) = if x == ox && y == oy
                                  then Just w
                                  else znajdzObs (x,y) obs

tests :: [Test]
tests = [SimpleTest (Puzzle 1 1 []) [[E]],
         CountTest (Puzzle 2 2 [(1,1,4)]) 0,
         CountTest (Puzzle 3 3 [(2,2,4)]) 4,
         SimpleTest (Puzzle 4 4 [(1,1,3), (3,1,2), (2,4,5), (4,3,3)])
            [[N 3,E  ,E  ,B  ],
             [B  ,E  ,E  ,N 5],
             [N 2,E  ,B  ,E  ],
             [B  ,E  ,N 3,E  ]],
         SimpleTest (Puzzle 5 5 [(1,3,8), (2,1,4), (2,4,3), (3,3,7), (3,5,2),
                                 (5,2,6)])
            [[B  ,E  ,N 8,E  ,E  ],
             [N 4,B  ,E  ,N 3,B  ],
             [E  ,E  ,N 7,B  ,N 2],
             [E  ,E  ,E  ,E  ,E  ],
             [E  ,N 6,E  ,E  ,B  ]],
         SimpleTest (Puzzle 6 6 [(1,1,8), (1,4,7), (2,4,6), (3,6,2), (4,3,7),
                                 (5,4,3), (6,2,10)])
            [[N  8,E   ,E   ,N  7,E   ,E   ],
             [E   ,E   ,E   ,N  6,E   ,B   ],
             [E   ,E   ,E   ,B   ,E   ,N  2],
             [B   ,E   ,N  7,E   ,E   ,B   ],
             [E   ,E   ,B   ,N  3,B   ,E   ],
             [B   ,N 10,E   ,E   ,E   ,E   ]],
         SimpleTest (Puzzle 7 7 [(1,4,7), (2,2,5), (3,4,2), (3,7,6), (4,5,9),
                                 (5,3,7), (6,5,7), (7,1,4)])
            [[E   ,E   ,E   ,N  7,E   ,E   ,E   ],
             [B   ,N  5,E   ,B   ,E   ,E   ,E   ],
             [E   ,E   ,B   ,N  2,E   ,B   ,N  6],
             [B   ,E   ,E   ,B   ,N  9,E   ,E   ],
             [E   ,B   ,N  7,E   ,E   ,E   ,E   ],
             [E   ,E   ,E   ,B   ,N  7,B   ,E   ],
             [N  4,E   ,B   ,E   ,E   ,E   ,B   ]],
         SimpleTest (Puzzle 8 8 [(1,7,6), (2,3,5), (3,1,10), (4,5,8), (5,1,3),
                                 (6,4,7), (6,8,8), (7,6,9),  (8,2,3)])
            [[E   ,E   ,E   ,E   ,E   ,B   ,N  6,E   ],
             [B   ,E   ,N  5,E   ,B   ,E   ,E   ,E   ],
             [N 10,E   ,E   ,E   ,E   ,E   ,E   ,E   ],
             [E   ,E   ,B   ,E   ,N  8,E   ,E   ,E   ],
             [N  3,B   ,E   ,B   ,E   ,E   ,E   ,E   ],
             [B   ,E   ,E   ,N  7,E   ,E   ,B   ,N  8],
             [E   ,E   ,E   ,E   ,B   ,N  9,E   ,E   ],
             [B   ,N  3,B   ,E   ,E   ,E   ,E   ,E   ]],
         SimpleTest (Puzzle 9 9 [(1,9,3), (2,4,2), (3,2,5), (3,7,5), (4,1,5),
                                 (4,4,2), (6,6,3), (7,9,5), (8,2,3), (8,3,4),
                                 (9,7,3), (9,9,5)])
            [[E   ,E   ,E   ,E   ,E   ,E   ,E   ,B   ,N  3],
             [B   ,E   ,B   ,N  2,B   ,E   ,B   ,E   ,E   ],
             [E   ,N  5,E   ,B   ,E   ,E   ,N  5,E   ,E   ],
             [N  5,B   ,E   ,N  2,B   ,E   ,B   ,E   ,B   ],
             [E   ,E   ,E   ,B   ,E   ,B   ,E   ,E   ,E   ],
             [E   ,E   ,B   ,E   ,E   ,N  3,B   ,E   ,E   ],
             [E   ,B   ,E   ,E   ,E   ,B   ,E   ,B   ,N  5],
             [B   ,N  3,N  4,B   ,E   ,E   ,E   ,E   ,E   ],
             [E   ,E   ,E   ,E   ,E   ,B   ,N  3,B   ,N  5]],
         SimpleTest (Puzzle 10 10 [(1,8,7),  (2,3,6), (3,6,9), (3,10,9),
                                   (4,1,10), (6,3,8), (6,8,6), (8,1,4),
                                   (8,6,6),  (9,4,7), (9,9,3), (10,7,5)])
            [[E   ,E   ,B   ,E   ,E   ,B   ,E   ,N  7,B   ,E   ],
             [E   ,E   ,N  6,E   ,E   ,E   ,B   ,E   ,E   ,E   ],
             [E   ,E   ,B   ,E   ,E   ,N  9,E   ,E   ,E   ,N  9],
             [N 10,E   ,E   ,E   ,E   ,E   ,B   ,E   ,E   ,B   ],
             [E   ,E   ,E   ,E   ,E   ,B   ,E   ,E   ,E   ,E   ],
             [B   ,E   ,N  8,B   ,E   ,E   ,B   ,N  6,B   ,E   ],
             [E   ,E   ,E   ,E   ,E   ,B   ,E   ,B   ,E   ,E   ],
             [N  4,B   ,E   ,B   ,E   ,N  6,E   ,E   ,B   ,E   ],
             [E   ,E   ,E   ,N  7,E   ,E   ,B   ,E   ,N  3,B   ],
             [E   ,B   ,E   ,E   ,E   ,E   ,N  5,B   ,E   ,E   ]],
         SimpleTest (Puzzle 11 11 [(1,7,5), (2,3,8), (2,11,3), (4,5,10),
                                   (5,7,4), (6,1,7), (6,10,7), (7,4,10),
                                   (7,9,7), (9,6,3), (10,3,5), (11,11,3)])
            [[E   ,E   ,E   ,B   ,E   ,E   ,N  5,E   ,B   ,E   ,E   ],
             [E   ,B   ,N  8,E   ,E   ,E   ,E   ,E   ,E   ,B   ,N  3],
             [B   ,E   ,B   ,E   ,B   ,E   ,B   ,E   ,E   ,E   ,E   ],
             [E   ,E   ,E   ,E   ,N 10,E   ,E   ,E   ,E   ,E   ,B   ],
             [E   ,E   ,E   ,E   ,B   ,E   ,N  4,B   ,E   ,E   ,E   ],
             [N  7,E   ,E   ,E   ,E   ,B   ,E   ,E   ,E   ,N  7,B   ],
             [B   ,E   ,B   ,N 10,E   ,E   ,B   ,E   ,N  7,B   ,E   ],
             [E   ,E   ,E   ,E   ,E   ,E   ,E   ,E   ,B   ,E   ,E   ],
             [E   ,E   ,E   ,E   ,B   ,N  3,B   ,E   ,E   ,E   ,E   ],
             [E   ,E   ,N  5,B   ,E   ,B   ,E   ,E   ,E   ,E   ,B   ],
             [E   ,E   ,B   ,E   ,E   ,E   ,E   ,B   ,E   ,E   ,N  3]],
         SimpleTest (Puzzle 12 12 [(2,2,5),  (2,11,4), (4,4,6),  (5,8,10),
                                   (5,12,6), (6,2,11), (6,9,10), (7,8,11),
                                   (8,5,9),  (8,12,4), (11,8,6), (12,11,7)])
            [[E   ,B   ,E   ,E   ,E   ,E   ,E   ,E   ,E   ,E   ,B   ,E   ],
             [E   ,N  5,E   ,B   ,E   ,E   ,E   ,B   ,E   ,E   ,N  4,E   ],
             [E   ,E   ,E   ,E   ,B   ,E   ,E   ,E   ,E   ,E   ,B   ,E   ],
             [E   ,E   ,E   ,N  6,E   ,B   ,E   ,E   ,B   ,E   ,E   ,E   ],
             [E   ,B   ,E   ,B   ,E   ,E   ,E   ,N 10,E   ,E   ,B   ,N  6],
             [E   ,N 11,E   ,E   ,E   ,E   ,E   ,E   ,N 10,B   ,E   ,E   ],
             [B   ,E   ,E   ,E   ,E   ,E   ,E   ,N 11,B   ,E   ,E   ,B   ],
             [E   ,E   ,E   ,E   ,N  9,B   ,E   ,B   ,E   ,E   ,E   ,N  4],
             [E   ,B   ,E   ,E   ,B   ,E   ,E   ,E   ,E   ,E   ,E   ,B   ],
             [E   ,E   ,E   ,E   ,E   ,E   ,E   ,B   ,E   ,E   ,E   ,E   ],
             [E   ,E   ,B   ,E   ,E   ,E   ,E   ,N  6,B   ,E   ,B   ,E   ],
             [E   ,E   ,E   ,B   ,E   ,E   ,E   ,E   ,E   ,E   ,N  7,B   ]],
         SimpleTest (Puzzle 13 13 [(2,5,3),  (3,9,6),  (4,2,5),  (5,12,6),
                                   (7,6,9),  (7,13,6), (8,1,13), (9,9,6),
                                   (10,7,8), (11,3,4), (11,9,9), (13,5,5),
                                   (13,11,2)])
            [[B,E,E,E,E,E,E,E,E,E,E,E,E],
             [E,E,B,E,N 3,B,E,E,E,E,E,E,E],
             [E,E,E,E,B,E,E,E,N 6,B,E,B,E],
             [E,N 5,B,E,E,E,E,E,B,E,E,E,B],
             [E,B,E,E,E,E,E,B,E,E,E,N 6,E],
             [E,E,E,E,E,B,E,E,E,E,E,B,E],
             [E,B,E,E,E,N 9,B,E,B,E,E,E,N 6],
             [N 13,E,E,E,E,E,E,B,E,E,E,E,B],
             [B,E,B,E,E,E,B,E,N 6,B,E,E,E],
             [E,E,E,E,E,E,N 8,B,E,E,E,E,E],
             [E,E,N 4,B,E,E,E,E,N 9,B,E,E,E],
             [E,E,B,E,B,E,B,E,E,E,B,E,E],
             [E,E,E,E,N 5,B,E,E,B,E,N 2,B,E]],
         SimpleTest (Puzzle 14 14 [(1,14,4),  (2,12,5),  (3,3,8),  (3,5,9),
                                   (6,1,6),   (6,8,7),   (9,6,10), (9,14,4),
                                   (11,9,10), (12,3,10), (13,6,9), (13,9,8),
                                   (13,14,3)])
            [[E,E,E,E,B,E,E,E,E,E,B,E,E,N 4],
             [E,E,E,E,E,E,E,B,E,E,E,N 5,B,E],
             [E,E,N 8,E,N 9,B,E,E,B,E,E,B,E,B],
             [E,E,E,E,E,E,E,E,E,E,E,E,E,E],
             [E,E,B,E,E,B,E,B,E,E,E,E,E,E],
             [N 6,B,E,E,E,E,E,N 7,E,B,E,E,E,E],
             [B,E,E,E,B,E,E,B,E,E,E,E,E,E],
             [E,E,E,E,E,E,E,E,E,E,E,E,E,B],
             [E,E,E,E,E,N 10,B,E,E,B,E,E,E,N 4],
             [E,E,E,E,E,E,E,E,E,E,E,E,E,B],
             [E,E,E,E,E,B,E,E,N 10,B,E,E,E,E],
             [E,E,N 10,B,E,E,E,E,B,E,E,E,E,B],
             [E,B,E,E,E,N 9,E,E,N 8,E,B,E,E,N 3],
             [E,E,B,E,E,B,E,E,B,E,E,E,E,B]],
         SimpleTest (Puzzle 15 15 [(2,2,7),  (2,11,11), (3,6,6),   (6,3,7),
                                   (7,5,10), (9,13,6),  (10,14,3), (11,8,5),
                                   (12,3,8), (12,15,4), (13,1,3),  (15,4,7),
                                   (15,11,4)])
            [[E,E,B,E,B,E,E,E,E,E,E,E,E,E,E],
             [B,N 7,E,E,E,B,E,E,E,E,N 11,E,E,E,E],
             [E,E,E,E,E,N 6,B,E,E,E,E,E,E,E,E],
             [E,E,E,E,E,B,E,E,E,E,B,E,E,E,E],
             [E,B,E,E,E,E,E,E,E,E,E,E,E,E,E],
             [B,E,N 7,B,E,E,E,E,E,E,E,E,E,E,E],
             [E,E,E,E,N 10,B,E,E,E,E,E,E,E,E,E],
             [E,E,B,E,B,E,E,E,E,E,E,E,B,E,E],
             [E,E,E,E,E,E,E,B,E,B,E,E,N 6,B,E],
             [B,E,E,E,E,E,E,E,E,E,B,E,E,N 3,B],
             [E,E,E,B,E,E,E,N 5,B,E,E,E,E,B,E],
             [E,E,N 8,E,B,E,E,B,E,E,E,B,E,E,N 4],
             [N 3,B,E,E,E,E,E,E,E,E,E,E,B,E,B],
             [B,E,B,E,E,E,E,E,E,E,B,E,E,E,E],
             [E,E,E,N 7,B,E,B,E,E,E,N 4,B,E,E,E]]
        ]

main :: IO ()
main = checkerMain solve tests
