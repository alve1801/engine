# engine
A game engine I use for graphics n I/O

I've been using the [OneLoneCoder Pixel Game Engine](https://github.com/OneLoneCoder/olcPixelGameEngine) for a lot of my projects, until it one day crashed my computer because of its terrible performance (pretty good engine altogether, I might have been overusing it). That is when I set out to make my own graphics engine, since I really didn't need a lot.

Like the original OLCPGE, it is implemented as a single header file that you include in your project and supply with a loop function. The font used is an encoded version of codepage 437, base64-compressed into a string and loaded into memory at initialization.

`main.cpp` and `Makefile` are for a quick squaremuncher demo I threw together to exemplify how it works.

Requires SDL2. Also has a hacked-together [explanation/documentation](https://avethenoul.neocities.org/frave.html) on my website.

TODO:
- enum for keysyms, because I currently have to figure out the hex value of every key by hand
- key modifiers don't work
- proper delay (should also fix the mouse issue)
