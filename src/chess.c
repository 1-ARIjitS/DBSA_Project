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
    char san[100]; // text is a built in PostgreSQL type
} chessgame;

/* Structure to represent the chessboard */
typedef struct chessboard
{
    char fen[80]; 
} chessboard;


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
    return game->san;
}

static char *
chessboard_to_str(const chessboard *board)
{
    return board->fen;
}


/*****************************************************************************/
// 4. _in, _out, _recv, _send, _cast_from_text, _cast_to_text

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
// 6. Here we should implement our Functions on the Datatypes





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




PG_FUNCTION_INFO_V1(chessgame_left);
Datum
chessgame_left(PG_FUNCTION_ARGS)
{
  //
}

PG_FUNCTION_INFO_V1(chessgame_right);
Datum
chessgame_right(PG_FUNCTION_ARGS)
{
    //
}

PG_FUNCTION_INFO_V1(chessgame_below);
Datum
chessgame_below(PG_FUNCTION_ARGS)
{
    //
}

/* Write the code for the following function 
 * This is the function for the strictly above '|>>' operator
 */
 
PG_FUNCTION_INFO_V1(chessgame_above);
Datum
chessgame_above(PG_FUNCTION_ARGS)
{
    //
}

// same for chessboard here

/*****************************************************************************/
//8. In this section in the complex.c some add and division functions were defined


/*****************************************************************************/
// 9. In this section in the complex.c some distance functions were defined


/*****************************************************************************/