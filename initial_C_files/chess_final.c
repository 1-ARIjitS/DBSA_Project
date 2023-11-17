#include <stdio.h>
#include <string.h>
#include <stdbool.h>
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

// PGN (SAN) Notation
typedef struct 
{
  char SAN[SCL_RECORD_MAX_LENGTH];
} chessgame;

// FEN Notation
typedef struct 
{
  char FEN[SCL_FEN_MAX_LENGTH];
} chessboard;

// Function 1: getBoard -> chessboard: Get FEN notation of the board after the given number of moves 
chessboard getBoard(chessgame cg, int moves)
{
  // Initializing record for storing the PGN (SAN) string
  SCL_Record r;
  SCL_recordInit(r);
  SCL_recordFromPGN(r, cg.SAN);

  // calculating the total number of half moves
  int num_half_moves= SCL_recordLength(r);

  // printing the number of half moves
  printf("number of half moves in the PGN notation= %d\n", num_half_moves);

  // initializing a board to store the represent and make moves using the PGN (SAN) notation
  SCL_Board b;
  SCL_boardInit(b);

  // printing the passed PGN (SAN) notation
  SCL_printPGN(r, putCharacter, b);
  printf("\n");

  // initializing the retrun type
  chessboard cb;

  // initializing a game to play the chess game
  SCL_Game g;
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
  SCL_gameInit(&g, 0);

  if(moves <= num_half_moves)
  {
    // applying a fixed number of half moves to the chess game as passed in the argument
    SCL_recordApply(r, b, moves);

    // printing the board after applying the record
    SCL_printBoardSimple(b, putCharacter, 1, SCL_PRINT_FORMAT_UTF8);
    printf("\n");

    // converting the board state to FEN notation
    char fen_string[SCL_FEN_MAX_LENGTH];
    int status= SCL_boardToFEN(b, fen_string);

    // storing the FEN notation into the chessboard datatype
    if (strlen(fen_string) < SCL_FEN_MAX_LENGTH)
    {
      strcpy(cb.FEN, fen_string);
    }
    else
    {
      printf("ERROR: Input SAN string exceeds the maximum length of allowed FEN string");
    }

    // have to figure out what does this number mean
    printf("status= %d\n", status);

    // initializing a new board to verify the results
    SCL_Board b2;
    SCL_boardInit(b2);

    // verifying the FEN notation using the new board
    SCL_boardFromFEN(b2, fen_string);
    SCL_printBoardSimple(b2, putCharacter, 1, SCL_PRINT_FORMAT_UTF8);
  }

  // returning the result
  return cb;
}

// Function 2: getFirstMoves -> chessgame: Get the SAN moves truncated till the given number of half moves
chessgame getFirstMoves(chessgame cg, int moves)
{
  chessgame cg_truncated;

  // Initializing record for storing the PGN (SAN) string
  SCL_Record r;
  SCL_recordInit(r);
  SCL_recordFromPGN(r, cg.SAN);

  // calculating the total number of half moves
  int num_half_moves= SCL_recordLength(r);

  // printing the number of half moves
  printf("number of half moves in the PGN notation= %d\n", num_half_moves);

  // initializing a board to store the represent and make moves using the PGN (SAN) notation
  SCL_Board b;
  SCL_boardInit(b);

  // printing the passed PGN (SAN) notation
  printf("Original PGN notation:\n");
  SCL_printPGN(r, putCharacter, b);
  printf("\n");

  // splitting the string using strtok
  if(num_half_moves > moves)
  {
    char *token= strtok(cg.SAN, " ");
    int half_move_count=1;
    int token_counter=0;
    char san_string_truncated[SCL_RECORD_MAX_SIZE];
    while( token != NULL ) 
    {
      token_counter++;
      // printf( "%s ", token);
      strcat(san_string_truncated, token);
      strcat(san_string_truncated, " ");
      if(token_counter==2 || token_counter==3)
      {
        // printf( " %s %d\n", token, half_move_count );
        // san_string_truncated[half_move_count-1]= *token;
        // strcat(san_string_truncated, token);
        half_move_count++;
        if(half_move_count>moves)
          break;
      }
      token = strtok(NULL, " ");
      if(token_counter==3)
      {
        token_counter=0;
      }
    }
    // printf("\n");

    // printing the truncated string
    // printf("%s\n", san_string_truncated);

    // copying the truncated string to the SAN string of the chessgame datatype
    strcpy(cg_truncated.SAN, san_string_truncated);
  }

  else
  {
    printf("ERROR: the number of half moves provided is > the number of half moves in the original PGN notation");
  }

  return cg_truncated; 
}
// // initializing a board to store the represent and make moves using the PGN (SAN) notation
//   SCL_Board b;
//   SCL_boardInit(b);

//   // printing the passed PGN (SAN) notation
//   // int count=0;
//   // for(int i=0; r[i]!='\0';i++)
//   // {
//   //   printf("%c\n",r[i]);
//   //   count++;
//   // }
//   // printf("%d\n", count);

//   // modifying the counting function to work for our case
//   uint16_t result = 0;
//   int result_num=0;
//   int count=0;

//   while ((r[result] & 0xc0) == 0)
//   {
//     result += 2;
//     printf("%d ", result);
//     result_num= (result/2) + 1;
//     printf("%d ", result_num);
//     count+=1;
//     printf("%d\n", count);
//     if(result_num==moves)
//     {
//       break;
//     }
//   }

