
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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
/* Line 1676 of yacc.c  */
#line 120 "drizzled/sql_yacc.yy"
ParserType
{

/* Line 1676 of yacc.c  */
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



/* Line 1676 of yacc.c  */
#line 834 "drizzled/sql_yacc.hh"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif




