SafeJNI
=======

C++11 library that makes easy to call Java from C++. 

Writing JNI code is very tedious. SafeJNI library takes advantage of advanced C++11 features like variadic templates and generates all the tedious JNI code in compile time. It automatically calculates the Java method signature according to the parameters passed by the user. It uses compile time strings to avoid runtime string concatenations while calculating the signature of a Java method. 

Right now SafeJNI supports primitives, strings, vectors and maps but it can be extended easily to support more object types.  

### How to compile

bin folder contains a ready to use version:  compiled static library and the safejni.h header 

If you want yo compile a new version just run compile.sh script in the src folder. ndk-build must be in your $PATH.

### How to use

See test project source code to see how to use it and how to add the library to your project.

### License

The MIT License (MIT)

Copyright (c) 2014 MortimerGoro

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.