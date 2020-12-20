#include "matrix.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <immintrin.h>
#include <x86intrin.h>
#endif

/* Below are some intel intrinsics that might be useful
 * void _mm256_storeu_pd (double * mem_addr, __m256d a)
 * __m256d _mm256_set1_pd (double a)
 * __m256d _mm256_set_pd (double e3, double e2, double e1, double e0)
 * __m256d _mm256_loadu_pd (double const * mem_addr)
 * __m256d _mm256_add_pd (__m256d a, __m256d b)
 * __m256d _mm256_sub_pd (__m256d a, __m256d b)
 * __m256d _mm256_fmadd_pd (__m256d a, __m256d b, __m256d c)
 * __m256d _mm256_mul_pd (__m256d a, __m256d b)
 * __m256d _mm256_cmp_pd (__m256d a, __m256d b, const int imm8)
 * __m256d _mm256_and_pd (__m256d a, __m256d b)
 * __m256d _mm256_max_pd (__m256d a, __m256d b)
*/

/*
 * Generates a random double between `low` and `high`.
 */
double rand_double(double low, double high) {
    double range = (high - low);
    double div = RAND_MAX / range;
    return low + (rand() / div);
}

/*
 * Generates a random matrix with `seed`.
 */
void rand_matrix(matrix *result, unsigned int seed, double low, double high) {
    srand(seed);
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            set(result, i, j, rand_double(low, high));
        }
    }
}

/*
 * Allocate space for a matrix struct pointed to by the double pointer mat with
 * `rows` rows and `cols` columns. You should also allocate memory for the data array
 * and initialize all entries to be zeros. Remember to set all fieds of the matrix struct.
 * `parent` should be set to NULL to indicate that this matrix is not a slice.
 * You should return -1 if either `rows` or `cols` or both have invalid values, or if any
 * call to allocate memory in this function fails. If you don't set python error messages here upon
 * failure, then remember to set it in numc.c.
 * Return 0 upon success and non-zero upon failure.
 */

int allocate_matrix(matrix **mat, int rows, int cols) {
    /* TODO: YOUR CODE HERE */
    /* Invalid row/col value */

    if (rows <= 0 || cols <= 0) {
        PyErr_SetString(PyExc_ValueError, "Dimensions must be positive");
        return -1;
    }

    /* Allocating an instance of the matrix */
    matrix* retMatrix = (matrix*) malloc(sizeof(matrix));
    if (retMatrix == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to allocate memory.");
        return -1;
    }

    retMatrix->rows = rows;
    retMatrix->cols = cols;
    if (rows == 1 || cols == 1) {
        retMatrix->is_1d = 1;
    } else {
        retMatrix->is_1d = 0;
    }
    double** data = (double**) malloc(sizeof(double*));
    if (data == NULL) {
        free(retMatrix);
        PyErr_SetString(PyExc_RuntimeError, "Failed to allocate memory.");
        return -1;
    }
    *data = (double*) calloc(rows * cols, sizeof(double));
    if (*data == NULL) {
        free(data);
        free(retMatrix);      
        PyErr_SetString(PyExc_RuntimeError, "Failed to allocate memory."); 
        return -1;
    }
    retMatrix->data = data;
    retMatrix->ref_cnt = 1;
    retMatrix->parent = NULL;
    retMatrix->baseParent = retMatrix;
    retMatrix->heightOffset = 0;
    retMatrix->widthOffset = 0;
    *mat = retMatrix;
    return 0;
}

/*
 * Allocate space for a matrix struct pointed to by `mat` with `rows` rows and `cols` columns.
 * This is equivalent to setting the new matrix to be
 * from[row_offset:row_offset + rows, col_offset:col_offset + cols]
 * If you don't set python error messages here upon failure, then remember to set it in numc.c.
 * Return 0 upon success and non-zero upon failure.
 */
/*
Ex: from: 5 x 6, mat: rOff = 2, cOff = 1, rows = 2, cols = 2
    newMatrix = from[rows 2:4, cols 1:3] --> dimensions: 2 by 2
*/

// int allocate_matrix_ref_old(matrix **mat, matrix *from, int row_offset, int col_offset,
//                         int rows, int cols) {
//     /* TODO: YOUR CODE HERE */
//     /* Invalid row/col value or invalid pointer */
//     if (rows <= 0 || cols <= 0 || from == NULL) {
//         return -1;
//     }

//     if (rows + row_offset > from->rows || cols + col_offset > from->cols) {
//         return -1;
//     }

//     matrix* newMat = (matrix*) malloc(sizeof(matrix));
//     if (newMat == NULL) {
//         PyErr_SetString(PyExc_RuntimeError, "Failed to allocate memory.");
//         return -1;
//     }
//     newMat->rows = rows;
//     newMat->cols = cols;
//     if (rows == 1 || cols == 1) {
//         newMat->is_1d = 1;
//     } else {
//         newMat->is_1d = 0;
//     }

//     double** fromData = from->data;
//     int fromHeight = from->rows;
//     int fromWidth = from->cols;

//     double** data = (double**) malloc(rows * sizeof(double*));
//     if (data == NULL) {
//         free(newMat);
//         PyErr_SetString(PyExc_RuntimeError, "Failed to allocate memory.");
//         return -1;
//     }

//     /* Fill in with the relevant data */
//     for (int i = 0; i < rows; i++) {
//         data[i] = fromData[row_offset + i] + col_offset;
//     }

//     // printf("0,0: %f\n", data[0][0]);
//     // printf("0,1: %f\n", data[0][1]);
//     // printf("1,0: %f\n", data[1][0]);
//     // printf("1,1: %f\n", data[1][1]);
//     newMat->data = data;
    
//     from->ref_cnt += 1;
//     newMat->parent = from;
//     newMat->ref_cnt = 1;
//     newMat->heightOffset = row_offset;
//     newMat->widthOffset = col_offset;
//     *mat = newMat;
    
//     return 0;

// }

// int allocate_matrix_ref1(matrix **mat, matrix *from, int row_offset, int col_offset,
//                         int rows, int cols) {
//     /* TODO: YOUR CODE HERE */
//     /* Invalid row/col value or invalid pointer */
//     if (rows <= 0 || cols <= 0 || from == NULL) {
//         return -1;
//     }

//     if (rows + row_offset > from->rows || cols + col_offset > from->cols) {
//         return -1;
//     }

//     matrix* newMat = NULL;
//     allocate_matrix(&newMat, rows, cols);
//     if (newMat == NULL) {
//         return -1;
//     }
//     double* newMatData = *(newMat->data);
//     double* fromData = *(from->data);
//     //int fromHeight = from->rows;
//     int fromWidth = from->cols;
//     int newSize = rows * cols;

//     /* Fill in with the relevant data */
//     for (int i = 0; i < newSize; i++) {
//         int newRow = i/cols;
//         int newCol = i % cols;
//         int oldIndex = (newRow + row_offset) * fromWidth + (newCol + col_offset);
//         newMatData[i] = fromData[oldIndex];
//     }
    
//     from->ref_cnt += 1;
//     newMat->parent = from;
//     newMat->ref_cnt = 1;
//     newMat->heightOffset = row_offset;
//     newMat->widthOffset = col_offset;
//     *mat = newMat;
    
//     return 0;

// }
int allocate_matrix_ref(matrix **mat, matrix *from, int row_offset, int col_offset,
                        int rows, int cols) {
    /* TODO: YOUR CODE HERE */
    /* Invalid row/col value or invalid pointer */
    if (rows <= 0 || cols <= 0 || from == NULL) {
        return -1;
    }

    if (rows + row_offset > from->rows || cols + col_offset > from->cols) {
        return -1;
    }
    
    // ceate a new matrix
    matrix* newMat = NULL;
    newMat = (matrix *) malloc(sizeof(matrix));
    if (newMat == NULL) {
        return -1;
    }
    newMat->rows = rows;
    newMat->cols = cols;
    newMat->is_1d = (rows == 1 || cols == 1) ? 1 : 0;

    newMat->data = (double**) malloc(sizeof(double *));
    if (newMat->data == NULL) {
        free(newMat);
        return -1;
    }
    // do NOT allocate the space for the data; just the pointer. Use its parent's data
    newMat->data = from->data + row_offset * from->cols + col_offset;
    
    from->baseParent->ref_cnt += 1;
    if (from != from->baseParent) {
        from->ref_cnt += 1;
    }
    newMat->parent = from;
    newMat->baseParent = from->baseParent;
    newMat->ref_cnt = 1;
    *mat = newMat;
    return 0;
}

