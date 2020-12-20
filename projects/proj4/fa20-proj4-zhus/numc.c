#include "numc.h"
#include <structmember.h>

PyTypeObject Matrix61cType;

/* Helper functions for initalization of matrices and vectors */

/*
 * Return a tuple given rows and cols
 */
PyObject *get_shape(int rows, int cols) {
  if (rows == 1 || cols == 1) {
    return PyTuple_Pack(1, PyLong_FromLong(rows * cols));
  } else {
    return PyTuple_Pack(2, PyLong_FromLong(rows), PyLong_FromLong(cols));
  }
}
/*
 * Matrix(rows, cols, low, high). Fill a matrix random double values
 */
int init_rand(PyObject *self, int rows, int cols, unsigned int seed, double low,
              double high) {
    matrix *new_mat;
    int alloc_failed = allocate_matrix(&new_mat, rows, cols);
    if (alloc_failed) return alloc_failed;
    rand_matrix(new_mat, seed, low, high);
    ((Matrix61c *)self)->mat = new_mat;
    ((Matrix61c *)self)->shape = get_shape(new_mat->rows, new_mat->cols);
    return 0;
}

/*
 * Matrix(rows, cols, val). Fill a matrix of dimension rows * cols with val
 */
int init_fill(PyObject *self, int rows, int cols, double val) {
    matrix *new_mat;
    int alloc_failed = allocate_matrix(&new_mat, rows, cols);
    if (alloc_failed)
        return alloc_failed;
    else {
        fill_matrix(new_mat, val);
        ((Matrix61c *)self)->mat = new_mat;
        ((Matrix61c *)self)->shape = get_shape(new_mat->rows, new_mat->cols);
    }
    return 0;
}

/*
 * Matrix(rows, cols, 1d_list). Fill a matrix with dimension rows * cols with 1d_list values
 */
int init_1d(PyObject *self, int rows, int cols, PyObject *lst) {
    if (rows * cols != PyList_Size(lst)) {
        PyErr_SetString(PyExc_ValueError, "Incorrect number of elements in list");
        return -1;
    }
    matrix *new_mat;
    int alloc_failed = allocate_matrix(&new_mat, rows, cols);
    if (alloc_failed) return alloc_failed;
    int count = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            set(new_mat, i, j, PyFloat_AsDouble(PyList_GetItem(lst, count)));
            count++;
        }
    }
    ((Matrix61c *)self)->mat = new_mat;
    ((Matrix61c *)self)->shape = get_shape(new_mat->rows, new_mat->cols);
    return 0;
}

/*
 * Matrix(2d_list). Fill a matrix with dimension len(2d_list) * len(2d_list[0])
 */
int init_2d(PyObject *self, PyObject *lst) {
    int rows = PyList_Size(lst);
    if (rows == 0) {
        PyErr_SetString(PyExc_ValueError,
                        "Cannot initialize numc.Matrix with an empty list");
        return -1;
    }
    int cols;
    if (!PyList_Check(PyList_GetItem(lst, 0))) {
        PyErr_SetString(PyExc_ValueError, "List values not valid");
        return -1;
    } else {
        cols = PyList_Size(PyList_GetItem(lst, 0));
    }
    for (int i = 0; i < rows; i++) {
        if (!PyList_Check(PyList_GetItem(lst, i)) ||
                PyList_Size(PyList_GetItem(lst, i)) != cols) {
            PyErr_SetString(PyExc_ValueError, "List values not valid");
            return -1;
        }
    }
    matrix *new_mat;
    int alloc_failed = allocate_matrix(&new_mat, rows, cols);
    if (alloc_failed) return alloc_failed;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            set(new_mat, i, j,
                PyFloat_AsDouble(PyList_GetItem(PyList_GetItem(lst, i), j)));
        }
    }
    ((Matrix61c *)self)->mat = new_mat;
    ((Matrix61c *)self)->shape = get_shape(new_mat->rows, new_mat->cols);
    return 0;
}

/*
 * This deallocation function is called when reference count is 0
 */
void Matrix61c_dealloc(Matrix61c *self) {
    deallocate_matrix(self->mat);
    Py_TYPE(self)->tp_free(self);
}

/* For immutable types all initializations should take place in tp_new */
PyObject *Matrix61c_new(PyTypeObject *type, PyObject *args,
                        PyObject *kwds) {
    /* size of allocated memory is tp_basicsize + nitems*tp_itemsize*/
    Matrix61c *self = (Matrix61c *)type->tp_alloc(type, 0);
    return (PyObject *)self;
}

/*
 * This matrix61c type is mutable, so needs init function. Return 0 on success otherwise -1
 */
int Matrix61c_init(PyObject *self, PyObject *args, PyObject *kwds) {
    /* Generate random matrices */
    if (kwds != NULL) {
        PyObject *rand = PyDict_GetItemString(kwds, "rand");
        if (!rand) {
            PyErr_SetString(PyExc_TypeError, "Invalid arguments");
            return -1;
        }
        if (!PyBool_Check(rand)) {
            PyErr_SetString(PyExc_TypeError, "Invalid arguments");
            return -1;
        }
        if (rand != Py_True) {
            PyErr_SetString(PyExc_TypeError, "Invalid arguments");
            return -1;
        }

        PyObject *low = PyDict_GetItemString(kwds, "low");
        PyObject *high = PyDict_GetItemString(kwds, "high");
        PyObject *seed = PyDict_GetItemString(kwds, "seed");
        double double_low = 0;
        double double_high = 1;
        unsigned int unsigned_seed = 0;

        if (low) {
            if (PyFloat_Check(low)) {
                double_low = PyFloat_AsDouble(low);
            } else if (PyLong_Check(low)) {
                double_low = PyLong_AsLong(low);
            }
        }

        if (high) {
            if (PyFloat_Check(high)) {
                double_high = PyFloat_AsDouble(high);
            } else if (PyLong_Check(high)) {
                double_high = PyLong_AsLong(high);
            }
        }

        if (double_low >= double_high) {
            PyErr_SetString(PyExc_TypeError, "Invalid arguments");
            return -1;
        }

        // Set seed if argument exists
        if (seed) {
            if (PyLong_Check(seed)) {
                unsigned_seed = PyLong_AsUnsignedLong(seed);
            }
        }

        PyObject *rows = NULL;
        PyObject *cols = NULL;
        if (PyArg_UnpackTuple(args, "args", 2, 2, &rows, &cols)) {
            if (rows && cols && PyLong_Check(rows) && PyLong_Check(cols)) {
                return init_rand(self, PyLong_AsLong(rows), PyLong_AsLong(cols), unsigned_seed, double_low,
                                 double_high);
            }
        } else {
            PyErr_SetString(PyExc_TypeError, "Invalid arguments");
            return -1;
        }
    }
    PyObject *arg1 = NULL;
    PyObject *arg2 = NULL;
    PyObject *arg3 = NULL;
    if (PyArg_UnpackTuple(args, "args", 1, 3, &arg1, &arg2, &arg3)) {
        /* arguments are (rows, cols, val) */
        if (arg1 && arg2 && arg3 && PyLong_Check(arg1) && PyLong_Check(arg2) && (PyLong_Check(arg3)
                || PyFloat_Check(arg3))) {
            if (PyLong_Check(arg3)) {
                return init_fill(self, PyLong_AsLong(arg1), PyLong_AsLong(arg2), PyLong_AsLong(arg3));
            } else
                return init_fill(self, PyLong_AsLong(arg1), PyLong_AsLong(arg2), PyFloat_AsDouble(arg3));
        } else if (arg1 && arg2 && arg3 && PyLong_Check(arg1) && PyLong_Check(arg2) && PyList_Check(arg3)) {
            /* Matrix(rows, cols, 1D list) */
            return init_1d(self, PyLong_AsLong(arg1), PyLong_AsLong(arg2), arg3);
        } else if (arg1 && PyList_Check(arg1) && arg2 == NULL && arg3 == NULL) {
            /* Matrix(rows, cols, 1D list) */
            return init_2d(self, arg1);
        } else if (arg1 && arg2 && PyLong_Check(arg1) && PyLong_Check(arg2) && arg3 == NULL) {
            /* Matrix(rows, cols, 1D list) */
            return init_fill(self, PyLong_AsLong(arg1), PyLong_AsLong(arg2), 0);
        } else {
            PyErr_SetString(PyExc_TypeError, "Invalid arguments");
            return -1;
        }
    } else {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments");
        return -1;
    }
}

