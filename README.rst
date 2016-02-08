Small repo for testing how ``dlopen`` namespaces work in Linux/ELF.

``_regex.so`` is a small Python extension taken from a Debian system,
that does not explictly link to ``libpython2.7.so.1``.

``linktest.c`` and ``plugin.c`` together make some programs to check
how different ways of linking and running Python extensions work.

::

   $ ./build.sh

   $ ./linktest-with-python
   Checking for a priori presence of python...
   FOUND: we were linked with -lpython
   Using dlopen to load libpython
   Using dlopen to load a python extension
   Initializing dlopen'ed python interpreter
     (hello world from dlopen embedded python!)
     Address of True is 7fcfa37f49f0
   Using dlopen'ed python interpreter to load a non-existent module
   Traceback (most recent call last):
     File "<string>", line 1, in <module>
   ImportError: No module named nosuchmodule
   ...failed, as expected
   Using dlopen'ed python interpreter to load an extension module
   ...succeeded
   Running in plugin:
     (hello world from plugin embedded python!)
     Address of True is 7fcfa37f49f0
   Plugin attempting to load a non-existent module
   Traceback (most recent call last):
     File "<string>", line 1, in <module>
   ImportError: No module named nosuchmodule
   ...failed, as expected
   Plugin attempting to load an extension module
   ...succeeded

   $ ./linktest-no-python
   Checking for a priori presence of python...
   NOT FOUND: we were NOT linked with -lpython
   Using dlopen to load libpython
   Using dlopen to load a python extension
     dlopen error: _regex: ./_regex.so: undefined symbol: _Py_ZeroStruct
   Initializing dlopen'ed python interpreter
     (hello world from dlopen embedded python!)
     Address of True is 7f7f9ef0d9f0
   Using dlopen'ed python interpreter to load a non-existent module
   Traceback (most recent call last):
     File "<string>", line 1, in <module>
   ImportError: No module named nosuchmodule
   ...failed, as expected
   Using dlopen'ed python interpreter to load an extension module
   Traceback (most recent call last):
     File "<string>", line 1, in <module>
   ImportError: /home/njs/manylinux/link-namespaces/_regex.so: undefined symbol: _Py_ZeroStruct
   ...failed
   Running in plugin:
     (hello world from plugin embedded python!)
     Address of True is 7f7f9ef0d9f0
   Plugin attempting to load a non-existent module
   Traceback (most recent call last):
     File "<string>", line 1, in <module>
   ImportError: No module named nosuchmodule
   ...failed, as expected
   Plugin attempting to load an extension module
   Traceback (most recent call last):
     File "<string>", line 1, in <module>
   ImportError: /home/njs/manylinux/link-namespaces/_regex.so: undefined symbol: _Py_ZeroStruct
   ...failed

It's also interesting to run these with ``LD_DEBUG=scopes`` to get a
sense of what's going on.
