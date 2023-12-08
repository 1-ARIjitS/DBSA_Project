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
#include <stdbool.h>
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
    char san[SCL_RECORD_MAX_SIZE]; 
} chessgame;

/* Structure to represent the chessboard */
typedef struct chessboard
{
    char fen[SCL_FEN_MAX_LENGTH]; 
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
    for(i=0;i<SCL_RECORD_MAX_SIZE;i++){
        game->san[i] = san[i];
    }                 
    return game;
}

static chessboard *
chessboard_make(const char *fen)
{
    int i;
    chessboard *board = palloc0(sizeof(chessboard));
    for(i=0;i<SCL_FEN_MAX_LENGTH;i++){
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
  else
  {
    print("ERROR: Number of moves passed is greater than the number of half moves in the PGN");
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
    chessboard *resultPtr = (chessboard *)palloc0(sizeof(chessboard));
    *resultPtr = result;

    // Return the pointer to the dynamically allocated copy
    PG_RETURN_ChessBoard(resultPtr);
}

// Function 2: getFirstMoves -> chessgame: Get the SAN moves truncated till the given number of half moves
chessgame getFirstMoves_internal(chessgame cg, int moves)
{
  chessgame cg_truncated;
  char san_str_truncated[SCL_RECORD_MAX_SIZE]={'\0'};

  // Initializing record for storing the PGN (SAN) string
  SCL_Record r;
  SCL_recordInit(r);
  SCL_recordFromPGN(r, cg.san);

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
  if(num_half_moves >= moves)
  {
    char *token= strtok(cg.san, " ");
    int token_counter=0;
    int half_move_counter=0;
    while( token != NULL ) 
    {
      token_counter++;
      // printf( " %s\n", token ); //printing each token
      // printf("%d ", token_counter);
      strcat(san_str_truncated, token);
      strcat(san_str_truncated, " ");
      if(token_counter==2 || token_counter==3)
      {
        half_move_counter++;
        // printf( " %s\n", token ); //printing each token
      }
      if(half_move_counter==moves)
      {
        break;
      }
      if(token_counter==3)
      {
        token_counter=0;
      }
      token = strtok(NULL, " ");
    }
    strcat(san_str_truncated, "\0");
    printf("%s\n", san_str_truncated);
    int ele_count=0;
    for(int i=0;san_str_truncated[i]!='\0';i++)
    {
      // printf("%c",san_str_truncated[i]);
      ele_count+=1;
    }
    // initialize the character array i.e. string
    for(int i=0;i<SCL_RECORD_MAX_SIZE;i++)
    {
      cg_truncated.san[i]='\0';
    }
    for(int i=0;san_str_truncated[i]!='\0';i++)
    {
      cg_truncated.san[i]= san_str_truncated[i];
    }
  }

  else
  {
    printf("ERROR: the number of half moves provided is > the number of half moves in the original PGN notation\n");
  }

  return cg_truncated; 
}

PG_FUNCTION_INFO_V1(getFirstMoves);
Datum 
getFirstMoves(PG_FUNCTION_ARGS)
{
    chessgame *cg = (chessgame *) PG_GETARG_POINTER(0);
    int moves = PG_GETARG_INT32(1);

    // Use getBoard_internal to get the chessboard result
    chessgame result = getFirstMoves_internal(*cg, moves);
    PG_FREE_IF_COPY(cg, 0);
    PG_FREE_IF_COPY(moves, 1); 

    // Create a dynamically allocated copy of the result to return
    chessgame *resultPtr = (chessgame *)palloc0(sizeof(chessgame));
    *resultPtr = result;

    // Return the pointer to the dynamically allocated copy 
    PG_RETURN_ChessBoard(resultPtr);
}


// Function 4: hasBoard -> bool: To check if the given chessgame has the given board state in the given half moves 
bool hasBoard_internal(chessgame cg, chessboard cb, int moves)
{
  chessboard cb_heuristic;
  for(int i=0; i<=moves; i++)
  {
    cb_heuristic= getBoard_internal(cg, i);
    if(strEquals(cb_heuristic.fen, cb.fen))
    {
      return true;
    }
  }
  return false;
}

PG_FUNCTION_INFO_V1(hasBoard);
Datum 
hasBoard(PG_FUNCTION_ARGS)
{
    chessgame *cg = (chessgame *) PG_GETARG_POINTER(0);
    chessboard *cb = (chessboard *) PG_GETARG_POINTER(0);
    int moves = PG_GETARG_INT32(1);

    // Use hasOpening_internal to get the bool result
    bool result = hasBoard_internal(*cg, *cb, moves);
    PG_FREE_IF_COPY(cg, 0);
    PG_FREE_IF_COPY(cb, 1); 
    PG_FREE_IF_COPY(moves, 2);
    PG_RETURN_BOOL(result);
}

/*****************************************************************************/

/* In this first part, we develop an option for the BTree. This option
 * consists on defining following operator: equality (=), inequality (!=), 
 * contains (>) and is contained (<).
 */

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

// /* This function answers to the operation contains (a > b, a constains b) */

PG_FUNCTION_INFO_V1(chessgame_gt);
Datum
chessgame_gt(PG_FUNCTION_ARGS)
{
     chessgame *b1 = (chessgame *) PG_GETARG_POINTER(0);
     chessgame *b2 = (chessgame *) PG_GETARG_POINTER(1);
     bool result;
    
     result = strcmp(b1->san, b2->san) > 0;

     PG_FREE_IF_COPY(b1, 0);
     PG_FREE_IF_COPY(b2, 1);
     PG_RETURN_BOOL(result);
}

// /* This function answers to the operation contains (a >= b, a constains b) */

PG_FUNCTION_INFO_V1(chessgame_ge);
Datum
chessgame_ge(PG_FUNCTION_ARGS)
{
     chessgame *b1 = (chessgame *) PG_GETARG_POINTER(0);
     chessgame *b2 = (chessgame *) PG_GETARG_POINTER(1);
     bool result;
    
     result = strcmp(b1->san, b2->san) >= 0;

     PG_FREE_IF_COPY(b1, 0);
     PG_FREE_IF_COPY(b2, 1);
     PG_RETURN_BOOL(result);
}

// /* This function answers to the operation constained in (a < b, a is contained in b) */


PG_FUNCTION_INFO_V1(chessgame_lt);
Datum
chessgame_lt(PG_FUNCTION_ARGS)
{
     chessgame *b1 = (chessgame *) PG_GETARG_POINTER(0);
     chessgame *b2 = (chessgame *) PG_GETARG_POINTER(1);
     bool result;

     result = strcmp(b1->san,b2->san) < 0;

     PG_FREE_IF_COPY(b1, 0);
     PG_FREE_IF_COPY(b2, 1);
     PG_RETURN_BOOL(result);
}

// /* This function answers to the operation constained in (a < b, a is contained in b) */


PG_FUNCTION_INFO_V1(chessgame_le);
Datum
chessgame_le(PG_FUNCTION_ARGS)
{
     chessgame *b1 = (chessgame *) PG_GETARG_POINTER(0);
     chessgame *b2 = (chessgame *) PG_GETARG_POINTER(1);
     bool result;

     result = strcmp(b1->san,b2->san) <= 0;

     PG_FREE_IF_COPY(b1, 0);
     PG_FREE_IF_COPY(b2, 1);
     PG_RETURN_BOOL(result);
}

// /* cmp function for the B-Tree */
  
PG_FUNCTION_INFO_V1(chessgame_cmp);
Datum
chessgame_cmp(PG_FUNCTION_ARGS)
{
    chessgame *cg1 = (chessgame *) PG_GETARG_POINTER(0);
    chessgame *cg2 = (chessgame *) PG_GETARG_POINTER(1);
    bool result;

    // Compare only the length of the second game's moves
    result = strcmp(cg1->san, cg2->san);

    PG_FREE_IF_COPY(cg1, 0);
    PG_FREE_IF_COPY(cg2, 1);
    PG_RETURN_INT32(result);
}

PG_FUNCTION_INFO_V1(chessgame_add);
Datum
chessgame_add(PG_FUNCTION_ARGS)
{
    chessgame *original_cg = (chessgame *) PG_GETARG_POINTER(0);
    
    // Create a new chessgame with the same san value as the original
    chessgame *new_cg = chessgame_make(original_cg->san);

    // Find the length of the san string in the new chessgame
    int len = strlen(new_cg->san);

    // Only modify the last character if the string is not empty
    if (len > 0) {
        // Increase the ASCII value of the last character by 1
        new_cg->san[len - 1] += 1;
    }

    // Return the new modified chessgame
    PG_RETURN_POINTER(new_cg);
}