/*
 * List of lists representations for matrices
 */
PyObject *Matrix61c_to_list(Matrix61c *self) {
    int rows = self->mat->rows;
    int cols = self->mat->cols;
    PyObject *py_lst = NULL;
    if (self->mat->is_1d) {  // If 1D matrix, print as a single list
        py_lst = PyList_New(rows * cols);
        int count = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                PyList_SetItem(py_lst, count, PyFloat_FromDouble(get(self->mat, i, j)));
                count++;
            }
        }
    } else {  // if 2D, print as nested list
        py_lst = PyList_New(rows);
        for (int i = 0; i < rows; i++) {
            PyList_SetItem(py_lst, i, PyList_New(cols));
            PyObject *curr_row = PyList_GetItem(py_lst, i);
            for (int j = 0; j < cols; j++) {
                PyList_SetItem(curr_row, j, PyFloat_FromDouble(get(self->mat, i, j)));
            }
        }
    }
    return py_lst;
}

PyObject *Matrix61c_class_to_list(Matrix61c *self, PyObject *args) {
    PyObject *mat = NULL;
    if (PyArg_UnpackTuple(args, "args", 1, 1, &mat)) {
        if (!PyObject_TypeCheck(mat, &Matrix61cType)) {
            PyErr_SetString(PyExc_TypeError, "Argument must of type numc.Matrix!");
            return NULL;
        }
        Matrix61c* mat61c = (Matrix61c*)mat;
        return Matrix61c_to_list(mat61c);
    } else {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments");
        return NULL;
    }
}

/*
 * Add class methods
 */
PyMethodDef Matrix61c_class_methods[] = {
    {"to_list", (PyCFunction)Matrix61c_class_to_list, METH_VARARGS, "Returns a list representation of numc.Matrix"},
    {NULL, NULL, 0, NULL}
};

/*
 * Matrix61c string representation. For printing purposes.
 */
PyObject *Matrix61c_repr(PyObject *self) {
    PyObject *py_lst = Matrix61c_to_list((Matrix61c *)self);
    return PyObject_Repr(py_lst);
}

/* NUMBER METHODS */

/*
 * Add the second numc.Matrix (Matrix61c) object to the first one. The first operand is
 * self, and the second operand can be obtained by casting `args`.
 */
PyObject *Matrix61c_add(Matrix61c* self, PyObject* args) {
    /* TODO: YOUR CODE HERE */

    // a + b: Element-wise sum of a and b. Returns a numc.Matrix object.
    // Throwing errors:
    //       TypeError if not both a and b are of type numc.Matrix.
    //       ValueError if a and b do not have the same dimensions.
    // Note: For any errors not specified, throw a runtime error (PyExc_RuntimeError).

    // TypeError if not both a and b are of type numc.Matrix
    if (!PyObject_TypeCheck(self, &Matrix61cType)) {
        PyErr_SetString(PyExc_TypeError, "self in Matrix61c_add() must be of type numc.Matrix!");
        return NULL;
    }
    if (!PyObject_TypeCheck(args, &Matrix61cType)) {
        PyErr_SetString(PyExc_TypeError, "second argument in Matrix61c_add() must be of type numc.Matrix!");
        return NULL;
    }
    Matrix61c* second = (Matrix61c*) args;
    
    // ValueError if a and b do not have the same dimensions.
    if (self->mat->rows != second->mat->rows || self->mat->cols != second->mat->cols) {
        PyErr_SetString(PyExc_ValueError, "Matrix61c_add(): a and b do not have the same dimensions.");
        return NULL;
    }
    // ValueError for invalid demensions.
    if (self->mat->rows < 1 ||  self->mat->cols < 1 || second->mat->rows < 1 || second->mat->cols < 1) {
        PyErr_SetString(PyExc_ValueError, "Matrix61c_add(): Invalid Dimensions");
        return NULL;
    }

    // add matrix
    matrix *result;
    int alloc_failed = allocate_matrix(&result, self->mat->rows, self->mat->cols);
    if (alloc_failed) {
        PyErr_SetString(PyExc_RuntimeError, "Matrix61c_add(): Failed to allocate memory for allocate_matrix()");
        return NULL;
    }
    
    // call matrix.c for add
    if (add_matrix(result, self->mat, second->mat) == -1) {
        PyErr_SetString(PyExc_RuntimeError, "Matrix61c_add(): add_matrix() Error");
        return NULL;
    }

    // final result (two members) 
    Matrix61c *final = (Matrix61c *) Matrix61c_new(&Matrix61cType, NULL, NULL);
    final->mat = result;
    final->shape = get_shape(result->rows, result->cols);
    return (PyObject*) final;
}

/*
 * Substract the second numc.Matrix (Matrix61c) object from the first one. The first operand is
 * self, and the second operand can be obtained by casting `args`.
 */
PyObject *Matrix61c_sub(Matrix61c* self, PyObject* args) {
    /* TODO: YOUR CODE HERE */

    // a - b: Element-wise subtraction of a and b. Returns a numc.Matrix object.
    // Throwing errors:
    //       TypeError if not both a and b are of type numc.Matrix.
    //       ValueError if a and b do not have the same dimensions.
    // Note: For any errors not specified, throw a runtime error (PyExc_RuntimeError).

    // TypeError if not both a and b are of type numc.Matrix
    if (!PyObject_TypeCheck(self, &Matrix61cType)) {
        PyErr_SetString(PyExc_TypeError, "self in Matrix61c_sub() must be of type numc.Matrix!");
        return NULL;
    }
    if (!PyObject_TypeCheck(args, &Matrix61cType)) {
        PyErr_SetString(PyExc_TypeError, "second argument in Matrix61c_sub() must be of type numc.Matrix!");
        return NULL;
    }
    Matrix61c* second = (Matrix61c*) args; 

    // ValueError if a and b do not have the same dimensions.
    if (self->mat->rows != second->mat->rows || self->mat->cols != second->mat->cols) {
        PyErr_SetString(PyExc_ValueError, " Matrix61c_sub(): a and b do not have the same dimensions.");
        return NULL;
    }
    // ValueError for invalid demensions.
    if (self->mat->rows < 1 ||  self->mat->cols < 1 || second->mat->rows < 1 || second->mat->cols < 1) {
        PyErr_SetString(PyExc_ValueError, " Matrix61c_sub(): Invalid Dimensions");
        return NULL;
    }

    // sub matrix
    matrix *result;
    int alloc_failed = allocate_matrix(&result, self->mat->rows, self->mat->cols);
    if (alloc_failed){
        PyErr_SetString(PyExc_RuntimeError, " Matrix61c_sub(): Failed to allocate memory for allocate_matrix()");
        return NULL;
    }
    
    // call matrix.c for sub
    if (sub_matrix(result, self->mat, second->mat) == -1) {
        PyErr_SetString(PyExc_RuntimeError, " Matrix61c_sub(): sub_matrix() Error");
        return NULL;
    }

    // final result (two members) 
    Matrix61c *final = (Matrix61c *) Matrix61c_new(&Matrix61cType, NULL, NULL);
    final->mat = result;
    final->shape = get_shape(result->rows, result->cols);
    return (PyObject*) final;
}

/*
 * NOT element-wise multiplication. The first operand is self, and the second operand
 * can be obtained by casting `args`.
 */