/*
 * This function will be called automatically by Python when a numc matrix loses all of its
 * reference pointers.
 * You need to make sure that you only free `mat->data` if no other existing matrices are also
 * referring this data array.
 * See the spec for more information.
 - Set function: set in the current matrix
    - Want it to be reflected in the parent matrix
- Check if it's a slice (parent != NULL)
    - Change the data in the parent
 */

// void deallocate_matrix1(matrix *mat) {
//     /* TODO: YOUR CODE HERE */
//     if (mat == NULL) {
//         return;
//     }

//     // If mat has no parent & isn't referenced elsewhere
//     mat->baseParent--;
//     if (mat->baseParent == mat && mat->ref_cnt == 0) {
//         free(*(mat->data));
//         for (int i = 0; i < mat->rows; i++) {
//             free(mat->data + i);
//         }
//         free(mat);
//     } else if (mat->ref_cnt > 1) {
//         // Other matrices reference this one
//         mat->ref_cnt -= 1;
//     } else if (mat->parent != NULL && mat->ref_cnt == 1) {
//         // Current matrix isn't referenced elsewhere but has a parent 
//         mat->parent->ref_cnt -= 1;
//         free(mat);
//     }
// }

void deallocate_matrix(matrix *mat) {
    /* TODO: YOUR CODE HERE */
    if (mat == NULL) {
        return;
    }

    // If mat is base parent & isn't referenced elsewhere (DINK: double income, no kids)
    (mat->baseParent->ref_cnt)--;

    if (mat->baseParent == mat && mat->ref_cnt == 0) {
        free(*(mat->data));
        free(mat->data);
        free(mat);
    } 
    // Not base parent
    else if (mat->baseParent != mat) {
        (mat->ref_cnt)--;
        if (mat->ref_cnt == 0) {
            free(mat);
        }
        if (mat->parent != mat->baseParent) {
            (mat->parent->ref_cnt)--;
            if (mat->parent->ref_cnt == 0) {
                free(mat->parent);
            }
        }
    }
}

// int copyData1(matrix *mat1, matrix *mat2) {
//     if (mat1 == NULL || mat2 == NULL) {
//         return -1;
//     }
//     double** data1 = mat1->data;
//     double** data2 = mat2->data;
//     for (int i = 0; i < mat1->rows; i++) {
//         for (int j = 0; j < mat1->cols; j++) {
//             data2[i][j] = data1[i][j];
//         }
//     }
//     return 0;
// }

int copyData1(matrix *mat1, matrix *mat2) {
    if (mat1 == NULL || mat2 == NULL) {
        return -1;
    }
    // need to consider the non-sliced and sliced matrix
    for (int i = 0; i < mat1->rows * mat1->cols; i++) {
        double val = get(mat1, i / mat1->cols, i % mat1->cols);
        set(mat2, i / mat1->cols, i % mat1->cols, val); 
    }
    return 0;
}

/* Helper method: Copy data from mat1 into mat2 */
int copyData(matrix *mat1, matrix *mat2) {
    if (mat1 == NULL || mat2 == NULL) {
        return -1;
    }
    int dims = mat1->rows * mat1->cols;
    double* rData = *(mat2->data);
    double* mData = *(mat1->data);
    #pragma omp parallel for if (dims >= 100000)
    for (int i = 0; i < dims/16 * 16; i += 16) {
        _mm256_storeu_pd((rData + i), _mm256_loadu_pd(mData + i));
        _mm256_storeu_pd((rData + i + 4), _mm256_loadu_pd(mData + i + 4));
        _mm256_storeu_pd((rData + i + 8), _mm256_loadu_pd(mData + i + 8));
        _mm256_storeu_pd((rData + i + 12), _mm256_loadu_pd(mData + i + 12));
    }
    for (int i = dims/16 * 16; i < dims; i++) {
        rData[i] = mData[i];
    }

    // double* data1 = *(mat1->data);
    // double* data2 = *(mat2->data);
    // for (int i = 0; i < mat1->rows * mat1->cols; i++) {
    //     data2[i] = data1[i];
    // }
    return 0;
}

// int makeIdentity1(matrix *mat) {
//     if (mat == NULL) {
//         return -1;
//     }
//     double** data = mat->data;
//     if (data == NULL) {
//         return -1;
//     }
//     for (int i = 0; i < mat->rows; i++) {
//         for (int j = 0; j < mat->cols; j++) {
//             if (i == j) {
//                 data[i][j] = 1;
//             }
//             else {
//                 data[i][j] = 0;
//             }
//         }
//     }
//     return 0;
// }

int makeIdentity1(matrix *mat) {
    if (mat == NULL) {
        return -1;
    }
    double** data = mat->data;
    if (data == NULL) {
        return -1;
    }
    // need to consider the non-sliced and sliced matrix
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        double val = (i / mat->rows == i % mat->cols) ? 1: 0; 
        set(mat, i / mat->cols, i % mat->cols, val); 
    }
    return 0;
}

/* Helper method: stash identity matrix into matrix data */
int makeIdentity(matrix *mat) {
    if (mat == NULL) {
        return -1;
    }
    double* data = *(mat->data);
    int height = mat->rows;
    int width = mat->cols;

    #pragma omp parallel for if (height >= 10) //if (height * width >= 100000)
    for (int i = 0; i < height * width; i++) {

        if (i/width == i % width) {
            data[i] = 1;
        }
        else {
            data[i] = 0;
        }
    }
    return 0;
}

// /* Helper method: free entire matrix */
// int freeMatrix1(matrix *mat) {
//     if (mat == NULL) {
//         return 0;
//     }
//     int height = mat->rows;
//     double** data = mat->data;
//     for (int i = 0; i < height; i++) {
//         free(data[i]);
//     }
//     free(data);
//     free(mat);
//     return 0;
// }

/* Helper method: free entire matrix */
int freeMatrix(matrix *mat) {
    if (mat == NULL) {
        return 0;
    }
    //int height = mat->rows;
    double** data = mat->data;
    free(*data);
    free(data);
    free(mat);
    return 0;
}

// double get1(matrix *mat, int row, int col) {
//     /* TODO: YOUR CODE HERE */
//     return (mat->data)[row][col];
// }

// /*
//  * Return the double value of the matrix at the given row and column.
//  * You may assume `row` and `col` are valid.
//  */
// double get_old(matrix *mat, int row, int col) {
//     /* TODO: YOUR CODE HERE */
//     return (mat->data)[row][col];
//     // double* data = *(mat->data);
//     // int offset = row * mat->cols + col;
//     // return data[offset];
// }

// void set1(matrix *mat, int row, int col, double val) {
//     /* TODO: YOUR CODE HERE */
//     (mat->data)[row][col] = val;
// }

double get(matrix *mat, int row, int col) {
    /* TODO: YOUR CODE HERE */
    if (mat->parent == NULL) {
        double* data = *(mat->data);
        return data[row * mat->cols + col];
    } else {
        // recover the offset by using the mat->data value from allocate_matrix_ref()
        int offset = mat->data - mat->parent->data;
        return get(mat->parent, row + offset / mat->parent->cols, col + offset % mat->parent->cols);
    }
}
/*
 * Set the value at the given row and column to val. You may assume `row` and
 * `col` are valid
 */
// void set_old(matrix *mat, int row, int col, double val) {
//     /* TODO: YOUR CODE HERE */
//     // FIXME: ACCOUNT FOR SLICES
//     (mat->data)[row][col] = val;
//     // int width = mat->cols;
//     // double* data = *(mat->data);
//     // int offset = row * mat->cols + col;
//     // data[offset] = val;
    
//     // while (mat->parent != NULL) {
//     //     width = mat->parent->rows;
//     //     col = col + mat->widthOffset;
//     //     row = row + mat->heightOffset;
//     //     offset = width * row + col;
//     //     data = *(mat->parent->data);
//     //     data[offset] = val;
//     //     mat = mat->parent;
//     // }
// }

// void fill_matrix1(matrix *mat, double val) {
//     /* TODO: YOUR CODE HERE */
//     if (mat == NULL) {
//         return;
//     }
//     else {
//         double** data = mat->data;
//         for (int i = 0; i < mat->rows; i++) {
//             for (int j = 0; j < mat->cols; j++) {
//                 data[i][j] = val;
//             }
//         }
//     }
// }

void set(matrix *mat, int row, int col, double val) {
    /* TODO: YOUR CODE HERE */
    if (mat->parent == NULL) {
        double* data = *(mat->data);
        data[row * mat->cols + col] = val;
    } else {
        // recover the offset by using the mat->data value from allocate_matrix_ref()
        // always set the value at the great-great parent level
        int offset = mat->data - mat->parent->data;
        set(mat->parent, row + offset / mat->parent->cols, col + offset % mat->parent->cols, val);
    }
}

