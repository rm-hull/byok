: (   41 parse 2drop ; immediate
( That was the definition for the comment word. )
( Now we can add comments to what we are doing! )
( Note: default decimal numeric input mode. )

: \ ( <line> -- , comment out the rest of line)
   13 parse 2drop ; immediate

\ 1 echo !   \ Uncomment this line to echo Forth code while compiling.

\ *********************************************************************
\ This is another style of comment that is common in Forth.
\ BYOK (portmanteau of BYE & OK) is a BYO kernel which happens to
\ implement a forth machine on i686 bare metal.
\ 
\ Substantial portions of this file were lifted straight from pForth
\ (http://pforth.googlecode.com/svn/trunk/fth/system.fth)
\ *********************************************************************
: CELL 1 ;
: BL 32 ;

: SPACE  ( -- )  bl emit ;
: CR  ( -- , cause output to appear at the beginning of the next line )  10 emit ;

: CELL+  ( x -- x+cell )  cell + ;
: CELL-  ( x -- x-cell )  cell - ;

: BINARY    2 base ! ;
: OCTAL     8 base ! ;
: DECIMAL  10 base ! ;
: HEX      16 base ! ;

: PAD ( -- addr ) here 128 + ;

: MOVE$  ( a1 n a2 -- ) swap cmove ;
: $MOVE  ( $src $dst ) over c@ 1+ cmove ;
 
: COUNT  dup 1 + swap c@ ;
: ON   ( addr -- , set true ) -1 swap ! ;
: OFF  ( addr -- , set false ) 0 swap ! ;

: [  ( -- , enter interpreter mode )  0 state ! ; immediate
: ]  ( -- , enter compile mode )      1 state ! ;

: EVEN-UP  ( n -- n | n+1, make even )  dup 1 and + ;
: ALIGNED  ( addr -- a-addr )
    [ cell 1- ] literal +
    [ cell 1- invert ] literal and ;
: ALIGN  ( -- , align DP ) dp @ aligned dp ! ;

\ Compiler support -------------------------------------------------
: COMPILE,   ( xt -- , compile call to xt ) , ;
: [COMPILE]  ( <name> -- , compile now even if immediate ) ' compile, ; immediate
: (COMPILE)  ( xt -- , postpone compilation of token ) 
    [compile] literal ( compile a call to literal )
    ( store xt of word to be compiled )

    [ ' compile, ] literal \ compile call to compile,
    compile, ;

: COMPILE  ( <name> --, save xt and compile later ) ' (compile) ; immediate

: :NONAME  ( -- xt , begin compilation of headerless secondary ) align here ] ;

\ Error codes defined in ANSI Exception word set -------------------
: ERR_ABORT       -1 ;
: ERR_ABORTQ      -2 ;
: ERR_EXECUTING  -14 ;
: ERR_PAIRS      -22 ;

: ABORT ( i*x -- ) err_abort throw ;

\ Conditionals in '83 form -----------------------------------------
: CONDITIONAL_KEY ( -- , lazy constant ) 29521 ;
: ?CONDITION  ( f -- )  conditional_key - err_pairs ?error ;
: >MARK       ( -- addr )   here 0 , ;
: >RESOLVE    ( addr -- )   here over - swap ! ;
: <MARK       ( -- addr )   here ;
: <RESOLVE    ( addr -- )   here - , ;

: ?COMP  ( -- , error if not compiling ) state @ 0= err_executing ?error ;
: ?PAIRS ( n m -- ) - err_pairs ?error ;

: .( 41 parse type ; immediate

\ Conditional primitives -------------------------------------------
: IF     ( -- f orig )  ?comp compile 0branch conditional_key >mark ; immediate
: THEN   ( f orig -- )  swap ?condition >resolve ; immediate
: BEGIN  ( -- f dest )  ?comp conditional_key <mark ; immediate
: AGAIN  ( f dest -- )  compile branch swap ?condition <resolve ; immediate
: UNTIL  ( f dest -- )  compile 0branch swap ?condition <resolve ; immediate
: AHEAD  ( -- f orig )  compile branch conditional_key >mark ; immediate

\ Conditionals built from primitives -------------------------------
: ELSE   ( f orig1 -- f orig2 )  [compile] ahead 2swap [compile] then ; immediate
: WHILE  ( f dest -- f origi f dest ) [compile] if 2swap ; immediate
: REPEAT  ( -- f orig f dest ) [compile] again [compile] then ; immediate

: [']  ( <name> -- xt , define compile time tick ) ?comp ' [compile] literal ; immediate


