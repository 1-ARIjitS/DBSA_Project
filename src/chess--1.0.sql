-- Esto es una prueba de github


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
 * Indexes
 ******************************************************************************/

/* In this first part, we develop an option for the BTree. This option
 * consists on defining following operator: equality (=), inequality (!=), 
 * contains (@>) and is-contained (<@).
 */

CREATE FUNCTION chessgame_eq(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chessgame_eq'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;
CREATE FUNCTION chessgame_neq(chessgame, chessgame)
  RETURNS boolean
  AS 'MODULE_PATHNAME', 'chessgame_neq'
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
  RETURNS int
  AS 'MODULE_PATHNAME', 'chessgame_cmp'
  LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

CREATE OPERATOR != (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chessgame_eq,
  COMMUTATOR = !=, NEGATOR = =
);

CREATE OPERATOR = (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chessgame_eq,
  COMMUTATOR = =, NEGATOR = !=
);

CREATE OPERATOR @> (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chessgame_left,
  COMMUTATOR = @>
);

CREATE OPERATOR <@ (
  LEFTARG = chessgame, RIGHTARG = chessgame,
  PROCEDURE = chessgame_right,
  COMMUTATOR = <@
);

CREATE OPERATOR CLASS chessgame_ops
    DEFAULT FOR TYPE chessgame USING btree AS
        OPERATOR        1       @> ,
        OPERATOR        2       <@ ,
        OPERATOR        3       = ,
        OPERATOR        4       != ,
        FUNCTION        1       chessgame_cmp(chessgame, chessgame);

/* NB: The operator's strategy number is not arbitrary, but follows the following table:
 * Operation	        Strategy Number
 *  left of               	1
 * left of or overlapping 	2
 * overlapping	            3
 * right of or overlapping	4
 * right of	                5
 * same	                    6 
 * contains	                7
 * contained by	            8
 */

/* In this part, we developed a second version of the BTree. This one 
 * makes use of the following operators: equality (=), inequality (!=),
 * greater than (>), less than (<), greater-equal than (>=) and 
 * less-equal than (<=) */

-- CREATE FUNCTION chessgame_eq(chessgame, chessgame)
--   RETURNS boolean
--   AS 'MODULE_PATHNAME', 'chessgame_eq'
--   LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

-- CREATE FUNCTION chessgame_gt(chessgame, chessgame)
--   RETURNS boolean
--   AS 'MODULE_PATHNAME', 'chessgame_gt'
--   LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

-- CREATE FUNCTION chessgame_lt(chessgame, chessgame)
--   RETURNS boolean
--   AS 'MODULE_PATHNAME', 'chessgame_lt'
--   LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

-- CREATE FUNCTION chessgame_ge(chessgame, chessgame)
--   RETURNS boolean
--   AS 'MODULE_PATHNAME', 'chessgame_ge'
--   LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

-- CREATE FUNCTION chessgame_le(chessgame, chessgame)
--   RETURNS boolean
--   AS 'MODULE_PATHNAME', 'chessgame_le'
--   LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

-- CREATE FUNCTION chessgame_cmp(chessgame, chessgame)
--   RETURNS int
--   AS 'MODULE_PATHNAME', 'chessgame_cmp'
--   LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

-- CREATE OPERATOR = (
--   LEFTARG = chessgame, RIGHTARG = chessgame,
--   PROCEDURE = chessgame_eq,
--   COMMUTATOR = =, NEGATOR = !=
-- );

-- CREATE OPERATOR > (
--   LEFTARG = chessgame, RIGHTARG = chessgame,
--   PROCEDURE = chessgame_gt,
--   COMMUTATOR = >
-- );

-- CREATE OPERATOR < (
--   LEFTARG = chessgame, RIGHTARG = chessgame,
--   PROCEDURE = chessgame_lt,
--   COMMUTATOR = <
-- );

-- CREATE OPERATOR >= (
--   LEFTARG = chessgame, RIGHTARG = chessgame,
--   PROCEDURE = chessgame_ge,
--   COMMUTATOR = >=
-- );

-- CREATE OPERATOR <= (
--   LEFTARG = chessgame, RIGHTARG = chessgame,
--   PROCEDURE = chessgame_le,
--   COMMUTATOR = <=
-- );

-- CREATE OPERATOR CLASS chessgame_btree
--     DEFAULT FOR TYPE chessgame USING btree AS
--         OPERATOR        1       < ,
--         OPERATOR        2       <= ,
--         OPERATOR        3       = ,
--         OPERATOR        4       > ,
--         OPERATOR        5       >= ,
--         FUNCTION        1       chessgame_cmp(chessgame, chessgame);

/* NB: The operator's strategy number is not arbitrary, but follows the following table:
 * Operation	      Strategy Number
 * less than	            1
 * less than or equal	    2 
 * equal	                3
 * greater than or equal	4
 * greater than	          5
 */

/* In this last part, we are going to design a GIN index for
 * the function on the chessgame type to support the 
 * hasBoard predicate.
 */ 

-- CREATE FUNCTION chessboard_eq(chessboard, chessboard)
--   RETURNS boolean
--   AS 'MODULE_PATHNAME', 'chessboard_eq'
--   LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

-- CREATE FUNCTION chessboard_at_greater(chessboard, chessboard)
--   RETURNS boolean
--   AS 'MODULE_PATHNAME', 'chessboard_at_greater'
--   LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

-- CREATE FUNCTION chessboard_at_less(chessboard, chessboard)
--   RETURNS boolean
--   AS 'MODULE_PATHNAME', 'chessboard_at_less'
--   LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

-- CREATE FUNCTION chessboard_and(chessboard, chessboard)
--   RETURNS boolean
--   AS 'MODULE_PATHNAME', 'chessboard_and'
--   LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;


-- CREATE FUNCTION gin_extractValue(chessboard, int32_t)
--   RETURNS chessboard
--   AS 'MODULE_PATHNAME', 'gin_extractValue'
--   LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

-- CREATE FUNCTION gin_consistent (bool, chessboard, int32_t, bool, chessboard)
--   RETURNS boolean
--   AS 'MODULE_PATHNAME', 'gin_consistent'
--   LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

-- CREATE FUNCTION gin_compare (chessboard, chessboard);
--   RETURNS int
--   AS 'MODULE_PATHNAME', 'gin_compare'
--   LANGUAGE C IMMUTABLE STRICT PARALLEL SAFE;

-- CREATE OPERATOR =(
--   LEFTARG = chessboard, RIGHTARG = chessboard,
--   PROCEDURE = chessboard_eq,
--   COMMUTATOR = =, NEGATOR = !=
-- );

-- CREATE OPERATOR && (
--   LEFTARG = chessboard, RIGHTARG = chessboard,
--   PROCEDURE = chessboard_and,
--   COMMUTATOR = &&
-- );

-- CREATE OPERATOR @> (
--   LEFTARG = chessboard, RIGHTARG = chessboard,
--   PROCEDURE = chessboard_at_greater,
--   COMMUTATOR = @>
-- );

-- CREATE OPERATOR <@ (
--   LEFTARG = chessboard, RIGHTARG = chessboard,
--   PROCEDURE = chessboard_at_less,
--   COMMUTATOR = <@
-- );

-- CREATE OPERATOR CLASS gin_ops
--     DEFAULT FOR TYPE chessboard USING gin AS
--         OPERATOR        3       && ,
--         OPERATOR        6       = ,
--         OPERATOR        7       @> ,
--         OPERATOR        8       <@ ,
--         FUNCTION        1       gin_extractValue (chessboard, int32_t),
--         FUNCTION        2       gin_consistent (bool, chessboard, int32_t, bool, chessboard),
--         FUNCTION        3       gin_compare (chessboard, chessboard);

/* NB: The operator's strategy number is not arbitrary, but follows the following table:
 * Operation	        Strategy Number
 *  left of               	1
 * left of or overlapping 	2
 * overlapping	            3
 * right of or overlapping	4
 * right of	                5
 * same	                    6 
 * contains	                7
 * contained by	            8
 */