/*
 * gcc linktest.c -lpython2.7 -ldl -o linktest-with-python
 * gcc linktest.c             -ldl -o linktest-no-python
 *
 * Copyright (C) Nathaniel J. Smith; MIT licensed.
 */

#include <stdio.h>
#include <dlfcn.h>

void check_dl(void * handle, const char * msg)
{
    if (!handle) {
        printf("  dlopen error: %s: %s\n", msg, dlerror());
    }
}

int main(int argc, char ** argv)
{
    /* Set stdout to unbuffered, to make sure output remains synced up in case
     * we use LD_DEBUG or similar.
     */
    setvbuf(stdout, NULL, _IONBF, 0);

    void * process_handle = dlopen(NULL, RTLD_LAZY);
    check_dl(process_handle, "process handle (can't fail!)");
    printf("Checking for a priori presence of python...\n");
    void * canary = dlsym(process_handle, "PyNumber_Add");
    if (canary) {
        printf("FOUND: we were linked with -lpython\n");
    } else {
        printf("NOT FOUND: we were NOT linked with -lpython\n");
    }

    printf("Using dlopen to load libpython\n");
    void * py_handle = dlopen("libpython2.7.so.1",
                              RTLD_LAZY | RTLD_LOCAL);
    check_dl(py_handle, "libpython");

    printf("Using dlopen to load a python extension\n");
    void * mod_handle = dlopen("./_regex.so", RTLD_LAZY | RTLD_LOCAL);
    check_dl(mod_handle, "_regex");

    printf("Initializing dlopen'ed python interpreter\n");
    void (*Py_Initialize)() = dlsym(py_handle, "Py_Initialize");
    Py_Initialize();
    int (*PyRun_SimpleString)(const char *command);
    PyRun_SimpleString = dlsym(py_handle, "PyRun_SimpleString");
    PyRun_SimpleString("print('  (hello world from dlopen embedded python!)')");
    PyRun_SimpleString("print('  Address of True is %x' % id(True))");

    printf("Using dlopen'ed python interpreter to load a non-existent module\n");
    if (PyRun_SimpleString("import nosuchmodule") < 0) {
        printf("...failed, as expected\n");
    } else {
        printf("...succeeded, WTF?\n");
    }

    printf("Using dlopen'ed python interpreter to load an extension module\n");
    if (PyRun_SimpleString("import _regex") < 0) {
        printf("...failed\n");
    } else {
        printf("...succeeded\n");
    }

    void *plugin_handle = dlopen("./plugin.so", RTLD_LAZY | RTLD_LOCAL);
    check_dl(plugin_handle, "./plugin.so");
    void (*do_plugin_stuff)() = dlsym(plugin_handle, "do_plugin_stuff");
    do_plugin_stuff();
}
