/* copyright 2003 Jim Bublitz <jbublitz@nwinternet.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <stdarg.h>
#include <string.h>

// this just gets rid of a warning when Python.h redefines it
#undef _POSIX_C_SOURCE

#include "pythonize.h"

#define debug 0

ObjectRef::ObjectRef (ObjectRef *oi, PyObject *o)
{
    prevObject = oi;
    object     = o;
}


Pythonize::Pythonize ()
{
    pythonInit  = 1;
    objects    = NULL;

    if (debug) printf ("\n\nPythonize constructor -- pid = %i\n", getpid ());

    if (!Py_IsInitialized ())
    {
        PyEval_InitThreads ();
        Py_Initialize ();
        if (!Py_IsInitialized ())
        {
            pythonInit = 0;
            return;
        }

        if (debug) printf ("Python interpreter initialized!\n\n");

        // free the lock
        PyEval_ReleaseLock();
    }
}

bool Pythonize::runScript (const char *scriptPath)
{
    FILE *f;
    int res;

    if (debug) printf ("Running script: %s\n", scriptPath);

    if (scriptPath == NULL || strlen (scriptPath) == 0) return false;

    f = fopen (scriptPath, "r");
    if (f == NULL) return false;

    res = PyRun_SimpleFile (f, scriptPath);

    fclose (f);
    return res == 0;
}


PyObject * Pythonize::runFunction (PyObject *object, PyObject *args)
{

    if (!PyCallable_Check (object))
        return NULL;

    PyObject *res = PyObject_CallObject (object, args ? args : PyTuple_New (0));
    Py_XINCREF (res);

    return res;
}

void * Pythonize::runFunctionVoid (PyObject *object, PyObject *args)
{

    if (!PyCallable_Check (object))
        return NULL;

    PyObject *pyRes = PyObject_CallObject (object, args ? args : PyTuple_New (0));
    void *res = PyLong_AsVoidPtr (pyRes);

    return res;
}

bool Pythonize::runString (const char *str)
{

    if (str == NULL || strlen (str) == 0) return false;

    int res = PyRun_SimpleString (str);

    return res == 0;
}

bool Pythonize::appendToSysPath (const char* newPath)
{
    if (newPath == NULL || strlen (newPath) == 0) return false;

    char *fmtString = "import sys\nif not '%s' in sys.path:\n\tsys.path.append ('%s')\n"; //print sys.path\n";
    int length      = strlen (fmtString) + 2*strlen (newPath) + 1;
    char *line      =  new char [length];
    if (!line) return false;
    snprintf (line, length, fmtString, newPath, newPath);

    int res = PyRun_SimpleString (line);

    delete line;
    return res == 0;
}

PyObject *Pythonize::importModule (const char *moduleName)
{
    if (moduleName == NULL || strlen (moduleName) == 0) return NULL;

    PyObject *module = PyImport_ImportModule (moduleName);

    objects = new ObjectRef (objects, module);
    if (!objects) return NULL;

    return module;
}

void Pythonize::raiseException(const char *msg) {
    PyErr_SetString(PyExc_RuntimeError, msg);
}

Pythonize::~Pythonize ()
{

    if (debug) printf ("Pythonize destructor\n");
    ObjectRef *top;

    while (objects)
    {
        top = objects;
        objects = objects->prevObject;
        delete top;
    }
    if (debug) printf (" --- Objects destroyed\n");

    Py_Finalize();

    if (debug) printf (" --- Py_Finalized\n");
}

