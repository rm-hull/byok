\ *********************************************************************
\ Soem random forth code to test the stack machine

\ https://groups.google.com/forum/?hl=en#!topic/comp.lang.forth/cA76I1ryQrk

: GCD ( a b -- n ) 
    begin  dup while  tuck mod  repeat drop ;

\ : gcd2 ( a b d -- c ) 
\        >R 1 R@ +! 
\        OVER IF OVER MOD SWAP R>  RECURSE 
\           ELSE SWAP R> 2DROP 
\           THEN ; 

: SQR  ( n -- n ) dup * ;
: QUADRATIC  ( a b c d -- n )  >r swap rot r@ * + r> * + ;
: ?DOZEN  12 = IF ." dozen " ELSE ." not a dozen" THEN ;
: GREETINGS   ." Hello earthling " cr ;