/*
 * Set all entries in mat to val
 */
void fill_matrix(matrix *mat, double val) {
    /* TODO: YOUR CODE HERE */
    if (mat == NULL) {
        return;
    }
    else {

        double* data = *(mat->data);
        __m256d vec = _mm256_set1_pd(val);
        int dims = mat->rows * mat-> cols;
        if (dims >= 100000) {
            omp_set_num_threads(8);
        }
        #pragma omp parallel for if (dims >= 100000)
        for (int i = 0; i < dims/16 * 16; i += 16) {
            _mm256_storeu_pd((data + i), vec);
            _mm256_storeu_pd((data + i + 4), vec);
            _mm256_storeu_pd((data + i + 8), vec);
            _mm256_storeu_pd((data + i + 12), vec);
        }
        for (int i = dims/16 * 16; i < dims; i++) {
            data[i] = val;
        }

        // for (int i = 0; i < mat->rows * mat->cols; i++) {
        //     data[i] = val;
        // }
    }
}


// int add_matrix1(matrix *result, matrix *mat1, matrix *mat2) {
//     /* TODO: YOUR CODE HERE */
//     /* Check for valid pointers */

//     if (mat1 == NULL || mat2 == NULL || result == NULL) {
//         return -1;
//     }

//     /* Obtain & check for valid dimensions */
//     int mat1Height = mat1->rows;
//     int mat1Width = mat1->cols;
//     int mat2Height = mat2->rows;
//     int mat2Width = mat2->cols;
//     int rHeight = result->rows;
//     int rWidth = result->cols;
//     if (mat1Width < 1 || mat1Height < 1 || mat1Width != mat2Width || mat1Height != mat2Height || mat1Height != rHeight || mat1Width != rWidth) {
//         return -1;
//     }
//     double** addMatrixData = result->data;
//     double** mat1Data = mat1->data;
//     double** mat2Data = mat2->data;
//     for (int i = 0; i < mat1Height; i++) {
//         for (int j = 0; j < mat1Width; j++) {
//             addMatrixData[i][j] = mat1Data[i][j] + mat2Data[i][j];
//         }
//     }
//     return 0;
// }

int add_matrix1(matrix *result, matrix *mat1, matrix *mat2) {
    /* TODO: YOUR CODE HERE */
    /* Check for valid pointers */

    if (mat1 == NULL || mat2 == NULL || result == NULL) {
        return -1;
    }

    /* Obtain & check for valid dimensions */
    int mat1Height = mat1->rows;
    int mat1Width = mat1->cols;
    int mat2Height = mat2->rows;
    int mat2Width = mat2->cols;
    int rHeight = result->rows;
    int rWidth = result->cols;
    if (mat1Width < 1 || mat1Height < 1 || mat1Width != mat2Width || mat1Height != mat2Height || mat1Height != rHeight || mat1Width != rWidth) {
        return -1;
    }
    for (int i = 0; i < mat1->rows * mat1->cols; i++) {
        double val = get(mat1, i / mat1->cols, i % mat1->cols) + 
                     get(mat2, i / mat1->cols, i % mat1->cols);
        set (result, i / mat1->cols, i % mat1->cols, val);
    }
    return 0;
}

/*
 * Store the result of adding mat1 and mat2 to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int add_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    /* TODO: YOUR CODE HERE */
    /* Check for valid pointers */

    if (mat1 == NULL || mat2 == NULL || result == NULL) {
        return -1;
    }

    if (mat1->parent != NULL || mat2->parent != NULL) {
        return add_matrix1(result, mat1, mat2);
    }

    /* Obtain & check for valid dimensions */
    int mat1Height = mat1->rows;
    int mat1Width = mat1->cols;
    int mat2Height = mat2->rows;
    int mat2Width = mat2->cols;
    int rHeight = result->rows;
    int rWidth = result->cols;
    if (mat1Width < 1 || mat1Height < 1 || mat1Width != mat2Width || mat1Height != mat2Height || mat1Height != rHeight || mat1Width != rWidth) {
        return -1;
    }

    /* Get + check for valid data */
    double* rData = *(result->data);
    double* mat1Data = *(mat1->data);
    double* mat2Data = *(mat2->data);
    int dims = mat1Height * mat1Width;

    int numThreads = 8;
    if (dims >= 100000) {
        omp_set_num_threads(numThreads);
    }

    #pragma omp parallel for if (dims >= 100000)
    for (int i = 0; i < dims/16 * 16; i += 16) {
        _mm256_storeu_pd((rData+ i),
        _mm256_add_pd(_mm256_loadu_pd(mat1Data + i), 
            _mm256_loadu_pd(mat2Data + i)));

        _mm256_storeu_pd((rData+ i + 4),
        _mm256_add_pd(_mm256_loadu_pd(mat1Data + i + 4), 
            _mm256_loadu_pd(mat2Data + i + 4)));

        _mm256_storeu_pd((rData+ i + 8),
        _mm256_add_pd(_mm256_loadu_pd(mat1Data + i + 8), 
            _mm256_loadu_pd(mat2Data + i + 8)));

        _mm256_storeu_pd((rData+ i + 12),
        _mm256_add_pd(_mm256_loadu_pd(mat1Data + i + 12), 
            _mm256_loadu_pd(mat2Data + i + 12)));
    }
    
    for (int i = dims/16 * 16; i < dims; i++) {
        rData[i] = mat1Data[i] + mat2Data[i];
    }
    return 0;

}

// int sub_matrix1(matrix *result, matrix *mat1, matrix *mat2) {
//     /* TODO: YOUR CODE HERE */
    
//     /* Check for valid pointers */
//     if (mat1 == NULL || mat2 == NULL || result == NULL) {
//         return -1;
//     }
//     /* Obtain & check for valid dimensions */
//     int mat1Height = mat1->rows;
//     int mat1Width = mat1->cols;
//     int mat2Height = mat2->rows;
//     int mat2Width = mat2->cols;
//     int rHeight = result->rows;
//     int rWidth = result->cols;
//     if (mat1Width < 1 || mat1Height < 1 || mat1Width != mat2Width || mat1Height != mat2Height || mat1Height != rHeight || mat1Width != rWidth) {
//         return -1;
//     }

//     /* Get + check for valid data */
//     double** subMatrixData = result->data;
//     double** mat1Data = mat1->data;
//     double** mat2Data = mat2->data;

//     for (int i = 0; i < mat1Height; i++) {
//         for (int j = 0; j < mat1Width; j++) {
//             subMatrixData[i][j] = mat1Data[i][j] - mat2Data[i][j];
//         }
//     }
//     return 0;
// }

int sub_matrix1(matrix *result, matrix *mat1, matrix *mat2) {
    /* TODO: YOUR CODE HERE */
    
    /* Check for valid pointers */
    if (mat1 == NULL || mat2 == NULL || result == NULL) {
        return -1;
    }
    /* Obtain & check for valid dimensions */
    int mat1Height = mat1->rows;
    int mat1Width = mat1->cols;
    int mat2Height = mat2->rows;
    int mat2Width = mat2->cols;
    int rHeight = result->rows;
    int rWidth = result->cols;
    if (mat1Width < 1 || mat1Height < 1 || mat1Width != mat2Width || mat1Height != mat2Height || mat1Height != rHeight || mat1Width != rWidth) {
        return -1;
    }
    for (int i = 0; i < mat1->rows * mat1->cols; i++) {
        double val = get(mat1, i / mat1->cols, i % mat1->cols) - 
                     get(mat2, i / mat1->cols, i % mat1->cols);
        set (result, i / mat1->cols, i % mat1->cols, val);
    }
    return 0;
}

