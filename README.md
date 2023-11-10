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

Next meeting: 13 November 16:00 💯

