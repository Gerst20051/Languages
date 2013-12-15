
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse         base_sql_parse
#define yylex           base_sql_lex
#define yyerror         base_sql_error
#define yylval          base_sql_lval
#define yychar          base_sql_char
#define yydebug         base_sql_debug
#define yynerrs         base_sql_nerrs


/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 23 "drizzled/sql_yacc.yy"

/* session is passed as an argument to yyparse(), and subsequently to yylex().
** The type will be void*, so it must be  cast to (Session*) when used.
** Use the YYSession macro for this.
*/

#define YYSession (session)

#define YYENABLE_NLS 0
#define YYLTYPE_IS_TRIVIAL 0

#define YYINITDEPTH 100
#define YYMAXDEPTH 3200                        /* Because of 64K stack */
#define Lex (session->lex())

#include <config.h>
#include <cstdio>
#include <drizzled/parser.h>
#include <drizzled/session.h>
#include <drizzled/alter_column.h>
#include <drizzled/alter_drop.h>
#include <drizzled/alter_info.h>
#include <drizzled/item/subselect.h>

int yylex(union ParserType *yylval, drizzled::Session *session);

#define yyoverflow(A,B,C,D,E,F)               \
  {                                           \
    unsigned long val= *(F);                          \
    if (drizzled::my_yyoverflow((B), (D), &val)) \
    {                                         \
      yyerror(NULL, (char*) (A));                   \
      return 2;                               \
    }                                         \
    else                                      \
    {                                         \
      *(F)= (YYSIZE_T)val;                    \
    }                                         \
  }

#define DRIZZLE_YYABORT                         \
  do                                          \
  {                                           \
    YYABORT;                                  \
  } while (0)

#define DRIZZLE_YYABORT_UNLESS(A)         \
  if (!(A))                             \
  {                                     \
    parser::my_parse_error(YYSession->m_lip);\
    DRIZZLE_YYABORT;                      \
  }

namespace drizzled
{

class Table_ident;
class Item;
class Item_num;

namespace item
{
class Boolean;
class True;
class False;
}


/**
  @brief Bison callback to report a syntax/OOM error

  This function is invoked by the bison-generated parser
  when a syntax error, a parse error or an out-of-memory
  condition occurs. This function is not invoked when the
  parser is requested to abort by semantic action code
  by means of YYABORT or YYACCEPT macros. This is why these
  macros should not be used (use DRIZZLE_YYABORT/DRIZZLE_YYACCEPT
  instead).

  The parser will abort immediately after invoking this callback.

  This function is not for use in semantic actions and is internal to
  the parser, as it performs some pre-return cleanup.
  In semantic actions, please use parser::my_parse_error or my_error to
  push an error into the error stack and DRIZZLE_YYABORT
  to abort from the parser.
*/

static void base_sql_error(drizzled::Session *session, const char *s)
{
  parser::errorOn(session, s);
}

} /* namespace drizzled; */

using namespace drizzled;


/* Line 189 of yacc.c  */
#line 180 "drizzled/sql_yacc.cc"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ABORT_SYM = 258,
     ACTION = 259,
     ADD_SYM = 260,
     ADDDATE_SYM = 261,
     AFTER_SYM = 262,
     AGGREGATE_SYM = 263,
     ALL = 264,
     ALTER_SYM = 265,
     ANALYZE_SYM = 266,
     AND_SYM = 267,
     ANY_SYM = 268,
     AS = 269,
     ASC = 270,
     ASENSITIVE_SYM = 271,
     AT_SYM = 272,
     AUTO_INC = 273,
     AVG_SYM = 274,
     BEFORE_SYM = 275,
     BEGIN_SYM = 276,
     BETWEEN_SYM = 277,
     BIGINT_SYM = 278,
     BINARY = 279,
     BIN_NUM = 280,
     BIT_SYM = 281,
     BLOB_SYM = 282,
     BOOLEAN_SYM = 283,
     BOOL_SYM = 284,
     BOTH = 285,
     BTREE_SYM = 286,
     BY = 287,
     CALL_SYM = 288,
     CASCADE = 289,
     CASCADED = 290,
     CASE_SYM = 291,
     CAST_SYM = 292,
     CATALOG_SYM = 293,
     CHAIN_SYM = 294,
     CHANGE_SYM = 295,
     CHAR_SYM = 296,
     CHECKSUM_SYM = 297,
     CHECK_SYM = 298,
     CLOSE_SYM = 299,
     COALESCE = 300,
     COLLATE_SYM = 301,
     COLLATION_SYM = 302,
     COLUMNS = 303,
     COLUMN_SYM = 304,
     COMMENT_SYM = 305,
     COMMITTED_SYM = 306,
     COMMIT_SYM = 307,
     COMPACT_SYM = 308,
     COMPRESSED_SYM = 309,
     CONCURRENT = 310,
     CONDITION_SYM = 311,
     CONNECTION_SYM = 312,
     CONSISTENT_SYM = 313,
     CONSTRAINT = 314,
     CONTAINS_SYM = 315,
     CONVERT_SYM = 316,
     COUNT_SYM = 317,
     CREATE = 318,
     CROSS = 319,
     CUBE_SYM = 320,
     CURDATE = 321,
     CURRENT_USER = 322,
     CURSOR_SYM = 323,
     DATABASE = 324,
     DATABASES = 325,
     DATA_SYM = 326,
     DATETIME_SYM = 327,
     DATE_ADD_INTERVAL = 328,
     DATE_SUB_INTERVAL = 329,
     DATE_SYM = 330,
     DAY_HOUR_SYM = 331,
     DAY_MICROSECOND_SYM = 332,
     DAY_MINUTE_SYM = 333,
     DAY_SECOND_SYM = 334,
     DAY_SYM = 335,
     DEALLOCATE_SYM = 336,
     DECIMAL_NUM = 337,
     DECIMAL_SYM = 338,
     DECLARE_SYM = 339,
     DEFAULT = 340,
     DELETE_SYM = 341,
     DESC = 342,
     DESCRIBE = 343,
     DETERMINISTIC_SYM = 344,
     DISABLE_SYM = 345,
     DISCARD = 346,
     DISTINCT = 347,
     DIV_SYM = 348,
     DO_SYM = 349,
     DOUBLE_SYM = 350,
     DROP = 351,
     DUMPFILE = 352,
     DUPLICATE_SYM = 353,
     DYNAMIC_SYM = 354,
     EACH_SYM = 355,
     ELSE = 356,
     ENABLE_SYM = 357,
     ENCLOSED = 358,
     END = 359,
     ENDS_SYM = 360,
     END_OF_INPUT = 361,
     ENGINE_SYM = 362,
     ENUM_SYM = 363,
     EQUAL_SYM = 364,
     ERRORS = 365,
     ESCAPED = 366,
     ESCAPE_SYM = 367,
     EXCLUSIVE_SYM = 368,
     EXECUTE_SYM = 369,
     EXISTS = 370,
     EXTENDED_SYM = 371,
     EXTRACT_SYM = 372,
     FALSE_SYM = 373,
     FILE_SYM = 374,
     FIRST_SYM = 375,
     FIXED_SYM = 376,
     FLOAT_NUM = 377,
     FLUSH_SYM = 378,
     FORCE_SYM = 379,
     FOREIGN = 380,
     FOR_SYM = 381,
     FOUND_SYM = 382,
     FRAC_SECOND_SYM = 383,
     FROM = 384,
     FULL = 385,
     GE = 386,
     GLOBAL_SYM = 387,
     GROUP_SYM = 388,
     GROUP_CONCAT_SYM = 389,
     HASH_SYM = 390,
     HAVING = 391,
     HEX_NUM = 392,
     HOUR_MICROSECOND_SYM = 393,
     HOUR_MINUTE_SYM = 394,
     HOUR_SECOND_SYM = 395,
     HOUR_SYM = 396,
     IDENT = 397,
     IDENTIFIED_SYM = 398,
     IDENTITY_SYM = 399,
     IDENT_QUOTED = 400,
     IF = 401,
     IGNORE_SYM = 402,
     IMPORT = 403,
     INDEXES = 404,
     INDEX_SYM = 405,
     INFILE = 406,
     INNER_SYM = 407,
     INOUT_SYM = 408,
     INSENSITIVE_SYM = 409,
     INSERT = 410,
     INTERVAL_SYM = 411,
     INTO = 412,
     INT_SYM = 413,
     IN_SYM = 414,
     IS = 415,
     ISOLATION = 416,
     ITERATE_SYM = 417,
     JOIN_SYM = 418,
     KEYS = 419,
     KEY_BLOCK_SIZE = 420,
     KEY_SYM = 421,
     KILL_SYM = 422,
     LAST_SYM = 423,
     LE = 424,
     LEADING = 425,
     LEFT = 426,
     LEVEL_SYM = 427,
     LEX_HOSTNAME = 428,
     LIKE = 429,
     LIMIT = 430,
     LINES = 431,
     LOAD = 432,
     LOCAL_SYM = 433,
     LOCKS_SYM = 434,
     LOCK_SYM = 435,
     LOGS_SYM = 436,
     LONG_NUM = 437,
     LONG_SYM = 438,
     MATCH = 439,
     MAX_SYM = 440,
     MAX_VALUE_SYM = 441,
     MEDIUM_SYM = 442,
     MERGE_SYM = 443,
     MICROSECOND_SYM = 444,
     MINUTE_MICROSECOND_SYM = 445,
     MINUTE_SECOND_SYM = 446,
     MINUTE_SYM = 447,
     MIN_SYM = 448,
     MODE_SYM = 449,
     MODIFIES_SYM = 450,
     MODIFY_SYM = 451,
     MOD_SYM = 452,
     MONTH_SYM = 453,
     NAMES_SYM = 454,
     NAME_SYM = 455,
     NATIONAL_SYM = 456,
     NATURAL = 457,
     NE = 458,
     NEW_SYM = 459,
     NEXT_SYM = 460,
     NONE_SYM = 461,
     NOT_SYM = 462,
     NOW_SYM = 463,
     NO_SYM = 464,
     NULL_SYM = 465,
     NUM = 466,
     NUMERIC_SYM = 467,
     OFFLINE_SYM = 468,
     OFFSET_SYM = 469,
     ON = 470,
     ONE_SHOT_SYM = 471,
     ONE_SYM = 472,
     ONLINE_SYM = 473,
     OPEN_SYM = 474,
     OPTIMIZE = 475,
     OPTION = 476,
     OPTIONALLY = 477,
     ORDER_SYM = 478,
     OR_SYM = 479,
     OUTER = 480,
     OUTFILE = 481,
     OUT_SYM = 482,
     PARTIAL = 483,
     POSITION_SYM = 484,
     PRECISION = 485,
     PREV_SYM = 486,
     PRIMARY_SYM = 487,
     PROCESS = 488,
     PROCESSLIST_SYM = 489,
     QUARTER_SYM = 490,
     QUERY_SYM = 491,
     RANGE_SYM = 492,
     READS_SYM = 493,
     READ_SYM = 494,
     READ_WRITE_SYM = 495,
     REAL = 496,
     REDUNDANT_SYM = 497,
     REGEXP_SYM = 498,
     REFERENCES = 499,
     RELEASE_SYM = 500,
     RENAME = 501,
     REPEATABLE_SYM = 502,
     REPEAT_SYM = 503,
     REPLACE = 504,
     REPLICATE = 505,
     REPLICATION = 506,
     RESTRICT = 507,
     RETURNS_SYM = 508,
     RETURN_SYM = 509,
     REVOKE = 510,
     RIGHT = 511,
     ROLLBACK_SYM = 512,
     ROLLUP_SYM = 513,
     ROUTINE_SYM = 514,
     ROWS_SYM = 515,
     ROW_FORMAT_SYM = 516,
     ROW_SYM = 517,
     SAVEPOINT_SYM = 518,
     SECOND_MICROSECOND_SYM = 519,
     SECOND_SYM = 520,
     SECURITY_SYM = 521,
     SELECT_SYM = 522,
     SENSITIVE_SYM = 523,
     SEPARATOR_SYM = 524,
     SERIALIZABLE_SYM = 525,
     SERIAL_SYM = 526,
     SESSION_SYM = 527,
     SERVER_SYM = 528,
     SET_SYM = 529,
     SET_VAR = 530,
     SHARE_SYM = 531,
     SHOW = 532,
     SIGNED_SYM = 533,
     SIMPLE_SYM = 534,
     SNAPSHOT_SYM = 535,
     SPECIFIC_SYM = 536,
     SQLEXCEPTION_SYM = 537,
     SQLSTATE_SYM = 538,
     SQLWARNING_SYM = 539,
     SQL_BIG_RESULT = 540,
     SQL_BUFFER_RESULT = 541,
     SQL_CALC_FOUND_ROWS = 542,
     SQL_SMALL_RESULT = 543,
     SQL_SYM = 544,
     STARTING = 545,
     START_SYM = 546,
     STATUS_SYM = 547,
     STDDEV_SAMP_SYM = 548,
     STD_SYM = 549,
     STOP_SYM = 550,
     STORED_SYM = 551,
     STRAIGHT_JOIN = 552,
     STRING_SYM = 553,
     SUBDATE_SYM = 554,
     SUBJECT_SYM = 555,
     SUBSTRING = 556,
     SUM_SYM = 557,
     SUSPEND_SYM = 558,
     SYSDATE = 559,
     TABLES = 560,
     TABLESPACE = 561,
     TABLE_REF_PRIORITY = 562,
     TABLE_SYM = 563,
     TEMPORARY_SYM = 564,
     TERMINATED = 565,
     TEXT_STRING = 566,
     TEXT_SYM = 567,
     THEN_SYM = 568,
     TIME_SYM = 569,
     TIMESTAMP_SYM = 570,
     TIMESTAMP_ADD = 571,
     TIMESTAMP_DIFF = 572,
     TO_SYM = 573,
     TRAILING = 574,
     TRANSACTION_SYM = 575,
     TRIM = 576,
     TRUE_SYM = 577,
     TRUNCATE_SYM = 578,
     TYPE_SYM = 579,
     ULONGLONG_NUM = 580,
     UNCOMMITTED_SYM = 581,
     UNDOFILE_SYM = 582,
     UNDO_SYM = 583,
     UNION_SYM = 584,
     UNIQUE_SYM = 585,
     UNKNOWN_SYM = 586,
     UNLOCK_SYM = 587,
     UNSIGNED_SYM = 588,
     UPDATE_SYM = 589,
     USAGE = 590,
     USER = 591,
     USE_SYM = 592,
     USING = 593,
     UTC_DATE_SYM = 594,
     UTC_TIMESTAMP_SYM = 595,
     UUID_SYM = 596,
     VALUES = 597,
     VALUE_SYM = 598,
     VARBINARY = 599,
     VARCHAR_SYM = 600,
     VARIABLES = 601,
     VARIANCE_SYM = 602,
     VARYING = 603,
     VAR_SAMP_SYM = 604,
     WAIT_SYM = 605,
     WARNINGS = 606,
     WEEK_SYM = 607,
     WHEN_SYM = 608,
     WHERE = 609,
     WITH = 610,
     WITH_ROLLUP_SYM = 611,
     WORK_SYM = 612,
     WRITE_SYM = 613,
     XOR = 614,
     YEAR_MONTH_SYM = 615,
     YEAR_SYM = 616,
     ZEROFILL_SYM = 617,
     LESS_THAN = 618,
     GREATER_THAN = 619,
     CONCAT = 620,
     SHIFT_RIGHT = 621,
     SHIFT_LEFT = 622,
     UMINUS = 623
   };
#endif
/* Tokens.  */
#define ABORT_SYM 258
#define ACTION 259
#define ADD_SYM 260
#define ADDDATE_SYM 261
#define AFTER_SYM 262
#define AGGREGATE_SYM 263
#define ALL 264
#define ALTER_SYM 265
#define ANALYZE_SYM 266
#define AND_SYM 267
#define ANY_SYM 268
#define AS 269
#define ASC 270
#define ASENSITIVE_SYM 271
#define AT_SYM 272
#define AUTO_INC 273
#define AVG_SYM 274
#define BEFORE_SYM 275
#define BEGIN_SYM 276
#define BETWEEN_SYM 277
#define BIGINT_SYM 278
#define BINARY 279
#define BIN_NUM 280
#define BIT_SYM 281
#define BLOB_SYM 282
#define BOOLEAN_SYM 283
#define BOOL_SYM 284
#define BOTH 285
#define BTREE_SYM 286
#define BY 287
#define CALL_SYM 288
#define CASCADE 289
#define CASCADED 290
#define CASE_SYM 291
#define CAST_SYM 292
#define CATALOG_SYM 293
#define CHAIN_SYM 294
#define CHANGE_SYM 295
#define CHAR_SYM 296
#define CHECKSUM_SYM 297
#define CHECK_SYM 298
#define CLOSE_SYM 299
#define COALESCE 300
#define COLLATE_SYM 301
#define COLLATION_SYM 302
#define COLUMNS 303
#define COLUMN_SYM 304
#define COMMENT_SYM 305
#define COMMITTED_SYM 306
#define COMMIT_SYM 307
#define COMPACT_SYM 308
#define COMPRESSED_SYM 309
#define CONCURRENT 310
#define CONDITION_SYM 311
#define CONNECTION_SYM 312
#define CONSISTENT_SYM 313
#define CONSTRAINT 314
#define CONTAINS_SYM 315
#define CONVERT_SYM 316
#define COUNT_SYM 317
#define CREATE 318
#define CROSS 319
#define CUBE_SYM 320
#define CURDATE 321
#define CURRENT_USER 322
#define CURSOR_SYM 323
#define DATABASE 324
#define DATABASES 325
#define DATA_SYM 326
#define DATETIME_SYM 327
#define DATE_ADD_INTERVAL 328
#define DATE_SUB_INTERVAL 329
#define DATE_SYM 330
#define DAY_HOUR_SYM 331
#define DAY_MICROSECOND_SYM 332
#define DAY_MINUTE_SYM 333
#define DAY_SECOND_SYM 334
#define DAY_SYM 335
#define DEALLOCATE_SYM 336
#define DECIMAL_NUM 337
#define DECIMAL_SYM 338
#define DECLARE_SYM 339
#define DEFAULT 340
#define DELETE_SYM 341
#define DESC 342
#define DESCRIBE 343
#define DETERMINISTIC_SYM 344
#define DISABLE_SYM 345
#define DISCARD 346
#define DISTINCT 347
#define DIV_SYM 348
#define DO_SYM 349
#define DOUBLE_SYM 350
#define DROP 351
#define DUMPFILE 352
#define DUPLICATE_SYM 353
#define DYNAMIC_SYM 354
#define EACH_SYM 355
#define ELSE 356
#define ENABLE_SYM 357
#define ENCLOSED 358
#define END 359
#define ENDS_SYM 360
#define END_OF_INPUT 361
#define ENGINE_SYM 362
#define ENUM_SYM 363
#define EQUAL_SYM 364
#define ERRORS 365
#define ESCAPED 366
#define ESCAPE_SYM 367
#define EXCLUSIVE_SYM 368
#define EXECUTE_SYM 369
#define EXISTS 370
#define EXTENDED_SYM 371
#define EXTRACT_SYM 372
#define FALSE_SYM 373
#define FILE_SYM 374
#define FIRST_SYM 375
#define FIXED_SYM 376
#define FLOAT_NUM 377
#define FLUSH_SYM 378
#define FORCE_SYM 379
#define FOREIGN 380
#define FOR_SYM 381
#define FOUND_SYM 382
#define FRAC_SECOND_SYM 383
#define FROM 384
#define FULL 385
#define GE 386
#define GLOBAL_SYM 387
#define GROUP_SYM 388
#define GROUP_CONCAT_SYM 389
#define HASH_SYM 390
#define HAVING 391
#define HEX_NUM 392
#define HOUR_MICROSECOND_SYM 393
#define HOUR_MINUTE_SYM 394
#define HOUR_SECOND_SYM 395
#define HOUR_SYM 396
#define IDENT 397
#define IDENTIFIED_SYM 398
#define IDENTITY_SYM 399
#define IDENT_QUOTED 400
#define IF 401
#define IGNORE_SYM 402
#define IMPORT 403
#define INDEXES 404
#define INDEX_SYM 405
#define INFILE 406
#define INNER_SYM 407
#define INOUT_SYM 408
#define INSENSITIVE_SYM 409
#define INSERT 410
#define INTERVAL_SYM 411
#define INTO 412
#define INT_SYM 413
#define IN_SYM 414
#define IS 415
#define ISOLATION 416
#define ITERATE_SYM 417
#define JOIN_SYM 418
#define KEYS 419
#define KEY_BLOCK_SIZE 420
#define KEY_SYM 421
#define KILL_SYM 422
#define LAST_SYM 423
#define LE 424
#define LEADING 425
#define LEFT 426
#define LEVEL_SYM 427
#define LEX_HOSTNAME 428
#define LIKE 429
#define LIMIT 430
#define LINES 431
#define LOAD 432
#define LOCAL_SYM 433
#define LOCKS_SYM 434
#define LOCK_SYM 435
#define LOGS_SYM 436
#define LONG_NUM 437
#define LONG_SYM 438
#define MATCH 439
#define MAX_SYM 440
#define MAX_VALUE_SYM 441
#define MEDIUM_SYM 442
#define MERGE_SYM 443
#define MICROSECOND_SYM 444
#define MINUTE_MICROSECOND_SYM 445
#define MINUTE_SECOND_SYM 446
#define MINUTE_SYM 447
#define MIN_SYM 448
#define MODE_SYM 449
#define MODIFIES_SYM 450
#define MODIFY_SYM 451
#define MOD_SYM 452
#define MONTH_SYM 453
#define NAMES_SYM 454
#define NAME_SYM 455
#define NATIONAL_SYM 456
#define NATURAL 457
#define NE 458
#define NEW_SYM 459
#define NEXT_SYM 460
#define NONE_SYM 461
#define NOT_SYM 462
#define NOW_SYM 463
#define NO_SYM 464
#define NULL_SYM 465
#define NUM 466
#define NUMERIC_SYM 467
#define OFFLINE_SYM 468
#define OFFSET_SYM 469
#define ON 470
#define ONE_SHOT_SYM 471
#define ONE_SYM 472
#define ONLINE_SYM 473
#define OPEN_SYM 474
#define OPTIMIZE 475
#define OPTION 476
#define OPTIONALLY 477
#define ORDER_SYM 478
#define OR_SYM 479
#define OUTER 480
#define OUTFILE 481
#define OUT_SYM 482
#define PARTIAL 483
#define POSITION_SYM 484
#define PRECISION 485
#define PREV_SYM 486
#define PRIMARY_SYM 487
#define PROCESS 488
#define PROCESSLIST_SYM 489
#define QUARTER_SYM 490
#define QUERY_SYM 491
#define RANGE_SYM 492
#define READS_SYM 493
#define READ_SYM 494
#define READ_WRITE_SYM 495
#define REAL 496
#define REDUNDANT_SYM 497
#define REGEXP_SYM 498
#define REFERENCES 499
#define RELEASE_SYM 500
#define RENAME 501
#define REPEATABLE_SYM 502
#define REPEAT_SYM 503
#define REPLACE 504
#define REPLICATE 505
#define REPLICATION 506
#define RESTRICT 507
#define RETURNS_SYM 508
#define RETURN_SYM 509
#define REVOKE 510
#define RIGHT 511
#define ROLLBACK_SYM 512
#define ROLLUP_SYM 513
#define ROUTINE_SYM 514
#define ROWS_SYM 515
#define ROW_FORMAT_SYM 516
#define ROW_SYM 517
#define SAVEPOINT_SYM 518
#define SECOND_MICROSECOND_SYM 519
#define SECOND_SYM 520
#define SECURITY_SYM 521
#define SELECT_SYM 522
#define SENSITIVE_SYM 523
#define SEPARATOR_SYM 524
#define SERIALIZABLE_SYM 525
#define SERIAL_SYM 526
#define SESSION_SYM 527
#define SERVER_SYM 528
#define SET_SYM 529
#define SET_VAR 530
#define SHARE_SYM 531
#define SHOW 532
#define SIGNED_SYM 533
#define SIMPLE_SYM 534
#define SNAPSHOT_SYM 535
#define SPECIFIC_SYM 536
#define SQLEXCEPTION_SYM 537
#define SQLSTATE_SYM 538
#define SQLWARNING_SYM 539
#define SQL_BIG_RESULT 540
#define SQL_BUFFER_RESULT 541
#define SQL_CALC_FOUND_ROWS 542
#define SQL_SMALL_RESULT 543
#define SQL_SYM 544
#define STARTING 545
#define START_SYM 546
#define STATUS_SYM 547
#define STDDEV_SAMP_SYM 548
#define STD_SYM 549
#define STOP_SYM 550
#define STORED_SYM 551
#define STRAIGHT_JOIN 552
#define STRING_SYM 553
#define SUBDATE_SYM 554
#define SUBJECT_SYM 555
#define SUBSTRING 556
#define SUM_SYM 557
#define SUSPEND_SYM 558
#define SYSDATE 559
#define TABLES 560
#define TABLESPACE 561
#define TABLE_REF_PRIORITY 562
#define TABLE_SYM 563
#define TEMPORARY_SYM 564
#define TERMINATED 565
#define TEXT_STRING 566
#define TEXT_SYM 567
#define THEN_SYM 568
#define TIME_SYM 569
#define TIMESTAMP_SYM 570
#define TIMESTAMP_ADD 571
#define TIMESTAMP_DIFF 572
#define TO_SYM 573
#define TRAILING 574
#define TRANSACTION_SYM 575
#define TRIM 576
#define TRUE_SYM 577
#define TRUNCATE_SYM 578
#define TYPE_SYM 579
#define ULONGLONG_NUM 580
#define UNCOMMITTED_SYM 581
#define UNDOFILE_SYM 582
#define UNDO_SYM 583
#define UNION_SYM 584
#define UNIQUE_SYM 585
#define UNKNOWN_SYM 586
#define UNLOCK_SYM 587
#define UNSIGNED_SYM 588
#define UPDATE_SYM 589
#define USAGE 590
#define USER 591
#define USE_SYM 592
#define USING 593
#define UTC_DATE_SYM 594
#define UTC_TIMESTAMP_SYM 595
#define UUID_SYM 596
#define VALUES 597
#define VALUE_SYM 598
#define VARBINARY 599
#define VARCHAR_SYM 600
#define VARIABLES 601
#define VARIANCE_SYM 602
#define VARYING 603
#define VAR_SAMP_SYM 604
#define WAIT_SYM 605
#define WARNINGS 606
#define WEEK_SYM 607
#define WHEN_SYM 608
#define WHERE 609
#define WITH 610
#define WITH_ROLLUP_SYM 611
#define WORK_SYM 612
#define WRITE_SYM 613
#define XOR 614
#define YEAR_MONTH_SYM 615
#define YEAR_SYM 616
#define ZEROFILL_SYM 617
#define LESS_THAN 618
#define GREATER_THAN 619
#define CONCAT 620
#define SHIFT_RIGHT 621
#define SHIFT_LEFT 622
#define UMINUS 623




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union 
/* Line 214 of yacc.c  */
#line 120 "drizzled/sql_yacc.yy"
ParserType
{

/* Line 214 of yacc.c  */
#line 120 "drizzled/sql_yacc.yy"

  bool boolean;
  int  num;
  unsigned long ulong_num;
  uint64_t ulonglong_number;
  int64_t longlong_number;
  drizzled::LEX_STRING lex_str;
  drizzled::LEX_STRING *lex_str_ptr;
  drizzled::LEX_SYMBOL symbol;
  drizzled::Table_ident *table;
  char *simple_string;
  drizzled::Item *item;
  drizzled::Item_num *item_num;
  drizzled::List<drizzled::Item> *item_list;
  drizzled::List<drizzled::String> *string_list;
  drizzled::String *string;
  drizzled::Key_part_spec *key_part;
  const drizzled::plugin::Function *udf;
  drizzled::TableList *table_list;
  drizzled::enum_field_types field_val;
  drizzled::sys_var_with_base variable;
  drizzled::sql_var_t var_type;
  drizzled::Key::Keytype key_type;
  drizzled::ha_key_alg key_alg;
  drizzled::ha_rkey_function ha_rkey_mode;
  drizzled::enum_tx_isolation tx_isolation;
  drizzled::Cast_target cast_type;
  const drizzled::CHARSET_INFO *charset;
  drizzled::thr_lock_type lock_type;
  drizzled::interval_type interval, interval_time_st;
  drizzled::type::timestamp_t date_time_type;
  drizzled::Select_Lex *select_lex;
  drizzled::chooser_compare_func_creator boolfunc2creator;
  drizzled::st_lex *lex;
  drizzled::index_hint_type index_hint;
  drizzled::enum_filetype filetype;
  drizzled::ha_build_method build_method;
  drizzled::message::Table::ForeignKeyConstraint::ForeignKeyOption m_fk_option;
  drizzled::execute_string_t execute_string;



/* Line 214 of yacc.c  */
#line 998 "drizzled/sql_yacc.cc"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */

/* Line 264 of yacc.c  */
#line 161 "drizzled/sql_yacc.yy"

namespace drizzled
{
bool my_yyoverflow(short **a, union ParserType **b, unsigned long *yystacksize);
}


/* Line 264 of yacc.c  */
#line 1018 "drizzled/sql_yacc.cc"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  279
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   14939

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  386
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  422
/* YYNRULES -- Number of rules.  */
#define YYNRULES  1211
/* YYNRULES -- Number of states.  */
#define YYNSTATES  2091

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   623

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   375,   368,     2,
     379,   380,   373,   372,   383,   371,   384,   374,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   363,     2,     2,   385,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,   376,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   381,   367,   382,   377,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   364,   365,
     366,   369,   370,   378
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    16,    18,
      20,    22,    24,    26,    28,    30,    32,    34,    36,    38,
      40,    42,    44,    46,    48,    50,    52,    54,    56,    58,
      60,    62,    64,    68,    69,    77,    78,    92,    93,   100,
     106,   107,   113,   118,   121,   124,   125,   126,   131,   132,
     139,   142,   144,   145,   146,   153,   154,   156,   158,   160,
     164,   166,   170,   174,   178,   182,   183,   185,   186,   190,
     191,   193,   195,   198,   200,   203,   207,   209,   213,   217,
     221,   225,   229,   233,   237,   241,   245,   247,   252,   257,
     259,   261,   263,   265,   267,   269,   271,   273,   276,   278,
     282,   284,   286,   289,   292,   300,   309,   318,   321,   324,
     325,   327,   330,   331,   333,   336,   337,   342,   345,   351,
     354,   357,   360,   366,   369,   370,   377,   383,   386,   389,
     392,   395,   399,   403,   407,   410,   413,   415,   418,   420,
     422,   427,   432,   435,   438,   442,   443,   447,   449,   455,
     456,   460,   461,   463,   465,   466,   470,   471,   473,   474,
     478,   480,   482,   485,   488,   490,   493,   494,   497,   501,
     504,   505,   508,   512,   515,   516,   518,   521,   523,   525,
     528,   531,   533,   534,   536,   539,   541,   543,   545,   549,
     552,   554,   555,   557,   560,   562,   564,   568,   571,   576,
     578,   579,   582,   584,   586,   588,   589,   591,   592,   594,
     600,   601,   605,   609,   611,   612,   615,   618,   621,   622,
     626,   630,   637,   644,   646,   648,   651,   654,   657,   659,
     662,   665,   667,   669,   670,   672,   674,   676,   678,   679,
     681,   682,   684,   687,   688,   690,   692,   695,   698,   700,
     704,   707,   709,   711,   716,   719,   721,   726,   727,   729,
     730,   733,   735,   739,   740,   748,   749,   755,   756,   759,
     762,   764,   765,   767,   769,   771,   775,   778,   782,   785,
     790,   791,   798,   799,   800,   809,   813,   818,   822,   826,
     829,   832,   839,   845,   849,   853,   855,   857,   859,   860,
     862,   863,   865,   866,   869,   871,   872,   874,   876,   880,
     881,   885,   886,   891,   892,   897,   898,   903,   905,   909,
     913,   915,   918,   923,   926,   930,   933,   937,   938,   942,
     943,   944,   951,   954,   956,   958,   961,   964,   972,   973,
     975,   978,   980,   982,   984,   986,   988,   990,   992,   994,
     996,   998,   999,  1002,  1007,  1011,  1013,  1015,  1019,  1024,
    1025,  1026,  1027,  1030,  1033,  1035,  1037,  1038,  1041,  1045,
    1049,  1053,  1056,  1060,  1065,  1069,  1074,  1078,  1083,  1085,
    1089,  1094,  1098,  1102,  1109,  1111,  1117,  1124,  1130,  1138,
    1145,  1154,  1160,  1167,  1172,  1178,  1182,  1187,  1189,  1193,
    1197,  1201,  1205,  1209,  1213,  1219,  1225,  1229,  1233,  1237,
    1241,  1245,  1249,  1251,  1253,  1255,  1257,  1259,  1261,  1263,
    1265,  1267,  1269,  1271,  1273,  1275,  1277,  1281,  1283,  1285,
    1287,  1291,  1294,  1297,  1300,  1304,  1308,  1314,  1321,  1326,
    1331,  1334,  1341,  1347,  1354,  1359,  1364,  1370,  1375,  1378,
    1383,  1388,  1393,  1404,  1411,  1420,  1427,  1432,  1437,  1444,
    1449,  1454,  1459,  1467,  1475,  1483,  1490,  1497,  1504,  1511,
    1515,  1520,  1527,  1536,  1539,  1548,  1557,  1564,  1567,  1572,
    1579,  1586,  1595,  1604,  1611,  1620,  1627,  1630,  1635,  1644,
    1653,  1656,  1659,  1664,  1669,  1673,  1677,  1683,  1692,  1698,
    1703,  1710,  1715,  1722,  1731,  1738,  1743,  1747,  1754,  1755,
    1761,  1762,  1764,  1766,  1770,  1775,  1780,  1786,  1792,  1797,
    1798,  1799,  1807,  1812,  1818,  1823,  1829,  1834,  1839,  1844,
    1849,  1854,  1860,  1861,  1870,  1871,  1875,  1879,  1881,  1886,
    1887,  1889,  1890,  1893,  1894,  1896,  1897,  1901,  1904,  1906,
    1908,  1911,  1913,  1915,  1918,  1921,  1923,  1925,  1927,  1930,
    1932,  1936,  1937,  1939,  1940,  1943,  1948,  1954,  1956,  1958,
    1960,  1962,  1967,  1969,  1973,  1977,  1981,  1982,  1989,  1990,
    1997,  1998,  2007,  2012,  2013,  2022,  2023,  2034,  2041,  2042,
    2051,  2052,  2063,  2070,  2072,  2075,  2078,  2079,  2084,  2088,
    2094,  2098,  2099,  2100,  2109,  2111,  2112,  2113,  2120,  2121,
    2125,  2126,  2127,  2133,  2134,  2136,  2137,  2139,  2140,  2143,
    2147,  2151,  2153,  2155,  2156,  2164,  2165,  2173,  2175,  2178,
    2179,  2180,  2183,  2184,  2187,  2188,  2190,  2192,  2194,  2196,
    2200,  2202,  2206,  2208,  2210,  2212,  2214,  2216,  2218,  2220,
    2222,  2224,  2226,  2228,  2230,  2232,  2234,  2236,  2238,  2240,
    2242,  2244,  2246,  2248,  2250,  2252,  2253,  2255,  2256,  2259,
    2260,  2262,  2263,  2264,  2268,  2269,  2270,  2274,  2277,  2278,
    2279,  2284,  2289,  2292,  2293,  2295,  2299,  2303,  2305,  2308,
    2309,  2311,  2312,  2317,  2322,  2325,  2326,  2328,  2330,  2331,
    2333,  2334,  2336,  2339,  2341,  2345,  2349,  2351,  2353,  2355,
    2356,  2359,  2361,  2363,  2365,  2367,  2369,  2371,  2373,  2375,
    2377,  2379,  2381,  2382,  2385,  2389,  2391,  2394,  2395,  2399,
    2400,  2406,  2409,  2411,  2415,  2421,  2422,  2430,  2435,  2437,
    2441,  2443,  2444,  2447,  2448,  2450,  2456,  2458,  2461,  2462,
    2466,  2467,  2469,  2470,  2472,  2473,  2474,  2482,  2483,  2484,
    2490,  2493,  2495,  2497,  2499,  2503,  2508,  2509,  2513,  2517,
    2519,  2522,  2525,  2526,  2530,  2531,  2537,  2541,  2543,  2547,
    2549,  2553,  2555,  2557,  2558,  2560,  2561,  2566,  2567,  2569,
    2573,  2575,  2577,  2579,  2580,  2581,  2588,  2589,  2599,  2603,
    2605,  2609,  2613,  2615,  2619,  2620,  2629,  2630,  2632,  2636,
    2637,  2639,  2640,  2644,  2647,  2651,  2655,  2660,  2666,  2672,
    2678,  2684,  2687,  2690,  2694,  2697,  2701,  2703,  2707,  2712,
    2713,  2716,  2718,  2720,  2721,  2724,  2727,  2728,  2733,  2734,
    2739,  2741,  2743,  2744,  2746,  2747,  2749,  2751,  2752,  2756,
    2760,  2762,  2763,  2767,  2772,  2774,  2776,  2779,  2780,  2782,
    2786,  2787,  2789,  2791,  2794,  2795,  2796,  2797,  2798,  2817,
    2819,  2820,  2822,  2823,  2825,  2827,  2828,  2830,  2832,  2834,
    2837,  2840,  2841,  2844,  2847,  2849,  2853,  2858,  2862,  2866,
    2867,  2870,  2873,  2875,  2879,  2883,  2884,  2888,  2890,  2892,
    2893,  2897,  2900,  2904,  2906,  2908,  2911,  2912,  2915,  2917,
    2920,  2922,  2924,  2926,  2928,  2931,  2934,  2936,  2938,  2940,
    2942,  2944,  2946,  2948,  2951,  2954,  2956,  2958,  2960,  2962,
    2964,  2967,  2970,  2972,  2974,  2976,  2978,  2980,  2982,  2985,
    2988,  2990,  2992,  2994,  2996,  2998,  3000,  3002,  3006,  3012,
    3014,  3016,  3018,  3022,  3027,  3033,  3035,  3041,  3045,  3048,
    3050,  3054,  3057,  3059,  3061,  3063,  3065,  3067,  3069,  3071,
    3073,  3075,  3077,  3079,  3081,  3083,  3085,  3087,  3089,  3091,
    3093,  3095,  3097,  3099,  3101,  3103,  3105,  3107,  3109,  3111,
    3113,  3115,  3117,  3119,  3121,  3123,  3125,  3127,  3129,  3131,
    3133,  3135,  3137,  3139,  3141,  3143,  3145,  3147,  3149,  3151,
    3153,  3155,  3157,  3159,  3161,  3163,  3165,  3167,  3169,  3171,
    3173,  3175,  3177,  3179,  3181,  3183,  3185,  3187,  3189,  3191,
    3193,  3195,  3197,  3199,  3201,  3203,  3205,  3207,  3209,  3211,
    3213,  3215,  3217,  3219,  3221,  3223,  3225,  3227,  3229,  3231,
    3233,  3235,  3237,  3239,  3241,  3243,  3245,  3247,  3249,  3251,
    3253,  3255,  3257,  3259,  3261,  3263,  3265,  3267,  3269,  3271,
    3273,  3275,  3277,  3279,  3281,  3283,  3285,  3287,  3289,  3291,
    3293,  3295,  3297,  3299,  3301,  3303,  3305,  3307,  3309,  3311,
    3313,  3315,  3317,  3319,  3321,  3323,  3325,  3327,  3329,  3331,
    3333,  3335,  3337,  3339,  3341,  3343,  3345,  3347,  3349,  3351,
    3353,  3355,  3357,  3359,  3361,  3363,  3365,  3367,  3369,  3371,
    3373,  3375,  3377,  3379,  3381,  3383,  3384,  3389,  3390,  3392,
    3394,  3398,  3399,  3402,  3404,  3406,  3408,  3410,  3411,  3413,
    3414,  3416,  3418,  3420,  3421,  3424,  3427,  3430,  3432,  3435,
    3440,  3446,  3451,  3458,  3460,  3462,  3464,  3466,  3468,  3470,
    3473,  3476,  3479,  3481,  3483,  3485,  3487,  3489,  3491,  3493,
    3495,  3496,  3500,  3501,  3505,  3506,  3508,  3509,  3513,  3516,
    3517,  3519,  3522,  3523,  3525,  3530,  3535,  3541,  3544,  3548,
    3550,  3551,  3553,  3554,  3559,  3560,  3562,  3564,  3565,  3568,
    3571,  3573,  3574,  3576,  3578,  3581,  3585,  3587,  3588,  3594,
    3598,  3599
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     387,     0,    -1,   106,    -1,   388,   106,    -1,   389,    -1,
     783,    -1,   483,    -1,   500,    -1,   502,    -1,   789,    -1,
     390,    -1,   688,    -1,   699,    -1,   645,    -1,   651,    -1,
     705,    -1,   656,    -1,   711,    -1,   714,    -1,   792,    -1,
     504,    -1,   659,    -1,   790,    -1,   791,    -1,   508,    -1,
     762,    -1,   693,    -1,   498,    -1,   691,    -1,   781,    -1,
     682,    -1,   713,    -1,    63,    38,   751,    -1,    -1,    63,
     407,   308,   408,   749,   391,   394,    -1,    -1,    63,   487,
     392,   470,   150,   756,   472,   215,   749,   379,   478,   380,
     473,    -1,    -1,    63,    69,   408,   750,   393,   404,    -1,
     379,   419,   380,   409,   396,    -1,    -1,   379,   400,   380,
     395,   797,    -1,   379,   399,   380,   409,    -1,   399,   409,
      -1,   409,   396,    -1,    -1,    -1,   722,   400,   397,   794,
      -1,    -1,   722,   379,   400,   380,   398,   797,    -1,   174,
     749,    -1,   401,    -1,    -1,    -1,   267,   402,   519,   525,
     403,   418,    -1,    -1,   415,    -1,   405,    -1,   406,    -1,
     406,   383,   405,    -1,   757,    -1,   250,   674,   322,    -1,
     250,   674,   118,    -1,   757,   673,   757,    -1,   757,   673,
     636,    -1,    -1,   309,    -1,    -1,   146,   207,   115,    -1,
      -1,   411,    -1,   412,    -1,   412,   410,    -1,   412,    -1,
     412,   411,    -1,   412,   383,   411,    -1,   413,    -1,   107,
     673,   757,    -1,    50,   674,   753,    -1,    18,   674,   636,
      -1,   250,   674,   322,    -1,   250,   674,   118,    -1,   261,
     673,   417,    -1,   119,   673,   753,    -1,   757,   673,   758,
      -1,   757,   673,   636,    -1,   414,    -1,   457,    46,   674,
     456,    -1,   457,    46,   674,   456,    -1,    53,    -1,    54,
      -1,    85,    -1,    99,    -1,   121,    -1,   242,    -1,   416,
      -1,   630,    -1,   518,   524,    -1,   420,    -1,   419,   383,
     420,    -1,   421,    -1,   422,    -1,   427,   423,    -1,   427,
     459,    -1,   465,   480,   472,   379,   478,   380,   473,    -1,
     425,   466,   480,   472,   379,   478,   380,   473,    -1,   425,
     125,   166,   480,   379,   478,   380,   459,    -1,   426,   423,
      -1,   425,   424,    -1,    -1,   424,    -1,    43,   531,    -1,
      -1,   426,    -1,    59,   480,    -1,    -1,   748,   428,   429,
     454,    -1,   315,   451,    -1,   315,   379,   211,   380,   451,
      -1,    72,   451,    -1,   432,   448,    -1,    41,   445,    -1,
     431,   379,   211,   380,   445,    -1,   312,   445,    -1,    -1,
     108,   430,   379,   482,   380,   445,    -1,   344,   379,   211,
     380,   443,    -1,   433,   448,    -1,    75,   443,    -1,   314,
     443,    -1,    27,   443,    -1,    83,   434,   448,    -1,   212,
     434,   448,    -1,   121,   434,   448,    -1,   341,   443,    -1,
      28,   444,    -1,   271,    -1,    41,   348,    -1,    41,    -1,
     345,    -1,   158,   438,   437,   439,    -1,    23,   438,   437,
     439,    -1,   241,   440,    -1,    95,   440,    -1,    95,   230,
     440,    -1,    -1,   379,   211,   380,    -1,   435,    -1,   379,
     211,   383,   211,   380,    -1,    -1,   379,   211,   380,    -1,
      -1,   278,    -1,   333,    -1,    -1,   379,   211,   380,    -1,
      -1,   362,    -1,    -1,   379,   211,   380,    -1,   435,    -1,
     210,    -1,   207,   210,    -1,   458,   166,    -1,   330,    -1,
     330,   166,    -1,    -1,   443,   441,    -1,   443,    85,   737,
      -1,   443,   442,    -1,    -1,   444,   441,    -1,   444,    85,
     740,    -1,   444,   442,    -1,    -1,   446,    -1,   446,   447,
      -1,   447,    -1,   441,    -1,    85,   737,    -1,    46,   455,
      -1,   442,    -1,    -1,   449,    -1,   449,   450,    -1,   450,
      -1,   441,    -1,    18,    -1,   271,    85,   343,    -1,    85,
     739,    -1,   442,    -1,    -1,   452,    -1,   452,   453,    -1,
     453,    -1,   441,    -1,    85,   208,   530,    -1,    85,   741,
      -1,   215,   334,   208,   530,    -1,   442,    -1,    -1,    50,
     753,    -1,   757,    -1,   455,    -1,    85,    -1,    -1,    85,
      -1,    -1,   232,    -1,   244,   749,   460,   462,   463,    -1,
      -1,   379,   461,   380,    -1,   461,   383,   756,    -1,   756,
      -1,    -1,   184,   130,    -1,   184,   228,    -1,   184,   279,
      -1,    -1,   215,   334,   464,    -1,   215,    86,   464,    -1,
     215,   334,   464,   215,    86,   464,    -1,   215,    86,   464,
     215,   334,   464,    -1,   252,    -1,    34,    -1,   274,   210,
      -1,   209,     4,    -1,   274,    85,    -1,   467,    -1,   232,
     166,    -1,   330,   468,    -1,   166,    -1,   150,    -1,    -1,
     467,    -1,   164,    -1,   150,    -1,   149,    -1,    -1,   330,
      -1,    -1,   471,    -1,   471,   475,    -1,    -1,   474,    -1,
     476,    -1,   474,   476,    -1,   338,   477,    -1,   475,    -1,
     165,   674,   635,    -1,    50,   753,    -1,    31,    -1,   135,
      -1,   478,   383,   479,   628,    -1,   479,   628,    -1,   756,
      -1,   756,   379,   211,   380,    -1,    -1,   748,    -1,    -1,
     384,   756,    -1,   736,    -1,   482,   383,   736,    -1,    -1,
      10,   487,   495,   308,   749,   484,   486,    -1,    -1,    10,
      69,   750,   485,   415,    -1,    -1,    91,   306,    -1,   148,
     306,    -1,   488,    -1,    -1,   218,    -1,   213,    -1,   490,
      -1,   488,   383,   490,    -1,     5,   494,    -1,   489,   421,
     496,    -1,     5,   422,    -1,   489,   379,   419,   380,    -1,
      -1,    40,   494,   748,   491,   427,   496,    -1,    -1,    -1,
     196,   494,   748,   492,   429,   454,   493,   496,    -1,    96,
     494,   748,    -1,    96,   125,   166,   480,    -1,    96,   232,
     166,    -1,    96,   467,   748,    -1,    90,   164,    -1,   102,
     164,    -1,    10,   494,   748,   274,    85,   737,    -1,    10,
     494,   748,    96,    85,    -1,   246,   497,   749,    -1,    61,
     318,   456,    -1,   410,    -1,   124,    -1,   621,    -1,    -1,
      49,    -1,    -1,   147,    -1,    -1,     7,   756,    -1,   120,
      -1,    -1,   318,    -1,    14,    -1,   291,   320,   499,    -1,
      -1,   355,    58,   280,    -1,    -1,    11,   780,   501,   647,
      -1,    -1,    43,   780,   503,   647,    -1,    -1,   246,   780,
     505,   506,    -1,   507,    -1,   506,   383,   507,    -1,   749,
     318,   749,    -1,   509,    -1,   267,   512,    -1,   379,   510,
     380,   797,    -1,   267,   514,    -1,   379,   510,   380,    -1,
     267,   582,    -1,   379,   511,   380,    -1,    -1,   514,   513,
     794,    -1,    -1,    -1,   515,   519,   525,   516,   517,   524,
      -1,   624,   630,    -1,   641,    -1,   518,    -1,   641,   518,
      -1,   518,   641,    -1,   129,   564,   613,   618,   615,   624,
     630,    -1,    -1,   520,    -1,   520,   523,    -1,   523,    -1,
      92,    -1,     9,    -1,   288,    -1,   285,    -1,   297,    -1,
     286,    -1,   522,    -1,   521,    -1,   287,    -1,    -1,   126,
     334,    -1,   180,   159,   276,   194,    -1,   525,   383,   526,
      -1,   526,    -1,   373,    -1,   527,   744,   528,    -1,   527,
     531,   528,   529,    -1,    -1,    -1,    -1,    14,   756,    -1,
      14,   753,    -1,   756,    -1,   753,    -1,    -1,   379,   380,
      -1,   531,   224,   531,    -1,   531,   359,   531,    -1,   531,
      12,   531,    -1,   207,   531,    -1,   532,   160,   322,    -1,
     532,   160,   207,   322,    -1,   532,   160,   118,    -1,   532,
     160,   207,   118,    -1,   532,   160,   331,    -1,   532,   160,
     207,   331,    -1,   532,    -1,   532,   160,   210,    -1,   532,
     160,   207,   210,    -1,   532,   109,   533,    -1,   532,   535,
     533,    -1,   532,   535,   536,   379,   805,   380,    -1,   533,
      -1,   534,   159,   379,   805,   380,    -1,   534,   207,   159,
     379,   805,   380,    -1,   534,   159,   379,   531,   380,    -1,
     534,   159,   379,   531,   383,   559,   380,    -1,   534,   207,
     159,   379,   531,   380,    -1,   534,   207,   159,   379,   531,
     383,   559,   380,    -1,   534,    22,   534,    12,   533,    -1,
     534,   207,    22,   534,    12,   533,    -1,   534,   174,   537,
     617,    -1,   534,   207,   174,   537,   617,    -1,   534,   243,
     534,    -1,   534,   207,   243,   534,    -1,   534,    -1,   534,
     367,   534,    -1,   534,   368,   534,    -1,   534,   369,   534,
      -1,   534,   370,   534,    -1,   534,   372,   534,    -1,   534,
     371,   534,    -1,   534,   372,   156,   531,   607,    -1,   534,
     371,   156,   531,   607,    -1,   534,   373,   534,    -1,   534,
     374,   534,    -1,   534,   375,   534,    -1,   534,    93,   534,
      -1,   534,   197,   534,    -1,   534,   376,   534,    -1,   537,
      -1,   363,    -1,   131,    -1,   365,    -1,   169,    -1,   364,
      -1,   203,    -1,     9,    -1,    13,    -1,   746,    -1,   538,
      -1,   539,    -1,   541,    -1,   540,    -1,   537,    46,   757,
      -1,   738,    -1,   550,    -1,   546,    -1,   537,   366,   537,
      -1,   372,   537,    -1,   371,   537,    -1,   377,   537,    -1,
     379,   805,   380,    -1,   379,   531,   380,    -1,   379,   531,
     383,   559,   380,    -1,   262,   379,   531,   383,   559,   380,
      -1,   115,   379,   805,   380,    -1,   381,   756,   531,   382,
      -1,    24,   537,    -1,    37,   379,   531,    14,   558,   380,
      -1,    36,   560,   562,   561,   104,    -1,    61,   379,   531,
     383,   558,   380,    -1,    85,   379,   746,   380,    -1,   342,
     379,   746,   380,    -1,   156,   531,   607,   372,   531,    -1,
      41,   379,   559,   380,    -1,    67,   530,    -1,    75,   379,
     531,   380,    -1,    80,   379,   531,   380,    -1,   141,   379,
     531,   380,    -1,   155,   379,   531,   383,   531,   383,   531,
     383,   531,   380,    -1,   156,   379,   531,   383,   531,   380,
      -1,   156,   379,   531,   383,   531,   383,   559,   380,    -1,
     171,   379,   531,   383,   531,   380,    -1,   192,   379,   531,
     380,    -1,   198,   379,   531,   380,    -1,   256,   379,   531,
     383,   531,   380,    -1,   265,   379,   531,   380,    -1,   315,
     379,   531,   380,    -1,   321,   379,   531,   380,    -1,   321,
     379,   170,   531,   129,   531,   380,    -1,   321,   379,   319,
     531,   129,   531,   380,    -1,   321,   379,    30,   531,   129,
     531,   380,    -1,   321,   379,   170,   129,   531,   380,    -1,
     321,   379,   319,   129,   531,   380,    -1,   321,   379,    30,
     129,   531,   380,    -1,   321,   379,   531,   129,   531,   380,
      -1,   336,   379,   380,    -1,   361,   379,   531,   380,    -1,
       6,   379,   531,   383,   531,   380,    -1,     6,   379,   531,
     383,   156,   531,   607,   380,    -1,    66,   530,    -1,    73,
     379,   531,   383,   156,   531,   607,   380,    -1,    74,   379,
     531,   383,   156,   531,   607,   380,    -1,   117,   379,   607,
     129,   531,   380,    -1,   208,   530,    -1,   208,   379,   531,
     380,    -1,   229,   379,   534,   159,   531,   380,    -1,   299,
     379,   531,   383,   531,   380,    -1,   299,   379,   531,   383,
     156,   531,   607,   380,    -1,   301,   379,   531,   383,   531,
     383,   531,   380,    -1,   301,   379,   531,   383,   531,   380,
      -1,   301,   379,   531,   129,   531,   126,   531,   380,    -1,
     301,   379,   531,   129,   531,   380,    -1,   304,   530,    -1,
     304,   379,   531,   380,    -1,   316,   379,   608,   383,   531,
     383,   531,   380,    -1,   317,   379,   608,   383,   531,   383,
     531,   380,    -1,   339,   530,    -1,   340,   530,    -1,    45,
     379,   559,   380,    -1,    47,   379,   531,   380,    -1,    69,
     379,   380,    -1,    38,   379,   380,    -1,   114,   379,   531,
     380,   655,    -1,   146,   379,   531,   383,   531,   383,   531,
     380,    -1,   167,   712,   379,   531,   380,    -1,   189,   379,
     531,   380,    -1,   197,   379,   531,   383,   531,   380,    -1,
     235,   379,   531,   380,    -1,   248,   379,   531,   383,   531,
     380,    -1,   249,   379,   531,   383,   531,   383,   531,   380,
      -1,   323,   379,   531,   383,   531,   380,    -1,   350,   379,
     531,   380,    -1,   341,   379,   380,    -1,   350,   379,   531,
     383,   531,   380,    -1,    -1,   752,   379,   542,   543,   380,
      -1,    -1,   544,    -1,   545,    -1,   544,   383,   545,    -1,
     527,   531,   528,   529,    -1,    19,   379,   556,   380,    -1,
      19,   379,    92,   556,   380,    -1,    62,   379,   612,   373,
     380,    -1,    62,   379,   556,   380,    -1,    -1,    -1,    62,
     379,    92,   547,   559,   548,   380,    -1,   193,   379,   556,
     380,    -1,   193,   379,    92,   556,   380,    -1,   185,   379,
     556,   380,    -1,   185,   379,    92,   556,   380,    -1,   294,
     379,   556,   380,    -1,   347,   379,   556,   380,    -1,   293,
     379,   556,   380,    -1,   349,   379,   556,   380,    -1,   302,
     379,   556,   380,    -1,   302,   379,    92,   556,   380,    -1,
      -1,   134,   379,   553,   549,   559,   555,   554,   380,    -1,
      -1,   385,   551,   552,    -1,   775,   275,   531,    -1,   775,
      -1,   385,   771,   775,   481,    -1,    -1,    92,    -1,    -1,
     269,   736,    -1,    -1,   625,    -1,    -1,   612,   557,   531,
      -1,    24,   436,    -1,    28,    -1,   278,    -1,   278,   158,
      -1,   158,    -1,   333,    -1,   333,   158,    -1,    41,   436,
      -1,    75,    -1,   314,    -1,    72,    -1,    83,   434,    -1,
     531,    -1,   559,   383,   531,    -1,    -1,   531,    -1,    -1,
     101,   531,    -1,   353,   531,   313,   531,    -1,   562,   353,
     531,   313,   531,    -1,   576,    -1,   567,    -1,   566,    -1,
     563,    -1,   381,   756,   563,   382,    -1,   565,    -1,   566,
     383,   565,    -1,   563,   575,   563,    -1,   563,   297,   576,
      -1,    -1,   563,   575,   563,   215,   568,   531,    -1,    -1,
     563,   297,   576,   215,   569,   531,    -1,    -1,   563,   575,
     563,   338,   570,   379,   606,   380,    -1,   563,   202,   163,
     576,    -1,    -1,   563,   171,   592,   163,   563,   215,   571,
     531,    -1,    -1,   563,   171,   592,   163,   576,   572,   338,
     379,   606,   380,    -1,   563,   202,   171,   592,   163,   576,
      -1,    -1,   563,   256,   592,   163,   563,   215,   573,   531,
      -1,    -1,   563,   256,   592,   163,   576,   574,   338,   379,
     606,   380,    -1,   563,   202,   256,   592,   163,   576,    -1,
     163,    -1,   152,   163,    -1,    64,   163,    -1,    -1,   577,
     749,   611,   601,    -1,   591,   590,   587,    -1,   379,   590,
     578,   380,   611,    -1,   585,   624,   630,    -1,    -1,    -1,
     578,   329,   801,   579,   802,   580,   624,   630,    -1,   582,
      -1,    -1,    -1,   583,   519,   525,   584,   418,   524,    -1,
      -1,   590,   586,   566,    -1,    -1,    -1,   588,   519,   525,
     589,   418,    -1,    -1,   267,    -1,    -1,   225,    -1,    -1,
     126,   163,    -1,   126,   223,    32,    -1,   126,   133,    32,
      -1,   124,    -1,   147,    -1,    -1,   594,   467,   593,   596,
     379,   605,   380,    -1,    -1,   337,   467,   593,   597,   379,
     603,   380,    -1,   595,    -1,   598,   595,    -1,    -1,    -1,
     600,   598,    -1,    -1,   602,   599,    -1,    -1,   605,    -1,
     756,    -1,   232,    -1,   604,    -1,   605,   383,   604,    -1,
     756,    -1,   606,   383,   756,    -1,   609,    -1,    76,    -1,
      77,    -1,    78,    -1,    79,    -1,   138,    -1,   139,    -1,
     140,    -1,   190,    -1,   191,    -1,   264,    -1,   360,    -1,
     609,    -1,   128,    -1,    80,    -1,   352,    -1,   141,    -1,
     192,    -1,   198,    -1,   235,    -1,   265,    -1,   189,    -1,
     361,    -1,    -1,    14,    -1,    -1,   610,   756,    -1,    -1,
       9,    -1,    -1,    -1,   354,   614,   531,    -1,    -1,    -1,
     136,   616,   531,    -1,   112,   537,    -1,    -1,    -1,   133,
      32,   619,   620,    -1,   619,   383,   745,   628,    -1,   745,
     628,    -1,    -1,   356,    -1,   223,    32,   622,    -1,   622,
     383,   623,    -1,   623,    -1,   746,   628,    -1,    -1,   625,
      -1,    -1,   223,    32,   626,   627,    -1,   627,   383,   745,
     628,    -1,   745,   628,    -1,    -1,    15,    -1,    87,    -1,
      -1,   631,    -1,    -1,   631,    -1,   175,   632,    -1,   633,
      -1,   633,   383,   633,    -1,   633,   214,   633,    -1,   325,
      -1,   182,    -1,   211,    -1,    -1,   175,   633,    -1,   211,
      -1,   137,    -1,   182,    -1,   325,    -1,    82,    -1,   122,
      -1,   211,    -1,   325,    -1,   182,    -1,    82,    -1,   122,
      -1,    -1,   638,   639,    -1,   639,   383,   640,    -1,   640,
      -1,   385,   775,    -1,    -1,   157,   642,   643,    -1,    -1,
     226,   755,   644,   723,   726,    -1,    97,   755,    -1,   637,
      -1,    96,    38,   751,    -1,    96,   650,   780,   649,   647,
      -1,    -1,    96,   487,   150,   756,   215,   749,   646,    -1,
      96,    69,   649,   750,    -1,   648,    -1,   647,   383,   648,
      -1,   749,    -1,    -1,   146,   115,    -1,    -1,   309,    -1,
     114,   652,   653,   654,   655,    -1,   775,    -1,   385,   775,
      -1,    -1,   355,   209,   254,    -1,    -1,    55,    -1,    -1,
     350,    -1,    -1,    -1,   155,   657,   495,   662,   658,   664,
     680,    -1,    -1,    -1,   249,   660,   662,   661,   664,    -1,
     157,   663,    -1,   663,    -1,   648,    -1,   667,    -1,   379,
     380,   667,    -1,   379,   666,   380,   667,    -1,    -1,   274,
     665,   671,    -1,   666,   383,   743,    -1,   743,    -1,   342,
     670,    -1,   343,   670,    -1,    -1,   401,   668,   794,    -1,
      -1,   379,   401,   380,   669,   797,    -1,   670,   383,   675,
      -1,   675,    -1,   671,   383,   672,    -1,   672,    -1,   746,
     673,   679,    -1,   363,    -1,   275,    -1,    -1,   673,    -1,
      -1,   379,   676,   677,   380,    -1,    -1,   678,    -1,   678,
     383,   679,    -1,   679,    -1,   531,    -1,    85,    -1,    -1,
      -1,   215,    98,   681,   166,   334,   686,    -1,    -1,   334,
     495,   749,   274,   684,   683,   613,   624,   634,    -1,   684,
     383,   685,    -1,   685,    -1,   746,   673,   679,    -1,   686,
     383,   687,    -1,   687,    -1,   746,   673,   679,    -1,    -1,
      86,   690,   129,   749,   689,   613,   624,   634,    -1,    -1,
     147,    -1,   323,   692,   648,    -1,    -1,   308,    -1,    -1,
     277,   694,   695,    -1,    70,   698,    -1,   305,   696,   698,
      -1,   309,   305,   698,    -1,   308,   292,   696,   698,    -1,
      48,   697,   749,   696,   698,    -1,   469,   697,   749,   696,
     613,    -1,    62,   379,   373,   380,   351,    -1,    62,   379,
     373,   380,   110,    -1,   351,   629,    -1,   110,   629,    -1,
     770,   292,   698,    -1,   758,   292,    -1,    63,   308,   749,
      -1,   234,    -1,   770,   346,   698,    -1,    63,    69,   408,
     756,    -1,    -1,   697,   756,    -1,   129,    -1,   159,    -1,
      -1,   174,   753,    -1,   354,   531,    -1,    -1,   702,   749,
     700,   704,    -1,    -1,   702,   703,   701,   508,    -1,    87,
      -1,    88,    -1,    -1,   116,    -1,    -1,   736,    -1,   756,
      -1,    -1,   123,   706,   707,    -1,   707,   383,   708,    -1,
     708,    -1,    -1,   780,   709,   710,    -1,   305,   355,   239,
     180,    -1,   181,    -1,   292,    -1,   132,   292,    -1,    -1,
     647,    -1,   167,   712,   531,    -1,    -1,    57,    -1,   236,
      -1,   337,   750,    -1,    -1,    -1,    -1,    -1,   177,   719,
     715,   720,   151,   755,   716,   721,   157,   717,   308,   749,
     718,   723,   726,   729,   731,   734,    -1,    71,    -1,    -1,
      55,    -1,    -1,   249,    -1,   147,    -1,    -1,    14,    -1,
     249,    -1,   147,    -1,   249,    14,    -1,   147,    14,    -1,
      -1,    48,   724,    -1,   724,   725,    -1,   725,    -1,   310,
      32,   736,    -1,   222,   103,    32,   736,    -1,   103,    32,
     736,    -1,   111,    32,   736,    -1,    -1,   176,   727,    -1,
     727,   728,    -1,   728,    -1,   310,    32,   736,    -1,   290,
      32,   736,    -1,    -1,   147,   211,   730,    -1,   176,    -1,
     260,    -1,    -1,   379,   732,   380,    -1,   379,   380,    -1,
     732,   383,   733,    -1,   733,    -1,   746,    -1,   385,   775,
      -1,    -1,   274,   686,    -1,   754,    -1,   735,   754,    -1,
     754,    -1,   137,    -1,    25,    -1,   738,    -1,   372,   742,
      -1,   371,   742,    -1,   735,    -1,   742,    -1,   210,    -1,
     118,    -1,   322,    -1,   137,    -1,    25,    -1,    75,   735,
      -1,   315,   735,    -1,   735,    -1,   137,    -1,    25,    -1,
     742,    -1,   210,    -1,   372,   742,    -1,   371,   742,    -1,
     210,    -1,   118,    -1,   322,    -1,   735,    -1,   210,    -1,
     742,    -1,    75,   735,    -1,   315,   735,    -1,   211,    -1,
     182,    -1,   325,    -1,    82,    -1,   122,    -1,   746,    -1,
     744,    -1,   756,   384,   373,    -1,   756,   384,   756,   384,
     373,    -1,   531,    -1,   756,    -1,   747,    -1,   756,   384,
     756,    -1,   384,   756,   384,   756,    -1,   756,   384,   756,
     384,   756,    -1,   756,    -1,   756,   384,   756,   384,   756,
      -1,   756,   384,   756,    -1,   384,   756,    -1,   756,    -1,
     750,   384,   756,    -1,   384,   756,    -1,   756,    -1,   756,
      -1,   142,    -1,   145,    -1,   311,    -1,   311,    -1,   311,
      -1,   752,    -1,   760,    -1,   752,    -1,   753,    -1,   752,
      -1,   753,    -1,   315,    -1,   286,    -1,   144,    -1,   761,
      -1,    21,    -1,    42,    -1,    44,    -1,    50,    -1,    52,
      -1,    60,    -1,    81,    -1,    94,    -1,   104,    -1,   123,
      -1,   209,    -1,   219,    -1,   257,    -1,   263,    -1,   266,
      -1,   273,    -1,   278,    -1,   291,    -1,   295,    -1,   323,
      -1,     4,    -1,     6,    -1,     7,    -1,     8,    -1,    13,
      -1,    17,    -1,    18,    -1,    19,    -1,    26,    -1,    29,
      -1,    28,    -1,    31,    -1,    35,    -1,    39,    -1,    45,
      -1,    47,    -1,    48,    -1,    51,    -1,    53,    -1,    54,
      -1,    55,    -1,    57,    -1,    58,    -1,    65,    -1,    71,
      -1,    70,    -1,    72,    -1,    75,    -1,    80,    -1,    90,
      -1,    91,    -1,    97,    -1,    98,    -1,    99,    -1,   105,
      -1,   108,    -1,   107,    -1,   110,    -1,   112,    -1,   113,
      -1,   116,    -1,   127,    -1,   102,    -1,   130,    -1,   119,
      -1,   120,    -1,   121,    -1,   128,    -1,   132,    -1,   135,
      -1,   141,    -1,   143,    -1,   148,    -1,   149,    -1,   161,
      -1,   165,    -1,   168,    -1,   172,    -1,   178,    -1,   179,
      -1,   181,    -1,   186,    -1,   187,    -1,   188,    -1,   189,
      -1,   192,    -1,   196,    -1,   194,    -1,   198,    -1,   200,
      -1,   199,    -1,   201,    -1,   205,    -1,   204,    -1,   206,
      -1,   213,    -1,   214,    -1,   216,    -1,   217,    -1,   218,
      -1,   228,    -1,   231,    -1,   233,    -1,   234,    -1,   235,
      -1,   236,    -1,   242,    -1,   247,    -1,   253,    -1,   258,
      -1,   259,    -1,   260,    -1,   261,    -1,   262,    -1,   265,
      -1,   271,    -1,   270,    -1,   272,    -1,   279,    -1,   276,
      -1,   280,    -1,   292,    -1,   298,    -1,   299,    -1,   300,
      -1,   303,    -1,   305,    -1,   306,    -1,   309,    -1,   312,
      -1,   320,    -1,   314,    -1,   316,    -1,   317,    -1,   324,
      -1,   326,    -1,   327,    -1,   331,    -1,   341,    -1,   336,
      -1,   346,    -1,   343,    -1,   351,    -1,   352,    -1,   357,
      -1,   361,    -1,    -1,   274,   764,   763,   765,    -1,    -1,
     221,    -1,   766,    -1,   765,   383,   766,    -1,    -1,   767,
     772,    -1,   769,    -1,   132,    -1,   178,    -1,   272,    -1,
      -1,   216,    -1,    -1,   132,    -1,   178,    -1,   272,    -1,
      -1,   132,   384,    -1,   178,   384,    -1,   272,   384,    -1,
     773,    -1,   769,   774,    -1,   768,   777,   673,   779,    -1,
     768,   320,   161,   172,   778,    -1,   385,   775,   673,   531,
      -1,   385,   385,   771,   777,   673,   779,    -1,   776,    -1,
     753,    -1,   173,    -1,   759,    -1,   752,    -1,   776,    -1,
     239,   326,    -1,   239,    51,    -1,   247,   239,    -1,   270,
      -1,   531,    -1,    85,    -1,   215,    -1,     9,    -1,    24,
      -1,   308,    -1,   305,    -1,    -1,   332,   782,   780,    -1,
      -1,    21,   784,   785,    -1,    -1,   357,    -1,    -1,    12,
     209,    39,    -1,    12,    39,    -1,    -1,   245,    -1,   209,
     245,    -1,    -1,   263,    -1,    52,   785,   786,   787,    -1,
     257,   785,   786,   787,    -1,   257,   785,   318,   788,   793,
      -1,   263,   793,    -1,   245,   263,   793,    -1,   752,    -1,
      -1,   795,    -1,    -1,   329,   801,   796,   509,    -1,    -1,
     795,    -1,   798,    -1,    -1,   799,   800,    -1,   625,   629,
      -1,   631,    -1,    -1,    92,    -1,     9,    -1,   267,   581,
      -1,   379,   511,   380,    -1,   802,    -1,    -1,   803,   329,
     801,   804,   802,    -1,   806,   803,   807,    -1,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   798,   798,   810,   814,   815,   820,   821,   822,   823,
     824,   825,   826,   827,   828,   829,   830,   831,   832,   833,
     834,   835,   836,   837,   838,   839,   840,   841,   842,   843,
     844,   845,   851,   856,   855,   870,   869,   883,   882,   893,
     896,   895,   900,   902,   904,   909,   911,   910,   916,   915,
     923,   933,   943,   964,   942,   978,   979,   980,   984,   985,
     989,   994,   998,  1002,  1006,  1013,  1014,  1018,  1019,  1022,
    1024,  1028,  1029,  1033,  1034,  1035,  1038,  1041,  1045,  1049,
    1053,  1057,  1061,  1065,  1069,  1073,  1077,  1081,  1091,  1098,
    1099,  1100,  1101,  1102,  1103,  1107,  1115,  1116,  1120,  1121,
    1125,  1126,  1130,  1131,  1138,  1142,  1147,  1151,  1155,  1161,
    1163,  1167,  1171,  1172,  1176,  1181,  1180,  1206,  1210,  1214,
    1221,  1225,  1229,  1233,  1242,  1241,  1252,  1256,  1261,  1268,
    1275,  1279,  1283,  1287,  1291,  1295,  1299,  1306,  1308,  1310,
    1315,  1319,  1326,  1330,  1334,  1342,  1343,  1345,  1350,  1358,
    1359,  1363,  1364,  1365,  1369,  1370,  1374,  1375,  1380,  1381,
    1383,  1388,  1392,  1404,  1408,  1412,  1419,  1420,  1422,  1429,
    1434,  1435,  1437,  1444,  1449,  1450,  1454,  1455,  1459,  1461,
    1468,  1481,  1486,  1487,  1491,  1492,  1496,  1498,  1502,  1506,
    1513,  1518,  1519,  1523,  1524,  1528,  1530,  1537,  1544,  1548,
    1553,  1554,  1565,  1576,  1577,  1581,  1582,  1585,  1587,  1591,
    1599,  1600,  1604,  1606,  1615,  1616,  1618,  1620,  1626,  1630,
    1635,  1640,  1646,  1655,  1656,  1657,  1658,  1659,  1663,  1667,
    1668,  1672,  1673,  1677,  1678,  1682,  1683,  1684,  1688,  1689,
    1693,  1705,  1706,  1710,  1711,  1715,  1716,  1720,  1724,  1725,
    1727,  1732,  1733,  1737,  1738,  1742,  1743,  1755,  1756,  1760,
    1761,  1765,  1766,  1774,  1773,  1790,  1789,  1801,  1803,  1808,
    1813,  1818,  1821,  1825,  1832,  1833,  1837,  1847,  1848,  1854,
    1862,  1861,  1869,  1882,  1868,  1896,  1903,  1907,  1911,  1915,
    1922,  1929,  1936,  1943,  1964,  1974,  1980,  1986,  1995,  1996,
    2000,  2001,  2005,  2006,  2010,  2017,  2018,  2019,  2023,  2030,
    2031,  2039,  2038,  2048,  2047,  2057,  2056,  2065,  2066,  2070,
    2087,  2095,  2096,  2100,  2105,  2110,  2115,  2120,  2119,  2138,
    2145,  2138,  2152,  2153,  2154,  2155,  2156,  2160,  2169,  2171,
    2176,  2177,  2181,  2191,  2204,  2214,  2228,  2229,  2235,  2237,
    2239,  2247,  2249,  2253,  2261,  2262,  2263,  2273,  2278,  2298,
    2305,  2312,  2313,  2314,  2315,  2316,  2320,  2321,  2326,  2375,
    2380,  2421,  2423,  2427,  2431,  2435,  2439,  2443,  2447,  2451,
    2455,  2459,  2463,  2467,  2471,  2475,  2479,  2484,  2488,  2494,
    2498,  2506,  2510,  2516,  2520,  2524,  2534,  2545,  2549,  2553,
    2557,  2561,  2565,  2569,  2573,  2577,  2581,  2585,  2589,  2593,
    2597,  2601,  2605,  2609,  2610,  2611,  2612,  2613,  2614,  2618,
    2619,  2623,  2624,  2625,  2626,  2627,  2628,  2635,  2636,  2637,
    2641,  2645,  2646,  2650,  2654,  2658,  2659,  2664,  2669,  2673,
    2674,  2679,  2686,  2688,  2695,  2700,  2705,  2717,  2719,  2727,
    2729,  2731,  2733,  2735,  2743,  2750,  2752,  2754,  2756,  2758,
    2760,  2762,  2764,  2766,  2768,  2770,  2772,  2774,  2776,  2778,
    2786,  2802,  2807,  2809,  2814,  2816,  2818,  2820,  2825,  2830,
    2832,  2837,  2839,  2850,  2860,  2871,  2881,  2886,  2891,  2893,
    2895,  2900,  2913,  2915,  2917,  2925,  2933,  2948,  2950,  2965,
    2967,  2969,  2971,  2973,  2975,  2977,  2986,  2994,  3016,  3015,
    3063,  3064,  3068,  3073,  3081,  3101,  3103,  3105,  3107,  3110,
    3112,  3109,  3115,  3122,  3124,  3126,  3128,  3130,  3132,  3134,
    3136,  3138,  3141,  3140,  3156,  3155,  3164,  3169,  3174,  3188,
    3189,  3194,  3197,  3202,  3205,  3217,  3216,  3232,  3234,  3236,
    3238,  3240,  3242,  3244,  3246,  3248,  3250,  3252,  3254,  3259,
    3264,  3272,  3273,  3277,  3278,  3282,  3288,  3299,  3300,  3308,
    3319,  3320,  3326,  3327,  3347,  3352,  3358,  3356,  3374,  3372,
    3390,  3388,  3395,  3404,  3402,  3420,  3419,  3429,  3440,  3438,
    3456,  3455,  3465,  3475,  3476,  3477,  3493,  3493,  3504,  3543,
    3584,  3588,  3593,  3585,  3605,  3624,  3631,  3624,  3640,  3639,
    3661,  3675,  3661,  3682,  3686,  3704,  3705,  3710,  3713,  3714,
    3715,  3719,  3720,  3725,  3724,  3730,  3729,  3737,  3738,  3741,
    3743,  3743,  3747,  3747,  3752,  3753,  3757,  3759,  3764,  3765,
    3769,  3777,  3787,  3788,  3789,  3790,  3791,  3792,  3793,  3794,
    3795,  3796,  3797,  3798,  3802,  3803,  3821,  3822,  3823,  3824,
    3825,  3826,  3827,  3828,  3829,  3832,  3834,  3838,  3839,  3845,
    3847,  3851,  3853,  3852,  3866,  3869,  3868,  3883,  3889,  3899,
    3901,  3905,  3907,  3912,  3913,  3937,  3941,  3942,  3946,  3958,
    3960,  3965,  3964,  3973,  3978,  3986,  3987,  3988,  3993,  3998,
    4002,  4003,  4007,  4011,  4018,  4025,  4035,  4036,  4037,  4042,
    4045,  4054,  4055,  4056,  4057,  4058,  4059,  4063,  4064,  4065,
    4066,  4067,  4071,  4071,  4080,  4081,  4085,  4104,  4103,  4110,
    4109,  4117,  4128,  4137,  4141,  4148,  4148,  4159,  4169,  4170,
    4174,  4182,  4183,  4187,  4188,  4196,  4203,  4207,  4213,  4214,
    4218,  4219,  4223,  4224,  4233,  4241,  4232,  4251,  4257,  4250,
    4266,  4267,  4271,  4279,  4280,  4281,  4283,  4282,  4292,  4293,
    4297,  4298,  4300,  4299,  4305,  4304,  4312,  4313,  4317,  4318,
    4322,  4331,  4332,  4336,  4337,  4342,  4341,  4354,  4355,  4359,
    4364,  4372,  4373,  4376,  4378,  4378,  4386,  4385,  4412,  4413,
    4417,  4425,  4426,  4430,  4442,  4441,  4457,  4458,  4462,  4470,
    4472,  4479,  4478,  4490,  4496,  4502,  4508,  4514,  4520,  4525,
    4529,  4533,  4537,  4541,  4546,  4551,  4556,  4561,  4566,  4573,
    4574,  4578,  4579,  4582,  4584,  4591,  4602,  4601,  4610,  4609,
    4618,  4619,  4623,  4624,  4628,  4629,  4630,  4643,  4642,  4651,
    4652,  4657,  4656,  4662,  4667,  4672,  4677,  4685,  4686,  4692,
    4699,  4700,  4701,  4707,  4718,  4726,  4734,  4739,  4717,  4754,
    4757,  4758,  4765,  4766,  4767,  4771,  4772,  4773,  4774,  4775,
    4776,  4779,  4781,  4785,  4786,  4790,  4795,  4801,  4806,  4813,
    4815,  4819,  4820,  4824,  4829,  4836,  4838,  4846,  4847,  4851,
    4852,  4853,  4857,  4859,  4864,  4865,  4870,  4871,  4877,  4881,
    4888,  4894,  4905,  4918,  4919,  4920,  4928,  4929,  4930,  4935,
    4936,  4937,  4938,  4939,  4940,  4944,  4945,  4946,  4947,  4948,
    4953,  4954,  4962,  4967,  4971,  4978,  4979,  4984,  4985,  4986,
    4990,  4995,  5000,  5002,  5010,  5025,  5026,  5030,  5034,  5041,
    5045,  5049,  5053,  5057,  5061,  5068,  5072,  5079,  5086,  5093,
    5097,  5101,  5108,  5112,  5116,  5120,  5138,  5145,  5152,  5159,
    5160,  5168,  5169,  5173,  5174,  5178,  5179,  5180,  5185,  5186,
    5187,  5188,  5189,  5190,  5191,  5192,  5193,  5194,  5195,  5196,
    5197,  5198,  5199,  5200,  5201,  5202,  5203,  5204,  5205,  5215,
    5216,  5217,  5218,  5219,  5220,  5221,  5222,  5223,  5224,  5225,
    5226,  5227,  5228,  5229,  5230,  5231,  5232,  5233,  5234,  5235,
    5236,  5237,  5238,  5239,  5240,  5241,  5242,  5243,  5244,  5245,
    5246,  5247,  5248,  5249,  5250,  5251,  5252,  5253,  5254,  5255,
    5256,  5257,  5258,  5259,  5260,  5261,  5262,  5263,  5264,  5265,
    5266,  5267,  5268,  5269,  5270,  5271,  5272,  5273,  5274,  5275,
    5276,  5277,  5278,  5279,  5280,  5281,  5282,  5283,  5284,  5285,
    5286,  5287,  5288,  5289,  5290,  5291,  5292,  5293,  5294,  5295,
    5296,  5297,  5298,  5299,  5300,  5301,  5302,  5303,  5304,  5305,
    5306,  5307,  5308,  5309,  5310,  5311,  5312,  5313,  5314,  5315,
    5316,  5317,  5318,  5319,  5320,  5321,  5322,  5323,  5324,  5325,
    5326,  5327,  5328,  5329,  5330,  5331,  5332,  5333,  5334,  5335,
    5336,  5337,  5338,  5339,  5340,  5347,  5346,  5355,  5356,  5360,
    5361,  5365,  5365,  5371,  5372,  5373,  5374,  5378,  5379,  5383,
    5384,  5385,  5386,  5390,  5391,  5392,  5393,  5397,  5398,  5402,
    5413,  5425,  5429,  5436,  5437,  5438,  5442,  5447,  5451,  5466,
    5467,  5468,  5469,  5473,  5474,  5475,  5476,  5477,  5481,  5482,
    5487,  5486,  5496,  5495,  5503,  5504,  5509,  5510,  5511,  5516,
    5517,  5518,  5522,  5523,  5527,  5534,  5538,  5545,  5552,  5559,
    5568,  5569,  5574,  5573,  5589,  5590,  5591,  5595,  5595,  5616,
    5617,  5621,  5622,  5623,  5627,  5631,  5638,  5641,  5639,  5654,
    5661,  5680
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ABORT_SYM", "ACTION", "ADD_SYM",
  "ADDDATE_SYM", "AFTER_SYM", "AGGREGATE_SYM", "ALL", "ALTER_SYM",
  "ANALYZE_SYM", "AND_SYM", "ANY_SYM", "AS", "ASC", "ASENSITIVE_SYM",
  "AT_SYM", "AUTO_INC", "AVG_SYM", "BEFORE_SYM", "BEGIN_SYM",
  "BETWEEN_SYM", "BIGINT_SYM", "BINARY", "BIN_NUM", "BIT_SYM", "BLOB_SYM",
  "BOOLEAN_SYM", "BOOL_SYM", "BOTH", "BTREE_SYM", "BY", "CALL_SYM",
  "CASCADE", "CASCADED", "CASE_SYM", "CAST_SYM", "CATALOG_SYM",
  "CHAIN_SYM", "CHANGE_SYM", "CHAR_SYM", "CHECKSUM_SYM", "CHECK_SYM",
  "CLOSE_SYM", "COALESCE", "COLLATE_SYM", "COLLATION_SYM", "COLUMNS",
  "COLUMN_SYM", "COMMENT_SYM", "COMMITTED_SYM", "COMMIT_SYM",
  "COMPACT_SYM", "COMPRESSED_SYM", "CONCURRENT", "CONDITION_SYM",
  "CONNECTION_SYM", "CONSISTENT_SYM", "CONSTRAINT", "CONTAINS_SYM",
  "CONVERT_SYM", "COUNT_SYM", "CREATE", "CROSS", "CUBE_SYM", "CURDATE",
  "CURRENT_USER", "CURSOR_SYM", "DATABASE", "DATABASES", "DATA_SYM",
  "DATETIME_SYM", "DATE_ADD_INTERVAL", "DATE_SUB_INTERVAL", "DATE_SYM",
  "DAY_HOUR_SYM", "DAY_MICROSECOND_SYM", "DAY_MINUTE_SYM",
  "DAY_SECOND_SYM", "DAY_SYM", "DEALLOCATE_SYM", "DECIMAL_NUM",
  "DECIMAL_SYM", "DECLARE_SYM", "DEFAULT", "DELETE_SYM", "DESC",
  "DESCRIBE", "DETERMINISTIC_SYM", "DISABLE_SYM", "DISCARD", "DISTINCT",
  "DIV_SYM", "DO_SYM", "DOUBLE_SYM", "DROP", "DUMPFILE", "DUPLICATE_SYM",
  "DYNAMIC_SYM", "EACH_SYM", "ELSE", "ENABLE_SYM", "ENCLOSED", "END",
  "ENDS_SYM", "END_OF_INPUT", "ENGINE_SYM", "ENUM_SYM", "EQUAL_SYM",
  "ERRORS", "ESCAPED", "ESCAPE_SYM", "EXCLUSIVE_SYM", "EXECUTE_SYM",
  "EXISTS", "EXTENDED_SYM", "EXTRACT_SYM", "FALSE_SYM", "FILE_SYM",
  "FIRST_SYM", "FIXED_SYM", "FLOAT_NUM", "FLUSH_SYM", "FORCE_SYM",
  "FOREIGN", "FOR_SYM", "FOUND_SYM", "FRAC_SECOND_SYM", "FROM", "FULL",
  "GE", "GLOBAL_SYM", "GROUP_SYM", "GROUP_CONCAT_SYM", "HASH_SYM",
  "HAVING", "HEX_NUM", "HOUR_MICROSECOND_SYM", "HOUR_MINUTE_SYM",
  "HOUR_SECOND_SYM", "HOUR_SYM", "IDENT", "IDENTIFIED_SYM", "IDENTITY_SYM",
  "IDENT_QUOTED", "IF", "IGNORE_SYM", "IMPORT", "INDEXES", "INDEX_SYM",
  "INFILE", "INNER_SYM", "INOUT_SYM", "INSENSITIVE_SYM", "INSERT",
  "INTERVAL_SYM", "INTO", "INT_SYM", "IN_SYM", "IS", "ISOLATION",
  "ITERATE_SYM", "JOIN_SYM", "KEYS", "KEY_BLOCK_SIZE", "KEY_SYM",
  "KILL_SYM", "LAST_SYM", "LE", "LEADING", "LEFT", "LEVEL_SYM",
  "LEX_HOSTNAME", "LIKE", "LIMIT", "LINES", "LOAD", "LOCAL_SYM",
  "LOCKS_SYM", "LOCK_SYM", "LOGS_SYM", "LONG_NUM", "LONG_SYM", "MATCH",
  "MAX_SYM", "MAX_VALUE_SYM", "MEDIUM_SYM", "MERGE_SYM", "MICROSECOND_SYM",
  "MINUTE_MICROSECOND_SYM", "MINUTE_SECOND_SYM", "MINUTE_SYM", "MIN_SYM",
  "MODE_SYM", "MODIFIES_SYM", "MODIFY_SYM", "MOD_SYM", "MONTH_SYM",
  "NAMES_SYM", "NAME_SYM", "NATIONAL_SYM", "NATURAL", "NE", "NEW_SYM",
  "NEXT_SYM", "NONE_SYM", "NOT_SYM", "NOW_SYM", "NO_SYM", "NULL_SYM",
  "NUM", "NUMERIC_SYM", "OFFLINE_SYM", "OFFSET_SYM", "ON", "ONE_SHOT_SYM",
  "ONE_SYM", "ONLINE_SYM", "OPEN_SYM", "OPTIMIZE", "OPTION", "OPTIONALLY",
  "ORDER_SYM", "OR_SYM", "OUTER", "OUTFILE", "OUT_SYM", "PARTIAL",
  "POSITION_SYM", "PRECISION", "PREV_SYM", "PRIMARY_SYM", "PROCESS",
  "PROCESSLIST_SYM", "QUARTER_SYM", "QUERY_SYM", "RANGE_SYM", "READS_SYM",
  "READ_SYM", "READ_WRITE_SYM", "REAL", "REDUNDANT_SYM", "REGEXP_SYM",
  "REFERENCES", "RELEASE_SYM", "RENAME", "REPEATABLE_SYM", "REPEAT_SYM",
  "REPLACE", "REPLICATE", "REPLICATION", "RESTRICT", "RETURNS_SYM",
  "RETURN_SYM", "REVOKE", "RIGHT", "ROLLBACK_SYM", "ROLLUP_SYM",
  "ROUTINE_SYM", "ROWS_SYM", "ROW_FORMAT_SYM", "ROW_SYM", "SAVEPOINT_SYM",
  "SECOND_MICROSECOND_SYM", "SECOND_SYM", "SECURITY_SYM", "SELECT_SYM",
  "SENSITIVE_SYM", "SEPARATOR_SYM", "SERIALIZABLE_SYM", "SERIAL_SYM",
  "SESSION_SYM", "SERVER_SYM", "SET_SYM", "SET_VAR", "SHARE_SYM", "SHOW",
  "SIGNED_SYM", "SIMPLE_SYM", "SNAPSHOT_SYM", "SPECIFIC_SYM",
  "SQLEXCEPTION_SYM", "SQLSTATE_SYM", "SQLWARNING_SYM", "SQL_BIG_RESULT",
  "SQL_BUFFER_RESULT", "SQL_CALC_FOUND_ROWS", "SQL_SMALL_RESULT",
  "SQL_SYM", "STARTING", "START_SYM", "STATUS_SYM", "STDDEV_SAMP_SYM",
  "STD_SYM", "STOP_SYM", "STORED_SYM", "STRAIGHT_JOIN", "STRING_SYM",
  "SUBDATE_SYM", "SUBJECT_SYM", "SUBSTRING", "SUM_SYM", "SUSPEND_SYM",
  "SYSDATE", "TABLES", "TABLESPACE", "TABLE_REF_PRIORITY", "TABLE_SYM",
  "TEMPORARY_SYM", "TERMINATED", "TEXT_STRING", "TEXT_SYM", "THEN_SYM",
  "TIME_SYM", "TIMESTAMP_SYM", "TIMESTAMP_ADD", "TIMESTAMP_DIFF", "TO_SYM",
  "TRAILING", "TRANSACTION_SYM", "TRIM", "TRUE_SYM", "TRUNCATE_SYM",
  "TYPE_SYM", "ULONGLONG_NUM", "UNCOMMITTED_SYM", "UNDOFILE_SYM",
  "UNDO_SYM", "UNION_SYM", "UNIQUE_SYM", "UNKNOWN_SYM", "UNLOCK_SYM",
  "UNSIGNED_SYM", "UPDATE_SYM", "USAGE", "USER", "USE_SYM", "USING",
  "UTC_DATE_SYM", "UTC_TIMESTAMP_SYM", "UUID_SYM", "VALUES", "VALUE_SYM",
  "VARBINARY", "VARCHAR_SYM", "VARIABLES", "VARIANCE_SYM", "VARYING",
  "VAR_SAMP_SYM", "WAIT_SYM", "WARNINGS", "WEEK_SYM", "WHEN_SYM", "WHERE",
  "WITH", "WITH_ROLLUP_SYM", "WORK_SYM", "WRITE_SYM", "XOR",
  "YEAR_MONTH_SYM", "YEAR_SYM", "ZEROFILL_SYM", "'='", "LESS_THAN",
  "GREATER_THAN", "CONCAT", "'|'", "'&'", "SHIFT_RIGHT", "SHIFT_LEFT",
  "'-'", "'+'", "'*'", "'/'", "'%'", "'^'", "'~'", "UMINUS", "'('", "')'",
  "'{'", "'}'", "','", "'.'", "'@'", "$accept", "query", "verb_clause",
  "statement", "create", "$@1", "$@2", "$@3", "create_table_definition",
  "$@4", "create_select_as", "$@5", "$@6", "create_like", "create_select",
  "stored_select", "$@7", "$@8", "opt_create_database_options",
  "opt_database_custom_options", "custom_database_option",
  "opt_table_options", "opt_if_not_exists", "opt_create_table_options",
  "create_table_options_space_separated", "create_table_options",
  "create_table_option", "custom_engine_option", "default_collation",
  "default_collation_schema", "row_format", "row_format_or_text",
  "opt_select_from", "field_list", "field_list_item", "column_def",
  "key_def", "opt_check_constraint", "check_constraint", "opt_constraint",
  "constraint", "field_spec", "$@9", "field_def", "$@10", "varchar",
  "int_type", "real_type", "float_options", "precision", "opt_len",
  "opt_field_number_signed", "ignored_field_number_length", "opt_zerofill",
  "opt_precision", "opt_attribute_not_null", "opt_attribute_index",
  "opt_attribute", "opt_attribute_boolean", "opt_attribute_string",
  "opt_attribute_list_string", "attribute_string", "opt_attribute_number",
  "opt_attribute_list_integer", "attribute_integer",
  "opt_attribute_timestamp", "opt_attribute_list_timestamp",
  "attribute_timestamp", "opt_attribute_comment", "collation_name",
  "collation_name_or_default", "opt_default", "opt_primary", "references",
  "opt_ref_list", "ref_list", "opt_match_clause", "opt_on_update_delete",
  "delete_option", "key_type", "constraint_key_type", "key_or_index",
  "opt_key_or_index", "keys_or_index", "opt_unique", "init_key_options",
  "key_alg", "key_options", "key_opts", "key_using_alg", "key_opt",
  "btree_or_rtree", "key_list", "key_part", "opt_ident", "opt_component",
  "string_list", "alter", "$@11", "$@12", "alter_commands", "build_method",
  "alter_list", "add_column", "alter_list_item", "$@13", "$@14", "$@15",
  "opt_column", "opt_ignore", "opt_place", "opt_to", "start",
  "start_transaction_opts", "analyze", "$@16", "check", "$@17", "rename",
  "$@18", "table_to_table_list", "table_to_table", "select", "select_init",
  "select_paren", "select_paren_derived", "select_init2", "$@19",
  "select_part2", "$@20", "$@21", "select_into", "select_from",
  "select_options", "select_option_list", "select_option_distinct_or_all",
  "select_option_small_or_big", "select_option", "select_lock_type",
  "select_item_list", "select_item", "remember_name", "remember_end",
  "select_alias", "optional_braces", "expr", "bool_pri", "predicate",
  "bit_expr", "comp_op", "all_or_any", "simple_expr",
  "function_call_keyword", "function_call_nonkeyword",
  "function_call_conflict", "function_call_generic", "@22",
  "opt_udf_expr_list", "udf_expr_list", "udf_expr", "sum_expr", "$@23",
  "$@24", "$@25", "variable", "$@26", "variable_aux", "opt_distinct",
  "opt_gconcat_separator", "opt_gorder_clause", "in_sum_expr", "$@27",
  "cast_type", "expr_list", "opt_expr", "opt_else", "when_list",
  "table_ref", "join_table_list", "esc_table_ref", "derived_table_list",
  "join_table", "$@28", "$@29", "$@30", "$@31", "$@32", "$@33", "$@34",
  "normal_join", "table_factor", "$@35", "select_derived_union", "$@36",
  "$@37", "select_init2_derived", "select_part2_derived", "$@38", "$@39",
  "select_derived", "$@40", "select_derived2", "$@41", "$@42",
  "get_select_lex", "select_derived_init", "opt_outer",
  "index_hint_clause", "index_hint_type", "index_hint_definition", "$@43",
  "$@44", "index_hints_list", "opt_index_hints_list", "$@45",
  "opt_key_definition", "$@46", "opt_key_usage_list", "key_usage_element",
  "key_usage_list", "using_list", "interval", "interval_time_stamp",
  "interval_time_st", "table_alias", "opt_table_alias", "opt_all",
  "where_clause", "$@47", "having_clause", "$@48", "opt_escape",
  "group_clause", "group_list", "olap_opt", "alter_order_clause",
  "alter_order_list", "alter_order_item", "opt_order_clause",
  "order_clause", "$@49", "order_list", "order_dir",
  "opt_limit_clause_init", "opt_limit_clause", "limit_clause",
  "limit_options", "limit_option", "delete_limit_clause", "ulong_num",
  "ulonglong_num", "select_var_list_init", "$@50", "select_var_list",
  "select_var_ident", "into", "$@51", "into_destination", "$@52", "drop",
  "$@53", "table_list", "table_name", "if_exists", "opt_temporary",
  "execute", "execute_var_or_string", "opt_status", "opt_concurrent",
  "opt_wait", "insert", "$@54", "$@55", "replace", "$@56", "$@57",
  "insert2", "insert_table", "insert_field_spec", "$@58", "fields",
  "insert_values", "$@59", "$@60", "values_list", "ident_eq_list",
  "ident_eq_value", "equal", "opt_equal", "no_braces", "$@61",
  "opt_values", "values", "expr_or_default", "opt_insert_update", "$@62",
  "update", "$@63", "update_list", "update_elem", "insert_update_list",
  "insert_update_elem", "delete", "$@64", "opt_delete_option", "truncate",
  "opt_table_sym", "show", "$@65", "show_param", "opt_db", "from_or_in",
  "show_wild", "describe", "$@66", "$@67", "describe_command",
  "opt_extended_describe", "opt_describe_column", "flush", "$@68",
  "flush_options", "flush_option", "$@69", "opt_table_list", "kill",
  "kill_option", "use", "load", "$@70", "$@71", "$@72", "$@73",
  "data_file", "load_data_lock", "opt_duplicate", "opt_duplicate_as",
  "opt_field_term", "field_term_list", "field_term", "opt_line_term",
  "line_term_list", "line_term", "opt_ignore_lines", "lines_or_rows",
  "opt_field_or_var_spec", "fields_or_vars", "field_or_var",
  "opt_load_data_set_spec", "text_literal", "text_string",
  "signed_literal", "literal", "integer_literal", "boolean_literal",
  "date_literal", "NUM_literal", "insert_ident", "table_wild",
  "order_ident", "simple_ident", "simple_ident_q", "field_ident",
  "table_ident", "schema_name", "catalog_name", "IDENT_sys",
  "TEXT_STRING_sys", "TEXT_STRING_literal", "TEXT_STRING_filesystem",
  "ident", "ident_or_text", "engine_option_value",
  "keyword_exception_for_variable", "keyword", "keyword_sp", "set", "$@74",
  "opt_option", "option_value_list", "option_type_value", "$@75",
  "option_type", "option_type2", "opt_var_type", "opt_var_ident_type",
  "ext_option_value", "sys_option_value", "option_value",
  "user_variable_ident", "internal_variable_ident",
  "internal_variable_name", "isolation_types", "set_expr_or_default",
  "table_or_tables", "unlock", "$@76", "begin", "$@77", "opt_work",
  "opt_chain", "opt_release", "opt_savepoint", "commit", "rollback",
  "savepoint", "release", "savepoint_ident", "union_clause", "union_list",
  "$@78", "union_opt", "union_order_or_limit", "$@79", "order_or_limit",
  "union_option", "query_specification", "query_expression_body", "$@80",
  "subselect", "subselect_start", "subselect_end", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   517,   518,   519,   520,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,   539,   540,   541,   542,   543,   544,
     545,   546,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   558,   559,   560,   561,   562,   563,   564,
     565,   566,   567,   568,   569,   570,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   580,   581,   582,   583,   584,
     585,   586,   587,   588,   589,   590,   591,   592,   593,   594,
     595,   596,   597,   598,   599,   600,   601,   602,   603,   604,
     605,   606,   607,   608,   609,   610,   611,   612,   613,   614,
     615,   616,   617,    61,   618,   619,   620,   124,    38,   621,
     622,    45,    43,    42,    47,    37,    94,   126,   623,    40,
      41,   123,   125,    44,    46,    64
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   386,   387,   387,   388,   388,   389,   389,   389,   389,
     389,   389,   389,   389,   389,   389,   389,   389,   389,   389,
     389,   389,   389,   389,   389,   389,   389,   389,   389,   389,
     389,   389,   390,   391,   390,   392,   390,   393,   390,   394,
     395,   394,   394,   394,   394,   396,   397,   396,   398,   396,
     399,   400,   402,   403,   401,   404,   404,   404,   405,   405,
     406,   406,   406,   406,   406,   407,   407,   408,   408,   409,
     409,   410,   410,   411,   411,   411,   412,   413,   413,   413,
     413,   413,   413,   413,   413,   413,   413,   414,   415,   416,
     416,   416,   416,   416,   416,   417,   418,   418,   419,   419,
     420,   420,   421,   421,   422,   422,   422,   422,   422,   423,
     423,   424,   425,   425,   426,   428,   427,   429,   429,   429,
     429,   429,   429,   429,   430,   429,   429,   429,   429,   429,
     429,   429,   429,   429,   429,   429,   429,   431,   431,   431,
     432,   432,   433,   433,   433,   434,   434,   434,   435,   436,
     436,   437,   437,   437,   438,   438,   439,   439,   440,   440,
     440,   441,   441,   442,   442,   442,   443,   443,   443,   443,
     444,   444,   444,   444,   445,   445,   446,   446,   447,   447,
     447,   447,   448,   448,   449,   449,   450,   450,   450,   450,
     450,   451,   451,   452,   452,   453,   453,   453,   453,   453,
     454,   454,   455,   456,   456,   457,   457,   458,   458,   459,
     460,   460,   461,   461,   462,   462,   462,   462,   463,   463,
     463,   463,   463,   464,   464,   464,   464,   464,   465,   466,
     466,   467,   467,   468,   468,   469,   469,   469,   470,   470,
     471,   472,   472,   473,   473,   474,   474,   475,   476,   476,
     476,   477,   477,   478,   478,   479,   479,   480,   480,   481,
     481,   482,   482,   484,   483,   485,   483,   486,   486,   486,
     486,   487,   487,   487,   488,   488,   489,   490,   490,   490,
     491,   490,   492,   493,   490,   490,   490,   490,   490,   490,
     490,   490,   490,   490,   490,   490,   490,   490,   494,   494,
     495,   495,   496,   496,   496,   497,   497,   497,   498,   499,
     499,   501,   500,   503,   502,   505,   504,   506,   506,   507,
     508,   509,   509,   510,   510,   511,   511,   513,   512,   515,
     516,   514,   517,   517,   517,   517,   517,   518,   519,   519,
     520,   520,   521,   521,   522,   522,   523,   523,   523,   523,
     523,   524,   524,   524,   525,   525,   525,   526,   526,   527,
     528,   529,   529,   529,   529,   529,   530,   530,   531,   531,
     531,   531,   531,   531,   531,   531,   531,   531,   531,   532,
     532,   532,   532,   532,   532,   533,   533,   533,   533,   533,
     533,   533,   533,   533,   533,   533,   533,   533,   534,   534,
     534,   534,   534,   534,   534,   534,   534,   534,   534,   534,
     534,   534,   534,   535,   535,   535,   535,   535,   535,   536,
     536,   537,   537,   537,   537,   537,   537,   537,   537,   537,
     537,   537,   537,   537,   537,   537,   537,   537,   537,   537,
     537,   537,   537,   537,   537,   537,   537,   538,   538,   538,
     538,   538,   538,   538,   538,   538,   538,   538,   538,   538,
     538,   538,   538,   538,   538,   538,   538,   538,   538,   538,
     538,   539,   539,   539,   539,   539,   539,   539,   539,   539,
     539,   539,   539,   539,   539,   539,   539,   539,   539,   539,
     539,   539,   540,   540,   540,   540,   540,   540,   540,   540,
     540,   540,   540,   540,   540,   540,   540,   540,   542,   541,
     543,   543,   544,   544,   545,   546,   546,   546,   546,   547,
     548,   546,   546,   546,   546,   546,   546,   546,   546,   546,
     546,   546,   549,   546,   551,   550,   552,   552,   552,   553,
     553,   554,   554,   555,   555,   557,   556,   558,   558,   558,
     558,   558,   558,   558,   558,   558,   558,   558,   558,   559,
     559,   560,   560,   561,   561,   562,   562,   563,   563,   564,
     565,   565,   566,   566,   567,   567,   568,   567,   569,   567,
     570,   567,   567,   571,   567,   572,   567,   567,   573,   567,
     574,   567,   567,   575,   575,   575,   577,   576,   576,   576,
     578,   579,   580,   578,   581,   583,   584,   582,   586,   585,
     588,   589,   587,   590,   591,   592,   592,   593,   593,   593,
     593,   594,   594,   596,   595,   597,   595,   598,   598,   599,
     600,   599,   602,   601,   603,   603,   604,   604,   605,   605,
     606,   606,   607,   607,   607,   607,   607,   607,   607,   607,
     607,   607,   607,   607,   608,   608,   609,   609,   609,   609,
     609,   609,   609,   609,   609,   610,   610,   611,   611,   612,
     612,   613,   614,   613,   615,   616,   615,   617,   617,   618,
     618,   619,   619,   620,   620,   621,   622,   622,   623,   624,
     624,   626,   625,   627,   627,   628,   628,   628,   629,   629,
     630,   630,   631,   632,   632,   632,   633,   633,   633,   634,
     634,   635,   635,   635,   635,   635,   635,   636,   636,   636,
     636,   636,   638,   637,   639,   639,   640,   642,   641,   644,
     643,   643,   643,   645,   645,   646,   645,   645,   647,   647,
     648,   649,   649,   650,   650,   651,   652,   652,   653,   653,
     654,   654,   655,   655,   657,   658,   656,   660,   661,   659,
     662,   662,   663,   664,   664,   664,   665,   664,   666,   666,
     667,   667,   668,   667,   669,   667,   670,   670,   671,   671,
     672,   673,   673,   674,   674,   676,   675,   677,   677,   678,
     678,   679,   679,   680,   681,   680,   683,   682,   684,   684,
     685,   686,   686,   687,   689,   688,   690,   690,   691,   692,
     692,   694,   693,   695,   695,   695,   695,   695,   695,   695,
     695,   695,   695,   695,   695,   695,   695,   695,   695,   696,
     696,   697,   697,   698,   698,   698,   700,   699,   701,   699,
     702,   702,   703,   703,   704,   704,   704,   706,   705,   707,
     707,   709,   708,   708,   708,   708,   708,   710,   710,   711,
     712,   712,   712,   713,   715,   716,   717,   718,   714,   719,
     720,   720,   721,   721,   721,   722,   722,   722,   722,   722,
     722,   723,   723,   724,   724,   725,   725,   725,   725,   726,
     726,   727,   727,   728,   728,   729,   729,   730,   730,   731,
     731,   731,   732,   732,   733,   733,   734,   734,   735,   735,
     736,   736,   736,   737,   737,   737,   738,   738,   738,   738,
     738,   738,   738,   738,   738,   739,   739,   739,   739,   739,
     739,   739,   740,   740,   740,   741,   741,   741,   741,   741,
     742,   742,   742,   742,   742,   743,   743,   744,   744,   745,
     746,   746,   747,   747,   747,   748,   748,   748,   748,   749,
     749,   749,   750,   751,   752,   752,   753,   754,   755,   756,
     756,   757,   757,   758,   758,   759,   759,   759,   760,   760,
     760,   760,   760,   760,   760,   760,   760,   760,   760,   760,
     760,   760,   760,   760,   760,   760,   760,   760,   760,   761,
     761,   761,   761,   761,   761,   761,   761,   761,   761,   761,
     761,   761,   761,   761,   761,   761,   761,   761,   761,   761,
     761,   761,   761,   761,   761,   761,   761,   761,   761,   761,
     761,   761,   761,   761,   761,   761,   761,   761,   761,   761,
     761,   761,   761,   761,   761,   761,   761,   761,   761,   761,
     761,   761,   761,   761,   761,   761,   761,   761,   761,   761,
     761,   761,   761,   761,   761,   761,   761,   761,   761,   761,
     761,   761,   761,   761,   761,   761,   761,   761,   761,   761,
     761,   761,   761,   761,   761,   761,   761,   761,   761,   761,
     761,   761,   761,   761,   761,   761,   761,   761,   761,   761,
     761,   761,   761,   761,   761,   761,   761,   761,   761,   761,
     761,   761,   761,   761,   761,   761,   761,   761,   761,   761,
     761,   761,   761,   761,   761,   763,   762,   764,   764,   765,
     765,   767,   766,   768,   768,   768,   768,   769,   769,   770,
     770,   770,   770,   771,   771,   771,   771,   772,   772,   773,
     773,   774,   774,   775,   775,   775,   776,   776,   777,   778,
     778,   778,   778,   779,   779,   779,   779,   779,   780,   780,
     782,   781,   784,   783,   785,   785,   786,   786,   786,   787,
     787,   787,   788,   788,   789,   790,   790,   791,   792,   793,
     794,   794,   796,   795,   797,   797,   797,   799,   798,   800,
     800,   801,   801,   801,   802,   802,   803,   804,   803,   805,
     806,   807
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     0,     7,     0,    13,     0,     6,     5,
       0,     5,     4,     2,     2,     0,     0,     4,     0,     6,
       2,     1,     0,     0,     6,     0,     1,     1,     1,     3,
       1,     3,     3,     3,     3,     0,     1,     0,     3,     0,
       1,     1,     2,     1,     2,     3,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     4,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     3,
       1,     1,     2,     2,     7,     8,     8,     2,     2,     0,
       1,     2,     0,     1,     2,     0,     4,     2,     5,     2,
       2,     2,     5,     2,     0,     6,     5,     2,     2,     2,
       2,     3,     3,     3,     2,     2,     1,     2,     1,     1,
       4,     4,     2,     2,     3,     0,     3,     1,     5,     0,
       3,     0,     1,     1,     0,     3,     0,     1,     0,     3,
       1,     1,     2,     2,     1,     2,     0,     2,     3,     2,
       0,     2,     3,     2,     0,     1,     2,     1,     1,     2,
       2,     1,     0,     1,     2,     1,     1,     1,     3,     2,
       1,     0,     1,     2,     1,     1,     3,     2,     4,     1,
       0,     2,     1,     1,     1,     0,     1,     0,     1,     5,
       0,     3,     3,     1,     0,     2,     2,     2,     0,     3,
       3,     6,     6,     1,     1,     2,     2,     2,     1,     2,
       2,     1,     1,     0,     1,     1,     1,     1,     0,     1,
       0,     1,     2,     0,     1,     1,     2,     2,     1,     3,
       2,     1,     1,     4,     2,     1,     4,     0,     1,     0,
       2,     1,     3,     0,     7,     0,     5,     0,     2,     2,
       1,     0,     1,     1,     1,     3,     2,     3,     2,     4,
       0,     6,     0,     0,     8,     3,     4,     3,     3,     2,
       2,     6,     5,     3,     3,     1,     1,     1,     0,     1,
       0,     1,     0,     2,     1,     0,     1,     1,     3,     0,
       3,     0,     4,     0,     4,     0,     4,     1,     3,     3,
       1,     2,     4,     2,     3,     2,     3,     0,     3,     0,
       0,     6,     2,     1,     1,     2,     2,     7,     0,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     2,     4,     3,     1,     1,     3,     4,     0,
       0,     0,     2,     2,     1,     1,     0,     2,     3,     3,
       3,     2,     3,     4,     3,     4,     3,     4,     1,     3,
       4,     3,     3,     6,     1,     5,     6,     5,     7,     6,
       8,     5,     6,     4,     5,     3,     4,     1,     3,     3,
       3,     3,     3,     3,     5,     5,     3,     3,     3,     3,
       3,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     1,     1,
       3,     2,     2,     2,     3,     3,     5,     6,     4,     4,
       2,     6,     5,     6,     4,     4,     5,     4,     2,     4,
       4,     4,    10,     6,     8,     6,     4,     4,     6,     4,
       4,     4,     7,     7,     7,     6,     6,     6,     6,     3,
       4,     6,     8,     2,     8,     8,     6,     2,     4,     6,
       6,     8,     8,     6,     8,     6,     2,     4,     8,     8,
       2,     2,     4,     4,     3,     3,     5,     8,     5,     4,
       6,     4,     6,     8,     6,     4,     3,     6,     0,     5,
       0,     1,     1,     3,     4,     4,     5,     5,     4,     0,
       0,     7,     4,     5,     4,     5,     4,     4,     4,     4,
       4,     5,     0,     8,     0,     3,     3,     1,     4,     0,
       1,     0,     2,     0,     1,     0,     3,     2,     1,     1,
       2,     1,     1,     2,     2,     1,     1,     1,     2,     1,
       3,     0,     1,     0,     2,     4,     5,     1,     1,     1,
       1,     4,     1,     3,     3,     3,     0,     6,     0,     6,
       0,     8,     4,     0,     8,     0,    10,     6,     0,     8,
       0,    10,     6,     1,     2,     2,     0,     4,     3,     5,
       3,     0,     0,     8,     1,     0,     0,     6,     0,     3,
       0,     0,     5,     0,     1,     0,     1,     0,     2,     3,
       3,     1,     1,     0,     7,     0,     7,     1,     2,     0,
       0,     2,     0,     2,     0,     1,     1,     1,     1,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     0,     2,     0,
       1,     0,     0,     3,     0,     0,     3,     2,     0,     0,
       4,     4,     2,     0,     1,     3,     3,     1,     2,     0,
       1,     0,     4,     4,     2,     0,     1,     1,     0,     1,
       0,     1,     2,     1,     3,     3,     1,     1,     1,     0,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     2,     3,     1,     2,     0,     3,     0,
       5,     2,     1,     3,     5,     0,     7,     4,     1,     3,
       1,     0,     2,     0,     1,     5,     1,     2,     0,     3,
       0,     1,     0,     1,     0,     0,     7,     0,     0,     5,
       2,     1,     1,     1,     3,     4,     0,     3,     3,     1,
       2,     2,     0,     3,     0,     5,     3,     1,     3,     1,
       3,     1,     1,     0,     1,     0,     4,     0,     1,     3,
       1,     1,     1,     0,     0,     6,     0,     9,     3,     1,
       3,     3,     1,     3,     0,     8,     0,     1,     3,     0,
       1,     0,     3,     2,     3,     3,     4,     5,     5,     5,
       5,     2,     2,     3,     2,     3,     1,     3,     4,     0,
       2,     1,     1,     0,     2,     2,     0,     4,     0,     4,
       1,     1,     0,     1,     0,     1,     1,     0,     3,     3,
       1,     0,     3,     4,     1,     1,     2,     0,     1,     3,
       0,     1,     1,     2,     0,     0,     0,     0,    18,     1,
       0,     1,     0,     1,     1,     0,     1,     1,     1,     2,
       2,     0,     2,     2,     1,     3,     4,     3,     3,     0,
       2,     2,     1,     3,     3,     0,     3,     1,     1,     0,
       3,     2,     3,     1,     1,     2,     0,     2,     1,     2,
       1,     1,     1,     1,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     1,     1,     1,     1,     1,     3,     5,     1,
       1,     1,     3,     4,     5,     1,     5,     3,     2,     1,
       3,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     4,     0,     1,     1,
       3,     0,     2,     1,     1,     1,     1,     0,     1,     0,
       1,     1,     1,     0,     2,     2,     2,     1,     2,     4,
       5,     4,     6,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     3,     0,     3,     0,     1,     0,     3,     2,     0,
       1,     2,     0,     1,     4,     4,     5,     2,     3,     1,
       0,     1,     0,     4,     0,     1,     1,     0,     2,     2,
       1,     0,     1,     1,     2,     3,     1,     0,     5,     3,
       0,     0
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,   271,     0,  1172,     0,  1174,   271,   806,   840,   841,
     743,     2,     0,   847,   754,   860,     0,     0,     0,   757,
    1174,     0,   329,  1127,   811,     0,   809,  1170,   300,     0,
       0,     0,     0,     4,    10,     6,    27,     7,     8,    20,
      24,   320,    13,    14,    16,    21,    30,    11,    28,    26,
      12,   842,    15,    17,    31,    18,    25,    29,     5,     9,
      22,    23,    19,     0,   273,   272,   300,  1169,  1168,   311,
    1174,   313,  1175,  1176,     0,    67,    66,     0,    35,   807,
       0,     0,   741,   744,     0,     0,   964,   977,   965,  1155,
     976,   966,   975,     0,   748,  1157,  1154,  1156,   746,  1153,
       0,   300,   861,   862,     0,   869,   864,     0,   315,     0,
    1176,  1189,  1187,   321,   327,   338,  1128,  1125,  1139,   309,
     810,     0,     0,   301,     0,   999,  1000,  1001,  1002,  1003,
    1004,  1005,  1006,   979,  1007,  1009,  1008,  1010,  1011,  1012,
     980,   981,  1013,  1014,  1015,   982,  1016,   983,  1017,  1018,
    1019,  1020,  1021,   984,  1022,  1024,  1023,  1025,  1026,  1027,
     985,  1028,  1029,   986,  1030,  1031,  1032,  1041,   987,  1033,
    1035,  1034,  1036,  1037,  1038,  1039,  1043,  1044,  1045,   988,
    1040,  1046,  1042,  1047,  1048,  1049,  1050,  1051,  1052,  1053,
    1054,  1055,  1056,  1057,  1058,  1059,  1060,  1061,  1062,  1063,
    1064,  1066,  1065,  1067,  1069,  1068,  1070,  1072,  1071,  1073,
     989,  1074,  1075,  1076,  1077,  1078,   990,  1079,  1080,  1081,
    1082,  1083,  1084,  1085,  1086,  1087,   991,  1088,  1089,  1090,
    1091,  1092,   992,  1093,   993,  1095,  1094,  1096,   994,  1098,
     995,  1097,  1099,   996,  1100,   997,  1101,  1102,  1103,  1104,
    1105,  1106,  1107,  1108,  1110,  1111,  1112,  1109,   998,  1113,
    1114,  1115,  1116,  1118,  1117,  1120,  1119,  1121,  1122,  1123,
    1124,   863,   969,   962,   970,   978,   329,     0,     0,     1,
       3,  1039,     0,   838,   836,     0,   959,   265,     0,     0,
    1173,     0,     0,  1179,    32,   963,     0,     0,    67,   238,
       0,   733,     0,     0,     0,   741,   747,     0,   750,     0,
     854,   855,  1169,   848,   850,   851,     0,  1000,  1006,     0,
     922,   561,     0,     0,     0,  1013,  1014,     0,     0,   366,
     366,     0,     0,     0,  1026,  1027,   943,     0,     0,     0,
       0,   919,   944,     0,   921,  1049,     0,     0,     0,   860,
       0,   941,     0,  1063,  1064,     0,     0,  1067,     0,   366,
     918,   940,     0,  1083,     0,     0,     0,  1092,  1093,     0,
       0,  1102,     0,     0,   366,   967,     0,  1111,  1112,     0,
     920,   998,   942,  1118,   366,   366,  1117,     0,     0,     0,
       0,  1124,     0,     0,     0,  1210,     0,     0,   534,   859,
     378,   384,   397,   412,   422,   423,   425,   424,   429,   428,
     916,   427,   917,   421,   951,   969,   908,   950,   870,  1188,
       0,     0,   762,   758,   761,   740,  1182,  1179,  1190,   343,
     342,   345,   347,   350,   344,   346,   359,   339,   349,   348,
     341,  1131,     0,     0,     0,   833,   698,  1140,   237,   236,
     235,  1141,   826,  1142,   829,     0,     0,   698,     0,   812,
     973,   974,     0,     0,     0,   308,   808,  1171,     0,   323,
       0,  1194,   961,     0,   844,     0,   205,     0,   312,   738,
     314,  1178,     0,     0,  1180,  1184,     0,    37,     0,   239,
       0,   804,   742,   737,     0,     0,     0,   751,   752,   856,
       0,     0,   857,   755,     0,   669,   440,   562,     0,     0,
       0,     0,     0,     0,     0,   669,     0,   473,   448,     0,
       0,     0,     0,   923,     0,     0,     0,  1210,     0,   539,
       0,     0,     0,  1210,     0,     0,     0,   669,     0,     0,
     669,     0,     0,   371,     0,   477,     0,     0,     0,     0,
       0,     0,     0,   669,   669,     0,     0,   669,     0,   486,
       0,   924,     0,     0,     0,     0,     0,   490,   491,     0,
       0,   669,   669,     0,     0,   432,   431,   433,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   414,     0,
     416,   418,   413,   417,   415,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   909,   508,     0,   871,     0,
     316,   317,     0,   760,     0,  1183,     0,  1185,  1201,   328,
    1191,   356,   330,   355,     0,   340,  1126,  1129,  1137,   831,
     832,     0,     0,    67,     0,     0,     0,   813,     0,   822,
     699,   833,     0,   829,   833,   821,     0,   824,   833,   833,
       0,     0,   324,  1195,   322,  1196,     0,   839,   912,   911,
     837,   845,   910,   846,   960,   206,   266,     0,   263,     0,
    1177,  1181,    68,    55,    33,     0,   671,     0,   734,   749,
     753,   745,     0,   849,   858,   852,     0,     0,   670,   669,
       0,   545,     0,   563,     0,   495,   559,     0,     0,     0,
       0,   519,     0,   545,   367,   494,     0,     0,     0,     0,
       0,     0,     0,   643,   644,   645,   646,   656,   647,   648,
     649,   658,   663,   650,   651,   659,   660,   661,   652,   662,
     657,   653,   664,     0,   642,   540,   532,     0,     0,     0,
       0,     0,     0,     0,   669,     0,     0,     0,   669,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   669,     0,     0,     0,   655,     0,
     654,     0,     0,     0,     0,     0,     0,   469,   506,     0,
       0,     0,     0,     0,   435,     0,   434,   605,     0,  1206,
    1211,     0,     0,  1143,   535,   537,   370,   368,   369,   381,
     374,     0,   379,   372,   376,   419,  1003,   382,     0,     0,
     409,  1210,   678,   410,     0,     0,     0,     0,   395,   398,
     399,   400,   401,     0,   403,     0,   402,   406,   407,   408,
     411,   971,   972,   426,   430,   359,   952,     0,     0,     0,
      52,   766,     0,     0,     0,   772,   759,   763,  1186,  1203,
    1202,  1192,   359,   689,   360,   360,   950,  1131,  1134,  1135,
    1138,  1136,     0,  1133,  1132,  1147,   829,     0,     0,   825,
     834,   835,   707,   708,   706,   702,   703,   814,   830,   833,
     815,   829,   823,   827,   310,   796,   799,     0,     0,   698,
    1200,  1198,   783,   205,   739,   783,    38,    57,    58,    56,
      60,    69,   240,   672,   689,   735,   853,   793,     0,     0,
     515,     0,     0,     0,     0,     0,     0,   447,     0,   492,
     493,     0,     0,   518,     0,     0,     0,   449,   450,   444,
     752,   438,     0,     0,   451,     0,     0,     0,     0,     0,
       0,     0,   524,   499,   456,     0,   522,     0,   457,   478,
       0,   501,     0,     0,     0,     0,   459,   528,   526,     0,
       0,     0,     0,   530,   487,   460,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   461,     0,   445,   527,   529,
     505,     0,   470,     0,  1204,   604,   338,   605,     0,     0,
    1201,  1209,   439,   953,     0,     0,     0,     0,     0,   375,
     380,   373,   377,  1210,     0,     0,     0,     0,   393,     0,
    1210,   678,   396,     0,     0,     0,     0,   511,   512,     0,
     968,   865,   318,   319,   338,     0,   785,   770,   777,   771,
       0,     0,     0,   769,   946,   945,  1190,     0,   354,   596,
     727,   351,   334,   700,   690,   333,   361,   357,     0,  1130,
       0,  1158,     0,     0,  1148,   833,     0,   828,     0,     0,
     816,   671,     0,   671,   782,   781,     0,   691,  1199,   784,
       0,   298,   298,   783,   298,   783,     0,     0,     0,   298,
       0,     0,     0,   296,     0,   298,     0,   305,   783,     0,
     295,    71,    76,    86,     0,   264,   270,     0,   274,   297,
       0,     0,     0,     0,     0,   112,    34,    69,   875,    70,
      73,   241,     0,     0,   709,   736,     0,   756,     0,     0,
     516,   546,     0,   564,     0,   442,   149,   548,   149,   557,
     555,   145,   551,   549,   556,   552,     0,   560,     0,   520,
     517,     0,     0,   496,     0,   543,     0,     0,     0,   446,
     498,     0,   525,   523,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   531,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   436,   359,   325,     0,
    1205,  1207,  1144,  1145,  1146,   259,   536,     0,   391,   387,
       0,   385,   677,     0,     0,     0,   394,   405,   404,   360,
     509,   359,   954,   872,   359,   767,   779,     0,   787,     0,
       0,   764,   774,     0,     0,   773,  1193,   614,   613,     0,
     570,   671,   572,   569,   568,   567,     0,   613,   722,     0,
       0,   331,   336,   332,   701,   335,     0,   358,   365,   364,
     947,   952,     0,     0,  1143,     0,   817,   820,   819,   705,
     704,   818,   798,   689,   792,   791,   800,     0,   204,   203,
      88,   202,   299,   257,   232,   231,   278,     0,   109,   257,
     228,   276,     0,     0,     0,     0,     0,   289,   268,     0,
       0,     0,     0,   290,     0,     0,   269,     0,     0,   307,
     306,     0,     0,     0,    72,   783,   205,   112,     0,   302,
     109,   115,   955,     0,    62,    61,    59,   720,   721,   719,
     717,   718,    64,    63,    50,     0,     0,    51,     0,    98,
     100,   101,    43,   876,   878,   877,    44,     0,   205,    74,
       0,   242,     0,   673,     0,   805,   794,     0,   471,   565,
       0,     0,   547,   554,     0,   558,   147,   550,   553,   441,
     443,     0,     0,     0,   476,   541,   544,     0,     0,   453,
       0,   455,   500,   479,   502,     0,   458,   437,     0,   480,
       0,   485,   483,     0,     0,     0,   467,     0,   465,     0,
     466,     0,   468,   504,   507,   606,   326,     0,     0,   538,
     383,     0,   392,   389,     0,   386,   361,   513,   874,   873,
       0,    53,     0,     0,     0,   788,   790,   776,  1194,   765,
     768,   613,   596,     0,     0,   593,   615,     0,   615,   596,
     596,   679,   596,   665,   610,     0,     0,   732,     0,   728,
     352,     0,   363,   362,     0,     0,  1166,  1167,  1164,  1165,
    1163,  1149,     0,     0,   709,   949,   692,   695,   114,   258,
       0,     0,     0,   233,   108,   257,   107,   110,   240,     0,
      79,   280,    78,   294,   257,   287,   288,   285,    77,    83,
     282,   685,   687,   695,   293,    81,    80,    89,    90,    91,
      92,    93,    94,    95,    82,     0,   275,     0,   958,     0,
     304,   277,     0,   102,   103,     0,     0,    85,    84,    69,
      40,    69,   112,   880,   879,     0,    46,    75,   251,   252,
     247,     0,   710,     0,     0,   566,     0,     0,   521,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   464,   462,   463,   700,  1208,   260,   388,     0,
     514,   866,   700,   778,   780,   786,     0,   775,     0,   689,
     608,     0,   595,   594,   616,     0,   596,   615,   615,     0,
     575,   574,     0,   674,   573,   666,     0,   632,   598,   338,
     731,   729,     0,   723,   725,     0,   948,     0,     0,  1162,
    1150,     0,  1151,   797,     0,   696,   697,   694,   111,   257,
     229,   234,   230,   240,     0,     0,     0,     0,   286,     0,
       0,   688,    87,   279,   303,   210,   154,   166,   170,   174,
     191,   166,   145,   158,   124,   145,   154,   145,   158,   136,
     174,   166,   191,   166,     0,   139,   200,     0,   182,   182,
     957,    42,  1197,   875,    99,     0,  1190,     0,     0,   472,
     150,   146,     0,   474,   475,   542,   533,   497,     0,   454,
     503,   481,   484,   482,   488,   489,   351,   351,    96,   390,
       0,    54,   789,  1201,   665,   700,   596,   571,   596,   582,
       0,     0,   596,   578,   576,   580,     0,   675,   689,   668,
     597,   629,   359,   881,   726,     0,   353,  1160,  1159,  1161,
       0,   695,     0,     0,     0,   292,     0,   302,   200,   686,
       0,   214,     0,   151,   130,   135,     0,     0,     0,   161,
     208,   164,   137,   178,   181,   121,   175,   177,     0,     0,
       0,   195,   199,   119,   192,   194,   128,   182,   158,     0,
     160,   143,     0,   182,   151,   182,   142,   123,   129,     0,
     117,   134,     0,     0,   116,     0,   187,     0,     0,   186,
     190,   120,   183,   185,   127,     0,    41,    39,    48,    47,
       0,   695,   255,     0,     0,     0,   607,    97,     0,   601,
     599,   600,   609,     0,   567,   596,   596,     0,   567,     0,
       0,     0,   683,   695,     0,   700,   633,     0,   611,     0,
     889,   724,  1152,   693,     0,     0,     0,     0,     0,     0,
       0,   291,   913,   281,   283,     0,   213,     0,   218,     0,
     152,   153,   156,     0,   167,   169,     0,   171,   173,   180,
     179,   162,   165,   176,   163,     0,   366,   936,     0,   935,
     197,   937,     0,   193,   131,   144,     0,     0,   133,   156,
     132,     0,     0,   201,     0,   927,   926,   929,     0,     0,
     925,   189,   928,     0,   184,   956,  1197,   243,     0,   254,
       0,   795,   802,     0,   148,   452,   867,     0,   583,     0,
     587,   592,   588,     0,   579,   577,     0,   684,     0,   680,
     682,   676,   337,   621,   622,     0,     0,   627,   631,   700,
       0,     0,     0,     0,   882,   884,     0,   730,     0,     0,
     243,   915,   914,   302,   211,     0,   215,   216,   217,     0,
     209,   155,   157,   141,   168,   933,   932,   934,   172,   938,
     196,   939,   366,   159,     0,   261,   140,   191,   166,   174,
     931,   930,   188,    49,     0,   783,    36,   244,   248,   245,
     695,     0,     0,     0,   881,   602,     0,     0,     0,     0,
       0,   640,   695,   617,   617,   628,   612,     0,     0,     0,
       0,   883,     0,     0,   890,   892,     0,   243,   104,   284,
     212,     0,     0,   198,   174,     0,   118,   126,   122,   250,
       0,   246,   253,   256,   801,   803,   889,   689,   584,     0,
     589,     0,   581,     0,   681,     0,   625,   623,   887,   888,
       0,   885,     0,     0,   891,   106,   105,   224,     0,   223,
       0,   220,   219,   125,   262,   715,   716,   712,   713,   711,
     714,   249,   895,   700,     0,     0,   641,     0,   618,     0,
       0,     0,   886,   894,   893,   226,   227,   225,     0,     0,
       0,   899,   603,   586,   591,   620,   619,   634,     0,     0,
       0,     0,     0,   906,   637,     0,   638,   635,   636,     0,
     222,   221,   897,   898,   896,   901,     0,     0,   903,   904,
       0,   868,   626,     0,   624,   905,   900,     0,   907,   639,
     902
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    31,    32,    33,    34,   911,   299,   683,  1116,  1642,
    1336,  1646,  1866,  1117,  1326,   855,  1034,  1552,   906,   907,
     908,    77,   297,  1118,  1100,  1119,  1120,  1102,  1103,   676,
    1493,  1494,  1666,  1328,  1329,  1330,  1331,  1466,  1467,  1277,
    1278,  1310,  1505,  1636,  1742,  1637,  1638,  1639,  1355,  1356,
    1352,  1822,  1713,  1923,  1741,  1759,  1760,  1714,  1715,  1725,
    1726,  1727,  1761,  1762,  1763,  1733,  1734,  1735,  1754,  1269,
    1270,  1104,  1728,  1504,  1711,  1815,  1818,  1920,  2021,  1279,
    1465,  1280,  1602,   458,   490,  1121,  1122,  1946,  1947,  1948,
    1949,  1520,  1770,  1771,  1458,  1399,  1934,    35,   903,   476,
    1105,    66,  1106,  1107,  1108,  1607,  1609,  1913,  1281,   124,
    1501,  1301,    36,   465,    37,   289,    38,   291,    39,   420,
     620,   621,    40,    41,   278,   999,   113,   428,   114,   115,
     863,  1051,  1667,   436,   437,   438,   439,   440,  1241,   632,
     633,   634,  1056,  1247,   517,   706,   400,   401,   402,   595,
     818,   403,   404,   405,   406,   407,   845,  1026,  1027,  1028,
     408,   932,  1361,   943,   409,   583,   804,   746,  1532,  1365,
     700,   921,  1146,   993,   508,   925,   703,  1230,  1231,  1232,
    1233,  1234,  1790,  1789,  1791,  1956,  1879,  1958,  1883,  1430,
    1235,  1236,  1558,  1877,  1997,   994,   995,   996,  1545,  1559,
    1676,  1578,  1579,  1899,  1421,  1237,  1565,  2006,  1896,  1897,
    2041,  2040,  1898,  1796,  1797,  1690,  1691,  2065,  2066,  2067,
    1960,   743,   779,   744,  1576,  1577,   701,   914,  1123,  1688,
    1794,  1018,  1573,  1792,  1889,  1109,  1481,  1482,  1053,  1054,
    1267,  1456,  1597,   649,  1668,  1244,   885,   886,  1345,  2031,
    1322,  1437,  1438,  1583,  1584,  1055,  1238,  1439,  1693,    42,
    1125,   478,   479,   303,    85,    43,    94,   308,   498,   691,
      44,   101,   696,    45,   109,   624,   423,   424,   856,  1035,
    1042,   857,  1046,  1418,  1037,  1215,  1216,  1079,  1080,  1038,
    1218,  1414,  1415,  1266,  1127,  1523,    46,  1073,   895,   896,
    1871,  1872,    47,   686,    80,    48,   121,    49,   118,   459,
     651,   652,   647,    50,   474,   473,    51,   283,   670,    52,
     100,   313,   314,   502,   695,    53,   104,    54,    55,   418,
    1213,  1670,  1954,   106,   619,  1410,  1337,  1800,  1904,  1905,
    1907,  1974,  1975,  2051,  2074,  2063,  2077,  2078,  2081,   410,
     671,  1811,   411,  1861,  1928,  1840,   412,  1043,  1044,  1457,
     413,   414,  1311,   425,   285,   294,   415,   842,   416,  1031,
     417,  1110,   462,    97,   274,   275,    56,   441,   117,   636,
     637,   638,   872,   873,   463,  1007,   874,   875,  1064,    98,
      99,  1062,  1590,  1451,   315,    57,   122,    58,    70,    73,
     293,   485,   626,    59,    60,    61,    62,   112,   629,   663,
    1047,   664,   665,   666,   901,   861,   799,   800,  1397,   579,
     580,  1001
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1565
static const yytype_int16 yypact[] =
{
    1833,   558,   -85, -1565,   -85,  -144,   487,   398, -1565, -1565,
    1028, -1565,    -8, -1565, -1565,   107,   151,   299,   -85, -1565,
    -144,   981, -1565,   394, -1565,   322,   340, -1565,   519, 14578,
       8,   698,   633, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
   -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
   -1565, 11168, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
   -1565, -1565, -1565, 14578, -1565, -1565,   519, -1565, -1565, -1565,
    -144, -1565, -1565,   720, 14578,   636, -1565,   495, -1565, -1565,
     728, 14578,   718, -1565,   733,   -85, -1565, -1565, -1565, -1565,
   -1565, -1565, -1565,  1471,   551, -1565, -1565, -1565, -1565, -1565,
    1136,   519, -1565, -1565,  5294, -1565, -1565,   981, -1565, 10486,
      49, -1565, -1565, -1565, -1565,   875, -1565, -1565,  1820,   590,
   -1565, 11509,   -85, -1565, 11509, -1565, -1565, -1565, -1565, -1565,
   -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
   -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
   -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
   -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
   -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
   -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
   -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
   -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
   -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
   -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
   -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
   -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
   -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
   -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
   -1565, -1565, -1565, -1565, -1565, -1565, -1565,     8,   566, -1565,
   -1565,   364, 14578, -1565, -1565,   577,   624, -1565,   675, 11509,
   -1565, 11509,    91,     3, -1565, -1565,   783, 14578,   636,   671,
   11509, -1565,   905, 14578, 14578,   718, -1565,   827,  1010,   777,
   -1565, -1565,   715,   699, -1565, -1565, 10486,   708,   753,  7586,
   -1565,  5294,   760,   762,   776,   785,   798,   805,   813,   814,
     814,   818,   825,   829,   305,   839, -1565,   849,   854,   861,
     863, -1565, -1565,   865, -1565,   878,   879,   885,  5676,   107,
     894, -1565,   896,   911,   920,   924,   937,   944,  5294,   953,
   -1565, -1565,   968,   970,   979,   980,   992,  1024,  1032,  1033,
    1037,  1046,  1048,  1051,  1054, -1565,   664,  1060,  1066,  1068,
   -1565,  1079, -1565,  1080,   814,   814,  1095,  1096,  1098,  1116,
    1125,  1128,  7586,  7586,  7586,  5294, 14578, 14578, -1565,    58,
    1050, -1565,   993,    67, -1565, -1565, -1565, -1565, -1565, -1565,
     781, -1565, -1565, -1565, -1565,  1129, -1565,   723,  1044, -1565,
   11509, 11509, -1565, -1565, -1565, -1565,   986,     3,   942, -1565,
   -1565, -1565, -1565, -1565, -1565, -1565,   739,   875, -1565, -1565,
   -1565, -1565,    68,  1134,    29,    -5,  1143, -1565, -1565, -1565,
   -1565, -1565, -1565, -1565,    68,  1085,  1084,  1143,    68, -1565,
   -1565, -1565,  1225,  -110,  1461, -1565, -1565, -1565,  1265, -1565,
    1152,   478, -1565,   429, 13214, 14578,  1448, 11509,  1160, -1565,
    1160, -1565,  1513,  1309, -1565, -1565,  1441, -1565, 11509, -1565,
    1412, -1565, -1565, -1565,  1352, 11509,  1317, -1565,  1239, -1565,
    1364,  1136, 11509, -1565,  5294,   170, -1565,    58,  1267,  5294,
    1221,  5294,  5294,  5294,  5294,   591,  1227, -1565, -1565,  1238,
    5294,  5294,  5294,   781,  5294, 11850,  5294, -1565,  2312,  1535,
    5294,  5294,  5294,  5294,  1945,  1250,  5294,   626,  5294,  5294,
     713,  5294,  5294, -1565,  3766, -1565,  7586,  5294,  5294,  5294,
    5294,  5294,  5294,  1623,  1623,  5294,  5294,   761,  3766, -1565,
    5294,   781,  2147,  2147,  3002,  5294,  1253, -1565, -1565,  1261,
   11850,  1623,  1623,  5294,  5294, -1565, -1565, -1565,    42,  1262,
     464,  5294,  1255,  1337,  5294,  5294,  5294,  7586, -1565,  1141,
   -1565, -1565, -1565, -1565, -1565,  6058,  7586,  7586,  1266,  7586,
    7586,   779,  7586,  7586,  7586,  7586,  7586,  7968,  8350,  7586,
    7586,  7586,  7586,   579,  7586, -1565, -1565, 14578, -1565,  1487,
    1263, -1565,  1332, -1565,  1283, -1565,   981, -1565,   888, -1565,
   -1565, -1565,  1270, -1565,  5294, -1565,  1272, -1565,  1047, -1565,
   -1565, 11509,  1284,   636, 11509,  1347,  5294, -1565,   -10, -1565,
   -1565,    -5, 14578,    68,    -5, -1565, 11509, -1565,    -5,    -5,
    1379, 11850, -1565, -1565, -1565, -1565,   734, -1565, -1565, -1565,
   -1565, -1565, -1565, -1565, -1565, -1565, -1565,  1614, -1565, 11509,
   -1565, -1565, -1565,  1120, -1565, 14578,  1310, 11509,  1160, -1565,
   -1565, -1565,  1483, -1565,  1160, -1565,  1283,    88, -1565,  1623,
    1288, -1565,  5294,   -23,    70, -1565,    58,   771,   788,   188,
      89, -1565,  1289,  1297, -1565, -1565,    92,    98,   261,   275,
    1292,   284,  1293, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
   -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
   -1565, -1565, -1565,  1545, -1565, -1565, -1565,   302,   129,   144,
      43,  1305,  5294,   150,  1623,  1298,   313,   317,  1623,  1300,
     156,   319,   326,  1362,   343,   159,   171,   181,   187,   345,
    1301,  1302,   190,    47,  1623,  1303,   360,   369, -1565,  1304,
   -1565,  1306,  4148,  4530,  4912,   134,   204, -1565, -1565,  1308,
    1313,  1314,    56,   374, -1565,  5294, -1565, -1565,   486, -1565,
    1356,    84, 14578,    18, -1565,  1411, -1565,    33,  1678, -1565,
   -1565,   609, -1565, -1565, -1565, -1565,  1321, -1565,  1322,   704,
    1320,  5294,    10,  1320,  7586,  1323,  7586,  7586,  1666,  1745,
     939,  1403,  1403,  5676,   -13,  5676,   -13,  1320,  1320,  1320,
    1327, -1565, -1565, -1565,    93,  1325,  1324,  1399, 11509, 11509,
   -1565, -1565,  1333,  1333, 10126, -1565, -1565, -1565, -1565, -1565,
   -1565, -1565, -1565,   563,    58, -1565,  1330, -1565, -1565, -1565,
   -1565, -1565,   773,  1326, -1565, -1565,    68,  1336, 14578, -1565,
   -1565,    58, -1565, -1565, -1565, -1565,   -74, -1565, -1565,    -5,
   -1565,    68, -1565, -1565, -1565,  1334, -1565,   -89,  1686,  1143,
   -1565, -1565,   -89,  2224, -1565,   -89, -1565, -1565,  1338, -1565,
     -89,  1124, -1565, -1565,  1501, -1565, -1565,  1510,  6440,  1346,
   -1565,  5294,   401,  5294,  5294,  1635,  1259, -1565,  5294, -1565,
   -1565,  1259,  5294, -1565,  1360,  1585,  1586, -1565, -1565, -1565,
    1239, -1565,  5294,  5294, -1565,  5294,  5294,  5294,  5294,   386,
    5294,  1363, -1565, -1565, -1565,  1365, -1565,  5294, -1565, -1565,
    5294, -1565,  5294,  5294,  5294,  5294, -1565, -1565, -1565,  6822,
    5294,  5294,  1368, -1565, -1565, -1565,  5294,  5294,  5294,   488,
    5294,   703,  5294,   726,  5294, -1565,  5294, -1565, -1565, -1565,
   -1565,  5294, -1565,   811, -1565, -1565,   875, -1565,   486,  1369,
     888, -1565, -1565, -1565,  1366,  1367,  1370,  1471,  5294, -1565,
   -1565, -1565, -1565, -1565,  7586,    73,  1372,  7586, -1565,   746,
    5294,    10,  1666,  1945,  1945,  5294,  1375,  1361, -1565, 14578,
   -1565, -1565, -1565, -1565,   875, 11850, -1565,  1373, -1565,  1373,
    1123,  1378,   823, -1565, -1565, -1565,   942,   429, -1565,  -125,
   -1565,    63,  1605,  1143, -1565,  1634, 13555, -1565, 12532, -1565,
    1603, -1565,   -89,  1380, -1565,    -5,   -31, -1565,   -10,   -10,
   -1565,  1310, 11850,  1310, -1565, -1565,  7204, -1565, -1565, -1565,
      35,  1088,  1718,   -89,  1718,   -89,  1450,  1607,  1475,   719,
    1619,   -89,   -89, -1565,  1478,  1718,  1755,    34,   -89,   -89,
   -1565,  1404, -1565, -1565,  1742, -1565,  1408, 10827, -1565, -1565,
     -89,    25,   584,  1850, 11509,  9444, -1565,  1404,   564, -1565,
     199,  1456,  1580,  5294,  1624, -1565,  1705, -1565,  5676,   430,
   -1565,    58,  5294,    58,   548, -1565,  1426, -1565,  1426, -1565,
   -1565,  1432, -1565,  1655, -1565,  1658,  1442,    58,  1449,  1438,
   -1565,  5294,  5294, -1565,   470,   -90,   207,   220,    75, -1565,
   -1565,   479, -1565, -1565,   523,   539,   541,   230,   545,   840,
    5676,   549,   175,    81, -1565,   245,   253,   553,  5294,   599,
    5294,   606,  5294,   607,   611,   638, -1565,   739, -1565,  1455,
   -1565, -1565, -1565, -1565, -1565,  1446,    58,  1457, -1565, -1565,
    5294, -1565,    67,  7586,    87,  1459, -1565, -1565, -1565,    58,
   -1565, -1565, -1565,   348,   739,  1462, -1565,   -89,  7204,  1333,
    1579, -1565, -1565,  1123, 11850, -1565, -1565, -1565, -1565, 14578,
    1258,  1310, -1565,  1465, -1565, -1565, 11509, -1565,    55,  1517,
    1693, -1565, -1565, -1565, -1565, -1565, 13896, -1565, -1565, -1565,
   -1565,  1469,  1683,  3384,    18,   -89, -1565, -1565, -1565, -1565,
   -1565, -1565, -1565,  1501,   849,    58, -1565,  5294, -1565, -1565,
   -1565, -1565, -1565, 12191, -1565, -1565, -1565,   296,   543, 12191,
   -1565, -1565, 12191,   499, 12191,  1347,    35, -1565, -1565,  1698,
    1700, 12191, 12191, -1565,   579,  1347, -1565, 12191, 11850, -1565,
   -1565, 11509,    57,   940, -1565,   -89,  2277,  9785, 14578,   148,
      83, -1565,  1485,  1850, -1565, -1565, -1565, -1565, -1565, -1565,
   -1565, -1565, -1565, -1565, -1565,  1479,  1490, -1565,   901, -1565,
   -1565, -1565, -1565, -1565,  1853,  1857, -1565,   526,  1386, -1565,
     210, -1565, 11509,    58,   -10, -1565, -1565,  1945, -1565,    58,
    5294,  1661, -1565, -1565,  1662, -1565, -1565, -1565, -1565, -1565,
   -1565,  1495,  1945,  1945, -1565,  1608, -1565,  5294,  5294, -1565,
    5294, -1565, -1565, -1565, -1565,  5294, -1565, -1565,  1945, -1565,
    5294, -1565, -1565,  5294,  5294,  5294, -1565,   639, -1565,   646,
   -1565,   647, -1565, -1565, -1565,  1270, -1565,   464, 14578, -1565,
   -1565,   945, -1565, -1565,  5294, -1565, 13555, -1565, -1565, -1565,
    1722,  1270, 11850,  7204,  1500,  1498, -1565, -1565,   478, -1565,
   -1565, -1565,   547,  1725,  1726, -1565,  1667,   892,  1667,   547,
     547,  1758,  -125,   631, -1565,  1399,  1399, -1565,  1508, -1565,
   -1565,  1618, -1565, -1565, 12873,   990, -1565,  7586,   849, -1565,
      58, -1565,  1057,  5294,  1624,    58,  1512,   138, -1565, -1565,
    5294,  1731,  1732,    48, -1565, 12191, -1565, -1565, -1565,    95,
   -1565, -1565, -1565, -1565, 12191, -1565, -1565, -1565, -1565, -1565,
   -1565,  1518, -1565,   138, -1565, -1565, -1565, -1565, -1565, -1565,
   -1565, -1565, -1565, -1565, -1565,    35, -1565,   973, -1565, 14578,
   -1565, -1565, 11509, -1565, -1565,  1936, 14578, -1565, -1565,  1404,
   -1565,  1404,  9785, -1565, -1565,  1579, -1565, -1565, -1565, -1565,
   -1565,  1523, -1565,  1740,   601,    58,  1527,   974, -1565,  1528,
    1529,    96,  1531,   648,   274,   998,   650,   724,   651,   653,
     657,   662, -1565, -1565, -1565,    77, -1565, -1565, -1565,  1015,
   -1565, -1565,    77, -1565, -1565, -1565,  7204, -1565,  -134,  1501,
   -1565,   994, -1565, -1565, -1565,  1749,   547,  1667,  1667,  1750,
    1699,   370,  1883,  1780, -1565, -1565, 14578, -1565, -1565,   875,
   -1565, -1565,  1471,  1534, -1565,  1724, -1565,   153,  1684, -1565,
   -1565,   -89,    58, -1565,  5294, -1565, -1565, -1565,    58, 12191,
   -1565, -1565, -1565, -1565,  1546,  1839,  1841, 12191, -1565,  1936,
   11850, -1565, -1565, -1565, -1565,  1548,  1549, -1565, -1565,  1045,
    1236, -1565,  1432,  -103, -1565,  1432,  1549,  1432,  1552, -1565,
     918, -1565,   727, -1565,  1554, -1565,  1884,  1556,   917,   917,
    1557, -1565,    -3,   564, -1565,  1558,   942, 14578,  1602, -1565,
   -1565, -1565,  1733, -1565, -1565, -1565, -1565, -1565,  5294, -1565,
   -1565, -1565, -1565, -1565, -1565, -1565,    63,    63, -1565, -1565,
    1637, -1565, -1565,   888,  1504,  1143,  -125, -1565,   547, -1565,
    1783,  1787,   547, -1565, -1565, -1565,  5294, -1565,  1501, -1565,
   -1565,   -16,   739,  1895, -1565,  1508, -1565, -1565, -1565, -1565,
    3384,   138,  1572,  1574, 14578, -1565,  1398,   148,  1884, -1565,
   14578,  1770,  1744,  -105,  1254,  1260,   579,  1398,  1748, -1565,
   -1565,  1794, -1565, -1565, -1565, -1565,   918, -1565,  1795,  1094,
    1632, -1565, -1565, -1565,  1236, -1565,  1254,   917,  1552,  1756,
   -1565, -1565,  1592,   917,  -105,   917, -1565, -1565,  1254,  1762,
   -1565,  1254,  1763,  1347, -1565,  1764, -1565,  1016,  1891, -1565,
   -1565, -1565,   917, -1565, -1565, 14578, -1565, -1565, -1565, -1565,
    1146,   138,  1600, 11850,  1601,   665, -1565, -1565, 11509, -1565,
   -1565, -1565,  1465,  1181,  1642,   547,   547,  1286,  1644,  5294,
    5294,  1604,  -146,   138,  5294,  1143, -1565,    14,  1270,    13,
    1814, -1565, -1565, -1565, 14578, 14578,  1147,   781,   781,   870,
     870, -1565, -1565, -1565, -1565,  1168, -1565,    19,  1778,  1616,
   -1565, -1565,  1639,  1398, -1565, -1565,   570, -1565, -1565, -1565,
   -1565, -1565, -1565, -1565, -1565,   781,   814, -1565,   781,   781,
   -1565, -1565,  1786, -1565, -1565, -1565,  1178,    96, -1565,  1639,
   -1565,  1617,  1622, -1565,  1625, -1565, -1565, -1565,   870,   870,
     781, -1565, -1565,  1656, -1565, -1565,    -3,    27, 14578, -1565,
    1792,  1621, -1565,   -89, -1565, -1565, -1565,   464, -1565,  1668,
   -1565, -1565, -1565,  1669,    58,    58, 14578, -1565,  5294, -1565,
   -1565,    58, -1565, -1565, -1565,    48,    48, -1565,    14,    77,
    1977,  1980,  1910,  1982,    13, -1565,   -59, -1565,  1180,  1189,
      27, -1565, -1565,   148, -1565, 14578, -1565, -1565, -1565,     6,
   -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565,   781,
   -1565,   781,   814, -1565,  1198, -1565, -1565,  1236, -1565,   918,
   -1565, -1565, -1565, -1565,  1347,   -89, -1565,    27, -1565, -1565,
     138,  1636, 11850,  7204,  1895, -1565,  5294,  1638,  5294,  1641,
    1202, -1565,   138,  1892,  1892, -1565, -1565,    96,    96,  1994,
      96, -1565,  1995,  1997,   -59, -1565,  1799,    27, -1565, -1565,
   -1565,   575,   575, -1565,   918,    96, -1565,  1254, -1565, -1565,
      23, -1565, -1565, -1565, -1565, -1565,  1814,  1501,    58, 14578,
      58, 14578, -1565, 14578, -1565,   871, -1565, -1565, -1565, -1565,
      96, -1565,    96,    96, -1565, -1565, -1565, -1565,  2026, -1565,
      59,  1832,  1834, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
   -1565, -1565,  1901,  1143,  1208,  1213, -1565,  2019, -1565,  2020,
    1674,  1676, -1565, -1565, -1565, -1565, -1565, -1565,  1728,  1970,
    1852,  1687, -1565, -1565, -1565, -1565, -1565, 14237, 14237,   575,
     575,   -22,  8732,  1791, -1565,  1690, -1565,  1689, -1565,  1234,
   -1565, -1565, -1565, -1565, -1565, -1565,  1471,  1248, -1565, -1565,
   11850, -1565, -1565, 14237, -1565, -1565, -1565,  9092,  1621, -1565,
   -1565
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
     424, -1565, -1565,   958,  -754,  -826, -1565, -1565, -1565,   962,
   -1565, -1565,  -225, -1074,   975, -1002,  -861, -1565, -1565,  1394,
   -1565, -1565, -1506,   774,   568,   982,  1006,   778,   816, -1565,
   -1565,   484, -1565,   489, -1565, -1565, -1565, -1565,  -287,  -893,
     957,   358,   480,   256, -1048,  -804,  -752, -1514, -1565, -1564,
   -1565,   378,   -94, -1565,   346, -1541, -1565,   388,   415,   410,
   -1163,  -348, -1565,   154, -1565, -1565, -1565, -1565,  -915, -1565,
   -1565, -1075, -1565, -1565, -1565, -1565, -1320, -1168, -1565,  1011,
     180, -1565,  -828,   265, -1235, -1565, -1565, -1565, -1565, -1565,
   -1565,   249, -1565, -1565,   832, -1565, -1565, -1565,   452,   152,
   -1540, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
   -1565,  1291,  1671,  1093,  1864,  1144, -1565, -1565,  1869, -1565,
   -1565, -1565,  -738,  -971, -1565, -1565, -1565,  1709,  -875, -1154,
    1285,  -743,  -790,   743,  -327,  1276, -1565,  -564,  1949, -1565,
   -1565,  -304, -1565, -1565, -1565, -1565, -1565, -1565, -1565,   941,
   -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
    1908, -1565,  1219,  -491, -1565, -1565, -1565,  -878, -1565,   721,
     475, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
    -830, -1565, -1565, -1565, -1565, -1565,  1157, -1565, -1565, -1565,
   -1565, -1565, -1565, -1565, -1102, -1565,  -457,   191, -1565,   262,
   -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565,    76,   104,
   -1060,  -522,  1609,   255, -1565,   490,  1648,  -177, -1565, -1565,
   -1565,  1145, -1565, -1565, -1565, -1565, -1565,   569,  -907,  -631,
   -1565, -1565, -1443,  -419, -1045,  -405, -1565,  -954,   714, -1565,
    -627, -1565, -1565, -1565,   481,  1121, -1565, -1565, -1565, -1565,
   -1565,   356,   -87,  1873, -1565, -1565, -1565, -1565, -1565,  1241,
   -1565, -1565, -1565, -1565, -1565, -1565,  1858,  1761,  1488, -1565,
   -1565,  -881, -1565, -1565,  1335, -1565,   775,  -884,  -895,   966,
   -1565, -1565, -1565, -1202, -1565, -1565, -1565, -1565, -1565,  1114,
     109,   238, -1565, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
    -117,   304,  -587, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
   -1565, -1565,  1691, -1565, -1565, -1565,  1842, -1565, -1565, -1565,
   -1565, -1565, -1565, -1565, -1565, -1565, -1565,   239, -1565,   291,
     200, -1565,   224, -1565, -1565, -1565, -1565,   114, -1565,  -323,
   -1481, -1551, -1559, -1565, -1565, -1565,    90,   978,  1569, -1529,
    -476, -1565,  -244,   -50,   617,  2124,   -12,    -7,  -391,  -512,
     100,  -607,   893, -1565, -1565, -1565, -1565, -1565, -1565, -1565,
    1341, -1565, -1565, -1565, -1565,   959, -1565, -1565, -1565,   -91,
    -845,   759, -1565,   514,  1171, -1565, -1565, -1565, -1565,   174,
    2105,  1789, -1565, -1565, -1565, -1565, -1565,   -78, -1022,  -424,
   -1565, -1382, -1565, -1565, -1565,  -982, -1360, -1565, -1565,  -497,
   -1565, -1565
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1198
static const yytype_int16 yytable[] =
{
      95,   284,   306,   518,   630,    96,   843,  1124,  1243,   111,
    1111,   523,   751,  1076,  1291,   506,  1416,   272,  1191,   615,
     707,   708,   422,   809,  1225,  1187,  1113,  1061,  1041,   419,
     722,   817,   545,  1395,   466,   899,  1557,  1546,   655,   272,
    1611,   650,  1101,  1332,  1468,   584,  1671,   559,  1299,   720,
    1655,   272,   650,   561,   584,   584,   613,   567,   568,   584,
    1411,   292,   272,  1214,   887,  1701,  1747,   890,   584,   272,
     584,   892,   893,   488,   468,  1057,   910,  1944,   923,  1257,
     597,    95,   584,   672,   926,   584,    96,   584,   575,   576,
     577,  1750,  1981,   584,   789,   111,   584,   272,   643,   584,
     584,   584,  1025, -1194,   584,  2025,   460,  1736,  -630,   272,
     584,   461,   272,   613,  1259,  1260,  1900,  1748,  1339,  1751,
    1268,   668,  1017,  1473,  1901,  1052,  1460,  1738,   677,   273,
     481,  -630,   615,   898,    86,  1434,    87,    88,  1893,   613,
    1068,   584,  1227,  1314,  2046,  2026,   584,  1812,  1604,  1916,
    1004,   286,  1435,  1595,  2072,  1499,   584,  1793,  1812,  1221,
    2027,  1894,   584,   273,   102,    89,  1830,  1813,   584,   645,
     615,   584,   882,  1820,   295,  1485,   970,    86,  1253,   698,
      88,   295,   658,   584,   600,   897,  1074,   584,  1283,  1239,
    1285,  1605,  1945,   584,   110,  1673,  1005,   639,  1274,   584,
     584,   883,   584,  1302,  1697,  2028,  1049,  1294,  1295,   286,
    1887,  1554,   483,    72,  1275,  1303,   584,  1083,   288,   584,
      67,   286,   105,    68,   286,  1596,  1313,   640,  1821,   422,
    1603,  1972,   584,   669,  2029,  1902,   659,  1888,  2073,  1608,
    1101,  1518,   584,  1240,   290,  -205,  1674,  1917,   484,  1085,
     491,  1973,   648,   316,  1228,    78,  1229,   584,  1803,    84,
    1766,   900,   699,   984,  1812,   584,   585,   585,  1500,  2047,
     272,   585,  1924,   584,  1075,   276,  1739,   272,    90,   272,
     585,  1436,   585,  1703,   675,   272,   584,   584,   272,  1327,
    1006,   272,   272,   928,   585,   822,   584,   585,  1918,   585,
     482,  1380,  1070,    91,   272,   585,  1091,    92,   585,  1069,
     844,   585,   585,   585,   584,   884,   585,  1245,  1092,  1560,
    1258,  -630,   585,  1903,  1016,   584,   628,  1502,  1869,   584,
     924,   584,  1612,  1413,   422,   677,  1517,   644,   584,  1460,
    1982,    86,  1419,   103,    88,  1519,    91,  1315,  2030,   646,
    1890,  1895,  1300,   585,  1672,   584,  1454,   584,   585,  1962,
     609,   610,   611,   612,  1702,  1340,  1935,   426,   585,  1606,
     622,  1453,   584,  1979,   585,  1988,   614,    93,  1045,  1486,
     585,   584,   472,   585,   272,   272,   584,   277,  1601,   286,
    1522,   286,   586,  1966,  1041,   585,  1986,   273,   584,   585,
     286,   586,   586,   273,   494,   585,   586,   375,   272,   272,
    1495,   585,   585,   584,   585,   586,   286,   586,   878,  1406,
    2023,  1461,   794,   794,  1987,   795,   947,   678,   585,   586,
     971,   585,   586,   614,   586,  1641,   990,  1643,   684,   991,
     586,  1149,   584,   586,   585,  1101,   586,   586,   586,  1098,
    1198,   586,  1155,  1199,   585,  1369,  1200,   586,  1370, -1198,
    1099,  1382,   272,   272,  1383,   272,  1002,  1403,  1025,   585,
    1404,   918,   931,  1271,  1169,   935,   272,   585,  1256,  1698,
    1078,   936,   584,   272,  1943,   585,  2008,  2009,   586,  2011,
     272,   584,   805,   586,   650,  1408,   581,   582,   585,   585,
     584,  1207,  1208,   586,  2024,   910,  1323,  1992,   585,   586,
      91,  1327,   945,   272,   985,   586,  1197,  1955,   586,  2004,
     286,   286,  1021,  1205,  1366,    74,   585,   946,  1462,  2042,
     586,  2043,  2044,   950,   586,   584,   889,   585,  1798,   957,
     586,   585,   962,   585,  1561,    79,   586,   586,   858,   586,
     585,   584,  1571,   584,   963,  1381,    75,   584,   272,  1217,
     584,   584,   107,   586,   964,   584,   586,   585,   930,   585,
     965,    95,  1427,   969,   673,   674,    96,   286,  1333,   586,
    1746,  1317,  1338,  1516,   585,  1684,  1460,   986,   286,   586,
    1367,   876,   904,   585,   879,   286,   897,  1409,   585,  1570,
     698,   841,   286,  1368,   586,   272,   891,  1061,  1692,  2017,
     585,   584,   586,  1375,   111,   116,   375,  1178,   584,   584,
     586,  1318,   630,   584,  1769,   585,  1463,    63,  1384,   272,
    1781,  -843,   272,   586,   586,   698,  1385,   915,   880,  1402,
     272,   937,   119,   586,   272,  1575,   271,   480,   120,   272,
     584,   584,  1675, -1197,   585,   938,  1470,  1658,   584,   584,
     584,   586,   584,   584,   940,   584,   123,   272,  -113,   584,
     -45,   841,   586,   272,   584,   272,   586,   584,   586,  1271,
     287,  1319,   944,   711,   522,   586,  1507,  1478,  1925,  1327,
    1845,  1779,  1049,   953,   585,  -667,    22,   954,   279,   958,
      64, -1197,   586,   585,   586,    65,   959,  1700,  1685,  1401,
    1320,  1334,   585,  1202,  1132,   584,  1014,   846,   754,   586,
    1050,    86,   698,   961,    88,   966,    86,  1009,   586,    88,
    1740,   797,   292,   586,   866,  1740,  1679,  -667,   584,   280,
     974,   286,  1978,  -843,   286,   586,   641,   585,  1045,   975,
    1892,  1995,   888,   997,   992,  -667,   286,  -667,  1203,  1065,
     586,  1645,   656,   585,  -667,   585,  1160,  -667,  1272,   585,
     698,    64,   585,   585,  1071,  -113,    65,   585,  -667,   286,
    1926,  1795,   296,  -667,  2018,   912,   898,   286,  -667,   586,
     272,  1776,  1777,   850,  -667,   -65,    76,   597,   622,  1033,
    1783,   824,  -667,   298,  1787,   758,  -667,   628,    30,  2016,
    1348,  -667,  1729,  1335,  1227,  1723,  1731,   780,   780,  1010,
    1963,  1964,  1483,   585,  1321,  1524,  1723,  2019,  1731,   586,
     585,   585,  1180,  -667,   905,   585,   272,   272,   586,   597,
    1529,  1530,   272,   798,  1289,  1740,  -667,   586,  1784,  2020,
    1364,   688,  1788,   774,  -667,  1182,  1537,   300,   694,  1371,
      95,  1350,   585,   585,   302,   998,   272,  1724,  1732,  1274,
     585,   585,   585,  -113,   585,   585,  1806,   585,  1724,  1535,
    1732,   585,   586,   304,   429,  1275,   585,  -667,  1271,   585,
      91,   841,  1927,  -207,  1261,    91,  1263,   859,   586,   841,
     586,   600,  1003,  1372,   586,  1515,   307,   586,   586,   648,
    1824,  1827,   586,  1549,   487,    86,  1195,    87,    88,  1373,
     493,  1374,  1723,  1580,  1581,  1376,  1228,   585,  -667,  1379,
    1731,  1011,  1824,  1386,  1718,  1756,  1217,  1719,   825,  2034,
    1012,  2035,  1730,   600,  1824,   464,   471,  1824,   286,   286,
     585,  1290,   336,   826,   866,  1880,  1881,   898,   586,  1720,
    -667,   475,  1825,  1828,  1716,   586,   586,   430,  -667,  -667,
     586,  1569,  1255,   948,  1724,   375,  1908,  1909,  1067,  1388,
     860,  1649,  1732,   477,  1825,  -667,  1390,  1392,  2052,  1953,
     486,  1393,   342,  1487,  1488,    95,  1825,   586,   586,  1825,
      96,   489,  1757,  1717,  2037,   586,   586,   586,  -962,   586,
     586,  -667,   586,  -667,  -667,   596,   586,   272,  1394,  1542,
     492,   586,   827,   272,   586,  1489,  1543,  1544,  1657,  1459,
    1660,  1662,   597,  1663,  2038,  1459,   496,  1664,  1469,  1490,
    1471,  1855,  1665,   560,   272,  1875,   272,  1476,  1477,  1248,
    1990,    95,   351,  1480,  1431,  1566,    96,  1721,  1423,    90,
     272,  1491,   586,  1567,  1324,   497,    81,  2022,   841,   499,
     500,   603,   604,   605,   606,   607,   608,   609,   610,   611,
     612,   361,   501,  -207,  -207,   586,   597,   504,    92,   841,
    2033,  1716,   375,  1060,  2039,   272,   948,    82,   336,   618,
     841,   841,   272,   272,  1661,   841,  1749,   617,   841,  1271,
    1680,  1681,   631,   603,   604,   605,   606,   607,   608,   609,
     610,   611,   612,    86,  1718,  1718,    88,  1719,  1719,  1212,
    1717,  -112,   505,  1731,  1483,  1723,   600,  1272,   342,   509,
     672,   510,  1083,   506,  2070,  2071,  1424,  1273,  1568,  1720,
    1720,   927,   598,  1856,   928,   511,  1249,  1425,  1251,   587,
     431,   432,   433,   434,   512,  1426,  -205,   599,   929,  1835,
    -205,   928,   435,    69,  1085,    71,   336,   513,  -271,   868,
    1723,   588,  1492,  1824,   514,  1732,  1433,  1724,  1758,   108,
     600,  1186,   515,   516,   928,   382,  1427,   519,   351,    86,
     601,    87,    88,  1223,   520,   675,  1224,  1312,   521,   675,
     589,  -207,   272,  -112,   286,  1312,   342,   272,   524,   590,
    1377,  1459,   630,   928,   272,   869,  1857,   361,   525,  1587,
    1459,  1091,  1724,   526,   272,  1825,   602,  1588,  1274,  1442,
     527,    64,   528,  1092,   529,  1423,    65,  1721,  1721,   625,
    1428,  1484,  1718,   591,  1275,  1719,   305,   530,   531,   810,
    1589,   272,    86,   870,   532,    88,    86,   272,   309,    88,
     272,   628,   272,   536,   841,   537,   351,  1720,  1472,   272,
     272,  1511,   841,  1136,  1512,   272,   272,  1137,  1479,   272,
     538,  1429,  1521,   467,   841,   272,   272,  1873,  1114,   539,
    1138,   460,  1836,   540,  1837,   361,   461, -1198,   605,   606,
     607,   608,   609,   610,   611,   612,   541,   310,   648,   871,
    -112,  1729,  1423,   542,   866,  1548,   841,   375,   928,  1422,
     272,  1139,   544,  1424,  1140,  1737,   286,    83,  1743,  1823,
    1745,   382,  1141,    90,  1425,  1826,  1443,   546,   811,   547,
    1423,   812,  1426,  1613,  1651,  1459,  1512,  1652,   548,   549,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
     905,   550,    92,  1312,  1098,  1721,  1677,   653,  1659,  1312,
     399,   928,  1312,  1427,  1312,  1099,   272,  1858,  1859,   654,
     850,  1312,  1312,  1722,   272,  1669,  1878,  1312,   928,  1248,
     272,   286,  -207,   551,  1083,   375,  1839,  1312,  1498,  1838,
    1424,   552,   553,   592,   593,   594,   554,  1142,  -112,   382,
    -207,  1425,  1083,   320,  -138,   555,  -207,   556,   311,  1426,
     557,    91,   272,   558,  1860,    91,  1085,  1428,  1424,   562,
      95,   312,   286,  1718,    68,   563,  1719,   564,   615,  1425,
    -205,  1730,  1615,   272,  1085,   597,   672,  1426,   565,   566,
    1427,  1718,   272,   813,  1719,   852,   853,  1718,  1720,   615,
    1719,   675,   814,  1807,   569,   570,  1873,   571,  1429,    86,
     336,    87,    88,   841,   523,   561,  1720,   272,  1427,   675,
     272,  1694,  1720,  1091,   272,   572,   597,   841,  1547,   841,
     272,  1882,  1220,  1115,   573,  1092,  1249,   574,   616,  1930,
      89,  1091,  1929,   642,  1428,  1931,   341,   657,  1575,   660,
     342,   960,    86,  1092,    87,    88,  1867,  1910,    86,  1868,
    1868,    88,   662,   675,  1282,   344,  1284,  1143,   615,   661,
     615,  1292,  1428,   679,  1212,  1764,    86,  1297,  1914,    88,
     850,  1915,   680,    89,   681,  1429,   682,   851,  1933,   600,
    1976,  1652,   685,  1868,   272,  1312,  1721,   687,  -667,  1977,
      95,   689,  1868,  1144,  1312,    96,   672,   672,  1984,   672,
     351,  1985,  2002,  1429,  1721,  2003,  2079,   272,  2053,   690,
    1721,  2003,  1145,  2054,   672,   272,  2003,   507,   272,  1614,
     600,   705,   286,   692,  1873,  1983,  1640,   714,   360,   361,
    -667,  2079,  1312,    86,  2084,    87,    88,  2083,   715,   672,
     702,   672,   672,    90,   534,   852,   853,   745,  2086,   752,
    -667,  2087,   698,   787,   543,   272,  1098,  -667,   847,   802,
    -667,   788,   796,  1844,    89,   821,   848,  1099,    91,  1848,
     849,  1850,    92,   862,  1098,   867,  -667,   877,    91,   894,
     902,  -667,   854,   916,   913,  1099,    90,  -667,   920,   933,
     934,   578,   939,   941,   942,  -667,  1689,   948,   952,  -667,
     956,   967,   968,   973,  -667,  1000,  1008,   976,   987,   977,
     584,    91,   272,   988,   989,    92,   612,    91,   272,  1312,
    -420,  1013,  1020, -1198,   841,  -510,  -667,  1312,  1029,   375,
    1030,  1063,  1036,  1808,  1058,    91,  1066,  1072,  1077,  -667,
     380,  1112,   803,   382,   898,  1126,  1130,  -667,  1876,   603,
     604,   605,   606,   607,   608,   609,   610,   611,   612,  1135,
    1150,  1151,  1152,  1162,  1211,  1163,  1853,  1772,  1174,  1190,
    1192,  1193,  1201,   272,  1194,  1210,  1219,    90,  1222,   597,
    -667,   272,  1050,  1049,  1252,  1254,   272,  1272,  1286,  1809,
    1810,  1287, -1198, -1198,   607,   608,   609,   610,   611,   612,
     697,  1288,    91,  1293,  1296,   704,    92,  1298,  1305,   709,
     710,  1306,   272,   272,  1340,  1342,   716,   717,   718,  1344,
     719,  -667,   721,  1346,  1772,  1351,   747,   748,   749,   750,
    1816,  1354,   753,  1357,   756,   757,  1358,   760,   761,  1841,
     762,   928,  1359,   764,   765,   766,   767,   768,   769,  1360,
    1398,   772,   773,  -667,   776,  1396,   777,  1400,   597,  1405,
     785,   786,  -667,     1,     2,  1412,   850,  1862,  1432,   792,
     793,  1440,  1441,  1444,     3,  1445,   272,   801,  -667,  1509,
     806,   807,   808,   600,  1474,  1865,  1475,  1513,   442,  1506,
    1510,  1514,  1526,  1527,   272,  1528,     4,  1531,   286,  1551,
    1555,  1556,   443,   444,  -667,     5,  -667,  -667,  1562,  1563,
     445,  1572,  1564,  1582,  1585,  1594,     6,  1599,  1600,  1911,
    1912,  1610,  1647,   272,  1772,  1772,  1648,  1650,  1653,  1654,
     864,  1656,  1678,  1682,  1683,  1686,  1687,  1695,  1696,     7,
       8,     9,   881,  1699,  1705,  1704,  1706,  1710,  1712,    10,
     446,  1739,  1317,  1752,  1753,  1755,  1773,  1989,  1768,    11,
     272,  1765,   600,  1799,  1774,  1778,  1785,    12,  1940,  1941,
    1786,  1804,   447,  1805,  1817,  1819,    13,   584,  1831,  1616,
    1832,  1834,    86,  1617,  1618,    88,  1842,  1846,  1772,   448,
     449,  1847,  1318,  1851,  1852,  1854,  1863,  1619,   922,  1870,
    -585,  1874,  -590,  1886,   450,  2085,  1961,   272,    14,   272,
    1906,   272,    86,  1919,  1932,    88,  1921,  1937,   451,  1942,
      15,  1922,  1938,  1951,  1952,  1939,  1957,  1959,  1620,  1967,
      16,  1621,  1968,  1969,  1970,  1980,  1993,  1999,  2005,  1622,
    2001,   723,   724,   725,   726,   727,  2010,  2012,   949,  2013,
    2045,  1623,  1319,   603,   604,   605,   606,   607,   608,   609,
     610,   611,   612,  1502,  1624,   272,   272,  2048,  2050,  2049,
     272,  2055,  2056,  2057,   452,  2058,  2060,  1625,   979,   981,
     983,  1320,  2059,  2061,    95,  2080,  2062,  1767,   272,    96,
    2082,   272,  2083,  1325,  1316,   272,  1304,   909,    17,    18,
    1644,  1497,    19,   728,   729,   730,   731,  1276,  1503,  1309,
      20,  1707,   453,  1464,  1626,  1353,    21,  1015,  1708,  1961,
      22,  1961,  1849,  2036,  1833,  1936,  1744,    23,  1864,  1023,
      24,  1024, -1198,   604,   605,   606,   607,   608,   609,   610,
     611,   612,  1843,  1814,    25,   454,  1829,  1991,   455,   456,
    2015,    91,  1341,  1950,   732,   733,   734,   735,  1496,  1032,
    1226,   470,  1189,   736,   667,   469,   635,  1048,  1627,  1550,
    1148,  1782,  1407,  1574,  1188,  2007,    26,  2068,  2068,  2089,
    1965,    91,  2069,   713,  1780,    27,  1206,    28,  1593,   585,
      29,   457,   781,  1242,   503,  1321,  1801,  1628,   495,  1709,
     737,  1153,   623,  2068,   917,  1417,  1262,  1553,  1039,  2088,
    1994,   535,   693,  1996,  1129,  1971,  2032,  1131,  2014,  1133,
    1134,  2090,  1420,   865,  1147,   301,  1508,  1629,  1059,   738,
     739,  1591,    30,  1452,  1802,   427,   627,     0,  1154,     0,
       0,  1156,  1157,  1158,  1159,     0,  1161,   727,     0,  1081,
       0,     0,     0,  1164,  1082,     0,  1165,     0,  1166,  1167,
    1168,     0,  1083,     0,     0,  1171,  1172,  1173,  1630,     0,
    1631,  1632,  1175,  1176,  1177,     0,  1179,     0,  1181,     0,
    1183,     0,  1184,     0,  1084,     0,     0,  1185,     0,     0,
       0,     0,     0,     0,  1085,   778,     0,  1633,     0,     0,
    1634,  1635,  1081,     0,  1196,  1086,     0,  1082,   731,     0,
       0,     0,     0,     0,     0,  1083,  1204,   740,     0,     0,
       0,  1209,     0,     0,   586,   741,   742,     0,     0,   675,
       0,     0,     0,     0,  1087,  1088,     0,  1084,     0,     0,
    1089,     0,     0,     0,     0,     0,  1090,  1085,     0,     0,
    -267,  1091,     0,     0,     0,     0,   732,     0,  1086,   735,
       0,     0,     0,  1092,     0,   736,     0,     0,  1093,     0,
       0,     0,  1265,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   675,     0,     0,     0,    86,  1087,     0,    88,
       0,     0,  1094,  1089,     0,     0,     0,     0,     0,  1090,
       0,     0,   737,     0,  1091,     0,     0,     0,   723,   724,
     725,   726,   727,     0,     0,     0,  1092,     0,     0,  1343,
       0,  1093,     0,     0,  1347,     0,     0,     0,  1349,     0,
       0,     0,   739,     0,     0,     0,     0,     0,     0,    86,
    1095,     0,    88,   712,     0,     0,     0,  1362,  1363,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   755,  1378,  1096,   759,     0,
     728,   729,   730,   731,  1387,     0,  1389,     0,  1391,     0,
       0,   770,   771,     0,     0,   775,     0,     0,     0,     0,
    1097,     0,     0,  1095,  1098,     0,     0,     0,     0,   790,
     791,     0,     0,     0,     0,  1099,     0,     0,     0,     0,
       0,     0,     0,     0,  1265,   763,     0,     0,     0,   740,
    1096,   732,   733,   734,   735,     0,     0,     0,   742,     0,
     736,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1097,     0,     0,     0,  1098,     0,  1450,
       0,     0,     0,     0,     0,    91,     0,     0,  1099,     0,
       0,     0,     0,  1455,     0,   819,   820,   737,     0,   823,
       0,   828,   829,   830,   831,   832,   834,   836,   837,   838,
     839,   840,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   738,   739,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   919,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1525,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1533,  1534,     0,     0,     0,     0,     0,
       0,  1536,     0,     0,     0,     0,  1538,     0,     0,  1539,
    1540,  1541,   951,     0,   740,     0,   955,     0,     0,     0,
       0,     0,   741,   742,     0,     0,     0,     0,     0,     0,
       0,     0,   972,     0,     0,     0,     0,     0,     0,  1265,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1592,
       0,     0,     0,     0,     0,     0,  1598,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1019,     0,     0,  1022,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1265,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1455,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1775,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1455,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1450,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   125,     0,   317,   127,
     128,     0,     0,     0,     0,   129,     0,     0,     0,   130,
     131,   318,     0,   133,     0,     0,   319,   320,   134,     0,
     135,   136,   782,   137,     0,     0,     0,   138,   321,   322,
     323,   139,     0,   324,   140,     0,   141,   325,     0,   326,
     144,     0,   145,   146,   147,   148,   149,   150,     0,   151,
     152,     0,   153,   327,   328,  1884,  1885,   154,   329,   330,
    1891,   331,   155,   156,   157,   332,   333,   334,     0,     0,
       0,     0,   335,   160,   336,     0,     0,   337,     0,     0,
       0,     0,   161,   162,     0,     0,   163,     0,     0,   164,
     165,   166,     0,     0,   167,     0,   168,   169,     0,   170,
     171,     0,   172,     0,   173,   174,   338,   339,   175,   340,
     341,   176,   177,   178,   342,   179,     0,     0,     0,   180,
     181,     0,   182,     0,   183,     0,   343,   184,     0,   344,
       0,     0,     0,   345,    86,   186,     0,    88,   346,     0,
     187,   188,     0,     0,     0,     0,     0,   347,   348,     0,
       0,     0,     0,   189,  1455,     0,     0,   190,     0,   349,
     191,     0,   783,   350,   192,     0,     0,     0,     0,     0,
     193,   194,     0,   195,   351,     0,     0,   352,   196,   197,
     198,   353,     0,     0,   354,   355,   201,     0,   202,   356,
     357,   204,   205,   206,     0,     0,   207,   208,   209,   358,
     359,   210,   360,   361,     0,   211,   212,     0,   213,   214,
     215,   216,     0,     0,     0,     0,     0,     0,     0,  1265,
     217,   362,  1998,   218,  2000,   219,   220,   363,   222,     0,
       0,     0,     0,     0,   223,     0,     0,     0,     0,   224,
     364,   365,     0,     0,     0,   225,     0,     0,   366,   226,
     227,   228,   229,   230,   367,   232,     0,   368,   234,     0,
       0,     0,   235,   236,   237,   238,     0,     0,   239,     0,
     240,   241,   242,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   243,   244,   369,   370,   245,     0,     0,
     246,   371,   248,   372,   373,   249,   374,   250,   251,     0,
       0,   252,     0,   375,   253,     0,   254,   376,   377,   378,
       0,   784,   257,   379,   380,   381,   259,   382,   260,   261,
       0,     0,     0,   262,     0,     0,     0,     0,   383,     0,
       0,   384,   385,   386,   387,   265,     0,     0,   266,   388,
       0,   389,   390,   267,   268,     0,     0,     0,     0,   269,
       0,     0,     0,   391,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   392,   393,     0,     0,     0,     0,   394,
       0,   395,     0,   396,     0,     0,   397,   398,   125,     0,
     317,   127,   128,  1446,     0,     0,     0,   129,     0,     0,
       0,   130,   131,   318,     0,   133,     0,     0,  1447,   320,
     134,     0,   135,   136,     0,   137,     0,     0,     0,   138,
     321,   322,   323,   139,     0,   324,   140,     0,   141,   325,
       0,   326,   144,     0,   145,   146,   147,   148,   149,   150,
       0,   151,   152,     0,   153,   327,   328,     0,     0,   154,
     329,   330,     0,   331,   155,   156,   157,   332,   333,   334,
       0,     0,     0,     0,   335,   160,   336,     0,     0,  1448,
       0,     0,     0,     0,   161,   162,     0,     0,   163,     0,
       0,   164,   165,   166,     0,     0,   167,     0,   168,   169,
       0,   170,   171,     0,   172,     0,   173,   174,   338,   339,
     175,   340,   341,   176,   177,   178,   342,   179,     0,     0,
       0,   180,   181,     0,   182,     0,   183,     0,   343,   184,
       0,   344,     0,     0,     0,   345,    86,   186,     0,    88,
     346,     0,   187,   188,     0,     0,     0,     0,     0,   347,
     348,     0,     0,     0,     0,   189,     0,     0,     0,   190,
       0,   349,   191,     0,     0,   350,   192,     0,     0,     0,
       0,     0,   193,   194,     0,   195,   351,     0,     0,   352,
     196,   197,   198,   353,     0,     0,   354,   355,   201,     0,
     202,   356,   357,   204,   205,   206,     0,     0,   207,   208,
     209,   358,   359,   210,   360,   361,     0,   211,   212,  1449,
     213,   214,   215,   216,     0,     0,     0,     0,     0,     0,
       0,     0,   217,   362,     0,   218,     0,   219,   220,   363,
     222,     0,     0,     0,     0,     0,   223,     0,     0,     0,
       0,   224,   364,   365,     0,     0,     0,   225,     0,     0,
     366,   226,   227,   228,   229,   230,   367,   232,     0,   368,
     234,     0,     0,     0,   235,   236,   237,   238,     0,     0,
     239,     0,   240,   241,   242,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   243,   244,   369,   370,   245,
       0,     0,   246,   371,   248,   372,   373,   249,   374,   250,
     251,     0,     0,   252,     0,   375,   253,     0,   254,   376,
     377,   378,     0,     0,   257,   379,   380,   381,   259,   382,
     260,   261,     0,     0,     0,   262,     0,     0,     0,     0,
     383,     0,     0,   384,   385,   386,   387,   265,     0,     0,
     266,   388,     0,   389,   390,   267,   268,     0,     0,     0,
       0,   269,     0,     0,     0,   391,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   392,   393,     0,     0,     0,
       0,   394,     0,   395,     0,   396,     0,     0,   397,   398,
     125,     0,   317,   127,   128,     0,     0,     0,     0,   129,
       0,     0,     0,   130,   131,   318,     0,   133,     0,     0,
     319,   320,   134,     0,   135,   136,     0,   137,     0,     0,
       0,   138,   321,   322,   323,   139,     0,   324,   140,     0,
     141,   325,     0,   326,   144,     0,   145,   146,   147,   148,
     149,   150,     0,   151,   152,     0,   153,   327,   328,     0,
       0,   154,   329,   330,     0,   331,   155,   156,   157,   332,
     333,   334,     0,     0,     0,     0,   335,   160,   336,     0,
       0,   337,     0,     0,     0,     0,   161,   162,     0,     0,
     163,     0,     0,   164,   165,   166,     0,     0,   167,     0,
     168,   169,     0,   170,   171,     0,   172,     0,   173,   174,
     338,   339,   175,   340,   341,   176,   177,   178,   342,   179,
       0,     0,     0,   180,   181,     0,   182,     0,   183,     0,
     343,   184,     0,   344,     0,     0,     0,   345,    86,   186,
       0,    88,   346,     0,   187,   188,     0,     0,     0,     0,
       0,   347,   348,     0,     0,     0,     0,   189,     0,     0,
       0,   190,     0,   349,   191,     0,     0,   350,   192,     0,
       0,     0,     0,     0,   193,   194,     0,   195,   351,     0,
       0,   352,   196,   197,   198,   353,     0,     0,   354,   355,
     201,     0,   202,   356,   357,   204,   205,   206,     0,     0,
     207,   208,   209,   358,   359,   210,   360,   361,     0,   211,
     212,     0,   213,   214,   215,   216,     0,     0,     0,     0,
       0,     0,     0,     0,   217,   362,     0,   218,     0,   219,
     220,   363,   222,     0,     0,     0,     0,     0,   223,     0,
       0,     0,     0,   224,   364,   365,     0,     0,     0,   225,
       0,     0,   366,   226,   227,   228,   229,   230,   367,   232,
       0,   368,   234,     0,     0,     0,   235,   236,   237,   238,
       0,     0,   239,     0,   240,   241,   242,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   243,   244,   369,
     370,   245,     0,     0,   246,   371,   248,   372,   373,   249,
     374,   250,   251,     0,     0,   252,     0,   375,   253,     0,
     254,   376,   377,   378,     0,     0,   257,   379,   380,   381,
     259,   382,   260,   261,     0,     0,     0,   262,     0,     0,
       0,     0,   383,     0,     0,   384,   385,   386,   387,   265,
       0,     0,   266,   388,     0,   389,   390,   267,   268,     0,
       0,     0,     0,   269,     0,     0,     0,   391,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   392,   393,     0,
       0,     0,     0,   394,     0,   395,   714,   396,     0,     0,
     397,   398,   125,     0,   317,   127,   128,     0,     0,     0,
       0,   129,     0,     0,     0,   130,   131,   318,     0,   133,
       0,     0,   319,   320,   134,     0,   135,   136,     0,   137,
       0,     0,     0,   138,   321,   322,   323,   139,     0,   324,
     140,     0,   141,   325,     0,   326,   144,     0,   145,   146,
     147,   148,   149,   150,     0,   151,   152,     0,   153,   327,
     328,     0,     0,   154,   329,   330,     0,   331,   155,   156,
     157,   332,   333,   334,     0,     0,     0,     0,   335,   160,
     336,     0,     0,   337,     0,     0,     0,     0,   161,   162,
       0,     0,   163,     0,     0,   164,   165,   166,     0,     0,
     167,     0,   168,   169,     0,   170,   171,     0,   172,     0,
     173,   174,   338,   339,   175,   340,   341,   176,   177,   178,
     342,   179,     0,     0,     0,   180,   181,   978,   182,     0,
     183,     0,   343,   184,     0,   344,     0,     0,     0,   345,
      86,   186,     0,    88,   346,     0,   187,   188,     0,     0,
       0,     0,     0,   347,   348,     0,     0,     0,     0,   189,
       0,     0,     0,   190,     0,   349,   191,     0,     0,   350,
     192,     0,     0,     0,     0,     0,   193,   194,     0,   195,
     351,     0,     0,   352,   196,   197,   198,   353,     0,     0,
     354,   355,   201,     0,   202,   356,   357,   204,   205,   206,
       0,     0,   207,   208,   209,   358,   359,   210,   360,   361,
       0,   211,   212,     0,   213,   214,   215,   216,     0,     0,
       0,     0,     0,     0,     0,     0,   217,   362,     0,   218,
       0,   219,   220,   363,   222,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,   224,   364,   365,     0,     0,
       0,   225,     0,     0,   366,   226,   227,   228,   229,   230,
     367,   232,     0,   368,   234,     0,     0,     0,   235,   236,
     237,   238,     0,     0,   239,     0,   240,   241,   242,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   243,
     244,   369,   370,   245,     0,     0,   246,   371,   248,   372,
     373,   249,   374,   250,   251,     0,     0,   252,     0,   375,
     253,     0,   254,   376,   377,   378,     0,     0,   257,   379,
     380,   381,   259,   382,   260,   261,     0,     0,     0,   262,
       0,     0,     0,     0,   383,     0,     0,   384,   385,   386,
     387,   265,     0,     0,   266,   388,     0,   389,   390,   267,
     268,     0,     0,     0,     0,   269,     0,     0,     0,   391,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   392,
     393,     0,     0,     0,     0,   394,     0,   395,     0,   396,
       0,     0,   397,   398,   125,     0,   317,   127,   128,     0,
       0,     0,     0,   129,     0,     0,     0,   130,   131,   318,
       0,   133,     0,     0,   319,   320,   134,     0,   135,   136,
       0,   137,     0,     0,     0,   138,   321,   322,   323,   139,
       0,   324,   140,     0,   141,   325,     0,   326,   144,     0,
     145,   146,   147,   148,   149,   150,     0,   151,   152,     0,
     153,   327,   328,     0,     0,   154,   329,   330,     0,   331,
     155,   156,   157,   332,   333,   334,     0,     0,     0,     0,
     335,   160,   336,     0,     0,   337,     0,     0,     0,     0,
     161,   162,     0,     0,   163,     0,     0,   164,   165,   166,
       0,     0,   167,     0,   168,   169,     0,   170,   171,     0,
     172,     0,   173,   174,   338,   339,   175,   340,   341,   176,
     177,   178,   342,   179,     0,     0,     0,   180,   181,   980,
     182,     0,   183,     0,   343,   184,     0,   344,     0,     0,
       0,   345,    86,   186,     0,    88,   346,     0,   187,   188,
       0,     0,     0,     0,     0,   347,   348,     0,     0,     0,
       0,   189,     0,     0,     0,   190,     0,   349,   191,     0,
       0,   350,   192,     0,     0,     0,     0,     0,   193,   194,
       0,   195,   351,     0,     0,   352,   196,   197,   198,   353,
       0,     0,   354,   355,   201,     0,   202,   356,   357,   204,
     205,   206,     0,     0,   207,   208,   209,   358,   359,   210,
     360,   361,     0,   211,   212,     0,   213,   214,   215,   216,
       0,     0,     0,     0,     0,     0,     0,     0,   217,   362,
       0,   218,     0,   219,   220,   363,   222,     0,     0,     0,
       0,     0,   223,     0,     0,     0,     0,   224,   364,   365,
       0,     0,     0,   225,     0,     0,   366,   226,   227,   228,
     229,   230,   367,   232,     0,   368,   234,     0,     0,     0,
     235,   236,   237,   238,     0,     0,   239,     0,   240,   241,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   243,   244,   369,   370,   245,     0,     0,   246,   371,
     248,   372,   373,   249,   374,   250,   251,     0,     0,   252,
       0,   375,   253,     0,   254,   376,   377,   378,     0,     0,
     257,   379,   380,   381,   259,   382,   260,   261,     0,     0,
       0,   262,     0,     0,     0,     0,   383,     0,     0,   384,
     385,   386,   387,   265,     0,     0,   266,   388,     0,   389,
     390,   267,   268,     0,     0,     0,     0,   269,     0,     0,
       0,   391,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   392,   393,     0,     0,     0,     0,   394,     0,   395,
       0,   396,     0,     0,   397,   398,   125,     0,   317,   127,
     128,     0,     0,     0,     0,   129,     0,     0,     0,   130,
     131,   318,     0,   133,     0,     0,   319,   320,   134,     0,
     135,   136,     0,   137,     0,     0,     0,   138,   321,   322,
     323,   139,     0,   324,   140,     0,   141,   325,     0,   326,
     144,     0,   145,   146,   147,   148,   149,   150,     0,   151,
     152,     0,   153,   327,   328,     0,     0,   154,   329,   330,
       0,   331,   155,   156,   157,   332,   333,   334,     0,     0,
       0,     0,   335,   160,   336,     0,     0,   337,     0,     0,
       0,     0,   161,   162,     0,     0,   163,     0,     0,   164,
     165,   166,     0,     0,   167,     0,   168,   169,     0,   170,
     171,     0,   172,     0,   173,   174,   338,   339,   175,   340,
     341,   176,   177,   178,   342,   179,     0,     0,     0,   180,
     181,   982,   182,     0,   183,     0,   343,   184,     0,   344,
       0,     0,     0,   345,    86,   186,     0,    88,   346,     0,
     187,   188,     0,     0,     0,     0,     0,   347,   348,     0,
       0,     0,     0,   189,     0,     0,     0,   190,     0,   349,
     191,     0,     0,   350,   192,     0,     0,     0,     0,     0,
     193,   194,     0,   195,   351,     0,     0,   352,   196,   197,
     198,   353,     0,     0,   354,   355,   201,     0,   202,   356,
     357,   204,   205,   206,     0,     0,   207,   208,   209,   358,
     359,   210,   360,   361,     0,   211,   212,     0,   213,   214,
     215,   216,     0,     0,     0,     0,     0,     0,     0,     0,
     217,   362,     0,   218,     0,   219,   220,   363,   222,     0,
       0,     0,     0,     0,   223,     0,     0,     0,     0,   224,
     364,   365,     0,     0,     0,   225,     0,     0,   366,   226,
     227,   228,   229,   230,   367,   232,     0,   368,   234,     0,
       0,     0,   235,   236,   237,   238,     0,     0,   239,     0,
     240,   241,   242,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   243,   244,   369,   370,   245,     0,     0,
     246,   371,   248,   372,   373,   249,   374,   250,   251,     0,
       0,   252,     0,   375,   253,     0,   254,   376,   377,   378,
       0,     0,   257,   379,   380,   381,   259,   382,   260,   261,
       0,     0,     0,   262,     0,     0,     0,     0,   383,     0,
       0,   384,   385,   386,   387,   265,     0,     0,   266,   388,
       0,   389,   390,   267,   268,     0,     0,     0,     0,   269,
       0,     0,     0,   391,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   392,   393,     0,     0,     0,     0,   394,
       0,   395,     0,   396,     0,     0,   397,   398,   125,     0,
     317,   127,   128,     0,     0,     0,     0,   129,     0,     0,
       0,   130,   131,   318,     0,   133,     0,     0,   319,   320,
     134,     0,   135,   136,     0,   137,     0,     0,     0,   138,
     321,   322,   323,   139,     0,   324,   140,     0,   141,   325,
       0,   326,   144,     0,   145,   146,   147,   148,   149,   150,
       0,   151,   152,     0,   153,   327,   328,     0,     0,   154,
     329,   330,     0,   331,   155,   156,   157,   332,   333,   334,
       0,     0,     0,     0,   335,   160,   336,     0,     0,   337,
       0,     0,     0,     0,   161,   162,     0,     0,   163,     0,
       0,   164,   165,   166,     0,     0,   167,     0,   168,   169,
       0,   170,   171,     0,   172,     0,   173,   174,   338,   339,
     175,   340,   341,   176,   177,   178,   342,   179,     0,     0,
       0,   180,   181,     0,   182,     0,   183,     0,   343,   184,
       0,   344,     0,     0,     0,   345,    86,   186,     0,    88,
     346,     0,   187,   188,     0,     0,     0,     0,     0,   347,
     348,     0,     0,     0,     0,   189,     0,     0,     0,   190,
       0,   349,   191,     0,     0,   350,   192,     0,     0,     0,
       0,     0,   193,   194,     0,   195,   351,     0,     0,   352,
     196,   197,   198,   353,     0,     0,   354,   355,   201,     0,
     202,   356,   357,   204,   205,   206,     0,     0,   207,   208,
     209,   358,   359,   210,   360,   361,     0,   211,   212,     0,
     213,   214,   215,   216,     0,     0,     0,     0,     0,     0,
       0,     0,   217,   362,     0,   218,     0,   219,   220,   363,
     222,     0,     0,     0,     0,     0,   223,     0,     0,     0,
       0,   224,   364,   365,     0,     0,     0,   225,     0,     0,
     366,   226,   227,   228,   229,   230,   367,   232,     0,   368,
     234,     0,     0,     0,   235,   236,   237,   238,     0,     0,
     239,     0,   240,   241,   242,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   243,   244,   369,   370,   245,
       0,     0,   246,   371,   248,   372,   373,   249,   374,   250,
     251,     0,     0,   252,     0,   375,   253,     0,   254,   376,
     377,   378,     0,     0,   257,   379,   380,   381,   259,   382,
     260,   261,     0,     0,     0,   262,     0,     0,     0,     0,
     383,     0,     0,   384,   385,   386,   387,   265,     0,     0,
     266,   388,     0,   389,   390,   267,   268,     0,     0,     0,
       0,   269,     0,     0,     0,   391,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   392,   393,     0,     0,     0,
       0,   394,     0,   395,     0,   396,     0,     0,   397,   398,
     125,     0,   317,   127,   128,     0,     0,     0,     0,   129,
       0,     0,     0,   130,   131,   318,     0,   133,     0,     0,
     319,   320,   134,     0,   135,   136,     0,   137,     0,     0,
       0,   138,   321,   322,   323,   139,     0,   324,   140,     0,
     141,   325,     0,   326,   144,     0,   145,   146,   147,   148,
     149,   150,     0,   151,   152,     0,   153,   327,   328,     0,
       0,   154,   329,   330,     0,   331,   155,   156,   157,   332,
     333,   334,     0,     0,     0,     0,   335,   160,   336,     0,
       0,   337,     0,     0,     0,     0,   161,   162,     0,     0,
     163,     0,     0,   164,   165,   166,     0,     0,   167,     0,
     168,   169,     0,   170,   171,     0,   172,     0,   173,   174,
     338,   339,   175,   340,   341,   176,   177,   178,   342,   179,
       0,     0,     0,   180,   181,     0,   182,     0,   183,     0,
     343,   184,     0,   344,     0,     0,     0,   345,    86,   186,
       0,    88,   346,     0,   187,   188,     0,     0,     0,     0,
       0,   347,   348,     0,     0,     0,     0,   189,     0,     0,
       0,   190,     0,   349,   191,     0,     0,   350,   192,     0,
       0,     0,     0,     0,   193,   194,     0,   195,   351,     0,
       0,   352,   196,   197,   198,   353,     0,     0,   354,   355,
     201,     0,   202,   356,   357,   204,   205,   206,     0,     0,
     207,   208,   209,   358,   359,   210,   360,   361,     0,   211,
     212,     0,   213,   214,   215,   216,     0,     0,     0,     0,
       0,     0,     0,     0,   217,   362,     0,   218,     0,   219,
     220,   363,   222,     0,     0,     0,     0,     0,   223,     0,
       0,     0,     0,   224,   364,   365,     0,     0,     0,   225,
       0,     0,   366,   226,   227,   228,   229,   230,   367,   232,
       0,   368,   234,     0,     0,     0,   235,   236,   237,   238,
       0,     0,   239,     0,   240,   241,   242,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   243,   244,   369,
     370,   245,     0,     0,   246,   371,   248,   372,   373,   249,
     374,   250,   251,     0,     0,   252,     0,   375,   253,     0,
     254,   376,   377,   378,     0,     0,   257,   379,   380,   381,
     259,   382,   260,   261,     0,     0,     0,   262,     0,     0,
       0,     0,   383,     0,     0,   384,   385,   386,   387,   265,
       0,     0,   266,   388,     0,   389,   390,   267,   268,     0,
       0,     0,     0,   269,     0,     0,     0,   391,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   392,   393,     0,
       0,     0,     0,   394,     0,   533,     0,   396,     0,     0,
     397,   398,   125,     0,   317,   127,   128,   815,     0,     0,
       0,   816,     0,     0,     0,   130,   131,   318,     0,   133,
       0,     0,   319,   320,   134,     0,   135,   136,     0,   137,
       0,     0,     0,   138,   321,   322,   323,   139,     0,   324,
     140,     0,   141,   325,     0,   326,   144,     0,   145,   146,
     147,   148,   149,   150,     0,   151,   152,     0,   153,   327,
     328,     0,     0,   154,   329,   330,     0,   331,   155,   156,
     157,   332,   333,   334,     0,     0,     0,     0,   335,   160,
     336,     0,     0,   337,     0,     0,     0,     0,   161,   162,
       0,     0,   163,     0,     0,   164,   165,   166,     0,     0,
     167,     0,   168,   169,     0,   170,   171,     0,   172,     0,
     173,   174,   338,   339,   175,   340,   341,   176,   177,   178,
     342,   179,     0,     0,     0,   180,   181,     0,   182,     0,
     183,     0,   343,   184,     0,   344,     0,     0,     0,   345,
      86,   186,     0,    88,   346,     0,   187,   188,     0,     0,
       0,     0,     0,   347,   348,     0,     0,     0,     0,   189,
       0,     0,     0,   190,     0,   349,   191,     0,     0,   350,
     192,     0,     0,     0,     0,     0,   193,   194,     0,   195,
     351,     0,     0,   352,   196,   197,   198,   353,     0,     0,
     354,   355,   201,     0,   202,   356,   357,   204,   205,   206,
       0,     0,   207,   208,   209,     0,   359,   210,   360,   361,
       0,   211,   212,     0,   213,   214,   215,   216,     0,     0,
       0,     0,     0,     0,     0,     0,   217,   362,     0,   218,
       0,   219,   220,   363,   222,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,   224,   364,   365,     0,     0,
       0,   225,     0,     0,   366,   226,   227,   228,   229,   230,
     367,   232,     0,   368,   234,     0,     0,     0,   235,   236,
     237,   238,     0,     0,   239,     0,   240,   241,   242,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   243,
     244,   369,   370,   245,     0,     0,   246,   371,   248,   372,
     373,   249,   374,   250,   251,     0,     0,   252,     0,   375,
     253,     0,   254,   376,   377,   378,     0,     0,   257,   379,
     380,   381,   259,   382,   260,   261,     0,     0,     0,   262,
       0,     0,     0,     0,   383,     0,     0,   384,   385,   386,
     387,   265,     0,     0,   266,   388,     0,   389,   390,   267,
     268,     0,     0,     0,     0,   269,     0,     0,     0,   391,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   392,
     393,     0,     0,     0,     0,   394,     0,   395,     0,   396,
       0,     0,   397,   398,   125,     0,   317,   127,   128,     0,
       0,     0,     0,   129,     0,     0,     0,   130,   131,   318,
       0,   133,     0,     0,   319,   320,   134,     0,   135,   136,
       0,   137,     0,     0,     0,   138,   321,   322,   323,   139,
       0,   324,   140,     0,   141,   325,     0,   326,   144,     0,
     145,   146,   147,   148,   149,   150,     0,   151,   152,     0,
     153,   327,   328,     0,     0,   154,   329,   330,     0,   331,
     155,   156,   157,   332,   333,   334,     0,     0,     0,     0,
     335,   160,   336,     0,     0,   337,     0,     0,     0,     0,
     161,   162,     0,     0,   163,     0,     0,   164,   165,   166,
       0,     0,   167,     0,   168,   169,     0,   170,   171,     0,
     172,     0,   173,   174,   338,   339,   175,   340,   341,   176,
     177,   178,   342,   179,     0,     0,     0,   180,   181,     0,
     182,     0,   183,     0,   343,   184,     0,   344,     0,     0,
       0,   345,    86,   186,     0,    88,   346,     0,   187,   188,
       0,     0,     0,     0,     0,   347,  1128,     0,     0,     0,
       0,   189,     0,     0,     0,   190,     0,   349,   191,     0,
       0,   350,   192,     0,     0,     0,     0,     0,   193,   194,
       0,   195,   351,     0,     0,   352,   196,   197,   198,   353,
       0,     0,   354,   355,   201,     0,   202,   356,   357,   204,
     205,   206,     0,     0,   207,   208,   209,   358,   359,   210,
     360,   361,     0,   211,   212,     0,   213,   214,   215,   216,
       0,     0,     0,     0,     0,     0,     0,     0,   217,   362,
       0,   218,     0,   219,   220,   363,   222,     0,     0,     0,
       0,     0,   223,     0,     0,     0,     0,   224,   364,   365,
       0,     0,     0,   225,     0,     0,   366,   226,   227,   228,
     229,   230,   367,   232,     0,   368,   234,     0,     0,     0,
     235,   236,   237,   238,     0,     0,   239,     0,   240,   241,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   243,   244,   369,   370,   245,     0,     0,   246,   371,
     248,   372,   373,   249,   374,   250,   251,     0,     0,   252,
       0,   375,   253,     0,   254,   376,   377,   378,     0,     0,
     257,   379,   380,   381,   259,   382,   260,   261,     0,     0,
       0,   262,     0,     0,     0,     0,   383,     0,     0,   384,
     385,   386,   387,   265,     0,     0,   266,   388,     0,   389,
     390,   267,   268,     0,     0,     0,     0,   269,     0,     0,
       0,   391,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   392,   393,     0,     0,     0,     0,   394,     0,   395,
       0,   396,     0,     0,   397,   398,   125,     0,   317,   127,
     128,     0,     0,     0,     0,   129,     0,     0,     0,   130,
     131,   318,     0,   133,     0,     0,   319,   320,   134,     0,
     135,   136,     0,   137,     0,     0,     0,   138,   321,   322,
     323,   139,     0,   324,   140,     0,   141,   325,     0,   326,
     144,     0,   145,   146,   147,   148,   149,   150,     0,   151,
     152,     0,   153,   327,   328,     0,     0,   154,   329,   330,
       0,   331,   155,   156,   157,   332,   333,   334,     0,     0,
       0,     0,   335,   160,   336,     0,     0,   337,     0,     0,
       0,     0,   161,   162,     0,     0,   163,     0,     0,   164,
     165,   166,     0,     0,   167,     0,   168,   169,     0,   170,
     171,     0,   172,     0,   173,   174,   338,   339,   175,   340,
     341,   176,   177,   178,   342,   179,     0,     0,     0,   180,
     181,     0,   182,     0,   183,     0,   343,   184,     0,   344,
       0,     0,     0,   345,    86,   186,     0,    88,   346,     0,
     187,   188,     0,     0,     0,     0,     0,   347,  1170,     0,
       0,     0,     0,   189,     0,     0,     0,   190,     0,   349,
     191,     0,     0,   350,   192,     0,     0,     0,     0,     0,
     193,   194,     0,   195,   351,     0,     0,   352,   196,   197,
     198,   353,     0,     0,   354,   355,   201,     0,   202,   356,
     357,   204,   205,   206,     0,     0,   207,   208,   209,   358,
     359,   210,   360,   361,     0,   211,   212,     0,   213,   214,
     215,   216,     0,     0,     0,     0,     0,     0,     0,     0,
     217,   362,     0,   218,     0,   219,   220,   363,   222,     0,
       0,     0,     0,     0,   223,     0,     0,     0,     0,   224,
     364,   365,     0,     0,     0,   225,     0,     0,   366,   226,
     227,   228,   229,   230,   367,   232,     0,   368,   234,     0,
       0,     0,   235,   236,   237,   238,     0,     0,   239,     0,
     240,   241,   242,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   243,   244,   369,   370,   245,     0,     0,
     246,   371,   248,   372,   373,   249,   374,   250,   251,     0,
       0,   252,     0,   375,   253,     0,   254,   376,   377,   378,
       0,     0,   257,   379,   380,   381,   259,   382,   260,   261,
       0,     0,     0,   262,     0,     0,     0,     0,   383,     0,
       0,   384,   385,   386,   387,   265,     0,     0,   266,   388,
       0,   389,   390,   267,   268,     0,     0,     0,     0,   269,
       0,     0,     0,   391,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   392,   393,     0,     0,     0,     0,   394,
       0,   395,     0,   396,     0,     0,   397,   398,   125,     0,
     317,   127,   128,     0,     0,     0,     0,   129,     0,     0,
       0,   130,   131,   318,     0,   133,     0,     0,   319,   320,
     134,     0,   135,   136,     0,   137,     0,     0,     0,   138,
     321,   322,   323,   139,     0,   324,   140,     0,   141,   325,
       0,   326,   144,     0,   145,   146,   147,   148,   149,   150,
       0,   151,   152,     0,   153,   327,   328,     0,     0,   154,
     329,   330,     0,   331,   155,   156,   157,   332,   333,   334,
       0,     0,     0,     0,   335,   160,   336,     0,     0,  1264,
       0,     0,     0,     0,   161,   162,     0,     0,   163,     0,
       0,   164,   165,   166,     0,     0,   167,     0,   168,   169,
       0,   170,   171,     0,   172,     0,   173,   174,   338,   339,
     175,   340,   341,   176,   177,   178,   342,   179,     0,     0,
       0,   180,   181,     0,   182,     0,   183,     0,   343,   184,
       0,   344,     0,     0,     0,   345,    86,   186,     0,    88,
     346,     0,   187,   188,     0,     0,     0,     0,     0,   347,
     348,     0,     0,     0,     0,   189,     0,     0,     0,   190,
       0,   349,   191,     0,     0,   350,   192,     0,     0,     0,
       0,     0,   193,   194,     0,   195,   351,     0,     0,   352,
     196,   197,   198,   353,     0,     0,   354,   355,   201,     0,
     202,   356,   357,   204,   205,   206,     0,     0,   207,   208,
     209,   358,   359,   210,   360,   361,     0,   211,   212,     0,
     213,   214,   215,   216,     0,     0,     0,     0,     0,     0,
       0,     0,   217,   362,     0,   218,     0,   219,   220,   363,
     222,     0,     0,     0,     0,     0,   223,     0,     0,     0,
       0,   224,   364,   365,     0,     0,     0,   225,     0,     0,
     366,   226,   227,   228,   229,   230,   367,   232,     0,   368,
     234,     0,     0,     0,   235,   236,   237,   238,     0,     0,
     239,     0,   240,   241,   242,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   243,   244,   369,   370,   245,
       0,     0,   246,   371,   248,   372,   373,   249,   374,   250,
     251,     0,     0,   252,     0,   375,   253,     0,   254,   376,
     377,   378,     0,     0,   257,   379,   380,   381,   259,   382,
     260,   261,     0,     0,     0,   262,     0,     0,     0,     0,
     383,     0,     0,   384,   385,   386,   387,   265,     0,     0,
     266,   388,     0,   389,   390,   267,   268,     0,     0,     0,
       0,   269,     0,     0,     0,   391,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   392,   393,     0,     0,     0,
       0,   394,     0,   395,     0,   396,     0,     0,   397,   398,
     125,     0,   317,   127,   128,     0,     0,     0,     0,   129,
       0,     0,     0,   130,   131,   318,     0,   133,     0,     0,
     319,   320,   134,     0,   135,   136,     0,   137,     0,     0,
       0,   138,   321,   322,   323,   139,     0,   324,   140,     0,
     141,   325,     0,   326,   144,     0,   145,   146,   147,   148,
     149,   150,     0,   151,   152,     0,   153,   327,   328,     0,
       0,   154,   329,   330,     0,   331,   155,   156,   157,   332,
     333,   334,     0,     0,     0,     0,   335,   160,   336,     0,
       0,   337,     0,     0,     0,     0,   161,   162,     0,     0,
     163,     0,     0,   164,   165,   166,     0,     0,   167,     0,
     168,   169,     0,   170,   171,     0,   172,     0,   173,   174,
     338,   339,   175,   340,   341,   176,   177,   178,   342,   179,
       0,     0,     0,   180,   181,     0,   182,     0,   183,     0,
     343,   184,     0,   344,     0,     0,     0,   345,    86,   186,
       0,    88,   346,     0,   187,   188,     0,     0,     0,     0,
       0,   347,   348,     0,     0,     0,     0,   189,     0,     0,
       0,   190,     0,   349,   191,     0,     0,   350,   192,     0,
       0,     0,     0,     0,   193,   194,     0,   195,   351,     0,
       0,   352,   196,   197,   198,   353,     0,     0,   354,   355,
     201,     0,   202,   356,   357,   204,   205,   206,     0,     0,
     207,   208,   209,     0,   359,   210,   360,   361,     0,   211,
     212,     0,   213,   214,   215,   216,     0,     0,     0,     0,
       0,     0,     0,     0,   217,   362,     0,   218,     0,   219,
     220,   363,   222,     0,     0,     0,     0,     0,   223,     0,
       0,     0,     0,   224,   364,   365,     0,     0,     0,   225,
       0,     0,   366,   226,   227,   228,   229,   230,   367,   232,
       0,   368,   234,     0,     0,     0,   235,   236,   237,   238,
       0,     0,   239,     0,   240,   241,   242,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   243,   244,   369,
     370,   245,     0,     0,   246,   371,   248,   372,   373,   249,
     374,   250,   251,     0,     0,   252,     0,   375,   253,     0,
     254,   376,   377,   378,     0,     0,   257,   379,   380,   381,
     259,   382,   260,   261,     0,     0,     0,   262,     0,     0,
       0,     0,   383,     0,     0,   384,   385,   386,   387,   265,
       0,     0,   266,   388,     0,   389,   390,   267,   268,     0,
       0,     0,     0,   269,     0,     0,     0,   391,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   392,   393,     0,
       0,     0,     0,   394,     0,   395,     0,   396,     0,     0,
     397,   398,   125,     0,   317,   127,   128,     0,     0,     0,
       0,   129,     0,     0,     0,   130,   131,   318,     0,   133,
       0,     0,   319,   320,   134,     0,   135,   136,     0,   137,
       0,     0,     0,   138,   321,   322,   323,   139,     0,   324,
     140,     0,   141,   325,     0,   326,   144,     0,   145,   146,
     147,   148,   149,   150,     0,   151,   152,     0,   153,   327,
     328,     0,     0,   154,   329,   330,     0,   331,   155,   156,
     157,   332,   333,   334,     0,     0,     0,     0,   335,   160,
     336,     0,     0,   337,     0,     0,     0,     0,   161,   162,
       0,     0,   163,     0,     0,   164,   165,   166,     0,     0,
     167,     0,   168,   169,     0,   170,   171,     0,   172,     0,
     173,   174,   338,   339,   175,   340,   341,   176,   177,   178,
     342,   179,     0,     0,     0,   180,   181,     0,   182,     0,
     183,     0,   343,   184,     0,   344,     0,     0,     0,   345,
      86,   186,     0,    88,   346,     0,   187,   188,     0,     0,
       0,     0,     0,   347,   833,     0,     0,     0,     0,   189,
       0,     0,     0,   190,     0,   349,   191,     0,     0,   350,
     192,     0,     0,     0,     0,     0,   193,   194,     0,   195,
     351,     0,     0,   352,   196,   197,   198,   353,     0,     0,
     354,   355,   201,     0,   202,   356,   357,   204,   205,   206,
       0,     0,   207,   208,   209,     0,   359,   210,   360,   361,
       0,   211,   212,     0,   213,   214,   215,   216,     0,     0,
       0,     0,     0,     0,     0,     0,   217,   362,     0,   218,
       0,   219,   220,   363,   222,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,   224,   364,   365,     0,     0,
       0,   225,     0,     0,   366,   226,   227,   228,   229,   230,
     367,   232,     0,   368,   234,     0,     0,     0,   235,   236,
     237,   238,     0,     0,   239,     0,   240,   241,   242,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   243,
     244,   369,   370,   245,     0,     0,   246,   371,   248,   372,
     373,   249,   374,   250,   251,     0,     0,   252,     0,   375,
     253,     0,   254,   376,   377,   378,     0,     0,   257,   379,
     380,   381,   259,   382,   260,   261,     0,     0,     0,   262,
       0,     0,     0,     0,   383,     0,     0,   384,   385,   386,
     387,   265,     0,     0,   266,   388,     0,   389,   390,   267,
     268,     0,     0,     0,     0,   269,     0,     0,     0,   391,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   392,
     393,     0,     0,     0,     0,   394,     0,   395,     0,   396,
       0,     0,   397,   398,   125,     0,   317,   127,   128,     0,
       0,     0,     0,   129,     0,     0,     0,   130,   131,   318,
       0,   133,     0,     0,   319,   320,   134,     0,   135,   136,
       0,   137,     0,     0,     0,   138,   321,   322,   323,   139,
       0,   324,   140,     0,   141,   325,     0,   326,   144,     0,
     145,   146,   147,   148,   149,   150,     0,   151,   152,     0,
     153,   327,   328,     0,     0,   154,   329,   330,     0,   331,
     155,   156,   157,   332,   333,   334,     0,     0,     0,     0,
     335,   160,   336,     0,     0,   337,     0,     0,     0,     0,
     161,   162,     0,     0,   163,     0,     0,   164,   165,   166,
       0,     0,   167,     0,   168,   169,     0,   170,   171,     0,
     172,     0,   173,   174,   338,   339,   175,   340,   341,   176,
     177,   178,   342,   179,     0,     0,     0,   180,   181,     0,
     182,     0,   183,     0,   343,   184,     0,   344,     0,     0,
       0,   345,    86,   186,     0,    88,   346,     0,   187,   188,
       0,     0,     0,     0,     0,   347,   835,     0,     0,     0,
       0,   189,     0,     0,     0,   190,     0,   349,   191,     0,
       0,   350,   192,     0,     0,     0,     0,     0,   193,   194,
       0,   195,   351,     0,     0,   352,   196,   197,   198,   353,
       0,     0,   354,   355,   201,     0,   202,   356,   357,   204,
     205,   206,     0,     0,   207,   208,   209,     0,   359,   210,
     360,   361,     0,   211,   212,     0,   213,   214,   215,   216,
       0,     0,     0,     0,     0,     0,     0,     0,   217,   362,
       0,   218,     0,   219,   220,   363,   222,     0,     0,     0,
       0,     0,   223,     0,     0,     0,     0,   224,   364,   365,
       0,     0,     0,   225,     0,     0,   366,   226,   227,   228,
     229,   230,   367,   232,     0,   368,   234,     0,     0,     0,
     235,   236,   237,   238,     0,     0,   239,     0,   240,   241,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   243,   244,   369,   370,   245,     0,     0,   246,   371,
     248,   372,   373,   249,   374,   250,   251,     0,     0,   252,
       0,   375,   253,     0,   254,   376,   377,   378,     0,     0,
     257,   379,   380,   381,   259,   382,   260,   261,     0,     0,
       0,   262,     0,     0,     0,     0,   383,     0,     0,   384,
     385,   386,   387,   265,     0,     0,   266,   388,     0,   389,
     390,   267,   268,     0,     0,     0,     0,   269,     0,     0,
       0,   391,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   392,   393,     0,     0,     0,     0,   394,     0,   395,
       0,   396,     0,     0,   397,   398,   125,     0,   126,   127,
     128,     0,     0,     0,     0,   129,     0,     0,     0,   130,
     131,   132,     0,   133,     0,     0,     0,     0,   134,     0,
     135,   136,     0,   137,     0,     0,     0,   138,     0,     0,
       0,   139,     0,     0,   140,     0,   141,   142,     0,   143,
     144,     0,   145,   146,   147,   148,   149,   150,     0,   151,
     152,     0,   153,     0,     0,     0,     0,   154,     0,     0,
       0,     0,   155,   156,   157,     0,     0,   158,     0,     0,
       0,     0,   159,   160,     0,     0,     0,     0,     0,     0,
       0,     0,   161,   162,     0,     0,   163,     0,     0,   164,
     165,   166,     0,     0,   167,     0,   168,   169,     0,   170,
     171,     0,   172,     0,   173,   174,     0,     0,   175,     0,
       0,   176,   177,   178,     0,   179,     0,     0,     0,   180,
     181,     0,   182,     0,   183,     0,     0,   184,     0,     0,
       0,     0,     0,   185,    86,   186,     0,    88,     0,     0,
     187,   188,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   189,     0,     0,     0,   190,     0,     0,
     191,     0,     0,     0,   192,     0,     0,     0,     0,     0,
     193,   194,     0,   195,     0,     0,     0,     0,   196,   197,
     198,   199,     0,     0,   200,     0,   201,     0,   202,     0,
     203,   204,   205,   206,     0,     0,   207,   208,   209,     0,
       0,   210,     0,     0,     0,   211,   212,     0,   213,   214,
     215,   216,     0,     0,     0,     0,     0,     0,     0,     0,
     217,     0,     0,   218,     0,   219,   220,   221,   222,     0,
       0,     0,     0,     0,   223,     0,     0,     0,     0,   224,
       0,     0,     0,     0,     0,   225,     0,     0,     0,   226,
     227,   228,   229,   230,   231,   232,     0,   233,   234,     0,
       0,     0,   235,   236,   237,   238,     0,     0,   239,     0,
     240,   241,   242,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   243,   244,     0,     0,   245,     0,     0,
     246,   247,   248,     0,     0,   249,     0,   250,   251,     0,
       0,   252,     0,     0,   253,     0,   254,     0,   255,   256,
       0,     0,   257,     0,     0,   258,   259,     0,   260,   261,
       0,     0,     0,   262,     0,     0,     0,     0,   263,     0,
       0,     0,     0,   264,     0,   265,     0,     0,   266,     0,
       0,     0,     0,   267,   268,     0,     0,     0,     0,   269,
       0,     0,     0,   270,     0,     0,   125,     0,   126,   127,
     128,     0,     0,     0,     0,   129,     0,     0,     0,   130,
     131,   132,  2075,   133,     0,     0,   397,  2076,   134,     0,
     135,   136,     0,   137,     0,     0,     0,   138,     0,     0,
       0,   139,     0,     0,   140,     0,   141,   142,     0,   143,
     144,     0,   145,   146,   147,   148,   149,   150,     0,   151,
     152,     0,   153,     0,     0,     0,     0,   154,     0,     0,
       0,     0,   155,   156,   157,     0,     0,   158,     0,     0,
       0,     0,   159,   160,     0,     0,     0,     0,     0,     0,
       0,     0,   161,   162,     0,     0,   163,     0,     0,   164,
     165,   166,     0,     0,   167,     0,   168,   169,     0,   170,
     171,     0,   172,     0,   173,   174,     0,     0,   175,     0,
       0,   176,   177,   178,     0,   179,     0,     0,     0,   180,
     181,     0,   182,     0,   183,     0,     0,   184,     0,     0,
       0,     0,     0,   185,    86,   186,     0,    88,     0,     0,
     187,   188,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   189,     0,     0,     0,   190,     0,     0,
     191,     0,     0,     0,   192,     0,     0,     0,     0,     0,
     193,   194,     0,   195,     0,     0,     0,     0,   196,   197,
     198,   199,     0,     0,   200,     0,   201,     0,   202,     0,
     203,   204,   205,   206,     0,     0,   207,   208,   209,     0,
       0,   210,     0,     0,     0,   211,   212,     0,   213,   214,
     215,   216,     0,     0,     0,     0,     0,     0,     0,     0,
     217,     0,     0,   218,     0,   219,   220,   221,   222,     0,
       0,     0,     0,     0,   223,     0,     0,     0,     0,   224,
       0,     0,     0,     0,     0,   225,     0,     0,     0,   226,
     227,   228,   229,   230,   231,   232,     0,   233,   234,     0,
       0,     0,   235,   236,   237,   238,     0,     0,   239,     0,
     240,   241,   242,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   243,   244,     0,     0,   245,     0,     0,
     246,   247,   248,     0,     0,   249,     0,   250,   251,     0,
       0,   252,     0,     0,   253,     0,   254,     0,   255,   256,
       0,     0,   257,     0,     0,   258,   259,     0,   260,   261,
       0,     0,     0,   262,     0,     0,     0,     0,   263,     0,
       0,     0,     0,   264,     0,   265,     0,     0,   266,     0,
       0,     0,     0,   267,   268,     0,     0,     0,   125,   269,
     126,   127,   128,   270,     0,     0,     0,   129,     0,     0,
       0,   130,   131,   132,     0,   133,     0,     0,     0,     0,
     134,     0,   135,   136,     0,   137,   397,  2076,     0,   138,
       0,     0,     0,   139,     0,     0,   140,     0,   141,   142,
       0,   143,   144,     0,   145,   146,   147,   148,   149,   150,
       0,   151,   152,  1273,   153,     0,     0,     0,     0,   154,
       0,     0,     0,     0,   155,   156,   157,     0,     0,   158,
       0,     0,     0,     0,   159,   160,     0,     0,     0,     0,
       0,     0,     0,     0,   161,   162,     0,     0,   163,     0,
       0,   164,   165,   166,     0,     0,   167,     0,   168,   169,
       0,   170,   171,     0,   172,     0,   173,   174,     0,     0,
     175,     0,     0,   176,   177,   178,     0,   179,     0,     0,
       0,   180,   181,     0,   182,     0,   183,     0,     0,   184,
       0,     0,     0,     0,     0,   185,    86,   186,     0,    88,
       0,     0,   187,   188,  1274,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   189,     0,     0,     0,   190,
    1275,     0,   191,     0,     0,     0,   192,     0,  1114,     0,
       0,     0,   193,   194,     0,   195,     0,     0,     0,     0,
     196,   197,   198,   199,     0,     0,   200,     0,   201,     0,
     202,     0,   203,   204,   205,   206,     0,     0,   207,   208,
     209,     0,     0,   210,     0,     0,     0,   211,   212,     0,
     213,   214,   215,   216,     0,     0,     0,     0,     0,     0,
       0,     0,   217,     0,     0,   218,     0,   219,   220,   221,
     222,     0,     0,     0,     0,     0,   223,     0,     0,     0,
       0,   224,     0,     0,     0,     0,     0,   225,     0,     0,
       0,   226,   227,   228,   229,   230,   231,   232,     0,   233,
     234,   850,     0,     0,   235,   236,   237,   238,     0,     0,
     239,     0,   240,   241,   242,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   243,   244,     0,     0,   245,
       0,     0,   246,   247,   248,     0,     0,   249,     0,   250,
     251,     0,     0,   252,     0,     0,   253,     0,   254,     0,
     255,   256,     0,     0,   257,     0,     0,   258,   259,     0,
     260,   261,     0,     0,     0,   262,     0,     0,     0,     0,
     263,     0,     0,     0,     0,   264,     0,   265,     0,   125,
     266,   126,   127,   128,     0,   267,   268,     0,   129,     0,
       0,   269,   130,   131,   132,   270,   133,     0,     0,     0,
       0,   134,     0,   135,   136,     0,   137,     0,     0,     0,
     138,     0,     0,     0,   139,     0,     0,   140,  1308,   141,
     142,     0,   143,   144,     0,   145,   146,   147,   148,   149,
     150,     0,   151,   152,  1273,   153,     0,     0,     0,     0,
     154,     0,     0,     0,     0,   155,   156,   157,     0,     0,
     158,     0,     0,     0,     0,   159,   160,     0,     0,     0,
       0,     0,     0,     0,     0,   161,   162,     0,     0,   163,
       0,     0,   164,   165,   166,     0,     0,   167,     0,   168,
     169,     0,   170,   171,     0,   172,     0,   173,   174,     0,
       0,   175,     0,     0,   176,   177,   178,     0,   179,     0,
       0,     0,   180,   181,     0,   182,     0,   183,     0,     0,
     184,     0,     0,     0,     0,     0,   185,    86,   186,     0,
      88,     0,     0,   187,   188,  1274,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   189,     0,     0,     0,
     190,  1275,     0,   191,     0,     0,     0,   192,     0,     0,
       0,     0,     0,   193,   194,     0,   195,     0,     0,     0,
       0,   196,   197,   198,   199,     0,     0,   200,     0,   201,
       0,   202,     0,   203,   204,   205,   206,     0,     0,   207,
     208,   209,     0,     0,   210,     0,     0,     0,   211,   212,
       0,   213,   214,   215,   216,     0,     0,     0,     0,     0,
       0,     0,     0,   217,     0,     0,   218,     0,   219,   220,
     221,   222,     0,     0,     0,     0,     0,   223,     0,     0,
       0,     0,   224,     0,     0,     0,     0,     0,   225,     0,
       0,     0,   226,   227,   228,   229,   230,   231,   232,     0,
     233,   234,     0,     0,     0,   235,   236,   237,   238,     0,
       0,   239,     0,   240,   241,   242,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,     0,     0,
     245,     0,     0,   246,   247,   248,     0,     0,   249,     0,
     250,   251,     0,     0,   252,     0,     0,   253,     0,   254,
       0,   255,   256,     0,     0,   257,     0,     0,   258,   259,
       0,   260,   261,     0,     0,     0,   262,     0,     0,     0,
       0,   263,     0,     0,     0,     0,   264,     0,   265,     0,
     125,   266,   126,   127,   128,     0,   267,   268,     0,   129,
       0,     0,   269,   130,   131,   132,   270,   133,     0,     0,
       0,     0,   134,     0,   135,   136,     0,   137,     0,     0,
       0,   138,     0,     0,     0,   139,     0,     0,   140,  1308,
     141,   142,     0,   143,   144,     0,   145,   146,   147,   148,
     149,   150,     0,   151,   152,     0,   153,     0,     0,     0,
       0,   154,     0,     0,     0,     0,   155,   156,   157,     0,
       0,   158,     0,     0,     0,     0,   159,   160,     0,     0,
       0,     0,     0,     0,     0,     0,   161,   162,     0,     0,
     163,     0,     0,   164,   165,   166,     0,     0,   167,     0,
     168,   169,     0,   170,   171,     0,   172,     0,   173,   174,
       0,     0,   175,     0,     0,   176,   177,   178,     0,   179,
       0,     0,     0,   180,   181,     0,   182,     0,   183,     0,
       0,   184,     0,     0,     0,     0,     0,   185,    86,   186,
       0,    88,     0,     0,   187,   188,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   189,     0,     0,
       0,   190,     0,     0,   191,     0,     0,     0,   192,     0,
       0,     0,     0,     0,   193,   194,     0,   195,     0,     0,
       0,     0,   196,   197,   198,   199,     0,     0,   200,     0,
     201,     0,   202,     0,   203,   204,   205,   206,     0,     0,
     207,   208,   209,     0,     0,   210,     0,     0,     0,   211,
     212,     0,   213,   214,   215,   216,     0,     0,     0,     0,
       0,     0,     0,     0,   217,     0,     0,   218,     0,   219,
     220,   221,   222,     0,     0,     0,     0,     0,   223,     0,
       0,     0,     0,   224,     0,     0,     0,     0,     0,   225,
       0,     0,     0,   226,   227,   228,   229,   230,   231,   232,
       0,   233,   234,   850,     0,     0,   235,   236,   237,   238,
       0,     0,   239,     0,   240,   241,   242,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   243,   244,     0,
       0,   245,     0,     0,   246,   247,   248,     0,     0,   249,
       0,   250,   251,     0,     0,   252,     0,     0,   253,     0,
     254,     0,   255,   256,     0,     0,   257,     0,     0,   258,
     259,     0,   260,   261,     0,     0,     0,   262,     0,     0,
       0,     0,   263,     0,     0,     0,     0,   264,     0,   265,
       0,     0,   266,     0,     0,     0,     0,   267,   268,     0,
       0,     0,     0,   269,     0,     0,     0,   270,     0,     0,
     125,     0,   126,   127,   128,     0,     0,     0,     0,   129,
       0,     0,     0,   130,   131,   132,  1040,   133,     0,     0,
     397,     0,   134,     0,   135,   136,     0,   137,     0,     0,
       0,   138,     0,     0,     0,   139,     0,     0,   140,     0,
     141,   142,     0,   143,   144,     0,   145,   146,   147,   148,
     149,   150,     0,   151,   152,     0,   153,     0,     0,     0,
       0,   154,     0,     0,     0,     0,   155,   156,   157,     0,
       0,   158,     0,     0,     0,     0,   159,   160,     0,     0,
       0,     0,     0,     0,     0,     0,   161,   162,     0,     0,
     163,     0,     0,   164,   165,   166,     0,     0,   167,     0,
     168,   169,     0,   170,   171,     0,   172,     0,   173,   174,
       0,     0,   175,     0,     0,   176,   177,   178,     0,   179,
       0,     0,     0,   180,   181,     0,   182,     0,   183,     0,
       0,   184,     0,     0,     0,     0,     0,   185,    86,   186,
       0,    88,     0,     0,   187,   188,     0,     0,     0,     0,
       0,     0,     0,   421,     0,     0,     0,   189,     0,     0,
       0,   190,     0,     0,   191,     0,     0,     0,   192,     0,
       0,     0,     0,     0,   193,   194,     0,   195,     0,     0,
       0,     0,   196,   197,   198,   199,     0,     0,   200,     0,
     201,     0,   202,     0,   203,   204,   205,   206,     0,     0,
     207,   208,   209,     0,     0,   210,     0,     0,     0,   211,
     212,     0,   213,   214,   215,   216,     0,     0,     0,     0,
       0,     0,     0,     0,   217,     0,     0,   218,     0,   219,
     220,   221,   222,     0,     0,     0,     0,     0,   223,     0,
       0,     0,     0,   224,     0,     0,     0,     0,     0,   225,
       0,     0,     0,   226,   227,   228,   229,   230,   231,   232,
       0,   233,   234,     0,     0,     0,   235,   236,   237,   238,
       0,     0,   239,     0,   240,   241,   242,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   243,   244,     0,
       0,   245,     0,     0,   246,   247,   248,     0,     0,   249,
       0,   250,   251,     0,     0,   252,     0,     0,   253,     0,
     254,     0,   255,   256,     0,     0,   257,     0,     0,   258,
     259,     0,   260,   261,     0,     0,     0,   262,     0,     0,
       0,     0,   263,     0,     0,     0,     0,   264,     0,   265,
       0,   125,   266,   126,   127,   128,     0,   267,   268,     0,
     129,     0,     0,   269,   130,   131,   132,   270,   133,     0,
       0,     0,     0,   134,     0,   135,   136,     0,   137,     0,
       0,     0,   138,     0,     0,     0,   139,     0,     0,   140,
     282,   141,   142,     0,   143,   144,     0,   145,   146,   147,
     148,   149,   150,     0,   151,   152,     0,   153,     0,     0,
       0,     0,   154,     0,     0,     0,     0,   155,   156,   157,
       0,     0,   158,     0,     0,     0,     0,   159,   160,     0,
       0,     0,     0,     0,     0,     0,     0,   161,   162,     0,
       0,   163,     0,     0,   164,   165,   166,     0,     0,   167,
       0,   168,   169,     0,   170,   171,     0,   172,     0,   173,
     174,     0,     0,   175,     0,     0,   176,   177,   178,     0,
     179,     0,     0,     0,   180,   181,     0,   182,     0,   183,
       0,     0,   184,     0,     0,     0,     0,     0,   185,    86,
     186,     0,    88,     0,     0,   187,   188,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   189,     0,
       0,     0,   190,     0,     0,   191,     0,     0,     0,   192,
       0,     0,     0,     0,     0,   193,   194,     0,   195,     0,
       0,     0,     0,   196,   197,   198,   199,     0,     0,   200,
       0,   201,     0,   202,     0,   203,   204,   205,   206,     0,
       0,   207,   208,   209,     0,     0,   210,     0,     0,     0,
     211,   212,     0,   213,   214,   215,   216,     0,     0,     0,
       0,     0,     0,     0,     0,   217,     0,     0,   218,     0,
     219,   220,   221,   222,     0,     0,     0,     0,     0,   223,
       0,     0,     0,     0,   224,     0,     0,     0,     0,     0,
     225,     0,     0,     0,   226,   227,   228,   229,   230,   231,
     232,     0,   233,   234,     0,     0,     0,   235,   236,   237,
     238,     0,     0,   239,     0,   240,   241,   242,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   243,   244,
       0,     0,   245,     0,     0,   246,   247,   248,     0,     0,
     249,     0,   250,   251,     0,     0,   252,     0,     0,   253,
       0,   254,     0,   255,   256,     0,     0,   257,     0,     0,
     258,   259,     0,   260,   261,     0,     0,     0,   262,     0,
       0,     0,     0,   263,     0,     0,     0,     0,   264,     0,
     265,     0,   125,   266,   126,   127,   128,     0,   267,   268,
       0,   129,     0,     0,   269,   130,   131,   132,   270,   133,
       0,     0,     0,     0,   134,     0,   135,   136,     0,   137,
       0,     0,     0,   138,     0,     0,  1307,   139,     0,     0,
     140,  1308,   141,   142,     0,   143,   144,     0,   145,   146,
     147,   148,   149,   150,     0,   151,   152,     0,   153,     0,
       0,     0,     0,   154,     0,     0,     0,     0,   155,   156,
     157,     0,     0,   158,     0,     0,     0,     0,   159,   160,
       0,     0,     0,     0,     0,     0,     0,     0,   161,   162,
       0,     0,   163,     0,     0,   164,   165,   166,     0,     0,
     167,     0,   168,   169,     0,   170,   171,     0,   172,     0,
     173,   174,     0,     0,   281,     0,     0,   176,   177,   178,
       0,   179,     0,     0,     0,   180,   181,     0,   182,     0,
     183,     0,     0,   184,     0,     0,     0,     0,     0,   185,
      86,   186,     0,    88,     0,     0,   187,   188,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   189,
       0,     0,     0,   190,     0,     0,   191,     0,     0,     0,
     192,     0,     0,     0,     0,     0,   193,   194,     0,   195,
       0,     0,     0,     0,   196,   197,   198,   199,     0,     0,
     200,     0,   201,     0,   202,     0,   203,   204,   205,   206,
       0,     0,   207,   208,   209,     0,     0,   210,     0,     0,
       0,   211,   212,     0,   213,   214,   215,   216,     0,     0,
       0,     0,     0,     0,     0,     0,   217,     0,     0,   218,
       0,   219,   220,   221,   222,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,   224,     0,     0,     0,     0,
       0,   225,     0,     0,     0,   226,   227,   228,   229,   230,
     231,   232,     0,   233,   234,     0,     0,     0,   235,   236,
     237,   238,     0,     0,   239,     0,   240,   241,   242,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   243,
     244,     0,     0,   245,     0,     0,   246,   247,   248,     0,
       0,   249,     0,   250,   251,     0,     0,   252,     0,     0,
     253,     0,   254,     0,   255,   256,     0,     0,   257,     0,
       0,   258,   259,     0,   260,   261,     0,     0,     0,   262,
       0,     0,     0,     0,   263,     0,     0,     0,     0,   264,
       0,   265,     0,   125,   266,   126,   127,   128,     0,   267,
     268,     0,   129,     0,     0,   269,   130,   131,   132,   270,
     133,     0,     0,     0,     0,   134,     0,   135,   136,     0,
     137,     0,     0,     0,   138,     0,     0,     0,   139,     0,
       0,   140,   282,   141,   142,     0,   143,   144,     0,   145,
     146,   147,   148,   149,   150,     0,   151,   152,     0,   153,
       0,     0,     0,     0,   154,     0,     0,     0,     0,   155,
     156,   157,     0,     0,   158,     0,     0,     0,     0,   159,
     160,     0,     0,     0,     0,     0,     0,     0,     0,   161,
     162,     0,     0,   163,     0,     0,   164,   165,   166,     0,
       0,   167,     0,   168,   169,     0,   170,   171,     0,   172,
       0,   173,   174,     0,     0,   175,     0,     0,   176,   177,
     178,     0,   179,     0,     0,     0,   180,   181,     0,   182,
       0,   183,     0,     0,   184,     0,     0,     0,     0,     0,
     185,    86,   186,     0,    88,     0,     0,   187,   188,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     189,     0,     0,     0,   190,     0,     0,   191,     0,     0,
       0,   192,     0,     0,     0,     0,     0,   193,   194,     0,
     195,     0,     0,     0,     0,   196,   197,   198,   199,     0,
       0,   200,     0,   201,     0,   202,     0,   203,   204,   205,
     206,     0,     0,   207,   208,   209,     0,     0,   210,     0,
       0,     0,   211,   212,     0,   213,   214,   215,   216,     0,
       0,     0,     0,     0,     0,     0,     0,   217,     0,     0,
     218,     0,   219,   220,   221,   222,     0,     0,     0,     0,
       0,   223,     0,     0,     0,     0,   224,     0,     0,     0,
       0,     0,   225,     0,     0,     0,   226,   227,   228,   229,
     230,   231,   232,     0,   233,   234,     0,     0,     0,   235,
     236,   237,   238,     0,     0,   239,     0,   240,   241,   242,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     243,   244,     0,     0,   245,     0,     0,   246,   247,   248,
       0,     0,   249,     0,   250,   251,     0,     0,   252,     0,
       0,   253,     0,   254,     0,   255,   256,     0,     0,   257,
       0,     0,   258,   259,     0,   260,   261,     0,     0,     0,
     262,     0,     0,     0,     0,   263,     0,     0,     0,     0,
     264,     0,   265,     0,   125,   266,   126,   127,   128,     0,
     267,   268,     0,   129,     0,     0,   269,   130,   131,   132,
     270,   133,     0,     0,     0,     0,   134,     0,   135,   136,
       0,   137,     0,     0,     0,   138,     0,     0,     0,   139,
       0,     0,   140,   282,   141,   142,     0,   143,   144,     0,
     145,   146,   147,   148,   149,   150,     0,   151,   152,     0,
     153,     0,     0,     0,     0,   154,     0,     0,     0,     0,
     155,   156,   157,     0,     0,   158,     0,     0,     0,     0,
     159,   160,     0,     0,     0,     0,     0,     0,     0,     0,
     161,   162,     0,     0,   163,     0,     0,   164,   165,   166,
       0,     0,   167,     0,   168,   169,     0,   170,   171,     0,
     172,     0,   173,   174,     0,     0,   175,     0,     0,   176,
     177,   178,     0,   179,     0,     0,     0,   180,   181,     0,
     182,     0,   183,     0,     0,   184,     0,     0,     0,     0,
       0,   185,    86,   186,     0,    88,     0,     0,   187,   188,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   189,     0,     0,     0,   190,     0,     0,   191,     0,
       0,     0,   192,     0,     0,     0,     0,     0,   193,   194,
       0,   195,     0,     0,     0,     0,   196,   197,   198,   199,
       0,     0,   200,     0,   201,     0,   202,     0,   203,   204,
     205,   206,     0,     0,   207,   208,   209,     0,     0,   210,
       0,     0,     0,   211,   212,     0,   213,   214,   215,   216,
       0,     0,     0,     0,     0,     0,     0,     0,   217,     0,
       0,   218,     0,   219,   220,   221,   222,     0,     0,     0,
       0,     0,   223,     0,     0,     0,     0,   224,     0,     0,
       0,     0,     0,   225,     0,     0,     0,   226,   227,   228,
     229,   230,   231,   232,     0,   233,   234,     0,     0,     0,
     235,   236,   237,   238,     0,     0,   239,     0,   240,   241,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   243,   244,     0,     0,   245,     0,     0,   246,   247,
     248,     0,     0,   249,     0,   250,   251,     0,     0,   252,
       0,     0,   253,     0,   254,     0,   255,   256,     0,     0,
     257,     0,     0,   258,   259,     0,   260,   261,     0,     0,
       0,   262,     0,     0,     0,     0,   263,     0,     0,     0,
       0,   264,     0,   265,     0,   125,   266,   126,   127,   128,
       0,   267,   268,     0,   129,     0,     0,   269,   130,   131,
     132,   270,   133,     0,     0,     0,     0,   134,     0,   135,
     136,     0,   137,     0,     0,     0,   138,     0,     0,     0,
     139,     0,     0,   140,   397,   141,   142,     0,   143,   144,
       0,   145,   146,   147,   148,   149,   150,     0,   151,   152,
       0,   153,     0,     0,     0,     0,   154,     0,     0,     0,
       0,   155,   156,   157,     0,     0,   158,     0,     0,     0,
       0,   159,   160,     0,     0,     0,     0,     0,     0,     0,
       0,   161,   162,     0,     0,   163,     0,     0,   164,   165,
     166,     0,     0,   167,     0,   168,   169,     0,   170,   171,
       0,   172,     0,   173,   174,     0,     0,   175,     0,     0,
     176,   177,   178,     0,   179,     0,     0,     0,   180,   181,
       0,   182,     0,   183,     0,     0,   184,     0,     0,     0,
       0,     0,   185,    86,   186,     0,    88,     0,     0,   187,
     188,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   189,     0,     0,     0,   190,     0,     0,   191,
       0,     0,     0,   192,     0,     0,     0,     0,     0,   193,
     194,     0,   195,     0,     0,     0,     0,   196,   197,   198,
     199,     0,     0,   200,     0,   201,     0,   202,     0,   203,
     204,   205,   206,     0,     0,   207,   208,   209,     0,     0,
     210,     0,     0,     0,   211,   212,     0,   213,   214,   215,
     216,     0,     0,     0,     0,     0,     0,     0,     0,   217,
       0,     0,   218,     0,   219,   220,   221,   222,     0,     0,
       0,     0,     0,   223,     0,     0,     0,     0,   224,     0,
       0,     0,     0,     0,   225,     0,     0,     0,   226,   227,
     228,   229,   230,   231,   232,     0,   233,   234,     0,     0,
       0,   235,   236,   237,   238,     0,     0,   239,     0,   240,
     241,   242,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   243,   244,     0,     0,   245,     0,     0,   246,
     247,   248,     0,     0,   249,     0,   250,   251,     0,     0,
     252,     0,     0,   253,     0,   254,     0,   255,   256,     0,
       0,   257,     0,     0,   258,   259,     0,   260,   261,     0,
       0,     0,   262,     0,     0,     0,     0,   263,     0,     0,
       0,     0,   264,     0,   265,     0,   125,   266,   126,   127,
     128,     0,   267,   268,     0,   129,     0,     0,   269,   130,
     131,   132,   270,   133,     0,     0,     0,     0,   134,     0,
     135,   136,     0,   137,     0,     0,     0,   138,     0,     0,
       0,   139,     0,     0,   140,  1308,   141,   142,     0,   143,
     144,     0,   145,   146,   147,   148,   149,   150,     0,   151,
     152,     0,   153,     0,     0,     0,     0,   154,     0,     0,
       0,     0,   155,   156,   157,     0,     0,   158,     0,     0,
       0,     0,   159,   160,     0,     0,     0,     0,     0,     0,
       0,     0,   161,   162,     0,     0,   163,     0,     0,   164,
     165,   166,     0,     0,   167,     0,   168,   169,     0,   170,
     171,     0,   172,     0,   173,   174,     0,     0,   175,     0,
       0,   176,   177,   178,     0,   179,     0,     0,     0,   180,
     181,     0,   182,     0,   183,     0,     0,   184,     0,     0,
       0,     0,     0,   185,    86,   186,     0,    88,     0,     0,
     187,   188,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   189,     0,     0,     0,   190,     0,     0,
     191,     0,     0,     0,   192,     0,     0,     0,     0,     0,
     193,   194,     0,   195,     0,     0,     0,     0,   196,   197,
     198,   199,     0,     0,   200,     0,   201,     0,   202,     0,
     203,   204,   205,   206,     0,     0,   207,   208,   209,     0,
       0,   210,     0,     0,     0,   211,   212,     0,   213,   214,
     215,   216,     0,     0,     0,     0,     0,     0,     0,     0,
     217,     0,     0,   218,     0,   219,   220,   221,   222,     0,
       0,     0,     0,     0,   223,     0,     0,     0,     0,   224,
       0,     0,     0,     0,     0,   225,     0,     0,     0,   226,
     227,   228,   229,   230,   231,   232,     0,   233,   234,     0,
       0,     0,   235,   236,   237,   238,     0,     0,   239,     0,
     240,   241,   242,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   243,   244,     0,     0,   245,     0,     0,
     246,   247,   248,     0,     0,   249,     0,   250,   251,     0,
       0,   252,     0,     0,   253,     0,   254,     0,   255,   256,
       0,     0,   257,     0,     0,   258,   259,     0,   260,   261,
       0,     0,     0,   262,     0,     0,     0,     0,   263,     0,
       0,     0,     0,   264,     0,   265,     0,   125,   266,   126,
     127,   128,     0,   267,   268,     0,   129,     0,     0,   269,
     130,   131,   132,   270,   133,     0,     0,     0,     0,   134,
       0,   135,   136,     0,   137,  1250,     0,     0,   138,     0,
       0,     0,   139,     0,     0,   140,     0,   141,   142,     0,
     143,   144,     0,   145,   146,   147,   148,   149,   150,     0,
     151,   152,     0,   153,     0,     0,     0,     0,   154,     0,
       0,     0,     0,   155,   156,   157,     0,     0,   158,     0,
       0,     0,     0,   159,   160,     0,     0,     0,     0,     0,
       0,     0,     0,   161,   162,     0,     0,   163,     0,     0,
     164,   165,   166,     0,     0,   167,     0,   168,   169,     0,
     170,   171,     0,   172,     0,   173,   174,     0,     0,   175,
       0,     0,   176,   177,   178,     0,   179,     0,     0,     0,
     180,   181,     0,   182,     0,   183,     0,     0,   184,     0,
       0,     0,     0,     0,   185,    86,   186,     0,    88,     0,
       0,   187,   188,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   189,     0,     0,     0,   190,     0,
       0,   191,     0,     0,     0,   192,     0,     0,     0,     0,
       0,   193,   194,     0,   195,     0,     0,     0,     0,   196,
     197,   198,   199,     0,     0,   200,     0,   201,     0,   202,
       0,   203,   204,   205,   206,     0,     0,   207,   208,   209,
       0,     0,   210,     0,     0,     0,   211,   212,     0,   213,
     214,   215,   216,     0,     0,     0,     0,     0,     0,     0,
       0,   217,     0,     0,   218,     0,   219,   220,   221,   222,
       0,     0,     0,     0,     0,   223,     0,     0,     0,     0,
     224,     0,     0,     0,     0,     0,   225,     0,     0,     0,
     226,   227,   228,   229,   230,   231,   232,     0,   233,   234,
       0,     0,     0,   235,   236,   237,   238,     0,     0,   239,
       0,   240,   241,   242,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   243,   244,     0,     0,   245,     0,
       0,   246,   247,   248,     0,     0,   249,     0,   250,   251,
       0,     0,   252,     0,     0,   253,     0,   254,     0,   255,
     256,     0,     0,   257,     0,     0,   258,   259,     0,   260,
     261,     0,     0,     0,   262,     0,     0,     0,     0,   263,
       0,     0,     0,     0,   264,     0,   265,     0,   125,   266,
     126,   127,   128,     0,   267,   268,     0,   129,     0,     0,
     269,   130,   131,   132,   270,   133,     0,     0,     0,   668,
     134,     0,   135,   136,     0,   137,  1586,     0,     0,   138,
       0,     0,     0,   139,     0,     0,   140,     0,   141,   142,
       0,   143,   144,     0,   145,   146,   147,   148,   149,   150,
       0,   151,   152,     0,   153,     0,     0,     0,     0,   154,
       0,     0,     0,     0,   155,   156,   157,     0,     0,   158,
       0,     0,     0,     0,   159,   160,     0,     0,     0,     0,
       0,     0,     0,     0,   161,   162,     0,     0,   163,     0,
       0,   164,   165,   166,     0,     0,   167,     0,   168,   169,
       0,   170,   171,     0,   172,     0,   173,   174,     0,     0,
     175,     0,     0,   176,   177,   178,     0,   179,     0,     0,
       0,   180,   181,     0,   182,     0,   183,     0,     0,   184,
       0,   669,     0,     0,     0,   185,    86,   186,     0,    88,
       0,     0,   187,   188,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   189,     0,     0,     0,   190,
       0,     0,   191,     0,     0,     0,   192,     0,     0,     0,
       0,     0,   193,   194,     0,   195,     0,     0,     0,     0,
     196,   197,   198,   199,     0,     0,   200,     0,   201,     0,
     202,     0,   203,   204,   205,   206,     0,     0,   207,   208,
     209,     0,     0,   210,     0,     0,     0,   211,   212,     0,
     213,   214,   215,   216,     0,     0,     0,     0,     0,     0,
       0,     0,   217,     0,     0,   218,     0,   219,   220,   221,
     222,     0,     0,     0,     0,     0,   223,     0,     0,     0,
       0,   224,     0,     0,     0,     0,     0,   225,     0,     0,
       0,   226,   227,   228,   229,   230,   231,   232,     0,   233,
     234,     0,     0,     0,   235,   236,   237,   238,     0,     0,
     239,     0,   240,   241,   242,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   243,   244,     0,     0,   245,
       0,     0,   246,   247,   248,     0,     0,   249,     0,   250,
     251,     0,     0,   252,     0,   375,   253,     0,   254,     0,
     255,   256,     0,     0,   257,     0,     0,   258,   259,     0,
     260,   261,     0,     0,     0,   262,     0,     0,     0,     0,
     263,     0,     0,     0,     0,   264,     0,   265,     0,   125,
     266,   126,   127,   128,     0,   267,   268,     0,   129,  1246,
       0,   269,   130,   131,   132,   270,   133,     0,     0,     0,
       0,   134,     0,   135,   136,     0,   137,     0,     0,     0,
     138,     0,     0,     0,   139,     0,     0,   140,     0,   141,
     142,     0,   143,   144,     0,   145,   146,   147,   148,   149,
     150,     0,   151,   152,     0,   153,     0,     0,     0,     0,
     154,     0,     0,     0,     0,   155,   156,   157,     0,     0,
     158,     0,     0,     0,     0,   159,   160,     0,     0,     0,
       0,     0,     0,     0,     0,   161,   162,     0,     0,   163,
       0,     0,   164,   165,   166,     0,     0,   167,     0,   168,
     169,     0,   170,   171,     0,   172,     0,   173,   174,     0,
       0,   175,     0,     0,   176,   177,   178,     0,   179,     0,
       0,     0,   180,   181,     0,   182,     0,   183,     0,     0,
     184,     0,     0,     0,     0,     0,   185,    86,   186,     0,
      88,     0,     0,   187,   188,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   189,     0,     0,     0,
     190,     0,     0,   191,     0,     0,     0,   192,     0,     0,
       0,     0,     0,   193,   194,     0,   195,     0,     0,     0,
       0,   196,   197,   198,   199,     0,     0,   200,     0,   201,
       0,   202,     0,   203,   204,   205,   206,     0,     0,   207,
     208,   209,     0,     0,   210,     0,     0,     0,   211,   212,
       0,   213,   214,   215,   216,     0,     0,     0,     0,     0,
       0,     0,     0,   217,     0,     0,   218,     0,   219,   220,
     221,   222,     0,     0,     0,     0,     0,   223,     0,     0,
       0,     0,   224,     0,     0,     0,     0,     0,   225,     0,
       0,     0,   226,   227,   228,   229,   230,   231,   232,     0,
     233,   234,     0,     0,     0,   235,   236,   237,   238,     0,
       0,   239,     0,   240,   241,   242,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,     0,     0,
     245,     0,     0,   246,   247,   248,     0,     0,   249,     0,
     250,   251,     0,     0,   252,     0,    91,   253,     0,   254,
       0,   255,   256,     0,     0,   257,     0,     0,   258,   259,
       0,   260,   261,     0,     0,     0,   262,     0,     0,     0,
       0,   263,     0,     0,     0,     0,   264,     0,   265,     0,
     125,   266,   126,   127,   128,     0,   267,   268,     0,   129,
       0,     0,   269,   130,   131,   132,   270,   133,     0,     0,
       0,     0,   134,     0,   135,   136,     0,   137,     0,     0,
       0,   138,     0,     0,     0,   139,     0,     0,   140,     0,
     141,   142,     0,   143,   144,     0,   145,   146,   147,   148,
     149,   150,     0,   151,   152,     0,   153,     0,     0,     0,
       0,   154,     0,     0,     0,     0,   155,   156,   157,     0,
       0,   158,     0,     0,     0,     0,   159,   160,     0,     0,
       0,     0,     0,     0,     0,     0,   161,   162,     0,     0,
     163,     0,     0,   164,   165,   166,     0,     0,   167,     0,
     168,   169,     0,   170,   171,     0,   172,     0,   173,   174,
       0,     0,   175,     0,     0,   176,   177,   178,     0,   179,
       0,     0,     0,   180,   181,     0,   182,     0,   183,     0,
       0,   184,     0,     0,     0,     0,     0,   185,    86,   186,
       0,    88,     0,     0,   187,   188,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   189,     0,     0,
       0,   190,     0,     0,   191,     0,     0,     0,   192,     0,
       0,     0,     0,     0,   193,   194,     0,   195,     0,     0,
       0,     0,   196,   197,   198,   199,     0,     0,   200,     0,
     201,     0,   202,     0,   203,   204,   205,   206,     0,     0,
     207,   208,   209,     0,     0,   210,     0,     0,     0,   211,
     212,     0,   213,   214,   215,   216,     0,     0,     0,     0,
       0,     0,     0,     0,   217,     0,     0,   218,     0,   219,
     220,   221,   222,     0,     0,     0,     0,     0,   223,     0,
       0,     0,     0,   224,     0,     0,     0,     0,     0,   225,
       0,     0,     0,   226,   227,   228,   229,   230,   231,   232,
       0,   233,   234,     0,     0,     0,   235,   236,   237,   238,
       0,     0,   239,     0,   240,   241,   242,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   243,   244,     0,
       0,   245,     0,     0,   246,   247,   248,     0,     0,   249,
       0,   250,   251,     0,     0,   252,     0,    91,   253,     0,
     254,     0,   255,   256,     0,     0,   257,     0,     0,   258,
     259,     0,   260,   261,     0,     0,     0,   262,     0,     0,
       0,     0,   263,     0,     0,     0,     0,   264,     0,   265,
       0,   125,   266,   126,   127,   128,     0,   267,   268,     0,
     129,     0,     0,   269,   130,   131,   132,   270,   133,     0,
       0,     0,     0,   134,     0,   135,   136,     0,   137,     0,
       0,     0,   138,     0,     0,     0,   139,     0,     0,   140,
       0,   141,   142,     0,   143,   144,     0,   145,   146,   147,
     148,   149,   150,     0,   151,   152,     0,   153,     0,     0,
       0,     0,   154,     0,     0,     0,     0,   155,   156,   157,
       0,     0,   158,     0,     0,     0,     0,   159,   160,     0,
       0,     0,     0,     0,     0,     0,     0,   161,   162,     0,
       0,   163,     0,     0,   164,   165,   166,     0,     0,   167,
       0,   168,   169,     0,   170,   171,     0,   172,     0,   173,
     174,     0,     0,   175,     0,     0,   176,   177,   178,     0,
     179,     0,     0,     0,   180,   181,     0,   182,     0,   183,
       0,     0,   184,     0,     0,     0,     0,     0,   185,    86,
     186,     0,    88,     0,     0,   187,   188,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   189,     0,
       0,     0,   190,     0,     0,   191,     0,     0,     0,   192,
       0,     0,     0,     0,     0,   193,   194,     0,   195,     0,
       0,     0,     0,   196,   197,   198,   199,     0,     0,   200,
       0,   201,     0,   202,     0,   203,   204,   205,   206,     0,
       0,   207,   208,   209,     0,     0,   210,     0,     0,     0,
     211,   212,     0,   213,   214,   215,   216,     0,     0,     0,
       0,     0,     0,     0,     0,   217,     0,     0,   218,  2064,
     219,   220,   221,   222,     0,     0,     0,     0,     0,   223,
       0,     0,     0,     0,   224,     0,     0,     0,     0,     0,
     225,     0,     0,     0,   226,   227,   228,   229,   230,   231,
     232,     0,   233,   234,     0,     0,     0,   235,   236,   237,
     238,     0,     0,   239,     0,   240,   241,   242,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   243,   244,
       0,     0,   245,     0,     0,   246,   247,   248,     0,     0,
     249,     0,   250,   251,     0,     0,   252,     0,     0,   253,
       0,   254,     0,   255,   256,     0,     0,   257,     0,     0,
     258,   259,     0,   260,   261,     0,     0,     0,   262,     0,
       0,     0,     0,   263,     0,     0,     0,     0,   264,     0,
     265,     0,   125,   266,   126,   127,   128,     0,   267,   268,
       0,   129,     0,     0,   269,   130,   131,   132,   270,   133,
       0,     0,     0,     0,   134,     0,   135,   136,     0,   137,
       0,     0,     0,   138,     0,     0,     0,   139,     0,     0,
     140,     0,   141,   142,     0,   143,   144,     0,   145,   146,
     147,   148,   149,   150,     0,   151,   152,     0,   153,     0,
       0,     0,     0,   154,     0,     0,     0,     0,   155,   156,
     157,     0,     0,   158,     0,     0,     0,     0,   159,   160,
       0,     0,     0,     0,     0,     0,     0,     0,   161,   162,
       0,     0,   163,     0,     0,   164,   165,   166,     0,     0,
     167,     0,   168,   169,     0,   170,   171,     0,   172,     0,
     173,   174,     0,     0,   175,     0,     0,   176,   177,   178,
       0,   179,     0,     0,     0,   180,   181,     0,   182,     0,
     183,     0,     0,   184,     0,     0,     0,     0,     0,   185,
      86,   186,     0,    88,     0,     0,   187,   188,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   189,
       0,     0,     0,   190,     0,     0,   191,     0,     0,     0,
     192,     0,     0,     0,     0,     0,   193,   194,     0,   195,
       0,     0,     0,     0,   196,   197,   198,   199,     0,     0,
     200,     0,   201,     0,   202,     0,   203,   204,   205,   206,
       0,     0,   207,   208,   209,     0,     0,   210,     0,     0,
       0,   211,   212,     0,   213,   214,   215,   216,     0,     0,
       0,     0,     0,     0,     0,     0,   217,     0,     0,   218,
       0,   219,   220,   221,   222,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,   224,     0,     0,     0,     0,
       0,   225,     0,     0,     0,   226,   227,   228,   229,   230,
     231,   232,     0,   233,   234,     0,     0,     0,   235,   236,
     237,   238,     0,     0,   239,     0,   240,   241,   242,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   243,
     244,     0,     0,   245,     0,     0,   246,   247,   248,     0,
       0,   249,     0,   250,   251,     0,     0,   252,     0,     0,
     253,     0,   254,     0,   255,   256,     0,     0,   257,     0,
       0,   258,   259,     0,   260,   261,     0,     0,     0,   262,
       0,     0,     0,     0,   263,     0,     0,     0,     0,   264,
       0,   265,     0,     0,   266,     0,     0,     0,     0,   267,
     268,     0,     0,     0,     0,   269,     0,     0,     0,   270
};

static const yytype_int16 yycheck[] =
{
      12,    51,    93,   330,   428,    12,   613,   914,  1053,    21,
     905,   334,   534,   897,  1089,   319,  1218,    29,  1000,   410,
     511,   512,   109,   587,  1046,   996,   910,   872,   854,   107,
     527,   595,   359,  1187,   121,   666,  1418,  1397,   457,    51,
    1483,   446,   903,  1117,  1279,    12,  1552,   374,    14,   525,
    1531,    63,   457,   376,    12,    12,    46,   384,   385,    12,
    1214,    12,    74,  1034,   651,  1594,  1630,   654,    12,    81,
      12,   658,   659,   298,   124,   865,   683,    50,   101,   110,
      93,    93,    12,   474,    14,    12,    93,    12,   392,   393,
     394,  1632,    86,    12,   570,   107,    12,   109,    69,    12,
      12,    12,   845,   106,    12,    82,   118,  1621,   124,   121,
      12,   118,   124,    46,  1068,  1069,   103,  1631,  1120,  1633,
      85,    25,   112,  1286,   111,   863,    43,   230,   476,    29,
      39,   147,   523,   223,   142,  1237,   144,   145,   124,    46,
     214,    12,   267,   118,    85,   122,    12,  1706,  1468,   130,
     132,    51,    97,    15,   176,     7,    12,  1686,  1717,  1040,
     137,   147,    12,    63,    57,   173,  1717,  1707,    12,   174,
     561,    12,   182,   278,    74,   118,   129,   142,  1062,     9,
     145,    81,   292,    12,   197,   661,   275,    12,  1083,   126,
    1085,    96,   165,    12,    20,   329,   178,   129,   150,    12,
      12,   211,    12,  1098,    51,   182,   129,  1091,  1092,   109,
     356,  1413,   209,   357,   166,  1099,    12,    18,    66,    12,
     305,   121,    71,   308,   124,    87,  1110,   159,   333,   316,
    1465,   290,    12,   137,   211,   222,   346,   383,   260,  1474,
    1101,    31,    12,   180,    70,    46,   380,   228,   245,    50,
     300,   310,   175,   101,   379,     6,   381,    12,  1701,    10,
    1642,   666,    92,   129,  1823,    12,   224,   224,   120,   210,
     282,   224,  1823,    12,   363,   267,   379,   289,   286,   291,
     224,   226,   224,  1603,    85,   297,    12,    12,   300,  1115,
     272,   303,   304,   383,   224,   599,    12,   224,   279,   224,
     209,   126,   889,   311,   316,   224,   107,   315,   224,   383,
     614,   224,   224,   224,    12,   325,   224,  1055,   119,  1421,
     351,   337,   224,   310,   821,    12,   329,   244,  1771,    12,
     353,    12,  1495,  1217,   421,   683,  1338,   308,    12,    43,
     334,   142,  1223,   236,   145,   135,   311,   322,   325,   354,
    1793,   337,   318,   224,  1556,    12,  1263,    12,   224,  1888,
     373,   374,   375,   376,  1599,   338,  1847,   318,   224,   274,
     420,  1255,    12,  1913,   224,  1939,   366,   385,   854,   322,
     224,    12,   282,   224,   396,   397,    12,   379,  1463,   289,
    1344,   291,   359,  1899,  1220,   224,  1937,   297,    12,   224,
     300,   359,   359,   303,   304,   224,   359,   311,   420,   421,
    1305,   224,   224,    12,   224,   359,   316,   359,   643,  1209,
    1984,   125,   380,   380,  1938,   383,   383,   477,   224,   359,
     383,   224,   359,   366,   359,  1509,   380,  1511,   488,   383,
     359,   932,    12,   359,   224,  1306,   359,   359,   359,   250,
    1014,   359,   943,   380,   224,   380,   383,   359,   383,   366,
     261,   380,   474,   475,   383,   477,   382,   380,  1211,   224,
     383,   383,   383,  1080,   965,   383,   488,   224,  1065,   326,
     899,   383,    12,   495,  1866,   224,  1967,  1968,   359,  1970,
     502,    12,   583,   359,   899,   147,   396,   397,   224,   224,
      12,  1023,  1024,   359,  1985,  1112,  1113,  1950,   224,   359,
     311,  1337,   383,   525,   380,   359,  1013,  1877,   359,  1962,
     420,   421,   826,  1020,  1155,    38,   224,   383,   232,  2010,
     359,  2012,  2013,   383,   359,    12,   653,   224,  1692,   383,
     359,   224,   383,   224,  1422,   147,   359,   359,   626,   359,
     224,    12,  1430,    12,   383,   380,    69,    12,   570,  1035,
      12,    12,   263,   359,   383,    12,   359,   224,   380,   224,
     383,   583,   202,   383,   474,   475,   583,   477,    14,   359,
    1628,    82,   383,  1337,   224,   215,    43,   383,   488,   359,
     383,   641,   679,   224,   644,   495,  1072,   249,   224,  1429,
       9,   613,   502,   383,   359,   617,   656,  1452,  1579,    34,
     224,    12,   359,   383,   626,   221,   311,   129,    12,    12,
     359,   122,  1046,    12,  1646,   224,   330,    69,   383,   641,
    1675,   267,   644,   359,   359,     9,   383,   687,   645,  1203,
     652,   380,   320,   359,   656,    14,    29,   291,   308,   661,
      12,    12,  1559,   175,   224,   380,  1283,   383,    12,    12,
      12,   359,    12,    12,   380,    12,   147,   679,   125,    12,
     106,   683,   359,   685,    12,   687,   359,    12,   359,  1286,
      63,   182,   380,    92,   379,   359,  1313,  1294,   118,  1515,
    1738,  1673,   129,   380,   224,    64,   267,   380,     0,   380,
     213,   223,   359,   224,   359,   218,   380,  1591,   338,  1200,
     211,   147,   224,  1017,   313,    12,    12,   617,    92,   359,
     157,   142,     9,   380,   145,   380,   142,   118,   359,   145,
    1623,   267,    12,   359,   634,  1628,  1566,   106,    12,   106,
     380,   641,  1910,   379,   644,   359,   442,   224,  1224,   380,
    1795,  1953,   652,   267,   380,   124,   656,   126,    12,   876,
     359,  1515,   458,   224,   133,   224,   380,   136,    49,   224,
       9,   213,   224,   224,   891,   232,   218,   224,   147,   679,
     210,  1688,   146,   152,   209,   685,   223,   687,   157,   359,
     802,  1666,  1667,   267,   163,   308,   309,    93,   848,   849,
    1678,    22,   171,   308,  1682,    92,   175,   329,   379,  1977,
     380,   180,    85,   249,   267,  1619,  1620,   562,   563,   210,
    1895,  1896,  1298,   224,   325,  1347,  1630,   252,  1632,   359,
     224,   224,   129,   202,   250,   224,   848,   849,   359,    93,
    1362,  1363,   854,   379,   125,  1738,   215,   359,  1678,   274,
     380,   495,  1682,    92,   223,   129,  1378,   129,   502,   380,
     872,   313,   224,   224,   146,   379,   878,  1619,  1620,   150,
     224,   224,   224,   330,   224,   224,  1704,   224,  1630,  1370,
    1632,   224,   359,   150,     9,   166,   224,   256,  1495,   224,
     311,   903,   322,   166,  1071,   311,  1073,     9,   359,   911,
     359,   197,   802,   380,   359,   379,   355,   359,   359,   175,
    1714,  1715,   359,  1404,   297,   142,  1007,   144,   145,   380,
     303,   380,  1726,  1435,  1436,   380,   379,   224,   297,   380,
    1734,   322,  1736,   380,   207,    18,  1412,   210,   159,  1999,
     331,  2001,   215,   197,  1748,   355,   380,  1751,   848,   849,
     224,   232,    82,   174,   854,  1785,  1786,   223,   359,   232,
     329,   384,  1714,  1715,    46,   359,   359,    92,   337,   338,
     359,  1428,  1063,   372,  1726,   311,  1804,  1805,   878,   380,
      92,   380,  1734,   308,  1736,   354,   380,   380,  2033,  1873,
     207,   380,   122,    53,    54,  1007,  1748,   359,   359,  1751,
    1007,   330,    85,    85,   133,   359,   359,   359,   384,   359,
     359,   380,   359,   382,   383,    22,   359,  1029,   380,   380,
     115,   359,   243,  1035,   359,    85,   380,   380,   380,  1273,
     380,   380,    93,   380,   163,  1279,   209,   380,  1282,    99,
    1284,    25,   380,   379,  1056,   380,  1058,  1291,  1292,  1056,
    1945,  1063,   182,  1297,  1231,   163,  1063,   330,    64,   286,
    1072,   121,   359,   171,  1114,    55,    38,  1982,  1080,   292,
     355,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   211,   383,   166,   166,   359,    93,   379,   315,  1101,
    1997,    46,   311,   320,   223,  1107,   372,    69,    82,    55,
    1112,  1113,  1114,  1115,   380,  1117,   379,   384,  1120,  1716,
    1567,  1568,   373,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   142,   207,   207,   145,   210,   210,  1029,
      85,    43,   379,  1937,  1610,  1939,   197,    49,   122,   379,
    1531,   379,    18,  1447,  2059,  2060,   152,    59,   256,   232,
     232,   380,   159,   137,   383,   379,  1056,   163,  1058,   109,
     285,   286,   287,   288,   379,   171,    46,   174,   380,    75,
      46,   383,   297,     2,    50,     4,    82,   379,   150,   132,
    1984,   131,   242,  1987,   379,  1937,  1236,  1939,   271,    18,
     197,   380,   379,   379,   383,   325,   202,   379,   182,   142,
     207,   144,   145,   380,   379,    85,   383,  1107,   379,    85,
     160,   166,  1224,   125,  1114,  1115,   122,  1229,   379,   169,
     380,  1465,  1646,   383,  1236,   178,   210,   211,   379,   239,
    1474,   107,  1984,   379,  1246,  1987,   243,   247,   150,  1246,
     379,   213,   379,   119,   379,    64,   218,   330,   330,   263,
     256,  1301,   207,   203,   166,   210,    85,   379,   379,   118,
     270,  1273,   142,   216,   379,   145,   142,  1279,   132,   145,
    1282,   329,  1284,   379,  1286,   379,   182,   232,  1285,  1291,
    1292,   380,  1294,    24,   383,  1297,  1298,    28,  1295,  1301,
     379,   297,  1342,   122,  1306,  1307,  1308,  1773,   174,   379,
      41,  1313,   208,   379,   210,   211,  1313,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   379,   181,   175,   272,
     232,    85,    64,   379,  1224,   380,  1338,   311,   383,  1229,
    1342,    72,   379,   152,    75,  1622,  1236,   309,  1625,    85,
    1627,   325,    83,   286,   163,    85,  1246,   379,   207,   379,
      64,   210,   171,   380,   380,  1599,   383,   383,   379,   379,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     250,   379,   315,  1273,   250,   330,   382,   292,   380,  1279,
     104,   383,  1282,   202,  1284,   261,  1398,   371,   372,   305,
     267,  1291,  1292,   348,  1406,   380,   215,  1297,   383,  1406,
    1412,  1301,   166,   379,    18,   311,  1729,  1307,  1308,   315,
     152,   379,   379,   363,   364,   365,   379,   158,   330,   325,
     166,   163,    18,    25,   379,   379,   166,   379,   292,   171,
     379,   311,  1444,   379,  1757,   311,    50,   256,   152,   379,
    1452,   305,  1342,   207,   308,   379,   210,   379,  1839,   163,
      46,   215,  1502,  1465,    50,    93,  1847,   171,   379,   379,
     202,   207,  1474,   322,   210,   342,   343,   207,   232,  1860,
     210,    85,   331,    75,   379,   379,  1952,   379,   297,   142,
      82,   144,   145,  1495,  1807,  1808,   232,  1499,   202,    85,
    1502,  1582,   232,   107,  1506,   379,    93,  1509,  1398,  1511,
    1512,   215,   379,   379,   379,   119,  1406,   379,   379,  1836,
     173,   107,  1835,   379,   256,  1838,   118,   292,    14,    58,
     122,   159,   142,   119,   144,   145,   380,   380,   142,   383,
     383,   145,   380,    85,  1082,   137,  1084,   278,  1929,   274,
    1931,  1089,   256,   383,  1444,  1639,   142,  1095,   380,   145,
     267,   383,    39,   173,   245,   297,   115,   274,   380,   197,
     380,   383,   150,   383,  1576,  1465,   330,   215,    64,   380,
    1582,   254,   383,   314,  1474,  1582,  1967,  1968,   380,  1970,
     182,   383,   380,   297,   330,   383,  2062,  1599,   380,   350,
     330,   383,   333,   380,  1985,  1607,   383,   321,  1610,  1499,
     197,   380,  1502,   239,  2080,  1932,  1506,   380,   210,   211,
     106,  2087,  1512,   142,   380,   144,   145,   383,   380,  2010,
     353,  2012,  2013,   286,   348,   342,   343,    92,   380,   379,
     126,   383,     9,   380,   358,  1647,   250,   133,   151,   384,
     136,   380,   380,  1737,   173,   379,   383,   261,   311,  1743,
     318,  1745,   315,   383,   250,   383,   152,   373,   311,   280,
      46,   157,   379,   180,   354,   261,   286,   163,   380,   380,
     373,   395,   380,   380,   129,   171,  1576,   372,   380,   175,
     380,   380,   380,   380,   180,   329,   275,   383,   380,   383,
      12,   311,  1704,   380,   380,   315,   376,   311,  1710,  1599,
     379,   379,   379,   376,  1716,   380,   202,  1607,   384,   311,
     311,   385,   379,   315,   384,   311,   380,   383,    32,   215,
     322,   383,   385,   325,   223,   215,   380,   223,  1778,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   104,
     380,   156,   156,   380,   383,   380,  1753,  1647,   380,   380,
     384,   384,   380,  1765,   384,   380,   383,   286,   380,    93,
     256,  1773,   157,   129,   161,   385,  1778,    49,   318,   371,
     372,   164,   369,   370,   371,   372,   373,   374,   375,   376,
     504,   306,   311,   164,   306,   509,   315,    32,    46,   513,
     514,   383,  1804,  1805,   338,   215,   520,   521,   522,   175,
     524,   297,   526,    98,  1704,   379,   530,   531,   532,   533,
    1710,   379,   536,   158,   538,   539,   158,   541,   542,  1729,
     544,   383,   380,   547,   548,   549,   550,   551,   552,   380,
     384,   555,   556,   329,   558,   380,   560,   380,    93,   380,
     564,   565,   338,    10,    11,   383,   267,  1757,   383,   573,
     574,   334,   159,   384,    21,   172,  1868,   581,   354,   380,
     584,   585,   586,   197,   166,  1765,   166,    14,    48,   384,
     380,    14,   211,   211,  1886,   380,    43,   269,  1778,   157,
     380,   383,    62,    63,   380,    52,   382,   383,   163,   163,
      70,   133,   225,   385,   276,   383,    63,   166,   166,  1809,
    1810,   383,   379,  1915,  1804,  1805,   166,   380,   380,   380,
     634,   380,   163,   163,   215,    32,   136,   383,   194,    86,
      87,    88,   646,   239,    85,   379,    85,   379,   379,    96,
     110,   379,    82,   379,    50,   379,   334,  1944,   380,   106,
    1952,   384,   197,    48,   211,   308,   163,   114,  1858,  1859,
     163,   379,   132,   379,   184,   211,   123,    12,   210,    23,
     166,   166,   142,    27,    28,   145,   334,   211,  1868,   149,
     150,   379,   122,   211,   211,   211,    85,    41,   702,   379,
     338,   380,   338,   379,   164,  2076,  1886,  1999,   155,  2001,
     176,  2003,   142,   215,   208,   145,   380,   380,   178,   343,
     167,   362,   380,   211,   383,   380,   338,   338,    72,    32,
     177,    75,    32,   103,    32,  1915,   380,   379,   126,    83,
     379,    76,    77,    78,    79,    80,    32,    32,   752,    32,
       4,    95,   182,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   244,   108,  2057,  2058,   215,   147,   215,
    2062,    32,    32,   379,   234,   379,    86,   121,   782,   783,
     784,   211,   334,   211,  2076,   274,   379,  1643,  2080,  2076,
     380,  2083,   383,  1115,  1112,  2087,  1101,   683,   245,   246,
    1512,  1307,   249,   138,   139,   140,   141,  1081,  1310,  1107,
     257,  1607,   272,  1277,   158,  1138,   263,   821,  1609,  1999,
     267,  2001,  1744,  2003,  1726,  1849,  1626,   274,  1762,   833,
     277,   835,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,  1734,  1708,   291,   305,  1716,  1947,   308,   309,
    1976,   311,  1121,  1868,   189,   190,   191,   192,  1306,   848,
    1047,   277,   998,   198,   473,   276,   437,   862,   212,  1406,
     931,  1676,  1211,  1432,   997,  1964,   323,  2057,  2058,  2083,
    1898,   311,  2058,   515,  1674,   332,  1021,   334,  1454,   224,
     337,   351,   563,  1052,   316,   325,  1695,   241,   305,  1610,
     235,   940,   421,  2083,   696,  1219,  1072,  1412,   853,  2080,
    1952,   349,   501,  1954,   918,  1904,  1996,   921,  1974,   923,
     924,  2087,  1224,   634,   928,    81,  1313,   271,   867,   264,
     265,  1452,   379,  1254,  1700,   110,   427,    -1,   942,    -1,
      -1,   945,   946,   947,   948,    -1,   950,    80,    -1,     5,
      -1,    -1,    -1,   957,    10,    -1,   960,    -1,   962,   963,
     964,    -1,    18,    -1,    -1,   969,   970,   971,   312,    -1,
     314,   315,   976,   977,   978,    -1,   980,    -1,   982,    -1,
     984,    -1,   986,    -1,    40,    -1,    -1,   991,    -1,    -1,
      -1,    -1,    -1,    -1,    50,   128,    -1,   341,    -1,    -1,
     344,   345,     5,    -1,  1008,    61,    -1,    10,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    18,  1020,   352,    -1,    -1,
      -1,  1025,    -1,    -1,   359,   360,   361,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    90,    91,    -1,    40,    -1,    -1,
      96,    -1,    -1,    -1,    -1,    -1,   102,    50,    -1,    -1,
     106,   107,    -1,    -1,    -1,    -1,   189,    -1,    61,   192,
      -1,    -1,    -1,   119,    -1,   198,    -1,    -1,   124,    -1,
      -1,    -1,  1076,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    -1,    -1,    -1,   142,    90,    -1,   145,
      -1,    -1,   148,    96,    -1,    -1,    -1,    -1,    -1,   102,
      -1,    -1,   235,    -1,   107,    -1,    -1,    -1,    76,    77,
      78,    79,    80,    -1,    -1,    -1,   119,    -1,    -1,  1123,
      -1,   124,    -1,    -1,  1128,    -1,    -1,    -1,  1132,    -1,
      -1,    -1,   265,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     196,    -1,   145,   515,    -1,    -1,    -1,  1151,  1152,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   537,  1170,   223,   540,    -1,
     138,   139,   140,   141,  1178,    -1,  1180,    -1,  1182,    -1,
      -1,   553,   554,    -1,    -1,   557,    -1,    -1,    -1,    -1,
     246,    -1,    -1,   196,   250,    -1,    -1,    -1,    -1,   571,
     572,    -1,    -1,    -1,    -1,   261,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1218,   546,    -1,    -1,    -1,   352,
     223,   189,   190,   191,   192,    -1,    -1,    -1,   361,    -1,
     198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   246,    -1,    -1,    -1,   250,    -1,  1253,
      -1,    -1,    -1,    -1,    -1,   311,    -1,    -1,   261,    -1,
      -1,    -1,    -1,  1267,    -1,   596,   597,   235,    -1,   600,
      -1,   602,   603,   604,   605,   606,   607,   608,   609,   610,
     611,   612,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   264,   265,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   311,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   699,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1350,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1367,  1368,    -1,    -1,    -1,    -1,    -1,
      -1,  1375,    -1,    -1,    -1,    -1,  1380,    -1,    -1,  1383,
    1384,  1385,   754,    -1,   352,    -1,   758,    -1,    -1,    -1,
      -1,    -1,   360,   361,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   774,    -1,    -1,    -1,    -1,    -1,    -1,  1413,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1453,
      -1,    -1,    -1,    -1,    -1,    -1,  1460,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   824,    -1,    -1,   827,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1556,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1594,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1658,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1686,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1700,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     4,    -1,     6,     7,
       8,    -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,    17,
      18,    19,    -1,    21,    -1,    -1,    24,    25,    26,    -1,
      28,    29,    30,    31,    -1,    -1,    -1,    35,    36,    37,
      38,    39,    -1,    41,    42,    -1,    44,    45,    -1,    47,
      48,    -1,    50,    51,    52,    53,    54,    55,    -1,    57,
      58,    -1,    60,    61,    62,  1789,  1790,    65,    66,    67,
    1794,    69,    70,    71,    72,    73,    74,    75,    -1,    -1,
      -1,    -1,    80,    81,    82,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    90,    91,    -1,    -1,    94,    -1,    -1,    97,
      98,    99,    -1,    -1,   102,    -1,   104,   105,    -1,   107,
     108,    -1,   110,    -1,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    -1,    -1,    -1,   127,
     128,    -1,   130,    -1,   132,    -1,   134,   135,    -1,   137,
      -1,    -1,    -1,   141,   142,   143,    -1,   145,   146,    -1,
     148,   149,    -1,    -1,    -1,    -1,    -1,   155,   156,    -1,
      -1,    -1,    -1,   161,  1888,    -1,    -1,   165,    -1,   167,
     168,    -1,   170,   171,   172,    -1,    -1,    -1,    -1,    -1,
     178,   179,    -1,   181,   182,    -1,    -1,   185,   186,   187,
     188,   189,    -1,    -1,   192,   193,   194,    -1,   196,   197,
     198,   199,   200,   201,    -1,    -1,   204,   205,   206,   207,
     208,   209,   210,   211,    -1,   213,   214,    -1,   216,   217,
     218,   219,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1953,
     228,   229,  1956,   231,  1958,   233,   234,   235,   236,    -1,
      -1,    -1,    -1,    -1,   242,    -1,    -1,    -1,    -1,   247,
     248,   249,    -1,    -1,    -1,   253,    -1,    -1,   256,   257,
     258,   259,   260,   261,   262,   263,    -1,   265,   266,    -1,
      -1,    -1,   270,   271,   272,   273,    -1,    -1,   276,    -1,
     278,   279,   280,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   291,   292,   293,   294,   295,    -1,    -1,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,   311,   312,    -1,   314,   315,   316,   317,
      -1,   319,   320,   321,   322,   323,   324,   325,   326,   327,
      -1,    -1,    -1,   331,    -1,    -1,    -1,    -1,   336,    -1,
      -1,   339,   340,   341,   342,   343,    -1,    -1,   346,   347,
      -1,   349,   350,   351,   352,    -1,    -1,    -1,    -1,   357,
      -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   371,   372,    -1,    -1,    -1,    -1,   377,
      -1,   379,    -1,   381,    -1,    -1,   384,   385,     4,    -1,
       6,     7,     8,     9,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    17,    18,    19,    -1,    21,    -1,    -1,    24,    25,
      26,    -1,    28,    29,    -1,    31,    -1,    -1,    -1,    35,
      36,    37,    38,    39,    -1,    41,    42,    -1,    44,    45,
      -1,    47,    48,    -1,    50,    51,    52,    53,    54,    55,
      -1,    57,    58,    -1,    60,    61,    62,    -1,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      -1,    -1,    -1,    -1,    80,    81,    82,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    90,    91,    -1,    -1,    94,    -1,
      -1,    97,    98,    99,    -1,    -1,   102,    -1,   104,   105,
      -1,   107,   108,    -1,   110,    -1,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    -1,    -1,
      -1,   127,   128,    -1,   130,    -1,   132,    -1,   134,   135,
      -1,   137,    -1,    -1,    -1,   141,   142,   143,    -1,   145,
     146,    -1,   148,   149,    -1,    -1,    -1,    -1,    -1,   155,
     156,    -1,    -1,    -1,    -1,   161,    -1,    -1,    -1,   165,
      -1,   167,   168,    -1,    -1,   171,   172,    -1,    -1,    -1,
      -1,    -1,   178,   179,    -1,   181,   182,    -1,    -1,   185,
     186,   187,   188,   189,    -1,    -1,   192,   193,   194,    -1,
     196,   197,   198,   199,   200,   201,    -1,    -1,   204,   205,
     206,   207,   208,   209,   210,   211,    -1,   213,   214,   215,
     216,   217,   218,   219,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   228,   229,    -1,   231,    -1,   233,   234,   235,
     236,    -1,    -1,    -1,    -1,    -1,   242,    -1,    -1,    -1,
      -1,   247,   248,   249,    -1,    -1,    -1,   253,    -1,    -1,
     256,   257,   258,   259,   260,   261,   262,   263,    -1,   265,
     266,    -1,    -1,    -1,   270,   271,   272,   273,    -1,    -1,
     276,    -1,   278,   279,   280,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   291,   292,   293,   294,   295,
      -1,    -1,   298,   299,   300,   301,   302,   303,   304,   305,
     306,    -1,    -1,   309,    -1,   311,   312,    -1,   314,   315,
     316,   317,    -1,    -1,   320,   321,   322,   323,   324,   325,
     326,   327,    -1,    -1,    -1,   331,    -1,    -1,    -1,    -1,
     336,    -1,    -1,   339,   340,   341,   342,   343,    -1,    -1,
     346,   347,    -1,   349,   350,   351,   352,    -1,    -1,    -1,
      -1,   357,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   371,   372,    -1,    -1,    -1,
      -1,   377,    -1,   379,    -1,   381,    -1,    -1,   384,   385,
       4,    -1,     6,     7,     8,    -1,    -1,    -1,    -1,    13,
      -1,    -1,    -1,    17,    18,    19,    -1,    21,    -1,    -1,
      24,    25,    26,    -1,    28,    29,    -1,    31,    -1,    -1,
      -1,    35,    36,    37,    38,    39,    -1,    41,    42,    -1,
      44,    45,    -1,    47,    48,    -1,    50,    51,    52,    53,
      54,    55,    -1,    57,    58,    -1,    60,    61,    62,    -1,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    -1,    -1,    -1,    -1,    80,    81,    82,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    90,    91,    -1,    -1,
      94,    -1,    -1,    97,    98,    99,    -1,    -1,   102,    -1,
     104,   105,    -1,   107,   108,    -1,   110,    -1,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
      -1,    -1,    -1,   127,   128,    -1,   130,    -1,   132,    -1,
     134,   135,    -1,   137,    -1,    -1,    -1,   141,   142,   143,
      -1,   145,   146,    -1,   148,   149,    -1,    -1,    -1,    -1,
      -1,   155,   156,    -1,    -1,    -1,    -1,   161,    -1,    -1,
      -1,   165,    -1,   167,   168,    -1,    -1,   171,   172,    -1,
      -1,    -1,    -1,    -1,   178,   179,    -1,   181,   182,    -1,
      -1,   185,   186,   187,   188,   189,    -1,    -1,   192,   193,
     194,    -1,   196,   197,   198,   199,   200,   201,    -1,    -1,
     204,   205,   206,   207,   208,   209,   210,   211,    -1,   213,
     214,    -1,   216,   217,   218,   219,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   228,   229,    -1,   231,    -1,   233,
     234,   235,   236,    -1,    -1,    -1,    -1,    -1,   242,    -1,
      -1,    -1,    -1,   247,   248,   249,    -1,    -1,    -1,   253,
      -1,    -1,   256,   257,   258,   259,   260,   261,   262,   263,
      -1,   265,   266,    -1,    -1,    -1,   270,   271,   272,   273,
      -1,    -1,   276,    -1,   278,   279,   280,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   291,   292,   293,
     294,   295,    -1,    -1,   298,   299,   300,   301,   302,   303,
     304,   305,   306,    -1,    -1,   309,    -1,   311,   312,    -1,
     314,   315,   316,   317,    -1,    -1,   320,   321,   322,   323,
     324,   325,   326,   327,    -1,    -1,    -1,   331,    -1,    -1,
      -1,    -1,   336,    -1,    -1,   339,   340,   341,   342,   343,
      -1,    -1,   346,   347,    -1,   349,   350,   351,   352,    -1,
      -1,    -1,    -1,   357,    -1,    -1,    -1,   361,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   371,   372,    -1,
      -1,    -1,    -1,   377,    -1,   379,   380,   381,    -1,    -1,
     384,   385,     4,    -1,     6,     7,     8,    -1,    -1,    -1,
      -1,    13,    -1,    -1,    -1,    17,    18,    19,    -1,    21,
      -1,    -1,    24,    25,    26,    -1,    28,    29,    -1,    31,
      -1,    -1,    -1,    35,    36,    37,    38,    39,    -1,    41,
      42,    -1,    44,    45,    -1,    47,    48,    -1,    50,    51,
      52,    53,    54,    55,    -1,    57,    58,    -1,    60,    61,
      62,    -1,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    -1,    -1,    -1,    -1,    80,    81,
      82,    -1,    -1,    85,    -1,    -1,    -1,    -1,    90,    91,
      -1,    -1,    94,    -1,    -1,    97,    98,    99,    -1,    -1,
     102,    -1,   104,   105,    -1,   107,   108,    -1,   110,    -1,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,    -1,    -1,    -1,   127,   128,   129,   130,    -1,
     132,    -1,   134,   135,    -1,   137,    -1,    -1,    -1,   141,
     142,   143,    -1,   145,   146,    -1,   148,   149,    -1,    -1,
      -1,    -1,    -1,   155,   156,    -1,    -1,    -1,    -1,   161,
      -1,    -1,    -1,   165,    -1,   167,   168,    -1,    -1,   171,
     172,    -1,    -1,    -1,    -1,    -1,   178,   179,    -1,   181,
     182,    -1,    -1,   185,   186,   187,   188,   189,    -1,    -1,
     192,   193,   194,    -1,   196,   197,   198,   199,   200,   201,
      -1,    -1,   204,   205,   206,   207,   208,   209,   210,   211,
      -1,   213,   214,    -1,   216,   217,   218,   219,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   228,   229,    -1,   231,
      -1,   233,   234,   235,   236,    -1,    -1,    -1,    -1,    -1,
     242,    -1,    -1,    -1,    -1,   247,   248,   249,    -1,    -1,
      -1,   253,    -1,    -1,   256,   257,   258,   259,   260,   261,
     262,   263,    -1,   265,   266,    -1,    -1,    -1,   270,   271,
     272,   273,    -1,    -1,   276,    -1,   278,   279,   280,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   291,
     292,   293,   294,   295,    -1,    -1,   298,   299,   300,   301,
     302,   303,   304,   305,   306,    -1,    -1,   309,    -1,   311,
     312,    -1,   314,   315,   316,   317,    -1,    -1,   320,   321,
     322,   323,   324,   325,   326,   327,    -1,    -1,    -1,   331,
      -1,    -1,    -1,    -1,   336,    -1,    -1,   339,   340,   341,
     342,   343,    -1,    -1,   346,   347,    -1,   349,   350,   351,
     352,    -1,    -1,    -1,    -1,   357,    -1,    -1,    -1,   361,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   371,
     372,    -1,    -1,    -1,    -1,   377,    -1,   379,    -1,   381,
      -1,    -1,   384,   385,     4,    -1,     6,     7,     8,    -1,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    17,    18,    19,
      -1,    21,    -1,    -1,    24,    25,    26,    -1,    28,    29,
      -1,    31,    -1,    -1,    -1,    35,    36,    37,    38,    39,
      -1,    41,    42,    -1,    44,    45,    -1,    47,    48,    -1,
      50,    51,    52,    53,    54,    55,    -1,    57,    58,    -1,
      60,    61,    62,    -1,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    -1,    -1,    -1,    -1,
      80,    81,    82,    -1,    -1,    85,    -1,    -1,    -1,    -1,
      90,    91,    -1,    -1,    94,    -1,    -1,    97,    98,    99,
      -1,    -1,   102,    -1,   104,   105,    -1,   107,   108,    -1,
     110,    -1,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,    -1,   127,   128,   129,
     130,    -1,   132,    -1,   134,   135,    -1,   137,    -1,    -1,
      -1,   141,   142,   143,    -1,   145,   146,    -1,   148,   149,
      -1,    -1,    -1,    -1,    -1,   155,   156,    -1,    -1,    -1,
      -1,   161,    -1,    -1,    -1,   165,    -1,   167,   168,    -1,
      -1,   171,   172,    -1,    -1,    -1,    -1,    -1,   178,   179,
      -1,   181,   182,    -1,    -1,   185,   186,   187,   188,   189,
      -1,    -1,   192,   193,   194,    -1,   196,   197,   198,   199,
     200,   201,    -1,    -1,   204,   205,   206,   207,   208,   209,
     210,   211,    -1,   213,   214,    -1,   216,   217,   218,   219,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   228,   229,
      -1,   231,    -1,   233,   234,   235,   236,    -1,    -1,    -1,
      -1,    -1,   242,    -1,    -1,    -1,    -1,   247,   248,   249,
      -1,    -1,    -1,   253,    -1,    -1,   256,   257,   258,   259,
     260,   261,   262,   263,    -1,   265,   266,    -1,    -1,    -1,
     270,   271,   272,   273,    -1,    -1,   276,    -1,   278,   279,
     280,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   291,   292,   293,   294,   295,    -1,    -1,   298,   299,
     300,   301,   302,   303,   304,   305,   306,    -1,    -1,   309,
      -1,   311,   312,    -1,   314,   315,   316,   317,    -1,    -1,
     320,   321,   322,   323,   324,   325,   326,   327,    -1,    -1,
      -1,   331,    -1,    -1,    -1,    -1,   336,    -1,    -1,   339,
     340,   341,   342,   343,    -1,    -1,   346,   347,    -1,   349,
     350,   351,   352,    -1,    -1,    -1,    -1,   357,    -1,    -1,
      -1,   361,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   371,   372,    -1,    -1,    -1,    -1,   377,    -1,   379,
      -1,   381,    -1,    -1,   384,   385,     4,    -1,     6,     7,
       8,    -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,    17,
      18,    19,    -1,    21,    -1,    -1,    24,    25,    26,    -1,
      28,    29,    -1,    31,    -1,    -1,    -1,    35,    36,    37,
      38,    39,    -1,    41,    42,    -1,    44,    45,    -1,    47,
      48,    -1,    50,    51,    52,    53,    54,    55,    -1,    57,
      58,    -1,    60,    61,    62,    -1,    -1,    65,    66,    67,
      -1,    69,    70,    71,    72,    73,    74,    75,    -1,    -1,
      -1,    -1,    80,    81,    82,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    90,    91,    -1,    -1,    94,    -1,    -1,    97,
      98,    99,    -1,    -1,   102,    -1,   104,   105,    -1,   107,
     108,    -1,   110,    -1,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    -1,    -1,    -1,   127,
     128,   129,   130,    -1,   132,    -1,   134,   135,    -1,   137,
      -1,    -1,    -1,   141,   142,   143,    -1,   145,   146,    -1,
     148,   149,    -1,    -1,    -1,    -1,    -1,   155,   156,    -1,
      -1,    -1,    -1,   161,    -1,    -1,    -1,   165,    -1,   167,
     168,    -1,    -1,   171,   172,    -1,    -1,    -1,    -1,    -1,
     178,   179,    -1,   181,   182,    -1,    -1,   185,   186,   187,
     188,   189,    -1,    -1,   192,   193,   194,    -1,   196,   197,
     198,   199,   200,   201,    -1,    -1,   204,   205,   206,   207,
     208,   209,   210,   211,    -1,   213,   214,    -1,   216,   217,
     218,   219,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     228,   229,    -1,   231,    -1,   233,   234,   235,   236,    -1,
      -1,    -1,    -1,    -1,   242,    -1,    -1,    -1,    -1,   247,
     248,   249,    -1,    -1,    -1,   253,    -1,    -1,   256,   257,
     258,   259,   260,   261,   262,   263,    -1,   265,   266,    -1,
      -1,    -1,   270,   271,   272,   273,    -1,    -1,   276,    -1,
     278,   279,   280,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   291,   292,   293,   294,   295,    -1,    -1,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,   311,   312,    -1,   314,   315,   316,   317,
      -1,    -1,   320,   321,   322,   323,   324,   325,   326,   327,
      -1,    -1,    -1,   331,    -1,    -1,    -1,    -1,   336,    -1,
      -1,   339,   340,   341,   342,   343,    -1,    -1,   346,   347,
      -1,   349,   350,   351,   352,    -1,    -1,    -1,    -1,   357,
      -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   371,   372,    -1,    -1,    -1,    -1,   377,
      -1,   379,    -1,   381,    -1,    -1,   384,   385,     4,    -1,
       6,     7,     8,    -1,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    17,    18,    19,    -1,    21,    -1,    -1,    24,    25,
      26,    -1,    28,    29,    -1,    31,    -1,    -1,    -1,    35,
      36,    37,    38,    39,    -1,    41,    42,    -1,    44,    45,
      -1,    47,    48,    -1,    50,    51,    52,    53,    54,    55,
      -1,    57,    58,    -1,    60,    61,    62,    -1,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      -1,    -1,    -1,    -1,    80,    81,    82,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    90,    91,    -1,    -1,    94,    -1,
      -1,    97,    98,    99,    -1,    -1,   102,    -1,   104,   105,
      -1,   107,   108,    -1,   110,    -1,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    -1,    -1,
      -1,   127,   128,    -1,   130,    -1,   132,    -1,   134,   135,
      -1,   137,    -1,    -1,    -1,   141,   142,   143,    -1,   145,
     146,    -1,   148,   149,    -1,    -1,    -1,    -1,    -1,   155,
     156,    -1,    -1,    -1,    -1,   161,    -1,    -1,    -1,   165,
      -1,   167,   168,    -1,    -1,   171,   172,    -1,    -1,    -1,
      -1,    -1,   178,   179,    -1,   181,   182,    -1,    -1,   185,
     186,   187,   188,   189,    -1,    -1,   192,   193,   194,    -1,
     196,   197,   198,   199,   200,   201,    -1,    -1,   204,   205,
     206,   207,   208,   209,   210,   211,    -1,   213,   214,    -1,
     216,   217,   218,   219,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   228,   229,    -1,   231,    -1,   233,   234,   235,
     236,    -1,    -1,    -1,    -1,    -1,   242,    -1,    -1,    -1,
      -1,   247,   248,   249,    -1,    -1,    -1,   253,    -1,    -1,
     256,   257,   258,   259,   260,   261,   262,   263,    -1,   265,
     266,    -1,    -1,    -1,   270,   271,   272,   273,    -1,    -1,
     276,    -1,   278,   279,   280,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   291,   292,   293,   294,   295,
      -1,    -1,   298,   299,   300,   301,   302,   303,   304,   305,
     306,    -1,    -1,   309,    -1,   311,   312,    -1,   314,   315,
     316,   317,    -1,    -1,   320,   321,   322,   323,   324,   325,
     326,   327,    -1,    -1,    -1,   331,    -1,    -1,    -1,    -1,
     336,    -1,    -1,   339,   340,   341,   342,   343,    -1,    -1,
     346,   347,    -1,   349,   350,   351,   352,    -1,    -1,    -1,
      -1,   357,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   371,   372,    -1,    -1,    -1,
      -1,   377,    -1,   379,    -1,   381,    -1,    -1,   384,   385,
       4,    -1,     6,     7,     8,    -1,    -1,    -1,    -1,    13,
      -1,    -1,    -1,    17,    18,    19,    -1,    21,    -1,    -1,
      24,    25,    26,    -1,    28,    29,    -1,    31,    -1,    -1,
      -1,    35,    36,    37,    38,    39,    -1,    41,    42,    -1,
      44,    45,    -1,    47,    48,    -1,    50,    51,    52,    53,
      54,    55,    -1,    57,    58,    -1,    60,    61,    62,    -1,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    -1,    -1,    -1,    -1,    80,    81,    82,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    90,    91,    -1,    -1,
      94,    -1,    -1,    97,    98,    99,    -1,    -1,   102,    -1,
     104,   105,    -1,   107,   108,    -1,   110,    -1,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
      -1,    -1,    -1,   127,   128,    -1,   130,    -1,   132,    -1,
     134,   135,    -1,   137,    -1,    -1,    -1,   141,   142,   143,
      -1,   145,   146,    -1,   148,   149,    -1,    -1,    -1,    -1,
      -1,   155,   156,    -1,    -1,    -1,    -1,   161,    -1,    -1,
      -1,   165,    -1,   167,   168,    -1,    -1,   171,   172,    -1,
      -1,    -1,    -1,    -1,   178,   179,    -1,   181,   182,    -1,
      -1,   185,   186,   187,   188,   189,    -1,    -1,   192,   193,
     194,    -1,   196,   197,   198,   199,   200,   201,    -1,    -1,
     204,   205,   206,   207,   208,   209,   210,   211,    -1,   213,
     214,    -1,   216,   217,   218,   219,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   228,   229,    -1,   231,    -1,   233,
     234,   235,   236,    -1,    -1,    -1,    -1,    -1,   242,    -1,
      -1,    -1,    -1,   247,   248,   249,    -1,    -1,    -1,   253,
      -1,    -1,   256,   257,   258,   259,   260,   261,   262,   263,
      -1,   265,   266,    -1,    -1,    -1,   270,   271,   272,   273,
      -1,    -1,   276,    -1,   278,   279,   280,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   291,   292,   293,
     294,   295,    -1,    -1,   298,   299,   300,   301,   302,   303,
     304,   305,   306,    -1,    -1,   309,    -1,   311,   312,    -1,
     314,   315,   316,   317,    -1,    -1,   320,   321,   322,   323,
     324,   325,   326,   327,    -1,    -1,    -1,   331,    -1,    -1,
      -1,    -1,   336,    -1,    -1,   339,   340,   341,   342,   343,
      -1,    -1,   346,   347,    -1,   349,   350,   351,   352,    -1,
      -1,    -1,    -1,   357,    -1,    -1,    -1,   361,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   371,   372,    -1,
      -1,    -1,    -1,   377,    -1,   379,    -1,   381,    -1,    -1,
     384,   385,     4,    -1,     6,     7,     8,     9,    -1,    -1,
      -1,    13,    -1,    -1,    -1,    17,    18,    19,    -1,    21,
      -1,    -1,    24,    25,    26,    -1,    28,    29,    -1,    31,
      -1,    -1,    -1,    35,    36,    37,    38,    39,    -1,    41,
      42,    -1,    44,    45,    -1,    47,    48,    -1,    50,    51,
      52,    53,    54,    55,    -1,    57,    58,    -1,    60,    61,
      62,    -1,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    -1,    -1,    -1,    -1,    80,    81,
      82,    -1,    -1,    85,    -1,    -1,    -1,    -1,    90,    91,
      -1,    -1,    94,    -1,    -1,    97,    98,    99,    -1,    -1,
     102,    -1,   104,   105,    -1,   107,   108,    -1,   110,    -1,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,    -1,    -1,    -1,   127,   128,    -1,   130,    -1,
     132,    -1,   134,   135,    -1,   137,    -1,    -1,    -1,   141,
     142,   143,    -1,   145,   146,    -1,   148,   149,    -1,    -1,
      -1,    -1,    -1,   155,   156,    -1,    -1,    -1,    -1,   161,
      -1,    -1,    -1,   165,    -1,   167,   168,    -1,    -1,   171,
     172,    -1,    -1,    -1,    -1,    -1,   178,   179,    -1,   181,
     182,    -1,    -1,   185,   186,   187,   188,   189,    -1,    -1,
     192,   193,   194,    -1,   196,   197,   198,   199,   200,   201,
      -1,    -1,   204,   205,   206,    -1,   208,   209,   210,   211,
      -1,   213,   214,    -1,   216,   217,   218,   219,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   228,   229,    -1,   231,
      -1,   233,   234,   235,   236,    -1,    -1,    -1,    -1,    -1,
     242,    -1,    -1,    -1,    -1,   247,   248,   249,    -1,    -1,
      -1,   253,    -1,    -1,   256,   257,   258,   259,   260,   261,
     262,   263,    -1,   265,   266,    -1,    -1,    -1,   270,   271,
     272,   273,    -1,    -1,   276,    -1,   278,   279,   280,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   291,
     292,   293,   294,   295,    -1,    -1,   298,   299,   300,   301,
     302,   303,   304,   305,   306,    -1,    -1,   309,    -1,   311,
     312,    -1,   314,   315,   316,   317,    -1,    -1,   320,   321,
     322,   323,   324,   325,   326,   327,    -1,    -1,    -1,   331,
      -1,    -1,    -1,    -1,   336,    -1,    -1,   339,   340,   341,
     342,   343,    -1,    -1,   346,   347,    -1,   349,   350,   351,
     352,    -1,    -1,    -1,    -1,   357,    -1,    -1,    -1,   361,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   371,
     372,    -1,    -1,    -1,    -1,   377,    -1,   379,    -1,   381,
      -1,    -1,   384,   385,     4,    -1,     6,     7,     8,    -1,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    17,    18,    19,
      -1,    21,    -1,    -1,    24,    25,    26,    -1,    28,    29,
      -1,    31,    -1,    -1,    -1,    35,    36,    37,    38,    39,
      -1,    41,    42,    -1,    44,    45,    -1,    47,    48,    -1,
      50,    51,    52,    53,    54,    55,    -1,    57,    58,    -1,
      60,    61,    62,    -1,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    -1,    -1,    -1,    -1,
      80,    81,    82,    -1,    -1,    85,    -1,    -1,    -1,    -1,
      90,    91,    -1,    -1,    94,    -1,    -1,    97,    98,    99,
      -1,    -1,   102,    -1,   104,   105,    -1,   107,   108,    -1,
     110,    -1,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,    -1,   127,   128,    -1,
     130,    -1,   132,    -1,   134,   135,    -1,   137,    -1,    -1,
      -1,   141,   142,   143,    -1,   145,   146,    -1,   148,   149,
      -1,    -1,    -1,    -1,    -1,   155,   156,    -1,    -1,    -1,
      -1,   161,    -1,    -1,    -1,   165,    -1,   167,   168,    -1,
      -1,   171,   172,    -1,    -1,    -1,    -1,    -1,   178,   179,
      -1,   181,   182,    -1,    -1,   185,   186,   187,   188,   189,
      -1,    -1,   192,   193,   194,    -1,   196,   197,   198,   199,
     200,   201,    -1,    -1,   204,   205,   206,   207,   208,   209,
     210,   211,    -1,   213,   214,    -1,   216,   217,   218,   219,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   228,   229,
      -1,   231,    -1,   233,   234,   235,   236,    -1,    -1,    -1,
      -1,    -1,   242,    -1,    -1,    -1,    -1,   247,   248,   249,
      -1,    -1,    -1,   253,    -1,    -1,   256,   257,   258,   259,
     260,   261,   262,   263,    -1,   265,   266,    -1,    -1,    -1,
     270,   271,   272,   273,    -1,    -1,   276,    -1,   278,   279,
     280,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   291,   292,   293,   294,   295,    -1,    -1,   298,   299,
     300,   301,   302,   303,   304,   305,   306,    -1,    -1,   309,
      -1,   311,   312,    -1,   314,   315,   316,   317,    -1,    -1,
     320,   321,   322,   323,   324,   325,   326,   327,    -1,    -1,
      -1,   331,    -1,    -1,    -1,    -1,   336,    -1,    -1,   339,
     340,   341,   342,   343,    -1,    -1,   346,   347,    -1,   349,
     350,   351,   352,    -1,    -1,    -1,    -1,   357,    -1,    -1,
      -1,   361,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   371,   372,    -1,    -1,    -1,    -1,   377,    -1,   379,
      -1,   381,    -1,    -1,   384,   385,     4,    -1,     6,     7,
       8,    -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,    17,
      18,    19,    -1,    21,    -1,    -1,    24,    25,    26,    -1,
      28,    29,    -1,    31,    -1,    -1,    -1,    35,    36,    37,
      38,    39,    -1,    41,    42,    -1,    44,    45,    -1,    47,
      48,    -1,    50,    51,    52,    53,    54,    55,    -1,    57,
      58,    -1,    60,    61,    62,    -1,    -1,    65,    66,    67,
      -1,    69,    70,    71,    72,    73,    74,    75,    -1,    -1,
      -1,    -1,    80,    81,    82,    -1,    -1,    85,    -1,    -1,
      -1,    -1,    90,    91,    -1,    -1,    94,    -1,    -1,    97,
      98,    99,    -1,    -1,   102,    -1,   104,   105,    -1,   107,
     108,    -1,   110,    -1,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,    -1,    -1,    -1,   127,
     128,    -1,   130,    -1,   132,    -1,   134,   135,    -1,   137,
      -1,    -1,    -1,   141,   142,   143,    -1,   145,   146,    -1,
     148,   149,    -1,    -1,    -1,    -1,    -1,   155,   156,    -1,
      -1,    -1,    -1,   161,    -1,    -1,    -1,   165,    -1,   167,
     168,    -1,    -1,   171,   172,    -1,    -1,    -1,    -1,    -1,
     178,   179,    -1,   181,   182,    -1,    -1,   185,   186,   187,
     188,   189,    -1,    -1,   192,   193,   194,    -1,   196,   197,
     198,   199,   200,   201,    -1,    -1,   204,   205,   206,   207,
     208,   209,   210,   211,    -1,   213,   214,    -1,   216,   217,
     218,   219,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     228,   229,    -1,   231,    -1,   233,   234,   235,   236,    -1,
      -1,    -1,    -1,    -1,   242,    -1,    -1,    -1,    -1,   247,
     248,   249,    -1,    -1,    -1,   253,    -1,    -1,   256,   257,
     258,   259,   260,   261,   262,   263,    -1,   265,   266,    -1,
      -1,    -1,   270,   271,   272,   273,    -1,    -1,   276,    -1,
     278,   279,   280,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   291,   292,   293,   294,   295,    -1,    -1,
     298,   299,   300,   301,   302,   303,   304,   305,   306,    -1,
      -1,   309,    -1,   311,   312,    -1,   314,   315,   316,   317,
      -1,    -1,   320,   321,   322,   323,   324,   325,   326,   327,
      -1,    -1,    -1,   331,    -1,    -1,    -1,    -1,   336,    -1,
      -1,   339,   340,   341,   342,   343,    -1,    -1,   346,   347,
      -1,   349,   350,   351,   352,    -1,    -1,    -1,    -1,   357,
      -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   371,   372,    -1,    -1,    -1,    -1,   377,
      -1,   379,    -1,   381,    -1,    -1,   384,   385,     4,    -1,
       6,     7,     8,    -1,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    17,    18,    19,    -1,    21,    -1,    -1,    24,    25,
      26,    -1,    28,    29,    -1,    31,    -1,    -1,    -1,    35,
      36,    37,    38,    39,    -1,    41,    42,    -1,    44,    45,
      -1,    47,    48,    -1,    50,    51,    52,    53,    54,    55,
      -1,    57,    58,    -1,    60,    61,    62,    -1,    -1,    65,
      66,    67,    -1,    69,    70,    71,    72,    73,    74,    75,
      -1,    -1,    -1,    -1,    80,    81,    82,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    90,    91,    -1,    -1,    94,    -1,
      -1,    97,    98,    99,    -1,    -1,   102,    -1,   104,   105,
      -1,   107,   108,    -1,   110,    -1,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    -1,    -1,
      -1,   127,   128,    -1,   130,    -1,   132,    -1,   134,   135,
      -1,   137,    -1,    -1,    -1,   141,   142,   143,    -1,   145,
     146,    -1,   148,   149,    -1,    -1,    -1,    -1,    -1,   155,
     156,    -1,    -1,    -1,    -1,   161,    -1,    -1,    -1,   165,
      -1,   167,   168,    -1,    -1,   171,   172,    -1,    -1,    -1,
      -1,    -1,   178,   179,    -1,   181,   182,    -1,    -1,   185,
     186,   187,   188,   189,    -1,    -1,   192,   193,   194,    -1,
     196,   197,   198,   199,   200,   201,    -1,    -1,   204,   205,
     206,   207,   208,   209,   210,   211,    -1,   213,   214,    -1,
     216,   217,   218,   219,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   228,   229,    -1,   231,    -1,   233,   234,   235,
     236,    -1,    -1,    -1,    -1,    -1,   242,    -1,    -1,    -1,
      -1,   247,   248,   249,    -1,    -1,    -1,   253,    -1,    -1,
     256,   257,   258,   259,   260,   261,   262,   263,    -1,   265,
     266,    -1,    -1,    -1,   270,   271,   272,   273,    -1,    -1,
     276,    -1,   278,   279,   280,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   291,   292,   293,   294,   295,
      -1,    -1,   298,   299,   300,   301,   302,   303,   304,   305,
     306,    -1,    -1,   309,    -1,   311,   312,    -1,   314,   315,
     316,   317,    -1,    -1,   320,   321,   322,   323,   324,   325,
     326,   327,    -1,    -1,    -1,   331,    -1,    -1,    -1,    -1,
     336,    -1,    -1,   339,   340,   341,   342,   343,    -1,    -1,
     346,   347,    -1,   349,   350,   351,   352,    -1,    -1,    -1,
      -1,   357,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   371,   372,    -1,    -1,    -1,
      -1,   377,    -1,   379,    -1,   381,    -1,    -1,   384,   385,
       4,    -1,     6,     7,     8,    -1,    -1,    -1,    -1,    13,
      -1,    -1,    -1,    17,    18,    19,    -1,    21,    -1,    -1,
      24,    25,    26,    -1,    28,    29,    -1,    31,    -1,    -1,
      -1,    35,    36,    37,    38,    39,    -1,    41,    42,    -1,
      44,    45,    -1,    47,    48,    -1,    50,    51,    52,    53,
      54,    55,    -1,    57,    58,    -1,    60,    61,    62,    -1,
      -1,    65,    66,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    -1,    -1,    -1,    -1,    80,    81,    82,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    90,    91,    -1,    -1,
      94,    -1,    -1,    97,    98,    99,    -1,    -1,   102,    -1,
     104,   105,    -1,   107,   108,    -1,   110,    -1,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
      -1,    -1,    -1,   127,   128,    -1,   130,    -1,   132,    -1,
     134,   135,    -1,   137,    -1,    -1,    -1,   141,   142,   143,
      -1,   145,   146,    -1,   148,   149,    -1,    -1,    -1,    -1,
      -1,   155,   156,    -1,    -1,    -1,    -1,   161,    -1,    -1,
      -1,   165,    -1,   167,   168,    -1,    -1,   171,   172,    -1,
      -1,    -1,    -1,    -1,   178,   179,    -1,   181,   182,    -1,
      -1,   185,   186,   187,   188,   189,    -1,    -1,   192,   193,
     194,    -1,   196,   197,   198,   199,   200,   201,    -1,    -1,
     204,   205,   206,    -1,   208,   209,   210,   211,    -1,   213,
     214,    -1,   216,   217,   218,   219,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   228,   229,    -1,   231,    -1,   233,
     234,   235,   236,    -1,    -1,    -1,    -1,    -1,   242,    -1,
      -1,    -1,    -1,   247,   248,   249,    -1,    -1,    -1,   253,
      -1,    -1,   256,   257,   258,   259,   260,   261,   262,   263,
      -1,   265,   266,    -1,    -1,    -1,   270,   271,   272,   273,
      -1,    -1,   276,    -1,   278,   279,   280,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   291,   292,   293,
     294,   295,    -1,    -1,   298,   299,   300,   301,   302,   303,
     304,   305,   306,    -1,    -1,   309,    -1,   311,   312,    -1,
     314,   315,   316,   317,    -1,    -1,   320,   321,   322,   323,
     324,   325,   326,   327,    -1,    -1,    -1,   331,    -1,    -1,
      -1,    -1,   336,    -1,    -1,   339,   340,   341,   342,   343,
      -1,    -1,   346,   347,    -1,   349,   350,   351,   352,    -1,
      -1,    -1,    -1,   357,    -1,    -1,    -1,   361,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   371,   372,    -1,
      -1,    -1,    -1,   377,    -1,   379,    -1,   381,    -1,    -1,
     384,   385,     4,    -1,     6,     7,     8,    -1,    -1,    -1,
      -1,    13,    -1,    -1,    -1,    17,    18,    19,    -1,    21,
      -1,    -1,    24,    25,    26,    -1,    28,    29,    -1,    31,
      -1,    -1,    -1,    35,    36,    37,    38,    39,    -1,    41,
      42,    -1,    44,    45,    -1,    47,    48,    -1,    50,    51,
      52,    53,    54,    55,    -1,    57,    58,    -1,    60,    61,
      62,    -1,    -1,    65,    66,    67,    -1,    69,    70,    71,
      72,    73,    74,    75,    -1,    -1,    -1,    -1,    80,    81,
      82,    -1,    -1,    85,    -1,    -1,    -1,    -1,    90,    91,
      -1,    -1,    94,    -1,    -1,    97,    98,    99,    -1,    -1,
     102,    -1,   104,   105,    -1,   107,   108,    -1,   110,    -1,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,    -1,    -1,    -1,   127,   128,    -1,   130,    -1,
     132,    -1,   134,   135,    -1,   137,    -1,    -1,    -1,   141,
     142,   143,    -1,   145,   146,    -1,   148,   149,    -1,    -1,
      -1,    -1,    -1,   155,   156,    -1,    -1,    -1,    -1,   161,
      -1,    -1,    -1,   165,    -1,   167,   168,    -1,    -1,   171,
     172,    -1,    -1,    -1,    -1,    -1,   178,   179,    -1,   181,
     182,    -1,    -1,   185,   186,   187,   188,   189,    -1,    -1,
     192,   193,   194,    -1,   196,   197,   198,   199,   200,   201,
      -1,    -1,   204,   205,   206,    -1,   208,   209,   210,   211,
      -1,   213,   214,    -1,   216,   217,   218,   219,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   228,   229,    -1,   231,
      -1,   233,   234,   235,   236,    -1,    -1,    -1,    -1,    -1,
     242,    -1,    -1,    -1,    -1,   247,   248,   249,    -1,    -1,
      -1,   253,    -1,    -1,   256,   257,   258,   259,   260,   261,
     262,   263,    -1,   265,   266,    -1,    -1,    -1,   270,   271,
     272,   273,    -1,    -1,   276,    -1,   278,   279,   280,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   291,
     292,   293,   294,   295,    -1,    -1,   298,   299,   300,   301,
     302,   303,   304,   305,   306,    -1,    -1,   309,    -1,   311,
     312,    -1,   314,   315,   316,   317,    -1,    -1,   320,   321,
     322,   323,   324,   325,   326,   327,    -1,    -1,    -1,   331,
      -1,    -1,    -1,    -1,   336,    -1,    -1,   339,   340,   341,
     342,   343,    -1,    -1,   346,   347,    -1,   349,   350,   351,
     352,    -1,    -1,    -1,    -1,   357,    -1,    -1,    -1,   361,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   371,
     372,    -1,    -1,    -1,    -1,   377,    -1,   379,    -1,   381,
      -1,    -1,   384,   385,     4,    -1,     6,     7,     8,    -1,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    17,    18,    19,
      -1,    21,    -1,    -1,    24,    25,    26,    -1,    28,    29,
      -1,    31,    -1,    -1,    -1,    35,    36,    37,    38,    39,
      -1,    41,    42,    -1,    44,    45,    -1,    47,    48,    -1,
      50,    51,    52,    53,    54,    55,    -1,    57,    58,    -1,
      60,    61,    62,    -1,    -1,    65,    66,    67,    -1,    69,
      70,    71,    72,    73,    74,    75,    -1,    -1,    -1,    -1,
      80,    81,    82,    -1,    -1,    85,    -1,    -1,    -1,    -1,
      90,    91,    -1,    -1,    94,    -1,    -1,    97,    98,    99,
      -1,    -1,   102,    -1,   104,   105,    -1,   107,   108,    -1,
     110,    -1,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    -1,    -1,    -1,   127,   128,    -1,
     130,    -1,   132,    -1,   134,   135,    -1,   137,    -1,    -1,
      -1,   141,   142,   143,    -1,   145,   146,    -1,   148,   149,
      -1,    -1,    -1,    -1,    -1,   155,   156,    -1,    -1,    -1,
      -1,   161,    -1,    -1,    -1,   165,    -1,   167,   168,    -1,
      -1,   171,   172,    -1,    -1,    -1,    -1,    -1,   178,   179,
      -1,   181,   182,    -1,    -1,   185,   186,   187,   188,   189,
      -1,    -1,   192,   193,   194,    -1,   196,   197,   198,   199,
     200,   201,    -1,    -1,   204,   205,   206,    -1,   208,   209,
     210,   211,    -1,   213,   214,    -1,   216,   217,   218,   219,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   228,   229,
      -1,   231,    -1,   233,   234,   235,   236,    -1,    -1,    -1,
      -1,    -1,   242,    -1,    -1,    -1,    -1,   247,   248,   249,
      -1,    -1,    -1,   253,    -1,    -1,   256,   257,   258,   259,
     260,   261,   262,   263,    -1,   265,   266,    -1,    -1,    -1,
     270,   271,   272,   273,    -1,    -1,   276,    -1,   278,   279,
     280,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   291,   292,   293,   294,   295,    -1,    -1,   298,   299,
     300,   301,   302,   303,   304,   305,   306,    -1,    -1,   309,
      -1,   311,   312,    -1,   314,   315,   316,   317,    -1,    -1,
     320,   321,   322,   323,   324,   325,   326,   327,    -1,    -1,
      -1,   331,    -1,    -1,    -1,    -1,   336,    -1,    -1,   339,
     340,   341,   342,   343,    -1,    -1,   346,   347,    -1,   349,
     350,   351,   352,    -1,    -1,    -1,    -1,   357,    -1,    -1,
      -1,   361,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   371,   372,    -1,    -1,    -1,    -1,   377,    -1,   379,
      -1,   381,    -1,    -1,   384,   385,     4,    -1,     6,     7,
       8,    -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,    17,
      18,    19,    -1,    21,    -1,    -1,    -1,    -1,    26,    -1,
      28,    29,    -1,    31,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    42,    -1,    44,    45,    -1,    47,
      48,    -1,    50,    51,    52,    53,    54,    55,    -1,    57,
      58,    -1,    60,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      -1,    -1,    70,    71,    72,    -1,    -1,    75,    -1,    -1,
      -1,    -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    91,    -1,    -1,    94,    -1,    -1,    97,
      98,    99,    -1,    -1,   102,    -1,   104,   105,    -1,   107,
     108,    -1,   110,    -1,   112,   113,    -1,    -1,   116,    -1,
      -1,   119,   120,   121,    -1,   123,    -1,    -1,    -1,   127,
     128,    -1,   130,    -1,   132,    -1,    -1,   135,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,    -1,   145,    -1,    -1,
     148,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   161,    -1,    -1,    -1,   165,    -1,    -1,
     168,    -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,
     178,   179,    -1,   181,    -1,    -1,    -1,    -1,   186,   187,
     188,   189,    -1,    -1,   192,    -1,   194,    -1,   196,    -1,
     198,   199,   200,   201,    -1,    -1,   204,   205,   206,    -1,
      -1,   209,    -1,    -1,    -1,   213,   214,    -1,   216,   217,
     218,   219,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     228,    -1,    -1,   231,    -1,   233,   234,   235,   236,    -1,
      -1,    -1,    -1,    -1,   242,    -1,    -1,    -1,    -1,   247,
      -1,    -1,    -1,    -1,    -1,   253,    -1,    -1,    -1,   257,
     258,   259,   260,   261,   262,   263,    -1,   265,   266,    -1,
      -1,    -1,   270,   271,   272,   273,    -1,    -1,   276,    -1,
     278,   279,   280,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   291,   292,    -1,    -1,   295,    -1,    -1,
     298,   299,   300,    -1,    -1,   303,    -1,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,   316,   317,
      -1,    -1,   320,    -1,    -1,   323,   324,    -1,   326,   327,
      -1,    -1,    -1,   331,    -1,    -1,    -1,    -1,   336,    -1,
      -1,    -1,    -1,   341,    -1,   343,    -1,    -1,   346,    -1,
      -1,    -1,    -1,   351,   352,    -1,    -1,    -1,    -1,   357,
      -1,    -1,    -1,   361,    -1,    -1,     4,    -1,     6,     7,
       8,    -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,    17,
      18,    19,   380,    21,    -1,    -1,   384,   385,    26,    -1,
      28,    29,    -1,    31,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    42,    -1,    44,    45,    -1,    47,
      48,    -1,    50,    51,    52,    53,    54,    55,    -1,    57,
      58,    -1,    60,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      -1,    -1,    70,    71,    72,    -1,    -1,    75,    -1,    -1,
      -1,    -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    91,    -1,    -1,    94,    -1,    -1,    97,
      98,    99,    -1,    -1,   102,    -1,   104,   105,    -1,   107,
     108,    -1,   110,    -1,   112,   113,    -1,    -1,   116,    -1,
      -1,   119,   120,   121,    -1,   123,    -1,    -1,    -1,   127,
     128,    -1,   130,    -1,   132,    -1,    -1,   135,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,    -1,   145,    -1,    -1,
     148,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   161,    -1,    -1,    -1,   165,    -1,    -1,
     168,    -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,
     178,   179,    -1,   181,    -1,    -1,    -1,    -1,   186,   187,
     188,   189,    -1,    -1,   192,    -1,   194,    -1,   196,    -1,
     198,   199,   200,   201,    -1,    -1,   204,   205,   206,    -1,
      -1,   209,    -1,    -1,    -1,   213,   214,    -1,   216,   217,
     218,   219,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     228,    -1,    -1,   231,    -1,   233,   234,   235,   236,    -1,
      -1,    -1,    -1,    -1,   242,    -1,    -1,    -1,    -1,   247,
      -1,    -1,    -1,    -1,    -1,   253,    -1,    -1,    -1,   257,
     258,   259,   260,   261,   262,   263,    -1,   265,   266,    -1,
      -1,    -1,   270,   271,   272,   273,    -1,    -1,   276,    -1,
     278,   279,   280,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   291,   292,    -1,    -1,   295,    -1,    -1,
     298,   299,   300,    -1,    -1,   303,    -1,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,   316,   317,
      -1,    -1,   320,    -1,    -1,   323,   324,    -1,   326,   327,
      -1,    -1,    -1,   331,    -1,    -1,    -1,    -1,   336,    -1,
      -1,    -1,    -1,   341,    -1,   343,    -1,    -1,   346,    -1,
      -1,    -1,    -1,   351,   352,    -1,    -1,    -1,     4,   357,
       6,     7,     8,   361,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    17,    18,    19,    -1,    21,    -1,    -1,    -1,    -1,
      26,    -1,    28,    29,    -1,    31,   384,   385,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    42,    -1,    44,    45,
      -1,    47,    48,    -1,    50,    51,    52,    53,    54,    55,
      -1,    57,    58,    59,    60,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    -1,    -1,    70,    71,    72,    -1,    -1,    75,
      -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    91,    -1,    -1,    94,    -1,
      -1,    97,    98,    99,    -1,    -1,   102,    -1,   104,   105,
      -1,   107,   108,    -1,   110,    -1,   112,   113,    -1,    -1,
     116,    -1,    -1,   119,   120,   121,    -1,   123,    -1,    -1,
      -1,   127,   128,    -1,   130,    -1,   132,    -1,    -1,   135,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,   145,
      -1,    -1,   148,   149,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,    -1,   165,
     166,    -1,   168,    -1,    -1,    -1,   172,    -1,   174,    -1,
      -1,    -1,   178,   179,    -1,   181,    -1,    -1,    -1,    -1,
     186,   187,   188,   189,    -1,    -1,   192,    -1,   194,    -1,
     196,    -1,   198,   199,   200,   201,    -1,    -1,   204,   205,
     206,    -1,    -1,   209,    -1,    -1,    -1,   213,   214,    -1,
     216,   217,   218,   219,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   228,    -1,    -1,   231,    -1,   233,   234,   235,
     236,    -1,    -1,    -1,    -1,    -1,   242,    -1,    -1,    -1,
      -1,   247,    -1,    -1,    -1,    -1,    -1,   253,    -1,    -1,
      -1,   257,   258,   259,   260,   261,   262,   263,    -1,   265,
     266,   267,    -1,    -1,   270,   271,   272,   273,    -1,    -1,
     276,    -1,   278,   279,   280,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   291,   292,    -1,    -1,   295,
      -1,    -1,   298,   299,   300,    -1,    -1,   303,    -1,   305,
     306,    -1,    -1,   309,    -1,    -1,   312,    -1,   314,    -1,
     316,   317,    -1,    -1,   320,    -1,    -1,   323,   324,    -1,
     326,   327,    -1,    -1,    -1,   331,    -1,    -1,    -1,    -1,
     336,    -1,    -1,    -1,    -1,   341,    -1,   343,    -1,     4,
     346,     6,     7,     8,    -1,   351,   352,    -1,    13,    -1,
      -1,   357,    17,    18,    19,   361,    21,    -1,    -1,    -1,
      -1,    26,    -1,    28,    29,    -1,    31,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    42,   384,    44,
      45,    -1,    47,    48,    -1,    50,    51,    52,    53,    54,
      55,    -1,    57,    58,    59,    60,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    -1,    -1,    70,    71,    72,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    -1,    -1,    94,
      -1,    -1,    97,    98,    99,    -1,    -1,   102,    -1,   104,
     105,    -1,   107,   108,    -1,   110,    -1,   112,   113,    -1,
      -1,   116,    -1,    -1,   119,   120,   121,    -1,   123,    -1,
      -1,    -1,   127,   128,    -1,   130,    -1,   132,    -1,    -1,
     135,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,
     145,    -1,    -1,   148,   149,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,    -1,
     165,   166,    -1,   168,    -1,    -1,    -1,   172,    -1,    -1,
      -1,    -1,    -1,   178,   179,    -1,   181,    -1,    -1,    -1,
      -1,   186,   187,   188,   189,    -1,    -1,   192,    -1,   194,
      -1,   196,    -1,   198,   199,   200,   201,    -1,    -1,   204,
     205,   206,    -1,    -1,   209,    -1,    -1,    -1,   213,   214,
      -1,   216,   217,   218,   219,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   228,    -1,    -1,   231,    -1,   233,   234,
     235,   236,    -1,    -1,    -1,    -1,    -1,   242,    -1,    -1,
      -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,   253,    -1,
      -1,    -1,   257,   258,   259,   260,   261,   262,   263,    -1,
     265,   266,    -1,    -1,    -1,   270,   271,   272,   273,    -1,
      -1,   276,    -1,   278,   279,   280,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   291,   292,    -1,    -1,
     295,    -1,    -1,   298,   299,   300,    -1,    -1,   303,    -1,
     305,   306,    -1,    -1,   309,    -1,    -1,   312,    -1,   314,
      -1,   316,   317,    -1,    -1,   320,    -1,    -1,   323,   324,
      -1,   326,   327,    -1,    -1,    -1,   331,    -1,    -1,    -1,
      -1,   336,    -1,    -1,    -1,    -1,   341,    -1,   343,    -1,
       4,   346,     6,     7,     8,    -1,   351,   352,    -1,    13,
      -1,    -1,   357,    17,    18,    19,   361,    21,    -1,    -1,
      -1,    -1,    26,    -1,    28,    29,    -1,    31,    -1,    -1,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    42,   384,
      44,    45,    -1,    47,    48,    -1,    50,    51,    52,    53,
      54,    55,    -1,    57,    58,    -1,    60,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    -1,    -1,    70,    71,    72,    -1,
      -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,    -1,
      94,    -1,    -1,    97,    98,    99,    -1,    -1,   102,    -1,
     104,   105,    -1,   107,   108,    -1,   110,    -1,   112,   113,
      -1,    -1,   116,    -1,    -1,   119,   120,   121,    -1,   123,
      -1,    -1,    -1,   127,   128,    -1,   130,    -1,   132,    -1,
      -1,   135,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
      -1,   145,    -1,    -1,   148,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,
      -1,   165,    -1,    -1,   168,    -1,    -1,    -1,   172,    -1,
      -1,    -1,    -1,    -1,   178,   179,    -1,   181,    -1,    -1,
      -1,    -1,   186,   187,   188,   189,    -1,    -1,   192,    -1,
     194,    -1,   196,    -1,   198,   199,   200,   201,    -1,    -1,
     204,   205,   206,    -1,    -1,   209,    -1,    -1,    -1,   213,
     214,    -1,   216,   217,   218,   219,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   228,    -1,    -1,   231,    -1,   233,
     234,   235,   236,    -1,    -1,    -1,    -1,    -1,   242,    -1,
      -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,   253,
      -1,    -1,    -1,   257,   258,   259,   260,   261,   262,   263,
      -1,   265,   266,   267,    -1,    -1,   270,   271,   272,   273,
      -1,    -1,   276,    -1,   278,   279,   280,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   291,   292,    -1,
      -1,   295,    -1,    -1,   298,   299,   300,    -1,    -1,   303,
      -1,   305,   306,    -1,    -1,   309,    -1,    -1,   312,    -1,
     314,    -1,   316,   317,    -1,    -1,   320,    -1,    -1,   323,
     324,    -1,   326,   327,    -1,    -1,    -1,   331,    -1,    -1,
      -1,    -1,   336,    -1,    -1,    -1,    -1,   341,    -1,   343,
      -1,    -1,   346,    -1,    -1,    -1,    -1,   351,   352,    -1,
      -1,    -1,    -1,   357,    -1,    -1,    -1,   361,    -1,    -1,
       4,    -1,     6,     7,     8,    -1,    -1,    -1,    -1,    13,
      -1,    -1,    -1,    17,    18,    19,   380,    21,    -1,    -1,
     384,    -1,    26,    -1,    28,    29,    -1,    31,    -1,    -1,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    42,    -1,
      44,    45,    -1,    47,    48,    -1,    50,    51,    52,    53,
      54,    55,    -1,    57,    58,    -1,    60,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    -1,    -1,    70,    71,    72,    -1,
      -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,    -1,
      94,    -1,    -1,    97,    98,    99,    -1,    -1,   102,    -1,
     104,   105,    -1,   107,   108,    -1,   110,    -1,   112,   113,
      -1,    -1,   116,    -1,    -1,   119,   120,   121,    -1,   123,
      -1,    -1,    -1,   127,   128,    -1,   130,    -1,   132,    -1,
      -1,   135,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
      -1,   145,    -1,    -1,   148,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   157,    -1,    -1,    -1,   161,    -1,    -1,
      -1,   165,    -1,    -1,   168,    -1,    -1,    -1,   172,    -1,
      -1,    -1,    -1,    -1,   178,   179,    -1,   181,    -1,    -1,
      -1,    -1,   186,   187,   188,   189,    -1,    -1,   192,    -1,
     194,    -1,   196,    -1,   198,   199,   200,   201,    -1,    -1,
     204,   205,   206,    -1,    -1,   209,    -1,    -1,    -1,   213,
     214,    -1,   216,   217,   218,   219,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   228,    -1,    -1,   231,    -1,   233,
     234,   235,   236,    -1,    -1,    -1,    -1,    -1,   242,    -1,
      -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,   253,
      -1,    -1,    -1,   257,   258,   259,   260,   261,   262,   263,
      -1,   265,   266,    -1,    -1,    -1,   270,   271,   272,   273,
      -1,    -1,   276,    -1,   278,   279,   280,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   291,   292,    -1,
      -1,   295,    -1,    -1,   298,   299,   300,    -1,    -1,   303,
      -1,   305,   306,    -1,    -1,   309,    -1,    -1,   312,    -1,
     314,    -1,   316,   317,    -1,    -1,   320,    -1,    -1,   323,
     324,    -1,   326,   327,    -1,    -1,    -1,   331,    -1,    -1,
      -1,    -1,   336,    -1,    -1,    -1,    -1,   341,    -1,   343,
      -1,     4,   346,     6,     7,     8,    -1,   351,   352,    -1,
      13,    -1,    -1,   357,    17,    18,    19,   361,    21,    -1,
      -1,    -1,    -1,    26,    -1,    28,    29,    -1,    31,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    42,
     384,    44,    45,    -1,    47,    48,    -1,    50,    51,    52,
      53,    54,    55,    -1,    57,    58,    -1,    60,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    -1,    -1,    70,    71,    72,
      -1,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,
      -1,    94,    -1,    -1,    97,    98,    99,    -1,    -1,   102,
      -1,   104,   105,    -1,   107,   108,    -1,   110,    -1,   112,
     113,    -1,    -1,   116,    -1,    -1,   119,   120,   121,    -1,
     123,    -1,    -1,    -1,   127,   128,    -1,   130,    -1,   132,
      -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,    -1,   145,    -1,    -1,   148,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,
      -1,    -1,   165,    -1,    -1,   168,    -1,    -1,    -1,   172,
      -1,    -1,    -1,    -1,    -1,   178,   179,    -1,   181,    -1,
      -1,    -1,    -1,   186,   187,   188,   189,    -1,    -1,   192,
      -1,   194,    -1,   196,    -1,   198,   199,   200,   201,    -1,
      -1,   204,   205,   206,    -1,    -1,   209,    -1,    -1,    -1,
     213,   214,    -1,   216,   217,   218,   219,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   228,    -1,    -1,   231,    -1,
     233,   234,   235,   236,    -1,    -1,    -1,    -1,    -1,   242,
      -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,
     253,    -1,    -1,    -1,   257,   258,   259,   260,   261,   262,
     263,    -1,   265,   266,    -1,    -1,    -1,   270,   271,   272,
     273,    -1,    -1,   276,    -1,   278,   279,   280,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   291,   292,
      -1,    -1,   295,    -1,    -1,   298,   299,   300,    -1,    -1,
     303,    -1,   305,   306,    -1,    -1,   309,    -1,    -1,   312,
      -1,   314,    -1,   316,   317,    -1,    -1,   320,    -1,    -1,
     323,   324,    -1,   326,   327,    -1,    -1,    -1,   331,    -1,
      -1,    -1,    -1,   336,    -1,    -1,    -1,    -1,   341,    -1,
     343,    -1,     4,   346,     6,     7,     8,    -1,   351,   352,
      -1,    13,    -1,    -1,   357,    17,    18,    19,   361,    21,
      -1,    -1,    -1,    -1,    26,    -1,    28,    29,    -1,    31,
      -1,    -1,    -1,    35,    -1,    -1,   379,    39,    -1,    -1,
      42,   384,    44,    45,    -1,    47,    48,    -1,    50,    51,
      52,    53,    54,    55,    -1,    57,    58,    -1,    60,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    -1,    -1,    70,    71,
      72,    -1,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      -1,    -1,    94,    -1,    -1,    97,    98,    99,    -1,    -1,
     102,    -1,   104,   105,    -1,   107,   108,    -1,   110,    -1,
     112,   113,    -1,    -1,   116,    -1,    -1,   119,   120,   121,
      -1,   123,    -1,    -1,    -1,   127,   128,    -1,   130,    -1,
     132,    -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,    -1,   145,    -1,    -1,   148,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,
      -1,    -1,    -1,   165,    -1,    -1,   168,    -1,    -1,    -1,
     172,    -1,    -1,    -1,    -1,    -1,   178,   179,    -1,   181,
      -1,    -1,    -1,    -1,   186,   187,   188,   189,    -1,    -1,
     192,    -1,   194,    -1,   196,    -1,   198,   199,   200,   201,
      -1,    -1,   204,   205,   206,    -1,    -1,   209,    -1,    -1,
      -1,   213,   214,    -1,   216,   217,   218,   219,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   228,    -1,    -1,   231,
      -1,   233,   234,   235,   236,    -1,    -1,    -1,    -1,    -1,
     242,    -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,
      -1,   253,    -1,    -1,    -1,   257,   258,   259,   260,   261,
     262,   263,    -1,   265,   266,    -1,    -1,    -1,   270,   271,
     272,   273,    -1,    -1,   276,    -1,   278,   279,   280,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   291,
     292,    -1,    -1,   295,    -1,    -1,   298,   299,   300,    -1,
      -1,   303,    -1,   305,   306,    -1,    -1,   309,    -1,    -1,
     312,    -1,   314,    -1,   316,   317,    -1,    -1,   320,    -1,
      -1,   323,   324,    -1,   326,   327,    -1,    -1,    -1,   331,
      -1,    -1,    -1,    -1,   336,    -1,    -1,    -1,    -1,   341,
      -1,   343,    -1,     4,   346,     6,     7,     8,    -1,   351,
     352,    -1,    13,    -1,    -1,   357,    17,    18,    19,   361,
      21,    -1,    -1,    -1,    -1,    26,    -1,    28,    29,    -1,
      31,    -1,    -1,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    42,   384,    44,    45,    -1,    47,    48,    -1,    50,
      51,    52,    53,    54,    55,    -1,    57,    58,    -1,    60,
      -1,    -1,    -1,    -1,    65,    -1,    -1,    -1,    -1,    70,
      71,    72,    -1,    -1,    75,    -1,    -1,    -1,    -1,    80,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    -1,    -1,    94,    -1,    -1,    97,    98,    99,    -1,
      -1,   102,    -1,   104,   105,    -1,   107,   108,    -1,   110,
      -1,   112,   113,    -1,    -1,   116,    -1,    -1,   119,   120,
     121,    -1,   123,    -1,    -1,    -1,   127,   128,    -1,   130,
      -1,   132,    -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,    -1,   145,    -1,    -1,   148,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     161,    -1,    -1,    -1,   165,    -1,    -1,   168,    -1,    -1,
      -1,   172,    -1,    -1,    -1,    -1,    -1,   178,   179,    -1,
     181,    -1,    -1,    -1,    -1,   186,   187,   188,   189,    -1,
      -1,   192,    -1,   194,    -1,   196,    -1,   198,   199,   200,
     201,    -1,    -1,   204,   205,   206,    -1,    -1,   209,    -1,
      -1,    -1,   213,   214,    -1,   216,   217,   218,   219,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   228,    -1,    -1,
     231,    -1,   233,   234,   235,   236,    -1,    -1,    -1,    -1,
      -1,   242,    -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,
      -1,    -1,   253,    -1,    -1,    -1,   257,   258,   259,   260,
     261,   262,   263,    -1,   265,   266,    -1,    -1,    -1,   270,
     271,   272,   273,    -1,    -1,   276,    -1,   278,   279,   280,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     291,   292,    -1,    -1,   295,    -1,    -1,   298,   299,   300,
      -1,    -1,   303,    -1,   305,   306,    -1,    -1,   309,    -1,
      -1,   312,    -1,   314,    -1,   316,   317,    -1,    -1,   320,
      -1,    -1,   323,   324,    -1,   326,   327,    -1,    -1,    -1,
     331,    -1,    -1,    -1,    -1,   336,    -1,    -1,    -1,    -1,
     341,    -1,   343,    -1,     4,   346,     6,     7,     8,    -1,
     351,   352,    -1,    13,    -1,    -1,   357,    17,    18,    19,
     361,    21,    -1,    -1,    -1,    -1,    26,    -1,    28,    29,
      -1,    31,    -1,    -1,    -1,    35,    -1,    -1,    -1,    39,
      -1,    -1,    42,   384,    44,    45,    -1,    47,    48,    -1,
      50,    51,    52,    53,    54,    55,    -1,    57,    58,    -1,
      60,    -1,    -1,    -1,    -1,    65,    -1,    -1,    -1,    -1,
      70,    71,    72,    -1,    -1,    75,    -1,    -1,    -1,    -1,
      80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    -1,    -1,    94,    -1,    -1,    97,    98,    99,
      -1,    -1,   102,    -1,   104,   105,    -1,   107,   108,    -1,
     110,    -1,   112,   113,    -1,    -1,   116,    -1,    -1,   119,
     120,   121,    -1,   123,    -1,    -1,    -1,   127,   128,    -1,
     130,    -1,   132,    -1,    -1,   135,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,    -1,   145,    -1,    -1,   148,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   161,    -1,    -1,    -1,   165,    -1,    -1,   168,    -1,
      -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,   178,   179,
      -1,   181,    -1,    -1,    -1,    -1,   186,   187,   188,   189,
      -1,    -1,   192,    -1,   194,    -1,   196,    -1,   198,   199,
     200,   201,    -1,    -1,   204,   205,   206,    -1,    -1,   209,
      -1,    -1,    -1,   213,   214,    -1,   216,   217,   218,   219,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   228,    -1,
      -1,   231,    -1,   233,   234,   235,   236,    -1,    -1,    -1,
      -1,    -1,   242,    -1,    -1,    -1,    -1,   247,    -1,    -1,
      -1,    -1,    -1,   253,    -1,    -1,    -1,   257,   258,   259,
     260,   261,   262,   263,    -1,   265,   266,    -1,    -1,    -1,
     270,   271,   272,   273,    -1,    -1,   276,    -1,   278,   279,
     280,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   291,   292,    -1,    -1,   295,    -1,    -1,   298,   299,
     300,    -1,    -1,   303,    -1,   305,   306,    -1,    -1,   309,
      -1,    -1,   312,    -1,   314,    -1,   316,   317,    -1,    -1,
     320,    -1,    -1,   323,   324,    -1,   326,   327,    -1,    -1,
      -1,   331,    -1,    -1,    -1,    -1,   336,    -1,    -1,    -1,
      -1,   341,    -1,   343,    -1,     4,   346,     6,     7,     8,
      -1,   351,   352,    -1,    13,    -1,    -1,   357,    17,    18,
      19,   361,    21,    -1,    -1,    -1,    -1,    26,    -1,    28,
      29,    -1,    31,    -1,    -1,    -1,    35,    -1,    -1,    -1,
      39,    -1,    -1,    42,   384,    44,    45,    -1,    47,    48,
      -1,    50,    51,    52,    53,    54,    55,    -1,    57,    58,
      -1,    60,    -1,    -1,    -1,    -1,    65,    -1,    -1,    -1,
      -1,    70,    71,    72,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    -1,    -1,    94,    -1,    -1,    97,    98,
      99,    -1,    -1,   102,    -1,   104,   105,    -1,   107,   108,
      -1,   110,    -1,   112,   113,    -1,    -1,   116,    -1,    -1,
     119,   120,   121,    -1,   123,    -1,    -1,    -1,   127,   128,
      -1,   130,    -1,   132,    -1,    -1,   135,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,    -1,   145,    -1,    -1,   148,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   161,    -1,    -1,    -1,   165,    -1,    -1,   168,
      -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,   178,
     179,    -1,   181,    -1,    -1,    -1,    -1,   186,   187,   188,
     189,    -1,    -1,   192,    -1,   194,    -1,   196,    -1,   198,
     199,   200,   201,    -1,    -1,   204,   205,   206,    -1,    -1,
     209,    -1,    -1,    -1,   213,   214,    -1,   216,   217,   218,
     219,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   228,
      -1,    -1,   231,    -1,   233,   234,   235,   236,    -1,    -1,
      -1,    -1,    -1,   242,    -1,    -1,    -1,    -1,   247,    -1,
      -1,    -1,    -1,    -1,   253,    -1,    -1,    -1,   257,   258,
     259,   260,   261,   262,   263,    -1,   265,   266,    -1,    -1,
      -1,   270,   271,   272,   273,    -1,    -1,   276,    -1,   278,
     279,   280,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   291,   292,    -1,    -1,   295,    -1,    -1,   298,
     299,   300,    -1,    -1,   303,    -1,   305,   306,    -1,    -1,
     309,    -1,    -1,   312,    -1,   314,    -1,   316,   317,    -1,
      -1,   320,    -1,    -1,   323,   324,    -1,   326,   327,    -1,
      -1,    -1,   331,    -1,    -1,    -1,    -1,   336,    -1,    -1,
      -1,    -1,   341,    -1,   343,    -1,     4,   346,     6,     7,
       8,    -1,   351,   352,    -1,    13,    -1,    -1,   357,    17,
      18,    19,   361,    21,    -1,    -1,    -1,    -1,    26,    -1,
      28,    29,    -1,    31,    -1,    -1,    -1,    35,    -1,    -1,
      -1,    39,    -1,    -1,    42,   384,    44,    45,    -1,    47,
      48,    -1,    50,    51,    52,    53,    54,    55,    -1,    57,
      58,    -1,    60,    -1,    -1,    -1,    -1,    65,    -1,    -1,
      -1,    -1,    70,    71,    72,    -1,    -1,    75,    -1,    -1,
      -1,    -1,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    91,    -1,    -1,    94,    -1,    -1,    97,
      98,    99,    -1,    -1,   102,    -1,   104,   105,    -1,   107,
     108,    -1,   110,    -1,   112,   113,    -1,    -1,   116,    -1,
      -1,   119,   120,   121,    -1,   123,    -1,    -1,    -1,   127,
     128,    -1,   130,    -1,   132,    -1,    -1,   135,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,    -1,   145,    -1,    -1,
     148,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   161,    -1,    -1,    -1,   165,    -1,    -1,
     168,    -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,
     178,   179,    -1,   181,    -1,    -1,    -1,    -1,   186,   187,
     188,   189,    -1,    -1,   192,    -1,   194,    -1,   196,    -1,
     198,   199,   200,   201,    -1,    -1,   204,   205,   206,    -1,
      -1,   209,    -1,    -1,    -1,   213,   214,    -1,   216,   217,
     218,   219,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     228,    -1,    -1,   231,    -1,   233,   234,   235,   236,    -1,
      -1,    -1,    -1,    -1,   242,    -1,    -1,    -1,    -1,   247,
      -1,    -1,    -1,    -1,    -1,   253,    -1,    -1,    -1,   257,
     258,   259,   260,   261,   262,   263,    -1,   265,   266,    -1,
      -1,    -1,   270,   271,   272,   273,    -1,    -1,   276,    -1,
     278,   279,   280,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   291,   292,    -1,    -1,   295,    -1,    -1,
     298,   299,   300,    -1,    -1,   303,    -1,   305,   306,    -1,
      -1,   309,    -1,    -1,   312,    -1,   314,    -1,   316,   317,
      -1,    -1,   320,    -1,    -1,   323,   324,    -1,   326,   327,
      -1,    -1,    -1,   331,    -1,    -1,    -1,    -1,   336,    -1,
      -1,    -1,    -1,   341,    -1,   343,    -1,     4,   346,     6,
       7,     8,    -1,   351,   352,    -1,    13,    -1,    -1,   357,
      17,    18,    19,   361,    21,    -1,    -1,    -1,    -1,    26,
      -1,    28,    29,    -1,    31,   373,    -1,    -1,    35,    -1,
      -1,    -1,    39,    -1,    -1,    42,    -1,    44,    45,    -1,
      47,    48,    -1,    50,    51,    52,    53,    54,    55,    -1,
      57,    58,    -1,    60,    -1,    -1,    -1,    -1,    65,    -1,
      -1,    -1,    -1,    70,    71,    72,    -1,    -1,    75,    -1,
      -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    91,    -1,    -1,    94,    -1,    -1,
      97,    98,    99,    -1,    -1,   102,    -1,   104,   105,    -1,
     107,   108,    -1,   110,    -1,   112,   113,    -1,    -1,   116,
      -1,    -1,   119,   120,   121,    -1,   123,    -1,    -1,    -1,
     127,   128,    -1,   130,    -1,   132,    -1,    -1,   135,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,    -1,   145,    -1,
      -1,   148,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,    -1,    -1,   165,    -1,
      -1,   168,    -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,
      -1,   178,   179,    -1,   181,    -1,    -1,    -1,    -1,   186,
     187,   188,   189,    -1,    -1,   192,    -1,   194,    -1,   196,
      -1,   198,   199,   200,   201,    -1,    -1,   204,   205,   206,
      -1,    -1,   209,    -1,    -1,    -1,   213,   214,    -1,   216,
     217,   218,   219,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   228,    -1,    -1,   231,    -1,   233,   234,   235,   236,
      -1,    -1,    -1,    -1,    -1,   242,    -1,    -1,    -1,    -1,
     247,    -1,    -1,    -1,    -1,    -1,   253,    -1,    -1,    -1,
     257,   258,   259,   260,   261,   262,   263,    -1,   265,   266,
      -1,    -1,    -1,   270,   271,   272,   273,    -1,    -1,   276,
      -1,   278,   279,   280,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   291,   292,    -1,    -1,   295,    -1,
      -1,   298,   299,   300,    -1,    -1,   303,    -1,   305,   306,
      -1,    -1,   309,    -1,    -1,   312,    -1,   314,    -1,   316,
     317,    -1,    -1,   320,    -1,    -1,   323,   324,    -1,   326,
     327,    -1,    -1,    -1,   331,    -1,    -1,    -1,    -1,   336,
      -1,    -1,    -1,    -1,   341,    -1,   343,    -1,     4,   346,
       6,     7,     8,    -1,   351,   352,    -1,    13,    -1,    -1,
     357,    17,    18,    19,   361,    21,    -1,    -1,    -1,    25,
      26,    -1,    28,    29,    -1,    31,   373,    -1,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    42,    -1,    44,    45,
      -1,    47,    48,    -1,    50,    51,    52,    53,    54,    55,
      -1,    57,    58,    -1,    60,    -1,    -1,    -1,    -1,    65,
      -1,    -1,    -1,    -1,    70,    71,    72,    -1,    -1,    75,
      -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    91,    -1,    -1,    94,    -1,
      -1,    97,    98,    99,    -1,    -1,   102,    -1,   104,   105,
      -1,   107,   108,    -1,   110,    -1,   112,   113,    -1,    -1,
     116,    -1,    -1,   119,   120,   121,    -1,   123,    -1,    -1,
      -1,   127,   128,    -1,   130,    -1,   132,    -1,    -1,   135,
      -1,   137,    -1,    -1,    -1,   141,   142,   143,    -1,   145,
      -1,    -1,   148,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,    -1,   165,
      -1,    -1,   168,    -1,    -1,    -1,   172,    -1,    -1,    -1,
      -1,    -1,   178,   179,    -1,   181,    -1,    -1,    -1,    -1,
     186,   187,   188,   189,    -1,    -1,   192,    -1,   194,    -1,
     196,    -1,   198,   199,   200,   201,    -1,    -1,   204,   205,
     206,    -1,    -1,   209,    -1,    -1,    -1,   213,   214,    -1,
     216,   217,   218,   219,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   228,    -1,    -1,   231,    -1,   233,   234,   235,
     236,    -1,    -1,    -1,    -1,    -1,   242,    -1,    -1,    -1,
      -1,   247,    -1,    -1,    -1,    -1,    -1,   253,    -1,    -1,
      -1,   257,   258,   259,   260,   261,   262,   263,    -1,   265,
     266,    -1,    -1,    -1,   270,   271,   272,   273,    -1,    -1,
     276,    -1,   278,   279,   280,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   291,   292,    -1,    -1,   295,
      -1,    -1,   298,   299,   300,    -1,    -1,   303,    -1,   305,
     306,    -1,    -1,   309,    -1,   311,   312,    -1,   314,    -1,
     316,   317,    -1,    -1,   320,    -1,    -1,   323,   324,    -1,
     326,   327,    -1,    -1,    -1,   331,    -1,    -1,    -1,    -1,
     336,    -1,    -1,    -1,    -1,   341,    -1,   343,    -1,     4,
     346,     6,     7,     8,    -1,   351,   352,    -1,    13,    14,
      -1,   357,    17,    18,    19,   361,    21,    -1,    -1,    -1,
      -1,    26,    -1,    28,    29,    -1,    31,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    39,    -1,    -1,    42,    -1,    44,
      45,    -1,    47,    48,    -1,    50,    51,    52,    53,    54,
      55,    -1,    57,    58,    -1,    60,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    -1,    -1,    70,    71,    72,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    -1,    -1,    94,
      -1,    -1,    97,    98,    99,    -1,    -1,   102,    -1,   104,
     105,    -1,   107,   108,    -1,   110,    -1,   112,   113,    -1,
      -1,   116,    -1,    -1,   119,   120,   121,    -1,   123,    -1,
      -1,    -1,   127,   128,    -1,   130,    -1,   132,    -1,    -1,
     135,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,    -1,
     145,    -1,    -1,   148,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,    -1,
     165,    -1,    -1,   168,    -1,    -1,    -1,   172,    -1,    -1,
      -1,    -1,    -1,   178,   179,    -1,   181,    -1,    -1,    -1,
      -1,   186,   187,   188,   189,    -1,    -1,   192,    -1,   194,
      -1,   196,    -1,   198,   199,   200,   201,    -1,    -1,   204,
     205,   206,    -1,    -1,   209,    -1,    -1,    -1,   213,   214,
      -1,   216,   217,   218,   219,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   228,    -1,    -1,   231,    -1,   233,   234,
     235,   236,    -1,    -1,    -1,    -1,    -1,   242,    -1,    -1,
      -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,   253,    -1,
      -1,    -1,   257,   258,   259,   260,   261,   262,   263,    -1,
     265,   266,    -1,    -1,    -1,   270,   271,   272,   273,    -1,
      -1,   276,    -1,   278,   279,   280,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   291,   292,    -1,    -1,
     295,    -1,    -1,   298,   299,   300,    -1,    -1,   303,    -1,
     305,   306,    -1,    -1,   309,    -1,   311,   312,    -1,   314,
      -1,   316,   317,    -1,    -1,   320,    -1,    -1,   323,   324,
      -1,   326,   327,    -1,    -1,    -1,   331,    -1,    -1,    -1,
      -1,   336,    -1,    -1,    -1,    -1,   341,    -1,   343,    -1,
       4,   346,     6,     7,     8,    -1,   351,   352,    -1,    13,
      -1,    -1,   357,    17,    18,    19,   361,    21,    -1,    -1,
      -1,    -1,    26,    -1,    28,    29,    -1,    31,    -1,    -1,
      -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    42,    -1,
      44,    45,    -1,    47,    48,    -1,    50,    51,    52,    53,
      54,    55,    -1,    57,    58,    -1,    60,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    -1,    -1,    70,    71,    72,    -1,
      -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,    -1,
      94,    -1,    -1,    97,    98,    99,    -1,    -1,   102,    -1,
     104,   105,    -1,   107,   108,    -1,   110,    -1,   112,   113,
      -1,    -1,   116,    -1,    -1,   119,   120,   121,    -1,   123,
      -1,    -1,    -1,   127,   128,    -1,   130,    -1,   132,    -1,
      -1,   135,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
      -1,   145,    -1,    -1,   148,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,
      -1,   165,    -1,    -1,   168,    -1,    -1,    -1,   172,    -1,
      -1,    -1,    -1,    -1,   178,   179,    -1,   181,    -1,    -1,
      -1,    -1,   186,   187,   188,   189,    -1,    -1,   192,    -1,
     194,    -1,   196,    -1,   198,   199,   200,   201,    -1,    -1,
     204,   205,   206,    -1,    -1,   209,    -1,    -1,    -1,   213,
     214,    -1,   216,   217,   218,   219,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   228,    -1,    -1,   231,    -1,   233,
     234,   235,   236,    -1,    -1,    -1,    -1,    -1,   242,    -1,
      -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,   253,
      -1,    -1,    -1,   257,   258,   259,   260,   261,   262,   263,
      -1,   265,   266,    -1,    -1,    -1,   270,   271,   272,   273,
      -1,    -1,   276,    -1,   278,   279,   280,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   291,   292,    -1,
      -1,   295,    -1,    -1,   298,   299,   300,    -1,    -1,   303,
      -1,   305,   306,    -1,    -1,   309,    -1,   311,   312,    -1,
     314,    -1,   316,   317,    -1,    -1,   320,    -1,    -1,   323,
     324,    -1,   326,   327,    -1,    -1,    -1,   331,    -1,    -1,
      -1,    -1,   336,    -1,    -1,    -1,    -1,   341,    -1,   343,
      -1,     4,   346,     6,     7,     8,    -1,   351,   352,    -1,
      13,    -1,    -1,   357,    17,    18,    19,   361,    21,    -1,
      -1,    -1,    -1,    26,    -1,    28,    29,    -1,    31,    -1,
      -1,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,    42,
      -1,    44,    45,    -1,    47,    48,    -1,    50,    51,    52,
      53,    54,    55,    -1,    57,    58,    -1,    60,    -1,    -1,
      -1,    -1,    65,    -1,    -1,    -1,    -1,    70,    71,    72,
      -1,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    -1,
      -1,    94,    -1,    -1,    97,    98,    99,    -1,    -1,   102,
      -1,   104,   105,    -1,   107,   108,    -1,   110,    -1,   112,
     113,    -1,    -1,   116,    -1,    -1,   119,   120,   121,    -1,
     123,    -1,    -1,    -1,   127,   128,    -1,   130,    -1,   132,
      -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,    -1,   145,    -1,    -1,   148,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,
      -1,    -1,   165,    -1,    -1,   168,    -1,    -1,    -1,   172,
      -1,    -1,    -1,    -1,    -1,   178,   179,    -1,   181,    -1,
      -1,    -1,    -1,   186,   187,   188,   189,    -1,    -1,   192,
      -1,   194,    -1,   196,    -1,   198,   199,   200,   201,    -1,
      -1,   204,   205,   206,    -1,    -1,   209,    -1,    -1,    -1,
     213,   214,    -1,   216,   217,   218,   219,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   228,    -1,    -1,   231,   232,
     233,   234,   235,   236,    -1,    -1,    -1,    -1,    -1,   242,
      -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,    -1,
     253,    -1,    -1,    -1,   257,   258,   259,   260,   261,   262,
     263,    -1,   265,   266,    -1,    -1,    -1,   270,   271,   272,
     273,    -1,    -1,   276,    -1,   278,   279,   280,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   291,   292,
      -1,    -1,   295,    -1,    -1,   298,   299,   300,    -1,    -1,
     303,    -1,   305,   306,    -1,    -1,   309,    -1,    -1,   312,
      -1,   314,    -1,   316,   317,    -1,    -1,   320,    -1,    -1,
     323,   324,    -1,   326,   327,    -1,    -1,    -1,   331,    -1,
      -1,    -1,    -1,   336,    -1,    -1,    -1,    -1,   341,    -1,
     343,    -1,     4,   346,     6,     7,     8,    -1,   351,   352,
      -1,    13,    -1,    -1,   357,    17,    18,    19,   361,    21,
      -1,    -1,    -1,    -1,    26,    -1,    28,    29,    -1,    31,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    39,    -1,    -1,
      42,    -1,    44,    45,    -1,    47,    48,    -1,    50,    51,
      52,    53,    54,    55,    -1,    57,    58,    -1,    60,    -1,
      -1,    -1,    -1,    65,    -1,    -1,    -1,    -1,    70,    71,
      72,    -1,    -1,    75,    -1,    -1,    -1,    -1,    80,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      -1,    -1,    94,    -1,    -1,    97,    98,    99,    -1,    -1,
     102,    -1,   104,   105,    -1,   107,   108,    -1,   110,    -1,
     112,   113,    -1,    -1,   116,    -1,    -1,   119,   120,   121,
      -1,   123,    -1,    -1,    -1,   127,   128,    -1,   130,    -1,
     132,    -1,    -1,   135,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,    -1,   145,    -1,    -1,   148,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,
      -1,    -1,    -1,   165,    -1,    -1,   168,    -1,    -1,    -1,
     172,    -1,    -1,    -1,    -1,    -1,   178,   179,    -1,   181,
      -1,    -1,    -1,    -1,   186,   187,   188,   189,    -1,    -1,
     192,    -1,   194,    -1,   196,    -1,   198,   199,   200,   201,
      -1,    -1,   204,   205,   206,    -1,    -1,   209,    -1,    -1,
      -1,   213,   214,    -1,   216,   217,   218,   219,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   228,    -1,    -1,   231,
      -1,   233,   234,   235,   236,    -1,    -1,    -1,    -1,    -1,
     242,    -1,    -1,    -1,    -1,   247,    -1,    -1,    -1,    -1,
      -1,   253,    -1,    -1,    -1,   257,   258,   259,   260,   261,
     262,   263,    -1,   265,   266,    -1,    -1,    -1,   270,   271,
     272,   273,    -1,    -1,   276,    -1,   278,   279,   280,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   291,
     292,    -1,    -1,   295,    -1,    -1,   298,   299,   300,    -1,
      -1,   303,    -1,   305,   306,    -1,    -1,   309,    -1,    -1,
     312,    -1,   314,    -1,   316,   317,    -1,    -1,   320,    -1,
      -1,   323,   324,    -1,   326,   327,    -1,    -1,    -1,   331,
      -1,    -1,    -1,    -1,   336,    -1,    -1,    -1,    -1,   341,
      -1,   343,    -1,    -1,   346,    -1,    -1,    -1,    -1,   351,
     352,    -1,    -1,    -1,    -1,   357,    -1,    -1,    -1,   361
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    10,    11,    21,    43,    52,    63,    86,    87,    88,
      96,   106,   114,   123,   155,   167,   177,   245,   246,   249,
     257,   263,   267,   274,   277,   291,   323,   332,   334,   337,
     379,   387,   388,   389,   390,   483,   498,   500,   502,   504,
     508,   509,   645,   651,   656,   659,   682,   688,   691,   693,
     699,   702,   705,   711,   713,   714,   762,   781,   783,   789,
     790,   791,   792,    69,   213,   218,   487,   305,   308,   780,
     784,   780,   357,   785,    38,    69,   309,   407,   487,   147,
     690,    38,    69,   309,   487,   650,   142,   144,   145,   173,
     286,   311,   315,   385,   652,   752,   753,   759,   775,   776,
     706,   657,    57,   236,   712,    71,   719,   263,   780,   660,
     785,   752,   793,   512,   514,   515,   221,   764,   694,   320,
     308,   692,   782,   147,   495,     4,     6,     7,     8,    13,
      17,    18,    19,    21,    26,    28,    29,    31,    35,    39,
      42,    44,    45,    47,    48,    50,    51,    52,    53,    54,
      55,    57,    58,    60,    65,    70,    71,    72,    75,    80,
      81,    90,    91,    94,    97,    98,    99,   102,   104,   105,
     107,   108,   110,   112,   113,   116,   119,   120,   121,   123,
     127,   128,   130,   132,   135,   141,   143,   148,   149,   161,
     165,   168,   172,   178,   179,   181,   186,   187,   188,   189,
     192,   194,   196,   198,   199,   200,   201,   204,   205,   206,
     209,   213,   214,   216,   217,   218,   219,   228,   231,   233,
     234,   235,   236,   242,   247,   253,   257,   258,   259,   260,
     261,   262,   263,   265,   266,   270,   271,   272,   273,   276,
     278,   279,   280,   291,   292,   295,   298,   299,   300,   303,
     305,   306,   309,   312,   314,   316,   317,   320,   323,   324,
     326,   327,   331,   336,   341,   343,   346,   351,   352,   357,
     361,   750,   752,   756,   760,   761,   267,   379,   510,     0,
     106,   116,   384,   703,   749,   750,   756,   750,   495,   501,
     785,   503,    12,   786,   751,   756,   146,   408,   308,   392,
     129,   751,   146,   649,   150,   780,   775,   355,   653,   132,
     181,   292,   305,   707,   708,   780,   495,     6,    19,    24,
      25,    36,    37,    38,    41,    45,    47,    61,    62,    66,
      67,    69,    73,    74,    75,    80,    82,    85,   114,   115,
     117,   118,   122,   134,   137,   141,   146,   155,   156,   167,
     171,   182,   185,   189,   192,   193,   197,   198,   207,   208,
     210,   211,   229,   235,   248,   249,   256,   262,   265,   293,
     294,   299,   301,   302,   304,   311,   315,   316,   317,   321,
     322,   323,   325,   336,   339,   340,   341,   342,   347,   349,
     350,   361,   371,   372,   377,   379,   381,   384,   385,   531,
     532,   533,   534,   537,   538,   539,   540,   541,   546,   550,
     735,   738,   742,   746,   747,   752,   754,   756,   715,   793,
     505,   157,   648,   662,   663,   749,   318,   786,   513,     9,
      92,   285,   286,   287,   288,   297,   519,   520,   521,   522,
     523,   763,    48,    62,    63,    70,   110,   132,   149,   150,
     164,   178,   234,   272,   305,   308,   309,   351,   469,   695,
     752,   753,   758,   770,   355,   499,   648,   780,   749,   514,
     510,   380,   756,   701,   700,   384,   485,   308,   647,   648,
     647,    39,   209,   209,   245,   787,   207,   750,   408,   330,
     470,   749,   115,   750,   756,   649,   209,    55,   654,   292,
     355,   383,   709,   662,   379,   379,   537,   531,   560,   379,
     379,   379,   379,   379,   379,   379,   379,   530,   530,   379,
     379,   379,   379,   735,   379,   379,   379,   379,   379,   379,
     379,   379,   379,   379,   531,   712,   379,   379,   379,   379,
     379,   379,   379,   531,   379,   530,   379,   379,   379,   379,
     379,   379,   379,   379,   379,   379,   379,   379,   379,   530,
     379,   735,   379,   379,   379,   379,   379,   530,   530,   379,
     379,   379,   379,   379,   379,   537,   537,   537,   531,   805,
     806,   756,   756,   551,    12,   224,   359,   109,   131,   160,
     169,   203,   363,   364,   365,   535,    22,    93,   159,   174,
     197,   207,   243,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,    46,   366,   754,   379,   384,    55,   720,
     506,   507,   749,   663,   661,   263,   788,   787,   329,   794,
     795,   373,   525,   526,   527,   523,   765,   766,   767,   129,
     159,   697,   379,    69,   308,   174,   354,   698,   175,   629,
     631,   696,   697,   292,   305,   629,   697,   292,   292,   346,
      58,   274,   380,   795,   797,   798,   799,   508,    25,   137,
     704,   736,   754,   756,   756,    85,   415,   457,   749,   383,
      39,   245,   115,   393,   749,   150,   689,   215,   647,   254,
     350,   655,   239,   708,   647,   710,   658,   531,     9,    92,
     556,   612,   353,   562,   531,   380,   531,   559,   559,   531,
     531,    92,   556,   612,   380,   380,   531,   531,   531,   531,
     746,   531,   805,    76,    77,    78,    79,    80,   138,   139,
     140,   141,   189,   190,   191,   192,   198,   235,   264,   265,
     352,   360,   361,   607,   609,    92,   553,   531,   531,   531,
     531,   607,   379,   531,    92,   556,   531,   531,    92,   556,
     531,   531,   531,   534,   531,   531,   531,   531,   531,   531,
     556,   556,   531,   531,    92,   556,   531,   531,   128,   608,
     609,   608,    30,   170,   319,   531,   531,   380,   380,   746,
     556,   556,   531,   531,   380,   383,   380,   267,   379,   802,
     803,   531,   384,   385,   552,   775,   531,   531,   531,   533,
     118,   207,   210,   322,   331,     9,    13,   533,   536,   534,
     534,   379,   537,   534,    22,   159,   174,   243,   534,   534,
     534,   534,   534,   156,   534,   156,   534,   534,   534,   534,
     534,   752,   753,   757,   537,   542,   756,   151,   383,   318,
     267,   274,   342,   343,   379,   401,   664,   667,   793,     9,
      92,   801,   383,   516,   531,   744,   756,   383,   132,   178,
     216,   272,   768,   769,   772,   773,   749,   373,   408,   749,
     753,   531,   182,   211,   325,   632,   633,   698,   756,   696,
     698,   749,   698,   698,   280,   684,   685,   746,   223,   625,
     631,   800,    46,   484,   648,   250,   404,   405,   406,   415,
     757,   391,   756,   354,   613,   749,   180,   664,   383,   556,
     380,   557,   531,   101,   353,   561,    14,   380,   383,   380,
     380,   383,   547,   380,   373,   383,   383,   380,   380,   380,
     380,   380,   129,   549,   380,   383,   383,   383,   372,   531,
     383,   556,   380,   380,   380,   556,   380,   383,   380,   380,
     159,   380,   383,   383,   383,   383,   380,   380,   380,   383,
     129,   383,   556,   380,   380,   380,   383,   383,   129,   531,
     129,   531,   129,   531,   129,   380,   383,   380,   380,   380,
     380,   383,   380,   559,   581,   582,   583,   267,   379,   511,
     329,   807,   382,   756,   132,   178,   272,   771,   275,   118,
     210,   322,   331,   379,    12,   531,   805,   112,   617,   534,
     379,   537,   534,   531,   531,   527,   543,   544,   545,   384,
     311,   755,   507,   749,   402,   665,   379,   670,   675,   670,
     380,   401,   666,   743,   744,   746,   668,   796,   526,   129,
     157,   517,   518,   624,   625,   641,   528,   528,   384,   766,
     320,   776,   777,   385,   774,   696,   380,   756,   214,   383,
     698,   696,   383,   683,   275,   363,   673,    32,   629,   673,
     674,     5,    10,    18,    40,    50,    61,    90,    91,    96,
     102,   107,   119,   124,   148,   196,   223,   246,   250,   261,
     410,   412,   413,   414,   457,   486,   488,   489,   490,   621,
     757,   674,   383,   673,   174,   379,   394,   399,   409,   411,
     412,   471,   472,   614,   624,   646,   215,   680,   156,   531,
     380,   531,   313,   531,   531,   104,    24,    28,    41,    72,
      75,    83,   158,   278,   314,   333,   558,   531,   558,   559,
     380,   156,   156,   655,   531,   559,   531,   531,   531,   531,
     380,   531,   380,   380,   531,   531,   531,   531,   531,   559,
     156,   531,   531,   531,   380,   531,   531,   531,   129,   531,
     129,   531,   129,   531,   531,   531,   380,   519,   582,   511,
     380,   801,   384,   384,   384,   775,   531,   805,   533,   380,
     383,   380,   537,    12,   531,   805,   617,   607,   607,   531,
     380,   383,   756,   716,   519,   671,   672,   746,   676,   383,
     379,   667,   380,   380,   383,   794,   509,   267,   379,   381,
     563,   564,   565,   566,   567,   576,   577,   591,   642,   126,
     180,   524,   641,   630,   631,   518,    14,   529,   753,   756,
     373,   756,   161,   673,   385,   775,   698,   110,   351,   633,
     633,   613,   685,   613,    85,   531,   679,   626,    85,   455,
     456,   757,    49,    59,   150,   166,   422,   425,   426,   465,
     467,   494,   494,   674,   494,   674,   318,   164,   306,   125,
     232,   467,   494,   164,   673,   673,   306,   494,    32,    14,
     318,   497,   674,   673,   410,    46,   383,   379,   384,   421,
     427,   748,   756,   673,   118,   322,   405,    82,   122,   182,
     211,   325,   636,   757,   749,   399,   400,   401,   419,   420,
     421,   422,   409,    14,   147,   249,   396,   722,   383,   411,
     338,   475,   215,   531,   175,   634,    98,   531,   380,   531,
     313,   379,   436,   436,   379,   434,   435,   158,   158,   380,
     380,   548,   531,   531,   380,   555,   625,   383,   383,   380,
     383,   380,   380,   380,   380,   383,   380,   380,   531,   380,
     126,   380,   380,   383,   383,   383,   380,   531,   380,   531,
     380,   531,   380,   380,   380,   525,   380,   804,   384,   481,
     380,   559,   533,   380,   383,   380,   528,   545,   147,   249,
     721,   525,   383,   673,   677,   678,   679,   675,   669,   667,
     743,   590,   756,    64,   152,   163,   171,   202,   256,   297,
     575,   613,   383,   749,   590,    97,   226,   637,   638,   643,
     334,   159,   753,   756,   384,   172,     9,    24,    85,   215,
     531,   779,   771,   673,   624,   531,   627,   745,   480,   748,
      43,   125,   232,   330,   424,   466,   423,   424,   480,   748,
     636,   748,   753,   456,   166,   166,   748,   748,   757,   753,
     748,   622,   623,   746,   749,   118,   322,    53,    54,    85,
      99,   121,   242,   416,   417,   674,   490,   419,   756,     7,
     120,   496,   244,   423,   459,   428,   384,   636,   758,   380,
     380,   380,   383,    14,    14,   379,   400,   411,    31,   135,
     477,   749,   633,   681,   607,   531,   211,   211,   380,   607,
     607,   269,   554,   531,   531,   559,   531,   607,   531,   531,
     531,   531,   380,   380,   380,   584,   802,   756,   380,   559,
     529,   157,   403,   672,   679,   380,   383,   797,   578,   585,
     590,   563,   163,   163,   225,   592,   163,   171,   256,   592,
     576,   563,   133,   618,   565,    14,   610,   611,   587,   588,
     755,   755,   385,   639,   640,   276,   373,   239,   247,   270,
     778,   777,   531,   634,   383,    15,    87,   628,   531,   166,
     166,   467,   468,   480,   472,    96,   274,   491,   480,   492,
     383,   628,   456,   380,   756,   749,    23,    27,    28,    41,
      72,    75,    83,    95,   108,   121,   158,   212,   241,   271,
     312,   314,   315,   341,   344,   345,   429,   431,   432,   433,
     756,   409,   395,   409,   420,   400,   397,   379,   166,   380,
     380,   380,   383,   380,   380,   736,   380,   380,   383,   380,
     380,   380,   380,   380,   380,   380,   418,   518,   630,   380,
     717,   418,   679,   329,   380,   624,   586,   382,   163,   576,
     592,   592,   163,   215,   215,   338,    32,   136,   615,   756,
     601,   602,   519,   644,   775,   383,   194,    51,   326,   239,
     673,   745,   480,   472,   379,    85,    85,   427,   429,   623,
     379,   460,   379,   438,   443,   444,    46,    85,   207,   210,
     232,   330,   348,   441,   442,   445,   446,   447,   458,    85,
     215,   441,   442,   451,   452,   453,   443,   434,   230,   379,
     435,   440,   430,   434,   438,   434,   440,   445,   443,   379,
     451,   443,   379,    50,   454,   379,    18,    85,   271,   441,
     442,   448,   449,   450,   448,   384,   797,   396,   380,   794,
     478,   479,   756,   334,   211,   531,   524,   524,   308,   801,
     611,   630,   566,   563,   576,   163,   163,   563,   576,   569,
     568,   570,   619,   745,   616,   624,   599,   600,   525,    48,
     723,   640,   779,   628,   379,   379,   478,    75,   315,   371,
     372,   737,   738,   496,   454,   461,   756,   184,   462,   211,
     278,   333,   437,    85,   441,   442,    85,   441,   442,   455,
     737,   210,   166,   447,   166,    75,   208,   210,   315,   735,
     741,   742,   334,   453,   448,   440,   211,   379,   448,   437,
     448,   211,   211,   753,   211,    25,   137,   210,   371,   372,
     735,   739,   742,    85,   450,   756,   398,   380,   383,   628,
     379,   686,   687,   746,   380,   380,   749,   579,   215,   572,
     576,   576,   215,   574,   531,   531,   379,   356,   383,   620,
     628,   531,   630,   124,   147,   337,   594,   595,   598,   589,
     103,   111,   222,   310,   724,   725,   176,   726,   478,   478,
     380,   742,   742,   493,   380,   383,   130,   228,   279,   215,
     463,   380,   362,   439,   737,   118,   210,   322,   740,   735,
     530,   735,   208,   380,   482,   736,   439,   380,   380,   380,
     742,   742,   343,   797,    50,   165,   473,   474,   475,   476,
     479,   211,   383,   673,   718,   802,   571,   338,   573,   338,
     606,   756,   745,   467,   467,   595,   418,    32,    32,   103,
      32,   725,   290,   310,   727,   728,   380,   380,   473,   496,
     756,    86,   334,   530,   380,   383,   451,   443,   445,   753,
     674,   476,   628,   380,   687,   679,   723,   580,   531,   379,
     531,   379,   380,   383,   628,   126,   593,   593,   736,   736,
      32,   736,    32,    32,   728,   459,   473,    34,   209,   252,
     274,   464,   464,   445,   736,    82,   122,   137,   182,   211,
     325,   635,   726,   624,   606,   606,   756,   133,   163,   223,
     597,   596,   736,   736,   736,     4,    85,   210,   215,   215,
     147,   729,   630,   380,   380,    32,    32,   379,   379,   334,
      86,   211,   379,   731,   232,   603,   604,   605,   756,   605,
     464,   464,   176,   260,   730,   380,   385,   732,   733,   746,
     274,   734,   380,   383,   380,   775,   380,   383,   686,   604,
     733
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (session, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, session)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, session); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, drizzled::Session *session)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, session)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    drizzled::Session *session;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (session);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, drizzled::Session *session)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, session)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    drizzled::Session *session;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, session);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule, drizzled::Session *session)
#else
static void
yy_reduce_print (yyvsp, yyrule, session)
    YYSTYPE *yyvsp;
    int yyrule;
    drizzled::Session *session;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       , session);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule, session); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, drizzled::Session *session)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, session)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    drizzled::Session *session;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (session);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (drizzled::Session *session);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */





/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (drizzled::Session *session)
#else
int
yyparse (session)
    drizzled::Session *session;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 799 "drizzled/sql_yacc.yy"
    {
            if (!(YYSession->lex().select_lex.options & OPTION_FOUND_COMMENT))
            {
              my_message(ER_EMPTY_QUERY, ER(ER_EMPTY_QUERY), MYF(0));
              DRIZZLE_YYABORT;
            }
            else
            {
              YYSession->lex().statement= new statement::EmptyQuery(YYSession);
            }
          }
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 810 "drizzled/sql_yacc.yy"
    {}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 852 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::catalog::Create(YYSession, (yyvsp[(3) - (3)].lex_str));
          }
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 856 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::CreateTable(YYSession, (yyvsp[(5) - (5)].table), (yyvsp[(2) - (5)].num));

            if (not Lex.select_lex.add_table_to_list(YYSession, (yyvsp[(5) - (5)].table), NULL,
                                                     TL_OPTION_UPDATING,
                                                     TL_WRITE))
              DRIZZLE_YYABORT;
            Lex.col_list.clear();
          }
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 866 "drizzled/sql_yacc.yy"
    {
            Lex.current_select= &Lex.select_lex;
          }
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 870 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::CreateIndex(YYSession, (yyvsp[(2) - (2)].build_method));
          }
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 874 "drizzled/sql_yacc.yy"
    {
            if (not Lex.current_select->add_table_to_list(Lex.session, (yyvsp[(9) - (13)].table),
                                                            NULL,
                                                            TL_OPTION_UPDATING))
              DRIZZLE_YYABORT;

            parser::buildKey(&Lex, (yyvsp[(4) - (13)].key_type), (yyvsp[(6) - (13)].lex_str));
          }
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 883 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::CreateSchema(YYSession);
          }
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 887 "drizzled/sql_yacc.yy"
    {
            Lex.name= (yyvsp[(4) - (6)].lex_str);
          }
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 894 "drizzled/sql_yacc.yy"
    { }
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 896 "drizzled/sql_yacc.yy"
    {
             Lex.current_select->set_braces(1);
           }
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 899 "drizzled/sql_yacc.yy"
    {}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 901 "drizzled/sql_yacc.yy"
    { }
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 903 "drizzled/sql_yacc.yy"
    { }
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 905 "drizzled/sql_yacc.yy"
    { }
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 909 "drizzled/sql_yacc.yy"
    {}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 911 "drizzled/sql_yacc.yy"
    {
            Lex.current_select->set_braces(0);
          }
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 914 "drizzled/sql_yacc.yy"
    {}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 916 "drizzled/sql_yacc.yy"
    {
            Lex.current_select->set_braces(1);
          }
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 919 "drizzled/sql_yacc.yy"
    {}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 924 "drizzled/sql_yacc.yy"
    {
            ((statement::CreateTable *)(YYSession->lex().statement))->is_create_table_like= true;

            if (not YYSession->lex().select_lex.add_table_to_list(YYSession, (yyvsp[(2) - (2)].table), NULL, 0, TL_READ))
              DRIZZLE_YYABORT;
          }
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 934 "drizzled/sql_yacc.yy"
    {
          }
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 943 "drizzled/sql_yacc.yy"
    {
            Lex.lock_option= TL_READ;
            if (Lex.sql_command == SQLCOM_INSERT)
            {
              delete Lex.statement;
              Lex.statement= new statement::InsertSelect(YYSession);
            }
            else if (Lex.sql_command == SQLCOM_REPLACE)
            {
              delete Lex.statement;
              Lex.statement= new statement::ReplaceSelect(YYSession);
            }
            /*
              The following work only with the local list, the global list
              is created correctly in this case
            */
            Lex.current_select->table_list.save_and_clear(&Lex.save_list);
            init_select(&Lex);
            Lex.current_select->parsing_place= SELECT_LIST;
          }
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 964 "drizzled/sql_yacc.yy"
    {
            Lex.current_select->parsing_place= NO_MATTER;
          }
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 968 "drizzled/sql_yacc.yy"
    {
            /*
              The following work only with the local list, the global list
              is created correctly in this case
            */
            Lex.current_select->table_list.push_front(&Lex.save_list);
          }
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 978 "drizzled/sql_yacc.yy"
    {}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 979 "drizzled/sql_yacc.yy"
    {}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 980 "drizzled/sql_yacc.yy"
    {}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 990 "drizzled/sql_yacc.yy"
    {
            statement::CreateSchema *statement= (statement::CreateSchema *)Lex.statement;
            statement->schema_message.mutable_engine()->add_options()->set_name((yyvsp[(1) - (1)].lex_str).str);
          }
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 995 "drizzled/sql_yacc.yy"
    {
            parser::buildReplicationOption(&Lex, true);
          }
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 999 "drizzled/sql_yacc.yy"
    {
            parser::buildReplicationOption(&Lex, false);
          }
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 1003 "drizzled/sql_yacc.yy"
    {
            parser::buildSchemaOption(&Lex, (yyvsp[(1) - (3)].lex_str).str, (yyvsp[(3) - (3)].lex_str));
          }
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 1007 "drizzled/sql_yacc.yy"
    {
            parser::buildSchemaOption(&Lex, (yyvsp[(1) - (3)].lex_str).str, (yyvsp[(3) - (3)].ulonglong_number));
          }
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 1013 "drizzled/sql_yacc.yy"
    { (yyval.num)= false; }
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 1014 "drizzled/sql_yacc.yy"
    { (yyval.num)= true; }
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 1018 "drizzled/sql_yacc.yy"
    { (yyval.boolean)= false; }
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 1019 "drizzled/sql_yacc.yy"
    { (yyval.boolean)= true; YYSession->lex().setExists(); }
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 1042 "drizzled/sql_yacc.yy"
    {
            Lex.table()->mutable_engine()->set_name((yyvsp[(3) - (3)].lex_str).str);
          }
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 1046 "drizzled/sql_yacc.yy"
    {
            Lex.table()->mutable_options()->set_comment((yyvsp[(3) - (3)].lex_str).str);
          }
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 1050 "drizzled/sql_yacc.yy"
    {
            Lex.table()->mutable_options()->set_auto_increment_value((yyvsp[(3) - (3)].ulonglong_number));
          }
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 1054 "drizzled/sql_yacc.yy"
    {
	    message::set_is_replicated(*Lex.table(), true);
          }
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 1058 "drizzled/sql_yacc.yy"
    {
	    message::set_is_replicated(*Lex.table(), false);
          }
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 1062 "drizzled/sql_yacc.yy"
    {
            parser::buildEngineOption(&Lex, "ROW_FORMAT", (yyvsp[(3) - (3)].lex_str));
          }
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 1066 "drizzled/sql_yacc.yy"
    {
            parser::buildEngineOption(&Lex, "FILE", (yyvsp[(3) - (3)].lex_str));
          }
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 1070 "drizzled/sql_yacc.yy"
    {
            parser::buildEngineOption(&Lex, (yyvsp[(1) - (3)].lex_str).str, (yyvsp[(3) - (3)].lex_str));
          }
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 1074 "drizzled/sql_yacc.yy"
    {
            parser::buildEngineOption(&Lex, (yyvsp[(1) - (3)].lex_str).str, (yyvsp[(3) - (3)].ulonglong_number));
          }
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 1082 "drizzled/sql_yacc.yy"
    {
            if (not parser::buildCollation(&Lex, (yyvsp[(4) - (4)].charset)))
            {
              DRIZZLE_YYABORT;
            }
          }
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 1092 "drizzled/sql_yacc.yy"
    {
            ((statement::CreateSchema *)Lex.statement)->schema_message.set_collation((yyvsp[(4) - (4)].charset)->name);
          }
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 1098 "drizzled/sql_yacc.yy"
    {}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 1099 "drizzled/sql_yacc.yy"
    {}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 1100 "drizzled/sql_yacc.yy"
    {}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 1101 "drizzled/sql_yacc.yy"
    {}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 1102 "drizzled/sql_yacc.yy"
    {}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 1103 "drizzled/sql_yacc.yy"
    {}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 1108 "drizzled/sql_yacc.yy"
    {
            (yyval.lex_str).str= YYSession->strmake((yyvsp[(1) - (1)].symbol).str, (yyvsp[(1) - (1)].symbol).length);
            (yyval.lex_str).length= (yyvsp[(1) - (1)].symbol).length;
          }
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 1115 "drizzled/sql_yacc.yy"
    {}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 1132 "drizzled/sql_yacc.yy"
    {
            Lex.col_list.clear(); /* Alloced by memory::sql_alloc */
          }
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 1139 "drizzled/sql_yacc.yy"
    {
            parser::buildKey(&Lex, (yyvsp[(1) - (7)].key_type), (yyvsp[(2) - (7)].lex_str));
          }
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 1144 "drizzled/sql_yacc.yy"
    {
            parser::buildKey(&Lex, (yyvsp[(2) - (8)].key_type), (yyvsp[(3) - (8)].lex_str).str ? (yyvsp[(3) - (8)].lex_str) : (yyvsp[(1) - (8)].lex_str));
          }
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 1148 "drizzled/sql_yacc.yy"
    {
            parser::buildForeignKey(&Lex, (yyvsp[(1) - (8)].lex_str).str ? (yyvsp[(1) - (8)].lex_str) : (yyvsp[(4) - (8)].lex_str), (yyvsp[(8) - (8)].table));
          }
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 1152 "drizzled/sql_yacc.yy"
    {
            Lex.col_list.clear(); /* Alloced by memory::sql_alloc */
          }
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 1156 "drizzled/sql_yacc.yy"
    {
            Lex.col_list.clear(); /* Alloced by memory::sql_alloc */
          }
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 1171 "drizzled/sql_yacc.yy"
    { (yyval.lex_str)= null_lex_str; }
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 1172 "drizzled/sql_yacc.yy"
    { (yyval.lex_str)= (yyvsp[(1) - (1)].lex_str); }
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 1176 "drizzled/sql_yacc.yy"
    { (yyval.lex_str)=(yyvsp[(2) - (2)].lex_str); }
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 1181 "drizzled/sql_yacc.yy"
    {
            parser::buildCreateFieldIdent(&Lex);
          }
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 1185 "drizzled/sql_yacc.yy"
    {
            statement::CreateTable *statement= (statement::CreateTable *)Lex.statement;

            if (Lex.field())
            {
              Lex.field()->set_name((yyvsp[(1) - (4)].lex_str).str);
            }

            if (add_field_to_list(Lex.session, &(yyvsp[(1) - (4)].lex_str), (enum enum_field_types) (yyvsp[(3) - (4)].field_val),
                                  Lex.length, Lex.dec, Lex.type,
                                  statement->column_format,
                                  statement->default_value, statement->on_update_value,
                                  &statement->comment,
                                  statement->change, &Lex.interval_list, Lex.charset))
              DRIZZLE_YYABORT;

            Lex.setField(NULL);
          }
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 1207 "drizzled/sql_yacc.yy"
    {
            (yyval.field_val)=parser::buildTimestampColumn(&Lex, NULL);
          }
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 1211 "drizzled/sql_yacc.yy"
    {
            (yyval.field_val)=parser::buildTimestampColumn(&Lex, (yyvsp[(3) - (5)].lex_str).str);
          }
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 1215 "drizzled/sql_yacc.yy"
    {
            (yyval.field_val)=DRIZZLE_TYPE_DATETIME;

            if (Lex.field())
              Lex.field()->set_type(message::Table::Field::DATETIME);
          }
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 1222 "drizzled/sql_yacc.yy"
    {
            (yyval.field_val)= (yyvsp[(1) - (2)].field_val);
          }
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 1226 "drizzled/sql_yacc.yy"
    {
            (yyval.field_val)= parser::buildVarcharColumn(&Lex, "1");
          }
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 1230 "drizzled/sql_yacc.yy"
    {
            (yyval.field_val)= parser::buildVarcharColumn(&Lex, (yyvsp[(3) - (5)].lex_str).str);
          }
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 1234 "drizzled/sql_yacc.yy"
    {
            (yyval.field_val)=DRIZZLE_TYPE_BLOB;
            Lex.length=(char*) 0; /* use default length */

            if (Lex.field())
              Lex.field()->set_type(message::Table::Field::BLOB);
          }
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 1242 "drizzled/sql_yacc.yy"
    {
            Lex.interval_list.clear();
          }
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 1246 "drizzled/sql_yacc.yy"
    {
            (yyval.field_val)=DRIZZLE_TYPE_ENUM;

            if (Lex.field())
              Lex.field()->set_type(message::Table::Field::ENUM);
          }
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 1253 "drizzled/sql_yacc.yy"
    {
            (yyval.field_val)= parser::buildVarbinaryColumn(&Lex, (yyvsp[(3) - (5)].lex_str).str);
          }
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 1257 "drizzled/sql_yacc.yy"
    {
            assert ((yyvsp[(1) - (2)].field_val) == DRIZZLE_TYPE_DOUBLE);
            (yyval.field_val)= parser::buildDoubleColumn(&Lex);
          }
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 1262 "drizzled/sql_yacc.yy"
    {
            (yyval.field_val)=DRIZZLE_TYPE_DATE;

            if (Lex.field())
              Lex.field()->set_type(message::Table::Field::DATE);
          }
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 1269 "drizzled/sql_yacc.yy"
    {
            (yyval.field_val)=DRIZZLE_TYPE_TIME;

            if (Lex.field())
              Lex.field()->set_type(message::Table::Field::TIME);
          }
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 1276 "drizzled/sql_yacc.yy"
    {
            (yyval.field_val)= parser::buildBlobColumn(&Lex);
          }
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 1280 "drizzled/sql_yacc.yy"
    {
            (yyval.field_val)= parser::buildDecimalColumn(&Lex);
          }
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 1284 "drizzled/sql_yacc.yy"
    {
            (yyval.field_val)= parser::buildDecimalColumn(&Lex);
          }
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 1288 "drizzled/sql_yacc.yy"
    {
            (yyval.field_val)= parser::buildDecimalColumn(&Lex);
          }
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 1292 "drizzled/sql_yacc.yy"
    {
            (yyval.field_val)= parser::buildUuidColumn(&Lex);
          }
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 1296 "drizzled/sql_yacc.yy"
    {
            (yyval.field_val)= parser::buildBooleanColumn(&Lex);
          }
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 1300 "drizzled/sql_yacc.yy"
    {
            (yyval.field_val)= parser::buildSerialColumn(&Lex);
          }
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 1307 "drizzled/sql_yacc.yy"
    { }
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 1309 "drizzled/sql_yacc.yy"
    { }
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 1311 "drizzled/sql_yacc.yy"
    { }
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 1316 "drizzled/sql_yacc.yy"
    {
            (yyval.field_val)= parser::buildIntegerColumn(&Lex, DRIZZLE_TYPE_LONG, ((yyvsp[(3) - (4)].boolean) or (yyvsp[(4) - (4)].boolean)));
          }
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 1320 "drizzled/sql_yacc.yy"
    {
            (yyval.field_val)= parser::buildIntegerColumn(&Lex, DRIZZLE_TYPE_LONGLONG, ((yyvsp[(3) - (4)].boolean) or (yyvsp[(4) - (4)].boolean)));
          }
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 1327 "drizzled/sql_yacc.yy"
    {
            (yyval.field_val)= DRIZZLE_TYPE_DOUBLE;
          }
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 1331 "drizzled/sql_yacc.yy"
    {
            (yyval.field_val)= DRIZZLE_TYPE_DOUBLE;
          }
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 1335 "drizzled/sql_yacc.yy"
    {
            (yyval.field_val)= DRIZZLE_TYPE_DOUBLE;
          }
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 1342 "drizzled/sql_yacc.yy"
    { Lex.dec=Lex.length= (char*)0; }
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 1344 "drizzled/sql_yacc.yy"
    { Lex.length=(yyvsp[(2) - (3)].lex_str).str; Lex.dec= (char*)0; }
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 1346 "drizzled/sql_yacc.yy"
    {}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 1351 "drizzled/sql_yacc.yy"
    {
            Lex.length= (yyvsp[(2) - (5)].lex_str).str;
            Lex.dec= (yyvsp[(4) - (5)].lex_str).str;
          }
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 1358 "drizzled/sql_yacc.yy"
    { Lex.length=(char*) 0; /* use default length */ }
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 1359 "drizzled/sql_yacc.yy"
    { Lex.length= (yyvsp[(2) - (3)].lex_str).str; }
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 1363 "drizzled/sql_yacc.yy"
    { (yyval.boolean)= false; }
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 1364 "drizzled/sql_yacc.yy"
    { (yyval.boolean)= false; }
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 1365 "drizzled/sql_yacc.yy"
    { (yyval.boolean)= true; Lex.type|= UNSIGNED_FLAG; }
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 1369 "drizzled/sql_yacc.yy"
    { }
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 1370 "drizzled/sql_yacc.yy"
    { }
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 1374 "drizzled/sql_yacc.yy"
    { (yyval.boolean)= false; }
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 1375 "drizzled/sql_yacc.yy"
    { (yyval.boolean)= true; Lex.type|= UNSIGNED_FLAG; }
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 1380 "drizzled/sql_yacc.yy"
    { Lex.dec=Lex.length= (char*)0; }
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 1382 "drizzled/sql_yacc.yy"
    { Lex.length=Lex.dec= (char*)0; }
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 1384 "drizzled/sql_yacc.yy"
    {}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 1389 "drizzled/sql_yacc.yy"
    {
            Lex.type&= ~ NOT_NULL_FLAG;
          }
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 1393 "drizzled/sql_yacc.yy"
    {
            Lex.type|= NOT_NULL_FLAG;

            if (Lex.field())
            {
              Lex.field()->mutable_constraints()->set_is_notnull(true);
            }
          }
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 1405 "drizzled/sql_yacc.yy"
    {
            parser::buildPrimaryOnColumn(&Lex);
          }
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 1409 "drizzled/sql_yacc.yy"
    {
            parser::buildKeyOnColumn(&Lex);
          }
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1413 "drizzled/sql_yacc.yy"
    {
            parser::buildKeyOnColumn(&Lex);
          }
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1419 "drizzled/sql_yacc.yy"
    {}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1421 "drizzled/sql_yacc.yy"
    { }
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1423 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= (statement::AlterTable *)Lex.statement;

            statement->default_value= (yyvsp[(3) - (3)].item);
            statement->alter_info.flags.set(ALTER_COLUMN_DEFAULT);
          }
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1430 "drizzled/sql_yacc.yy"
    { }
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1434 "drizzled/sql_yacc.yy"
    { }
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1436 "drizzled/sql_yacc.yy"
    { }
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1438 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= (statement::AlterTable *)Lex.statement;

            statement->default_value= (yyvsp[(3) - (3)].item);
            statement->alter_info.flags.set(ALTER_COLUMN_DEFAULT);
          }
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1445 "drizzled/sql_yacc.yy"
    { }
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1449 "drizzled/sql_yacc.yy"
    {}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1450 "drizzled/sql_yacc.yy"
    {}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1454 "drizzled/sql_yacc.yy"
    {}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1460 "drizzled/sql_yacc.yy"
    { }
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1462 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= (statement::AlterTable *)Lex.statement;

            statement->default_value=(yyvsp[(2) - (2)].item);
            statement->alter_info.flags.set(ALTER_COLUMN_DEFAULT);
          }
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1469 "drizzled/sql_yacc.yy"
    {
            if (Lex.charset && !my_charset_same(Lex.charset,(yyvsp[(2) - (2)].charset)))
            {
              my_error(ER_COLLATION_CHARSET_MISMATCH, MYF(0),
                       (yyvsp[(2) - (2)].charset)->name,Lex.charset->csname);
              DRIZZLE_YYABORT;
            }
            else
            {
              Lex.charset=(yyvsp[(2) - (2)].charset);
            }
          }
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1482 "drizzled/sql_yacc.yy"
    { }
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1486 "drizzled/sql_yacc.yy"
    {}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1487 "drizzled/sql_yacc.yy"
    {}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1491 "drizzled/sql_yacc.yy"
    {}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1497 "drizzled/sql_yacc.yy"
    { }
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1499 "drizzled/sql_yacc.yy"
    {
            parser::buildAutoOnColumn(&Lex);
          }
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1503 "drizzled/sql_yacc.yy"
    {
            (void)parser::buildSerialColumn(&Lex);
          }
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1507 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= (statement::AlterTable *)Lex.statement;

            statement->default_value=(yyvsp[(2) - (2)].item);
            statement->alter_info.flags.set(ALTER_COLUMN_DEFAULT);
          }
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1514 "drizzled/sql_yacc.yy"
    { }
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1518 "drizzled/sql_yacc.yy"
    {}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1519 "drizzled/sql_yacc.yy"
    {}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1523 "drizzled/sql_yacc.yy"
    {}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1529 "drizzled/sql_yacc.yy"
    { }
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1531 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= (statement::AlterTable *)Lex.statement;

            statement->default_value= new Item_func_now_local();
            statement->alter_info.flags.set(ALTER_COLUMN_DEFAULT);
          }
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1538 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= (statement::AlterTable *)Lex.statement;

            statement->default_value=(yyvsp[(2) - (2)].item);
            statement->alter_info.flags.set(ALTER_COLUMN_DEFAULT);
          }
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1545 "drizzled/sql_yacc.yy"
    {
            ((statement::AlterTable *)Lex.statement)->on_update_value= new Item_func_now_local();
          }
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1549 "drizzled/sql_yacc.yy"
    { }
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1553 "drizzled/sql_yacc.yy"
    { }
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1555 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= (statement::AlterTable *)Lex.statement;
            statement->comment= (yyvsp[(2) - (2)].lex_str);

            if (Lex.field())
              Lex.field()->set_comment((yyvsp[(2) - (2)].lex_str).str);
          }
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1566 "drizzled/sql_yacc.yy"
    {
            if (!((yyval.charset)=get_charset_by_name((yyvsp[(1) - (1)].lex_str).str)))
            {
              my_error(ER_UNKNOWN_COLLATION, MYF(0), (yyvsp[(1) - (1)].lex_str).str);
              DRIZZLE_YYABORT;
            }
          }
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1576 "drizzled/sql_yacc.yy"
    { (yyval.charset)=(yyvsp[(1) - (1)].charset); }
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1577 "drizzled/sql_yacc.yy"
    { (yyval.charset)=NULL; }
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1581 "drizzled/sql_yacc.yy"
    {}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1582 "drizzled/sql_yacc.yy"
    {}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1592 "drizzled/sql_yacc.yy"
    {
            (yyval.table)=(yyvsp[(2) - (5)].table);
          }
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1599 "drizzled/sql_yacc.yy"
    { Lex.ref_list.clear(); }
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1605 "drizzled/sql_yacc.yy"
    { Lex.ref_list.push_back(new Key_part_spec((yyvsp[(3) - (3)].lex_str), 0)); }
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1607 "drizzled/sql_yacc.yy"
    {
            Lex.ref_list.clear();
            Lex.ref_list.push_back(new Key_part_spec((yyvsp[(1) - (1)].lex_str), 0));
          }
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1615 "drizzled/sql_yacc.yy"
    { ((statement::CreateTable *)Lex.statement)->fk_match_option= drizzled::message::Table::ForeignKeyConstraint::MATCH_UNDEFINED; }
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1617 "drizzled/sql_yacc.yy"
    { ((statement::CreateTable *)Lex.statement)->fk_match_option= drizzled::message::Table::ForeignKeyConstraint::MATCH_FULL; }
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1619 "drizzled/sql_yacc.yy"
    { ((statement::CreateTable *)Lex.statement)->fk_match_option= drizzled::message::Table::ForeignKeyConstraint::MATCH_PARTIAL; }
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1621 "drizzled/sql_yacc.yy"
    { ((statement::CreateTable *)Lex.statement)->fk_match_option= drizzled::message::Table::ForeignKeyConstraint::MATCH_SIMPLE; }
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1626 "drizzled/sql_yacc.yy"
    {
            ((statement::CreateTable *)Lex.statement)->fk_update_opt= drizzled::message::Table::ForeignKeyConstraint::OPTION_UNDEF;
            ((statement::CreateTable *)Lex.statement)->fk_delete_opt= drizzled::message::Table::ForeignKeyConstraint::OPTION_UNDEF;
          }
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1631 "drizzled/sql_yacc.yy"
    {
            ((statement::CreateTable *)Lex.statement)->fk_update_opt= (yyvsp[(3) - (3)].m_fk_option);
            ((statement::CreateTable *)Lex.statement)->fk_delete_opt= drizzled::message::Table::ForeignKeyConstraint::OPTION_UNDEF;
          }
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1636 "drizzled/sql_yacc.yy"
    {
            ((statement::CreateTable *)Lex.statement)->fk_update_opt= drizzled::message::Table::ForeignKeyConstraint::OPTION_UNDEF;
            ((statement::CreateTable *)Lex.statement)->fk_delete_opt= (yyvsp[(3) - (3)].m_fk_option);
          }
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1642 "drizzled/sql_yacc.yy"
    {
            ((statement::CreateTable *)Lex.statement)->fk_update_opt= (yyvsp[(3) - (6)].m_fk_option);
            ((statement::CreateTable *)Lex.statement)->fk_delete_opt= (yyvsp[(6) - (6)].m_fk_option);
          }
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1648 "drizzled/sql_yacc.yy"
    {
            ((statement::CreateTable *)Lex.statement)->fk_update_opt= (yyvsp[(6) - (6)].m_fk_option);
            ((statement::CreateTable *)Lex.statement)->fk_delete_opt= (yyvsp[(3) - (6)].m_fk_option);
          }
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1655 "drizzled/sql_yacc.yy"
    { (yyval.m_fk_option)= drizzled::message::Table::ForeignKeyConstraint::OPTION_RESTRICT; }
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1656 "drizzled/sql_yacc.yy"
    { (yyval.m_fk_option)= drizzled::message::Table::ForeignKeyConstraint::OPTION_CASCADE; }
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1657 "drizzled/sql_yacc.yy"
    { (yyval.m_fk_option)= drizzled::message::Table::ForeignKeyConstraint::OPTION_SET_NULL; }
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1658 "drizzled/sql_yacc.yy"
    { (yyval.m_fk_option)= drizzled::message::Table::ForeignKeyConstraint::OPTION_NO_ACTION; }
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1659 "drizzled/sql_yacc.yy"
    { (yyval.m_fk_option)= drizzled::message::Table::ForeignKeyConstraint::OPTION_SET_DEFAULT;  }
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1663 "drizzled/sql_yacc.yy"
    { (yyval.key_type)= Key::MULTIPLE; }
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1667 "drizzled/sql_yacc.yy"
    { (yyval.key_type)= Key::PRIMARY; }
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1668 "drizzled/sql_yacc.yy"
    { (yyval.key_type)= Key::UNIQUE; }
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1672 "drizzled/sql_yacc.yy"
    {}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1673 "drizzled/sql_yacc.yy"
    {}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1677 "drizzled/sql_yacc.yy"
    {}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1682 "drizzled/sql_yacc.yy"
    {}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1683 "drizzled/sql_yacc.yy"
    {}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1684 "drizzled/sql_yacc.yy"
    {}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1688 "drizzled/sql_yacc.yy"
    { (yyval.key_type)= Key::MULTIPLE; }
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1689 "drizzled/sql_yacc.yy"
    { (yyval.key_type)= Key::UNIQUE; }
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1693 "drizzled/sql_yacc.yy"
    {
            ((statement::CreateTable *)Lex.statement)->key_create_info= default_key_create_info;
          }
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1710 "drizzled/sql_yacc.yy"
    {}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1720 "drizzled/sql_yacc.yy"
    { ((statement::CreateTable *)Lex.statement)->key_create_info.algorithm= (yyvsp[(2) - (2)].key_alg); }
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1726 "drizzled/sql_yacc.yy"
    { ((statement::CreateTable *)Lex.statement)->key_create_info.block_size= (yyvsp[(3) - (3)].ulong_num); }
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1728 "drizzled/sql_yacc.yy"
    { ((statement::CreateTable *)Lex.statement)->key_create_info.comment= (yyvsp[(2) - (2)].lex_str); }
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1732 "drizzled/sql_yacc.yy"
    { (yyval.key_alg)= HA_KEY_ALG_BTREE; }
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1733 "drizzled/sql_yacc.yy"
    { (yyval.key_alg)= HA_KEY_ALG_HASH; }
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1737 "drizzled/sql_yacc.yy"
    { Lex.col_list.push_back((yyvsp[(3) - (4)].key_part)); }
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1738 "drizzled/sql_yacc.yy"
    { Lex.col_list.push_back((yyvsp[(1) - (2)].key_part)); }
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1742 "drizzled/sql_yacc.yy"
    { (yyval.key_part)=new Key_part_spec((yyvsp[(1) - (1)].lex_str), 0); }
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1744 "drizzled/sql_yacc.yy"
    {
            int key_part_len= atoi((yyvsp[(3) - (4)].lex_str).str);
            if (!key_part_len)
            {
              my_error(ER_KEY_PART_0, MYF(0), (yyvsp[(1) - (4)].lex_str).str);
            }
            (yyval.key_part)=new Key_part_spec((yyvsp[(1) - (4)].lex_str), (uint) key_part_len);
          }
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1755 "drizzled/sql_yacc.yy"
    { (yyval.lex_str)= null_lex_str; }
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1756 "drizzled/sql_yacc.yy"
    { (yyval.lex_str)= (yyvsp[(1) - (1)].lex_str); }
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1760 "drizzled/sql_yacc.yy"
    { (yyval.lex_str)= null_lex_str; }
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1761 "drizzled/sql_yacc.yy"
    { (yyval.lex_str)= (yyvsp[(2) - (2)].lex_str); }
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 1765 "drizzled/sql_yacc.yy"
    { Lex.interval_list.push_back((yyvsp[(1) - (1)].string)); }
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1766 "drizzled/sql_yacc.yy"
    { Lex.interval_list.push_back((yyvsp[(3) - (3)].string)); }
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1774 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= new statement::AlterTable(YYSession, (yyvsp[(5) - (5)].table), (yyvsp[(2) - (5)].build_method));
            Lex.statement= statement;
            Lex.duplicates= DUP_ERROR;
            if (not Lex.select_lex.add_table_to_list(YYSession, (yyvsp[(5) - (5)].table), NULL, TL_OPTION_UPDATING))
            {
              DRIZZLE_YYABORT;
            }

            Lex.col_list.clear();
            Lex.select_lex.init_order();
            Lex.select_lex.db= const_cast<char *>(((TableList*) Lex.select_lex.table_list.first)->getSchemaName());
          }
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1788 "drizzled/sql_yacc.yy"
    {}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 1790 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::AlterSchema(YYSession);
          }
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 1794 "drizzled/sql_yacc.yy"
    {
            Lex.name= (yyvsp[(3) - (5)].lex_str);
            if (Lex.name.str == NULL && Lex.copy_db_to(&Lex.name.str, &Lex.name.length))
              DRIZZLE_YYABORT;
          }
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 1804 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= (statement::AlterTable *)Lex.statement;
            statement->alter_info.tablespace_op= DISCARD_TABLESPACE;
          }
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 1809 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= (statement::AlterTable *)Lex.statement;
            statement->alter_info.tablespace_op= IMPORT_TABLESPACE;
          }
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 1818 "drizzled/sql_yacc.yy"
    {
            (yyval.build_method)= HA_BUILD_DEFAULT;
          }
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 1822 "drizzled/sql_yacc.yy"
    {
            (yyval.build_method)= HA_BUILD_ONLINE;
          }
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 1826 "drizzled/sql_yacc.yy"
    {
            (yyval.build_method)= HA_BUILD_OFFLINE;
          }
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 1838 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= (statement::AlterTable *)Lex.statement;

            statement->change=0;
            statement->alter_info.flags.set(ALTER_ADD_COLUMN);
          }
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 1847 "drizzled/sql_yacc.yy"
    { }
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 1849 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= (statement::AlterTable *)Lex.statement;

            statement->alter_info.flags.set(ALTER_ADD_INDEX);
          }
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 1855 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= (statement::AlterTable *)Lex.statement;

            statement->alter_info.flags.set(ALTER_ADD_COLUMN);
            statement->alter_info.flags.set(ALTER_ADD_INDEX);
          }
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 1862 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= (statement::AlterTable *)Lex.statement;
            statement->change= (yyvsp[(3) - (3)].lex_str).str;
            statement->alter_info.flags.set(ALTER_CHANGE_COLUMN);
          }
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 1869 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= (statement::AlterTable *)Lex.statement;
            Lex.length= Lex.dec=0;
            Lex.type= 0;
            statement->default_value= statement->on_update_value= 0;
            statement->comment= null_lex_str;
            Lex.charset= NULL;
            statement->alter_info.flags.set(ALTER_CHANGE_COLUMN);
            statement->column_format= COLUMN_FORMAT_TYPE_DEFAULT;

            Lex.setField(NULL);
          }
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 1882 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= (statement::AlterTable *)Lex.statement;

            if (add_field_to_list(Lex.session,&(yyvsp[(3) - (6)].lex_str),
                                  (enum enum_field_types) (yyvsp[(5) - (6)].field_val),
                                  Lex.length, Lex.dec, Lex.type,
                                  statement->column_format,
                                  statement->default_value,
                                  statement->on_update_value,
                                  &statement->comment,
                                  (yyvsp[(3) - (6)].lex_str).str, &Lex.interval_list, Lex.charset))
              DRIZZLE_YYABORT;
          }
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 1897 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= (statement::AlterTable *)Lex.statement;

            statement->alter_info.drop_list.push_back(AlterDrop(AlterDrop::COLUMN, (yyvsp[(3) - (3)].lex_str).str));
            statement->alter_info.flags.set(ALTER_DROP_COLUMN);
          }
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 1904 "drizzled/sql_yacc.yy"
    {
            parser::buildAddAlterDropIndex(&Lex, (yyvsp[(4) - (4)].lex_str).str, true);
          }
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 1908 "drizzled/sql_yacc.yy"
    {
            parser::buildAddAlterDropIndex(&Lex, "PRIMARY");
          }
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 1912 "drizzled/sql_yacc.yy"
    {
            parser::buildAddAlterDropIndex(&Lex, (yyvsp[(3) - (3)].lex_str).str);
          }
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 1916 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= (statement::AlterTable *)Lex.statement;

            statement->alter_info.keys_onoff= DISABLE;
            statement->alter_info.flags.set(ALTER_KEYS_ONOFF);
          }
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 1923 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= (statement::AlterTable *)Lex.statement;

            statement->alter_info.keys_onoff= ENABLE;
            statement->alter_info.flags.set(ALTER_KEYS_ONOFF);
          }
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 1930 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= (statement::AlterTable *)Lex.statement;

            statement->alter_info.alter_list.push_back(AlterColumn((yyvsp[(3) - (6)].lex_str).str,(yyvsp[(6) - (6)].item)));
            statement->alter_info.flags.set(ALTER_COLUMN_DEFAULT);
          }
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 1937 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= (statement::AlterTable *)Lex.statement;

            statement->alter_info.alter_list.push_back(AlterColumn((yyvsp[(3) - (5)].lex_str).str, (Item*) 0));
            statement->alter_info.flags.set(ALTER_COLUMN_DEFAULT);
          }
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 1944 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= (statement::AlterTable *)Lex.statement;
            size_t dummy;

            Lex.select_lex.db=(yyvsp[(3) - (3)].table)->db.str;
            if (Lex.select_lex.db == NULL &&
                Lex.copy_db_to(&Lex.select_lex.db, &dummy))
            {
              DRIZZLE_YYABORT;
            }

            if (check_table_name((yyvsp[(3) - (3)].table)->table.str,(yyvsp[(3) - (3)].table)->table.length))
            {
              my_error(ER_WRONG_TABLE_NAME, MYF(0), (yyvsp[(3) - (3)].table)->table.str);
              DRIZZLE_YYABORT;
            }

            Lex.name= (yyvsp[(3) - (3)].table)->table;
            statement->alter_info.flags.set(ALTER_RENAME);
          }
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 1965 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= (statement::AlterTable *)Lex.statement;

            statement->create_info().table_charset=
            statement->create_info().default_table_charset= (yyvsp[(3) - (3)].charset);
            statement->create_info().used_fields|= (HA_CREATE_USED_CHARSET |
              HA_CREATE_USED_DEFAULT_CHARSET);
            statement->alter_info.flags.set(ALTER_CONVERT);
          }
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 1975 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= (statement::AlterTable *)Lex.statement;

            statement->alter_info.flags.set(ALTER_OPTIONS);
          }
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 1981 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= (statement::AlterTable *)Lex.statement;

            statement->alter_info.flags.set(ALTER_FORCE);
          }
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 1987 "drizzled/sql_yacc.yy"
    {
            statement::AlterTable *statement= (statement::AlterTable *)Lex.statement;

            statement->alter_info.flags.set(ALTER_ORDER);
          }
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 1995 "drizzled/sql_yacc.yy"
    {}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 1996 "drizzled/sql_yacc.yy"
    {}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2000 "drizzled/sql_yacc.yy"
    { Lex.ignore= 0;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2001 "drizzled/sql_yacc.yy"
    { Lex.ignore= 1;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 2005 "drizzled/sql_yacc.yy"
    {}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2007 "drizzled/sql_yacc.yy"
    {
            parser::storeAlterColumnPosition(&Lex, (yyvsp[(2) - (2)].lex_str).str);
          }
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2011 "drizzled/sql_yacc.yy"
    {
            parser::storeAlterColumnPosition(&Lex, first_keyword);
          }
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2017 "drizzled/sql_yacc.yy"
    {}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2018 "drizzled/sql_yacc.yy"
    {}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2019 "drizzled/sql_yacc.yy"
    {}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2024 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::StartTransaction(YYSession, (start_transaction_option_t)(yyvsp[(3) - (3)].num));
          }
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2030 "drizzled/sql_yacc.yy"
    { (yyval.num) = START_TRANS_NO_OPTIONS; }
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2032 "drizzled/sql_yacc.yy"
    {
            (yyval.num)= START_TRANS_OPT_WITH_CONS_SNAPSHOT;
          }
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2039 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::Analyze(YYSession);
          }
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2043 "drizzled/sql_yacc.yy"
    {}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2048 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::Check(YYSession);
          }
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2052 "drizzled/sql_yacc.yy"
    {}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2057 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::RenameTable(YYSession);
          }
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2061 "drizzled/sql_yacc.yy"
    {}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2071 "drizzled/sql_yacc.yy"
    {
            Select_Lex *sl= Lex.current_select;
            if (!sl->add_table_to_list(Lex.session, (yyvsp[(1) - (3)].table),NULL,TL_OPTION_UPDATING,
                                       TL_IGNORE) ||
                !sl->add_table_to_list(Lex.session, (yyvsp[(3) - (3)].table),NULL,TL_OPTION_UPDATING,
                                       TL_IGNORE))
              DRIZZLE_YYABORT;
          }
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2088 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::Select(YYSession);
          }
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2101 "drizzled/sql_yacc.yy"
    {
            if (parser::setup_select_in_parentheses(YYSession, &Lex))
              DRIZZLE_YYABORT;
          }
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2111 "drizzled/sql_yacc.yy"
    {
            if (parser::setup_select_in_parentheses(YYSession, &Lex))
              DRIZZLE_YYABORT;
          }
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2120 "drizzled/sql_yacc.yy"
    {
            Select_Lex * sel= Lex.current_select;
            if (Lex.current_select->set_braces(0))
            {
              parser::my_parse_error(YYSession->m_lip);
              DRIZZLE_YYABORT;
            }
            if (sel->linkage == UNION_TYPE &&
                sel->master_unit()->first_select()->braces)
            {
              parser::my_parse_error(YYSession->m_lip);
              DRIZZLE_YYABORT;
            }
          }
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2138 "drizzled/sql_yacc.yy"
    {
            Select_Lex *sel= Lex.current_select;
            if (sel->linkage != UNION_TYPE)
              init_select(&Lex);
            Lex.current_select->parsing_place= SELECT_LIST;
          }
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2145 "drizzled/sql_yacc.yy"
    {
            Lex.current_select->parsing_place= NO_MATTER;
          }
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2152 "drizzled/sql_yacc.yy"
    {}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2162 "drizzled/sql_yacc.yy"
    {
            Lex.current_select->context.table_list=
              Lex.current_select->context.first_name_resolution_table=
                reinterpret_cast<TableList *>(Lex.current_select->table_list.first);
          }
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2172 "drizzled/sql_yacc.yy"
    { }
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2182 "drizzled/sql_yacc.yy"
    {
            Lex.current_select->options|= SELECT_DISTINCT;

            if (Lex.current_select->options & SELECT_DISTINCT && Lex.current_select->options & SELECT_ALL)
            {
              my_error(ER_WRONG_USAGE, MYF(0), "ALL", "DISTINCT");
              DRIZZLE_YYABORT;
            }
          }
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2192 "drizzled/sql_yacc.yy"
    {
            Lex.current_select->options|= SELECT_ALL;

            if (Lex.current_select->options & SELECT_DISTINCT && Lex.current_select->options & SELECT_ALL)
            {
              my_error(ER_WRONG_USAGE, MYF(0), "ALL", "DISTINCT");
              DRIZZLE_YYABORT;
            }
          }
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2205 "drizzled/sql_yacc.yy"
    {
            Lex.current_select->options|= SELECT_SMALL_RESULT;

            if (Lex.current_select->options & SELECT_SMALL_RESULT && Lex.current_select->options & SELECT_BIG_RESULT)
            {
              my_error(ER_WRONG_USAGE, MYF(0), "SELECT_SMALL_RESULT", "SELECT_SMALL_RESULT");
              DRIZZLE_YYABORT;
            }
          }
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2215 "drizzled/sql_yacc.yy"
    {
            Lex.current_select->options|= SELECT_BIG_RESULT;

            if (Lex.current_select->options & SELECT_SMALL_RESULT && Lex.current_select->options & SELECT_BIG_RESULT)
            {
              my_error(ER_WRONG_USAGE, MYF(0), "SELECT_SMALL_RESULT", "SELECT_SMALL_RESULT");
              DRIZZLE_YYABORT;
            }
          }
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2228 "drizzled/sql_yacc.yy"
    { Lex.current_select->options|= SELECT_STRAIGHT_JOIN; }
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2230 "drizzled/sql_yacc.yy"
    {
            if (check_simple_select(YYSession))
              DRIZZLE_YYABORT;
            Lex.current_select->options|= OPTION_BUFFER_RESULT;
          }
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 2236 "drizzled/sql_yacc.yy"
    { }
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2238 "drizzled/sql_yacc.yy"
    { }
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2240 "drizzled/sql_yacc.yy"
    {
            if (check_simple_select(YYSession))
              DRIZZLE_YYABORT;
            Lex.current_select->options|= OPTION_FOUND_ROWS;
          }
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2250 "drizzled/sql_yacc.yy"
    {
            Lex.current_select->set_lock_for_tables(TL_WRITE);
          }
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2254 "drizzled/sql_yacc.yy"
    {
            Lex.current_select->
              set_lock_for_tables(TL_READ_WITH_SHARED_LOCKS);
          }
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2264 "drizzled/sql_yacc.yy"
    {
            if (YYSession->add_item_to_list( new Item_field(&YYSession->lex().current_select->context, NULL, NULL, "*")))
              DRIZZLE_YYABORT;

            (YYSession->lex().current_select->with_wild)++;
          }
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2274 "drizzled/sql_yacc.yy"
    {
            if (YYSession->add_item_to_list((yyvsp[(2) - (3)].item)))
              DRIZZLE_YYABORT;
          }
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2279 "drizzled/sql_yacc.yy"
    {
            assert((yyvsp[(1) - (4)].simple_string) < (yyvsp[(3) - (4)].simple_string));

            if (YYSession->add_item_to_list((yyvsp[(2) - (4)].item)))
              DRIZZLE_YYABORT;

            if ((yyvsp[(4) - (4)].lex_str).str)
            {
              (yyvsp[(2) - (4)].item)->is_autogenerated_name= false;
              (yyvsp[(2) - (4)].item)->set_name((yyvsp[(4) - (4)].lex_str).str, (yyvsp[(4) - (4)].lex_str).length, system_charset_info);
            }
            else if (!(yyvsp[(2) - (4)].item)->name)
            {
              (yyvsp[(2) - (4)].item)->set_name((yyvsp[(1) - (4)].simple_string), (uint) ((yyvsp[(3) - (4)].simple_string) - (yyvsp[(1) - (4)].simple_string)), YYSession->charset());
            }
          }
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2298 "drizzled/sql_yacc.yy"
    {
            Lex_input_stream *lip= YYSession->m_lip;
            (yyval.simple_string)= (char*) lip->get_cpp_tok_start();
          }
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2305 "drizzled/sql_yacc.yy"
    {
            Lex_input_stream *lip= YYSession->m_lip;
            (yyval.simple_string)= (char*) lip->get_cpp_tok_end();
          }
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2312 "drizzled/sql_yacc.yy"
    { (yyval.lex_str)=null_lex_str;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2313 "drizzled/sql_yacc.yy"
    { (yyval.lex_str)=(yyvsp[(2) - (2)].lex_str); }
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 2314 "drizzled/sql_yacc.yy"
    { (yyval.lex_str)=(yyvsp[(2) - (2)].lex_str); }
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2315 "drizzled/sql_yacc.yy"
    { (yyval.lex_str)=(yyvsp[(1) - (1)].lex_str); }
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2316 "drizzled/sql_yacc.yy"
    { (yyval.lex_str)=(yyvsp[(1) - (1)].lex_str); }
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 2320 "drizzled/sql_yacc.yy"
    {}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 2321 "drizzled/sql_yacc.yy"
    {}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 2327 "drizzled/sql_yacc.yy"
    {
            /*
              Design notes:
              Do not use a manually maintained stack like session->Lex.xxx_list,
              but use the internal bison stack ($$, $1 and $3) instead.
              Using the bison stack is:
              - more robust to changes in the grammar,
              - guaranteed to be in sync with the parser state,
              - better for performances (no memory allocation).
            */
            Item_cond_or *item1;
            Item_cond_or *item3;
            if (is_cond_or((yyvsp[(1) - (3)].item)))
            {
              item1= (Item_cond_or*) (yyvsp[(1) - (3)].item);
              if (is_cond_or((yyvsp[(3) - (3)].item)))
              {
                item3= (Item_cond_or*) (yyvsp[(3) - (3)].item);
                /*
                  (X1 OR X2) OR (Y1 OR Y2) ==> OR (X1, X2, Y1, Y2)
                */
                item3->add_at_head(item1->argument_list());
                (yyval.item) = (yyvsp[(3) - (3)].item);
              }
              else
              {
                /*
                  (X1 OR X2) OR Y ==> OR (X1, X2, Y)
                */
                item1->add((yyvsp[(3) - (3)].item));
                (yyval.item) = (yyvsp[(1) - (3)].item);
              }
            }
            else if (is_cond_or((yyvsp[(3) - (3)].item)))
            {
              item3= (Item_cond_or*) (yyvsp[(3) - (3)].item);
              /*
                X OR (Y1 OR Y2) ==> OR (X, Y1, Y2)
              */
              item3->add_at_head((yyvsp[(1) - (3)].item));
              (yyval.item) = (yyvsp[(3) - (3)].item);
            }
            else
            {
              /* X OR Y */
              (yyval.item) = new (YYSession->mem_root) Item_cond_or((yyvsp[(1) - (3)].item), (yyvsp[(3) - (3)].item));
            }
          }
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 2376 "drizzled/sql_yacc.yy"
    {
            /* XOR is a proprietary extension */
            (yyval.item) = new (YYSession->mem_root) Item_cond_xor((yyvsp[(1) - (3)].item), (yyvsp[(3) - (3)].item));
          }
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 2381 "drizzled/sql_yacc.yy"
    {
            /* See comments in rule expr: expr or expr */
            Item_cond_and *item1;
            Item_cond_and *item3;
            if (is_cond_and((yyvsp[(1) - (3)].item)))
            {
              item1= (Item_cond_and*) (yyvsp[(1) - (3)].item);
              if (is_cond_and((yyvsp[(3) - (3)].item)))
              {
                item3= (Item_cond_and*) (yyvsp[(3) - (3)].item);
                /*
                  (X1 AND X2) AND (Y1 AND Y2) ==> AND (X1, X2, Y1, Y2)
                */
                item3->add_at_head(item1->argument_list());
                (yyval.item) = (yyvsp[(3) - (3)].item);
              }
              else
              {
                /*
                  (X1 AND X2) AND Y ==> AND (X1, X2, Y)
                */
                item1->add((yyvsp[(3) - (3)].item));
                (yyval.item) = (yyvsp[(1) - (3)].item);
              }
            }
            else if (is_cond_and((yyvsp[(3) - (3)].item)))
            {
              item3= (Item_cond_and*) (yyvsp[(3) - (3)].item);
              /*
                X AND (Y1 AND Y2) ==> AND (X, Y1, Y2)
              */
              item3->add_at_head((yyvsp[(1) - (3)].item));
              (yyval.item) = (yyvsp[(3) - (3)].item);
            }
            else
            {
              /* X AND Y */
              (yyval.item) = new (YYSession->mem_root) Item_cond_and((yyvsp[(1) - (3)].item), (yyvsp[(3) - (3)].item));
            }
          }
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 2422 "drizzled/sql_yacc.yy"
    { (yyval.item)= negate_expression(YYSession, (yyvsp[(2) - (2)].item)); }
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 2424 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new (YYSession->mem_root) Item_func_istrue((yyvsp[(1) - (3)].item));
          }
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 2428 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new (YYSession->mem_root) Item_func_isnottrue((yyvsp[(1) - (4)].item));
          }
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 2432 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new (YYSession->mem_root) Item_func_isfalse((yyvsp[(1) - (3)].item));
          }
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 2436 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new (YYSession->mem_root) Item_func_isnotfalse((yyvsp[(1) - (4)].item));
          }
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 2440 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new Item_func_isnull((yyvsp[(1) - (3)].item));
          }
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 2444 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new Item_func_isnotnull((yyvsp[(1) - (4)].item));
          }
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 2452 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new Item_func_isnull((yyvsp[(1) - (3)].item));
          }
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 2456 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new Item_func_isnotnull((yyvsp[(1) - (4)].item));
          }
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 2460 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new Item_func_equal((yyvsp[(1) - (3)].item),(yyvsp[(3) - (3)].item));
          }
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 2464 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= (*(yyvsp[(2) - (3)].boolfunc2creator))(0)->create((yyvsp[(1) - (3)].item),(yyvsp[(3) - (3)].item));
          }
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 2468 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= all_any_subquery_creator((yyvsp[(1) - (6)].item), (yyvsp[(2) - (6)].boolfunc2creator), (yyvsp[(3) - (6)].num), (yyvsp[(5) - (6)].select_lex));
          }
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 2476 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new (YYSession->mem_root) Item_in_subselect((yyvsp[(1) - (5)].item), (yyvsp[(4) - (5)].select_lex));
          }
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 2480 "drizzled/sql_yacc.yy"
    {
            Item *item= new (YYSession->mem_root) Item_in_subselect((yyvsp[(1) - (6)].item), (yyvsp[(5) - (6)].select_lex));
            (yyval.item)= negate_expression(YYSession, item);
          }
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 2485 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= parser::handle_sql2003_note184_exception(YYSession, (yyvsp[(1) - (5)].item), true, (yyvsp[(4) - (5)].item));
          }
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 2489 "drizzled/sql_yacc.yy"
    {
            (yyvsp[(6) - (7)].item_list)->push_front((yyvsp[(4) - (7)].item));
            (yyvsp[(6) - (7)].item_list)->push_front((yyvsp[(1) - (7)].item));
            (yyval.item)= new (YYSession->mem_root) Item_func_in(*(yyvsp[(6) - (7)].item_list));
          }
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 2495 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= parser::handle_sql2003_note184_exception(YYSession, (yyvsp[(1) - (6)].item), false, (yyvsp[(5) - (6)].item));
          }
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 2499 "drizzled/sql_yacc.yy"
    {
            (yyvsp[(7) - (8)].item_list)->push_front((yyvsp[(5) - (8)].item));
            (yyvsp[(7) - (8)].item_list)->push_front((yyvsp[(1) - (8)].item));
            Item_func_in *item = new (YYSession->mem_root) Item_func_in(*(yyvsp[(7) - (8)].item_list));
            item->negate();
            (yyval.item)= item;
          }
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 2507 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new Item_func_between((yyvsp[(1) - (5)].item),(yyvsp[(3) - (5)].item),(yyvsp[(5) - (5)].item));
          }
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 2511 "drizzled/sql_yacc.yy"
    {
            Item_func_between *item= new Item_func_between((yyvsp[(1) - (6)].item),(yyvsp[(4) - (6)].item),(yyvsp[(6) - (6)].item));
            item->negate();
            (yyval.item)= item;
          }
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 2517 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new Item_func_like((yyvsp[(1) - (4)].item),(yyvsp[(3) - (4)].item),(yyvsp[(4) - (4)].item),Lex.escape_used);
          }
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 2521 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new Item_func_not(new Item_func_like((yyvsp[(1) - (5)].item),(yyvsp[(4) - (5)].item),(yyvsp[(5) - (5)].item), Lex.escape_used));
          }
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 2525 "drizzled/sql_yacc.yy"
    {
            List<Item> *args= new (YYSession->mem_root) List<Item>;
            args->push_back((yyvsp[(1) - (3)].item));
            args->push_back((yyvsp[(3) - (3)].item));
            if (! ((yyval.item)= parser::reserved_keyword_function(YYSession, "regex", args)))
            {
              DRIZZLE_YYABORT;
            }
          }
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 2535 "drizzled/sql_yacc.yy"
    {
            List<Item> *args= new (YYSession->mem_root) List<Item>;
            args->push_back((yyvsp[(1) - (4)].item));
            args->push_back((yyvsp[(4) - (4)].item));
            args->push_back(new (YYSession->mem_root) Item_int(1));
            if (! ((yyval.item)= parser::reserved_keyword_function(YYSession, "regex", args)))
            {
              DRIZZLE_YYABORT;
            }
          }
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 2550 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new function::bit::Or((yyvsp[(1) - (3)].item), (yyvsp[(3) - (3)].item));
          }
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 2554 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new function::bit::And((yyvsp[(1) - (3)].item), (yyvsp[(3) - (3)].item));
          }
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 2558 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new function::bit::ShiftRight((yyvsp[(1) - (3)].item), (yyvsp[(3) - (3)].item));
          }
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 2562 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new function::bit::ShiftLeft((yyvsp[(1) - (3)].item), (yyvsp[(3) - (3)].item));
          }
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 2566 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new Item_func_plus((yyvsp[(1) - (3)].item),(yyvsp[(3) - (3)].item));
          }
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 2570 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new Item_func_minus((yyvsp[(1) - (3)].item),(yyvsp[(3) - (3)].item));
          }
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 2574 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new Item_date_add_interval((yyvsp[(1) - (5)].item),(yyvsp[(4) - (5)].item),(yyvsp[(5) - (5)].interval),0);
          }
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 2578 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new Item_date_add_interval((yyvsp[(1) - (5)].item),(yyvsp[(4) - (5)].item),(yyvsp[(5) - (5)].interval),1);
          }
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 2582 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new Item_func_mul((yyvsp[(1) - (3)].item),(yyvsp[(3) - (3)].item));
          }
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 2586 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new Item_func_div(YYSession,(yyvsp[(1) - (3)].item),(yyvsp[(3) - (3)].item));
          }
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 2590 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new Item_func_mod((yyvsp[(1) - (3)].item),(yyvsp[(3) - (3)].item));
          }
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 2594 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new Item_func_int_div((yyvsp[(1) - (3)].item),(yyvsp[(3) - (3)].item));
          }
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 2598 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new Item_func_mod((yyvsp[(1) - (3)].item),(yyvsp[(3) - (3)].item));
          }
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 2602 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new (YYSession->mem_root) function::bit::Xor((yyvsp[(1) - (3)].item), (yyvsp[(3) - (3)].item));
          }
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 2609 "drizzled/sql_yacc.yy"
    { (yyval.boolfunc2creator) = &comp_eq_creator; }
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 2610 "drizzled/sql_yacc.yy"
    { (yyval.boolfunc2creator) = &comp_ge_creator; }
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 2611 "drizzled/sql_yacc.yy"
    { (yyval.boolfunc2creator) = &comp_gt_creator; }
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 2612 "drizzled/sql_yacc.yy"
    { (yyval.boolfunc2creator) = &comp_le_creator; }
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 2613 "drizzled/sql_yacc.yy"
    { (yyval.boolfunc2creator) = &comp_lt_creator; }
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 2614 "drizzled/sql_yacc.yy"
    { (yyval.boolfunc2creator) = &comp_ne_creator; }
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 2618 "drizzled/sql_yacc.yy"
    { (yyval.num) = 1; }
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 2619 "drizzled/sql_yacc.yy"
    { (yyval.num) = 0; }
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 2629 "drizzled/sql_yacc.yy"
    {
            Item *i1= new (YYSession->mem_root) Item_string((yyvsp[(3) - (3)].lex_str).str,
                                                      (yyvsp[(3) - (3)].lex_str).length,
                                                      YYSession->charset());
            (yyval.item)= new (YYSession->mem_root) Item_func_set_collation((yyvsp[(1) - (3)].item), i1);
          }
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 2638 "drizzled/sql_yacc.yy"
    {
            Lex.setSumExprUsed();
          }
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 2642 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new (YYSession->mem_root) Item_func_concat(*YYSession, (yyvsp[(1) - (3)].item), (yyvsp[(3) - (3)].item));
          }
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 2645 "drizzled/sql_yacc.yy"
    { (yyval.item)= (yyvsp[(2) - (2)].item); }
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 2647 "drizzled/sql_yacc.yy"
    {
	    (yyval.item)= new (YYSession->mem_root) Item_func_neg((yyvsp[(2) - (2)].item));
	  }
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 2651 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new (YYSession->mem_root) function::bit::Neg((yyvsp[(2) - (2)].item));
          }
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 2655 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new (YYSession->mem_root) Item_singlerow_subselect((yyvsp[(2) - (3)].select_lex));
          }
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 2658 "drizzled/sql_yacc.yy"
    { (yyval.item)= (yyvsp[(2) - (3)].item); }
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 2660 "drizzled/sql_yacc.yy"
    {
            (yyvsp[(4) - (5)].item_list)->push_front((yyvsp[(2) - (5)].item));
            (yyval.item)= new (YYSession->mem_root) Item_row(*(yyvsp[(4) - (5)].item_list));
          }
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 2665 "drizzled/sql_yacc.yy"
    {
            (yyvsp[(5) - (6)].item_list)->push_front((yyvsp[(3) - (6)].item));
            (yyval.item)= new (YYSession->mem_root) Item_row(*(yyvsp[(5) - (6)].item_list));
          }
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 2670 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new (YYSession->mem_root) Item_exists_subselect((yyvsp[(3) - (4)].select_lex));
          }
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 2673 "drizzled/sql_yacc.yy"
    { (yyval.item)= (yyvsp[(3) - (4)].item); }
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 2675 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= create_func_cast(YYSession, (yyvsp[(2) - (2)].item), ITEM_CAST_CHAR, NULL, NULL,
                                 &my_charset_bin);
          }
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 2680 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= create_func_cast(YYSession, (yyvsp[(3) - (6)].item), (yyvsp[(5) - (6)].cast_type), Lex.length, Lex.dec,
                                 Lex.charset);
            if (!(yyval.item))
              DRIZZLE_YYABORT;
          }
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 2687 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_case(* (yyvsp[(3) - (5)].item_list), (yyvsp[(2) - (5)].item), (yyvsp[(4) - (5)].item) ); }
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 2689 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= create_func_cast(YYSession, (yyvsp[(3) - (6)].item), (yyvsp[(5) - (6)].cast_type), Lex.length, Lex.dec,
                                 Lex.charset);
            if (!(yyval.item))
              DRIZZLE_YYABORT;
          }
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 2696 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new (YYSession->mem_root) Item_default_value(Lex.current_context(),
                                                         (yyvsp[(3) - (4)].item));
          }
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 2701 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new (YYSession->mem_root) Item_insert_value(Lex.current_context(),
                                                        (yyvsp[(3) - (4)].item));
          }
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 2707 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_date_add_interval((yyvsp[(5) - (5)].item),(yyvsp[(2) - (5)].item),(yyvsp[(3) - (5)].interval),0); }
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 2718 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_char(*(yyvsp[(3) - (4)].item_list)); }
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 2720 "drizzled/sql_yacc.yy"
    {
            if (! ((yyval.item)= parser::reserved_keyword_function(YYSession, "user", NULL)))
            {
              DRIZZLE_YYABORT;
            }
            Lex.setCacheable(false);
          }
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 2728 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_date_typecast((yyvsp[(3) - (4)].item)); }
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 2730 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_dayofmonth((yyvsp[(3) - (4)].item)); }
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 2732 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_hour((yyvsp[(3) - (4)].item)); }
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 2734 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_insert(*YYSession, (yyvsp[(3) - (10)].item), (yyvsp[(5) - (10)].item), (yyvsp[(7) - (10)].item), (yyvsp[(9) - (10)].item)); }
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 2736 "drizzled/sql_yacc.yy"
    {
            List<Item> *list= new (YYSession->mem_root) List<Item>;
            list->push_front((yyvsp[(5) - (6)].item));
            list->push_front((yyvsp[(3) - (6)].item));
            Item_row *item= new (YYSession->mem_root) Item_row(*list);
            (yyval.item)= new (YYSession->mem_root) Item_func_interval(item);
          }
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 2744 "drizzled/sql_yacc.yy"
    {
            (yyvsp[(7) - (8)].item_list)->push_front((yyvsp[(5) - (8)].item));
            (yyvsp[(7) - (8)].item_list)->push_front((yyvsp[(3) - (8)].item));
            Item_row *item= new (YYSession->mem_root) Item_row(*(yyvsp[(7) - (8)].item_list));
            (yyval.item)= new (YYSession->mem_root) Item_func_interval(item);
          }
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 2751 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_left((yyvsp[(3) - (6)].item),(yyvsp[(5) - (6)].item)); }
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 2753 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_minute((yyvsp[(3) - (4)].item)); }
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 2755 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_month((yyvsp[(3) - (4)].item)); }
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 2757 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_right((yyvsp[(3) - (6)].item),(yyvsp[(5) - (6)].item)); }
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 2759 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_second((yyvsp[(3) - (4)].item)); }
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 2761 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_datetime_typecast((yyvsp[(3) - (4)].item)); }
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 2763 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_trim((yyvsp[(3) - (4)].item)); }
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 2765 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_ltrim((yyvsp[(6) - (7)].item),(yyvsp[(4) - (7)].item)); }
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 2767 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_rtrim((yyvsp[(6) - (7)].item),(yyvsp[(4) - (7)].item)); }
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 2769 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_trim((yyvsp[(6) - (7)].item),(yyvsp[(4) - (7)].item)); }
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 2771 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_ltrim((yyvsp[(5) - (6)].item)); }
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 2773 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_rtrim((yyvsp[(5) - (6)].item)); }
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 2775 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_trim((yyvsp[(5) - (6)].item)); }
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 2777 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_trim((yyvsp[(5) - (6)].item),(yyvsp[(3) - (6)].item)); }
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 2779 "drizzled/sql_yacc.yy"
    {
            if (! ((yyval.item)= parser::reserved_keyword_function(YYSession, "user", NULL)))
            {
              DRIZZLE_YYABORT;
            }
            Lex.setCacheable(false);
          }
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 2787 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_year((yyvsp[(3) - (4)].item)); }
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 2803 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new (YYSession->mem_root) Item_date_add_interval((yyvsp[(3) - (6)].item), (yyvsp[(5) - (6)].item),
                                                             INTERVAL_DAY, 0);
          }
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 2808 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_date_add_interval((yyvsp[(3) - (8)].item), (yyvsp[(6) - (8)].item), (yyvsp[(7) - (8)].interval), 0); }
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 2810 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new (YYSession->mem_root) Item_func_curdate_local();
            Lex.setCacheable(false);
          }
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 2815 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_date_add_interval((yyvsp[(3) - (8)].item),(yyvsp[(6) - (8)].item),(yyvsp[(7) - (8)].interval),0); }
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 2817 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_date_add_interval((yyvsp[(3) - (8)].item),(yyvsp[(6) - (8)].item),(yyvsp[(7) - (8)].interval),1); }
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 2819 "drizzled/sql_yacc.yy"
    { (yyval.item)=new (YYSession->mem_root) Item_extract( (yyvsp[(3) - (6)].interval), (yyvsp[(5) - (6)].item)); }
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 2821 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new (YYSession->mem_root) Item_func_now_local();
            Lex.setCacheable(false);
          }
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 2826 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new (YYSession->mem_root) Item_func_now_local((yyvsp[(3) - (4)].item));
            Lex.setCacheable(false);
          }
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 2831 "drizzled/sql_yacc.yy"
    { (yyval.item) = new (YYSession->mem_root) Item_func_locate((yyvsp[(5) - (6)].item),(yyvsp[(3) - (6)].item)); }
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 2833 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new (YYSession->mem_root) Item_date_add_interval((yyvsp[(3) - (6)].item), (yyvsp[(5) - (6)].item),
                                                             INTERVAL_DAY, 1);
          }
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 2838 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_date_add_interval((yyvsp[(3) - (8)].item), (yyvsp[(6) - (8)].item), (yyvsp[(7) - (8)].interval), 1); }
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 2840 "drizzled/sql_yacc.yy"
    {
            List<Item> *args= new (YYSession->mem_root) List<Item>;
            args->push_back((yyvsp[(3) - (8)].item));
            args->push_back((yyvsp[(5) - (8)].item));
            args->push_back((yyvsp[(7) - (8)].item));
            if (! ((yyval.item)= parser::reserved_keyword_function(YYSession, "substr", args)))
            {
              DRIZZLE_YYABORT;
            }
          }
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 2851 "drizzled/sql_yacc.yy"
    {
            List<Item> *args= new (YYSession->mem_root) List<Item>;
            args->push_back((yyvsp[(3) - (6)].item));
            args->push_back((yyvsp[(5) - (6)].item));
            if (! ((yyval.item)= parser::reserved_keyword_function(YYSession, "substr", args)))
            {
              DRIZZLE_YYABORT;
            }
          }
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 2861 "drizzled/sql_yacc.yy"
    {
            List<Item> *args= new (YYSession->mem_root) List<Item>;
            args->push_back((yyvsp[(3) - (8)].item));
            args->push_back((yyvsp[(5) - (8)].item));
            args->push_back((yyvsp[(7) - (8)].item));
            if (! ((yyval.item)= parser::reserved_keyword_function(YYSession, "substr", args)))
            {
              DRIZZLE_YYABORT;
            }
          }
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 2872 "drizzled/sql_yacc.yy"
    {
            List<Item> *args= new (YYSession->mem_root) List<Item>;
            args->push_back((yyvsp[(3) - (6)].item));
            args->push_back((yyvsp[(5) - (6)].item));
            if (! ((yyval.item)= parser::reserved_keyword_function(YYSession, "substr", args)))
            {
              DRIZZLE_YYABORT;
            }
          }
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 2882 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new (YYSession->mem_root) Item_func_sysdate_local();
            Lex.setCacheable(false);
          }
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 2887 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new (YYSession->mem_root) Item_func_sysdate_local((yyvsp[(3) - (4)].item));
            Lex.setCacheable(false);
          }
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 2892 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_date_add_interval((yyvsp[(7) - (8)].item),(yyvsp[(5) - (8)].item),(yyvsp[(3) - (8)].interval_time_st),0); }
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 2894 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_timestamp_diff((yyvsp[(5) - (8)].item),(yyvsp[(7) - (8)].item),(yyvsp[(3) - (8)].interval_time_st)); }
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 2896 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new (YYSession->mem_root) Item_func_curdate_utc();
            Lex.setCacheable(false);
          }
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 2901 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new (YYSession->mem_root) Item_func_now_utc();
            Lex.setCacheable(false);
          }
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 2914 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_coalesce(* (yyvsp[(3) - (4)].item_list)); }
    break;

  case 493:

/* Line 1455 of yacc.c  */
#line 2916 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_collation((yyvsp[(3) - (4)].item)); }
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 2918 "drizzled/sql_yacc.yy"
    {
            if (! ((yyval.item)= parser::reserved_keyword_function(YYSession, "database", NULL)))
            {
              DRIZZLE_YYABORT;
            }
            Lex.setCacheable(false);
	  }
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 2926 "drizzled/sql_yacc.yy"
    {
            if (! ((yyval.item)= parser::reserved_keyword_function(YYSession, "catalog", NULL)))
            {
              DRIZZLE_YYABORT;
            }
            Lex.setCacheable(false);
	  }
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 2934 "drizzled/sql_yacc.yy"
    {
            List<Item> *args= new (YYSession->mem_root) List<Item>;
            args->push_back((yyvsp[(3) - (5)].item));

            if ((yyvsp[(5) - (5)].boolean))
            {
              args->push_back(new (YYSession->mem_root) Item_int(1));
            }

            if (! ((yyval.item)= parser::reserved_keyword_function(YYSession, "execute", args)))
            {
              DRIZZLE_YYABORT;
            }
          }
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 2949 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_if((yyvsp[(3) - (8)].item),(yyvsp[(5) - (8)].item),(yyvsp[(7) - (8)].item)); }
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 2951 "drizzled/sql_yacc.yy"
    {
            List<Item> *args= new (YYSession->mem_root) List<Item>;
            args->push_back((yyvsp[(4) - (5)].item));

            if ((yyvsp[(2) - (5)].num))
            {
              args->push_back(new (YYSession->mem_root) Item_uint(1));
            }

            if (! ((yyval.item)= parser::reserved_keyword_function(YYSession, "kill", args)))
            {
              DRIZZLE_YYABORT;
            }
          }
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 2966 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_microsecond((yyvsp[(3) - (4)].item)); }
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 2968 "drizzled/sql_yacc.yy"
    { (yyval.item) = new (YYSession->mem_root) Item_func_mod( (yyvsp[(3) - (6)].item), (yyvsp[(5) - (6)].item)); }
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 2970 "drizzled/sql_yacc.yy"
    { (yyval.item) = new (YYSession->mem_root) Item_func_quarter((yyvsp[(3) - (4)].item)); }
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 2972 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_repeat(*YYSession, (yyvsp[(3) - (6)].item), (yyvsp[(5) - (6)].item)); }
    break;

  case 503:

/* Line 1455 of yacc.c  */
#line 2974 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_replace(*YYSession, (yyvsp[(3) - (8)].item), (yyvsp[(5) - (8)].item), (yyvsp[(7) - (8)].item)); }
    break;

  case 504:

/* Line 1455 of yacc.c  */
#line 2976 "drizzled/sql_yacc.yy"
    { (yyval.item)= new (YYSession->mem_root) Item_func_round((yyvsp[(3) - (6)].item),(yyvsp[(5) - (6)].item),1); }
    break;

  case 505:

/* Line 1455 of yacc.c  */
#line 2978 "drizzled/sql_yacc.yy"
    {
            List<Item> *args= new (YYSession->mem_root) List<Item>;
            args->push_back((yyvsp[(3) - (4)].item));
            if (! ((yyval.item)= parser::reserved_keyword_function(YYSession, "wait", args)))
            {
              DRIZZLE_YYABORT;
            }
          }
    break;

  case 506:

/* Line 1455 of yacc.c  */
#line 2987 "drizzled/sql_yacc.yy"
    {
            if (! ((yyval.item)= parser::reserved_keyword_function(YYSession, "uuid", NULL)))
            {
              DRIZZLE_YYABORT;
            }
            Lex.setCacheable(false);
	  }
    break;

  case 507:

/* Line 1455 of yacc.c  */
#line 2995 "drizzled/sql_yacc.yy"
    {
            List<Item> *args= new (YYSession->mem_root) List<Item>;
            args->push_back((yyvsp[(3) - (6)].item));
            args->push_back((yyvsp[(5) - (6)].item));
            if (! ((yyval.item)= parser::reserved_keyword_function(YYSession, "wait", args)))
            {
              DRIZZLE_YYABORT;
            }
          }
    break;

  case 508:

/* Line 1455 of yacc.c  */
#line 3016 "drizzled/sql_yacc.yy"
    {
            const plugin::Function *udf= plugin::Function::get(std::string((yyvsp[(1) - (2)].lex_str).str, (yyvsp[(1) - (2)].lex_str).length));

            /* Temporary placing the result of getFunction in $3 */
            (yyval.udf)= udf;
          }
    break;

  case 509:

/* Line 1455 of yacc.c  */
#line 3023 "drizzled/sql_yacc.yy"
    {
            Create_func *builder;
            Item *item= NULL;

            /*
              Implementation note:
              names are resolved with the following order:
              - MySQL native functions,
              - User Defined Functions,
              - Stored Functions (assuming the current <use> database)

              This will be revised with WL#2128 (SQL PATH)
            */
            builder= find_native_function_builder((yyvsp[(1) - (5)].lex_str));
            if (builder)
            {
              item= builder->create(YYSession, (yyvsp[(1) - (5)].lex_str), (yyvsp[(4) - (5)].item_list));
            }
            else
            {
              /* Retrieving the result of service::Function::get */
              const plugin::Function *udf= (yyvsp[(3) - (5)].udf);
              if (udf)
              {
                item= Create_udf_func::s_singleton.create(YYSession, udf, (yyvsp[(4) - (5)].item_list));
              } else {
                /* fix for bug 250065, from Andrew Garner <muzazzi@gmail.com> */
                my_error(ER_SP_DOES_NOT_EXIST, MYF(0), "FUNCTION", (yyvsp[(1) - (5)].lex_str).str);
              }
            }

            if (! ((yyval.item)= item))
            {
              DRIZZLE_YYABORT;
            }
            Lex.setCacheable(false);
          }
    break;

  case 510:

/* Line 1455 of yacc.c  */
#line 3063 "drizzled/sql_yacc.yy"
    { (yyval.item_list)= NULL; }
    break;

  case 511:

/* Line 1455 of yacc.c  */
#line 3064 "drizzled/sql_yacc.yy"
    { (yyval.item_list)= (yyvsp[(1) - (1)].item_list); }
    break;

  case 512:

/* Line 1455 of yacc.c  */
#line 3069 "drizzled/sql_yacc.yy"
    {
            (yyval.item_list)= new (YYSession->mem_root) List<Item>;
            (yyval.item_list)->push_back((yyvsp[(1) - (1)].item));
          }
    break;

  case 513:

/* Line 1455 of yacc.c  */
#line 3074 "drizzled/sql_yacc.yy"
    {
            (yyvsp[(1) - (3)].item_list)->push_back((yyvsp[(3) - (3)].item));
            (yyval.item_list)= (yyvsp[(1) - (3)].item_list);
          }
    break;

  case 514:

/* Line 1455 of yacc.c  */
#line 3082 "drizzled/sql_yacc.yy"
    {
            /*
             Use Item::name as a storage for the attribute value of user
             defined function argument. It is safe to use Item::name
             because the syntax will not allow having an explicit name here.
             See WL#1017 re. udf attributes.
            */
            if ((yyvsp[(4) - (4)].lex_str).str)
            {
              (yyvsp[(2) - (4)].item)->is_autogenerated_name= false;
              (yyvsp[(2) - (4)].item)->set_name((yyvsp[(4) - (4)].lex_str).str, (yyvsp[(4) - (4)].lex_str).length, system_charset_info);
            }
            else
              (yyvsp[(2) - (4)].item)->set_name((yyvsp[(1) - (4)].simple_string), (uint) ((yyvsp[(3) - (4)].simple_string) - (yyvsp[(1) - (4)].simple_string)), YYSession->charset());
            (yyval.item)= (yyvsp[(2) - (4)].item);
          }
    break;

  case 515:

/* Line 1455 of yacc.c  */
#line 3102 "drizzled/sql_yacc.yy"
    { (yyval.item)=new Item_sum_avg((yyvsp[(3) - (4)].item)); }
    break;

  case 516:

/* Line 1455 of yacc.c  */
#line 3104 "drizzled/sql_yacc.yy"
    { (yyval.item)=new Item_sum_avg_distinct((yyvsp[(4) - (5)].item)); }
    break;

  case 517:

/* Line 1455 of yacc.c  */
#line 3106 "drizzled/sql_yacc.yy"
    { (yyval.item)=new Item_sum_count(new Item_int((int32_t) 0L,1)); }
    break;

  case 518:

/* Line 1455 of yacc.c  */
#line 3108 "drizzled/sql_yacc.yy"
    { (yyval.item)=new Item_sum_count((yyvsp[(3) - (4)].item)); }
    break;

  case 519:

/* Line 1455 of yacc.c  */
#line 3110 "drizzled/sql_yacc.yy"
    { Lex.current_select->in_sum_expr++; }
    break;

  case 520:

/* Line 1455 of yacc.c  */
#line 3112 "drizzled/sql_yacc.yy"
    { Lex.current_select->in_sum_expr--; }
    break;

  case 521:

/* Line 1455 of yacc.c  */
#line 3114 "drizzled/sql_yacc.yy"
    { (yyval.item)=new Item_sum_count_distinct(* (yyvsp[(5) - (7)].item_list)); }
    break;

  case 522:

/* Line 1455 of yacc.c  */
#line 3116 "drizzled/sql_yacc.yy"
    { (yyval.item)=new Item_sum_min((yyvsp[(3) - (4)].item)); }
    break;

  case 523:

/* Line 1455 of yacc.c  */
#line 3123 "drizzled/sql_yacc.yy"
    { (yyval.item)=new Item_sum_min((yyvsp[(4) - (5)].item)); }
    break;

  case 524:

/* Line 1455 of yacc.c  */
#line 3125 "drizzled/sql_yacc.yy"
    { (yyval.item)=new Item_sum_max((yyvsp[(3) - (4)].item)); }
    break;

  case 525:

/* Line 1455 of yacc.c  */
#line 3127 "drizzled/sql_yacc.yy"
    { (yyval.item)=new Item_sum_max((yyvsp[(4) - (5)].item)); }
    break;

  case 526:

/* Line 1455 of yacc.c  */
#line 3129 "drizzled/sql_yacc.yy"
    { (yyval.item)=new Item_sum_std((yyvsp[(3) - (4)].item), 0); }
    break;

  case 527:

/* Line 1455 of yacc.c  */
#line 3131 "drizzled/sql_yacc.yy"
    { (yyval.item)=new Item_sum_variance((yyvsp[(3) - (4)].item), 0); }
    break;

  case 528:

/* Line 1455 of yacc.c  */
#line 3133 "drizzled/sql_yacc.yy"
    { (yyval.item)=new Item_sum_std((yyvsp[(3) - (4)].item), 1); }
    break;

  case 529:

/* Line 1455 of yacc.c  */
#line 3135 "drizzled/sql_yacc.yy"
    { (yyval.item)=new Item_sum_variance((yyvsp[(3) - (4)].item), 1); }
    break;

  case 530:

/* Line 1455 of yacc.c  */
#line 3137 "drizzled/sql_yacc.yy"
    { (yyval.item)=new Item_sum_sum((yyvsp[(3) - (4)].item)); }
    break;

  case 531:

/* Line 1455 of yacc.c  */
#line 3139 "drizzled/sql_yacc.yy"
    { (yyval.item)=new Item_sum_sum_distinct((yyvsp[(4) - (5)].item)); }
    break;

  case 532:

/* Line 1455 of yacc.c  */
#line 3141 "drizzled/sql_yacc.yy"
    { Lex.current_select->in_sum_expr++; }
    break;

  case 533:

/* Line 1455 of yacc.c  */
#line 3145 "drizzled/sql_yacc.yy"
    {
            Select_Lex *sel= Lex.current_select;
            sel->in_sum_expr--;
            (yyval.item)=new Item_func_group_concat(Lex.current_context(), (yyvsp[(3) - (8)].num), (yyvsp[(5) - (8)].item_list),
                                          sel->gorder_list, (yyvsp[(7) - (8)].string));
            (yyvsp[(5) - (8)].item_list)->clear();
          }
    break;

  case 534:

/* Line 1455 of yacc.c  */
#line 3156 "drizzled/sql_yacc.yy"
    { }
    break;

  case 535:

/* Line 1455 of yacc.c  */
#line 3158 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= (yyvsp[(3) - (3)].item);
          }
    break;

  case 536:

/* Line 1455 of yacc.c  */
#line 3165 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new Item_func_set_user_var((yyvsp[(1) - (3)].lex_str), (yyvsp[(3) - (3)].item));
            Lex.setCacheable(false);
          }
    break;

  case 537:

/* Line 1455 of yacc.c  */
#line 3170 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new Item_func_get_user_var(*YYSession, (yyvsp[(1) - (1)].lex_str));
            Lex.setCacheable(false);
          }
    break;

  case 538:

/* Line 1455 of yacc.c  */
#line 3175 "drizzled/sql_yacc.yy"
    {
            /* disallow "SELECT @@global.global.variable" */
            if ((yyvsp[(3) - (4)].lex_str).str && (yyvsp[(4) - (4)].lex_str).str && parser::check_reserved_words(&(yyvsp[(3) - (4)].lex_str)))
            {
              parser::my_parse_error(YYSession->m_lip);
              DRIZZLE_YYABORT;
            }
            if (!((yyval.item)= get_system_var(YYSession, (yyvsp[(2) - (4)].var_type), (yyvsp[(3) - (4)].lex_str), (yyvsp[(4) - (4)].lex_str))))
              DRIZZLE_YYABORT;
          }
    break;

  case 539:

/* Line 1455 of yacc.c  */
#line 3188 "drizzled/sql_yacc.yy"
    { (yyval.num) = false; }
    break;

  case 540:

/* Line 1455 of yacc.c  */
#line 3189 "drizzled/sql_yacc.yy"
    { (yyval.num) = true; }
    break;

  case 541:

/* Line 1455 of yacc.c  */
#line 3194 "drizzled/sql_yacc.yy"
    {
              (yyval.string)= new (YYSession->mem_root) String(",", 1, &my_charset_utf8_general_ci);
            }
    break;

  case 542:

/* Line 1455 of yacc.c  */
#line 3197 "drizzled/sql_yacc.yy"
    { (yyval.string) = (yyvsp[(2) - (2)].string); }
    break;

  case 543:

/* Line 1455 of yacc.c  */
#line 3202 "drizzled/sql_yacc.yy"
    {
            Lex.current_select->gorder_list = NULL;
          }
    break;

  case 544:

/* Line 1455 of yacc.c  */
#line 3206 "drizzled/sql_yacc.yy"
    {
            Select_Lex *select= Lex.current_select;
            select->gorder_list=
              (SQL_LIST*) memory::sql_memdup((char*) &select->order_list,
                                     sizeof(st_sql_list));
            select->order_list.clear();
          }
    break;

  case 545:

/* Line 1455 of yacc.c  */
#line 3217 "drizzled/sql_yacc.yy"
    {
            if (Lex.current_select->inc_in_sum_expr())
            {
              parser::my_parse_error(YYSession->m_lip);
              DRIZZLE_YYABORT;
            }
          }
    break;

  case 546:

/* Line 1455 of yacc.c  */
#line 3225 "drizzled/sql_yacc.yy"
    {
            Lex.current_select->in_sum_expr--;
            (yyval.item)= (yyvsp[(3) - (3)].item);
          }
    break;

  case 547:

/* Line 1455 of yacc.c  */
#line 3233 "drizzled/sql_yacc.yy"
    { (yyval.cast_type)=ITEM_CAST_CHAR; Lex.charset= &my_charset_bin; Lex.dec= 0; }
    break;

  case 548:

/* Line 1455 of yacc.c  */
#line 3235 "drizzled/sql_yacc.yy"
    { (yyval.cast_type)=ITEM_CAST_BOOLEAN; Lex.charset= &my_charset_bin; Lex.dec= 0; }
    break;

  case 549:

/* Line 1455 of yacc.c  */
#line 3237 "drizzled/sql_yacc.yy"
    { (yyval.cast_type)=ITEM_CAST_SIGNED; Lex.charset= NULL; Lex.dec=Lex.length= (char*)0; }
    break;

  case 550:

/* Line 1455 of yacc.c  */
#line 3239 "drizzled/sql_yacc.yy"
    { (yyval.cast_type)=ITEM_CAST_SIGNED; Lex.charset= NULL; Lex.dec=Lex.length= (char*)0; }
    break;

  case 551:

/* Line 1455 of yacc.c  */
#line 3241 "drizzled/sql_yacc.yy"
    { (yyval.cast_type)=ITEM_CAST_SIGNED; Lex.charset= NULL; Lex.dec=Lex.length= (char*)0; }
    break;

  case 552:

/* Line 1455 of yacc.c  */
#line 3243 "drizzled/sql_yacc.yy"
    { (yyval.cast_type)=ITEM_CAST_UNSIGNED; Lex.charset= NULL; Lex.dec=Lex.length= (char*)0; }
    break;

  case 553:

/* Line 1455 of yacc.c  */
#line 3245 "drizzled/sql_yacc.yy"
    { (yyval.cast_type)=ITEM_CAST_UNSIGNED; Lex.charset= NULL; Lex.dec=Lex.length= (char*)0; }
    break;

  case 554:

/* Line 1455 of yacc.c  */
#line 3247 "drizzled/sql_yacc.yy"
    { (yyval.cast_type)=ITEM_CAST_CHAR; Lex.dec= 0; }
    break;

  case 555:

/* Line 1455 of yacc.c  */
#line 3249 "drizzled/sql_yacc.yy"
    { (yyval.cast_type)=ITEM_CAST_DATE; Lex.charset= NULL; Lex.dec=Lex.length= (char*)0; }
    break;

  case 556:

/* Line 1455 of yacc.c  */
#line 3251 "drizzled/sql_yacc.yy"
    { (yyval.cast_type)=ITEM_CAST_TIME; Lex.charset= NULL; Lex.dec=Lex.length= (char*)0; }
    break;

  case 557:

/* Line 1455 of yacc.c  */
#line 3253 "drizzled/sql_yacc.yy"
    { (yyval.cast_type)=ITEM_CAST_DATETIME; Lex.charset= NULL; Lex.dec=Lex.length= (char*)0; }
    break;

  case 558:

/* Line 1455 of yacc.c  */
#line 3255 "drizzled/sql_yacc.yy"
    { (yyval.cast_type)=ITEM_CAST_DECIMAL; Lex.charset= NULL; }
    break;

  case 559:

/* Line 1455 of yacc.c  */
#line 3260 "drizzled/sql_yacc.yy"
    {
            (yyval.item_list)= new (YYSession->mem_root) List<Item>;
            (yyval.item_list)->push_back((yyvsp[(1) - (1)].item));
          }
    break;

  case 560:

/* Line 1455 of yacc.c  */
#line 3265 "drizzled/sql_yacc.yy"
    {
            (yyvsp[(1) - (3)].item_list)->push_back((yyvsp[(3) - (3)].item));
            (yyval.item_list)= (yyvsp[(1) - (3)].item_list);
          }
    break;

  case 561:

/* Line 1455 of yacc.c  */
#line 3272 "drizzled/sql_yacc.yy"
    { (yyval.item)= NULL; }
    break;

  case 562:

/* Line 1455 of yacc.c  */
#line 3273 "drizzled/sql_yacc.yy"
    { (yyval.item)= (yyvsp[(1) - (1)].item); }
    break;

  case 563:

/* Line 1455 of yacc.c  */
#line 3277 "drizzled/sql_yacc.yy"
    { (yyval.item)= NULL; }
    break;

  case 564:

/* Line 1455 of yacc.c  */
#line 3278 "drizzled/sql_yacc.yy"
    { (yyval.item)= (yyvsp[(2) - (2)].item); }
    break;

  case 565:

/* Line 1455 of yacc.c  */
#line 3283 "drizzled/sql_yacc.yy"
    {
            (yyval.item_list)= new List<Item>;
            (yyval.item_list)->push_back((yyvsp[(2) - (4)].item));
            (yyval.item_list)->push_back((yyvsp[(4) - (4)].item));
          }
    break;

  case 566:

/* Line 1455 of yacc.c  */
#line 3289 "drizzled/sql_yacc.yy"
    {
            (yyvsp[(1) - (5)].item_list)->push_back((yyvsp[(3) - (5)].item));
            (yyvsp[(1) - (5)].item_list)->push_back((yyvsp[(5) - (5)].item));
            (yyval.item_list)= (yyvsp[(1) - (5)].item_list);
          }
    break;

  case 567:

/* Line 1455 of yacc.c  */
#line 3299 "drizzled/sql_yacc.yy"
    { (yyval.table_list)=(yyvsp[(1) - (1)].table_list); }
    break;

  case 568:

/* Line 1455 of yacc.c  */
#line 3301 "drizzled/sql_yacc.yy"
    {
            if (!((yyval.table_list)= Lex.current_select->nest_last_join(Lex.session)))
              DRIZZLE_YYABORT;
          }
    break;

  case 569:

/* Line 1455 of yacc.c  */
#line 3308 "drizzled/sql_yacc.yy"
    { DRIZZLE_YYABORT_UNLESS((yyval.table_list)=(yyvsp[(1) - (1)].table_list)); }
    break;

  case 570:

/* Line 1455 of yacc.c  */
#line 3319 "drizzled/sql_yacc.yy"
    { (yyval.table_list)=(yyvsp[(1) - (1)].table_list); }
    break;

  case 571:

/* Line 1455 of yacc.c  */
#line 3320 "drizzled/sql_yacc.yy"
    { (yyval.table_list)=(yyvsp[(3) - (4)].table_list); }
    break;

  case 572:

/* Line 1455 of yacc.c  */
#line 3326 "drizzled/sql_yacc.yy"
    { (yyval.table_list)=(yyvsp[(1) - (1)].table_list); }
    break;

  case 573:

/* Line 1455 of yacc.c  */
#line 3328 "drizzled/sql_yacc.yy"
    {
            DRIZZLE_YYABORT_UNLESS((yyvsp[(1) - (3)].table_list) && ((yyval.table_list)=(yyvsp[(3) - (3)].table_list)));
          }
    break;

  case 574:

/* Line 1455 of yacc.c  */
#line 3348 "drizzled/sql_yacc.yy"
    {
            DRIZZLE_YYABORT_UNLESS((yyvsp[(1) - (3)].table_list) && ((yyval.table_list)=(yyvsp[(3) - (3)].table_list)));
            Lex.is_cross= false;
          }
    break;

  case 575:

/* Line 1455 of yacc.c  */
#line 3353 "drizzled/sql_yacc.yy"
    {
            DRIZZLE_YYABORT_UNLESS((yyvsp[(1) - (3)].table_list) && ((yyval.table_list)=(yyvsp[(3) - (3)].table_list))); (yyvsp[(3) - (3)].table_list)->straight=1;
          }
    break;

  case 576:

/* Line 1455 of yacc.c  */
#line 3358 "drizzled/sql_yacc.yy"
    {
            DRIZZLE_YYABORT_UNLESS((yyvsp[(1) - (4)].table_list) && (yyvsp[(3) - (4)].table_list));
            DRIZZLE_YYABORT_UNLESS( not Lex.is_cross );
            /* Change the current name resolution context to a local context. */
            if (push_new_name_resolution_context(YYSession, (yyvsp[(1) - (4)].table_list), (yyvsp[(3) - (4)].table_list)))
              DRIZZLE_YYABORT;
            Lex.current_select->parsing_place= IN_ON;
          }
    break;

  case 577:

/* Line 1455 of yacc.c  */
#line 3367 "drizzled/sql_yacc.yy"
    {
            add_join_on((yyvsp[(3) - (6)].table_list),(yyvsp[(6) - (6)].item));
            Lex.pop_context();
            Lex.current_select->parsing_place= NO_MATTER;
          }
    break;

  case 578:

/* Line 1455 of yacc.c  */
#line 3374 "drizzled/sql_yacc.yy"
    {
            DRIZZLE_YYABORT_UNLESS((yyvsp[(1) - (4)].table_list) && (yyvsp[(3) - (4)].table_list));
            /* Change the current name resolution context to a local context. */
            if (push_new_name_resolution_context(YYSession, (yyvsp[(1) - (4)].table_list), (yyvsp[(3) - (4)].table_list)))
              DRIZZLE_YYABORT;
            Lex.current_select->parsing_place= IN_ON;
          }
    break;

  case 579:

/* Line 1455 of yacc.c  */
#line 3382 "drizzled/sql_yacc.yy"
    {
            (yyvsp[(3) - (6)].table_list)->straight=1;
            add_join_on((yyvsp[(3) - (6)].table_list),(yyvsp[(6) - (6)].item));
            Lex.pop_context();
            Lex.current_select->parsing_place= NO_MATTER;
          }
    break;

  case 580:

/* Line 1455 of yacc.c  */
#line 3390 "drizzled/sql_yacc.yy"
    {
            DRIZZLE_YYABORT_UNLESS((yyvsp[(1) - (4)].table_list) && (yyvsp[(3) - (4)].table_list));
          }
    break;

  case 581:

/* Line 1455 of yacc.c  */
#line 3394 "drizzled/sql_yacc.yy"
    { add_join_natural((yyvsp[(1) - (8)].table_list),(yyvsp[(3) - (8)].table_list),(yyvsp[(7) - (8)].string_list),Lex.current_select); (yyval.table_list)=(yyvsp[(3) - (8)].table_list); }
    break;

  case 582:

/* Line 1455 of yacc.c  */
#line 3396 "drizzled/sql_yacc.yy"
    {
            DRIZZLE_YYABORT_UNLESS((yyvsp[(1) - (4)].table_list) && ((yyval.table_list)=(yyvsp[(4) - (4)].table_list)));
            add_join_natural((yyvsp[(1) - (4)].table_list),(yyvsp[(4) - (4)].table_list),NULL,Lex.current_select);
          }
    break;

  case 583:

/* Line 1455 of yacc.c  */
#line 3404 "drizzled/sql_yacc.yy"
    {
            DRIZZLE_YYABORT_UNLESS((yyvsp[(1) - (6)].table_list) && (yyvsp[(5) - (6)].table_list));
            /* Change the current name resolution context to a local context. */
            if (push_new_name_resolution_context(YYSession, (yyvsp[(1) - (6)].table_list), (yyvsp[(5) - (6)].table_list)))
              DRIZZLE_YYABORT;
            Lex.current_select->parsing_place= IN_ON;
          }
    break;

  case 584:

/* Line 1455 of yacc.c  */
#line 3412 "drizzled/sql_yacc.yy"
    {
            add_join_on((yyvsp[(5) - (8)].table_list),(yyvsp[(8) - (8)].item));
            Lex.pop_context();
            (yyvsp[(5) - (8)].table_list)->outer_join|=JOIN_TYPE_LEFT;
            (yyval.table_list)=(yyvsp[(5) - (8)].table_list);
            Lex.current_select->parsing_place= NO_MATTER;
          }
    break;

  case 585:

/* Line 1455 of yacc.c  */
#line 3420 "drizzled/sql_yacc.yy"
    {
            DRIZZLE_YYABORT_UNLESS((yyvsp[(1) - (5)].table_list) && (yyvsp[(5) - (5)].table_list));
          }
    break;

  case 586:

/* Line 1455 of yacc.c  */
#line 3424 "drizzled/sql_yacc.yy"
    {
            add_join_natural((yyvsp[(1) - (10)].table_list),(yyvsp[(5) - (10)].table_list),(yyvsp[(9) - (10)].string_list),Lex.current_select);
            (yyvsp[(5) - (10)].table_list)->outer_join|=JOIN_TYPE_LEFT;
            (yyval.table_list)=(yyvsp[(5) - (10)].table_list);
          }
    break;

  case 587:

/* Line 1455 of yacc.c  */
#line 3430 "drizzled/sql_yacc.yy"
    {
            DRIZZLE_YYABORT_UNLESS((yyvsp[(1) - (6)].table_list) && (yyvsp[(6) - (6)].table_list));
            add_join_natural((yyvsp[(1) - (6)].table_list),(yyvsp[(6) - (6)].table_list),NULL,Lex.current_select);
            (yyvsp[(6) - (6)].table_list)->outer_join|=JOIN_TYPE_LEFT;
            (yyval.table_list)=(yyvsp[(6) - (6)].table_list);
          }
    break;

  case 588:

/* Line 1455 of yacc.c  */
#line 3440 "drizzled/sql_yacc.yy"
    {
            DRIZZLE_YYABORT_UNLESS((yyvsp[(1) - (6)].table_list) && (yyvsp[(5) - (6)].table_list));
            /* Change the current name resolution context to a local context. */
            if (push_new_name_resolution_context(YYSession, (yyvsp[(1) - (6)].table_list), (yyvsp[(5) - (6)].table_list)))
              DRIZZLE_YYABORT;
            Lex.current_select->parsing_place= IN_ON;
          }
    break;

  case 589:

/* Line 1455 of yacc.c  */
#line 3448 "drizzled/sql_yacc.yy"
    {
            if (!((yyval.table_list)= Lex.current_select->convert_right_join()))
              DRIZZLE_YYABORT;
            add_join_on((yyval.table_list), (yyvsp[(8) - (8)].item));
            Lex.pop_context();
            Lex.current_select->parsing_place= NO_MATTER;
          }
    break;

  case 590:

/* Line 1455 of yacc.c  */
#line 3456 "drizzled/sql_yacc.yy"
    {
            DRIZZLE_YYABORT_UNLESS((yyvsp[(1) - (5)].table_list) && (yyvsp[(5) - (5)].table_list));
          }
    break;

  case 591:

/* Line 1455 of yacc.c  */
#line 3460 "drizzled/sql_yacc.yy"
    {
            if (!((yyval.table_list)= Lex.current_select->convert_right_join()))
              DRIZZLE_YYABORT;
            add_join_natural((yyval.table_list),(yyvsp[(5) - (10)].table_list),(yyvsp[(9) - (10)].string_list),Lex.current_select);
          }
    break;

  case 592:

/* Line 1455 of yacc.c  */
#line 3466 "drizzled/sql_yacc.yy"
    {
            DRIZZLE_YYABORT_UNLESS((yyvsp[(1) - (6)].table_list) && (yyvsp[(6) - (6)].table_list));
            add_join_natural((yyvsp[(6) - (6)].table_list),(yyvsp[(1) - (6)].table_list),NULL,Lex.current_select);
            if (!((yyval.table_list)= Lex.current_select->convert_right_join()))
              DRIZZLE_YYABORT;
          }
    break;

  case 593:

/* Line 1455 of yacc.c  */
#line 3475 "drizzled/sql_yacc.yy"
    {}
    break;

  case 594:

/* Line 1455 of yacc.c  */
#line 3476 "drizzled/sql_yacc.yy"
    {}
    break;

  case 595:

/* Line 1455 of yacc.c  */
#line 3478 "drizzled/sql_yacc.yy"
    {
            Lex.is_cross= true;
            Lex.current_select->is_cross= true;
          }
    break;

  case 596:

/* Line 1455 of yacc.c  */
#line 3493 "drizzled/sql_yacc.yy"
    {
          }
    break;

  case 597:

/* Line 1455 of yacc.c  */
#line 3496 "drizzled/sql_yacc.yy"
    {
            if (!((yyval.table_list)= Lex.current_select->add_table_to_list(YYSession, (yyvsp[(2) - (4)].table), (yyvsp[(3) - (4)].lex_str_ptr),
                             0,
                             Lex.lock_option,
                             Lex.current_select->pop_index_hints())))
              DRIZZLE_YYABORT;
            Lex.current_select->add_joined_table((yyval.table_list));
          }
    break;

  case 598:

/* Line 1455 of yacc.c  */
#line 3505 "drizzled/sql_yacc.yy"
    {
            Select_Lex *sel= Lex.current_select;
            if ((yyvsp[(1) - (3)].num))
            {
              if (sel->set_braces(1))
              {
                parser::my_parse_error(YYSession->m_lip);
                DRIZZLE_YYABORT;
              }
              /* select in braces, can't contain global parameters */
              if (sel->master_unit()->fake_select_lex)
                sel->master_unit()->global_parameters=
                   sel->master_unit()->fake_select_lex;
            }
            if ((yyvsp[(2) - (3)].select_lex)->init_nested_join(Lex.session))
              DRIZZLE_YYABORT;
            (yyval.table_list)= 0;
            /* incomplete derived tables return NULL, we must be
               nested in select_derived rule to be here. */
          }
    break;

  case 599:

/* Line 1455 of yacc.c  */
#line 3544 "drizzled/sql_yacc.yy"
    {
            /* Use $2 instead of Lex.current_select as derived table will
               alter value of Lex.current_select. */
            if (!((yyvsp[(3) - (5)].table_list) || (yyvsp[(5) - (5)].lex_str_ptr)) && (yyvsp[(2) - (5)].select_lex)->embedding &&
                !(yyvsp[(2) - (5)].select_lex)->embedding->getNestedJoin()->join_list.size())
            {
              /* we have a derived table ($3 == NULL) but no alias,
                 Since we are nested in further parentheses so we
                 can pass NULL to the outer level parentheses
                 Permits parsing of "((((select ...))) as xyz)" */
              (yyval.table_list)= 0;
            }
            else if (!(yyvsp[(3) - (5)].table_list))
            {
              /* Handle case of derived table, alias may be NULL if there
                 are no outer parentheses, add_table_to_list() will throw
                 error in this case */
              Select_Lex *sel= Lex.current_select;
              Select_Lex_Unit *unit= sel->master_unit();
              Lex.current_select= sel= unit->outer_select();
              if (!((yyval.table_list)= sel->add_table_to_list(Lex.session,
                                               new Table_ident(unit), (yyvsp[(5) - (5)].lex_str_ptr), 0,
                                               TL_READ)))

                DRIZZLE_YYABORT;
              sel->add_joined_table((yyval.table_list));
              Lex.pop_context();
            }
            else if (((yyvsp[(3) - (5)].table_list)->select_lex && (yyvsp[(3) - (5)].table_list)->select_lex->master_unit()->is_union()) || (yyvsp[(5) - (5)].lex_str_ptr))
            {
              /* simple nested joins cannot have aliases or unions */
              parser::my_parse_error(YYSession->m_lip);
              DRIZZLE_YYABORT;
            }
            else
              (yyval.table_list)= (yyvsp[(3) - (5)].table_list);
          }
    break;

  case 601:

/* Line 1455 of yacc.c  */
#line 3588 "drizzled/sql_yacc.yy"
    {
            if (parser::add_select_to_union_list(YYSession, &Lex, (bool)(yyvsp[(3) - (3)].num)))
              DRIZZLE_YYABORT;
          }
    break;

  case 602:

/* Line 1455 of yacc.c  */
#line 3593 "drizzled/sql_yacc.yy"
    {
            /*
              Remove from the name resolution context stack the context of the
              last select in the union.
             */
            Lex.pop_context();
          }
    break;

  case 604:

/* Line 1455 of yacc.c  */
#line 3606 "drizzled/sql_yacc.yy"
    {
            Select_Lex * sel= Lex.current_select;
            if (Lex.current_select->set_braces(0))
            {
              parser::my_parse_error(YYSession->m_lip);
              DRIZZLE_YYABORT;
            }
            if (sel->linkage == UNION_TYPE &&
                sel->master_unit()->first_select()->braces)
            {
              parser::my_parse_error(YYSession->m_lip);
              DRIZZLE_YYABORT;
            }
          }
    break;

  case 605:

/* Line 1455 of yacc.c  */
#line 3624 "drizzled/sql_yacc.yy"
    {
            Select_Lex *sel= Lex.current_select;
            if (sel->linkage != UNION_TYPE)
              init_select(&Lex);
            Lex.current_select->parsing_place= SELECT_LIST;
          }
    break;

  case 606:

/* Line 1455 of yacc.c  */
#line 3631 "drizzled/sql_yacc.yy"
    {
            Lex.current_select->parsing_place= NO_MATTER;
          }
    break;

  case 608:

/* Line 1455 of yacc.c  */
#line 3640 "drizzled/sql_yacc.yy"
    {
            if ((yyvsp[(1) - (1)].select_lex)->init_nested_join(Lex.session))
              DRIZZLE_YYABORT;
          }
    break;

  case 609:

/* Line 1455 of yacc.c  */
#line 3645 "drizzled/sql_yacc.yy"
    {
            /* for normal joins, $3 != NULL and end_nested_join() != NULL,
               for derived tables, both must equal NULL */

            if (!((yyval.table_list)= (yyvsp[(1) - (3)].select_lex)->end_nested_join(Lex.session)) && (yyvsp[(3) - (3)].table_list))
              DRIZZLE_YYABORT;

            if (!(yyvsp[(3) - (3)].table_list) && (yyval.table_list))
            {
              parser::my_parse_error(YYSession->m_lip);
              DRIZZLE_YYABORT;
            }
          }
    break;

  case 610:

/* Line 1455 of yacc.c  */
#line 3661 "drizzled/sql_yacc.yy"
    {
            Lex.derived_tables|= DERIVED_SUBQUERY;
            if (not Lex.expr_allows_subselect)
            {
              parser::my_parse_error(YYSession->m_lip);
              DRIZZLE_YYABORT;
            }
            if (Lex.current_select->linkage == GLOBAL_OPTIONS_TYPE || new_select(&Lex, 1))
              DRIZZLE_YYABORT;
            init_select(&Lex);
            Lex.current_select->linkage= DERIVED_TABLE_TYPE;
            Lex.current_select->parsing_place= SELECT_LIST;
          }
    break;

  case 611:

/* Line 1455 of yacc.c  */
#line 3675 "drizzled/sql_yacc.yy"
    {
            Lex.current_select->parsing_place= NO_MATTER;
          }
    break;

  case 613:

/* Line 1455 of yacc.c  */
#line 3682 "drizzled/sql_yacc.yy"
    { (yyval.select_lex)= Lex.current_select; }
    break;

  case 614:

/* Line 1455 of yacc.c  */
#line 3687 "drizzled/sql_yacc.yy"
    {
            Select_Lex *sel= Lex.current_select;
            TableList *embedding;
            if (!sel->embedding || sel->end_nested_join(Lex.session))
            {
              /* we are not in parentheses */
              parser::my_parse_error(YYSession->m_lip);
              DRIZZLE_YYABORT;
            }
            embedding= Lex.current_select->embedding;
            (yyval.num)= embedding &&
                !embedding->getNestedJoin()->join_list.size();
            /* return true if we are deeply nested */
          }
    break;

  case 615:

/* Line 1455 of yacc.c  */
#line 3704 "drizzled/sql_yacc.yy"
    {}
    break;

  case 616:

/* Line 1455 of yacc.c  */
#line 3705 "drizzled/sql_yacc.yy"
    {}
    break;

  case 617:

/* Line 1455 of yacc.c  */
#line 3710 "drizzled/sql_yacc.yy"
    {
            (yyval.num)= INDEX_HINT_MASK_ALL;
          }
    break;

  case 618:

/* Line 1455 of yacc.c  */
#line 3713 "drizzled/sql_yacc.yy"
    { (yyval.num)= INDEX_HINT_MASK_JOIN;  }
    break;

  case 619:

/* Line 1455 of yacc.c  */
#line 3714 "drizzled/sql_yacc.yy"
    { (yyval.num)= INDEX_HINT_MASK_ORDER; }
    break;

  case 620:

/* Line 1455 of yacc.c  */
#line 3715 "drizzled/sql_yacc.yy"
    { (yyval.num)= INDEX_HINT_MASK_GROUP; }
    break;

  case 621:

/* Line 1455 of yacc.c  */
#line 3719 "drizzled/sql_yacc.yy"
    { (yyval.index_hint)= INDEX_HINT_FORCE; }
    break;

  case 622:

/* Line 1455 of yacc.c  */
#line 3720 "drizzled/sql_yacc.yy"
    { (yyval.index_hint)= INDEX_HINT_IGNORE; }
    break;

  case 623:

/* Line 1455 of yacc.c  */
#line 3725 "drizzled/sql_yacc.yy"
    {
            Lex.current_select->set_index_hint_type((yyvsp[(1) - (3)].index_hint), (yyvsp[(3) - (3)].num));
          }
    break;

  case 625:

/* Line 1455 of yacc.c  */
#line 3730 "drizzled/sql_yacc.yy"
    {
            Lex.current_select->set_index_hint_type(INDEX_HINT_USE, (yyvsp[(3) - (3)].num));
          }
    break;

  case 630:

/* Line 1455 of yacc.c  */
#line 3743 "drizzled/sql_yacc.yy"
    { Lex.current_select->alloc_index_hints(YYSession); }
    break;

  case 632:

/* Line 1455 of yacc.c  */
#line 3747 "drizzled/sql_yacc.yy"
    {  Lex.current_select->clear_index_hints(); }
    break;

  case 634:

/* Line 1455 of yacc.c  */
#line 3752 "drizzled/sql_yacc.yy"
    { Lex.current_select->add_index_hint(YYSession, NULL, 0); }
    break;

  case 635:

/* Line 1455 of yacc.c  */
#line 3753 "drizzled/sql_yacc.yy"
    {}
    break;

  case 636:

/* Line 1455 of yacc.c  */
#line 3758 "drizzled/sql_yacc.yy"
    { Lex.current_select->add_index_hint(YYSession, (yyvsp[(1) - (1)].lex_str).str, (yyvsp[(1) - (1)].lex_str).length); }
    break;

  case 637:

/* Line 1455 of yacc.c  */
#line 3760 "drizzled/sql_yacc.yy"
    { Lex.current_select->add_index_hint(YYSession, (char *)"PRIMARY", 7); }
    break;

  case 640:

/* Line 1455 of yacc.c  */
#line 3770 "drizzled/sql_yacc.yy"
    {
            if (!((yyval.string_list)= new List<String>))
              DRIZZLE_YYABORT;
            (yyval.string_list)->push_back(new (YYSession->mem_root)
                              String((const char *) (yyvsp[(1) - (1)].lex_str).str, (yyvsp[(1) - (1)].lex_str).length,
                                      system_charset_info));
          }
    break;

  case 641:

/* Line 1455 of yacc.c  */
#line 3778 "drizzled/sql_yacc.yy"
    {
            (yyvsp[(1) - (3)].string_list)->push_back(new (YYSession->mem_root)
                              String((const char *) (yyvsp[(3) - (3)].lex_str).str, (yyvsp[(3) - (3)].lex_str).length,
                                      system_charset_info));
            (yyval.string_list)= (yyvsp[(1) - (3)].string_list);
          }
    break;

  case 642:

/* Line 1455 of yacc.c  */
#line 3787 "drizzled/sql_yacc.yy"
    {}
    break;

  case 643:

/* Line 1455 of yacc.c  */
#line 3788 "drizzled/sql_yacc.yy"
    { (yyval.interval)=INTERVAL_DAY_HOUR; }
    break;

  case 644:

/* Line 1455 of yacc.c  */
#line 3789 "drizzled/sql_yacc.yy"
    { (yyval.interval)=INTERVAL_DAY_MICROSECOND; }
    break;

  case 645:

/* Line 1455 of yacc.c  */
#line 3790 "drizzled/sql_yacc.yy"
    { (yyval.interval)=INTERVAL_DAY_MINUTE; }
    break;

  case 646:

/* Line 1455 of yacc.c  */
#line 3791 "drizzled/sql_yacc.yy"
    { (yyval.interval)=INTERVAL_DAY_SECOND; }
    break;

  case 647:

/* Line 1455 of yacc.c  */
#line 3792 "drizzled/sql_yacc.yy"
    { (yyval.interval)=INTERVAL_HOUR_MICROSECOND; }
    break;

  case 648:

/* Line 1455 of yacc.c  */
#line 3793 "drizzled/sql_yacc.yy"
    { (yyval.interval)=INTERVAL_HOUR_MINUTE; }
    break;

  case 649:

/* Line 1455 of yacc.c  */
#line 3794 "drizzled/sql_yacc.yy"
    { (yyval.interval)=INTERVAL_HOUR_SECOND; }
    break;

  case 650:

/* Line 1455 of yacc.c  */
#line 3795 "drizzled/sql_yacc.yy"
    { (yyval.interval)=INTERVAL_MINUTE_MICROSECOND; }
    break;

  case 651:

/* Line 1455 of yacc.c  */
#line 3796 "drizzled/sql_yacc.yy"
    { (yyval.interval)=INTERVAL_MINUTE_SECOND; }
    break;

  case 652:

/* Line 1455 of yacc.c  */
#line 3797 "drizzled/sql_yacc.yy"
    { (yyval.interval)=INTERVAL_SECOND_MICROSECOND; }
    break;

  case 653:

/* Line 1455 of yacc.c  */
#line 3798 "drizzled/sql_yacc.yy"
    { (yyval.interval)=INTERVAL_YEAR_MONTH; }
    break;

  case 654:

/* Line 1455 of yacc.c  */
#line 3802 "drizzled/sql_yacc.yy"
    {}
    break;

  case 655:

/* Line 1455 of yacc.c  */
#line 3803 "drizzled/sql_yacc.yy"
    {
                                  (yyval.interval_time_st)=INTERVAL_MICROSECOND;
                                  /*
                                    FRAC_SECOND was mistakenly implemented with
                                    a wrong resolution. According to the ODBC
                                    standard it should be nanoseconds, not
                                    microseconds. Changing it to nanoseconds
                                    in MySQL would mean making TIMESTAMPDIFF
                                    and TIMESTAMPADD to return DECIMAL, since
                                    the return value would be too big for BIGINT
                                    Hence we just deprecate the incorrect
                                    implementation without changing its
                                    resolution.
                                  */
                                }
    break;

  case 656:

/* Line 1455 of yacc.c  */
#line 3821 "drizzled/sql_yacc.yy"
    { (yyval.interval_time_st)=INTERVAL_DAY; }
    break;

  case 657:

/* Line 1455 of yacc.c  */
#line 3822 "drizzled/sql_yacc.yy"
    { (yyval.interval_time_st)=INTERVAL_WEEK; }
    break;

  case 658:

/* Line 1455 of yacc.c  */
#line 3823 "drizzled/sql_yacc.yy"
    { (yyval.interval_time_st)=INTERVAL_HOUR; }
    break;

  case 659:

/* Line 1455 of yacc.c  */
#line 3824 "drizzled/sql_yacc.yy"
    { (yyval.interval_time_st)=INTERVAL_MINUTE; }
    break;

  case 660:

/* Line 1455 of yacc.c  */
#line 3825 "drizzled/sql_yacc.yy"
    { (yyval.interval_time_st)=INTERVAL_MONTH; }
    break;

  case 661:

/* Line 1455 of yacc.c  */
#line 3826 "drizzled/sql_yacc.yy"
    { (yyval.interval_time_st)=INTERVAL_QUARTER; }
    break;

  case 662:

/* Line 1455 of yacc.c  */
#line 3827 "drizzled/sql_yacc.yy"
    { (yyval.interval_time_st)=INTERVAL_SECOND; }
    break;

  case 663:

/* Line 1455 of yacc.c  */
#line 3828 "drizzled/sql_yacc.yy"
    { (yyval.interval_time_st)=INTERVAL_MICROSECOND; }
    break;

  case 664:

/* Line 1455 of yacc.c  */
#line 3829 "drizzled/sql_yacc.yy"
    { (yyval.interval_time_st)=INTERVAL_YEAR; }
    break;

  case 667:

/* Line 1455 of yacc.c  */
#line 3838 "drizzled/sql_yacc.yy"
    { (yyval.lex_str_ptr)=0; }
    break;

  case 668:

/* Line 1455 of yacc.c  */
#line 3840 "drizzled/sql_yacc.yy"
    {
            (yyval.lex_str_ptr)= (drizzled::LEX_STRING*) memory::sql_memdup(&(yyvsp[(2) - (2)].lex_str),sizeof(drizzled::LEX_STRING));
          }
    break;

  case 671:

/* Line 1455 of yacc.c  */
#line 3851 "drizzled/sql_yacc.yy"
    { Lex.current_select->where= 0; }
    break;

  case 672:

/* Line 1455 of yacc.c  */
#line 3853 "drizzled/sql_yacc.yy"
    {
            Lex.current_select->parsing_place= IN_WHERE;
          }
    break;

  case 673:

/* Line 1455 of yacc.c  */
#line 3857 "drizzled/sql_yacc.yy"
    {
            Select_Lex *select= Lex.current_select;
            select->where= (yyvsp[(3) - (3)].item);
            select->parsing_place= NO_MATTER;
            if ((yyvsp[(3) - (3)].item))
              (yyvsp[(3) - (3)].item)->top_level_item();
          }
    break;

  case 675:

/* Line 1455 of yacc.c  */
#line 3869 "drizzled/sql_yacc.yy"
    {
            Lex.current_select->parsing_place= IN_HAVING;
          }
    break;

  case 676:

/* Line 1455 of yacc.c  */
#line 3873 "drizzled/sql_yacc.yy"
    {
            Select_Lex *sel= Lex.current_select;
            sel->having= (yyvsp[(3) - (3)].item);
            sel->parsing_place= NO_MATTER;
            if ((yyvsp[(3) - (3)].item))
              (yyvsp[(3) - (3)].item)->top_level_item();
          }
    break;

  case 677:

/* Line 1455 of yacc.c  */
#line 3884 "drizzled/sql_yacc.yy"
    {
            Lex.escape_used= true;
            (yyval.item)= (yyvsp[(2) - (2)].item);
          }
    break;

  case 678:

/* Line 1455 of yacc.c  */
#line 3889 "drizzled/sql_yacc.yy"
    {
            Lex.escape_used= false;
            (yyval.item)= new Item_string("\\", 1, &my_charset_utf8_general_ci);
          }
    break;

  case 681:

/* Line 1455 of yacc.c  */
#line 3906 "drizzled/sql_yacc.yy"
    { if (YYSession->add_group_to_list((yyvsp[(3) - (4)].item),(bool) (yyvsp[(4) - (4)].num))) DRIZZLE_YYABORT; }
    break;

  case 682:

/* Line 1455 of yacc.c  */
#line 3908 "drizzled/sql_yacc.yy"
    { if (YYSession->add_group_to_list((yyvsp[(1) - (2)].item),(bool) (yyvsp[(2) - (2)].num))) DRIZZLE_YYABORT; }
    break;

  case 683:

/* Line 1455 of yacc.c  */
#line 3912 "drizzled/sql_yacc.yy"
    {}
    break;

  case 684:

/* Line 1455 of yacc.c  */
#line 3914 "drizzled/sql_yacc.yy"
    {
            /*
              'WITH ROLLUP' is needed for backward compatibility,
              and cause LALR(2) conflicts.
              This syntax is not standard.
              MySQL syntax: GROUP BY col1, col2, col3 WITH ROLLUP
              SQL-2003: GROUP BY ... ROLLUP(col1, col2, col3)
            */
            if (Lex.current_select->linkage == GLOBAL_OPTIONS_TYPE)
            {
              my_error(ER_WRONG_USAGE, MYF(0), "WITH ROLLUP",
                       "global union parameters");
              DRIZZLE_YYABORT;
            }
            Lex.current_select->olap= ROLLUP_TYPE;
          }
    break;

  case 688:

/* Line 1455 of yacc.c  */
#line 3947 "drizzled/sql_yacc.yy"
    {
            bool ascending= ((yyvsp[(2) - (2)].num) == 1) ? true : false;
            if (YYSession->add_order_to_list((yyvsp[(1) - (2)].item), ascending))
              DRIZZLE_YYABORT;
          }
    break;

  case 691:

/* Line 1455 of yacc.c  */
#line 3965 "drizzled/sql_yacc.yy"
    {
            if (not parser::buildOrderBy(&Lex))
              DRIZZLE_YYABORT;
          }
    break;

  case 693:

/* Line 1455 of yacc.c  */
#line 3974 "drizzled/sql_yacc.yy"
    {
            if (YYSession->add_order_to_list((yyvsp[(3) - (4)].item),(bool) (yyvsp[(4) - (4)].num)))
              DRIZZLE_YYABORT;
          }
    break;

  case 694:

/* Line 1455 of yacc.c  */
#line 3979 "drizzled/sql_yacc.yy"
    {
            if (YYSession->add_order_to_list((yyvsp[(1) - (2)].item),(bool) (yyvsp[(2) - (2)].num)))
              DRIZZLE_YYABORT;
          }
    break;

  case 695:

/* Line 1455 of yacc.c  */
#line 3986 "drizzled/sql_yacc.yy"
    { (yyval.num) =  1; }
    break;

  case 696:

/* Line 1455 of yacc.c  */
#line 3987 "drizzled/sql_yacc.yy"
    { (yyval.num) =1; }
    break;

  case 697:

/* Line 1455 of yacc.c  */
#line 3988 "drizzled/sql_yacc.yy"
    { (yyval.num) =0; }
    break;

  case 698:

/* Line 1455 of yacc.c  */
#line 3993 "drizzled/sql_yacc.yy"
    {
            Select_Lex *sel= Lex.current_select;
            sel->offset_limit= 0;
            sel->select_limit= 0;
          }
    break;

  case 699:

/* Line 1455 of yacc.c  */
#line 3998 "drizzled/sql_yacc.yy"
    {}
    break;

  case 700:

/* Line 1455 of yacc.c  */
#line 4002 "drizzled/sql_yacc.yy"
    {}
    break;

  case 701:

/* Line 1455 of yacc.c  */
#line 4003 "drizzled/sql_yacc.yy"
    {}
    break;

  case 702:

/* Line 1455 of yacc.c  */
#line 4007 "drizzled/sql_yacc.yy"
    {}
    break;

  case 703:

/* Line 1455 of yacc.c  */
#line 4012 "drizzled/sql_yacc.yy"
    {
            Select_Lex *sel= Lex.current_select;
            sel->select_limit= (yyvsp[(1) - (1)].item);
            sel->offset_limit= 0;
            sel->explicit_limit= 1;
          }
    break;

  case 704:

/* Line 1455 of yacc.c  */
#line 4019 "drizzled/sql_yacc.yy"
    {
            Select_Lex *sel= Lex.current_select;
            sel->select_limit= (yyvsp[(3) - (3)].item);
            sel->offset_limit= (yyvsp[(1) - (3)].item);
            sel->explicit_limit= 1;
          }
    break;

  case 705:

/* Line 1455 of yacc.c  */
#line 4026 "drizzled/sql_yacc.yy"
    {
            Select_Lex *sel= Lex.current_select;
            sel->select_limit= (yyvsp[(1) - (3)].item);
            sel->offset_limit= (yyvsp[(3) - (3)].item);
            sel->explicit_limit= 1;
          }
    break;

  case 706:

/* Line 1455 of yacc.c  */
#line 4035 "drizzled/sql_yacc.yy"
    { (yyval.item)= new Item_uint((yyvsp[(1) - (1)].lex_str).str, (yyvsp[(1) - (1)].lex_str).length); }
    break;

  case 707:

/* Line 1455 of yacc.c  */
#line 4036 "drizzled/sql_yacc.yy"
    { (yyval.item)= new Item_uint((yyvsp[(1) - (1)].lex_str).str, (yyvsp[(1) - (1)].lex_str).length); }
    break;

  case 708:

/* Line 1455 of yacc.c  */
#line 4037 "drizzled/sql_yacc.yy"
    { (yyval.item)= new Item_uint((yyvsp[(1) - (1)].lex_str).str, (yyvsp[(1) - (1)].lex_str).length); }
    break;

  case 709:

/* Line 1455 of yacc.c  */
#line 4042 "drizzled/sql_yacc.yy"
    {
            Lex.current_select->select_limit= 0;
          }
    break;

  case 710:

/* Line 1455 of yacc.c  */
#line 4046 "drizzled/sql_yacc.yy"
    {
            Select_Lex *sel= Lex.current_select;
            sel->select_limit= (yyvsp[(2) - (2)].item);
            sel->explicit_limit= 1;
          }
    break;

  case 711:

/* Line 1455 of yacc.c  */
#line 4054 "drizzled/sql_yacc.yy"
    { int error; (yyval.ulong_num)= (unsigned long) internal::my_strtoll10((yyvsp[(1) - (1)].lex_str).str, (char**) 0, &error); }
    break;

  case 712:

/* Line 1455 of yacc.c  */
#line 4055 "drizzled/sql_yacc.yy"
    { (yyval.ulong_num)= (unsigned long) strtol((yyvsp[(1) - (1)].lex_str).str, (char**) 0, 16); }
    break;

  case 713:

/* Line 1455 of yacc.c  */
#line 4056 "drizzled/sql_yacc.yy"
    { int error; (yyval.ulong_num)= (unsigned long) internal::my_strtoll10((yyvsp[(1) - (1)].lex_str).str, (char**) 0, &error); }
    break;

  case 714:

/* Line 1455 of yacc.c  */
#line 4057 "drizzled/sql_yacc.yy"
    { int error; (yyval.ulong_num)= (unsigned long) internal::my_strtoll10((yyvsp[(1) - (1)].lex_str).str, (char**) 0, &error); }
    break;

  case 715:

/* Line 1455 of yacc.c  */
#line 4058 "drizzled/sql_yacc.yy"
    { int error; (yyval.ulong_num)= (unsigned long) internal::my_strtoll10((yyvsp[(1) - (1)].lex_str).str, (char**) 0, &error); }
    break;

  case 716:

/* Line 1455 of yacc.c  */
#line 4059 "drizzled/sql_yacc.yy"
    { int error; (yyval.ulong_num)= (unsigned long) internal::my_strtoll10((yyvsp[(1) - (1)].lex_str).str, (char**) 0, &error); }
    break;

  case 717:

/* Line 1455 of yacc.c  */
#line 4063 "drizzled/sql_yacc.yy"
    { int error; (yyval.ulonglong_number)= (uint64_t) internal::my_strtoll10((yyvsp[(1) - (1)].lex_str).str, (char**) 0, &error); }
    break;

  case 718:

/* Line 1455 of yacc.c  */
#line 4064 "drizzled/sql_yacc.yy"
    { int error; (yyval.ulonglong_number)= (uint64_t) internal::my_strtoll10((yyvsp[(1) - (1)].lex_str).str, (char**) 0, &error); }
    break;

  case 719:

/* Line 1455 of yacc.c  */
#line 4065 "drizzled/sql_yacc.yy"
    { int error; (yyval.ulonglong_number)= (uint64_t) internal::my_strtoll10((yyvsp[(1) - (1)].lex_str).str, (char**) 0, &error); }
    break;

  case 720:

/* Line 1455 of yacc.c  */
#line 4066 "drizzled/sql_yacc.yy"
    { int error; (yyval.ulonglong_number)= (uint64_t) internal::my_strtoll10((yyvsp[(1) - (1)].lex_str).str, (char**) 0, &error); }
    break;

  case 721:

/* Line 1455 of yacc.c  */
#line 4067 "drizzled/sql_yacc.yy"
    { int error; (yyval.ulonglong_number)= (uint64_t) internal::my_strtoll10((yyvsp[(1) - (1)].lex_str).str, (char**) 0, &error); }
    break;

  case 722:

/* Line 1455 of yacc.c  */
#line 4071 "drizzled/sql_yacc.yy"
    {
            if (not Lex.describe && (not (Lex.result= new select_dumpvar())))
              DRIZZLE_YYABORT;
          }
    break;

  case 723:

/* Line 1455 of yacc.c  */
#line 4076 "drizzled/sql_yacc.yy"
    {}
    break;

  case 725:

/* Line 1455 of yacc.c  */
#line 4081 "drizzled/sql_yacc.yy"
    {}
    break;

  case 726:

/* Line 1455 of yacc.c  */
#line 4086 "drizzled/sql_yacc.yy"
    {
            if (Lex.result)
            {
              ((select_dumpvar *)Lex.result)->var_list.push_back( new var((yyvsp[(2) - (2)].lex_str),0,0,(enum_field_types)0));
            }
            else
            {
              /*
                The parser won't create select_result instance only
                if it's an EXPLAIN.
              */
              assert(Lex.describe);
            }
          }
    break;

  case 727:

/* Line 1455 of yacc.c  */
#line 4104 "drizzled/sql_yacc.yy"
    { }
    break;

  case 729:

/* Line 1455 of yacc.c  */
#line 4110 "drizzled/sql_yacc.yy"
    {
            Lex.setCacheable(false);
            if (!(Lex.exchange= new file_exchange((yyvsp[(2) - (2)].lex_str).str, 0)) ||
                !(Lex.result= new select_export(Lex.exchange)))
              DRIZZLE_YYABORT;
          }
    break;

  case 731:

/* Line 1455 of yacc.c  */
#line 4118 "drizzled/sql_yacc.yy"
    {
            if (not Lex.describe)
            {
              Lex.setCacheable(false);
              if (not (Lex.exchange= new file_exchange((yyvsp[(2) - (2)].lex_str).str,1)))
                DRIZZLE_YYABORT;
              if (not (Lex.result= new select_dump(Lex.exchange)))
                DRIZZLE_YYABORT;
            }
          }
    break;

  case 732:

/* Line 1455 of yacc.c  */
#line 4129 "drizzled/sql_yacc.yy"
    {Lex.setCacheable(false);}
    break;

  case 733:

/* Line 1455 of yacc.c  */
#line 4138 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::catalog::Drop(YYSession, (yyvsp[(3) - (3)].lex_str));
          }
    break;

  case 734:

/* Line 1455 of yacc.c  */
#line 4142 "drizzled/sql_yacc.yy"
    {
            statement::DropTable *statement= new statement::DropTable(YYSession);
            Lex.statement= statement;
            statement->drop_temporary= (yyvsp[(2) - (5)].boolean);
            statement->drop_if_exists= (yyvsp[(4) - (5)].boolean);
          }
    break;

  case 735:

/* Line 1455 of yacc.c  */
#line 4148 "drizzled/sql_yacc.yy"
    {}
    break;

  case 736:

/* Line 1455 of yacc.c  */
#line 4149 "drizzled/sql_yacc.yy"
    {
            statement::DropIndex *statement= new statement::DropIndex(YYSession);
            Lex.statement= statement;
            statement->alter_info.flags.set(ALTER_DROP_INDEX);
            statement->alter_info.build_method= (yyvsp[(2) - (7)].build_method);
            statement->alter_info.drop_list.push_back(AlterDrop(AlterDrop::KEY, (yyvsp[(4) - (7)].lex_str).str));
            if (not Lex.current_select->add_table_to_list(Lex.session, (yyvsp[(6) - (7)].table), NULL,
                                                          TL_OPTION_UPDATING))
              DRIZZLE_YYABORT;
          }
    break;

  case 737:

/* Line 1455 of yacc.c  */
#line 4160 "drizzled/sql_yacc.yy"
    {
            statement::DropSchema *statement= new statement::DropSchema(YYSession);
            Lex.statement= statement;
            statement->drop_if_exists=(yyvsp[(3) - (4)].boolean);
            Lex.name= (yyvsp[(4) - (4)].lex_str);
          }
    break;

  case 740:

/* Line 1455 of yacc.c  */
#line 4175 "drizzled/sql_yacc.yy"
    {
            if (!Lex.current_select->add_table_to_list(YYSession, (yyvsp[(1) - (1)].table), NULL, TL_OPTION_UPDATING))
              DRIZZLE_YYABORT;
          }
    break;

  case 741:

/* Line 1455 of yacc.c  */
#line 4182 "drizzled/sql_yacc.yy"
    { (yyval.boolean)= false; }
    break;

  case 742:

/* Line 1455 of yacc.c  */
#line 4183 "drizzled/sql_yacc.yy"
    { (yyval.boolean)= true; }
    break;

  case 743:

/* Line 1455 of yacc.c  */
#line 4187 "drizzled/sql_yacc.yy"
    { (yyval.boolean)= false; }
    break;

  case 744:

/* Line 1455 of yacc.c  */
#line 4188 "drizzled/sql_yacc.yy"
    { (yyval.boolean)= true; }
    break;

  case 745:

/* Line 1455 of yacc.c  */
#line 4197 "drizzled/sql_yacc.yy"
    {
          Lex.statement= new statement::Execute(YYSession, (yyvsp[(2) - (5)].execute_string), (yyvsp[(3) - (5)].boolean), (yyvsp[(4) - (5)].boolean), (yyvsp[(5) - (5)].boolean));
        }
    break;

  case 746:

/* Line 1455 of yacc.c  */
#line 4204 "drizzled/sql_yacc.yy"
    {
            (yyval.execute_string).set((yyvsp[(1) - (1)].lex_str));
         }
    break;

  case 747:

/* Line 1455 of yacc.c  */
#line 4208 "drizzled/sql_yacc.yy"
    {
            (yyval.execute_string).set((yyvsp[(2) - (2)].lex_str), true);
        }
    break;

  case 748:

/* Line 1455 of yacc.c  */
#line 4213 "drizzled/sql_yacc.yy"
    { (yyval.boolean)= false; }
    break;

  case 749:

/* Line 1455 of yacc.c  */
#line 4214 "drizzled/sql_yacc.yy"
    { (yyval.boolean)= true; }
    break;

  case 750:

/* Line 1455 of yacc.c  */
#line 4218 "drizzled/sql_yacc.yy"
    { (yyval.boolean)= false; }
    break;

  case 751:

/* Line 1455 of yacc.c  */
#line 4219 "drizzled/sql_yacc.yy"
    { (yyval.boolean)= true; }
    break;

  case 752:

/* Line 1455 of yacc.c  */
#line 4223 "drizzled/sql_yacc.yy"
    { (yyval.boolean)= false; }
    break;

  case 753:

/* Line 1455 of yacc.c  */
#line 4224 "drizzled/sql_yacc.yy"
    { (yyval.boolean)= true; }
    break;

  case 754:

/* Line 1455 of yacc.c  */
#line 4233 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::Insert(YYSession);
            Lex.duplicates= DUP_ERROR;
            init_select(&Lex);
            /* for subselects */
            Lex.lock_option= TL_READ;
          }
    break;

  case 755:

/* Line 1455 of yacc.c  */
#line 4241 "drizzled/sql_yacc.yy"
    {
            Lex.current_select->set_lock_for_tables(TL_WRITE_CONCURRENT_INSERT);
            Lex.current_select= &Lex.select_lex;
          }
    break;

  case 756:

/* Line 1455 of yacc.c  */
#line 4246 "drizzled/sql_yacc.yy"
    {}
    break;

  case 757:

/* Line 1455 of yacc.c  */
#line 4251 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::Replace(YYSession);
            Lex.duplicates= DUP_REPLACE;
            init_select(&Lex);
          }
    break;

  case 758:

/* Line 1455 of yacc.c  */
#line 4257 "drizzled/sql_yacc.yy"
    {
            Lex.current_select->set_lock_for_tables(TL_WRITE_DEFAULT);
            Lex.current_select= &Lex.select_lex;
          }
    break;

  case 759:

/* Line 1455 of yacc.c  */
#line 4262 "drizzled/sql_yacc.yy"
    {}
    break;

  case 760:

/* Line 1455 of yacc.c  */
#line 4266 "drizzled/sql_yacc.yy"
    {}
    break;

  case 761:

/* Line 1455 of yacc.c  */
#line 4267 "drizzled/sql_yacc.yy"
    {}
    break;

  case 762:

/* Line 1455 of yacc.c  */
#line 4272 "drizzled/sql_yacc.yy"
    {
            Lex.field_list.clear();
            Lex.many_values.clear();
            Lex.insert_list=0;
          }
    break;

  case 763:

/* Line 1455 of yacc.c  */
#line 4279 "drizzled/sql_yacc.yy"
    {}
    break;

  case 764:

/* Line 1455 of yacc.c  */
#line 4280 "drizzled/sql_yacc.yy"
    {}
    break;

  case 765:

/* Line 1455 of yacc.c  */
#line 4281 "drizzled/sql_yacc.yy"
    {}
    break;

  case 766:

/* Line 1455 of yacc.c  */
#line 4283 "drizzled/sql_yacc.yy"
    {
            if (not (Lex.insert_list = new List_item) ||
                Lex.many_values.push_back(Lex.insert_list))
              DRIZZLE_YYABORT;
          }
    break;

  case 768:

/* Line 1455 of yacc.c  */
#line 4292 "drizzled/sql_yacc.yy"
    { Lex.field_list.push_back((yyvsp[(3) - (3)].item)); }
    break;

  case 769:

/* Line 1455 of yacc.c  */
#line 4293 "drizzled/sql_yacc.yy"
    { Lex.field_list.push_back((yyvsp[(1) - (1)].item)); }
    break;

  case 770:

/* Line 1455 of yacc.c  */
#line 4297 "drizzled/sql_yacc.yy"
    {}
    break;

  case 771:

/* Line 1455 of yacc.c  */
#line 4298 "drizzled/sql_yacc.yy"
    {}
    break;

  case 772:

/* Line 1455 of yacc.c  */
#line 4300 "drizzled/sql_yacc.yy"
    {
            Lex.current_select->set_braces(0);
          }
    break;

  case 773:

/* Line 1455 of yacc.c  */
#line 4303 "drizzled/sql_yacc.yy"
    {}
    break;

  case 774:

/* Line 1455 of yacc.c  */
#line 4305 "drizzled/sql_yacc.yy"
    {
            Lex.current_select->set_braces(1);
          }
    break;

  case 775:

/* Line 1455 of yacc.c  */
#line 4308 "drizzled/sql_yacc.yy"
    {}
    break;

  case 780:

/* Line 1455 of yacc.c  */
#line 4323 "drizzled/sql_yacc.yy"
    {
            if (Lex.field_list.push_back((yyvsp[(1) - (3)].item)) ||
                Lex.insert_list->push_back((yyvsp[(3) - (3)].item)))
              DRIZZLE_YYABORT;
          }
    break;

  case 781:

/* Line 1455 of yacc.c  */
#line 4331 "drizzled/sql_yacc.yy"
    {}
    break;

  case 782:

/* Line 1455 of yacc.c  */
#line 4332 "drizzled/sql_yacc.yy"
    {}
    break;

  case 783:

/* Line 1455 of yacc.c  */
#line 4336 "drizzled/sql_yacc.yy"
    {}
    break;

  case 784:

/* Line 1455 of yacc.c  */
#line 4337 "drizzled/sql_yacc.yy"
    {}
    break;

  case 785:

/* Line 1455 of yacc.c  */
#line 4342 "drizzled/sql_yacc.yy"
    {
              if (!(Lex.insert_list = new List_item))
                DRIZZLE_YYABORT;
          }
    break;

  case 786:

/* Line 1455 of yacc.c  */
#line 4347 "drizzled/sql_yacc.yy"
    {
            if (Lex.many_values.push_back(Lex.insert_list))
              DRIZZLE_YYABORT;
          }
    break;

  case 787:

/* Line 1455 of yacc.c  */
#line 4354 "drizzled/sql_yacc.yy"
    {}
    break;

  case 789:

/* Line 1455 of yacc.c  */
#line 4360 "drizzled/sql_yacc.yy"
    {
            if (Lex.insert_list->push_back((yyvsp[(3) - (3)].item)))
              DRIZZLE_YYABORT;
          }
    break;

  case 790:

/* Line 1455 of yacc.c  */
#line 4365 "drizzled/sql_yacc.yy"
    {
            if (Lex.insert_list->push_back((yyvsp[(1) - (1)].item)))
              DRIZZLE_YYABORT;
          }
    break;

  case 791:

/* Line 1455 of yacc.c  */
#line 4372 "drizzled/sql_yacc.yy"
    { (yyval.item)= (yyvsp[(1) - (1)].item);}
    break;

  case 792:

/* Line 1455 of yacc.c  */
#line 4373 "drizzled/sql_yacc.yy"
    {(yyval.item)= new Item_default_value(Lex.current_context()); }
    break;

  case 794:

/* Line 1455 of yacc.c  */
#line 4378 "drizzled/sql_yacc.yy"
    { Lex.duplicates= DUP_UPDATE; }
    break;

  case 796:

/* Line 1455 of yacc.c  */
#line 4386 "drizzled/sql_yacc.yy"
    {
            init_select(&Lex);
            Lex.statement= new statement::Update(YYSession);
            Lex.lock_option= TL_UNLOCK; /* Will be set later */
            Lex.duplicates= DUP_ERROR;
            if (not Lex.select_lex.add_table_to_list(YYSession, (yyvsp[(3) - (5)].table), NULL,0))
              DRIZZLE_YYABORT;

            if (Lex.select_lex.get_table_list()->derived)
            {
              /* it is single table update and it is update of derived table */
              my_error(ER_NON_UPDATABLE_TABLE, MYF(0),
                       Lex.select_lex.get_table_list()->alias, "UPDATE");
              DRIZZLE_YYABORT;
            }
            /*
              In case of multi-update setting write lock for all tables may
              be too pessimistic. We will decrease lock level if possible in
              multi_update().
            */
            Lex.current_select->set_lock_for_tables(TL_WRITE_DEFAULT);
          }
    break;

  case 797:

/* Line 1455 of yacc.c  */
#line 4408 "drizzled/sql_yacc.yy"
    {}
    break;

  case 800:

/* Line 1455 of yacc.c  */
#line 4418 "drizzled/sql_yacc.yy"
    {
            if (YYSession->add_item_to_list((yyvsp[(1) - (3)].item)) || YYSession->add_value_to_list((yyvsp[(3) - (3)].item)))
              DRIZZLE_YYABORT;
          }
    break;

  case 803:

/* Line 1455 of yacc.c  */
#line 4431 "drizzled/sql_yacc.yy"
    {
          if (Lex.update_list.push_back((yyvsp[(1) - (3)].item)) ||
              Lex.value_list.push_back((yyvsp[(3) - (3)].item)))
              DRIZZLE_YYABORT;
          }
    break;

  case 804:

/* Line 1455 of yacc.c  */
#line 4442 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::Delete(YYSession);
            init_select(&Lex);
            Lex.lock_option= TL_WRITE_DEFAULT;
            Lex.select_lex.init_order();

            if (!Lex.current_select->add_table_to_list(YYSession, (yyvsp[(4) - (4)].table), NULL, TL_OPTION_UPDATING,
                                           Lex.lock_option))
              DRIZZLE_YYABORT;
          }
    break;

  case 805:

/* Line 1455 of yacc.c  */
#line 4453 "drizzled/sql_yacc.yy"
    {}
    break;

  case 806:

/* Line 1455 of yacc.c  */
#line 4457 "drizzled/sql_yacc.yy"
    { Lex.ignore= 0; }
    break;

  case 807:

/* Line 1455 of yacc.c  */
#line 4458 "drizzled/sql_yacc.yy"
    { Lex.ignore= 1; }
    break;

  case 808:

/* Line 1455 of yacc.c  */
#line 4463 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::Truncate(YYSession);
            Lex.select_lex.options= 0;
            Lex.select_lex.init_order();
          }
    break;

  case 811:

/* Line 1455 of yacc.c  */
#line 4479 "drizzled/sql_yacc.yy"
    {
            Lex.lock_option= TL_READ;
            init_select(&Lex);
            Lex.current_select->parsing_place= SELECT_LIST;
          }
    break;

  case 812:

/* Line 1455 of yacc.c  */
#line 4485 "drizzled/sql_yacc.yy"
    {}
    break;

  case 813:

/* Line 1455 of yacc.c  */
#line 4491 "drizzled/sql_yacc.yy"
    {
             if (not show::buildScemas(YYSession))
               DRIZZLE_YYABORT;
           }
    break;

  case 814:

/* Line 1455 of yacc.c  */
#line 4497 "drizzled/sql_yacc.yy"
    {
             if (not show::buildTables(YYSession, (yyvsp[(2) - (3)].simple_string)))
               DRIZZLE_YYABORT;
           }
    break;

  case 815:

/* Line 1455 of yacc.c  */
#line 4503 "drizzled/sql_yacc.yy"
    {
             if (not show::buildTemporaryTables(YYSession))
               DRIZZLE_YYABORT;
           }
    break;

  case 816:

/* Line 1455 of yacc.c  */
#line 4509 "drizzled/sql_yacc.yy"
    {
             if (not show::buildTableStatus(YYSession, (yyvsp[(3) - (4)].simple_string)))
               DRIZZLE_YYABORT;
           }
    break;

  case 817:

/* Line 1455 of yacc.c  */
#line 4515 "drizzled/sql_yacc.yy"
    {
             if (not show::buildColumns(YYSession, (yyvsp[(4) - (5)].simple_string), (yyvsp[(3) - (5)].table)))
               DRIZZLE_YYABORT;
           }
    break;

  case 818:

/* Line 1455 of yacc.c  */
#line 4521 "drizzled/sql_yacc.yy"
    {
             if (not show::buildIndex(YYSession, (yyvsp[(4) - (5)].simple_string), (yyvsp[(3) - (5)].table)))
               DRIZZLE_YYABORT;
           }
    break;

  case 819:

/* Line 1455 of yacc.c  */
#line 4526 "drizzled/sql_yacc.yy"
    {
            show::buildSelectWarning(YYSession);
          }
    break;

  case 820:

/* Line 1455 of yacc.c  */
#line 4530 "drizzled/sql_yacc.yy"
    {
            show::buildSelectError(YYSession);
          }
    break;

  case 821:

/* Line 1455 of yacc.c  */
#line 4534 "drizzled/sql_yacc.yy"
    {
            show::buildWarnings(YYSession);
          }
    break;

  case 822:

/* Line 1455 of yacc.c  */
#line 4538 "drizzled/sql_yacc.yy"
    {
            show::buildErrors(YYSession);
          }
    break;

  case 823:

/* Line 1455 of yacc.c  */
#line 4542 "drizzled/sql_yacc.yy"
    {
            if (not show::buildStatus(YYSession, (yyvsp[(1) - (3)].var_type)))
              DRIZZLE_YYABORT;
          }
    break;

  case 824:

/* Line 1455 of yacc.c  */
#line 4547 "drizzled/sql_yacc.yy"
    {
            if (not show::buildEngineStatus(YYSession, (yyvsp[(1) - (2)].lex_str)))
              DRIZZLE_YYABORT;
          }
    break;

  case 825:

/* Line 1455 of yacc.c  */
#line 4552 "drizzled/sql_yacc.yy"
    {
            if (not show::buildCreateTable(YYSession, (yyvsp[(3) - (3)].table)))
              DRIZZLE_YYABORT;
          }
    break;

  case 826:

/* Line 1455 of yacc.c  */
#line 4557 "drizzled/sql_yacc.yy"
    {
            if (not show::buildProcesslist(YYSession))
              DRIZZLE_YYABORT;
          }
    break;

  case 827:

/* Line 1455 of yacc.c  */
#line 4562 "drizzled/sql_yacc.yy"
    {
            if (not show::buildVariables(YYSession, (yyvsp[(1) - (3)].var_type)))
              DRIZZLE_YYABORT;
          }
    break;

  case 828:

/* Line 1455 of yacc.c  */
#line 4567 "drizzled/sql_yacc.yy"
    {
            if (not show::buildCreateSchema(YYSession, (yyvsp[(4) - (4)].lex_str)))
              DRIZZLE_YYABORT;
          }
    break;

  case 829:

/* Line 1455 of yacc.c  */
#line 4573 "drizzled/sql_yacc.yy"
    { (yyval.simple_string)= 0; }
    break;

  case 830:

/* Line 1455 of yacc.c  */
#line 4574 "drizzled/sql_yacc.yy"
    { (yyval.simple_string)= (yyvsp[(2) - (2)].lex_str).str; }
    break;

  case 834:

/* Line 1455 of yacc.c  */
#line 4585 "drizzled/sql_yacc.yy"
    {
            Lex.wild= new (YYSession->mem_root) String((yyvsp[(2) - (2)].lex_str).str, (yyvsp[(2) - (2)].lex_str).length,
                                                    system_charset_info);
            if (Lex.wild == NULL)
              DRIZZLE_YYABORT;
          }
    break;

  case 835:

/* Line 1455 of yacc.c  */
#line 4592 "drizzled/sql_yacc.yy"
    {
            Lex.current_select->where= (yyvsp[(2) - (2)].item);
            if ((yyvsp[(2) - (2)].item))
              (yyvsp[(2) - (2)].item)->top_level_item();
          }
    break;

  case 836:

/* Line 1455 of yacc.c  */
#line 4602 "drizzled/sql_yacc.yy"
    {
            if (not show::buildDescribe(YYSession, (yyvsp[(2) - (2)].table)))
            {
              DRIZZLE_YYABORT;
            }
          }
    break;

  case 837:

/* Line 1455 of yacc.c  */
#line 4608 "drizzled/sql_yacc.yy"
    {}
    break;

  case 838:

/* Line 1455 of yacc.c  */
#line 4610 "drizzled/sql_yacc.yy"
    { Lex.describe|= DESCRIBE_NORMAL; }
    break;

  case 839:

/* Line 1455 of yacc.c  */
#line 4612 "drizzled/sql_yacc.yy"
    {
            Lex.select_lex.options|= SELECT_DESCRIBE;
          }
    break;

  case 842:

/* Line 1455 of yacc.c  */
#line 4623 "drizzled/sql_yacc.yy"
    {}
    break;

  case 843:

/* Line 1455 of yacc.c  */
#line 4624 "drizzled/sql_yacc.yy"
    { Lex.describe|= DESCRIBE_EXTENDED; }
    break;

  case 844:

/* Line 1455 of yacc.c  */
#line 4628 "drizzled/sql_yacc.yy"
    {}
    break;

  case 845:

/* Line 1455 of yacc.c  */
#line 4629 "drizzled/sql_yacc.yy"
    { Lex.wild= (yyvsp[(1) - (1)].string); }
    break;

  case 846:

/* Line 1455 of yacc.c  */
#line 4631 "drizzled/sql_yacc.yy"
    {
            Lex.wild= new (YYSession->mem_root) String((const char*) (yyvsp[(1) - (1)].lex_str).str,
                                                    (yyvsp[(1) - (1)].lex_str).length,
                                                    system_charset_info);
          }
    break;

  case 847:

/* Line 1455 of yacc.c  */
#line 4643 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::Flush(YYSession);
          }
    break;

  case 848:

/* Line 1455 of yacc.c  */
#line 4647 "drizzled/sql_yacc.yy"
    {}
    break;

  case 851:

/* Line 1455 of yacc.c  */
#line 4657 "drizzled/sql_yacc.yy"
    {
            statement::Flush *statement= (statement::Flush*)Lex.statement;
            statement->setFlushTables(true);
          }
    break;

  case 852:

/* Line 1455 of yacc.c  */
#line 4661 "drizzled/sql_yacc.yy"
    {}
    break;

  case 853:

/* Line 1455 of yacc.c  */
#line 4663 "drizzled/sql_yacc.yy"
    {
            statement::Flush *statement= (statement::Flush*)Lex.statement;
            statement->setFlushTablesWithReadLock(true);
          }
    break;

  case 854:

/* Line 1455 of yacc.c  */
#line 4668 "drizzled/sql_yacc.yy"
    {
            statement::Flush *statement= (statement::Flush*)Lex.statement;
            statement->setFlushLog(true);
          }
    break;

  case 855:

/* Line 1455 of yacc.c  */
#line 4673 "drizzled/sql_yacc.yy"
    {
            statement::Flush *statement= (statement::Flush*)Lex.statement;
            statement->setFlushStatus(true);
          }
    break;

  case 856:

/* Line 1455 of yacc.c  */
#line 4678 "drizzled/sql_yacc.yy"
    {
            statement::Flush *statement= (statement::Flush*)Lex.statement;
            statement->setFlushGlobalStatus(true);
          }
    break;

  case 857:

/* Line 1455 of yacc.c  */
#line 4685 "drizzled/sql_yacc.yy"
    {}
    break;

  case 858:

/* Line 1455 of yacc.c  */
#line 4686 "drizzled/sql_yacc.yy"
    {}
    break;

  case 859:

/* Line 1455 of yacc.c  */
#line 4693 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::Kill(YYSession, (yyvsp[(3) - (3)].item), (yyvsp[(2) - (3)].num));
          }
    break;

  case 860:

/* Line 1455 of yacc.c  */
#line 4699 "drizzled/sql_yacc.yy"
    { (yyval.num)= false; }
    break;

  case 861:

/* Line 1455 of yacc.c  */
#line 4700 "drizzled/sql_yacc.yy"
    { (yyval.num)= false; }
    break;

  case 862:

/* Line 1455 of yacc.c  */
#line 4701 "drizzled/sql_yacc.yy"
    { (yyval.num)= true; }
    break;

  case 863:

/* Line 1455 of yacc.c  */
#line 4708 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::ChangeSchema(YYSession);
            Lex.select_lex.db= (yyvsp[(2) - (2)].lex_str).str;
          }
    break;

  case 864:

/* Line 1455 of yacc.c  */
#line 4718 "drizzled/sql_yacc.yy"
    {
            statement::Load *statement= new statement::Load(YYSession);
            Lex.statement= statement;

            Lex_input_stream *lip= YYSession->m_lip;
            statement->fname_start= lip->get_ptr();
          }
    break;

  case 865:

/* Line 1455 of yacc.c  */
#line 4726 "drizzled/sql_yacc.yy"
    {
            Lex.lock_option= (yyvsp[(4) - (6)].lock_type);
            Lex.duplicates= DUP_ERROR;
            Lex.ignore= 0;
            if (not (Lex.exchange= new file_exchange((yyvsp[(6) - (6)].lex_str).str, 0, (yyvsp[(2) - (6)].filetype))))
              DRIZZLE_YYABORT;
          }
    break;

  case 866:

/* Line 1455 of yacc.c  */
#line 4734 "drizzled/sql_yacc.yy"
    {
            Lex_input_stream *lip= YYSession->m_lip;
            ((statement::Load *)Lex.statement)->fname_end= lip->get_ptr();
          }
    break;

  case 867:

/* Line 1455 of yacc.c  */
#line 4739 "drizzled/sql_yacc.yy"
    {
            if (!Lex.current_select->add_table_to_list(YYSession,
                    (yyvsp[(12) - (12)].table), NULL, TL_OPTION_UPDATING,
                    Lex.lock_option))
              DRIZZLE_YYABORT;
            Lex.field_list.clear();
            Lex.update_list.clear();
            Lex.value_list.clear();
          }
    break;

  case 868:

/* Line 1455 of yacc.c  */
#line 4750 "drizzled/sql_yacc.yy"
    {}
    break;

  case 869:

/* Line 1455 of yacc.c  */
#line 4754 "drizzled/sql_yacc.yy"
    { (yyval.filetype)= FILETYPE_CSV; }
    break;

  case 870:

/* Line 1455 of yacc.c  */
#line 4757 "drizzled/sql_yacc.yy"
    { (yyval.lock_type)= TL_WRITE_DEFAULT; }
    break;

  case 871:

/* Line 1455 of yacc.c  */
#line 4759 "drizzled/sql_yacc.yy"
    {
              (yyval.lock_type)= TL_WRITE_CONCURRENT_INSERT;
          }
    break;

  case 872:

/* Line 1455 of yacc.c  */
#line 4765 "drizzled/sql_yacc.yy"
    { Lex.duplicates=DUP_ERROR; }
    break;

  case 873:

/* Line 1455 of yacc.c  */
#line 4766 "drizzled/sql_yacc.yy"
    { Lex.duplicates=DUP_REPLACE; }
    break;

  case 874:

/* Line 1455 of yacc.c  */
#line 4767 "drizzled/sql_yacc.yy"
    { Lex.ignore= 1; }
    break;

  case 875:

/* Line 1455 of yacc.c  */
#line 4771 "drizzled/sql_yacc.yy"
    { Lex.duplicates=DUP_ERROR; }
    break;

  case 876:

/* Line 1455 of yacc.c  */
#line 4772 "drizzled/sql_yacc.yy"
    { Lex.duplicates=DUP_ERROR; }
    break;

  case 877:

/* Line 1455 of yacc.c  */
#line 4773 "drizzled/sql_yacc.yy"
    { Lex.duplicates=DUP_REPLACE; }
    break;

  case 878:

/* Line 1455 of yacc.c  */
#line 4774 "drizzled/sql_yacc.yy"
    { Lex.ignore= true; }
    break;

  case 879:

/* Line 1455 of yacc.c  */
#line 4775 "drizzled/sql_yacc.yy"
    { Lex.duplicates=DUP_REPLACE; }
    break;

  case 880:

/* Line 1455 of yacc.c  */
#line 4776 "drizzled/sql_yacc.yy"
    { Lex.ignore= true; }
    break;

  case 885:

/* Line 1455 of yacc.c  */
#line 4791 "drizzled/sql_yacc.yy"
    {
            assert(Lex.exchange != 0);
            Lex.exchange->field_term= (yyvsp[(3) - (3)].string);
          }
    break;

  case 886:

/* Line 1455 of yacc.c  */
#line 4796 "drizzled/sql_yacc.yy"
    {
            assert(Lex.exchange != 0);
            Lex.exchange->enclosed= (yyvsp[(4) - (4)].string);
            Lex.exchange->opt_enclosed= 1;
          }
    break;

  case 887:

/* Line 1455 of yacc.c  */
#line 4802 "drizzled/sql_yacc.yy"
    {
            assert(Lex.exchange != 0);
            Lex.exchange->enclosed= (yyvsp[(3) - (3)].string);
          }
    break;

  case 888:

/* Line 1455 of yacc.c  */
#line 4807 "drizzled/sql_yacc.yy"
    {
            assert(Lex.exchange != 0);
            Lex.exchange->escaped= (yyvsp[(3) - (3)].string);
          }
    break;

  case 893:

/* Line 1455 of yacc.c  */
#line 4825 "drizzled/sql_yacc.yy"
    {
            assert(Lex.exchange != 0);
            Lex.exchange->line_term= (yyvsp[(3) - (3)].string);
          }
    break;

  case 894:

/* Line 1455 of yacc.c  */
#line 4830 "drizzled/sql_yacc.yy"
    {
            assert(Lex.exchange != 0);
            Lex.exchange->line_start= (yyvsp[(3) - (3)].string);
          }
    break;

  case 896:

/* Line 1455 of yacc.c  */
#line 4839 "drizzled/sql_yacc.yy"
    {
            assert(Lex.exchange != 0);
            Lex.exchange->skip_lines= atol((yyvsp[(2) - (3)].lex_str).str);
          }
    break;

  case 897:

/* Line 1455 of yacc.c  */
#line 4846 "drizzled/sql_yacc.yy"
    { }
    break;

  case 898:

/* Line 1455 of yacc.c  */
#line 4847 "drizzled/sql_yacc.yy"
    { }
    break;

  case 899:

/* Line 1455 of yacc.c  */
#line 4851 "drizzled/sql_yacc.yy"
    {}
    break;

  case 900:

/* Line 1455 of yacc.c  */
#line 4852 "drizzled/sql_yacc.yy"
    {}
    break;

  case 901:

/* Line 1455 of yacc.c  */
#line 4853 "drizzled/sql_yacc.yy"
    {}
    break;

  case 902:

/* Line 1455 of yacc.c  */
#line 4858 "drizzled/sql_yacc.yy"
    { Lex.field_list.push_back((yyvsp[(3) - (3)].item)); }
    break;

  case 903:

/* Line 1455 of yacc.c  */
#line 4860 "drizzled/sql_yacc.yy"
    { Lex.field_list.push_back((yyvsp[(1) - (1)].item)); }
    break;

  case 904:

/* Line 1455 of yacc.c  */
#line 4864 "drizzled/sql_yacc.yy"
    {(yyval.item)= (yyvsp[(1) - (1)].item);}
    break;

  case 905:

/* Line 1455 of yacc.c  */
#line 4866 "drizzled/sql_yacc.yy"
    { (yyval.item)= new Item_user_var_as_out_param((yyvsp[(2) - (2)].lex_str)); }
    break;

  case 906:

/* Line 1455 of yacc.c  */
#line 4870 "drizzled/sql_yacc.yy"
    {}
    break;

  case 907:

/* Line 1455 of yacc.c  */
#line 4871 "drizzled/sql_yacc.yy"
    {}
    break;

  case 908:

/* Line 1455 of yacc.c  */
#line 4878 "drizzled/sql_yacc.yy"
    {
          (yyval.item) = new Item_string((yyvsp[(1) - (1)].lex_str).str, (yyvsp[(1) - (1)].lex_str).length, YYSession->variables.getCollation());
        }
    break;

  case 909:

/* Line 1455 of yacc.c  */
#line 4882 "drizzled/sql_yacc.yy"
    {
            ((Item_string*) (yyvsp[(1) - (2)].item))->append((yyvsp[(2) - (2)].lex_str).str, (yyvsp[(2) - (2)].lex_str).length);
          }
    break;

  case 910:

/* Line 1455 of yacc.c  */
#line 4889 "drizzled/sql_yacc.yy"
    {
            (yyval.string)= new (YYSession->mem_root) String((yyvsp[(1) - (1)].lex_str).str,
                                             (yyvsp[(1) - (1)].lex_str).length,
                                             YYSession->variables.getCollation());
          }
    break;

  case 911:

/* Line 1455 of yacc.c  */
#line 4895 "drizzled/sql_yacc.yy"
    {
            Item *tmp= new Item_hex_string((yyvsp[(1) - (1)].lex_str).str, (yyvsp[(1) - (1)].lex_str).length);
            /*
              it is OK only emulate fix_fields, because we need only
              value of constant
            */
            (yyval.string)= tmp ?
              tmp->quick_fix_field(), tmp->val_str((String*) 0) :
              (String*) 0;
          }
    break;

  case 912:

/* Line 1455 of yacc.c  */
#line 4906 "drizzled/sql_yacc.yy"
    {
            Item *tmp= new Item_bin_string((yyvsp[(1) - (1)].lex_str).str, (yyvsp[(1) - (1)].lex_str).length);
            /*
              it is OK only emulate fix_fields, because we need only
              value of constant
            */
            (yyval.string)= tmp ? tmp->quick_fix_field(), tmp->val_str((String*) 0) :
              (String*) 0;
          }
    break;

  case 913:

/* Line 1455 of yacc.c  */
#line 4918 "drizzled/sql_yacc.yy"
    { (yyval.item) = (yyvsp[(1) - (1)].item); }
    break;

  case 914:

/* Line 1455 of yacc.c  */
#line 4919 "drizzled/sql_yacc.yy"
    { (yyval.item) = (yyvsp[(2) - (2)].item_num); }
    break;

  case 915:

/* Line 1455 of yacc.c  */
#line 4921 "drizzled/sql_yacc.yy"
    {
            (yyvsp[(2) - (2)].item_num)->max_length++;
            (yyval.item)= (yyvsp[(2) - (2)].item_num)->neg();
          }
    break;

  case 916:

/* Line 1455 of yacc.c  */
#line 4928 "drizzled/sql_yacc.yy"
    { (yyval.item) = (yyvsp[(1) - (1)].item); }
    break;

  case 917:

/* Line 1455 of yacc.c  */
#line 4929 "drizzled/sql_yacc.yy"
    { (yyval.item) = (yyvsp[(1) - (1)].item_num); }
    break;

  case 918:

/* Line 1455 of yacc.c  */
#line 4931 "drizzled/sql_yacc.yy"
    {
            (yyval.item) = new Item_null();
            YYSession->m_lip->next_state=MY_LEX_OPERATOR_OR_IDENT;
          }
    break;

  case 919:

/* Line 1455 of yacc.c  */
#line 4935 "drizzled/sql_yacc.yy"
    { (yyval.item)= new drizzled::item::False(); }
    break;

  case 920:

/* Line 1455 of yacc.c  */
#line 4936 "drizzled/sql_yacc.yy"
    { (yyval.item)= new drizzled::item::True(); }
    break;

  case 921:

/* Line 1455 of yacc.c  */
#line 4937 "drizzled/sql_yacc.yy"
    { (yyval.item) = new Item_hex_string((yyvsp[(1) - (1)].lex_str).str, (yyvsp[(1) - (1)].lex_str).length);}
    break;

  case 922:

/* Line 1455 of yacc.c  */
#line 4938 "drizzled/sql_yacc.yy"
    { (yyval.item)= new Item_bin_string((yyvsp[(1) - (1)].lex_str).str, (yyvsp[(1) - (1)].lex_str).length); }
    break;

  case 923:

/* Line 1455 of yacc.c  */
#line 4939 "drizzled/sql_yacc.yy"
    { (yyval.item) = (yyvsp[(2) - (2)].item); }
    break;

  case 924:

/* Line 1455 of yacc.c  */
#line 4940 "drizzled/sql_yacc.yy"
    { (yyval.item) = (yyvsp[(2) - (2)].item); }
    break;

  case 925:

/* Line 1455 of yacc.c  */
#line 4944 "drizzled/sql_yacc.yy"
    { (yyval.item) = (yyvsp[(1) - (1)].item); }
    break;

  case 926:

/* Line 1455 of yacc.c  */
#line 4945 "drizzled/sql_yacc.yy"
    { (yyval.item) = new Item_hex_string((yyvsp[(1) - (1)].lex_str).str, (yyvsp[(1) - (1)].lex_str).length);}
    break;

  case 927:

/* Line 1455 of yacc.c  */
#line 4946 "drizzled/sql_yacc.yy"
    { (yyval.item)= new Item_bin_string((yyvsp[(1) - (1)].lex_str).str, (yyvsp[(1) - (1)].lex_str).length); }
    break;

  case 928:

/* Line 1455 of yacc.c  */
#line 4947 "drizzled/sql_yacc.yy"
    { (yyval.item) = (yyvsp[(1) - (1)].item_num); }
    break;

  case 929:

/* Line 1455 of yacc.c  */
#line 4949 "drizzled/sql_yacc.yy"
    {
            (yyval.item) = new Item_null();
            YYSession->m_lip->next_state=MY_LEX_OPERATOR_OR_IDENT;
          }
    break;

  case 930:

/* Line 1455 of yacc.c  */
#line 4953 "drizzled/sql_yacc.yy"
    { (yyval.item) = (yyvsp[(2) - (2)].item_num); }
    break;

  case 931:

/* Line 1455 of yacc.c  */
#line 4955 "drizzled/sql_yacc.yy"
    {
            (yyvsp[(2) - (2)].item_num)->max_length++;
            (yyval.item)= (yyvsp[(2) - (2)].item_num)->neg();
          }
    break;

  case 932:

/* Line 1455 of yacc.c  */
#line 4963 "drizzled/sql_yacc.yy"
    {
            (yyval.item) = new Item_null();
            YYSession->m_lip->next_state=MY_LEX_OPERATOR_OR_IDENT;
          }
    break;

  case 933:

/* Line 1455 of yacc.c  */
#line 4968 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new drizzled::item::False();
          }
    break;

  case 934:

/* Line 1455 of yacc.c  */
#line 4972 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= new drizzled::item::True();
          }
    break;

  case 935:

/* Line 1455 of yacc.c  */
#line 4978 "drizzled/sql_yacc.yy"
    { (yyval.item) = (yyvsp[(1) - (1)].item); }
    break;

  case 936:

/* Line 1455 of yacc.c  */
#line 4980 "drizzled/sql_yacc.yy"
    {
            (yyval.item) = new Item_null();
            YYSession->m_lip->next_state=MY_LEX_OPERATOR_OR_IDENT;
          }
    break;

  case 937:

/* Line 1455 of yacc.c  */
#line 4984 "drizzled/sql_yacc.yy"
    { (yyval.item) = (yyvsp[(1) - (1)].item_num); }
    break;

  case 938:

/* Line 1455 of yacc.c  */
#line 4985 "drizzled/sql_yacc.yy"
    { (yyval.item) = (yyvsp[(2) - (2)].item); }
    break;

  case 939:

/* Line 1455 of yacc.c  */
#line 4986 "drizzled/sql_yacc.yy"
    { (yyval.item) = (yyvsp[(2) - (2)].item); }
    break;

  case 940:

/* Line 1455 of yacc.c  */
#line 4991 "drizzled/sql_yacc.yy"
    {
            int error;
            (yyval.item_num) = new Item_int((yyvsp[(1) - (1)].lex_str).str, (int64_t) internal::my_strtoll10((yyvsp[(1) - (1)].lex_str).str, NULL, &error), (yyvsp[(1) - (1)].lex_str).length);
          }
    break;

  case 941:

/* Line 1455 of yacc.c  */
#line 4996 "drizzled/sql_yacc.yy"
    {
            int error;
            (yyval.item_num) = new Item_int((yyvsp[(1) - (1)].lex_str).str, (int64_t) internal::my_strtoll10((yyvsp[(1) - (1)].lex_str).str, NULL, &error), (yyvsp[(1) - (1)].lex_str).length);
          }
    break;

  case 942:

/* Line 1455 of yacc.c  */
#line 5001 "drizzled/sql_yacc.yy"
    { (yyval.item_num) = new Item_uint((yyvsp[(1) - (1)].lex_str).str, (yyvsp[(1) - (1)].lex_str).length); }
    break;

  case 943:

/* Line 1455 of yacc.c  */
#line 5003 "drizzled/sql_yacc.yy"
    {
            (yyval.item_num)= new Item_decimal((yyvsp[(1) - (1)].lex_str).str, (yyvsp[(1) - (1)].lex_str).length, YYSession->charset());
            if (YYSession->is_error())
            {
              DRIZZLE_YYABORT;
            }
          }
    break;

  case 944:

/* Line 1455 of yacc.c  */
#line 5011 "drizzled/sql_yacc.yy"
    {
            (yyval.item_num) = new Item_float((yyvsp[(1) - (1)].lex_str).str, (yyvsp[(1) - (1)].lex_str).length);
            if (YYSession->is_error())
            {
              DRIZZLE_YYABORT;
            }
          }
    break;

  case 945:

/* Line 1455 of yacc.c  */
#line 5025 "drizzled/sql_yacc.yy"
    { (yyval.item)=(yyvsp[(1) - (1)].item); }
    break;

  case 946:

/* Line 1455 of yacc.c  */
#line 5026 "drizzled/sql_yacc.yy"
    { (yyval.item)=(yyvsp[(1) - (1)].item); }
    break;

  case 947:

/* Line 1455 of yacc.c  */
#line 5031 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= parser::buildTableWild(&Lex, NULL_LEX_STRING, (yyvsp[(1) - (3)].lex_str));
          }
    break;

  case 948:

/* Line 1455 of yacc.c  */
#line 5035 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= parser::buildTableWild(&Lex, (yyvsp[(1) - (5)].lex_str), (yyvsp[(3) - (5)].lex_str));
          }
    break;

  case 949:

/* Line 1455 of yacc.c  */
#line 5041 "drizzled/sql_yacc.yy"
    { (yyval.item)=(yyvsp[(1) - (1)].item); }
    break;

  case 950:

/* Line 1455 of yacc.c  */
#line 5046 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= parser::buildIdent(&Lex, NULL_LEX_STRING, NULL_LEX_STRING, (yyvsp[(1) - (1)].lex_str));
          }
    break;

  case 951:

/* Line 1455 of yacc.c  */
#line 5049 "drizzled/sql_yacc.yy"
    { (yyval.item)= (yyvsp[(1) - (1)].item); }
    break;

  case 952:

/* Line 1455 of yacc.c  */
#line 5054 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= parser::buildIdent(&Lex, NULL_LEX_STRING, (yyvsp[(1) - (3)].lex_str), (yyvsp[(3) - (3)].lex_str));
          }
    break;

  case 953:

/* Line 1455 of yacc.c  */
#line 5058 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= parser::buildIdent(&Lex, NULL_LEX_STRING, (yyvsp[(2) - (4)].lex_str), (yyvsp[(4) - (4)].lex_str));
          }
    break;

  case 954:

/* Line 1455 of yacc.c  */
#line 5062 "drizzled/sql_yacc.yy"
    {
            (yyval.item)= parser::buildIdent(&Lex, (yyvsp[(1) - (5)].lex_str), (yyvsp[(3) - (5)].lex_str), (yyvsp[(5) - (5)].lex_str));
          }
    break;

  case 955:

/* Line 1455 of yacc.c  */
#line 5069 "drizzled/sql_yacc.yy"
    {
            (yyval.lex_str)=(yyvsp[(1) - (1)].lex_str);
          }
    break;

  case 956:

/* Line 1455 of yacc.c  */
#line 5073 "drizzled/sql_yacc.yy"
    {
            if (not parser::checkFieldIdent(&Lex, (yyvsp[(1) - (5)].lex_str), (yyvsp[(3) - (5)].lex_str)))
              DRIZZLE_YYABORT;

            (yyval.lex_str)=(yyvsp[(5) - (5)].lex_str);
          }
    break;

  case 957:

/* Line 1455 of yacc.c  */
#line 5080 "drizzled/sql_yacc.yy"
    {
            if (not parser::checkFieldIdent(&Lex, NULL_LEX_STRING, (yyvsp[(1) - (3)].lex_str)))
              DRIZZLE_YYABORT;

            (yyval.lex_str)=(yyvsp[(3) - (3)].lex_str);
          }
    break;

  case 958:

/* Line 1455 of yacc.c  */
#line 5087 "drizzled/sql_yacc.yy"
    { /* For Delphi */
            (yyval.lex_str)=(yyvsp[(2) - (2)].lex_str);
          }
    break;

  case 959:

/* Line 1455 of yacc.c  */
#line 5094 "drizzled/sql_yacc.yy"
    {
            (yyval.table)= new Table_ident((yyvsp[(1) - (1)].lex_str));
          }
    break;

  case 960:

/* Line 1455 of yacc.c  */
#line 5098 "drizzled/sql_yacc.yy"
    {
            (yyval.table)=new Table_ident((yyvsp[(1) - (3)].lex_str),(yyvsp[(3) - (3)].lex_str));
          }
    break;

  case 961:

/* Line 1455 of yacc.c  */
#line 5102 "drizzled/sql_yacc.yy"
    { /* For Delphi */
          (yyval.table)= new Table_ident((yyvsp[(2) - (2)].lex_str));
        }
    break;

  case 964:

/* Line 1455 of yacc.c  */
#line 5117 "drizzled/sql_yacc.yy"
    {
            (yyval.lex_str)= (yyvsp[(1) - (1)].lex_str);
          }
    break;

  case 965:

/* Line 1455 of yacc.c  */
#line 5121 "drizzled/sql_yacc.yy"
    {
            const CHARSET_INFO * const cs= system_charset_info;
            int dummy_error;
            uint32_t wlen= cs->cset->well_formed_len(cs, (yyvsp[(1) - (1)].lex_str).str,
                                                 (yyvsp[(1) - (1)].lex_str).str+(yyvsp[(1) - (1)].lex_str).length,
                                                 (yyvsp[(1) - (1)].lex_str).length, &dummy_error);
            if (wlen < (yyvsp[(1) - (1)].lex_str).length)
            {
              my_error(ER_INVALID_CHARACTER_STRING, MYF(0),
                       cs->csname, (yyvsp[(1) - (1)].lex_str).str + wlen);
              DRIZZLE_YYABORT;
            }
            (yyval.lex_str)= (yyvsp[(1) - (1)].lex_str);
          }
    break;

  case 966:

/* Line 1455 of yacc.c  */
#line 5139 "drizzled/sql_yacc.yy"
    {
            (yyval.lex_str)= (yyvsp[(1) - (1)].lex_str);
          }
    break;

  case 967:

/* Line 1455 of yacc.c  */
#line 5146 "drizzled/sql_yacc.yy"
    {
            (yyval.lex_str)= (yyvsp[(1) - (1)].lex_str);
          }
    break;

  case 968:

/* Line 1455 of yacc.c  */
#line 5153 "drizzled/sql_yacc.yy"
    {
            (yyval.lex_str)= (yyvsp[(1) - (1)].lex_str);
          }
    break;

  case 969:

/* Line 1455 of yacc.c  */
#line 5159 "drizzled/sql_yacc.yy"
    { (yyval.lex_str)=(yyvsp[(1) - (1)].lex_str); }
    break;

  case 970:

/* Line 1455 of yacc.c  */
#line 5161 "drizzled/sql_yacc.yy"
    {
            (yyval.lex_str).str= YYSession->strmake((yyvsp[(1) - (1)].symbol).str, (yyvsp[(1) - (1)].symbol).length);
            (yyval.lex_str).length= (yyvsp[(1) - (1)].symbol).length;
          }
    break;

  case 971:

/* Line 1455 of yacc.c  */
#line 5168 "drizzled/sql_yacc.yy"
    { (yyval.lex_str)=(yyvsp[(1) - (1)].lex_str);}
    break;

  case 972:

/* Line 1455 of yacc.c  */
#line 5169 "drizzled/sql_yacc.yy"
    { (yyval.lex_str)=(yyvsp[(1) - (1)].lex_str);}
    break;

  case 973:

/* Line 1455 of yacc.c  */
#line 5173 "drizzled/sql_yacc.yy"
    { (yyval.lex_str)=(yyvsp[(1) - (1)].lex_str);}
    break;

  case 974:

/* Line 1455 of yacc.c  */
#line 5174 "drizzled/sql_yacc.yy"
    { (yyval.lex_str)=(yyvsp[(1) - (1)].lex_str);}
    break;

  case 975:

/* Line 1455 of yacc.c  */
#line 5178 "drizzled/sql_yacc.yy"
    {}
    break;

  case 976:

/* Line 1455 of yacc.c  */
#line 5179 "drizzled/sql_yacc.yy"
    {}
    break;

  case 977:

/* Line 1455 of yacc.c  */
#line 5180 "drizzled/sql_yacc.yy"
    {}
    break;

  case 978:

/* Line 1455 of yacc.c  */
#line 5185 "drizzled/sql_yacc.yy"
    {}
    break;

  case 979:

/* Line 1455 of yacc.c  */
#line 5186 "drizzled/sql_yacc.yy"
    {}
    break;

  case 980:

/* Line 1455 of yacc.c  */
#line 5187 "drizzled/sql_yacc.yy"
    {}
    break;

  case 981:

/* Line 1455 of yacc.c  */
#line 5188 "drizzled/sql_yacc.yy"
    {}
    break;

  case 982:

/* Line 1455 of yacc.c  */
#line 5189 "drizzled/sql_yacc.yy"
    {}
    break;

  case 983:

/* Line 1455 of yacc.c  */
#line 5190 "drizzled/sql_yacc.yy"
    {}
    break;

  case 984:

/* Line 1455 of yacc.c  */
#line 5191 "drizzled/sql_yacc.yy"
    {}
    break;

  case 985:

/* Line 1455 of yacc.c  */
#line 5192 "drizzled/sql_yacc.yy"
    {}
    break;

  case 986:

/* Line 1455 of yacc.c  */
#line 5193 "drizzled/sql_yacc.yy"
    {}
    break;

  case 987:

/* Line 1455 of yacc.c  */
#line 5194 "drizzled/sql_yacc.yy"
    {}
    break;

  case 988:

/* Line 1455 of yacc.c  */
#line 5195 "drizzled/sql_yacc.yy"
    {}
    break;

  case 989:

/* Line 1455 of yacc.c  */
#line 5196 "drizzled/sql_yacc.yy"
    {}
    break;

  case 990:

/* Line 1455 of yacc.c  */
#line 5197 "drizzled/sql_yacc.yy"
    {}
    break;

  case 991:

/* Line 1455 of yacc.c  */
#line 5198 "drizzled/sql_yacc.yy"
    {}
    break;

  case 992:

/* Line 1455 of yacc.c  */
#line 5199 "drizzled/sql_yacc.yy"
    {}
    break;

  case 993:

/* Line 1455 of yacc.c  */
#line 5200 "drizzled/sql_yacc.yy"
    {}
    break;

  case 994:

/* Line 1455 of yacc.c  */
#line 5201 "drizzled/sql_yacc.yy"
    {}
    break;

  case 995:

/* Line 1455 of yacc.c  */
#line 5202 "drizzled/sql_yacc.yy"
    {}
    break;

  case 996:

/* Line 1455 of yacc.c  */
#line 5203 "drizzled/sql_yacc.yy"
    {}
    break;

  case 997:

/* Line 1455 of yacc.c  */
#line 5204 "drizzled/sql_yacc.yy"
    {}
    break;

  case 998:

/* Line 1455 of yacc.c  */
#line 5205 "drizzled/sql_yacc.yy"
    {}
    break;

  case 999:

/* Line 1455 of yacc.c  */
#line 5215 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1000:

/* Line 1455 of yacc.c  */
#line 5216 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1001:

/* Line 1455 of yacc.c  */
#line 5217 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1002:

/* Line 1455 of yacc.c  */
#line 5218 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1003:

/* Line 1455 of yacc.c  */
#line 5219 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1004:

/* Line 1455 of yacc.c  */
#line 5220 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1005:

/* Line 1455 of yacc.c  */
#line 5221 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1006:

/* Line 1455 of yacc.c  */
#line 5222 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1007:

/* Line 1455 of yacc.c  */
#line 5223 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1008:

/* Line 1455 of yacc.c  */
#line 5224 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1009:

/* Line 1455 of yacc.c  */
#line 5225 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1010:

/* Line 1455 of yacc.c  */
#line 5226 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1011:

/* Line 1455 of yacc.c  */
#line 5227 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1012:

/* Line 1455 of yacc.c  */
#line 5228 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1013:

/* Line 1455 of yacc.c  */
#line 5229 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1014:

/* Line 1455 of yacc.c  */
#line 5230 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1015:

/* Line 1455 of yacc.c  */
#line 5231 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1016:

/* Line 1455 of yacc.c  */
#line 5232 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1017:

/* Line 1455 of yacc.c  */
#line 5233 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1018:

/* Line 1455 of yacc.c  */
#line 5234 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1019:

/* Line 1455 of yacc.c  */
#line 5235 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1020:

/* Line 1455 of yacc.c  */
#line 5236 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1021:

/* Line 1455 of yacc.c  */
#line 5237 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1022:

/* Line 1455 of yacc.c  */
#line 5238 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1023:

/* Line 1455 of yacc.c  */
#line 5239 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1024:

/* Line 1455 of yacc.c  */
#line 5240 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1025:

/* Line 1455 of yacc.c  */
#line 5241 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1026:

/* Line 1455 of yacc.c  */
#line 5242 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1027:

/* Line 1455 of yacc.c  */
#line 5243 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1028:

/* Line 1455 of yacc.c  */
#line 5244 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1029:

/* Line 1455 of yacc.c  */
#line 5245 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1030:

/* Line 1455 of yacc.c  */
#line 5246 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1031:

/* Line 1455 of yacc.c  */
#line 5247 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1032:

/* Line 1455 of yacc.c  */
#line 5248 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1033:

/* Line 1455 of yacc.c  */
#line 5249 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1034:

/* Line 1455 of yacc.c  */
#line 5250 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1035:

/* Line 1455 of yacc.c  */
#line 5251 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1036:

/* Line 1455 of yacc.c  */
#line 5252 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1037:

/* Line 1455 of yacc.c  */
#line 5253 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1038:

/* Line 1455 of yacc.c  */
#line 5254 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1039:

/* Line 1455 of yacc.c  */
#line 5255 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1040:

/* Line 1455 of yacc.c  */
#line 5256 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1041:

/* Line 1455 of yacc.c  */
#line 5257 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1042:

/* Line 1455 of yacc.c  */
#line 5258 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1043:

/* Line 1455 of yacc.c  */
#line 5259 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1044:

/* Line 1455 of yacc.c  */
#line 5260 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1045:

/* Line 1455 of yacc.c  */
#line 5261 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1046:

/* Line 1455 of yacc.c  */
#line 5262 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1047:

/* Line 1455 of yacc.c  */
#line 5263 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1048:

/* Line 1455 of yacc.c  */
#line 5264 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1049:

/* Line 1455 of yacc.c  */
#line 5265 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1050:

/* Line 1455 of yacc.c  */
#line 5266 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1051:

/* Line 1455 of yacc.c  */
#line 5267 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1052:

/* Line 1455 of yacc.c  */
#line 5268 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1053:

/* Line 1455 of yacc.c  */
#line 5269 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1054:

/* Line 1455 of yacc.c  */
#line 5270 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1055:

/* Line 1455 of yacc.c  */
#line 5271 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1056:

/* Line 1455 of yacc.c  */
#line 5272 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1057:

/* Line 1455 of yacc.c  */
#line 5273 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1058:

/* Line 1455 of yacc.c  */
#line 5274 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1059:

/* Line 1455 of yacc.c  */
#line 5275 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1060:

/* Line 1455 of yacc.c  */
#line 5276 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1061:

/* Line 1455 of yacc.c  */
#line 5277 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1062:

/* Line 1455 of yacc.c  */
#line 5278 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1063:

/* Line 1455 of yacc.c  */
#line 5279 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1064:

/* Line 1455 of yacc.c  */
#line 5280 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1065:

/* Line 1455 of yacc.c  */
#line 5281 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1066:

/* Line 1455 of yacc.c  */
#line 5282 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1067:

/* Line 1455 of yacc.c  */
#line 5283 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1068:

/* Line 1455 of yacc.c  */
#line 5284 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1069:

/* Line 1455 of yacc.c  */
#line 5285 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1070:

/* Line 1455 of yacc.c  */
#line 5286 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1071:

/* Line 1455 of yacc.c  */
#line 5287 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1072:

/* Line 1455 of yacc.c  */
#line 5288 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1073:

/* Line 1455 of yacc.c  */
#line 5289 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1074:

/* Line 1455 of yacc.c  */
#line 5290 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1075:

/* Line 1455 of yacc.c  */
#line 5291 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1076:

/* Line 1455 of yacc.c  */
#line 5292 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1077:

/* Line 1455 of yacc.c  */
#line 5293 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1078:

/* Line 1455 of yacc.c  */
#line 5294 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1079:

/* Line 1455 of yacc.c  */
#line 5295 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1080:

/* Line 1455 of yacc.c  */
#line 5296 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1081:

/* Line 1455 of yacc.c  */
#line 5297 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1082:

/* Line 1455 of yacc.c  */
#line 5298 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1083:

/* Line 1455 of yacc.c  */
#line 5299 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1084:

/* Line 1455 of yacc.c  */
#line 5300 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1085:

/* Line 1455 of yacc.c  */
#line 5301 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1086:

/* Line 1455 of yacc.c  */
#line 5302 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1087:

/* Line 1455 of yacc.c  */
#line 5303 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1088:

/* Line 1455 of yacc.c  */
#line 5304 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1089:

/* Line 1455 of yacc.c  */
#line 5305 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1090:

/* Line 1455 of yacc.c  */
#line 5306 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1091:

/* Line 1455 of yacc.c  */
#line 5307 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1092:

/* Line 1455 of yacc.c  */
#line 5308 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1093:

/* Line 1455 of yacc.c  */
#line 5309 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1094:

/* Line 1455 of yacc.c  */
#line 5310 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1095:

/* Line 1455 of yacc.c  */
#line 5311 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1096:

/* Line 1455 of yacc.c  */
#line 5312 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1097:

/* Line 1455 of yacc.c  */
#line 5313 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1098:

/* Line 1455 of yacc.c  */
#line 5314 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1099:

/* Line 1455 of yacc.c  */
#line 5315 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1100:

/* Line 1455 of yacc.c  */
#line 5316 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1101:

/* Line 1455 of yacc.c  */
#line 5317 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1102:

/* Line 1455 of yacc.c  */
#line 5318 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1103:

/* Line 1455 of yacc.c  */
#line 5319 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1104:

/* Line 1455 of yacc.c  */
#line 5320 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1105:

/* Line 1455 of yacc.c  */
#line 5321 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1106:

/* Line 1455 of yacc.c  */
#line 5322 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1107:

/* Line 1455 of yacc.c  */
#line 5323 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1108:

/* Line 1455 of yacc.c  */
#line 5324 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1109:

/* Line 1455 of yacc.c  */
#line 5325 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1110:

/* Line 1455 of yacc.c  */
#line 5326 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1111:

/* Line 1455 of yacc.c  */
#line 5327 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1112:

/* Line 1455 of yacc.c  */
#line 5328 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1113:

/* Line 1455 of yacc.c  */
#line 5329 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1114:

/* Line 1455 of yacc.c  */
#line 5330 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1115:

/* Line 1455 of yacc.c  */
#line 5331 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1116:

/* Line 1455 of yacc.c  */
#line 5332 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1117:

/* Line 1455 of yacc.c  */
#line 5333 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1118:

/* Line 1455 of yacc.c  */
#line 5334 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1119:

/* Line 1455 of yacc.c  */
#line 5335 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1120:

/* Line 1455 of yacc.c  */
#line 5336 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1121:

/* Line 1455 of yacc.c  */
#line 5337 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1122:

/* Line 1455 of yacc.c  */
#line 5338 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1123:

/* Line 1455 of yacc.c  */
#line 5339 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1124:

/* Line 1455 of yacc.c  */
#line 5340 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1125:

/* Line 1455 of yacc.c  */
#line 5347 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::SetOption(YYSession);
          }
    break;

  case 1126:

/* Line 1455 of yacc.c  */
#line 5351 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1127:

/* Line 1455 of yacc.c  */
#line 5355 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1128:

/* Line 1455 of yacc.c  */
#line 5356 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1131:

/* Line 1455 of yacc.c  */
#line 5365 "drizzled/sql_yacc.yy"
    { }
    break;

  case 1132:

/* Line 1455 of yacc.c  */
#line 5367 "drizzled/sql_yacc.yy"
    { }
    break;

  case 1133:

/* Line 1455 of yacc.c  */
#line 5371 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1134:

/* Line 1455 of yacc.c  */
#line 5372 "drizzled/sql_yacc.yy"
    { (yyval.var_type)=OPT_GLOBAL; }
    break;

  case 1135:

/* Line 1455 of yacc.c  */
#line 5373 "drizzled/sql_yacc.yy"
    { (yyval.var_type)=OPT_SESSION; }
    break;

  case 1136:

/* Line 1455 of yacc.c  */
#line 5374 "drizzled/sql_yacc.yy"
    { (yyval.var_type)=OPT_SESSION; }
    break;

  case 1137:

/* Line 1455 of yacc.c  */
#line 5378 "drizzled/sql_yacc.yy"
    { (yyval.num)= OPT_DEFAULT; }
    break;

  case 1138:

/* Line 1455 of yacc.c  */
#line 5379 "drizzled/sql_yacc.yy"
    { ((statement::SetOption *)Lex.statement)->one_shot_set= true; (yyval.num)= OPT_SESSION; }
    break;

  case 1139:

/* Line 1455 of yacc.c  */
#line 5383 "drizzled/sql_yacc.yy"
    { (yyval.var_type)=OPT_SESSION; }
    break;

  case 1140:

/* Line 1455 of yacc.c  */
#line 5384 "drizzled/sql_yacc.yy"
    { (yyval.var_type)=OPT_GLOBAL; }
    break;

  case 1141:

/* Line 1455 of yacc.c  */
#line 5385 "drizzled/sql_yacc.yy"
    { (yyval.var_type)=OPT_SESSION; }
    break;

  case 1142:

/* Line 1455 of yacc.c  */
#line 5386 "drizzled/sql_yacc.yy"
    { (yyval.var_type)=OPT_SESSION; }
    break;

  case 1143:

/* Line 1455 of yacc.c  */
#line 5390 "drizzled/sql_yacc.yy"
    { (yyval.var_type)=OPT_DEFAULT; }
    break;

  case 1144:

/* Line 1455 of yacc.c  */
#line 5391 "drizzled/sql_yacc.yy"
    { (yyval.var_type)=OPT_GLOBAL; }
    break;

  case 1145:

/* Line 1455 of yacc.c  */
#line 5392 "drizzled/sql_yacc.yy"
    { (yyval.var_type)=OPT_SESSION; }
    break;

  case 1146:

/* Line 1455 of yacc.c  */
#line 5393 "drizzled/sql_yacc.yy"
    { (yyval.var_type)=OPT_SESSION; }
    break;

  case 1149:

/* Line 1455 of yacc.c  */
#line 5403 "drizzled/sql_yacc.yy"
    {
            if ((yyvsp[(2) - (4)].variable).var)
            { /* System variable */
              if ((yyvsp[(1) - (4)].var_type))
              {
                Lex.option_type= (yyvsp[(1) - (4)].var_type);
              }
              Lex.var_list.push_back(SetVarPtr(new set_var(Lex.option_type, (yyvsp[(2) - (4)].variable).var, &(yyvsp[(2) - (4)].variable).base_name, (yyvsp[(4) - (4)].item))));
            }
          }
    break;

  case 1150:

/* Line 1455 of yacc.c  */
#line 5414 "drizzled/sql_yacc.yy"
    {
            Lex.option_type= (yyvsp[(1) - (5)].var_type);
            Lex.var_list.push_back(SetVarPtr(new set_var(Lex.option_type,
                                              find_sys_var("tx_isolation"),
                                              &null_lex_str,
                                              new Item_int((int32_t)
                                              (yyvsp[(5) - (5)].tx_isolation)))));
          }
    break;

  case 1151:

/* Line 1455 of yacc.c  */
#line 5426 "drizzled/sql_yacc.yy"
    {
            Lex.var_list.push_back(SetVarPtr(new set_var_user(new Item_func_set_user_var((yyvsp[(2) - (4)].lex_str),(yyvsp[(4) - (4)].item)))));
          }
    break;

  case 1152:

/* Line 1455 of yacc.c  */
#line 5430 "drizzled/sql_yacc.yy"
    {
            Lex.var_list.push_back(SetVarPtr(new set_var((yyvsp[(3) - (6)].var_type), (yyvsp[(4) - (6)].variable).var, &(yyvsp[(4) - (6)].variable).base_name, (yyvsp[(6) - (6)].item))));
          }
    break;

  case 1153:

/* Line 1455 of yacc.c  */
#line 5436 "drizzled/sql_yacc.yy"
    { (yyval.lex_str)=(yyvsp[(1) - (1)].lex_str);}
    break;

  case 1154:

/* Line 1455 of yacc.c  */
#line 5437 "drizzled/sql_yacc.yy"
    { (yyval.lex_str)=(yyvsp[(1) - (1)].lex_str);}
    break;

  case 1155:

/* Line 1455 of yacc.c  */
#line 5438 "drizzled/sql_yacc.yy"
    { (yyval.lex_str)=(yyvsp[(1) - (1)].lex_str);}
    break;

  case 1156:

/* Line 1455 of yacc.c  */
#line 5443 "drizzled/sql_yacc.yy"
    {
            (yyval.lex_str).str= YYSession->strmake((yyvsp[(1) - (1)].symbol).str, (yyvsp[(1) - (1)].symbol).length);
            (yyval.lex_str).length= (yyvsp[(1) - (1)].symbol).length;
          }
    break;

  case 1157:

/* Line 1455 of yacc.c  */
#line 5447 "drizzled/sql_yacc.yy"
    { (yyval.lex_str)=(yyvsp[(1) - (1)].lex_str); }
    break;

  case 1158:

/* Line 1455 of yacc.c  */
#line 5452 "drizzled/sql_yacc.yy"
    {
            /* We have to lookup here since local vars can shadow sysvars */
            {
              /* Not an SP local variable */
              sys_var *tmp= find_sys_var(std::string((yyvsp[(1) - (1)].lex_str).str, (yyvsp[(1) - (1)].lex_str).length));
              if (!tmp)
                DRIZZLE_YYABORT;
              (yyval.variable).var= tmp;
              (yyval.variable).base_name= null_lex_str;
            }
          }
    break;

  case 1159:

/* Line 1455 of yacc.c  */
#line 5466 "drizzled/sql_yacc.yy"
    { (yyval.tx_isolation)= ISO_READ_UNCOMMITTED; }
    break;

  case 1160:

/* Line 1455 of yacc.c  */
#line 5467 "drizzled/sql_yacc.yy"
    { (yyval.tx_isolation)= ISO_READ_COMMITTED; }
    break;

  case 1161:

/* Line 1455 of yacc.c  */
#line 5468 "drizzled/sql_yacc.yy"
    { (yyval.tx_isolation)= ISO_REPEATABLE_READ; }
    break;

  case 1162:

/* Line 1455 of yacc.c  */
#line 5469 "drizzled/sql_yacc.yy"
    { (yyval.tx_isolation)= ISO_SERIALIZABLE; }
    break;

  case 1163:

/* Line 1455 of yacc.c  */
#line 5473 "drizzled/sql_yacc.yy"
    { (yyval.item)=(yyvsp[(1) - (1)].item); }
    break;

  case 1164:

/* Line 1455 of yacc.c  */
#line 5474 "drizzled/sql_yacc.yy"
    { (yyval.item)=0; }
    break;

  case 1165:

/* Line 1455 of yacc.c  */
#line 5475 "drizzled/sql_yacc.yy"
    { (yyval.item)=new Item_string("ON",  2, system_charset_info); }
    break;

  case 1166:

/* Line 1455 of yacc.c  */
#line 5476 "drizzled/sql_yacc.yy"
    { (yyval.item)=new Item_string("ALL", 3, system_charset_info); }
    break;

  case 1167:

/* Line 1455 of yacc.c  */
#line 5477 "drizzled/sql_yacc.yy"
    { (yyval.item)=new Item_string("binary", 6, system_charset_info); }
    break;

  case 1170:

/* Line 1455 of yacc.c  */
#line 5487 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::UnlockTables(YYSession);
          }
    break;

  case 1171:

/* Line 1455 of yacc.c  */
#line 5491 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1172:

/* Line 1455 of yacc.c  */
#line 5496 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::StartTransaction(YYSession);
          }
    break;

  case 1173:

/* Line 1455 of yacc.c  */
#line 5499 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1174:

/* Line 1455 of yacc.c  */
#line 5503 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1175:

/* Line 1455 of yacc.c  */
#line 5504 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1176:

/* Line 1455 of yacc.c  */
#line 5509 "drizzled/sql_yacc.yy"
    { (yyval.num)= (YYSession->variables.completion_type == 1); }
    break;

  case 1177:

/* Line 1455 of yacc.c  */
#line 5510 "drizzled/sql_yacc.yy"
    { (yyval.num)=0; }
    break;

  case 1178:

/* Line 1455 of yacc.c  */
#line 5511 "drizzled/sql_yacc.yy"
    { (yyval.num)=1; }
    break;

  case 1179:

/* Line 1455 of yacc.c  */
#line 5516 "drizzled/sql_yacc.yy"
    { (yyval.num)= (YYSession->variables.completion_type == 2); }
    break;

  case 1180:

/* Line 1455 of yacc.c  */
#line 5517 "drizzled/sql_yacc.yy"
    { (yyval.num)=1; }
    break;

  case 1181:

/* Line 1455 of yacc.c  */
#line 5518 "drizzled/sql_yacc.yy"
    { (yyval.num)=0; }
    break;

  case 1182:

/* Line 1455 of yacc.c  */
#line 5522 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1183:

/* Line 1455 of yacc.c  */
#line 5523 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1184:

/* Line 1455 of yacc.c  */
#line 5528 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::Commit(YYSession, (yyvsp[(3) - (4)].num), (yyvsp[(4) - (4)].num));
          }
    break;

  case 1185:

/* Line 1455 of yacc.c  */
#line 5535 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::Rollback(YYSession, (yyvsp[(3) - (4)].num), (yyvsp[(4) - (4)].num));
          }
    break;

  case 1186:

/* Line 1455 of yacc.c  */
#line 5539 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::RollbackToSavepoint(YYSession, (yyvsp[(5) - (5)].lex_str));
          }
    break;

  case 1187:

/* Line 1455 of yacc.c  */
#line 5546 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::Savepoint(YYSession, (yyvsp[(2) - (2)].lex_str));
          }
    break;

  case 1188:

/* Line 1455 of yacc.c  */
#line 5553 "drizzled/sql_yacc.yy"
    {
            Lex.statement= new statement::ReleaseSavepoint(YYSession, (yyvsp[(3) - (3)].lex_str));
          }
    break;

  case 1190:

/* Line 1455 of yacc.c  */
#line 5568 "drizzled/sql_yacc.yy"
    {}
    break;

  case 1192:

/* Line 1455 of yacc.c  */
#line 5574 "drizzled/sql_yacc.yy"
    {
            if (parser::add_select_to_union_list(YYSession, &Lex, (bool)(yyvsp[(2) - (2)].num)))
              DRIZZLE_YYABORT;
          }
    break;

  case 1193:

/* Line 1455 of yacc.c  */
#line 5579 "drizzled/sql_yacc.yy"
    {
            /*
              Remove from the name resolution context stack the context of the
              last select in the union.
            */
            Lex.pop_context();
          }
    break;

  case 1194:

/* Line 1455 of yacc.c  */
#line 5589 "drizzled/sql_yacc.yy"
    { (yyval.num)= 0; }
    break;

  case 1195:

/* Line 1455 of yacc.c  */
#line 5590 "drizzled/sql_yacc.yy"
    { (yyval.num)= 1; }
    break;

  case 1196:

/* Line 1455 of yacc.c  */
#line 5591 "drizzled/sql_yacc.yy"
    { (yyval.num)= 1; }
    break;

  case 1197:

/* Line 1455 of yacc.c  */
#line 5595 "drizzled/sql_yacc.yy"
    {
            assert(Lex.current_select->linkage != GLOBAL_OPTIONS_TYPE);
            Select_Lex *sel= Lex.current_select;
            Select_Lex_Unit *unit= sel->master_unit();
            Select_Lex *fake= unit->fake_select_lex;
            if (fake)
            {
              unit->global_parameters= fake;
              fake->no_table_names_allowed= 1;
              Lex.current_select= fake;
            }
            YYSession->setWhere("global ORDER clause");
          }
    break;

  case 1198:

/* Line 1455 of yacc.c  */
#line 5609 "drizzled/sql_yacc.yy"
    {
            YYSession->lex().current_select->no_table_names_allowed= 0;
            YYSession->setWhere("");
          }
    break;

  case 1201:

/* Line 1455 of yacc.c  */
#line 5621 "drizzled/sql_yacc.yy"
    { (yyval.num)=1; }
    break;

  case 1202:

/* Line 1455 of yacc.c  */
#line 5622 "drizzled/sql_yacc.yy"
    { (yyval.num)=1; }
    break;

  case 1203:

/* Line 1455 of yacc.c  */
#line 5623 "drizzled/sql_yacc.yy"
    { (yyval.num)=0; }
    break;

  case 1204:

/* Line 1455 of yacc.c  */
#line 5628 "drizzled/sql_yacc.yy"
    {
            (yyval.select_lex)= Lex.current_select->master_unit()->first_select();
          }
    break;

  case 1205:

/* Line 1455 of yacc.c  */
#line 5632 "drizzled/sql_yacc.yy"
    {
            (yyval.select_lex)= Lex.current_select->master_unit()->first_select();
          }
    break;

  case 1207:

/* Line 1455 of yacc.c  */
#line 5641 "drizzled/sql_yacc.yy"
    {
            if (parser::add_select_to_union_list(YYSession, &Lex, (bool)(yyvsp[(3) - (3)].num)))
              DRIZZLE_YYABORT;
          }
    break;

  case 1208:

/* Line 1455 of yacc.c  */
#line 5646 "drizzled/sql_yacc.yy"
    {
            Lex.pop_context();
            (yyval.select_lex)= (yyvsp[(1) - (5)].select_lex);
          }
    break;

  case 1209:

/* Line 1455 of yacc.c  */
#line 5655 "drizzled/sql_yacc.yy"
    {
            (yyval.select_lex)= (yyvsp[(2) - (3)].select_lex);
          }
    break;

  case 1210:

/* Line 1455 of yacc.c  */
#line 5661 "drizzled/sql_yacc.yy"
    {
            if (not Lex.expr_allows_subselect)
            {
              parser::my_parse_error(YYSession->m_lip);
              DRIZZLE_YYABORT;
            }
            /*
              we are making a "derived table" for the parenthesis
              as we need to have a lex level to fit the union
              after the parenthesis, e.g.
              (SELECT .. ) UNION ...  becomes
              SELECT * FROM ((SELECT ...) UNION ...)
            */
            if (new_select(&Lex, 1))
              DRIZZLE_YYABORT;
          }
    break;

  case 1211:

/* Line 1455 of yacc.c  */
#line 5680 "drizzled/sql_yacc.yy"
    {
            Lex.pop_context();
            Select_Lex *child= Lex.current_select;
            Lex.current_select= Lex.current_select->return_after_parsing();
            Lex.nest_level--;
            Lex.current_select->n_child_sum_items += child->n_sum_items;
            /*
              A subselect can add fields to an outer select. Reserve space for
              them.
            */
            Lex.current_select->select_n_where_fields+=
            child->select_n_where_fields;
          }
    break;



/* Line 1455 of yacc.c  */
#line 16183 "drizzled/sql_yacc.cc"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (session, YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (session, yymsg);
	  }
	else
	  {
	    yyerror (session, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, session);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, session);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (session, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, session);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, session);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