/*
 * Store the result of subtracting mat2 from mat1 to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int sub_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    /* TODO: YOUR CODE HERE */
    /* TODO: YOUR CODE HERE */
    /* Check for valid pointers */

    if (mat1 == NULL || mat2 == NULL || result == NULL) {
        return -1;
    }

    if (mat1->parent != NULL || mat2->parent != NULL) {
        return sub_matrix1(result, mat1, mat2);
    }

    /* Obtain & check for valid dimensions */
    int mat1Height = mat1->rows;
    int mat1Width = mat1->cols;
    int mat2Height = mat2->rows;
    int mat2Width = mat2->cols;
    int rHeight = result->rows;
    int rWidth = result->cols;
    if (mat1Width < 1 || mat1Height < 1 || mat1Width != mat2Width || mat1Height != mat2Height || mat1Height != rHeight || mat1Width != rWidth) {
        return -1;
    }

    /* Get + check for valid data */
    double* rData = *(result->data);
    double* mat1Data = *(mat1->data);
    double* mat2Data = *(mat2->data);
    int dims = mat1Height * mat1Width;

    int numThreads = 8;
    omp_set_num_threads(numThreads);

    #pragma omp parallel for if (dims >= 100000)
    for (int i = 0; i < dims/16 * 16; i += 16) {
        _mm256_storeu_pd((rData+ i),
        _mm256_sub_pd(_mm256_loadu_pd(mat1Data + i), 
            _mm256_loadu_pd(mat2Data + i)));

        _mm256_storeu_pd((rData+ i + 4),
        _mm256_sub_pd(_mm256_loadu_pd(mat1Data + i + 4), 
            _mm256_loadu_pd(mat2Data + i + 4)));

        _mm256_storeu_pd((rData+ i + 8),
        _mm256_sub_pd(_mm256_loadu_pd(mat1Data + i + 8), 
            _mm256_loadu_pd(mat2Data + i + 8)));

        _mm256_storeu_pd((rData+ i + 12),
        _mm256_sub_pd(_mm256_loadu_pd(mat1Data + i + 12), 
            _mm256_loadu_pd(mat2Data + i + 12)));
    }
    

    for (int i = dims/16 * 16; i < dims; i++) {
        rData[i] = mat1Data[i] - mat2Data[i];
    }

    return 0;

}

// double multiplyRowCol1(double** m1, double** m2, int h1, int w1, int h2, int w2, int r1, int c2) {
//     double total = 0;
//     for (int i = 0; i < w1; i++) {
//         double val1 = m1[r1][i];
//         double val2 = m2[i][c2];
//         total += val1 * val2;
//     }
//     return total;
// }

// int mul_matrix1(matrix *result, matrix *mat1, matrix *mat2) {
//     /* Check for valid pointers */
//     if (mat1 == NULL || mat2 == NULL || result == NULL) {
//         printf("WARNING: Invalid pointers\n");
//         return -1;
//     }
//     /* Obtain & check for valid dimensions */
//     int mat1Height = mat1->rows;
//     int mat1Width = mat1->cols;
//     int mat2Height = mat2->rows;
//     int mat2Width = mat2->cols;
//     int rHeight = result->rows;
//     int rWidth = result->cols;
//     /* New dimensions will by mat1Height x mat2Width */
//     if (mat1Width < 1 || mat1Height < 1 || mat2Width < 1 || mat2Width < 1 || mat1Width != mat2Height 
//         || rHeight != mat1Height || rWidth != mat2Width) {
//         printf("WARNING: Dimensions incorrect\n");
//         return -1;
//     }
//     /* Perform actual multiplication */
//     for (int i = 0; i < mat1Height; i++) {
//         for (int j = 0; j < mat2Width; j++) {
//             (result->data)[i][j] = multiplyRowCol1(mat1->data, mat2->data, mat1Height, mat1Width, mat2Height, mat2Width, i, j);
//         }
//     }
//     return 0; 
// }

int mul_matrix1(matrix *result, matrix *mat1, matrix *mat2) {
    /* Check for valid pointers */
    if (mat1 == NULL || mat2 == NULL || result == NULL) {
        return -1;
    }
    /* Obtain & check for valid dimensions */
    int mat1Height = mat1->rows;
    int mat1Width = mat1->cols;
    int mat2Height = mat2->rows;
    int mat2Width = mat2->cols;
    int rHeight = result->rows;
    int rWidth = result->cols;
    /* New dimensions will by mat1Height x mat2Width */
    if (mat1Width < 1 || mat1Height < 1 || mat2Width < 1 || mat2Width < 1 || mat1Width != mat2Height 
        || rHeight != mat1Height || rWidth != mat2Width) {
        return -1;
    }
    /* Perform actual multiplication */
    for (int i = 0; i < mat1->rows * mat2->cols; i++) {
        double total = 0;
        for (int k = 0; k < mat1->cols; k++) {
            total += get(mat1, i / mat1->rows, k) * get(mat2, k, i % mat2->cols);
        }
        set(result, i / mat2->cols, i % mat2->cols, total);
    }
    return 0; 
}

/* Return the product of 2 vectors from the matrix given rowNum of m1 and colNum of m2 */
double multiplyRowCol(double* m1, double* m2, int h1, int w1, int h2, int w2, int r1, int c2, int r1w1) {
    double total = 0;
    for (int i = 0; i < w1; i++) {
        total += m1[r1w1 + i] * m2[i * w2 + c2];
    }
    return total;
}

/*
 * Store the result of multiplying mat1 and mat2 to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 * Remember that matrix multiplication is not the same as multiplying individual elements.
 */
int mul_matrix_small(matrix *result, matrix *mat1, matrix *mat2) {
    /* Check for valid pointers */
    if (mat1 == NULL || mat2 == NULL || result == NULL) {
        return -1;
    }
    /* Obtain & check for valid dimensions */
    int mat1Height = mat1->rows;
    int mat1Width = mat1->cols;
    int mat2Height = mat2->rows;
    int mat2Width = mat2->cols;
    int rHeight = result->rows;
    int rWidth = result->cols;

    /* New dimensions will by mat1Height x mat2Width */
    if (mat1Width < 1 || mat1Height < 1 || mat2Width < 1 || mat2Width < 1 || mat1Width != mat2Height 
        || rHeight != mat1Height || rWidth != mat2Width) {
        return -1;
    }
    double* mat1Data = *(mat1->data);
    double* mat2Data = *(mat2->data);
    double* rData = *(result->data);

    for (int i = 0; i < mat1Height; i++) {
        int r1w1 = i * mat1Width;
        for (int j = 0; j < mat2Width; j++) {
            rData[i*mat2Width + j] = multiplyRowCol(mat1Data, mat2Data, mat1Height, mat1Width, mat2Height, mat2Width, i, j, r1w1);
        }
    }
    return 0; 
}

int mul_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    // check for valid matrices and no need to validate the matrix sizes (done in numc.c)
    if (result == NULL || mat1 == NULL || mat2 == NULL) {
        return -1;
    }

    if (mat1->parent != NULL || mat2->parent != NULL) {
        return mul_matrix1(result, mat1, mat2);
    }

    // for simple case
    if (result->rows <= 100 && mat1->cols <= 100 && result->cols <= 100) {
        mul_matrix_small(result, mat1, mat2);
        return 0;
    }

    // for easy access
    double *mat1Data    = *(mat1->data);
    double *mat2Data    = *(mat2->data);
    double *resultData  = *(result->data);

    // allocate the transpose data
    double *tranData  = malloc(mat2->rows * mat2->cols * sizeof(double));
    if (tranData == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to allocate a transpose data.");
        return -1;
    }

    // set 8 threads for experiments

    int m2rows = mat2->rows;
    int m2cols = mat2->cols;
    int m1cols = mat1->cols;

    // set up the transpose matrix
    

    // if (m2rows * m2cols >= 100000) {
    //     omp_set_num_threads(8);
    // }
    //#pragma omp parallel for if (m2rows * m2cols >= 100000)
    #pragma omp parallel for
    for (int i = 0; i < m2rows * m2cols; i++) {
        tranData[i] = mat2Data[(i % m2rows) * m2cols + i / m2rows];
    }

    //#pragma omp parallel for if (result->rows * result->cols >= 100000)
    #pragma omp parallel for
    for (int index = 0; index < result->rows * result->cols; index++) {
        double total = 0;
         __m256d sums = _mm256_set1_pd(0);
        int offsetMat1 = (index / result->cols) * m1cols; 
        int offsetTran = (index % result->cols) * m1cols;
        for (int i = 0 ; i < m1cols / 16 * 16; i = i + 16){ 
            sums = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + offsetMat1 + i),
                                    _mm256_loadu_pd (tranData + offsetTran + i),  
                                    sums);
            sums = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + offsetMat1 + i + 4),
                                    _mm256_loadu_pd (tranData + offsetTran + i + 4),
                                    sums);
            sums = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + offsetMat1 + i + 8),
                                    _mm256_loadu_pd (tranData + offsetTran + i + 8),
                                    sums);
            sums = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + offsetMat1 + i + 12),
                                    _mm256_loadu_pd (tranData + offsetTran + i + 12),
                                    sums);
        }
        total = sums[0] + sums[1] + sums[2] + sums[3];

        for (int i = m1cols / 16 * 16; i < m1cols; i++) {
            total += mat1Data[offsetMat1 + i] * tranData[offsetTran + i];
        }
        resultData[index] = total;
    }
    free(tranData);
    return 0;
}


