#include <stdio.h>
#include "smallchesslib.h"

void putCharacter(char c)
{
  putchar(c);
}

char str[4096];

void putCharStr(char c)
{
  char *s = str;

  while (*s != 0)
    s++;

  *s = c;
  *(s + 1) = 0;
}

uint8_t test(const char *str, uint8_t cond)
{
  printf("- testing %s: ",str);

  if (cond)
  {
    puts("OK");
    return 1;
  }

  puts("ERROR");
  return 0;
}

int strEquals(const char *s1, const char *s2)
{
  while (1)
  {
    if (*s1 != *s2)
      return 0;

    if (*s1 == 0 || *s2 == 0)
      break;

    s1++;
    s2++;
  }

  return 1;
}

int main(){

  // dummy SAN strings used for example
  // char *pgn_string= "1. Nf3 Nf6 2. c4 g6 3. Nc3 Bg7 4. d4 O-O 5. Bf4 d5 6. Qb3 dxc4 7."
  //                        "Qxc4 c6 8. e4 Nbd7 9. Rd1 Nb6 10. Qc5 Bg4 11. Bg5 Na4 12. Qa3 Nxc3 "
  //                        "13. bxc3 Nxe4 14. Bxe7 Qb6 15. Bc4 Nxc3 16. Bc5 Rfe8+ 17."
  //                        "Kf1 Be6 18. Bxb6 Bxc4+ 19. Kg1 Ne2+ 20. Kf1 Nxd4+ 21. Kg1 Ne2+"
  //                        "22. Kf1 Nc3+ 23. Kg1 axb6 24. Qb4 Ra4 25. Qxb6 Nxd1 26. h3 Rxa2"
  //                        "27. Kh2 Nxf2 28. Re1 Rxe1 29. Qd8+ Bf8 30. Nxe1 Bd5 31. Nf3 Ne4"
  //                        "32. Qb8 b5 33. h4 h5 34. Ne5 Kg7 35. Kg1 Bc5+ 36. Kf1 Ng3+ 37."
  //                        "Ke1 Bb4+ 38. Kd1 Bb3+ 39. Kc1 Ne2+ 40. Kb1 Nc3+ 41. Kc1 Rc2#";

 const char* pgn_string = "1. e4 d5 2. exd5 Qxd5 3. Nc3 Qd8 4. Bc4 Nf6 5. Nf3 Bg4 6. h3 Bxf3 "
                        "7. Qxf3 e6 8. Qxb7 Nbd7 9. Nb5 Rc8 10. Nxa7 Nb6 11. Nxc8 Nxc8 "
                        "12. d4 Nd6 13. Bb5+ Nxb5 14. Qxb5+ Nd7 15. d5 exd5 16. Be3 Bd6 "
                        "17. Rd1 Qf6 18. Rxd5 Qg6 19. Bf4 Bxf4 20. Qxd7+ Kf8 21. Qd8#";

  // Initializing record for storing the PGN (SAN) string
  SCL_Record r;
  SCL_recordInit(r);
  SCL_recordFromPGN(r, pgn_string);

  // calcvulating the total number of half moves
  int num_half_moves= SCL_recordLength(r);

  // printing the number of half moves
  printf("number of half moves in the PGN notation= %d\n", num_half_moves);

  // initializing a board to store the represent and make moves using the PGN (SAN) notation
  SCL_Board b;
  SCL_boardInit(b);

  // printing the passed PGN (SAN) notation
  SCL_printPGN(r, putCharacter, b);
  printf("\n");

  // initializing a game to play the chess game
  SCL_Game g;
  // g.board= b;
  // g.record= r;
  int i=0;
  for(int i=0; i<SCL_BOARD_STATE_SIZE; i++)
  {
    g.board[i]= b[i];
  }
  for(int i=0; i<num_half_moves; i++)
  {
    g.record[i]= r[i];
  }
  g.state= SCL_GAME_STATE_PLAYING;
  g.ply= num_half_moves;

  // printing to see the board state containin the ASCII values of all the chess pieces
  // for(int i=0;i<SCL_BOARD_STATE_SIZE;i++)
  // {
  //   printf("%d\n", g.board[i]);
  // }
  SCL_gameInit(&g, 0);

  // applying a fixed number of half moves to the chess game as passed in the argument
  SCL_recordApply(r, b, 20);

  // printing the board after applying the record
  SCL_printBoardSimple(b, putCharacter, 1, SCL_PRINT_FORMAT_UTF8);
  printf("\n");

  // initializing a new board to verify the results
  SCL_Board b2;
  SCL_boardInit(b2);
  // printf("inter");

  // printing the PGN notation after the moves- just for sake of printing does not make sense
  SCL_printPGN(r, putCharacter, b);
  printf("\n");
  // printf("inter");

  // converting the board state to FEN notation
  char fen_string[SCL_FEN_MAX_LENGTH];
  int status= SCL_boardToFEN(b, fen_string);

  // have to figure out what does this number mean
  printf("status= %d\n", status);

  // Printing the obtained FEN notation
  for(int i=0; fen_string[i]!='\0'; i++)
  {
    printf("%c", fen_string[i]);
  }
  printf("\n");

  // verifying the FEN notation using the new board
  SCL_boardFromFEN(b2, fen_string);
  SCL_printBoardSimple(b2, putCharacter, 1, SCL_PRINT_FORMAT_UTF8);
}