PyObject *Matrix61c_multiply(Matrix61c* self, PyObject *args) {
    /* TODO: YOUR CODE HERE */

    // a * b: Matrix multiplication of a and b. Returns a numc.Matrix object. 
    // Remember that this is a matrix multiplication, not an element-wise multiplication.
    // Throwing errors:
    //       TypeError if not both a and b are of type numc.Matrix.
    //       ValueError if a and b do not have the same dimensions.
    // Note: For any errors not specified, throw a runtime error (PyExc_RuntimeError). 
    
    // TypeError if not both a and b are of type numc.Matrix
    if (!PyObject_TypeCheck(self, &Matrix61cType)) {
        PyErr_SetString(PyExc_TypeError, "self in Matrix61c_multiply() must be of type numc.Matrix!");
        return NULL;
    }
    if (!PyObject_TypeCheck(args, &Matrix61cType)) {
        PyErr_SetString(PyExc_TypeError, "second argument in Matrix61c_multiply() must be of type numc.Matrix!");
        return NULL;
    }
    Matrix61c* second = (Matrix61c*) args; 
  
    // ValueError if a and b do not have the same dimensions.
    if (self->mat->cols != second->mat->rows) {
        PyErr_SetString(PyExc_ValueError, "Matrix61c_multiply(): a and b do not have the same dimensions.");
        return NULL;
    }
    // ValueError for invalid demensions.
    if (self->mat->rows < 1 ||  self->mat->cols < 1 || second->mat->rows < 1 || second->mat->cols < 1) {
        PyErr_SetString(PyExc_ValueError, "Matrix61c_multiply(): Invalid Dimensions");
        return NULL;
    }

    // multiply matrix
    matrix *result;
    int alloc_failed = allocate_matrix(&result, self->mat->rows, second->mat->cols);
    if (alloc_failed) {
        PyErr_SetString(PyExc_RuntimeError, "Matrix61c_multiply(): Failed to allocate memory for allocate_matrix()");
        return NULL;
    }
    
    // call matrix.c for multiply
    if (mul_matrix(result, self->mat, second->mat) == -1) {
        PyErr_SetString(PyExc_RuntimeError, "Matrix61c_multiply(): mul_matrix() Error");
        return NULL;
    }

    // final result (two members)
    Matrix61c *final = (Matrix61c *) Matrix61c_new(&Matrix61cType, NULL, NULL);
    final->mat = result;
    final->shape = get_shape(result->rows, result->cols);
    return (PyObject*) final;
}

/*
 * Negates the given numc.Matrix.
 */
PyObject *Matrix61c_neg(Matrix61c* self) {
    /* TODO: YOUR CODE HERE */

    // -a: Element-wise negation of a. Returns a numc.Matrix object.
     
    // TypeError if self is not of type numc.Matrix
    if (!PyObject_TypeCheck(self, &Matrix61cType)) {
        PyErr_SetString(PyExc_TypeError, "self in Matrix61c_neg() must be of type numc.Matrix!");
        return NULL;
    }

    // ValueError for invalid demensions.
    if (self->mat->rows < 1 ||  self->mat->cols < 1) {
        PyErr_SetString(PyExc_ValueError, "Matrix61c_neg(): Invalid Dimensions");
        return NULL;
    }

    // negate matrix
    matrix *result;
    int alloc_failed = allocate_matrix(&result, self->mat->rows, self->mat->cols);
    if (alloc_failed) {
        PyErr_SetString(PyExc_RuntimeError, "Matrix61c_neg(): Failed to allocate memory for allocate_matrix()");
        return NULL;
    }
    
    // call matrix.c for neg
    if (neg_matrix(result, self->mat) == -1) {
        PyErr_SetString(PyExc_RuntimeError, "Matrix61c_neg(): neg_matrix() Error");
        return NULL;
    }

    // final result (two members)
    Matrix61c *final = (Matrix61c *) Matrix61c_new(&Matrix61cType, NULL, NULL);
    final->mat = result;
    final->shape = get_shape(result->rows, result->cols);
    return (PyObject*) final;
}

/*
 * Take the element-wise absolute value of this numc.Matrix.
 */
PyObject *Matrix61c_abs(Matrix61c *self) {
    /* TODO: YOUR CODE HERE */

    // abs(a): Element-wise absolute value of a. Returns a numc.Matrix object.
    
    // TypeError if self is not of type numc.Matrix
    if (!PyObject_TypeCheck(self, &Matrix61cType)) {
        PyErr_SetString(PyExc_TypeError, "self in Matrix61c_abs() must be of type numc.Matrix!");
        return NULL;
    }

    // ValueError for invalid demensions.
    if (self->mat->rows < 1 ||  self->mat->cols < 1) {
        PyErr_SetString(PyExc_ValueError, "Matrix61c_abs(): Invalid Dimensions");
        return NULL;
    }

    // neg matrix
    matrix *result;
    int alloc_failed = allocate_matrix(&result, self->mat->rows, self->mat->cols);
    if (alloc_failed){
        PyErr_SetString(PyExc_RuntimeError, "Matrix61c_abs(): Failed to allocate memory for allocate_matrix()");
        return NULL;
    }
    
    // call matrix.c for abs
    if (abs_matrix(result, self->mat) == -1) {;
        PyErr_SetString(PyExc_RuntimeError, "Matrix61c_abs(): abs_matrix() Error");
        return NULL;
    }

    // final result (two members)
    Matrix61c *final = (Matrix61c *) Matrix61c_new(&Matrix61cType, NULL, NULL);
    final->mat = result;
    final->shape = get_shape(result->rows, result->cols);
    return (PyObject*) final;
}

/*
 * Raise numc.Matrix (Matrix61c) to the `pow`th power. You can ignore the argument `optional`.
 */
PyObject *Matrix61c_pow(Matrix61c *self, PyObject *pow, PyObject *optional) {
    /* TODO: YOUR CODE HERE */

    // a ** pow: Raise a to the powth power. a to the 0th power is the identity matrix 
    // (1 on the top left to bottom right diagonal and 0 everywhere else). 
    // Returns a numc.Matrix object. 
    // This operator is defined in terms of matrix multiplication, not element-wise multiplication.
    //
    // Throwing errors:
    //      TypeError if a is not of type numc.Matrix or pow is not an integer.
    //      ValueError if a is not a square matrix or if pow is negative.

    // TypeError if self is not of type numc.Matrix
    if (!PyObject_TypeCheck(self, &Matrix61cType)) {
        PyErr_SetString(PyExc_TypeError, "self in Matrix61c_pow() must be of type numc.Matrix!");
        return NULL;
    }
    // TypeError if pow is not an integer
    if (!PyLong_Check(pow)) {
        PyErr_SetString(PyExc_TypeError, "Matrix61c_pow(): power must be an integer!");
        return NULL;
    }

    // ValueError for invalid demensions.
    if (self->mat->rows < 1 ||  self->mat->cols < 1) {
        PyErr_SetString(PyExc_ValueError, "Matrix61c_pow(): Invalid Dimensions");
        return NULL;
    }
    // ValueError for invalid demensions.
    if (self->mat->rows != self->mat->cols) {
        PyErr_SetString(PyExc_ValueError, "Matrix61c_pow(): row and col must be the same!");
        return NULL;
    }

    // the power
    int power = PyLong_AsLong(pow); 
    // ValueError if pow is negative.
    if (power < 0) {
        PyErr_SetString(PyExc_ValueError, "Matrix61c_pow(): power is negative!");
        return NULL;
    }

    // power matrix
    matrix *result;
    int alloc_failed = allocate_matrix(&result, self->mat->rows, self->mat->cols);
    if (alloc_failed){
        PyErr_SetString(PyExc_TypeError, "Matrix61c_pow(): Failed to allocate memory for allocate_matrix()");
        return NULL;
    }
    
    // call matrix.c for pow
    if (pow_matrix(result, self->mat, power) == -1) {;
        PyErr_SetString(PyExc_TypeError, "Matrix61c_pow(): pow_matrix() Error");
        return NULL;
    }

    // final result (two members)
    Matrix61c *final = (Matrix61c *) Matrix61c_new(&Matrix61cType, NULL, NULL);
    final->mat = result;
    final->shape = get_shape(result->rows, result->cols);
    return (PyObject*) final;
}

/*
 * Create a PyNumberMethods struct for overloading operators with all the number methods you have
 * define. You might find this link helpful: https://docs.python.org/3.6/c-api/typeobj.html
 */
PyNumberMethods Matrix61c_as_number = {
    /* TODO: YOUR CODE HERE */
    // PyNumberMethods at https://docs.python.org/3.6/c-api/typeobj.html
    //    This structure holds pointers to the functions which an object uses to implement the number protocol. 
    //    Each function is used by the function of similar name documented in the Number Protocol section.
    // 
    (binaryfunc) Matrix61c_add,             // binaryfunc nb_add;
    (binaryfunc) Matrix61c_sub,             // binaryfunc nb_subtract;
    (binaryfunc) Matrix61c_multiply,        // binaryfunc nb_multiply;
    NULL,                                   // binaryfunc nb_remainder; (unused)
    NULL,                                   // binaryfunc nb_divmod; (unused)
    (ternaryfunc) Matrix61c_pow,            // ternaryfunc nb_power;
    (unaryfunc)  Matrix61c_neg,             // unaryfunc nb_negative;
    NULL,                                   // unaryfunc nb_positive; (unused)
    (unaryfunc)  Matrix61c_abs,             // unaryfunc nb_absolute;
    NULL,                                   // No more
};


