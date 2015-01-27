\ ***************************************************************************
\ Forth Fractal
\ from: http://www.qemu-advent-calendar.org/#day-19
\       http://www.qemu-advent-calendar.org/download/bootinfo.txt
\ Contributed by Dr. David Alan Gilbert.
\ ***************************************************************************
hex

\ TODO: need to support proper curses color mappings for this to work
: plotval
    7 and dup 1b emit 5b emit 33 emit 30 + emit 6d emit 40 + emit ;

decimal

: mlpt 2dup 1 >r begin 
    2dup dup * 2000 / swap dup * 2000 / + 4 2000 * < i 100 < and  while
    r> 1 + >r 2dup >r >r * 2 * 2000 / rot dup -rot + rot r> dup * r> dup * swap - 2000 / swap dup rot + rot swap
    repeat drop drop drop drop r> plotval ;

: c2s 2dup - >r rot r> swap / -rot ;

: xloop c2s do swap dup i mlpt swap dup +loop  drop drop ;

: yloop c2s do cr i rot dup >r >r rot dup r> swap >r >r >r rot dup rot swap r> r>
    xloop swap r> swap r> swap dup +loop drop drop drop drop ;

: prompt 1b emit 5b emit 33 emit 37 emit 6d emit cr ." Forth Mandelbrot.   i: zoom in | o: zoom out | h/l: left/right | k/j: up/down | q: quit ?" ;

: 6dup 5 pick 5 pick 5 pick 5 pick 5 pick 5 pick ;

: zin drop 2dup - 4 / rot drop dup rot + dup -rot swap 2 * + swap >r >r >r
      2dup - 4 / rot drop dup rot + dup -rot swap 2 * + swap r> r> r> 33 ;
: zout drop 2dup - 2 / rot drop dup rot swap - dup -rot swap 4 * + swap >r >r >r
      2dup - 2 / rot drop dup rot swap - dup -rot swap 4 * + swap r> r> r> 33 ;
: mleft drop >r >r >r 2dup - 2 / dup rot swap - -rot - swap r> r> r> 33 ;
: mright drop >r >r >r 2dup - 2 / dup rot + -rot + swap r> r> r> 33 ;
: mup drop 2dup - 2 / dup rot swap - -rot - swap 33 ;
: mdown drop 2dup - 2 / dup rot + -rot + swap 33 ;

: ops dup 69 = if zin else dup 6f = if zout else dup 68 = if mleft else dup 6c = if mright else dup 6b = if mup else dup 6a = if mdown then then then then then then ;
: mloop 40 4000 -4000 10 4000 -4000 begin 6dup yloop prompt key ops 71 = until ;
