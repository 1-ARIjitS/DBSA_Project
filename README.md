# DBSA_Project
Project for the Database Systems Architecture course at ULB

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
       8 /__////__////__////__////    
      7 ////__////__////__////__/    
     6 /__////__////__////__////    
    5 ////__////__////__////__/    
   4 /__////__////__////__////   
  3 ////__////__////__////__/    
 2 /__////__////__////__////   
1 ////__////__////__////__/   
   a  b  c  d  e  f  g  h

```