int mul_matrix_pow(matrix *result, matrix *mat1, matrix *mat2) {
    // for easy access
    double *mat1Data    = *(mat1->data);
    double *mat2Data    = *(mat2->data);
    double *resultData  = *(result->data);

    int size = result->rows; 
    memset(resultData, 0, size * size * sizeof(double));
    if (size < 32) { // simple naive
        // double total = 0;
        // for (int index = 0; index < size * size; index++) {
        //     int i = index / size; int j = index % size;
        //     total = 0; 
        //     for (int k = 0; k < size; k++) {
        //         total += mat1Data[i * size + k] * mat2Data[k * size + j];
        //     }
        //     resultData[index] = total;
        // }
        // return 0;

        #pragma omp parallel for if (size >= 4) 
        for (int i = 0; i < size; i++) {
            for (int k = 0; k < size; k++) {
                for (int j = 0 ; j < size; j++) { 
                    resultData[i * size + j] += mat1Data[i * size + k] * mat2Data[k * size + j]; 
                }
            }
        }
        return 0;
    } else if (size < 192) {
        // allocate the transpose data
        double *tranData  = malloc(size * size * sizeof(double));
        if (tranData == NULL) {
            PyErr_SetString(PyExc_RuntimeError, "Failed to allocate a transpose data.");
            return -1;
        }

        #pragma omp parallel for
        for (int i = 0; i < size * size; i++) {
            tranData[i] = mat2Data[(i % size) * size + i / size];
        }

        #pragma omp parallel for
        for (int index = 0; index < size * size; index++) {
            double total = 0;
            __m256d sums = _mm256_set1_pd(0);
            int i = index / size * size; int j = index % size * size;
            for (int k = 0 ; k < size / 16 * 16; k = k + 16) { 
                sums = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i + k), 
                                        _mm256_loadu_pd (tranData + j + k),
                                        sums);
                sums = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i + k + 4),
                                        _mm256_loadu_pd (tranData + j + k + 4),
                                        sums);
                sums = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i + k + 8),
                                        _mm256_loadu_pd (tranData + j + k + 8),
                                        sums);
                sums = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i + k + 12),
                                        _mm256_loadu_pd (tranData + j + k + 12),
                                        sums);
            }
            total = sums[0] + sums[1] + sums[2] + sums[3];
            for (int k = size / 16 * 16; k < size; k++) {
                total += mat1Data[i + k] * tranData[j + k];
            }
            resultData[index] = total;
        }
        free(tranData);
        return 0;
    } else {
        // allocate the transpose data
        double *tranData  = malloc(size * size * sizeof(double));
        if (tranData == NULL) {
            PyErr_SetString(PyExc_RuntimeError, "Failed to allocate a transpose data.");
            return -1;
        }

        #pragma omp parallel for
        for (int i = 0; i < size * size; i++) {
            tranData[i] = mat2Data[(i % size) * size + i / size];
        }

        #pragma omp parallel for
        for (int index = 0; index < (size * size) / 4 * 4; index = index + 4) {
            // #0
            double total = 0;
            __m256d sums = _mm256_set1_pd(0);
            __m256d secs = _mm256_set1_pd(0);
            int i = index / size * size; int j = index % size * size;
            for (int k = 0 ; k < size / 32 * 32; k = k + 32) { 
                sums = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i + k), 
                                        _mm256_loadu_pd (tranData + j + k),
                                        sums);
                sums = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i + k + 4),
                                        _mm256_loadu_pd (tranData + j + k + 4),
                                        sums);
                sums = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i + k + 8),
                                        _mm256_loadu_pd (tranData + j + k + 8),
                                        sums);
                sums = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i + k + 12),
                                        _mm256_loadu_pd (tranData + j + k + 12),
                                        sums);    
                secs = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i + k + 16), 
                                        _mm256_loadu_pd (tranData + j + k + 16),
                                        secs);
                secs = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i + k + 20),
                                        _mm256_loadu_pd (tranData + j + k + 20),
                                        secs);
                secs = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i + k + 24),
                                        _mm256_loadu_pd (tranData + j + k + 24),
                                        secs);
                secs = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i + k + 28),
                                        _mm256_loadu_pd (tranData + j + k + 28),
                                        secs);                    
            }
            total = sums[0] + sums[1] + sums[2] + sums[3] + secs[0] + secs[1] + secs[2] + secs[3];
            for (int k = size / 32 * 32; k < size; k++) {
                total += mat1Data[i + k] * tranData[j + k];
            }
            resultData[index] = total;
        
            // #1
            double total1 = 0;
            __m256d sums1 = _mm256_set1_pd(0);
            __m256d secs1 = _mm256_set1_pd(0);
            int i1 = (index + 1) / size * size; int j1 = (index + 1) % size * size;
            for (int k = 0 ; k < size / 32 * 32; k = k + 32) { 
                sums1 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i1 + k),
                                         _mm256_loadu_pd (tranData + j1 + k), 
                                         sums1);
                sums1 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i1 + k + 4),
                                         _mm256_loadu_pd (tranData + j1 + k + 4),
                                         sums1);
                sums1 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i1 + k + 8),
                                         _mm256_loadu_pd (tranData + j1 + k + 8),
                                         sums1);
                sums1 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i1 + k + 12),
                                         _mm256_loadu_pd (tranData + j1 + k + 12),
                                         sums1);
                secs1 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i1 + k + 16), 
                                         _mm256_loadu_pd (tranData + j1 + k + 16),
                                         secs1);
                secs1 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i1 + k + 20),
                                         _mm256_loadu_pd (tranData + j1 + k + 20),
                                         secs1);
                secs1 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i1 + k + 24),
                                         _mm256_loadu_pd (tranData + j1 + k + 24),
                                         secs1);
                secs1 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i1 + k + 28),
                                         _mm256_loadu_pd (tranData + j1 + k + 28),
                                         secs1);            
            }
            total1 = sums1[0] + sums1[1] + sums1[2] + sums1[3] + secs1[0] + secs1[1] + secs1[2] + secs1[3];
            for (int k = size / 32 * 32; k < size; k++) {
                total1 += mat1Data[i1 + k] * tranData[j1 + k];
            }
            resultData[index + 1] = total1;
    
            // #2
            double total2 = 0;
            __m256d sums2 = _mm256_set1_pd(0);
            __m256d secs2 = _mm256_set1_pd(0);
            int i2 = (index + 2) / size * size; int j2 = (index + 2) % size * size;
            for (int k = 0 ; k < size / 32 * 32; k = k + 32) { 
                sums2 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i2 + k),
                                         _mm256_loadu_pd (tranData + j2 + k),
                                         sums2);
                sums2 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i2 + k + 4),
                                         _mm256_loadu_pd (tranData + j2 + k + 4),
                                         sums2);
                sums2 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i2 + k + 8),
                                         _mm256_loadu_pd (tranData + j2 + k + 8),
                                         sums2);
                sums2 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i2 + k + 12),
                                         _mm256_loadu_pd (tranData + j2 + k + 12),
                                         sums2);
                secs2 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i2 + k + 16), 
                                         _mm256_loadu_pd (tranData + j2 + k + 16),
                                         secs2);
                secs2 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i2 + k + 20),
                                         _mm256_loadu_pd (tranData + j2 + k + 20),
                                         secs2);
                secs2 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i2 + k + 24),
                                         _mm256_loadu_pd (tranData + j2 + k + 24),
                                         secs2);
                secs2 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i2 + k + 28),
                                         _mm256_loadu_pd (tranData + j2 + k + 28),
                                         secs2);
            }
            total2 = sums2[0] + sums2[1] + sums2[2] + sums2[3] + secs2[0] + secs2[1] + secs2[2] + secs2[3];
            for (int k = size / 32 * 32; k < size; k++) {
                total2 += mat1Data[i2 + k] * tranData[j2 + k];
            }
            resultData[index + 2] = total2;
        
            // #3
            double total3 = 0;
            __m256d sums3 = _mm256_set1_pd(0);
            __m256d secs3 = _mm256_set1_pd(0);
            int i3 = (index + 3) / size * size; int j3 = (index + 3) % size * size;
            for (int k = 0 ; k < size / 32 * 32; k = k + 32){ 
                sums3 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i3 + k),
                                         _mm256_loadu_pd (tranData + j3 + k),
                                         sums3);
                sums3 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i3 + k + 4),
                                         _mm256_loadu_pd (tranData + j3 + k + 4),
                                         sums3);
                sums3 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i3 + k + 8),
                                         _mm256_loadu_pd (tranData + j3 + k + 8),
                                         sums3);
                sums3 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i3 + k + 12),
                                         _mm256_loadu_pd (tranData + j3 + k + 12),
                                         sums3);
                secs3 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i3 + k + 16), 
                                         _mm256_loadu_pd (tranData + j3 + k + 16),
                                         secs3);
                secs3 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i3 + k + 20),
                                         _mm256_loadu_pd (tranData + j3 + k + 20),
                                         secs3);
                secs3 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i3 + k + 24),
                                         _mm256_loadu_pd (tranData + j3 + k + 24),
                                         secs3);
                secs3 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i3 + k + 28),
                                         _mm256_loadu_pd (tranData + j3 + k + 28),
                                         secs3);      
            }
            total3 = sums3[0] + sums3[1] + sums3[2] + sums3[3] + secs3[0] + secs3[1] + secs3[2] + secs3[3];
            for (int k = size / 32 * 32; k < size; k++) {
                total3 += mat1Data[i3 + k] * tranData[j3 + k];
            }
            resultData[index + 3] = total3;
        }
        // tail case
        for (int index = (size * size) / 4 * 4; index < size * size; index++) {
            int i = index / size; int j = index % size;
            double total = 0; 
            for (int k = 0; k < size; k++) {
                total += mat1Data[i * size + k] * mat2Data[k * size + j];
            }
            resultData[index] = total;
        }
        free(tranData);
        return 0;
    }
}

