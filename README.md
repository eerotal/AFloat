# AFloat - An arbitary precision math library written in C

## What is it?

Programming languages often do non-integer math with a type called
floating point numbers, which make it possible to do precise calculations
with decimal values. However, floating point math poses a problem:
Because of how floating point values work, calculations using floating
point math introduce small errors even into simple calculations. More
information on this subject can easily be found online, which is why I
won't explain it here. There are multiple libraries for different
programming languages that solve this problem by implementing arbitary
precision floating point numbers that never introduce any errors in
calculations. These kinds of libraries also exist for C, and this is
another attempt at writing one.

**NOTE: This library is still VERY much work in progress. In fact it
is not even compiled as a library yet, which means it can't actually
be used as a library in other programs. The development of this library
is at such and early stage that you really shouldn't use this for
anything yet.**

## License

This library is licensed under the BSD 3-clause license. The full license
text can be found in the file LICENSE.txt in the source root of this
library.

Copyright 2017 Eero Talus
