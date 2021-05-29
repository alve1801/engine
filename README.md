# engine
A game engine I use for stuff
I've been using the [OneLoneCoder Pixel Game Engine](https://github.com/OneLoneCoder/olcPixelGameEngine) for a lot of my projects, until it one day crashed my computer because of its terrible performance (pretty good engine altogether, I might have been overusing it). That is when I set out to make my own graphics engine, since I really didn't need a lot.

Like the original OLCPGE, it is implemented as a single header file that you include in your project and supply with a loop function. The `font` file is a binary-encoded copy of codepage 437 used for text display (I haven't come up with an efficient way to hardcode it).

Requires SDL2.

Basic usage:
(will write this later)

TODO:
- enum for keysyms, because I currently have to figure out the hex value of every key by hand
- key modifiers don't work
- proper delay (should also fix the mouse issue)