// int mul_pow2(matrix *result, matrix *mat1, matrix *mat2) {
//     // check for valid matrices and no need to validate the matrix sizes (done in numc.c)
//     if (result == NULL || mat1 == NULL || mat2 == NULL) {
//         return -1;
//     }

//     if (mat1->parent != NULL || mat2->parent != NULL) {
//         return mul_matrix1(result, mat1, mat2);
//     }

//     double *mat1Data    = *(mat1->data);
//     double *mat2Data    = *(mat2->data);
//     double *resultData  = *(result->data);

//     int size = result->rows; 
//     if (size <= 100) { // simple naive
//         double total = 0;
//         for (int index = 0; index < size * size; index++) {
//             int i = index / size; int j = index % size;
//             total = 0; 
//             for (int k = 0; k < size; k++) {
//                 total += mat1Data[i * size + k] * mat2Data[k * size + j];
//             }
//             resultData[index] = total;
//         }
//         return 0;
//     } else if (size <= 600) { // no transpose
//         #pragma omp parallel for
//         for (int index = 0; index < (size * size) / 4 * 4; index = index + 4) {
//             // #0
//             double total = 0;
//             __m256d sums = _mm256_set1_pd(0);
//             int i = index / size; int j = index % size;
//             for (int k = 0 ; k < size / 16 * 16; k = k + 16){ 
//                 sums = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i * size + k),
//                                         _mm256_loadu_pd (mat2Data + k * size + j),  
//                                         sums);
//                 sums = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i * size + k + 4),
//                                         _mm256_loadu_pd (mat2Data + (k +  4) * size + j),
//                                         sums);
//                 sums = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i * size + k + 8),
//                                         _mm256_loadu_pd (mat2Data + (k +  8) * size + j),
//                                         sums);
//                 sums = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i * size + k + 12),
//                                         _mm256_loadu_pd (mat2Data + (k + 12) * size + j),
//                                         sums);
//             }
//             total = sums[0] + sums[1] + sums[2] + sums[3];
//             for (int k = size / 16 * 16; k < size; k++) {
//                 total += mat1Data[i * size + k] * mat2Data[k * size + j];
//             }
//             resultData[index] = total;

//             // #1
//             double total1 = 0;
//             __m256d sums1 = _mm256_set1_pd(0);
//             int i1 = (index + 1) / size; int j1 = (index + 1) % size;
//             for (int k = 0 ; k < size / 16 * 16; k = k + 16){ 
//                 sums1 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i1 * size + k),
//                                          _mm256_loadu_pd (mat2Data + k * size + j1),  
//                                          sums1);
//                 sums1 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i1 * size + k + 4),
//                                          _mm256_loadu_pd (mat2Data + (k +  4) * size + j1),
//                                          sums1);
//                 sums1 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i1 * size + k + 8),
//                                          _mm256_loadu_pd (mat2Data + (k +  8) * size + j1),
//                                          sums1);
//                 sums1 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i1 * size + k + 12),
//                                          _mm256_loadu_pd (mat2Data + (k + 12) * size + j1),
//                                          sums1);
//             }
//             total1 = sums1[0] + sums1[1] + sums1[2] + sums1[3];
//             for (int k = size / 16 * 16; k < size; k++) {
//                 total1 += mat1Data[i1 * size + k] * mat2Data[k * size + j1];
//             }
//             resultData[index + 1] = total1;
        
//             // #2
//             double total2 = 0;
//             __m256d sums2 = _mm256_set1_pd(0);
//             int i2 = (index + 2) / size; int j2 = (index + 2) % size;
//             for (int k = 0 ; k < size / 16 * 16; k = k + 16){ 
//                 sums2 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i2 * size + k),
//                                          _mm256_loadu_pd (mat2Data + k * size + j2),  
//                                          sums2);
//                 sums2 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i2 * size + k + 4),
//                                          _mm256_loadu_pd (mat2Data + (k +  4) * size + j2),
//                                          sums2);
//                 sums2 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i2 * size + k + 8),
//                                          _mm256_loadu_pd (mat2Data + (k +  8) * size + j2),
//                                          sums2);
//                 sums2 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i2 * size + k + 12),
//                                          _mm256_loadu_pd (mat2Data + (k + 12) * size + j2),
//                                          sums2);
//             }
//             total2 = sums2[0] + sums2[1] + sums2[2] + sums2[3];
//             for (int k = size / 16 * 16; k < size; k++) {
//                 total2 += mat1Data[i2 * size + k] * mat2Data[k * size + j2];
//             }
//             resultData[index + 2] = total2;
        
//             // #3
//             double total3 = 0;
//             __m256d sums3 = _mm256_set1_pd(0);
//             int i3 = (index + 3) / size; int j3 = (index + 3) % size;
//             for (int k = 0 ; k < size / 16 * 16; k = k + 16){ 
//                 sums3 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i3 * size + k),
//                                          _mm256_loadu_pd (mat2Data + k * size + j3),  
//                                          sums3);
//                 sums3 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i3 * size + k + 4),
//                                          _mm256_loadu_pd (mat2Data + (k +  4) * size + j3),
//                                          sums3);
//                 sums3 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i3 * size + k + 8),
//                                          _mm256_loadu_pd (mat2Data + (k +  8) * size + j3),
//                                          sums3);
//                 sums3 = _mm256_fmadd_pd( _mm256_loadu_pd (mat1Data + i3 * size + k + 12),
//                                          _mm256_loadu_pd (mat2Data + (k + 12) * size + j3),
//                                          sums3);
//             }
//             total3 = sums3[0] + sums3[1] + sums3[2] + sums3[3];
//             for (int k = size / 16 * 16; k < size; k++) {
//                 total3 += mat1Data[i3 * size + k] * mat2Data[k * size + j3];
//             }
//             resultData[index + 3] = total3;
//         }
//         // tail case
//         for (int index = (size * size) / 4 * 4; index < size * size; index++) {
//             int i = index / size; int j = index % size;
//             double total = 0; 
//             for (int k = 0; k < size; k++) {
//                 total += mat1Data[i * size + k] * mat2Data[k * size + j];
//             }
//             resultData[index] = total;
//         }
//         return 0;
//     } else {
//         mul_matrix(result, mat1, mat2);
//         return 0;
//     }
// }

// int mul_matrix_self1(matrix *result, matrix *mat1, matrix *mat2) {
//     /* Check for valid pointers */
//     if (mat1 == NULL || mat2 == NULL || result == NULL) {
//         printf("WARNING: Invalid pointers\n");
//         return -1;
//     }

//     /* Obtain & check for valid dimensions */
//     int mat1Height = mat1->rows;
//     int mat1Width = mat1->cols;
//     int mat2Height = mat2->rows;
//     int mat2Width = mat2->cols;
//     int rHeight = result->rows;
//     int rWidth = result->cols;
//     /* New dimensions will by mat1Height x mat2Width */
//     if (mat1Width < 1 || mat1Height < 1 || mat2Width < 1 || mat2Width < 1 || mat1Width != mat2Height 
//         || rHeight != mat1Height || rWidth != mat2Width) {
//         printf("WARNING: Dimensions incorrect\n");
//         return -1;
//     }
//     /* Perform actual multiplication */
//     matrix* copy = NULL;
//     if (allocate_matrix(&copy, result->rows, result->cols) != 0) {
//         return -1;
//     }
//     for (int i = 0; i < mat1->rows * mat2->cols; i++) {
//         double total = 0;
//         for (int k = 0; k < mat1->cols; k++) {
//             total += get(mat1, i / mat1->rows, k) * get(mat2, k, i % mat2->cols);
//         }
//         set(copy, i / mat2->cols, i % mat2->cols, total);
//     }
//     copyData1(copy, result);
//     freeMatrix(copy);
//     return 0;
// }

