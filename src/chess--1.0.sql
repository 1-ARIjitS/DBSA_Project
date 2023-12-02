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
  internallength = 512,
  input          = chessgame_in,
  output         = chessgame_out
);

CREATE TYPE chessboard (
  internallength =90,
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

CREATE FUNCTION getBoard(chessgame, int)
RETURNS chessboard
AS 'MODULE_PATHNAME', 'getBoard'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION getFirstMoves(chessgame, int)
RETURNS chessgame
AS 'MODULE_PATHNAME', 'getFirstMoves'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION hasOpening(chessgame, chessgame)
RETURNS boolean
AS 'MODULE_PATHNAME', 'hasOpening'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION hasBoard(chessgame, chessboard, int)
RETURNS boolean
AS 'MODULE_PATHNAME', 'hasBoard'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;


/******************************************************************************
 * Operators
 ******************************************************************************/

/* We are going to create the compare functions: */

CREATE FUNCTION chessgame_eq(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chessgame_eq'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;
CREATE FUNCTION chessgame_ne(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chessgame_ne'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;
CREATE FUNCTION chessgame_left(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chessgame_left'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;
CREATE FUNCTION chessgame_right(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chessgame_right'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION chessgame_cmp(chessgame, chessgame)
  RETURNS integer
  AS 'MODULE_PATHNAME', 'chessgame_cmp'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

/* We are going to create the Btree index: */

CREATE OPERATOR = (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chessgame_eq,
  COMMUTATOR = =, NEGATOR = !=
);

CREATE OPERATOR != (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chessgame_ne,
  COMMUTATOR = !=, NEGATOR = =
);

CREATE OPERATOR > (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chessgame_left,
  COMMUTATOR = >
);

CREATE OPERATOR < (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chessgame_right,
  COMMUTATOR = <
);


CREATE OPERATOR CLASS chessgame_ops
    DEFAULT FOR TYPE chessgame USING btree AS
        OPERATOR        1       < ,
        OPERATOR        2       > ,
        OPERATOR        3       = ,
        OPERATOR        4       != ,
        FUNCTION        1       chessgame_cmp(chessgame, chessgame);