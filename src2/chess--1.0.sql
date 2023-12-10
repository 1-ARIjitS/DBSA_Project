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

/******************************************************************************
 * Indexes
 ******************************************************************************/

/* In this part, we develop an implementation for the BTree. This implementation
 * consists of defining the following operators: equality (=), inequality (!=), 
 * contains (>) and is contained (<).
 */

CREATE FUNCTION chessgame_eq(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chessgame_eq'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION chessgame_gt(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chessgame_gt'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION chessgame_lt(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chessgame_lt'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION chessgame_ge(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chessgame_ge'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION chessgame_le(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chessgame_le'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION chessgame_cmp(chessgame, chessgame)
  RETURNS integer
  AS 'MODULE_PATHNAME', 'chessgame_cmp'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION chessgame_add(chessgame)
RETURNS chessgame
AS 'MODULE_PATHNAME', 'chessgame_add'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OPERATOR = (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chessgame_eq,
  COMMUTATOR = =
);

CREATE OPERATOR > (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chessgame_gt,
  COMMUTATOR = >, NEGATOR = <=
);

CREATE OPERATOR < (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chessgame_lt,
  COMMUTATOR = <, NEGATOR = >=
);

CREATE OPERATOR >= (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chessgame_ge,
  COMMUTATOR = >=, NEGATOR = <
);

CREATE OPERATOR <= (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chessgame_le,
  COMMUTATOR = <=, NEGATOR = >
);

CREATE OPERATOR CLASS chessgame_ops
    DEFAULT FOR TYPE chessgame USING btree AS
        OPERATOR        1       <,
        OPERATOR        2       <=,
        OPERATOR        3       =,
        OPERATOR        4       >=,
        OPERATOR        5       >,
        FUNCTION        1       chessgame_cmp(chessgame, chessgame);


CREATE FUNCTION hasOpening(chessgame1 chessgame, chessgame2 chessgame)
RETURNS boolean AS $$
SELECT (chessgame1 >= chessgame2 AND chessgame1 < chessgame_add(chessgame2));
$$ LANGUAGE sql STABLE;



/* In this part, we develop an implementation for the GIN index. 
 */

CREATE FUNCTION gin_extractValue(chessgame, internal)
RETURNS internal
AS 'MODULE_PATHNAME', 'gin_extractValue'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION gin_extractQuery(chessboard, internal, integer, internal, internal, internal, internal)
RETURNS internal
AS 'MODULE_PATHNAME', 'gin_extractQuery'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION gin_consistent()
RETURNS BOOLEAN
AS 'MODULE_PATHNAME', 'gin_consistent'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION gin_compare(chessboard, chessboard)
RETURNS integer
AS 'MODULE_PATHNAME', 'gin_compare'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE FUNCTION chessboard_at_op(chessgame, chessboard)
RETURNS BOOLEAN
AS 'MODULE_PATHNAME', 'chessboard_at_op'
LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;


CREATE OPERATOR @ (
  LEFTARG = chessgame, RIGHTARG = chessboard,
  PROCEDURE = chessboard_at_op,
  COMMUTATOR = @
);

CREATE OPERATOR CLASS gin_ops
    DEFAULT FOR TYPE chessgame USING gin AS
        OPERATOR        1       @ (chessgame, chessboard) ,
        FUNCTION        1       gin_compare (chessboard, chessboard),
        FUNCTION        2       gin_extractValue(chessgame, internal),
        FUNCTION        3       gin_extractQuery(chessboard, internal, integer, internal, internal, internal, internal),
        FUNCTION        4       gin_consistent(),
        STORAGE chessboard;

CREATE FUNCTION hasBoard(cg chessgame, cb chessboard, moves integer)
RETURNS boolean AS $$
SELECT ((cg @ cb) AND (getFirstMoves(cg,moves) @ cb));
$$ LANGUAGE sql STABLE;