// int mul_matrix_self(matrix *result, matrix *mat1, matrix *mat2) {

//     /* Check for valid pointers */
//     if (mat1 == NULL || mat2 == NULL || result == NULL) {
//         printf("WARNING: Invalid pointers\n");
//         return -1;
//     }
//     if (mat1->parent != NULL || mat2->parent != NULL) {
//         return mul_matrix_self1(result, mat1, mat2);
//     }
//     /* Obtain & check for valid dimensions */
//     int mat1Height = mat1->rows;
//     int mat1Width = mat1->cols;
//     int mat2Height = mat2->rows;
//     int mat2Width = mat2->cols;
//     int rHeight = result->rows;
//     int rWidth = result->cols;
//     /* New dimensions will by mat1Height x mat2Width */
//     if (mat1Width < 1 || mat1Height < 1 || mat2Width < 1 || mat2Width < 1 || mat1Width != mat2Height 
//         || rHeight != mat1Height || rWidth != mat2Width) {
//         printf("WARNING: Dimensions incorrect\n");
//         return -1;
//     }
//     matrix* copy = NULL;
//     if (allocate_matrix(&copy, result->rows, result->cols) != 0) {
//         return -1;
//     }

//     double* mat1Data = *(mat1->data);
//     double* mat2Data = *(mat2->data);
//     double* cData = *(copy->data);

//     for (int i = 0; i < mat1Height; i++) {
//         int r1w1 = i * mat1Width;
//         for (int j = 0; j < mat2Width; j++) {
//             cData[i*mat2Width + j] = multiplyRowCol(mat1Data, mat2Data, mat1Height, mat1Width, mat2Height, mat2Width, i, j, r1w1);
//         }
//     }
    
//     copyData(copy, result);
//     freeMatrix(copy);
//     return 0; 
// }


// int pow_matrix_prev(matrix *result, matrix *mat, int pow) {
//     /* TODO: YOUR CODE HERE */
//     if (mat == NULL) {
//         return -1;
//     }
//     int size = mat->rows * mat->cols * sizeof(double); 
//     if (pow == 0) {
//         return makeIdentity(result);
//     } else if (pow == 1) {
//         memcpy(*(result->data), *(mat->data), size);
//         return 0;
//     }
    
//     matrix *temp1 = NULL, *temp2 = NULL;
//     if (allocate_matrix(&temp1, result->rows, result->cols) == -1) {
//         return -1;     
//     }
//     if (allocate_matrix(&temp2, result->rows, result->cols) == -1) {
//         return -1;     
//     }
//     memcpy(*(temp1->data), *(mat->data), size);
//     makeIdentity(result);
            
//     while (pow > 0) {
//         if (pow % 2 == 1) {
//             mul_matrix1(temp2, result, temp1);
//             memcpy(*(result->data), *(temp2->data), size);
//         }
//         pow = pow / 2;
//         if (pow > 0) {
//             mul_matrix1(temp2, temp1, temp1);
//             memcpy(*(temp1->data), *(temp2->data), size);
//         }
//     }
//     freeMatrix(temp1); 
//     freeMatrix(temp2); 
//     return 0;  
// }

int pow_matrix_prev(matrix *result, matrix *mat, int pow) {
    /* TODO: YOUR CODE HERE */
    if (mat == NULL) {
        return -1;
    }
    if (pow == 0) {
        return makeIdentity1(result);
    } else if (pow == 1) {
        copyData1(mat, result); 
        return 0;
    }
    
    matrix *temp1 = NULL, *temp2 = NULL;
    if (allocate_matrix(&temp1, result->rows, result->cols) == -1) {
        return -1;     
    }
    if (allocate_matrix(&temp2, result->rows, result->cols) == -1) {
        return -1;     
    }
    copyData1(mat, temp1);
    makeIdentity1(result);
            
    while (pow > 0) {
        if (pow % 2 == 1) {
            mul_matrix1(temp2, result, temp1);
            copyData1(temp2, result);
        }
        pow = pow / 2;
        if (pow > 0) {
            mul_matrix1(temp2, temp1, temp1);
            copyData1(temp2, temp1);
        }
    }
    freeMatrix(temp1); 
    freeMatrix(temp2); 
    return 0;  
}



int mul_pow1(matrix* result, matrix* mat1, matrix* mat2) {
    if (result == NULL || mat1 == NULL || mat2 == NULL) {
        return -1;
    }
    // for easy access
    double *mat1Data    = *(mat1->data);
    double *mat2Data    = *(mat2->data);
    double *resultData  = *(result->data);
    int size = mat1->rows;
    double total = 0;
    for (int index = 0; index < size * size; index++) {
        //printf("REACHED 1\n");
        int i = index / size; 
        int j = index % size;
        total = 0; 
        for (int k = 0; k < size; k++) {
            total += mat1Data[i * size + k] * mat2Data[k * size + j];
            //printf("REACHED 2\n");
        }
        //printf("total: %f\n", total);
        resultData[index] = total;
    }
    return 0;
}


int mul_pow1a(matrix* result, matrix* mat1, matrix* mat2) {
    if (result == NULL || mat1 == NULL || mat2 == NULL) {
        return -1;
    }
    // for easy access
    double *mat1Data    = *(mat1->data);
    double *mat2Data    = *(mat2->data);
    double *resultData  = *(result->data);
    int size = mat1->rows;
    double total = 0;
    for (int index = 0; index < size * size; index++) {
        int i = index / size; 
        int j = index % size;
        total = 0; 
        for (int k = 0; k < size/4 * 4; k += 4) {
            total += mat1Data[i * size + k] * mat2Data[k * size + j];
            total += mat1Data[i * size + k + 1] * mat2Data[(k + 1) * size + j];
            total += mat1Data[i * size + k + 2] * mat2Data[(k + 2) * size + j];
            total += mat1Data[i * size + k + 3] * mat2Data[(k + 3) * size + j];
        }
        for (int k = size/4 * 4; k < size; k++) {
            total += mat1Data[i * size + k] * mat2Data[k * size + j];
        }
        resultData[index] = total;
    }
    return 0;
}

int mul_pow1b(matrix* result, matrix* mat1, matrix* mat2) {
    if (result == NULL || mat1 == NULL || mat2 == NULL) {
        return -1;
    }
    // for easy access
    double *mat1Data    = *(mat1->data);
    double *mat2Data    = *(mat2->data);
    double *resultData  = *(result->data);
    int size = mat1->rows;
    double total = 0;
    for (int index = 0; index < size * size; index++) {
        int i = index / size; 
        int j = index % size;
        total = 0; 
        for (int k = 0; k < size/8 * 8; k += 8) {
            total += mat1Data[i * size + k] * mat2Data[k * size + j];
            total += mat1Data[i * size + k + 1] * mat2Data[(k + 1) * size + j];
            total += mat1Data[i * size + k + 2] * mat2Data[(k + 2) * size + j];
            total += mat1Data[i * size + k + 3] * mat2Data[(k + 3) * size + j];
            total += mat1Data[i * size + k + 4] * mat2Data[(k + 4) * size + j];
            total += mat1Data[i * size + k + 5] * mat2Data[(k + 5) * size + j];
            total += mat1Data[i * size + k + 6] * mat2Data[(k + 6) * size + j];
            total += mat1Data[i * size + k + 7] * mat2Data[(k + 7) * size + j];
        }
        for (int k = size/8 * 8; k < size; k++) {
            total += mat1Data[i * size + k] * mat2Data[k * size + j];
        }
        resultData[index] = total;
    }
    return 0;
}

int pow_matrix(matrix *result, matrix *mat, int pow) {
    /* TODO: YOUR CODE HERE */
    if (mat == NULL) {
        return -1;
    }
    if (mat->parent != NULL) {
        return pow_matrix_prev(result, mat, pow);
    }
    int size = mat->rows * mat->cols * sizeof(double); 
    if (pow == 0) {
        return makeIdentity(result);
    } else if (pow == 1) {
        memcpy(*(result->data), *(mat->data), size);
        return 0;
    }
    
    matrix *temp1 = NULL, *temp2 = NULL;
    if (allocate_matrix(&temp1, result->rows, result->cols) == -1) {
        return -1;     
    }
    if (allocate_matrix(&temp2, result->rows, result->cols) == -1) {
        return -1;     
    }
    memcpy(*(temp1->data), *(mat->data), size);
    makeIdentity(result);

    double **swap = NULL;    
    while (pow > 0) {
        if (pow % 2 == 1) {
            // if (mat->rows <= 25) {
            //     mul_matrix_small(temp2, result, temp1);
            // }
            // else {
            //     mul_matrix_pow(temp2, result, temp1);
            // }
            mul_matrix_pow(temp2, result, temp1);
            // memcpy(*(result->data), *(temp2->data), size);
            swap = result->data;
            result->data = temp2->data;
            temp2->data = swap;
        }
        pow = pow / 2;
        if (pow > 0) {
            // if (mat->rows <= 25) {
            //     mul_matrix_small(temp2, temp1, temp1);
            // }
            // else {
            //     mul_matrix_pow(temp2, temp1, temp1);
            // }
            mul_matrix_pow(temp2, temp1, temp1);
            // memcpy(*(temp1->data), *(temp2->data), size);
            swap = temp1->data;
            temp1->data = temp2->data;
            temp2->data = swap;
        }
    }
    freeMatrix(temp1); 
    freeMatrix(temp2); 
    return 0;    
}


