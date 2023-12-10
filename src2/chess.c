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
#include <string.h>
#include <stdbool.h>
#include "utils/builtins.h"
#include "libpq/pqformat.h"
#include "access/skey.h" 
#include "fmgr.h"
#include "catalog/pg_collation.h"
#include "catalog/pg_proc.h"
#include "catalog/pg_type.h"
#include "access/gin.h"
#include "access/skey.h"
#include "utils/fmgroids.h"
#include "utils/builtins.h"
#include "utils/numeric.h"
#include "utils/timestamp.h"
#include "utils/array.h"
#include "utils/lsyscache.h"
#include "utils/formatting.h"
#include "utils/fmgroids.h"
#include "smallchesslib.h"

#include "smallchesslib.h"

PG_MODULE_MAGIC;

/*****************************************************************************/
// 1. Type defining

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
// 2. Datatype make functions

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
// 3. Parse functions
 
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
// 4. _in, _out, _recv, _send, _cast_from_text, _cast_to_text functions

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

char str[4096];

void putCharacter(char c)
{
  putchar(c);
}

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


/*****************************************************************************/
// Functions specified in the assignment

// Function 1: getBoard -> chessboard: Get FEN notation of the board after the given number of moves 
chessboard getBoard_internal(chessgame cg, int moves)
{
  // Initializing record for storing the PGN (SAN) string
  int num_half_moves;
  SCL_Record r;
  SCL_recordInit(r);
  SCL_recordFromPGN(r, cg.san);

  // Calculating the total number of half moves
  num_half_moves= SCL_recordLength(r);

  // Initializing a board to store the represent and make moves using the PGN (SAN) notation
  SCL_Board b;
  SCL_boardInit(b);

  // Initializing the retrun type
  chessboard cb;

  // Initializing a game to play the chess game
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
    // Applying a fixed number of half moves to the chess game as passed in the argument
    SCL_recordApply(r, b, moves);

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

    // Initializing a new board to verify the results
    SCL_Board b2;
    SCL_boardInit(b2);

    // Verifying the FEN notation using the new board
    SCL_boardFromFEN(b2, fen_string);
    SCL_printBoardSimple(b2, putCharacter, 1, SCL_PRINT_FORMAT_UTF8);
  }
  else
  {
    print("ERROR: Number of moves passed is greater than the number of half moves in the PGN");
  }

  // Returning the result
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

  // Initializing a board to store the represent and make moves using the PGN (SAN) notation
  SCL_Board b;
  SCL_boardInit(b);

  // Splitting the string using strtok
  if(num_half_moves >= moves)
  {
    char *token= strtok(cg.san, " ");
    int token_counter=0;
    int half_move_counter=0;
    while( token != NULL ) 
    {
      token_counter++;
      strcat(san_str_truncated, token);
      strcat(san_str_truncated, " ");
      if(token_counter==2 || token_counter==3)
      {
        half_move_counter++;
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
      ele_count+=1;
    }
    // Initialize the character array i.e. string
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

/*****************************************************************************/

/* In this part, we develop an implementation for the BTree. This implementation
 * consists of defining the following operators: equality (=), inequality (!=), 
 * contains (>) and is contained (<).
 */

PG_FUNCTION_INFO_V1(chessgame_eq);
Datum
chessgame_eq(PG_FUNCTION_ARGS)
{
     chessgame *g1 = (chessgame *) PG_GETARG_POINTER(0);
     chessgame *g2 = (chessgame *) PG_GETARG_POINTER(1);

     bool result = strcmp(g1->san, g2->san) == 0;
     PG_FREE_IF_COPY(g1, 0);
     PG_FREE_IF_COPY(g2, 1);
     PG_RETURN_BOOL(result);
}

// /* This function answers to the operation contains (a > b) */
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

// /* This function answers to the operation contains (a >= b) */
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

// /* This function answers to the operation constained in (a < b) */
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

// /* This function answers to the operation constained in (a < b) */
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

// /* Cmp function for the B-Tree */
PG_FUNCTION_INFO_V1(chessgame_cmp);
Datum
chessgame_cmp(PG_FUNCTION_ARGS)
{
    chessgame *cg1 = (chessgame *) PG_GETARG_POINTER(0);
    chessgame *cg2 = (chessgame *) PG_GETARG_POINTER(1);
    int result;

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
    chessgame *new_cg = chessgame_make(original_cg->san);

    int len = strlen(new_cg->san);

    if (len > 0) {
        // Increase the ASCII value of the last character by 1
        new_cg->san[len - 1] += 1;
    }

    PG_RETURN_POINTER(new_cg);
}

/*****************************************************************************/
/* In this part, we develop an implementation for the GIN index. 
 */
// Function 4: hasBoard -> bool: To check if the given chessgame has the given board state in the given half moves 
chessboard* truncate_fen(chessboard cb)
{
  char *token;
  chessboard *cb_truncated=(chessboard *)palloc(sizeof(chessboard));
  for(int i=0; i<SCL_FEN_MAX_LENGTH; i++)
  {
    cb_truncated->fen[i]= '\0';
  }
  token = strtok(cb.fen, " ");
  
  // for(int i=0; token[i]!='\0'; i++)
  // {
  //   // printf("%c\n", token[i]);
  //   cb_truncated.fen[i]= token[i];
  // }
  strcpy(cb_truncated->fen, token);
  ereport(WARNING, errmsg_internal("cb_truncated = %s\n", cb_truncated->fen));
  // strcat(cb_truncated.fen, "\0");
  // free(token);
  ereport(WARNING, errmsg_internal("done"));
  return cb_truncated;
}

static int
calc_half_moves(chessgame cg)
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
  return num_half_moves;
}

static bool
at_operator_internal(chessgame cg, chessboard cb)
{
  int num_half_moves= calc_half_moves(cg);
  bool result= false;
  chessboard cb_truncated;
  // chessboard board_state;
  // chessboard truncate_board_state;
  printf("@ num of half moves= %d\n", num_half_moves);
  printf("@ san= %s\n", cg.san);
  printf("@ fen= %s\n", cb.fen);
  cb_truncated= *truncate_fen(cb);
  printf("@ fen truncated= %s\n", cb_truncated.fen);
  int run=0;
  for(int i=0;i<=num_half_moves;i++)
  {
    chessboard board_state;
    chessboard truncate_board_state;
    board_state= getBoard_internal(cg, i);
    truncate_board_state= *truncate_fen(board_state);
    printf("@ run= %d\n", run);
    printf("@ board state fen= %s\n",board_state.fen);
    printf("@ san to truncated fen= %s\n", truncate_board_state.fen);
    if( strcmp( cb_truncated.fen, truncate_board_state.fen ) == 0 )
    {
      result=true;
      printf("@ result= %d\nGOT THE FEN BREAK\n", result);
      break;
    }
    run+=1;
    printf("@ result= %d\n", result);
  }
  return result;
}

PG_FUNCTION_INFO_V1(chessboard_at_op);
Datum
chessboard_at_op(PG_FUNCTION_ARGS)
{
    chessgame *cg = (chessgame *) PG_GETARG_POINTER(0);
    chessboard *cb = (chessboard *) PG_GETARG_POINTER(1);
    bool result = at_operator_internal(*cg, *cb);
    PG_FREE_IF_COPY(cg, 0);
    PG_FREE_IF_COPY(cb, 1);
    PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(gin_extractValue);
Datum
gin_extractValue(PG_FUNCTION_ARGS)
{
    ereport(WARNING, errmsg_internal("%d",1));
    chessgame *cg = PG_GETARG_ChessGame(0);
    ereport(WARNING, errmsg_internal("%d",2));
    int32 *nkeys = (int32 *)PG_GETARG_POINTER(1);
    ereport(WARNING, errmsg_internal("%d",3));
    // bool **nullFlags = (bool **) PG_GETARG_POINTER(2);
    int num_half_moves = calc_half_moves(*cg);
    // *nullFlags = NULL;
    elog(WARNING, "this is the gin_extractValue\n");
    ereport(WARNING, errmsg_internal("%d\n", num_half_moves));
    // printf("@the number of half moves= %d\n", num_half_moves);
    // printf("@size of board_states array= %d\n", (*nkeys) * sizeof(chessboard));
    int total_half_moves= num_half_moves+1;
    // Use getBoard_internal to get the chessboard result
    int count=1;
    Datum *board_states = (Datum*) palloc((total_half_moves) * sizeof(Datum));
    // for (int i = 0; i < total_half_moves; i++)
    // {
    //   for(int j=0;j<SCL_FEN_MAX_LENGTH;j++)
    //   {
    //     board_states[i].fen[j] = '\0';
    //   }
    // }
    for(int i = 0; i < total_half_moves; i++)
    { 
      ereport(WARNING, errmsg_internal("run= %d\n", count));
      chessboard state;
      chessboard *truncate_board_state;
      state= getBoard_internal(*cg, i);
      ereport(WARNING, errmsg_internal("board extracted"));
      truncate_board_state= truncate_fen(state);
      ereport(WARNING, errmsg_internal("truncated_fen= %s\n",truncate_board_state->fen));
      // strcpy(board_states[i].fen, truncate_board_state.fen);
      board_states[i]= truncate_board_state;
      // printf("board_state %d = %s\n", i, board_states[i].fen);
      ereport(WARNING, errmsg_internal("board_state assigned properly"));
      count+=1;
    }
    printf("truncation done for each FEN and board state created\n");
    elog(WARNING, "gin_extractValue is working\n");
    *nkeys = num_half_moves;
    ereport(WARNING, errmsg_internal("nkeys= %d", *nkeys));
    PG_FREE_IF_COPY(cg, 0);
    PG_RETURN_POINTER(board_states);
}

PG_FUNCTION_INFO_V1(gin_extractQuery);
Datum
gin_extractQuery(PG_FUNCTION_ARGS)
{
    elog(WARNING, "this is gin_extractQuery start\n");
    chessboard *cb_query = (chessboard *)PG_GETARG_POINTER(0);
    int32 *nkeys = (int32 *) PG_GETARG_POINTER(1);
    StrategyNumber strategy = PG_GETARG_UINT16(2);
    bool **nullFlags = (bool **) PG_GETARG_POINTER(5);
    int32 *searchMode = (int32 *) PG_GETARG_POINTER(6);
    *searchMode = GIN_SEARCH_MODE_DEFAULT;
    elog(WARNING, "this is gin_extractQuery\n");
    *nkeys= 1;
    Datum *keys = (Datum*)palloc(*nkeys * sizeof(Datum));
    chessboard cb_res;
    for(int i=0; i<SCL_FEN_MAX_LENGTH; i++)
    {
      cb_res.fen[i]='\0';
    }
    elog(WARNING, "this is gin_extractQuery middle\n");
    for (int i = 0; i < *nkeys; i++)
    {
        strcpy(cb_res.fen, cb_query->fen);
        keys[i] = ChessBoardPGetDatum(cb_res.fen);
    }
     elog(WARNING, "this is gin_extractQuery done\n");
    *nullFlags = NULL;
    PG_FREE_IF_COPY(cb_query, 0);
    PG_RETURN_POINTER(keys);
}

PG_FUNCTION_INFO_V1(gin_consistent);
Datum
gin_consistent(PG_FUNCTION_ARGS)
{  
    elog(WARNING, "this is the gin_consistent strat\n");
    bool result=true;
    PG_RETURN_BOOL(result);
    elog(WARNING, "this is the gin_consistent end\n");
}

int gin_compare_internal(chessboard cb1, chessboard cb2)
{
  elog(WARNING, "this is the gin_compare_internal\n");
  int res= strcmp(truncate_fen(cb1)->fen, truncate_fen(cb2)->fen);
  ereport(WARNING, errmsg_internal("cb_truncated = %d\n", res));
  if(res > 0)
  {
    res=1;
  }
  else if(res < 0)
  {
    res=-1;
  }
  else
  {
    res=0;
  }
  return res;
}

PG_FUNCTION_INFO_V1(gin_compare);
Datum
gin_compare(PG_FUNCTION_ARGS)
{
    elog(WARNING, "this is the gin_compare start\n");
    chessboard *cb1 = PG_GETARG_ChessBoard(0);
    chessboard *cb2 = PG_GETARG_ChessBoard(1);
   
    elog(WARNING, "this is the gin_compare\n");
    ereport(WARNING, errmsg_internal("cb_truncated = %s\n", cb1->fen));
    ereport(WARNING, errmsg_internal("cb_truncated = %s\n", cb2->fen));
    int32_t result = gin_compare_internal(*cb1, *cb2);
    PG_FREE_IF_COPY(cb1, 0);
    PG_FREE_IF_COPY(cb2, 1);
    PG_RETURN_INT32(result);
}