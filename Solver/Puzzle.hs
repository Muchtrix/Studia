-- ----------------------------------------------------------------------------
-- Puzzle.hs
-- WERSJA 0.1
-- ZADANIE 0 (Obserwatorzy)
-- ----------------------------------------------------------------------------
module Puzzle where

data Field = B | E | N Int deriving Eq

data Puzzle = Puzzle Int Int [(Int, Int, Int)]
type Result = [[Field]]
type Solver = Int -> Int -> [(Int, Int, Int)] -> [Result]

data Test
    = SimpleTest Puzzle Result
    | CountTest  Puzzle Int

runSolver :: Solver -> Puzzle -> [Result]
runSolver solver (Puzzle m n l) = solver m n l

checkSolution :: Result -> Result -> Bool
checkSolution = ( == )

baseTests :: [Test]
baseTests =
    [ SimpleTest
        (Puzzle 7 7 [(1,2,3), (2,3,6), (3,6,3), (4,4,13), (5,2,2), (6,5,4), (7,6,6)])
        [[E, N 3, B,  E,    E,  E,  E],
        [B, E,  N 6, E,    E,  B,  E],
        [E, B,  E,  E,    B,  N 3, B],
        [E, E,  E,  N 13, E,  E,  E],
        [B, N 2, B,  E,    B,  E,  E],
        [E, B,  E,  E,    N 4, B,  E],
        [E, E,  E,  E,    E,  N 6, B]]
    , CountTest
        (Puzzle 7 7 [(1,2,3), (2,3,6), (3,6,3), (4,4,13), (5,2,2), (6,5,4), (7,6,6)])
        1
    ]
