#include <postgres.h>
#include "postgres.h"
#include <fmgr.h>
#include <utils/builtins.h>
#include <stdlib.h>

PG_MODULE_MAGIC;


/*
 * Internal storage format for email addresses: The data field
 * contains the domain followed by the local part.  (The '@' is not
 * stored.)  The local_offset field records where the local part
 * starts.  So for example, 'pgsql-hackers@postgresql.org' is stored
 * as { 14, 'postgresql.orgpgsql-hackers' }.
 *
 * This takes the same amount of space as the original but gives us a
 * preparsed form that is easier for sorting and processing.  The
 * local_offset only needs to be 1 byte because domain names are
 * restricted to 255 bytes in length (RFC 1034, RFC 1035, RFC 1123).
 */
typedef struct chessgame {
	char	vl_len_[4];
	uint8	local_offset;
	char	data[FLEXIBLE_ARRAY_MEMBER]; //array with unspecified size
} chessgame;


#define DatumGetEmailAddrP(X)   ((chessgame *) PG_DETOAST_DATUM(X))
#define DatumGetEmailAddrPP(X)  ((chessgame *) PG_DETOAST_DATUM_PACKED(X))
#define EmailAddrPGetDatum(X)   PointerGetDatum(X)

#define PG_GETARG_EMAILADDR_P(n)  DatumGetEmailAddrP(PG_GETARG_DATUM(n))
#define PG_GETARG_EMAILADDR_PP(n) DatumGetEmailAddrPP(PG_GETARG_DATUM(n))
#define PG_RETURN_EMAILADDR_P(x)  PG_RETURN_POINTER(x)



/*****************************************************************************/
// 4. _in, _out, _recv, _send, _cast_from_text, _cast_to_text


PG_FUNCTION_INFO_V1(chessgame_in);
Datum
chessgame_in(PG_FUNCTION_ARGS)
{
	char *s = PG_GETARG_CSTRING(0);
	chessgame *result;
	char *p;
	size_t len;
	int32 result_len;
	size_t domain_len;

	p = strchr(s, '@');

	len =  strlen(s);
	domain_len = len - (p - s) - 1;
	if (domain_len > 255)
		ereport(ERROR,
			(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
			 errmsg("invalid input syntax for type chessgame: domain too long")));

	parse_dot_atom(s, (p - s));
	if (*(p + 1) == '[')
		parse_domain_literal(p + 1, domain_len);
	else
		parse_dot_atom(p + 1, domain_len);

	result_len = offsetof(chessgame, data) + len - 1;
	result = palloc(result_len);
	SET_VARSIZE(result, result_len);
	result->local_offset = domain_len;
	memcpy(result->data, p + 1, domain_len);
	memcpy(result->data + domain_len, s, (p - s));

	PG_RETURN_EMAILADDR_P(result);
}

PG_FUNCTION_INFO_V1(chessgame_out);
Datum
chessgame_out(PG_FUNCTION_ARGS)
{
	chessgame *arg = PG_GETARG_EMAILADDR_P(0);
	char *result;
	size_t result_len;
	size_t local_len;

	result_len = VARSIZE(arg) - offsetof(chessgame, data) + 1 + 1;
	result = palloc(result_len);
	local_len = VARSIZE(arg) - offsetof(chessgame, data) - arg->local_offset;
	memcpy(result, arg->data + arg->local_offset, local_len);
	result[local_len] = '@';
	memcpy(result + local_len + 1, arg->data, arg->local_offset);
	result[result_len - 1] = '\0';

	PG_RETURN_CSTRING(result);
}

static int
strnncmp(const char *s1, size_t n1, const char *s2, size_t n2)
{
	int res;

	res = strncmp(s1, s2, Min(n1, n2));
	if (res != 0)
		return res;

	res = n1 - n2;
	return res;
}

static int
_chessgame_cmp(chessgame *a, chessgame *b)
{
	int res;

	res = strnncmp(a->data, a->local_offset, b->data, b->local_offset);
	if (res != 0)
		return res;

	res = strnncmp(a->data + a->local_offset, VARSIZE(a) - offsetof(chessgame, data) - a->local_offset,
				   b->data + b->local_offset, VARSIZE(b) - offsetof(chessgame, data) - b->local_offset);

	return res;
}

PG_FUNCTION_INFO_V1(chessgame_lt);
Datum
chessgame_lt(PG_FUNCTION_ARGS)
{
	chessgame *arg1 = PG_GETARG_EMAILADDR_PP(0);
	chessgame *arg2 = PG_GETARG_EMAILADDR_PP(1);

	PG_RETURN_BOOL(_chessgame_cmp(arg1, arg2) < 0);
}

PG_FUNCTION_INFO_V1(chessgame_le);
Datum
chessgame_le(PG_FUNCTION_ARGS)
{
	chessgame *arg1 = PG_GETARG_EMAILADDR_PP(0);
	chessgame *arg2 = PG_GETARG_EMAILADDR_PP(1);

	PG_RETURN_BOOL(_chessgame_cmp(arg1, arg2) <= 0);
}

PG_FUNCTION_INFO_V1(chessgame_eq);
Datum
chessgame_eq(PG_FUNCTION_ARGS)
{
	chessgame *arg1 = PG_GETARG_EMAILADDR_PP(0);
	chessgame *arg2 = PG_GETARG_EMAILADDR_PP(1);

	PG_RETURN_BOOL(_chessgame_cmp(arg1, arg2) == 0);
}

PG_FUNCTION_INFO_V1(chessgame_ne);
Datum
chessgame_ne(PG_FUNCTION_ARGS)
{
	chessgame *arg1 = PG_GETARG_EMAILADDR_PP(0);
	chessgame *arg2 = PG_GETARG_EMAILADDR_PP(1);

	PG_RETURN_BOOL(_chessgame_cmp(arg1, arg2) != 0);
}

PG_FUNCTION_INFO_V1(chessgame_ge);
Datum
chessgame_ge(PG_FUNCTION_ARGS)
{
	chessgame *arg1 = PG_GETARG_EMAILADDR_PP(0);
	chessgame *arg2 = PG_GETARG_EMAILADDR_PP(1);

	PG_RETURN_BOOL(_chessgame_cmp(arg1, arg2) >= 0);
}

PG_FUNCTION_INFO_V1(chessgame_gt);
Datum
chessgame_gt(PG_FUNCTION_ARGS)
{
	chessgame *arg1 = PG_GETARG_EMAILADDR_PP(0);
	chessgame *arg2 = PG_GETARG_EMAILADDR_PP(1);

	PG_RETURN_BOOL(_chessgame_cmp(arg1, arg2) > 0);
}

PG_FUNCTION_INFO_V1(chessgame_cmp);
Datum
chessgame_cmp(PG_FUNCTION_ARGS)
{
	chessgame *arg1 = PG_GETARG_EMAILADDR_PP(0);
	chessgame *arg2 = PG_GETARG_EMAILADDR_PP(1);

	PG_RETURN_INT32(_chessgame_cmp(arg1, arg2));
}
