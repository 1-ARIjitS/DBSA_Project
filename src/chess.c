/*
 * chess.C 
 *
 * PostgreSQL Chess Type:
 *
 * Chessgame
 * Chessboard
 *
 * Authors: Simon Coessens, Benjamin Gold, Arijit Samal, Nils Van Es Ostos
 */

#include <stdio.h>
#include <postgres.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include "utils/builtins.h"
#include "libpq/pqformat.h"

#include "smallchesslib.h"

PG_MODULE_MAGIC;

#define EPSILON         1.0E-06

#define FPzero(A)       (fabs(A) <= EPSILON)
#define FPeq(A,B)       (fabs((A) - (B)) <= EPSILON)
#define FPne(A,B)       (fabs((A) - (B)) > EPSILON)
#define FPlt(A,B)       ((B) - (A) > EPSILON)
#define FPle(A,B)       ((A) - (B) <= EPSILON)
#define FPgt(A,B)       ((A) - (B) > EPSILON)
#define FPge(A,B)       ((B) - (A) <= EPSILON)

/*****************************************************************************/
// 1.

/* Structure to represent the chessgame */
typedef struct chessgame
{
    char san[1000]; 
} chessgame;

/* Structure to represent the chessboard */
typedef struct chessboard
{
    char fen[1000]; 
} chessboard;

#define DatumGetChessBoard(X)  ((chessboard *) DatumGetPointer(X))
#define ChessBoardPGetDatum(X)  PointerGetDatum(X)
#define PG_GETARG_ChessBoard(n) DatumGetChessBoard(PG_GETARG_DATUM(n))
#define PG_RETURN_ChessBoard(x) return ChessBoardPGetDatum(x)

#define DatumGetChessGame(X)  ((chessgame *) DatumGetPointer(X))
#define ChessGamePGetDatum(X)  PointerGetDatum(X)
#define PG_GETARG_ChessGame(n) DatumGetChessGame(PG_GETARG_DATUM(n))
#define PG_RETURN_ChessGame(x) return ChessGamePGetDatum(x)


/*****************************************************************************/
// 2. 

static chessgame *
chessgame_make(const char *san)
{
    int i;
    chessgame *game = palloc0(sizeof(chessgame));
    for(i=0;i<100;i++){
        game->san[i] = san[i];
    }                 
    return game;
}

static chessboard *
chessboard_make(const char *fen)
{
    int i;
    chessboard *board = palloc0(sizeof(chessboard));
    for(i=0;i<100;i++){
        board->fen[i] = fen[i];
    }                 
    return board;
}

/*****************************************************************************/
// 3. parse functions

// This function needs to check for illegal args. 
static chessgame *
chessgame_parse(char **str)
{
    return chessgame_make(*str);
}

static chessboard *
chessboard_parse(char **str)
{
    return chessboard_make(*str);
}

static char *
chessgame_to_str(const chessgame *game)
{
    char *result = psprintf("%s", game->san);
    return result;
}

static char *
chessboard_to_str(const chessboard *board)
{
    char *result = psprintf("%s", board->fen);
    return result;
}


/*****************************************************************************/
// 4. _in, _out, _recv, _send, _cast_from_text, _cast_to_text (I removed send and recv)

PG_FUNCTION_INFO_V1(chessgame_in);
Datum
chessgame_in(PG_FUNCTION_ARGS)
{
    char *str = PG_GETARG_CSTRING(0);
    PG_RETURN_POINTER(chessgame_parse(&str));
}

PG_FUNCTION_INFO_V1(chessgame_out);
Datum
chessgame_out(PG_FUNCTION_ARGS)
{
    chessgame *game = (chessgame *) PG_GETARG_POINTER(0);
    char *result = chessgame_to_str(game);
    PG_FREE_IF_COPY(game, 0);
    PG_RETURN_CSTRING(result);
}

PG_FUNCTION_INFO_V1(chessboard_in);
Datum
chessboard_in(PG_FUNCTION_ARGS)
{
    char *str = PG_GETARG_CSTRING(0);
    PG_RETURN_POINTER(chessboard_parse(&str));
}

