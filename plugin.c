/* Little plugin that is linked to libpython via DT_NEEDED, and then dlopen'ed
 * into the test host.
 */

#include <stdio.h>
#include <Python.h>

void do_plugin_stuff()
{
    printf("Running in plugin:\n");

    Py_Initialize();
    PyRun_SimpleString("print('  (hello world from plugin embedded python!)')");
    PyRun_SimpleString("print('  Address of True is %x' % id(True))");

    printf("Plugin attempting to load a non-existent module\n");
    if (PyRun_SimpleString("import nosuchmodule") < 0) {
        printf("...failed, as expected\n");
    } else {
        printf("...succeeded, WTF?\n");
    }

    printf("Plugin attempting to load an extension module\n");
    if (PyRun_SimpleString("import _regex") < 0) {
        printf("...failed\n");
    } else {
        printf("...succeeded\n");
    }
}