/* INSTANCE METHODS */

/*
 * Given a numc.Matrix self, parse `args` to (int) row, (int) col, and (double/int) val.
 * Return None in Python (this is different from returning null).
 */
PyObject *Matrix61c_set_value(Matrix61c *self, PyObject* args) {
    /* TODO: YOUR CODE HERE */

    //set(self, i, j, val): Set self’s entry at the ith row and jth column to val.
    //  Throwing errors:
    //      TypeError if the number of arguments parsed from args is not 3, if i and j are not integers, or if val is not a float or int.
    //      IndexError if i or j or both are out of range.
    // 
    // https://docs.python.org/3/c-api/exceptions.html has the detailed exception types such as type error or index error
    //
    // int PyArg_UnpackTuple(PyObject *args, const char *name, Py_ssize_t min, Py_ssize_t max, ...)
    // https://docs.python.org/3/c-api/arg.html
    
    PyObject *rowArg = NULL;
    PyObject *colArg = NULL;
    PyObject *valArg = NULL;
    // throw a type error if # of args is not 3.
    if (!PyArg_UnpackTuple(args, "args", 3, 3, &rowArg, &colArg, &valArg)) {   
        PyErr_SetString(PyExc_TypeError, "Matrix61c_set_value(): Number of arguments parsed from args is not 3.");
        return NULL;
    }

    // throw a type error if i and j are not integers, or if val is not a float or int.
    // https://docs.python.org/3/c-api/long.html (int is a subtype of Long - see examples above)
    if (!(rowArg && colArg && valArg && PyLong_Check(rowArg) && PyLong_Check(colArg) && (PyLong_Check(valArg) || PyFloat_Check(valArg)))) {
        PyErr_SetString(PyExc_TypeError, "Matrix61c_set_value(): Arguments are of wrong types.");
        return NULL;
    }

    // throw an index error if row or col is out of range
    int row = PyLong_AsLong(rowArg);
    int col = PyLong_AsLong(colArg);
    if (row < 0 || col < 0 || self->mat->rows <= row  || self->mat->cols <= col) {
        PyErr_SetString(PyExc_IndexError, "Matrix61c_set_value(): row or col is out of range!");
        return NULL;
    }

    // Normal...call set() in matrix.c
    if (PyLong_Check(valArg)) {
        int val = PyLong_AsLong(valArg);
        set(self->mat, row, col, val);
    } 
    if (PyFloat_Check(valArg)) {
        double val = PyFloat_AsDouble(valArg);
        set(self->mat, row, col, val);
    }    
    return Py_None;
}

/*
 * Given a numc.Matrix `self`, parse `args` to (int) row and (int) col.
 * Return the value at the `row`th row and `col`th column, which is a Python
 * float/int.
 */
PyObject *Matrix61c_get_value(Matrix61c *self, PyObject* args) {
    /* TODO: YOUR CODE HERE */

    //get(self, i, j): Returns the entry at the ith row and jth column. 
    //  Throw a type error if the number of arguments parsed from args is not 2 or if the arguments are of the wrong types. 
    //  Throw an index error if either i, j, or both are out of range. 
    //Return value is a Python float.
    // 
    // https://docs.python.org/3/c-api/exceptions.html has the detailed exception types such as type error or index error
    //
    // int PyArg_UnpackTuple(PyObject *args, const char *name, Py_ssize_t min, Py_ssize_t max, ...)
    // https://docs.python.org/3/c-api/arg.html

    PyObject *rowArg = NULL;
    PyObject *colArg = NULL;
    // throw a type error if # of args is not 2
    if (!PyArg_UnpackTuple(args, "args", 2, 2, &rowArg, &colArg)) {   
        PyErr_SetString(PyExc_TypeError, "Matrix61c_get_value(): Number of arguments parsed from args is not 2.");
        return NULL;
    }

    // throw a type error if the arguments are of the wrong types
    // https://docs.python.org/3/c-api/long.html
    if (!(rowArg && colArg && PyLong_Check(rowArg) && PyLong_Check(colArg))) {
        PyErr_SetString(PyExc_TypeError, "Matrix61c_get_value(): the arguments are of wrong types.");
        return NULL;
    }

    // throw an index error if the row or col is invalid
    int row = PyLong_AsLong(rowArg);
    int col = PyLong_AsLong(colArg);
    if (row < 0 || col < 0 || self->mat->rows <= row  || self->mat->cols <= col) {
        PyErr_SetString(PyExc_IndexError, "Matrix61c_get_value(): the row or col is invalid!");
        return NULL;
    }

    // normal...call get() in matrix.c
    // https://docs.python.org/3/c-api/float.html
    double result = get(self->mat, row, col);
    return PyFloat_FromDouble(result);
}

/*
 * Create an array of PyMethodDef structs to hold the instance methods.
 * Name the python function corresponding to Matrix61c_get_value as "get" and Matrix61c_set_value
 * as "set"
 * You might find this link helpful: https://docs.python.org/3.6/c-api/structures.html
 */
PyMethodDef Matrix61c_methods[] = {
    /* TODO: YOUR CODE HERE */

    // PyMethodDef struct is used to describe a method of an extension type.
    // This structure has four fields: 
    // 1. ml_name: char *       (name of the method)
    // 2. ml_meth: PyCFuntion   (pointer to the C impllementation; cast to PyCFuntion)
    // 3. ml_flags: int         (flags bits indicating how the call should be constructed)
    //                          METH_VARARGS: typical and expects two PyObject* values (self and args -- all arguments). 
    //                          METH_KEYWORDS: for type PyCFunctionWithKeywords, 3 parameters (self, args, and a dictionary)
    //                          METHJ_NOARGS: without parameters
    //                          METH_O: Methods with a single object argument can be listed with the METH_O flag.           
    // 4. ml_doc: char *        (points to the contents of the docstring)
    {"get", (PyCFunction) Matrix61c_get_value, METH_VARARGS, NULL},
    {"set", (PyCFunction) Matrix61c_set_value, METH_VARARGS, NULL},
    {NULL, NULL, 0, NULL}
};

/* INDEXING */

/*
 * Given a numc.Matrix `self`, index into it with `key`. Return the indexed result.
 */
