# DBSA_Project
Project for the Database Systems Architecture course at ULB.


# Meetings

1. Mon 6 nov. 16:00

# Steps:
1. Implementing the two data types: chessgame, chessboard
2. Implementing the functions and predicates
   - ```getBoard(game, move):``` Get board state at a specific move.
   - ```getFirstMoves(game, n):``` Get the opening moves.
   - ```hasOpening(game, opening):``` Check if game starts with the same opening.
   - ```hasBoard(game, board, n):``` Check if board state is in initial moves.
3. Implementing the two indexes

```
                                                     _:_
                                                    '-.-'
                                           ()      __.'.__
                                        .-:--:-.  |_______|
                                 ()      \____/    \=====/
                                 /\      {====}     )___(
                      (\=,      //\\      )__(     /_____\
      __    |'-'-'|  //  .\    (    )    /____\     |   |
     /  \   |_____| (( \_  \    )__(      |  |      |   |
     \__/    |===|   ))  `\_)  /____\     |  |      |   |
    /____\   |   |  (/     \    |  |      |  |      |   |
     |  |    |   |   | _.-'|    |  |      |  |      |   |
     |__|    )___(    )___(    /____\    /____\    /_____\
    (====)  (=====)  (=====)  (======)  (======)  (=======)
    }===={  }====={  }====={  }======{  }======{  }======={
   (______)(_______)(_______)(________)(________)(_________)

          __    __    __    __         
       8 /__////__////__////__////    Simon Coessens 🇧🇪
      7 ////__////__////__////__/        Ben Gold 🇺🇸
     6 /__////__////__////__////             Nils Van Es Ostos 🇪🇸
    5 ////__////__////__////__/                   Arijit Samal 🇮🇳
   4 /__////__////__////__////   
  3 ////__////__////__////__/    
 2 /__////__////__////__////   
1 ////__////__////__////__/   
   a  b  c  d  e  f  g  h

```

Requirements: 

#1. Deciding the Datastructures for the new Data types

Chessboard: FEN
Chessgame: SAN


# Task division 

6 november: 
   - Arijit & Niels: Implement Data type in C
   - Simon: ```getBoard(chessgame, integer) -> chessboard:```, ```getFirstMoves(chessgame, integer) -> chessgame:```
   - Ben: ```hasOpening(chessgame, chessgame) -> bool:```, ```hasBoard(chessgame, chessboard, integer) -> bool:```

Algorithms needed: 
   - SAN to FEN: paper in the readings folder gives an alg. to update FEN notation
        - For ```getBoard(chessgame, integer)```we need an algorithm that converts SAN to FEN directly
        - For ```hasBoard(chessgame, chessboard, integer)```we need an algorithm that updates FEN after each move (paper gives an alg. for that)
        - another interesting link: https://github.com/SindreSvendby/pgnToFen

# Next meeting: 13 November 16:00 💯



TODO: 
   1. Dummy project for just the datatypes, without indexes and stuff 
   2. Interpret the smallchess lib file (write our own SAN -> FEN function)
Everyone works on these things

# NEXT MEETING: 21 November 


DATAtype working and testedn: 

```
postgres=# -- Create a test table for chess games
CREATE TABLE test_chessgames (
    id serial PRIMARY KEY,
    game chessgame
);

-- Insert sample data into the test table
-- Note: Replace 'sample_game_data' with actual data in the correct format for a chessgame
INSERT INTO test_chessgames (game) VALUES ('sample_game_data');

-- Retrieve all records to verify insertion
SELECT * FROM test_chessgames;

-- Test custom functions (if any)
-- Note: Replace 'function_name' and 'arguments' with your actual function names and appropriate arguments
DROP TABLE test_chessgames;r testing = 1;1;game_data' WHERE id = 1; chessgame
ERROR:  relation "test_chessgames" already exists
INSERT 0 1
 id |       game       
----+------------------
  1 | test
  2 | sample_game_data
(2 rows)

UPDATE 1
 id |       game        
----+-------------------
  1 | updated_game_data
(1 row)

DELETE 1
 id |       game       
----+------------------
  2 | sample_game_data
(1 row)

DROP TABLE
postgres=#
```
