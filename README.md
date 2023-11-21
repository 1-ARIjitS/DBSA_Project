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

working functionality 
```[sudo] password for simonc: 
postgres@simonlinux:~$ psql
psql (14.9 (Ubuntu 14.9-0ubuntu0.22.04.1))
Type "help" for help.

postgres=# select getBoard(chessgame('1. e4 d5 2. exd5 Qxd5 3. Nc3 Qd8 4. Bc4 Nf6 5. Nf3 Bg4 6. h3 Bxf3 7. Qxf3 e6 8. Qxb7 Nbd7 9. Nb5 Rc8 10. Nxa7 Nb6 11. Nxc8 Nxc8 12. d4 Nd6 13. Bb5+ Nxb5 14. Qxb5+ Nd7 15. d5 exd5 16. Be3 Bd6 17. Rd1 Qf6 18. Rxd5 Qg6 19. Bf4 Bxf4 20. Qxd7+ Kf8 21. Qd8#'),19);
 getboard 
----------
 ˉ
(1 row)

postgres=# select getBoard(chessgame('1. e4 d5 2. exd5 Qxd5 3. Nc3 Qd8 4. Bc4 Nf6 5. Nf3 Bg4 6. h3 Bxf3 7. Qxf3 e6 8. Qxb7 Nbd7 9. Nb5 Rc8 10. Nxa7 Nb6 11. Nxc8 Nxc8 12. d4 Nd6 13. Bb5+ Nxb5 14. Qxb5+ Nd7 15. d5 exd5 16. Be3 Bd6 17. Rd1 Qf6 18. Rxd5 Qg6 19. Bf4 Bxf4 20. Qxd7+ Kf8 21. Qd8#'),18);
                            getboard                            
----------------------------------------------------------------
 2rqkb1r/pQpn1ppp/4pn2/1N6/2B5/7P/PPPP1PP1/R1B1K2R w KQk - 3 10
(1 row)

postgres=# ls
postgres-# ^Z
[1]+  Stopped                 psql
postgres@simonlinux:~$ psql
psql (14.9 (Ubuntu 14.9-0ubuntu0.22.04.1))
Type "help" for help.

postgres=# create extension chess;
ERROR:  extension "chess" already exists
postgres=# drop extension chess;
DROP EXTENSION
postgres=# create extension chess;
CREATE EXTENSION
postgres=# select getFirstMoves(chessgame('1. Nf3 Nf6 2. c4 g6 3. Nc3 Bg7 4. d4 O-O 5. Bf4 d5 6. Qb3 dxc4 7. Qxc4 c6 8. e4 Nbd7 9. Rd1 Nb6 10. Qc5 Bg4 11. Bg5 Na4 12. Qa3 Nxc3 13. bxc3 Nxe4 14. Bxe7 Qb6 15. Bc4 Nxc3 16. Bc5 Rfe8+ 17. Kf1 Be6 18. Bxb6 Bxc4+ 19. Kg1 Ne2+ 20. Kf1 Nxd4+ 21. Kg1 Ne2+ 22. Kf1 Nc3+ 23. Kg1 axb6 24. Qb4 Ra4 25. Qxb6 Nxd1 26. h3 Rxa2 27. Kh2 Nxf2 28. Re1 Rxe1 29. Qd8+ Bf8 30. Nxe1 Bd5 31. Nf3 Ne4 32. Qb8 b5 33. h4 h5 34. Ne5 Kg7 35. Kg1 Bc5+ 36. Kf1 Ng3+ 37. Ke1 Bb4+ 38. Kd1 Bb3+ 39. Kc1 Ne2+ 40. Kb1 Nc3+ 41. Kc1 Rc2#'),10);
server closed the connection unexpectedly
	This probably means the server terminated abnormally
	before or while processing the request.
The connection to the server was lost. Attempting reset: Failed.
!?> ^Z
[2]+  Stopped                 psql
postgres@simonlinux:~$ psql
psql (14.9 (Ubuntu 14.9-0ubuntu0.22.04.1))
Type "help" for help.

postgres=# drop extension chess;
DROP EXTENSION
postgres=# create extension chess;
CREATE EXTENSION
postgres=# select getFirstMoves(chessgame('1. Nf3 Nf6 2. c4 g6 3. Nc3 Bg7 4. d4 O-O 5. Bf4 d5 6. Qb3 dxc4 7. Qxc4 c6 8. e4 Nbd7 9. Rd1 Nb6 10. Qc5 Bg4 11. Bg5 Na4 12. Qa3 Nxc3 13. bxc3 Nxe4 14. Bxe7 Qb6 15. Bc4 Nxc3 16. Bc5 Rfe8+ 17. Kf1 Be6 18. Bxb6 Bxc4+ 19. Kg1 Ne2+ 20. Kf1 Nxd4+ 21. Kg1 Ne2+ 22. Kf1 Nc3+ 23. Kg1 axb6 24. Qb4 Ra4 25. Qxb6 Nxd1 26. h3 Rxa2 27. Kh2 Nxf2 28. Re1 Rxe1 29. Qd8+ Bf8 30. Nxe1 Bd5 31. Nf3 Ne4 32. Qb8 b5 33. h4 h5 34. Ne5 Kg7 35. Kg1 Bc5+ 36. Kf1 Ng3+ 37. Ke1 Bb4+ 38. Kd1 Bb3+ 39. Kc1 Ne2+ 40. Kb1 Nc3+ 41. Kc1 Rc2#'),10);
                    getfirstmoves                     
------------------------------------------------------
 щ1. Nf3 Nf6 2. c4 g6 3. Nc3 Bg7 4. d4 O-O 5. Bf4 d5 
(1 row)

postgres=# select getFirstMoves(chessgame('1. Nf3 Nf6 2. c4 g6 3. Nc3 Bg7 4. d4 O-O 5. Bf4 d5 6. Qb3 dxc4 7. Qxc4 c6 8. e4 Nbd7 9. Rd1 Nb6 10. Qc5 Bg4 11. Bg5 Na4 12. Qa3 Nxc3 13. bxc3 Nxe4 14. Bxe7 Qb6 15. Bc4 Nxc3 16. Bc5 Rfe8+ 17. Kf1 Be6 18. Bxb6 Bxc4+ 19. Kg1 Ne2+ 20. Kf1 Nxd4+ 21. Kg1 Ne2+ 22. Kf1 Nc3+ 23. Kg1 axb6 24. Qb4 Ra4 25. Qxb6 Nxd1 26. h3 Rxa2 27. Kh2 Nxf2 28. Re1 Rxe1 29. Qd8+ Bf8 30. Nxe1 Bd5 31. Nf3 Ne4 32. Qb8 b5 33. h4 h5 34. Ne5 Kg7 35. Kg1 Bc5+ 36. Kf1 Ng3+ 37. Ke1 Bb4+ 38. Kd1 Bb3+ 39. Kc1 Ne2+ 40. Kb1 Nc3+ 41. Kc1 Rc2#'),15);                                                getfirstmoves                                                
-------------------------------------------------------------------------------------------------------------
 щ1. Nf3 Nf6 2. c4 g6 3. Nc3\x011. Nf3 Nf6 2. c4 g6 3. Nc3 Bg7 4. d4 O-O 5. Bf4 d5 6. Qb3 dxc4 7. QS cstring+
   AS '$libdir/ches@
(1 row)

postgres=# drop extension chess;
DROP EXTENSION
postgres=# ^Z
[3]+  Stopped                 psql
postgres@simonlinux:~$ psql
psql (14.9 (Ubuntu 14.9-0ubuntu0.22.04.1))
Type "help" for help.

postgres=# create extension chess;
CREATE EXTENSION
postgres=# select hasOpening(chessgame('test123'),chessgame('test'))
postgres-# ;
 hasopening 
------------
 t
(1 row)

postgres=# 
```