PyObject *Matrix61c_subscript(Matrix61c* self, PyObject* key) {
    /* TODO: YOUR CODE HERE */
    // Error Checks
    if (!PyObject_TypeCheck(self, &Matrix61cType)) {
        PyErr_SetString(PyExc_TypeError, "self in Matrix61c_subscript() must be of type numc.Matrix!");
        return NULL;
    }
    if (self == NULL) {
        PyErr_SetString(PyExc_ValueError, "self in Matrix61c_subscript() must not be NULL!");
        return NULL;
    }

    // For a 1D matrix, the key could either be an integer or a single slice
    if (self->mat->rows == 1 || self->mat->cols == 1) {
        // TypeError if key is not an integer or a slice.
        if (!(PyLong_Check(key) || PySlice_Check(key))) {
            PyErr_SetString(PyExc_TypeError, "Matrix61c_subscript(): Key must be either an integer or a single slice for 1D matrix");
            return NULL;
        }

        // if it's an integer    
        if (PyLong_Check(key)) {
            int index = PyLong_AsLong(key);
            // IndexError if key is an integer but is out of range
            if (index < 0 || (self->mat->rows == 1 && index >= self->mat->cols) 
                          || (self->mat->cols == 1 && index >= self->mat->rows)) {
                PyErr_SetString(PyExc_IndexError, "Matrix61c_subscript(): key as an integer is out of range for 1D matrix.");
                return NULL;
            }
            // return a float object accordingly
            return (self->mat->rows == 1)? PyFloat_FromDouble(get(self->mat, 0, index)) : 
                                           PyFloat_FromDouble(get(self->mat, index, 0));
        } else {  // if it is a slice
            Py_ssize_t length = (self->mat->rows == 1) ? self->mat->cols : self->mat->rows;
            Py_ssize_t start = 0, stop = 0, step = 0, slicelength = 0;
            if (PySlice_GetIndicesEx(key, length, &start, &stop, &step, &slicelength) < 0) {
                PyErr_SetString(PyExc_TypeError, "Matrix61c_subscript(): Unable to get indices from the slice in the 1D matrix");
                return NULL;
            }
            // step_size is not 1 or the length of slice is < 1
            if (step != 1 || slicelength < 1) {
                PyErr_SetString(PyExc_ValueError, "Matrix61c_subscript(): step_size is not 1 or the length of slice is < 1 for the 1D matrix");
                return NULL;
            }

            // if key is one integer slice, and the integers are out of range.
             if (start < 0 || (self->mat->rows == 1 && start >= self->mat->cols) ||
                              (self->mat->cols == 1 && start >= self->mat->rows) ||
                 stop <= 0 || (self->mat->rows == 1 && stop  >  self->mat->cols) ||
                              (self->mat->cols == 1 && stop  >  self->mat->rows) ||
                 stop <= start /*maybe more conditions - should be ok*/ ) {
                PyErr_SetString(PyExc_IndexError, "Matrix61c_subscript(): key is a slice that is out of range for the 1D matrix");
                return NULL;
            }

            // returns a single number if the resulting slice is 1 by 1
            if (stop == start + 1) {
                return (self->mat->rows == 1)?  PyFloat_FromDouble(get(self->mat, 0, start)) : 
                                                PyFloat_FromDouble(get(self->mat, start, 0));
            } else {
                // it is a slice from the 1D matrix
                matrix *result;
                int ref_failed = (self->mat->rows == 1) ? 
                                allocate_matrix_ref(&result, self->mat, 0, start, 1, stop - start) :
                                allocate_matrix_ref(&result, self->mat, start, 0, stop - start, 1);
                if (ref_failed) {
                    PyErr_SetString(PyExc_RuntimeError, "Matrix61c_subscript(): allocate_matrix_ref() runtime error for 1D matrix");
                    return NULL;
                }
            
                Matrix61c *final = (Matrix61c *) Matrix61c_new(&Matrix61cType, NULL, NULL);
                final->mat = result;
                final->shape = get_shape(result->rows, result->cols);
                return (PyObject*) final;
            }
        } 
    }

    // 2D matrix: TypeError if key is not an integer, a slice, or a length-2 tuple of slices/ints.
    if (!(PyLong_Check(key) || PySlice_Check(key) || PyTuple_CheckExact(key))) {
        PyErr_SetString(PyExc_TypeError, "Matrix61c_subscript(): key is not an integer, a slice, or a tuple for 2D matrix");
        return NULL;
    }
    // 2D matrix if key is an integer    
    if (PyLong_Check(key)) {
        int row = PyLong_AsLong(key);
        // IndexError if key (row) is an integer but is out of range
        if (row < 0 || row >= self->mat->rows) {
            PyErr_SetString(PyExc_IndexError, "Matrix61c_subscript(): key as the row is out of range for 2D matrix.");
            return NULL;
        }

        // return a matrix
        matrix *result;
        int ref_failed = allocate_matrix_ref(&result, self->mat, row, 0, 1, self->mat->cols);                            
        if (ref_failed) {
            PyErr_SetString(PyExc_RuntimeError, "Matrix61c_subscript(): allocate_matrix_ref() runtime error in 2D matrix");
            return NULL;
        }
           
        Matrix61c *final = (Matrix61c *) Matrix61c_new(&Matrix61cType, NULL, NULL);
        final->mat = result;
        final->shape = get_shape(1, result->cols);
        return (PyObject*) final;
    } else if (PySlice_Check(key)) {
        // for a single slice
        Py_ssize_t length = self->mat->rows;
        Py_ssize_t start = 0, stop = 0, step = 0, slicelength = 0;
        if (PySlice_GetIndicesEx(key, length, &start, &stop, &step, &slicelength) < 0) {
            PyErr_SetString(PyExc_TypeError, "Matrix61c_subscript(): Unable to get indices from the single slice in the 2D matrix");
            return NULL;
        }

        // step_size is not 1 or the length of slice is < 1
        if (step != 1 || slicelength < 1) {
            PyErr_SetString(PyExc_ValueError, "Matrix61c_subscript(): step_size is not 1 or the length of slice is < 1 for the 2D matrix");
            return NULL;
        }

        // if key is one integer slice, and the integers are out of range.
        if (start < 0 || start >= self->mat->rows ||
            stop <= 0 || stop  >  self->mat->rows ||
            stop <= start /*maybe more conditions - should be ok*/ ) {
                PyErr_SetString(PyExc_IndexError, "Matrix61c_subscript(): key is a slice that is out of range for the 2D matrix");
                return NULL;
        }

        matrix *result;
        int ref_failed = allocate_matrix_ref(&result, self->mat, start, 0, stop - start, self->mat->cols); 
        if (ref_failed) {
            PyErr_SetString(PyExc_RuntimeError, "Matrix61c_subscript(): allocate_matrix_ref() runtime error in 2D matrix");
            return NULL;
        }

        Matrix61c *final = (Matrix61c *) Matrix61c_new(&Matrix61cType, NULL, NULL);
        final->mat = result;
        final->shape = get_shape(result->rows, result->cols);
        return (PyObject*) final;
    } else {     
        // It must be a length-2 tuple of slices/ints.
        if (PyTuple_CheckExact(key) && PyTuple_GET_SIZE(key) != 2) {
            PyErr_SetString(PyExc_TypeError, "Matrix61c_subscript(): key is not a length-2 tuple of slices/ints for 2D matrix");
            return NULL;
        }
        // two tuple objects can be (int, int), (int slice), (slice, int) or (slice, slice)
        PyObject *tuple1 = PyTuple_GetItem(key, 0);
        PyObject *tuple2 = PyTuple_GetItem(key, 1);
        if (!tuple1 || !tuple2) {
            PyErr_SetString(PyExc_TypeError, "Matrix61c_subscript(): either tuple is empty in the length-2 tuple of slices/ints for 2D matrix");
            return NULL;
        }
        if (!((PyLong_Check(tuple1) && PyLong_Check (tuple2)) || (PySlice_Check(tuple1) && PySlice_Check(tuple2)) ||
              (PyLong_Check(tuple1) && PySlice_Check(tuple2)) || (PySlice_Check(tuple1) && PyLong_Check (tuple2))) ) {
            PyErr_SetString(PyExc_TypeError, "Matrix61c_subscript(): must be (int, int), (int slice), (slice, int) or (slice, slice) for 2D matrix");
            return NULL;
        }

        // locations for the slice
        int fromRow = 0, fromCol = 0, toRow = 0, toCol = 0;
        if (PyLong_Check(tuple1)) {        
            int row = PyLong_AsLong(tuple1);
            // IndexError if key (row) is an integer but is out of range
            if (row < 0 || row >= self->mat->rows) {
                PyErr_SetString(PyExc_IndexError, "Matrix61c_subscript(): row as an integer in tuple1 is out of range for 2D matrix.");
                return NULL;
            }
            fromRow = row; 
            toRow = row;
        } else { // a slice
            Py_ssize_t length = self->mat->rows;
            Py_ssize_t start = 0, stop = 0, step = 0, slicelength = 0;
            if (PySlice_GetIndicesEx(tuple1, length, &start, &stop, &step, &slicelength) < 0) {
                PyErr_SetString(PyExc_TypeError, "Matrix61c_subscript(): Unable to get indices from the tuple1 in the 2D matrix");
                return NULL;
            }

            // step_size is not 1 or the length of slice is < 1
            if (step != 1 || slicelength < 1) {
                PyErr_SetString(PyExc_ValueError, "Matrix61c_subscript(): tuple1 -> step_size is not 1 or the length of slice is < 1 for the 2D matrix");
                return NULL;
            }
            // if key is one integer slice, and the integera are out of range.
            if (start < 0 || start >= self->mat->rows ||
                stop <= 0 || stop >  self->mat->rows ||
                stop <= start /*maybe more conditions??*/ ) {
                    PyErr_SetString(PyExc_IndexError, "Matrix61c_subscript(): tuple1 -> key is a slice that is out of range for the 2D matrix");
                    return NULL;
            }
            fromRow = start; 
            toRow = stop - 1;
        }   
        // for tuple2
        if (PyLong_Check(tuple2)) {        
            int col = PyLong_AsLong(tuple2);
            // IndexError if key (col) is an integer but is out of range
            if (col < 0 || col >= self->mat->cols) {
                PyErr_SetString(PyExc_IndexError, "Matrix61c_subscript(): col as an integer in tuple2 is out of range for 2D matrix.");
                return NULL;
            }
            fromCol = col; 
            toCol   = col;
        } else { // a slice
            Py_ssize_t length = self->mat->cols;
            Py_ssize_t start = 0, stop = 0, step = 0, slicelength = 0;
            if (PySlice_GetIndicesEx(tuple2, length, &start, &stop, &step, &slicelength) < 0) {
                PyErr_SetString(PyExc_TypeError, "Matrix61c_subscript(): Unable to get indices from the tuple2 in the 2D matrix");
                return NULL;
            }

            // step_size is not 1 or the length of slice is < 1
            if (step != 1 || slicelength < 1) {
                PyErr_SetString(PyExc_ValueError, "Matrix61c_subscript(): tuple2 -> step_size is not 1 or the length of slice is < 1 for the 2D matrix");
                return NULL;
            }
            // if key is one integer slice, and the integera are out of range.
            if (start < 0 || start >= self->mat->cols ||
                stop <= 0 || stop >  self->mat->cols ||
                stop <= start /*maybe more conditions??*/ ) {
                    PyErr_SetString(PyExc_IndexError, "Matrix61c_subscript(): tuple2 -> key is a slice that is out of range for the 2D matrix");
                    return NULL;
            }
            fromCol = start; 
            toCol   = stop - 1;
        }   
    
        // returns a single number if the resulting matrix is 1 by 1
        if (fromRow == toRow && fromCol == toCol) {
            return PyFloat_FromDouble(get(self->mat, fromRow, fromCol));
        } else {
            // it is a slice from the 2D matrix
            matrix *result;
            int ref_failed = allocate_matrix_ref(&result, self->mat, fromRow, fromCol, toRow - fromRow + 1, toCol - fromCol + 1); 
            if (ref_failed) {
                PyErr_SetString(PyExc_RuntimeError, "Matrix61c_subscript(): allocate_matrix_ref() runtime error in 2D matrix");
                return NULL;
            }

            Matrix61c *final = (Matrix61c *) Matrix61c_new(&Matrix61cType, NULL, NULL);
            final->mat = result;
            final->shape = get_shape(result->rows, result->cols);
            return (PyObject*) final;
        }    
    }
}

