Coral Documentation
===================

Coral has two kinds of documentation:

1. **General documentation**, such as guides, examples, etc., which are hand-mantained markup files.
2. **API documentation**, generated from inline comments in C++ headers and CSL files.

Most files in `/docs/*` are part of the general documentation. The only exception are those in `/docs/doxygen`, which are auxiliary files for the API documentation.


Generating the General Documentation
------------------------------------

The hand-maintained documentation must be processed by a tool called [nanoc](http://nanoc.stoneship.org/), a static-website compiler written in Ruby. If you have nanoc installed, you can use the CMake custom target `docs-general` to compile the general documentation automatically. But if you want to do it manually, here are the steps:

1. Install nanoc and the required dependencies. For syntax highlighting, we use the excellent [highlight](http://www.andre-simon.de/zip/download.html) tool.
2. Copy `/docs/config.yaml.in` to `/docs/config.yaml` and manually set all `${variables}`.
3. Call `nanoc compile` in the `/docs` dir.


Generating the API Documentation
--------------------------------

The API Documentation is generated in three steps:

1. First we must use nanoc to compile the general documentation, as it will also generate files for the API documentation.
2. Then we run the Coral Compiler to extract the documentation in CSL files. The compiler will generate a `typename.dox` file for each Coral type.
3. Finally, we run [Doxygen](http://doxygen.org) to produce the final result.

Thankfully, this process is handled automatically by CMake in a custom target named `docs`. See [/docs/CMakeLists.txt](CMakeLists.txt) for the code, which is based on re-usable macros from the Coral package.

Using CMake, the general documentation is placed in `${CMAKE_BINARY_DIR}/docs/output/doc`, and the API documentation is placed in `${CMAKE_BINARY_DIR}/docs/output/doc/api`.