PG_FUNCTION_INFO_V1(chessboard_out);
Datum
chessboard_out(PG_FUNCTION_ARGS)
{
    chessboard *board = (chessboard *) PG_GETARG_POINTER(0);
    char *result = chessboard_to_str(board);
    PG_FREE_IF_COPY(board, 0);
    PG_RETURN_CSTRING(result);
}


//PG_FUNCTION_INFO_V1(chessgame_recv);
//Datum
//chessgame_recv(PG_FUNCTION_ARGS)
//{
    //StringInfo buf = (StringInfo) PG_GETARG_POINTER(0);
    // Read the text string from the buffer
    //chessgame *game = palloc(sizeof(chessgame));
    //game->san = pq_getmsgstring(buf);
    //PG_RETURN_POINTER(game);
//}



//PG_FUNCTION_INFO_V1(chessgame_send);
//Datum
//chessgame_send(PG_FUNCTION_ARGS)
//{
    //chessgame *game = (chessgame *) PG_GETARG_POINTER(0);
    //StringInfoData buf;
    //pq_begintypsend(&buf);
    //pq_sendtext(&buf, game->san);
    //PG_FREE_IF_COPY(game, 0);
    //PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
//}


/*****************************************************************************/
// 5.  Constructor functions

PG_FUNCTION_INFO_V1(chessgame_constructor);
Datum
chessgame_constructor(PG_FUNCTION_ARGS)
{
    char *gameStr = PG_GETARG_CSTRING(0);
    PG_RETURN_POINTER(chessgame_make(gameStr));
}

PG_FUNCTION_INFO_V1(chessboard_constructor);
Datum
chessboard_constructor(PG_FUNCTION_ARGS)
{
    char *boardStr = PG_GETARG_CSTRING(0);
    PG_RETURN_POINTER(chessboard_make(boardStr));
}

/*****************************************************************************/
// Help functions

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



// Function 1: getBoard -> chessboard: Get FEN notation of the board after the given number of moves 
chessboard getBoard_internal(chessgame cg, int moves)
{
  // Initializing record for storing the PGN (SAN) string
  int num_half_moves;
  SCL_Record r;
  SCL_recordInit(r);
  SCL_recordFromPGN(r, cg.san);


  // calculating the total number of half moves
  num_half_moves= SCL_recordLength(r);

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
      strcpy(cb.fen, fen_string);
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

PG_FUNCTION_INFO_V1(getBoard);
Datum 
getBoard(PG_FUNCTION_ARGS)
{
    chessgame *cg = (chessgame *) PG_GETARG_POINTER(0);
    int moves = PG_GETARG_INT32(1);

    // Use getBoard_internal to get the chessboard result
    chessboard result = getBoard_internal(*cg, moves);
    PG_FREE_IF_COPY(cg, 0);
    PG_FREE_IF_COPY(moves, 1);

    // Create a dynamically allocated copy of the result to return
    chessboard *resultPtr = (chessboard *)palloc(sizeof(chessboard));
    *resultPtr = result;

    // Return the pointer to the dynamically allocated copy
    PG_RETURN_ChessBoard(resultPtr);
}


// Function 2: getFirstMoves -> chessgame: Get the SAN moves truncated till the given number of half moves
PG_FUNCTION_INFO_V1(getFirstMoves);
Datum getFirstMoves(PG_FUNCTION_ARGS)
{
  chessgame cg_truncated;

  // Initializing record for storing the PGN (SAN) string
  chessgame *cg = (chessgame *) PG_GETARG_POINTER(0);
  int moves = PG_GETARG_INT32(1);
  SCL_Record r;
  SCL_recordInit(r);
  SCL_recordFromPGN(r, cg->san);

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
    char *token= strtok(cg->san, " ");
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
    strcpy(cg_truncated.san, san_string_truncated);
  }

  else
  {
    printf("ERROR: the number of half moves provided is > the number of half moves in the original PGN notation");
  }

  // Create a dynamically allocated copy of the result to return
  chessgame *resultPtr = (chessgame *)palloc(sizeof(chessgame));
  *resultPtr = cg_truncated;

  PG_FREE_IF_COPY(cg, 0);
  PG_FREE_IF_COPY(moves,1);

  // Return the pointer to the dynamically allocated copy
  PG_RETURN_ChessGame(resultPtr);
}