/*
 * Given a numc.Matrix `self`, index into it with `key`, and set the indexed result to `v`.
 */
int Matrix61c_set_subscript(Matrix61c* self, PyObject *key, PyObject *v) {
    /* TODO: YOUR CODE HERE */
    // Error Checks
    if (!PyObject_TypeCheck(self, &Matrix61cType)) {
        PyErr_SetString(PyExc_TypeError, "self in Matrix61c_set_subscript() must be of type numc.Matrix!");
        return -1;
    }
    if (self == NULL) {
        PyErr_SetString(PyExc_ValueError, "self in Matrix61c_set_subscript() must not be NULL!");
        return -1;
    }

    // For a 1D matrix, the key could either be an integer or a single slice
    if (self->mat->rows == 1 || self->mat->cols == 1) {
        // TypeError if key is not an integer or a slice.
        if (!(PyLong_Check(key) || PySlice_Check(key))) {
            PyErr_SetString(PyExc_TypeError, "Matrix61c_set_subscript(): Key must be either an integer or a single slice for 1D matrix");
            return -1;
        }

        // if it's an integer    
        if (PyLong_Check(key)) {
            int index = PyLong_AsLong(key);
            // IndexError if key is an integer but is out of range
            if (index < 0 || (self->mat->rows == 1 && index >= self->mat->cols) 
                          || (self->mat->cols == 1 && index >= self->mat->rows)) {
                PyErr_SetString(PyExc_IndexError, "Matrix61c_set_subscript(): key as an integer is out of range for 1D matrix.");
                return -1;
            }

            // v must be a float or int if 1x1
            if (!PyFloat_Check(v) && !PyLong_Check(v)) {
                PyErr_SetString(PyExc_TypeError, "Matrix61c_set_subscript(): value for 1D matrix is invalid (must be float or int)");
                return -1;
            }

            // set up the val as needed
            double val = PyLong_Check(v) ? PyLong_AsDouble(v) : PyFloat_AsDouble(v);
            if (self->mat->rows == 1) {
                set(self->mat, 0, index, val);
            } else { 
                set(self->mat, index, 0, val);
            }
            return 0; // success
        } else {
            // if it is a slice
            Py_ssize_t length = (self->mat->rows == 1) ? self->mat->cols : self->mat->rows;
            Py_ssize_t start = 0, stop = 0, step = 0, slicelength = 0;
            if (PySlice_GetIndicesEx(key, length, &start, &stop, &step, &slicelength) < 0) {
                PyErr_SetString(PyExc_TypeError, "Matrix61c_set_subscript(): Unable to get indices from the slice in the 1D matrix");
                return -1;
            }

            // step_size is not 1 or the length of slice is < 1
            if (step != 1 || slicelength < 1) {
                PyErr_SetString(PyExc_ValueError, "Matrix61c_subsMatrix61c_set_subscriptcript(): step_size is not 1 or the length of slice is < 1 for the 1D matrix");
                return -1;
            }

            // if key is one integer slice, and the integers are out of range.
             if (start < 0 || (self->mat->rows == 1 && start >= self->mat->cols) ||
                              (self->mat->cols == 1 && start >= self->mat->rows) ||
                 stop <= 0 || (self->mat->rows == 1 && stop  >  self->mat->cols) ||
                              (self->mat->cols == 1 && stop  >  self->mat->rows) ||
                 stop <= start /*maybe more conditions - should be ok*/ ) {
                PyErr_SetString(PyExc_IndexError, "Matrix61c_set_subscript(): key is a slice that is out of range for the 1D matrix");
                return -1;
            }

            // returns a single number if the resulting slice is 1 by 1
            if (stop == start + 1) {
                // v must be a float or int if 1x1
                if (!PyFloat_Check(v) && !PyLong_Check(v)) {
                    PyErr_SetString(PyExc_TypeError, "Matrix61c_set_subscript(): value for 1D matrix is invalid (must be float or int)");
                    return -1;
                }
                // set up the val as needed
                double val = PyLong_Check(v) ? PyLong_AsDouble(v) : PyFloat_AsDouble(v);
                if (self->mat->rows == 1) {
                    set(self->mat, 0, start, val);
                } else { 
                    set(self->mat, start, 0, val);
                }
                return 0; // success
            } else {
                // it is a slice from the 1D matrix
                if (!PyList_Check(v)) {
                    PyErr_SetString(PyExc_TypeError, "Matrix61c_set_subscript(): Value must be a list.");
                    return -1;
                }
                if (PyList_Size(v) != stop - start) {
                    PyErr_SetString(PyExc_ValueError, "Matrix61c_set_subscript(): Value has wrong length.");
                    return -1;
                }
                if (self->mat->rows == 1) {
                    for (int i = 0; i < stop - start; i++) {
                        if (!PyFloat_Check(PyList_GetItem(v, i)) && !PyLong_Check(PyList_GetItem(v, i))) {
                            PyErr_SetString(PyExc_ValueError, "Matrix61c_set_subscript(): Value must be float or integer");
                            return -1;
                        }
                        double val = PyLong_Check(PyList_GetItem(v, i)) ? PyLong_AsDouble(PyList_GetItem(v, i)) :
                                                                          PyFloat_AsDouble(PyList_GetItem(v, i));
                        set(self->mat, 0, start + i, val);
                    }    
                } else {
                    for (int i = 0; i < stop - start; i++) {
                        if (!PyFloat_Check(PyList_GetItem(v, i)) && !PyLong_Check(PyList_GetItem(v, i))) {
                            PyErr_SetString(PyExc_ValueError, "Matrix61c_set_subscript(): Value must be float or integer");
                            return -1;
                        }
                        double val = PyLong_Check(PyList_GetItem(v, i)) ? PyLong_AsDouble(PyList_GetItem(v, i)) :
                                                                          PyFloat_AsDouble(PyList_GetItem(v, i));
                        set(self->mat, start + i, 0, val);
                    }        
                }
                return 0; // success
            }
        } 
    } 

    // 2D matrix: TypeError if key is not an integer, a slice, or a length-2 tuple of slices/ints.
    if (!(PyLong_Check(key) || PySlice_Check(key) || PyTuple_CheckExact(key))) {
        PyErr_SetString(PyExc_TypeError, "Matrix61c_set_subscript(): key is not an integer, a slice, or a tuple for 2D matrix");
        return -1;
    }

    // 2D matrix if key is an integer    
    if (PyLong_Check(key)) {
        int row = PyLong_AsLong(key);
        // IndexError if key (row) is an integer but is out of range
        if (row < 0 || row >= self->mat->rows) {
            PyErr_SetString(PyExc_IndexError, "Matrix61c_set_subscript(): key as the row is out of range for 2D matrix.");
            return -1;
        }

        // the value must be a list
        if (!PyList_Check(v)) {
            PyErr_SetString(PyExc_TypeError, "Matrix61c_set_subscript(): Value must be a list (2D: int).");
            return -1;
        }
        if (PyList_Size(v) != self->mat->cols) {
            PyErr_SetString(PyExc_ValueError, "Matrix61c_set_subscript(): Value has wrong length (should #cols).");
            return -1;
        }
        // set each element in the row
        for (int i = 0; i < self->mat->cols; i++) {
            if (!PyFloat_Check(PyList_GetItem(v, i)) && !PyLong_Check(PyList_GetItem(v, i))) {
                    PyErr_SetString(PyExc_ValueError, "Matrix61c_set_subscript(): Value must be float or integer in list");
                    return -1;
            }
            double val = PyLong_Check(PyList_GetItem(v, i)) ? PyLong_AsDouble(PyList_GetItem(v, i)) :
                                                              PyFloat_AsDouble(PyList_GetItem(v, i));
            set(self->mat, row, i, val);
        }
        return 0; // success
    } else if PySlice_Check(key) {
        // for a single slice
        Py_ssize_t length = self->mat->rows;
        Py_ssize_t start = 0, stop = 0, step = 0, slicelength = 0;
        if (PySlice_GetIndicesEx(key, length, &start, &stop, &step, &slicelength) < 0) {
            PyErr_SetString(PyExc_TypeError, "Matrix61c_set_subscript(): Unable to get indices from the single slice in the 2D matrix");
            return -1;
        }

        // step_size is not 1 or the length of slice is < 1
        if (step != 1 || slicelength < 1) {
            PyErr_SetString(PyExc_ValueError, "Matrix61c_set_subscript(): step_size is not 1 or the length of slice is < 1 for the 2D matrix");
            return -1;
        }

        // if key is one integer slice, and the integera are out of range.
        if (start < 0 || start >= self->mat->rows ||
            stop <= 0 || stop  >  self->mat->rows ||
            stop <= start /*maybe more conditions - should be ok*/ ) {
                PyErr_SetString(PyExc_IndexError, "Matrix61c_set_subscript(): key is a slice that is out of range for the 2D matrix");
                return -1;
        }

        // the value must be a list
        if (!PyList_Check(v)) {
            PyErr_SetString(PyExc_TypeError, "Matrix61c_set_subscript(): Value must be a list (2D: slice).");
            return -1;
        }
        
        // Special Processing for stop - start = 1 (do not have to be a nested list)
        if (stop - start == 1) {
            if (PyList_Size(v) != self->mat->cols) {
                PyErr_SetString(PyExc_ValueError, "Matrix61c_set_subscript(): Value has wrong length (should be: mat->cols).");
                return -1;
            }
            for (int j = 0; j < PyList_Size(v); j++) {
                if (!PyFloat_Check(PyList_GetItem(v, j)) && !PyLong_Check(PyList_GetItem(v, j))) {
                    PyErr_SetString(PyExc_ValueError, "Matrix61c_set_subscript(): Value must be float or integer in nested list");
                    return -1;
                }
                double val = PyLong_Check(PyList_GetItem(v, j)) ? PyLong_AsDouble(PyList_GetItem(v, j)) :
                                                                  PyFloat_AsDouble(PyList_GetItem(v, j));
                set(self->mat, start, j, val);   
            }
            return 0; // success     
        }

        if (PyList_Size(v) != stop - start) {
            PyErr_SetString(PyExc_ValueError, "Matrix61c_set_subscript(): Value has wrong length (should be: stop - start).");
            return -1;
        }
        // set each element in the row (a nested list)
        for (int i = 0; i < stop - start; i++) {
            if (!PyList_Check(PyList_GetItem(v, i))) {
                PyErr_SetString(PyExc_TypeError, "Matrix61c_set_subscript(): Value must be list inside the list");
                return -1;
            }

            // nested list size (be the #cols)
            PyObject *item = PyList_GetItem(v, i);
            if (PyList_Size(item) != self->mat->cols) {
                PyErr_SetString(PyExc_ValueError, "Matrix61c_set_subscript(): Value has wrong length in the nested list.");
                return -1;
            }
            for (int j = 0; j < PyList_Size(item); j++) {
                if (!PyFloat_Check(PyList_GetItem(item, j)) && !PyLong_Check(PyList_GetItem(item, j))) {
                    PyErr_SetString(PyExc_ValueError, "Matrix61c_set_subscript(): Value must be float or integer in nested list");
                    return -1;
                }
                double val = PyLong_Check(PyList_GetItem(item, j)) ? PyLong_AsDouble(PyList_GetItem(item, j)) :
                                                                  PyFloat_AsDouble(PyList_GetItem(item, j));
                set(self->mat, start + i, j, val);   
            }
        }
        return 0; // success        
    } else {     
        // It must be a length-2 tuple of slices/ints.
        if (PyTuple_CheckExact(key) && PyTuple_GET_SIZE(key) != 2) {
            PyErr_SetString(PyExc_TypeError, "Matrix61c_set_subscript(): key is not a length-2 tuple of slices/ints for 2D matrix");
            return -1;
        }
        // two tuple objects can be (int, int), (int slice), (slice, int) or (slice, slice)
        PyObject *tuple1 = PyTuple_GetItem(key, 0);
        PyObject *tuple2 = PyTuple_GetItem(key, 1);
        if (!tuple1 || !tuple2) {
            PyErr_SetString(PyExc_TypeError, "Matrix61c_set_subscript(): either tuple is empty in the length-2 tuple of slices/ints for 2D matrix");
            return -1;
        }
        if (!((PyLong_Check(tuple1) && PyLong_Check (tuple2)) || (PySlice_Check(tuple1) && PySlice_Check(tuple2)) ||
              (PyLong_Check(tuple1) && PySlice_Check(tuple2)) || (PySlice_Check(tuple1) && PyLong_Check (tuple2))) ) {
            PyErr_SetString(PyExc_TypeError, "Matrix61c_set_subscript(): must be (int, int), (int slice), (slice, int) or (slice, slice) for 2D matrix");
            return -1;
        }

        // locations for the slice
        int fromRow = 0, fromCol = 0, toRow = 0, toCol = 0;
        if (PyLong_Check(tuple1)) {        
            int row = PyLong_AsLong(tuple1);
            // IndexError if key (row) is an integer but is out of range
            if (row < 0 || row >= self->mat->rows) {
                PyErr_SetString(PyExc_IndexError, "Matrix61c_set_subscript(): row as an integer in tuple1 is out of range for 2D matrix.");
                return -1;
            }
            fromRow = row; 
            toRow = row;
        } else { // a slice
            Py_ssize_t length = self->mat->rows;
            Py_ssize_t start = 0, stop = 0, step = 0, slicelength = 0;
            if (PySlice_GetIndicesEx(tuple1, length, &start, &stop, &step, &slicelength) < 0) {
                PyErr_SetString(PyExc_TypeError, "Matrix61c_set_subscript(): Unable to get indices from the tuple1 in the 2D matrix");
                return -1;
            }

            // step_size is not 1 or the length of slice is < 1
            if (step != 1 || slicelength < 1) {
                PyErr_SetString(PyExc_ValueError, "Matrix61c_set_subscript(): tuple1 -> step_size is not 1 or the length of slice is < 1 for the 2D matrix");
                return -1;
            }
            // if key is one integer slice, and the integera are out of range.
            if (start < 0 || start >= self->mat->rows ||
                stop <= 0 || stop  >  self->mat->rows ||
                stop <= start /*maybe more conditions??*/ ) {
                    PyErr_SetString(PyExc_IndexError, "MatrixMatrix61c_set_subscript61c_subscript(): tuple1 -> key is a slice that is out of range for the 2D matrix");
                    return -1;
            }
            fromRow = start; 
            toRow = stop - 1;
        }   
        // for tuple2
        if (PyLong_Check(tuple2)) {        
            int col = PyLong_AsLong(tuple2);
            // IndexError if key (col) is an integer but is out of range
            if (col < 0 || col >= self->mat->cols) {
                PyErr_SetString(PyExc_IndexError, "Matrix61c_set_subscript(): col as an integer in tuple2 is out of range for 2D matrix.");
                return -1;
            }
            fromCol = col; 
            toCol   = col;
        } else { // a slice
            Py_ssize_t length = self->mat->cols;
            Py_ssize_t start = 0, stop = 0, step = 0, slicelength = 0;
            if (PySlice_GetIndicesEx(tuple2, length, &start, &stop, &step, &slicelength) < 0) {
                PyErr_SetString(PyExc_TypeError, "Matrix61c_set_subscript(): Unable to get indices from the tuple2 in the 2D matrix");
                return -1;
            }

            // step_size is not 1 or the length of slice is < 1
            if (step != 1 || slicelength < 1) {
                PyErr_SetString(PyExc_ValueError, "Matrix61c_set_subscript(): tuple2 -> step_size is not 1 or the length of slice is < 1 for the 2D matrix");
                return -1;
            }
            // if key is one integer slice, and the integera are out of range.
            if (start < 0 || start >= self->mat->cols ||
                stop <= 0 || stop  >  self->mat->cols ||
                stop <= start /*maybe more conditions??*/ ) {
                    PyErr_SetString(PyExc_IndexError, "Matrix61c_set_subscript(): tuple2 -> key is a slice that is out of range for the 2D matrix");
                    return -1;
            }
            fromCol = start; 
            toCol   = stop - 1;
        }   
    
        // set a single number if the resulting matrix is 1 by 1
        if (fromRow == toRow && fromCol == toCol) {
            // v must be a float or int if 1x1
            if (!PyFloat_Check(v) && !PyLong_Check(v)) {
                PyErr_SetString(PyExc_TypeError, "Matrix61c_set_subscript(): value for 2D matrix is invalid (must be float or int for 1x1)");
                return -1;
            }
            // set up the val as needed
            double val = PyLong_Check(v) ? PyLong_AsDouble(v) : PyFloat_AsDouble(v);
            set(self->mat, fromRow, fromCol, val);
            return 0;   
        } 

        // Normal 2D: the value must be a list
        if (!PyList_Check(v)) {
            PyErr_SetString(PyExc_TypeError, "Matrix61c_set_subscript(): Value must be a list (2D: 2 tuples).");
            return -1;
        }

        // Have to deal with special use cases! - Caught here from example on indexing with colon
        if (fromRow == toRow)  {
            if (PyList_Size(v) == (toCol - fromCol)) {
                PyErr_SetString(PyExc_ValueError, "Matrix61c_set_subscript(): List has wrong length.");
                return -1;
            }
            // set each element in the row
            for (int i = 0; i < toCol - fromCol + 1; i++) {
                if (!PyLong_Check(PyList_GetItem(v, i)) && !PyFloat_Check(PyList_GetItem(v, i))) {
                    PyErr_SetString(PyExc_TypeError, "Matrix61c_set_subscript(): Value must be a float/int");
                    return -1;
                }
                double val = PyLong_Check(PyList_GetItem(v, i)) ? PyLong_AsDouble(PyList_GetItem(v, i)) :
                                                                  PyFloat_AsDouble(PyList_GetItem(v, i));
                set(self->mat, fromRow, fromCol + i, val); 
            }
        } else if (fromCol == toCol)  {
            if (PyList_Size(v) == (toRow - fromRow)) {
                PyErr_SetString(PyExc_ValueError, "Matrix61c_set_subscript(): List has wrong length.");
                return -1;
            }
            // set each element in the col
            for (int i = 0; i < toRow - fromRow + 1; i++) {
                if (!PyLong_Check(PyList_GetItem(v, i)) && !PyFloat_Check(PyList_GetItem(v, i))) {
                    PyErr_SetString(PyExc_TypeError, "Matrix61c_set_subscript(): Value must be a float/int");
                    return -1;
                }
                double val = PyLong_Check(PyList_GetItem(v, i)) ? PyLong_AsDouble(PyList_GetItem(v, i)) :
                                                                  PyFloat_AsDouble(PyList_GetItem(v, i));
                set(self->mat, fromRow + i, fromCol, val); 
            }
        } else {

            if (PyList_Size(v) != toRow - fromRow + 1) {
                PyErr_SetString(PyExc_ValueError, "Matrix61c_set_subscript(): Value has wrong length (should be: toRow - fromRow + 1).");
                return -1;
            }
            
            // set each element in the row (a nested list)
            for (int i = 0; i < toRow - fromRow + 1; i++) {
                if (!PyList_Check(PyList_GetItem(v, i))) {
                    PyErr_SetString(PyExc_TypeError, "Matrix61c_set_subscript(): Value must be list inside the list");
                    return -1;
                }
                // nested list size (be the #cols)
                PyObject *item = PyList_GetItem(v, i);
                if (PyList_Size(item) != toCol - fromCol + 1) {
                    PyErr_SetString(PyExc_ValueError, "Matrix61c_set_subscript(): Value has wrong length in the nested list.");
                    return -1;
                }
                for (int j = 0; j < PyList_Size(item); j++) {
                    if (!PyFloat_Check(PyList_GetItem(item, j)) && !PyLong_Check(PyList_GetItem(item, j))) {
                        PyErr_SetString(PyExc_ValueError, "Matrix61c_set_subscript(): Value must be float or integer in nested list");
                        return -1;
                    }
                    double val = PyLong_Check(PyList_GetItem(item, j)) ? PyLong_AsDouble(PyList_GetItem(item, j)) :
                                                                      PyFloat_AsDouble(PyList_GetItem(item, j));
                    set(self->mat, fromRow + i, fromCol + j, val);    
                }
            }
        }
        return 0;
    }
}

