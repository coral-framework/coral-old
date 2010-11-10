Coral Documentation
===================

Coral has two kinds of documentation:

1. **General documentation**, such as guides, examples, etc., which are hand-mantained markup files.
2. **API documentation**, generated from inline comments in C++ headers and CSL files.

Most files in `/docs/*` are part of the general documentation. The only exception are those in `/docs/doxygen`, which are auxiliary files for generating the API documentation.

Generating the General Documentation
------------------------------------

The hand-maintained documentation must be processed by a tool called [nanoc](http://nanoc.stoneship.org/), a static-website compiler written in Ruby. Once you get nanoc installed, all you need to do is call `nanoc compile` in the `/docs` directory. The output is placed in `${CMAKE_BINARY_DIR}/docs/output/doc`.

Generating the API Documentation
--------------------------------

The API Documentation is generated in three steps:

1. First we must use nanoc to compile the General Documentation, as it will also create required files for the API Documentation.
2. Then we run the Coral Compiler to extract documentation from the CSL files. The compiler will generate a `typename.dox` file for each Coral type.
3. Finally, we run [Doxygen](http://doxygen.org).

Thankfully, this process is handled automatically by CMake in a custom target named `docs`. See [/docs/CMakeLists.txt](CMakeLists.txt) for the code, which is based on re-usable macros from the Coral package.

The output is placed in `${CMAKE_BINARY_DIR}/docs/output/doc/api`.
