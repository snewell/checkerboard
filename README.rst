checkerboard
============
checkerboard is a learning/experimentation project.  The goal is to implement a
socket library that uses C++ language features to ensure compile-time safety
(e.g., trying to bind an :code:`AF_INET` socket with an :code:`AF_INET6`
address shouldn't compile).

checkerboard isn't expected to reach production quality, either in terms of
implementation quality or support.


Inspiration
-----------
Lisa Lippincott's `excellent talk from CppCon 2014 <https://www.youtube.com/watch?v=3ZO0V4Prefc>`_
is a major source of inspiration and guidance.  Anything clever in checkerboard
is based on something Lisa presented; the dumb things are all mine.


Building
--------
Because checkerboard is a header-only library, the build process is only for
tests and installation.  To build, you'll need CMake_ and steps similar to the
following:

1. :code:`mkdir build`
2. :code:`cd build`
3. :code:`cmake /path/to/checkerboard/src`
4. :code:`cmake --build .`
5. :code:`cmake --build . --target test` (Optional, requires GTest_)
6. :code:`cmake --build . --target install`


Target Platforms
~~~~~~~~~~~~~~~~
Any Unix-like operating system should work, but all development and testing is
performed on x86-64 Linux machines.


.. _CMake: https://cmake.org
.. _GTest: https://github.com/google/googletest
