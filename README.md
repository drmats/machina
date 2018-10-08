# machina

<a href="https://github.com/drmats/machina/">
    <img
        src="https://raw.githubusercontent.com/drmats/machina/master/icon.png"
        align="left"
        hspace="10"
    >
</a>

_Basic_ 3D math in action.

Points, Vectors and Matrices.<br>
Dot Products, Cross Products, Translations, Rotations, Scales and more.

Parts of code written in 2011.

<br />




## compilation

```bash
$ make help
Available targets:
    linux  -  build using "gcc/g++" (GNU C/C++ Compiler) [default]
    win32  -  build using "i686-w64-mingw32-g++" (32bit Windows target)
    win64  -  build using "x86_64-w64-mingw32-g++" (64bit Windows target)
    clean  -  remove compiled objects and main program
```

<br />




## dependencies

* [**GNU Make**](http://www.gnu.org/software/make/)
* C++ compiler with [**C++11**](https://en.wikipedia.org/wiki/C%2B%2B11) support ([**gcc**](http://gcc.gnu.org/) or [**clang**](http://clang.llvm.org/))
* [**Mingw-w64**](http://mingw-w64.sourceforge.net/) for windows cross-compilation
* [**Simple Directmedia Layer**](https://www.libsdl.org/)
* [**OpenGL Extension Wrangler Library**](http://glew.sourceforge.net/)

<br />




## license

**machina** is released under the Simplified BSD license. See the
[LICENSE](https://raw.githubusercontent.com/drmats/machina/master/LICENSE)
for more details.
