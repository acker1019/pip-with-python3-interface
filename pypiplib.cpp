
#include <stdlib.h>
#include <Python.h>

#include <iostream>
#include "piplib.hpp"


template<typename T>
static T* listToArr(pip::ind* size, PyObject* list) {
    if(!PyList_CheckExact(list)) {
        printf("support native python list only.\n");
        return NULL;
    }
    pip::ind arrlen = (pip::ind)PyObject_Length(list);
    if (arrlen < 0) {
        printf("bad list lenth.\n");
        return NULL;
    }
    T* arr = new T[arrlen];
    if(arr == NULL) {
        delete[] arr;
        printf("out of memory.\n");
        return NULL;
    }
    for(pip::ind i = 0; i < arrlen; ++i) {
        PyObject* item = PyList_GetItem(list, i);
        if(PyFloat_CheckExact(item)) {
            arr[i] = (T)PyFloat_AsDouble(item);
        } else if(PyLong_CheckExact(item)) {
            arr[i] = (T)PyLong_AsLong(item);
        } else {
            delete[] arr;
            printf("support native python int/float only.\n");
            return NULL;
        }
    }// for
    *size = arrlen;
    return arr;
}// listToArr

static PyObject* stepToConverge(PyObject *self, PyObject *args) {
    PyObject* float_list;
    pip::ind arrlen;
    double* arr;
    
    double th;

    // 1. from py to c++
    if(!PyArg_ParseTuple(args, "Od", &float_list, &th)) {
        printf("bad parameters.\n");
        return NULL;
    }
    arr = listToArr<double>(&arrlen, float_list);
    if(arr == NULL) {
        return NULL;
    }

    // 2. do c++ calculation
    pip::ind steps = pip::stepToConverge(arr, arrlen, th);
    delete[] arr;

    return Py_BuildValue("i", steps);
}// stepToConverge

static PyObject* pipPattern(PyObject *self, PyObject *args) {
    PyObject* float_list;
    pip::ind arrlen;
    double* arr;
    
    pip::ind n;

    // 1. from py to c++
    if(!PyArg_ParseTuple(args, "OI", &float_list, &n)) {
        printf("bad parameters.\n");
        return NULL;
    }
    arr = listToArr<double>(&arrlen, float_list);
    if(arr == NULL) {
        return NULL;
    }

    // 2. do c++ calculation
    double* pattern = pip::pipPattern(arr, arrlen, n);
    delete[] arr;

    // 3. from c++ to py
    float_list = PyList_New(n);
    for(pip::ind i = 0; i < n; ++i) {
      PyList_SetItem(float_list, i, Py_BuildValue("d", pattern[i]));
    }// for

    // 4. release
    delete[] pattern;

    return float_list;
}// pipPattern

static PyObject* pipPattern_axis(PyObject *self, PyObject *args) {
    PyObject* float_list;
    pip::ind arrlen;
    double* arr;
    
    pip::ind n;

    // 1. from py to c++
    if(!PyArg_ParseTuple(args, "OI", &float_list, &n)) {
        printf("bad parameters.\n");
        return NULL;
    }
    arr = listToArr<double>(&arrlen, float_list);
    if(arr == NULL) {
        return NULL;
    }

    // 2. do c++ calculation
    std::tuple<pip::ind*, double*> tp = pip::pipPattern_axis(arr, arrlen, n);
    delete[] arr;
    pip::ind* patternX = std::get<0>(tp);
    double* patternY = std::get<1>(tp);

    // 3. from c++ to py
    PyObject* ind_list_x = PyList_New(n);
    for(pip::ind i = 0; i < n; ++i) {
      PyList_SetItem(ind_list_x, i, Py_BuildValue("I", patternX[i]));
    }// for

    PyObject* float_list_y = PyList_New(n);
    for(pip::ind i = 0; i < n; ++i) {
      PyList_SetItem(float_list_y, i, Py_BuildValue("d", patternY[i]));
    }// for

    // 4. release
    delete[] patternX;
    delete[] patternY;

    PyObject* out = PyTuple_New(2);
    PyTuple_SetItem(out, 0, ind_list_x);
    PyTuple_SetItem(out, 1, float_list_y);
    return out;
}// pipPattern_axis

static PyObject* pipPatternNext(PyObject *self, PyObject *args) {
    PyObject* ind_list;
    pip::ind n;
    pip::ind* xs;

    PyObject* float_list;
    pip::ind arrlen;
    double* arr;

    // 1. from py to c++
    if(!PyArg_ParseTuple(args, "OO", &ind_list, &float_list)) {
        printf("bad parameters.\n");
        return NULL;
    }
    xs = listToArr<pip::ind>(&n, ind_list);
    arr = listToArr<double>(&arrlen, float_list);

    // 2. do c++ calculation
    std::tuple<pip::ind, double, double, pip::ind> tp = pip::pipPatternNext(xs, n, arr, arrlen);

    // 3. release
    delete[] xs;
    delete[] arr;

    return Py_BuildValue("(IddI)", std::get<0>(tp), std::get<1>(tp), std::get<2>(tp), std::get<3>(tp));
}// pipPatternNext

static PyObject* pipPatternPrev(PyObject *self, PyObject *args) {
    PyObject* ind_list;
    pip::ind n;
    pip::ind* xs;

    PyObject* float_list;
    pip::ind arrlen;
    double* arr;

    // 1. from py to c++
    if(!PyArg_ParseTuple(args, "OO", &ind_list, &float_list)) {
        printf("bad parameters.\n");
        return NULL;
    }
    xs = listToArr<pip::ind>(&n, ind_list);
    arr = listToArr<double>(&arrlen, float_list);

    // 2. do c++ calculation
    std::tuple<pip::ind, double, double, pip::ind> tp = pip::pipPatternPrev(xs, n, arr, arrlen);

    // 3. release
    delete[] xs;
    delete[] arr;

    return Py_BuildValue("(IddI)", std::get<0>(tp), std::get<1>(tp), std::get<2>(tp), std::get<3>(tp));
}// pipPatternPrev

static PyMethodDef Methods[] = {
    {"pipPattern", pipPattern, METH_VARARGS, "Return a pip pattern from a given series."},
    {"pipPattern_axis", pipPattern_axis, METH_VARARGS, "Return a pip pattern with x-axis from a given series."},
    {"pipPatternNext", pipPatternNext, METH_VARARGS, "Return next significant point from a given pattern and series."},
    {"pipPatternPrev", pipPatternPrev, METH_VARARGS, "Return previous significant point from a given pattern and series."},
    {"stepToConverge", stepToConverge, METH_VARARGS, "Return how much to converge with a given threshold."},
    {NULL, NULL, 0, NULL}
};// PyMethodDef GreetMethods

static struct PyModuleDef pypiplib = {
		PyModuleDef_HEAD_INIT,
		"pypiplib", /* name of module */
		"",      /* module documentation, may be NULL */
		-1,      /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
		Methods
};// struct greet

PyMODINIT_FUNC PyInit_pypiplib(void) {
		return PyModule_Create(&pypiplib);
}// PyInit_greet
