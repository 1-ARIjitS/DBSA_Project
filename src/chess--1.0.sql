-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION chess" to load this file. \quit


CREATE TYPE chessgame;
CREATE TYPE chessboard;

/******************************************************************************
 * Input/Output
 ******************************************************************************/

CREATE OR REPLACE FUNCTION chessgame_in(cstring)
  RETURNS chessgame
  AS 'MODULE_PATHNAME'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OR REPLACE FUNCTION chessgame_out(chessgame)
  RETURNS cstring
  AS 'MODULE_PATHNAME'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;
  
CREATE OR REPLACE FUNCTION chessboard_in(cstring)
  RETURNS chessboard
  AS 'MODULE_PATHNAME'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;
  
  CREATE OR REPLACE FUNCTION chessboard_out(chessboard)
  RETURNS cstring
  AS 'MODULE_PATHNAME'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;
  

CREATE TYPE chessgame (
  internallength = 100,
  input          = chessgame_in,
  output         = chessgame_out
);

CREATE TYPE chessboard (
  internallength = 100,
  input          = chessboard_in,
  output         = chessboard_out
);


-- CREATE OR REPLACE FUNCTION chessgame(text)
--   RETURNS chessgame
--   AS 'MODULE_PATHNAME', 'chessgame_cast_from_text'
--   LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

-- CREATE OR REPLACE FUNCTION text(chessgame)
--   RETURNS text
--   AS 'MODULE_PATHNAME', 'chessgame_cast_to_text'
--   LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;
  
-- CREATE OR REPLACE FUNCTION chessboard(text)
--   RETURNS chessboard
--   AS 'MODULE_PATHNAME', 'chessboard_cast_from_text'
--   LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

-- CREATE OR REPLACE FUNCTION text(chessboard)
--   RETURNS text
--   AS 'MODULE_PATHNAME', 'chessboard_cast_to_text'
--   LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;
  

-- CREATE CAST (text as chessgame) WITH FUNCTION chessgame(text) AS IMPLICIT;
-- CREATE CAST (chessgame as text) WITH FUNCTION text(chessgame);

-- CREATE CAST (text as chessboard) WITH FUNCTION chessboard(text) AS IMPLICIT;
-- CREATE CAST (chessboard as text) WITH FUNCTION text(chessboard);


/******************************************************************************
 * Constructors
 ******************************************************************************/

CREATE FUNCTION chessgame(text)
  RETURNS chessgame
  AS 'MODULE_PATHNAME', 'chessgame_constructor'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

  CREATE FUNCTION chessboard(text)
  RETURNS chessboard
  AS 'MODULE_PATHNAME', 'chessboard_constructor'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

/*****************************************************************************
 * Functions from the assignment
 *****************************************************************************/




/******************************************************************************
 * Operators
 ******************************************************************************/

CREATE FUNCTION chessgame_eq(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chessgame_eq'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION chessboard_eq(chessboard, chessboard)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chessboard_eq'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION chessgame_ne(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chessgame_ne'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

  CREATE FUNCTION chessboard_ne(chessboard, chessboard)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chessboard_ne'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;


/* Complete the following code by filling in the dots (...) 
 * This is the function for the strictly right '>>' operator
 */
/*
CREATE FUNCTION complex_right(...)
  RETURNS ...
  AS 'MODULE_PATHNAME', '...'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;
*/

/*CREATE FUNCTION complex_below(complex, complex)
  *RETURNS boolean
  *AS 'MODULE_PATHNAME', 'complex_below'
  *LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;
*/

/* Write the code for the following function 
 * This is the function for the strictly above '|>>' operator
 */
/*
CREATE FUNCTION ...
*/

CREATE OPERATOR ~= (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chessgame_eq,
  COMMUTATOR = ~=, NEGATOR = <>
);

CREATE OPERATOR ~= (
  LEFTARG = chessboard, RIGHTARG = chessboard,
  PROCEDURE = chessboard_eq,
  COMMUTATOR = ~=, NEGATOR = <>
);


CREATE OPERATOR <> (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chessgame_ne,
  COMMUTATOR = <>, NEGATOR = ~=
);

CREATE OPERATOR <> (
  LEFTARG = chessboard, RIGHTARG = chessboard,
  PROCEDURE = chessboard_ne,
  COMMUTATOR = <>, NEGATOR = ~=
);

/*CREATE OPERATOR << (
  *LEFTARG = complex, RIGHTARG = complex,
  *PROCEDURE = complex_left,
  *COMMUTATOR = >>
*);
*/

/* Complete the following code by filling in the dots (...) 
 * This is the strictly right '>>' operator
 */
/*
CREATE OPERATOR >> (
  LEFTARG = ..., RIGHTARG = ...,
  PROCEDURE = ...,
  COMMUTATOR = ...
);
*/

/*CREATE OPERATOR <<| (
  *LEFTARG = complex, RIGHTARG = complex,
  *PROCEDURE = complex_below,
  *COMMUTATOR = |>>
*);
*/

/* Write the code for the following operator 
 * This is the strictly above '|>>' operator
 */
/*
CREATE OPERATOR ...
*/

/******************************************************************************/



/******************************************************************************/


/******************************************************************************/