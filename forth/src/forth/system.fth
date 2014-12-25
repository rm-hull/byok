: (   41 parse 2drop ; immediate
( That was the definition for the comment word. )
( Now we can add comments to what we are doing! )
( Note: default decimal numeric input mode. )

: \ ( <line> -- , comment out the rest of line)
   13 parse 2drop ; immediate

\ 1 echo !   \ Uncomment this line to echo Foth code while compiling.

\ *********************************************************************
\ This is another style of comment that is common in Forth.
\ BYOK (portmanteau of BYE & OK) is a BYO kernel which happens to
\ implement a forth machine on i686 bare metal.
\ 
\ Substantial portions of this file were lifted straight from pForth
\ (http://pforth.googlecode.com/svn/trunk/fth/system.fth)
\ *********************************************************************

 -1 constant ERR_ABORT
 -2 constant ERR_ABORTQ
-14 constant ERR_EXECUTING
-22 constant ERR_PAIRS

: BINARY    2 base ! ;
: OCTAL     8 base ! ;
: DECIMAL  10 base ! ;
: HEX      16 base ! ;

: COUNT  dup 1 + swap c@ ;
: ON   ( addr -- , set true ) -1 swap ! ;
: OFF  ( addr -- , set false ) 0 swap ! ;

: ABORT ( i*x -- ) err_abort throw ;

: [  ( -- , enter interpreter mode )  0 state ! ; immediate
: ]  ( -- , enter compile mode )      1 state ! ;

29521 constant CONDITIONAL_KEY
: ?CONDITION  ( f -- )  conditional_key - err_pairs ?error ;
: >MARK       ( -- addr )   here 0 , ;
: >RESOLVE    ( addr -- )   here over - swap ! ;
: <MARK       ( -- addr )   here ;
: <RESOLVE    ( addr -- )   here - , ;

: ?COMP  ( -- , error if not compiling ) state @ 0= err_executing ?error ;
: ?PAIRS ( n m -- ) - err_pairs ?error ;

: .( 41 parse type ; immediate