PyMappingMethods Matrix61c_mapping = {
    NULL,
    (binaryfunc) Matrix61c_subscript,
    (objobjargproc) Matrix61c_set_subscript,
};

/* INSTANCE ATTRIBUTES*/
PyMemberDef Matrix61c_members[] = {
    {
        "shape", T_OBJECT_EX, offsetof(Matrix61c, shape), 0,
        "(rows, cols)"
    },
    {NULL}  /* Sentinel */
};

PyTypeObject Matrix61cType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "numc.Matrix",
    .tp_basicsize = sizeof(Matrix61c),
    .tp_dealloc = (destructor)Matrix61c_dealloc,
    .tp_repr = (reprfunc)Matrix61c_repr,
    .tp_as_number = &Matrix61c_as_number,
    .tp_flags = Py_TPFLAGS_DEFAULT |
    Py_TPFLAGS_BASETYPE,
    .tp_doc = "numc.Matrix objects",
    .tp_methods = Matrix61c_methods,
    .tp_members = Matrix61c_members,
    .tp_as_mapping = &Matrix61c_mapping,
    .tp_init = (initproc)Matrix61c_init,
    .tp_new = Matrix61c_new
};


struct PyModuleDef numcmodule = {
    PyModuleDef_HEAD_INIT,
    "numc",
    "Numc matrix operations",
    -1,
    Matrix61c_class_methods
};

/* Initialize the numc module */
PyMODINIT_FUNC PyInit_numc(void) {
    PyObject* m;

    if (PyType_Ready(&Matrix61cType) < 0)
        return NULL;

    m = PyModule_Create(&numcmodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&Matrix61cType);
    PyModule_AddObject(m, "Matrix", (PyObject *)&Matrix61cType);
    printf("CS61C Fall 2020 Project 4: numc imported!\n");
    fflush(stdout);
    return m;
}