// int neg_matrix1(matrix *result, matrix *mat) {
//     /* TODO: YOUR CODE HERE */

//     /* Check for a valid pointer */
//     if (mat == NULL || result == NULL) {
//         return -1;
//     }
//     int mHeight = mat->rows;
//     int mWidth = mat->cols;
//     int rHeight = result->rows;
//     int rWidth = result->cols;

//     /* Check for valid dimensions */
//     if (mHeight <= 0 || mWidth <= 0 || mHeight != rHeight || mWidth != rWidth) {
//         return -1;
//     }

//     /* Get data + check that it's valid */
//     double** matData = mat->data;
//     double** resultData = result->data;

//     /* Perform negate */
//     for (int i = 0; i < mHeight; i++) {
//         for (int j = 0; j < mWidth; j++) {
//             resultData[i][j] = -1 * matData[i][j];
//         }
//     }
//     return 0;
// }

int neg_matrix1(matrix *result, matrix *mat) {
    /* TODO: YOUR CODE HERE */

    /* Check for a valid pointer */
    if (mat == NULL || result == NULL) {
        return -1;
    }
    int mHeight = mat->rows;
    int mWidth = mat->cols;
    int rHeight = result->rows;
    int rWidth = result->cols;

    /* Check for valid dimensions */
    if (mHeight <= 0 || mWidth <= 0 || mHeight != rHeight || mWidth != rWidth) {
        return -1;
    }
    /* Perform negate */
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        double val = get(mat, i / mat->cols, i % mat->cols);
        set (result, i / mat->cols, i % mat->cols, -1 * val); 
    }
    return 0;
}

/*
 * Store the result of element-wise negating mat's entries to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int neg_matrix(matrix *result, matrix *mat) {
    /* TODO: YOUR CODE HERE */

    if (mat == NULL || result == NULL) {
        return -1;
    }
    if (mat->parent != NULL) {
        return neg_matrix1(result, mat);
    }
    int mHeight = mat->rows;
    int mWidth = mat->cols;
    int rHeight = result->rows;
    int rWidth = result->cols;

    /* Check for valid dimensions */
    if (mHeight <= 0 || mWidth <= 0 || mHeight != rHeight || mWidth != rWidth) {
        return -1;
    }

    /* Get + check for valid data */
    double* rData = *(result->data);
    double* mData = *(mat->data);
    int dims = mHeight * mWidth;
    __m256d zeros = _mm256_set1_pd(0);

    int numThreads = 8;
    omp_set_num_threads(numThreads);

    #pragma omp parallel for if (dims >= 100000)
    for (int i = 0; i < dims/16 * 16; i += 16) {
        _mm256_storeu_pd((rData + i + 0),
        _mm256_sub_pd(zeros, _mm256_loadu_pd(mData + i + 0)));

        _mm256_storeu_pd((rData + i + 4),
        _mm256_sub_pd(zeros, _mm256_loadu_pd(mData + i + 4)));

        _mm256_storeu_pd((rData + i + 8),
        _mm256_sub_pd(zeros, _mm256_loadu_pd(mData + i + 8)));

        _mm256_storeu_pd((rData + i + 12),
        _mm256_sub_pd(zeros, _mm256_loadu_pd(mData + i + 12)));
    }

    for (int i = dims/16 * 16; i < dims; i++) {
        rData[i] = 0 - mData[i];
    }

    return 0;
}

// int abs_matrix1(matrix *result, matrix *mat) {
//     /* TODO: YOUR CODE HERE */
//     /* Check for a valid pointer */
//     if (mat == NULL || result == NULL) {
//         return -1;
//     }
//     int mHeight = mat->rows;
//     int mWidth = mat->cols;
//     int rHeight = result->rows;
//     int rWidth = result->cols;
//     /* Check for valid dimensions */
//     if (mHeight <= 0 || mWidth <= 0 || mHeight != rHeight || mWidth != rWidth) {
//         return -1;
//     }

//     /* Get data + check that it's valid */
//     double** matData = mat->data;
//     double** resultData = result->data;

//     /* Perform abs */
//     for (int i = 0; i < mHeight; i++) {
//         if (resultData[i] == NULL || matData[i] == NULL) {
//             return -1;
//         }
//         for (int j = 0; j < mWidth; j++) {
//             if (matData[i][j] < 0) {
//                 resultData[i][j] = -1 * matData[i][j];
//             }
//             else {
//                 resultData[i][j] = matData[i][j];
//             }
//         }
//     }

//     return 0;
// }

int abs_matrix1(matrix *result, matrix *mat) {
    /* TODO: YOUR CODE HERE */
    /* Check for a valid pointer */
    if (mat == NULL || result == NULL) {
        return -1;
    }
    int mHeight = mat->rows;
    int mWidth = mat->cols;
    int rHeight = result->rows;
    int rWidth = result->cols;
    /* Check for valid dimensions */
    if (mHeight <= 0 || mWidth <= 0 || mHeight != rHeight || mWidth != rWidth) {
        return -1;
    }

    /* Perform abs */
    for (int i = 0; i < mat->rows * mat->cols; i++) {
        double val = get(mat, i / mat->cols, i % mat->cols);
        if (val < 0) {
            set(result, i / mat->cols, i % mat->cols, -1 * val);     
        } else {
            set(result, i / mat->cols, i % mat->cols, val);     
        }
    }
    return 0;
}

int abs_matrix(matrix *result, matrix *mat) {
    /* TODO: YOUR CODE HERE */
    if (mat == NULL || result == NULL) {
        return -1;
    }
    if (mat->parent != NULL) {
        return abs_matrix1(result, mat);
    }
    int mHeight = mat->rows;
    int mWidth = mat->cols;
    int rHeight = result->rows;
    int rWidth = result->cols;

    /* Check for valid dimensions */
    if (mHeight <= 0 || mWidth <= 0 || mHeight != rHeight || mWidth != rWidth) {
        return -1;
    }

    /* Get + check for valid data */
    double* rData = *(result->data);
    double* mData = *(mat->data);
    int dims = mHeight * mWidth;
    __m256d zeros = _mm256_set1_pd(0);

    int numThreads = 8;
    omp_set_num_threads(numThreads);
    
    #pragma omp parallel for if (dims >= 100000)
    for (int i = 0; i < dims/16 * 16; i += 16) {
        __m256d m1_v0 = _mm256_loadu_pd(mData + i + 0);
        __m256d dif_v0 = _mm256_sub_pd(zeros, m1_v0);
        __m256d abs_v0 = _mm256_max_pd(dif_v0, m1_v0);
        _mm256_storeu_pd((rData + i + 0), abs_v0);

        __m256d m1_v1 = _mm256_loadu_pd(mData + i + 4);
        __m256d dif_v1 = _mm256_sub_pd(zeros, m1_v1);
        __m256d abs_v1 = _mm256_max_pd(dif_v1, m1_v1);
        _mm256_storeu_pd((rData + i + 4), abs_v1);

        __m256d m1_v2 = _mm256_loadu_pd(mData + i + 8);
        __m256d dif_v2 = _mm256_sub_pd(zeros, m1_v2);
        __m256d abs_v2 = _mm256_max_pd(dif_v2, m1_v2);
        _mm256_storeu_pd((rData + i + 8), abs_v2);

        __m256d m1_v3 = _mm256_loadu_pd(mData + i + 12);
        __m256d dif_v3 = _mm256_sub_pd(zeros, m1_v3);
        __m256d abs_v3 = _mm256_max_pd(dif_v3, m1_v3);
        _mm256_storeu_pd((rData + i + 12), abs_v3);
    }

    for (int i = dims/16 * 16; i < dims; i++) {
        double val = mData[i];
        rData[i] = (val >= 0) ? val : -val;
    }

    return 0;
}