// Function 3: hasOpening -> bool: To check if the original and the given PGN have the same opening
bool hasOpening(chessgame cg1, chessgame cg2)
{
  SCL_Record r1, r2;

  SCL_recordInit(r1);
  SCL_recordInit(r2);

  SCL_recordFromPGN(r1, cg1.san);
  SCL_recordFromPGN(r2, cg2.san);

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
    cb_heuristic= getBoard_internal(cg, i);
    if(strEquals(cb_heuristic.fen, cb.fen))
    {
      return true;
    }
  }
  return false;
}



/*****************************************************************************/
//7. Compare functions and operators

static bool
chessgame_eq_internal(chessgame *g1, chessgame *g2)
{
    return strcmp(g1->san, g2->san) == 0;
}

PG_FUNCTION_INFO_V1(chessgame_eq);
Datum
chessgame_eq(PG_FUNCTION_ARGS)
{
    chessgame *g1 = (chessgame *) PG_GETARG_POINTER(0);
    chessgame *g2 = (chessgame *) PG_GETARG_POINTER(1);
    bool result = chessgame_eq_internal(g1, g2);
    PG_FREE_IF_COPY(g1, 0);
    PG_FREE_IF_COPY(g2, 1);
    PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(chessgame_ne);
Datum
chessgame_ne(PG_FUNCTION_ARGS)
{
    chessgame *g1 = (chessgame *) PG_GETARG_POINTER(0);
    chessgame *g2 = (chessgame *) PG_GETARG_POINTER(1);
    bool result = !chessgame_eq_internal(g1, g2);
    PG_FREE_IF_COPY(g1, 0);
    PG_FREE_IF_COPY(g2, 1);
    PG_RETURN_BOOL(result);
}

static bool
chessboard_eq_internal(chessboard *b1, chessboard *b2)
{
    return strcmp(b1->fen, b2->fen) == 0;
}

PG_FUNCTION_INFO_V1(chessboard_eq);
Datum
chessboard_eq(PG_FUNCTION_ARGS)
{
    chessboard *b1 = (chessboard *) PG_GETARG_POINTER(0);
    chessboard *b2 = (chessboard *) PG_GETARG_POINTER(1);
    bool result = chessboard_eq_internal(b1, b2);
    PG_FREE_IF_COPY(b1, 0);
    PG_FREE_IF_COPY(b2, 1);
    PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(chessboard_ne);
Datum
chessboard_ne(PG_FUNCTION_ARGS)
{
    chessboard *b1 = (chessboard *) PG_GETARG_POINTER(0);
    chessboard *b2 = (chessboard *) PG_GETARG_POINTER(1);
    bool result = !chessboard_eq_internal(b1, b2);
    PG_FREE_IF_COPY(b1, 0);
    PG_FREE_IF_COPY(b2, 1);
    PG_RETURN_BOOL(result);
}



//PG_FUNCTION_INFO_V1(chessgame_left);
//Datum
//chessgame_left(PG_FUNCTION_ARGS)
//{
  
//}

//PG_FUNCTION_INFO_V1(chessgame_right);
//Datum
//chessgame_right(PG_FUNCTION_ARGS)
//{
//    
//}

//PG_FUNCTION_INFO_V1(chessgame_below);
//Datum
//chessgame_below(PG_FUNCTION_ARGS)
//{
    
//}

/* Write the code for the following function 
 * This is the function for the strictly above '|>>' operator
 */
 
//PG_FUNCTION_INFO_V1(chessgame_above);
//Datum
//chessgame_above(PG_FUNCTION_ARGS)
//{
    //
//}

// same for chessboard here

/*****************************************************************************/
//8. In this section in the complex.c some add and division functions were defined


/*****************************************************************************/
// 9. In this section in the complex.c some distance functions were defined


/*****************************************************************************/