//   if(moves%2==0)
//   {
//     moves+=(moves/2);
//   }
//   else
//   {
//     moves+=(moves/2+1);
//   }
//   printf("moves= %d",moves);
//   printf("Original PGN notation\n");
//   SCL_printPGN(r, putCharacter, b);
//   printf("\n");

//   // creating new record to store the truncated record
//   SCL_Record r_truncated;
//   SCL_recordInit(r_truncated);
//   result=0;
//   int i=0;
//   while ( ((r[result] & 0xc0) == 0) && (result<=moves))
//   {
//     result+=2;
//     r_truncated[i]= r[i];
//     i++;
//   }

//   //printing the truncated PGN notation
//   SCL_Board b2;
//   SCL_boardInit(b2);
//   printf("truncated PGN notation\n");
//   SCL_printPGN(r_truncated, putCharacter, b);
//   printf("\n");

//   // assigning the PGN to the truncated board
//   if (strlen(r_truncated) < SCL_RECORD_MAX_LENGTH)
//   {
//     strcpy(cg_truncated.SAN, r_truncated);
//   }

// Function 3: hasOpening -> bool: To check if the original and the given PGN have the same opening
bool hasOpening(chessgame cg1, chessgame cg2)
{
  SCL_Record r1, r2;

  SCL_recordInit(r1);
  SCL_recordInit(r2);

  SCL_recordFromPGN(r1, cg1.SAN);
  SCL_recordFromPGN(r2, cg2.SAN);

  int num_half_moves1= SCL_recordLength(r1);
  printf("half moves in original PGN= %d\n", num_half_moves1);
  int num_half_moves2= SCL_recordLength(r2);
  printf("half moves in check PGN= %d\n", num_half_moves2);

  // int size_r1= sizeof(r1)/sizeof(r1[0]);
  // printf("%d\n", size_r1);
  // int size_r2= sizeof(r2)/sizeof(r2[0]);
  // printf("%d\n", size_r2);

  int count1=0, count2=0;
  for(int i=0; r1[i]!=0; i++)
  {
    // printf("%d", r1[i]);
    count1++;
  }
  printf("c1=%d\n", count1);
  // printf("\n");
  for(int i=0; r2[i]!=0; i++)
  {
    // printf("%d", r2[i]);
    count2++;
  }
  printf("c2=%d\n", count2);

  if(num_half_moves2 < num_half_moves1)
  {
    for(int i=0, j=0; j<(count2-2); i++, j++)
    {
      if(r1[i] != r2[j])
      {
        return false;
      }
    }
  }

  return true;
}

// Function 4: hasBoard -> bool: To check if the given chessgame has the given board state in the given half moves 
bool hasBoard(chessgame cg, chessboard cb, int moves)
{
  chessboard cb_heuristic;
  for(int i=1; i<moves; i++)
  {
    cb_heuristic= getBoard(cg, i);
    if(strEquals(cb_heuristic.FEN, cb.FEN))
    {
      return true;
    }
  }
  return false;
}

int main(){

  // dummy SAN strings used for example
  // const char *pgn_string= "1. Nf3 Nf6 2. c4 g6 3. Nc3 Bg7 4. d4 O-O 5. Bf4 d5 6. Qb3 dxc4 7."
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

 const char* check_string= "1. e4 d5 2. exd5 Qxd5 3. Nc3 Qd8 4. Bc4 Nf6 5. Nf3 Bg4 6. h3 Bxf3 7. Qxf3 e6 8. Qxb7";
  
  // Initializing the PGN (SAN) string
  chessgame cg;
  if (strlen(pgn_string) < SCL_RECORD_MAX_LENGTH)
  {
    strcpy(cg.SAN, pgn_string);
  }
  else
  {
    printf("ERROR: Input SAN string exceeds the maximum length of allowed SAN string");
  }

  chessgame cg_check;
  if (strlen(check_string) < SCL_RECORD_MAX_LENGTH)
  {
    strcpy(cg_check.SAN, check_string);
  }
  else
  {
    printf("ERROR: Input SAN string exceeds the maximum length of allowed SAN string");
  }

  // calling the getBoard function
  // printf("--------------------\n");
  // printf("Testing Function 1\n");
  // printf("--------------------\n");
  // chessboard cb;
  // int num_moves_fun_1= 23;
  // cb= getBoard(cg, num_moves_fun_1);
  // printf("The FEN notation of the board state after %d half moves\n", num_moves_fun_1);
  // // for(int i=0; cb.FEN[i]!='\0'; i++)
  // // {
  // //   printf("%c",cb.FEN[i]);
  // // }
  // printf("%s\n",cb.FEN);
  // printf("\n");

  //calling the getFirstMoves fucntion
  printf("-------------------\n");
  printf("Testing Function 2\n");
  printf("--------------------\n");
  chessgame cg2;
  int num_moves_dun_2= 40;
  cg2= getFirstMoves(cg, num_moves_dun_2);

  // printing the truncated PGN notation
  printf("Truncated PGN notation:\n");
  printf("%s\n", cg2.SAN);

  // calling the hasOpening function
  printf("--------------------\n");
  printf("Testing Function 3\n");
  printf("--------------------\n");
  bool result= hasOpening(cg, cg_check);
  if(result==1)
    printf("provided PGN strings have same opening\n");
  else
    printf("provided PGN strings have different opening\n");
}
