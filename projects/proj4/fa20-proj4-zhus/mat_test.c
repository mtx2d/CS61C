#include <stdio.h>

#include "CUnit/Basic.h"
#include "CUnit/CUnit.h"
#include "matrix.h"

/* Test Suite setup and cleanup functions: */
int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

/************* Test case functions ****************/
void add_test(void) {
    matrix *result = NULL;
    matrix *mat1 = NULL;
    matrix *mat2 = NULL;
    CU_ASSERT_EQUAL(allocate_matrix(&result, 2, 2), 0);
    CU_ASSERT_EQUAL(allocate_matrix(&mat1, 2, 2), 0);
    CU_ASSERT_EQUAL(allocate_matrix(&mat2, 2, 2), 0);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            set(mat1, i, j, i * 2 + j);
            set(mat2, i, j, i * 2 + j);
        }
    }
    add_matrix(result, mat1, mat2);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            CU_ASSERT_EQUAL(get(result, i, j), 2 * (i * 2 + j));
        }
    }
    deallocate_matrix(result);
    deallocate_matrix(mat1);
    deallocate_matrix(mat2);
}

void sub_test(void) {
    matrix *result = NULL;
    matrix *mat1 = NULL;
    matrix *mat2 = NULL;
    CU_ASSERT_EQUAL(allocate_matrix(&result, 2, 2), 0);
    CU_ASSERT_EQUAL(allocate_matrix(&mat1, 2, 2), 0);
    CU_ASSERT_EQUAL(allocate_matrix(&mat2, 2, 2), 0);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            set(mat1, i, j, i * 2 + j);
            set(mat2, i, j, (i * 2 + j) * 3);
        }
    }
    sub_matrix(result, mat1, mat2);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            CU_ASSERT_EQUAL(get(result, i, j), (-2) * (i * 2 + j));
        }
    }
    deallocate_matrix(result);
    deallocate_matrix(mat1);
    deallocate_matrix(mat2);
}

void mul_test(void) {
    matrix *result = NULL;
    matrix *mat1 = NULL;
    matrix *mat2 = NULL;
    CU_ASSERT_EQUAL(allocate_matrix(&result, 3, 3), 0);
    CU_ASSERT_EQUAL(allocate_matrix(&mat1, 3, 3), 0);
    CU_ASSERT_EQUAL(allocate_matrix(&mat2, 3, 3), 0);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            set(mat1, i, j, i * 3 + j + 1);
            set(mat2, i, j, i * 3 + j + 1);
        }
    }
    mul_matrix(result, mat1, mat2);
    CU_ASSERT_EQUAL(get(result, 0, 0), 30);
    CU_ASSERT_EQUAL(get(result, 0, 1), 36);
    CU_ASSERT_EQUAL(get(result, 0, 2), 42);
    CU_ASSERT_EQUAL(get(result, 1, 0), 66);
    CU_ASSERT_EQUAL(get(result, 1, 1), 81);
    CU_ASSERT_EQUAL(get(result, 1, 2), 96);
    CU_ASSERT_EQUAL(get(result, 2, 0), 102);
    CU_ASSERT_EQUAL(get(result, 2, 1), 126);
    CU_ASSERT_EQUAL(get(result, 2, 2), 150);
    deallocate_matrix(result);
    deallocate_matrix(mat1);
    deallocate_matrix(mat2);
}

void neg_test(void) {
    matrix *result = NULL;
    matrix *mat = NULL;
    CU_ASSERT_EQUAL(allocate_matrix(&result, 2, 2), 0);
    CU_ASSERT_EQUAL(allocate_matrix(&mat, 2, 2), 0);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            set(mat, i, j, i * 2 + j);
        }
    }
    neg_matrix(result, mat);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            CU_ASSERT_EQUAL(get(result, i, j), -(i * 2 + j));
        }
    }
    deallocate_matrix(result);
    deallocate_matrix(mat);
}

void abs_test(void) {
    matrix *result = NULL;
    matrix *mat = NULL;
    CU_ASSERT_EQUAL(allocate_matrix(&result, 2, 2), 0);
    CU_ASSERT_EQUAL(allocate_matrix(&mat, 2, 2), 0);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (j % 2 == 0)
                set(mat, i, j, i * 2 + j);
            else
                set(mat, i, j, -(i * 2 + j));
        }
    }
    abs_matrix(result, mat);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            CU_ASSERT_EQUAL(get(result, i, j), i * 2 + j);
        }
    }
    deallocate_matrix(result);
    deallocate_matrix(mat);
}

void pow_test(void) {
    matrix *result = NULL;
    matrix *mat = NULL;
    CU_ASSERT_EQUAL(allocate_matrix(&result, 2, 2

                                   ),
                    0);
    CU_ASSERT_EQUAL(allocate_matrix(&mat, 2, 2), 0);
    set(mat, 0, 0, 1);
    set(mat, 0, 1, 1);
    set(mat, 1, 0, 1);
    set(mat, 1, 1, 0);
    pow_matrix(result, mat, 3);
    CU_ASSERT_EQUAL(get(result, 0, 0), 3);
    CU_ASSERT_EQUAL(get(result, 0, 1), 2);
    CU_ASSERT_EQUAL(get(result, 1, 0), 2);
    CU_ASSERT_EQUAL(get(result, 1, 1), 1);
    pow_matrix(result, mat, 10);
    CU_ASSERT_EQUAL(get(result, 0, 0), 89);
    CU_ASSERT_EQUAL(get(result, 0, 1), 55);
    CU_ASSERT_EQUAL(get(result, 1, 0), 55);
    CU_ASSERT_EQUAL(get(result, 1, 1), 34);
    deallocate_matrix(result);
    deallocate_matrix(mat);
}

void alloc_fail_test(void) {
    matrix *mat = NULL;
    CU_ASSERT_EQUAL(allocate_matrix(&mat, 0, 0), -1);
    CU_ASSERT_EQUAL(allocate_matrix(&mat, 0, 1), -1);
    CU_ASSERT_EQUAL(allocate_matrix(&mat, 1, 0), -1);
}

void alloc_success_test(void) {
    matrix *mat = NULL;
    CU_ASSERT_EQUAL(allocate_matrix(&mat, 3, 2), 0);
    CU_ASSERT_EQUAL(mat->parent, NULL);
    CU_ASSERT_EQUAL(mat->ref_cnt, 1);
    CU_ASSERT_EQUAL(mat->rows, 3);
    CU_ASSERT_EQUAL(mat->cols, 2);
    CU_ASSERT_NOT_EQUAL(mat->data, NULL);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            CU_ASSERT_EQUAL(get(mat, i, j), 0);
        }
    }
    deallocate_matrix(mat);
}

void alloc_ref_test(void) {
    matrix *mat1 = NULL;
    matrix *mat2 = NULL;
    matrix *from = NULL;
    allocate_matrix(&from, 3, 2);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            set(from, i, j, i * 2 + j);
        }
    }
    /* 2D slice */
    CU_ASSERT_EQUAL(allocate_matrix_ref(&mat1, from, 1, 0, 2, 2), 0);
    CU_ASSERT_PTR_EQUAL(mat1->parent, from);
    CU_ASSERT_EQUAL(mat1->parent->ref_cnt, 2);
    CU_ASSERT_EQUAL(mat1->rows, 2);
    CU_ASSERT_EQUAL(mat1->cols, 2);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            CU_ASSERT_EQUAL(get(mat1, i, j), get(from, i + 1, j));
        }
    }
    /* 1D slice */
    CU_ASSERT_EQUAL(allocate_matrix_ref(&mat2, from, 1, 0, 2, 1), 0);
    CU_ASSERT_PTR_EQUAL(mat2->parent, from);
    CU_ASSERT_EQUAL(mat2->parent->ref_cnt, 3);
    CU_ASSERT_EQUAL(mat2->rows, 2);
    CU_ASSERT_EQUAL(mat2->cols, 1);
    CU_ASSERT_NOT_EQUAL(mat2->is_1d, 0);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 1; j++) {
            CU_ASSERT_EQUAL(get(mat2, i, j), get(from, i + 1, j));
        }
    }
    /* Now we compare the data in the reference matrix */
    deallocate_matrix(from);
    deallocate_matrix(mat1);
    deallocate_matrix(mat2);
}

void allocate_ref1() {
    matrix *from;
    allocate_matrix(&from, 2, 2);
    // from contains: [[1, 2], [3, 4]]
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            set(from, i, j, i * 2 + j + 1);
        }
    }
    matrix *to;
    // to contains: [[3, 4]]
    allocate_matrix_ref(&to, from, 1, 0, 1, 2); // row_offset, col_offset, rows, cols
    double *toData = *(to->data);
    double *fromData = *(from->data);
    printf("FROM (pre-set) 0, 0: %f\n", fromData[0]);
    printf("FROM (pre-set) 0, 1: %f\n", fromData[1]);
    printf("FROM (pre-set) 1, 0: %f\n", fromData[2]);
    printf("FROM (pre-set) 1, 1: %f\n", fromData[3]);
    printf("TO (pre-set) 0, 0: %f\n", toData[0]);
    printf("TO (pre-set) 0, 1: %f\n", toData[1]);
    set(to, 0, 0, 10);
    printf("FROM (post-set) 0, 0: %f\n", fromData[0]);
    printf("FROM (post-set) 0, 1: %f\n", fromData[1]);
    printf("FROM (post-set) 1, 0: %f\n", fromData[2]);
    printf("FROM (post-set) 1, 1: %f\n", fromData[3]);
    printf("TO (post-set) 0, 0: %f\n", toData[0]);
    printf("TO (post-set) 0, 1: %f\n", toData[1]);
    

}

void allocate_ref2() {
    matrix *from;
    allocate_matrix(&from, 4, 4);
    // from contains: [[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12], [13, 14, 15, 16]]
    int counter = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            set(from, i, j, counter);
            counter++;
        }
    }

    matrix *to;
    // to contains: [[6, 7], [10, 11]]
    allocate_matrix_ref(&to, from, 1, 1, 2, 2); // row_offset, col_offset, rows, cols
    
    matrix *new;
    // contains: [10, 11]
    allocate_matrix_ref(&new, to, 1, 0, 1, 2); // row_offset, col_offset, rows, cols

    double* toData = *(to->data);
    double* fromData = *(from->data);
    double* newData = *(new->data);
    
    counter = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("FROM preset %i, %i: %f\n", i, j, fromData[counter]);
            counter++;
        }
    }

    counter = 0;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            printf("TO preset %i, %i: %f\n", i, j, toData[counter]);
            counter++;
        }
    }

    printf("New 0, 0: %f\n", newData[0]);
    printf("New 0, 1: %f\n", newData[1]);

    set(new, 0, 0, 20);

    counter = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("FROM preset %i, %i: %f\n", i, j, fromData[counter]);
            counter++;
        }
    }

    counter = 0;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            printf("TO preset %i, %i: %f\n", i, j, toData[counter]);
            counter++;
        }
    }
    printf("New 0, 0: %f\n", newData[0]);
    printf("New 0, 1: %f\n", newData[1]);

}

void allocate_ref3() {
    matrix *from;
    allocate_matrix(&from, 2, 2);
    // from contains: [[1, 0], [2, 2]]
    set(from, 0, 0, 1);
    set(from, 0, 1, 0);
    set(from, 1, 0, 2);
    set(from, 1, 1, 2);

    matrix *to;
    // to contains: [[2, 2]]
    allocate_matrix_ref(&to, from, 1, 0, 1, 2); // row_offset, col_offset, rows, cols
    double *toData = *(to->data);
    double *fromData = *(from->data);
    printf("FROM (pre-set) 0, 0: %f\n", fromData[0]);
    printf("FROM (pre-set) 0, 1: %f\n", fromData[1]);
    printf("FROM (pre-set) 1, 0: %f\n", fromData[2]);
    printf("FROM (pre-set) 1, 1: %f\n", fromData[3]);
    printf("TO (pre-set) 0, 0: %f\n", toData[0]);
    printf("TO (pre-set) 0, 1: %f\n", toData[1]);
    set(to, 0, 1, 5); // FROM: [[1, 0], [5, 2]], TO: [[5, 2]]
    //set(from, 0, 1, 3); // FROM: [[1, 3], [5, 2]], TO: [[5, 2]]
    printf("FROM (post-set) 0, 0: %f\n", fromData[0]);
    printf("FROM (post-set) 0, 1: %f\n", fromData[1]);
    printf("FROM (post-set) 1, 0: %f\n", fromData[2]);
    printf("FROM (post-set) 1, 1: %f\n", fromData[3]);
    printf("TO (post-set) 0, 0: %f\n", toData[0]);
    printf("TO (post-set) 0, 1: %f\n", toData[1]);
    

}

/* Test the null case doesn't crash */
void dealloc_null_test(void) {
    matrix *mat = NULL;
    deallocate_matrix(mat);
}

void dealloc_test(void) {
    matrix *mat0 = NULL;
    matrix *mat1 = NULL;
    matrix *mat2 = NULL;
    matrix *mat3 = NULL;
    allocate_matrix(&mat0, 4, 4);
    allocate_matrix_ref(&mat1, mat0, 0, 0, 3, 3);
    allocate_matrix_ref(&mat2, mat1, 0, 0, 2, 2);
    allocate_matrix_ref(&mat3, mat0, 0, 1, 2, 2);
    deallocate_matrix(mat0);
    deallocate_matrix(mat1);
    deallocate_matrix(mat2);
    deallocate_matrix(mat3);

}

void get_test(void) {
    matrix *mat = NULL;
    allocate_matrix(&mat, 2, 2);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            set(mat, i, j, i * 2 + j);
        }
    }
    CU_ASSERT_EQUAL(get(mat, 0, 0), 0);
    CU_ASSERT_EQUAL(get(mat, 0, 1), 1);
    CU_ASSERT_EQUAL(get(mat, 1, 0), 2);
    CU_ASSERT_EQUAL(get(mat, 1, 1), 3);
    deallocate_matrix(mat);
}

void set_test(void) {
    matrix *mat = NULL;
    allocate_matrix(&mat, 2, 2);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            set(mat, i, j, i * 2 + j);
        }
    }
    CU_ASSERT_EQUAL(get(mat, 0, 0), 0);
    CU_ASSERT_EQUAL(get(mat, 0, 1), 1);
    CU_ASSERT_EQUAL(get(mat, 1, 0), 2);
    CU_ASSERT_EQUAL(get(mat, 1, 1), 3);
    deallocate_matrix(mat);
}

/* Custom function: test mul individually */
void mulTester() {
    matrix *mat1 = NULL;
    matrix *r1 = NULL;
    allocate_matrix(&mat1, 2, 2);
    allocate_matrix(&r1, 2, 2);
    set(mat1, 0, 0, 1);
    set(mat1, 0, 1, 1);
    set(mat1, 1, 0, 1);
    set(mat1, 1, 1, 0);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            //printf("%i, %i: %f\n", i, j, (r1->data)[i][j]);
        }
    }
    mul_matrix(r1, mat1, mat1);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            //printf("%i, %i: %f\n", i, j, (r1->data)[i][j]);
        }
    }
    mul_matrix(r1, r1, mat1);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            printf("%i, %i: %f\n", i, j, (r1->data)[i][j]);
        }
    }
}

void mulTester2() {
    matrix *result = NULL;
    matrix *mat1 = NULL;
    matrix *mat2 = NULL;
    allocate_matrix(&mat1, 3, 3);
    allocate_matrix(&mat2, 3, 3);
    allocate_matrix(&result, 3, 3);
    // CU_ASSERT_EQUAL(allocate_matrix(&result, 3, 3), 0);
    // CU_ASSERT_EQUAL(allocate_matrix(&mat1, 3, 3), 0);
    // CU_ASSERT_EQUAL(allocate_matrix(&mat2, 3, 3), 0);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            set(mat1, i, j, i * 3 + j + 1);
            set(mat2, i, j, i * 3 + j + 1);
        }
    }
    mul_matrix(result, mat1, mat2);
    printf("0, 0: %f", get(result, 0, 0));
    printf("0, 1: %f", get(result, 0, 1));
    printf("0, 2: %f", get(result, 0, 2));
    printf("1, 0: %f", get(result, 1, 0));
    printf("1, 1: %f", get(result, 1, 1));
    printf("1, 2: %f", get(result, 1, 2));
    printf("2, 0: %f", get(result, 2, 0));
    printf("2, 1: %f", get(result, 2, 1));
    printf("2, 2: %f", get(result, 2, 2));
    // CU_ASSERT_EQUAL(get(result, 0, 0), 30);
    // CU_ASSERT_EQUAL(get(result, 0, 1), 36);
    // CU_ASSERT_EQUAL(get(result, 0, 2), 42);
    // CU_ASSERT_EQUAL(get(result, 1, 0), 66);
    // CU_ASSERT_EQUAL(get(result, 1, 1), 81);
    // CU_ASSERT_EQUAL(get(result, 1, 2), 96);
    // CU_ASSERT_EQUAL(get(result, 2, 0), 102);
    // CU_ASSERT_EQUAL(get(result, 2, 1), 126);
    // CU_ASSERT_EQUAL(get(result, 2, 2), 150);
    deallocate_matrix(result);
    deallocate_matrix(mat1);
    deallocate_matrix(mat2);
}

/* Custom Function: Individual Tester */
void powSimple() {
    matrix *result = NULL;
    matrix *mat = NULL;
    printf("Working here? 1\n");
    allocate_matrix(&result, 2, 2);
    allocate_matrix(&mat, 2, 2);
    printf("Working here? 2\n");
    set(mat, 0, 0, 1);
    set(mat, 0, 1, 1);
    set(mat, 1, 0, 1);
    set(mat, 1, 1, 0);
    printf("Working here? 3\n");
    pow_matrix(result, mat, 3);
    // mul_matrix_self(mat, mat, mat);
    // printf("%i, %i: %f\n", 0, 0, get(mat, 0, 0));
    // printf("%i, %i: %f\n", 0, 1, get(mat, 0, 1));
    // printf("%i, %i: %f\n", 1, 0, get(mat, 1, 0));
    // printf("%i, %i: %f\n", 1, 1, get(mat, 1, 1));


    printf("%i, %i: %f\n", 0, 0, get(result, 0, 0));
    printf("%i, %i: %f\n", 0, 1, get(result, 0, 1));
    printf("%i, %i: %f\n", 1, 0, get(result, 1, 0));
    printf("%i, %i: %f\n", 1, 1, get(result, 1, 1));
    // CU_ASSERT_EQUAL(get(result, 0, 0), 3);
    // CU_ASSERT_EQUAL(get(result, 0, 1), 2);
    // CU_ASSERT_EQUAL(get(result, 1, 0), 2);
    // CU_ASSERT_EQUAL(get(result, 1, 1), 1);
}

void allocRefTest() {
    //printf("Working1\n");
    matrix *mat = NULL;
    matrix *from = NULL;
    allocate_matrix(&from, 3, 2);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            set(from, i, j, i * 2 + j);
            //printf("%i, %i: %i\n", i, j, i * 2 + j);
        }
    }
    //printf("Working 2\n");
    /* 2D slice */
    //CU_ASSERT_EQUAL(allocate_matrix_ref(&mat, from, 1, 0, 2, 2), 0);
    int x = allocate_matrix_ref(&mat, from, 1, 0, 2, 2);
    //printf("x val: %i\n", x);
    //printf("rows: %i\n", mat->rows);
    //printf("cols: %i\n", mat->cols);
    //printf("parent ref cnt: %i\n", mat->parent->ref_cnt);
    //CU_ASSERT_PTR_EQUAL(mat->parent, from);
    // CU_ASSERT_EQUAL(mat->parent->ref_cnt, 2);
    // CU_ASSERT_EQUAL(mat->rows, 2);
    // CU_ASSERT_EQUAL(mat->cols, 2);
    //printf("Working3\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            //CU_ASSERT_EQUAL(get(mat, i, j), get(from, i + 1, j));
            printf("FROM: %i, %i: %f\n", i, j, get(from, i+1, j));
            printf("TO: %i, %i: %f\n", i, j, get(mat, i, j));
            //printf("i, j, matVal, fromVal: %i, %i, %f, %f\n", i, j, get(mat, i, j), get(from, i+1, j));
        }
    }
    //printf("Working4\n");
    /* 1D slice */

    //CU_ASSERT_EQUAL(allocate_matrix_ref(&mat, from, 1, 0, 2, 1), 0);
    int newAlloc = allocate_matrix_ref(&mat, from, 1, 0, 2, 1);
    printf("New alloc: %i\n", newAlloc);
    printf("rows: %i\n", mat->rows);
    printf("cols: %i\n", mat->cols);
    printf("parent ref cnt: %i\n", mat->parent->ref_cnt);
    printf("1d? %i\n", mat->is_1d);
    // CU_ASSERT_PTR_EQUAL(mat->parent, from);
    // CU_ASSERT_EQUAL(mat->parent->ref_cnt, 3);
    // CU_ASSERT_EQUAL(mat->rows, 2);
    // CU_ASSERT_EQUAL(mat->cols, 1);
    // CU_ASSERT_NOT_EQUAL(mat->is_1d, 0);
    //printf("Working5\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 1; j++) {
            //CU_ASSERT_EQUAL(get(mat, i, j), get(from, i + 1, j));
            printf("i, j, matVal, fromVal: %i, %i, %f, %f\n", i, j, get(mat, i, j), get(from, i+1, j));
        }
    }
    //printf("Working6\n");
    /* Now we compare the data in the reference matrix */
    deallocate_matrix(from);
    deallocate_matrix(mat);
}

/************* Test Runner Code goes here **************/

int main(void) {
    //mulTester2();
    //powSimple();
    //allocRefTest();
    // allocate_ref1();
    // printf("\n\n\n");
    // allocate_ref2();
    // printf("\n\n\n");
    // allocate_ref3();
    Py_Initialize();  // Need to call this so that Python.h functions won't
    // segfault
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if (CU_initialize_registry() != CUE_SUCCESS) return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite("mat_test_suite", init_suite, clean_suite);
    if (pSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    /* add the tests to the suite */
    if ((CU_add_test(pSuite, "add_test", add_test) == NULL) ||
        (CU_add_test(pSuite, "sub_test", sub_test) == NULL) ||
        (CU_add_test(pSuite, "mul_test", mul_test) == NULL) ||
        (CU_add_test(pSuite, "neg_test", neg_test) == NULL) ||
        (CU_add_test(pSuite, "abs_test", abs_test) == NULL) ||
        (CU_add_test(pSuite, "pow_test", pow_test) == NULL) ||
        (CU_add_test(pSuite, "alloc_fail_test", alloc_fail_test) == NULL) ||
        (CU_add_test(pSuite, "alloc_success_test", alloc_success_test) == NULL) ||
        (CU_add_test(pSuite, "dealloc_null_test", dealloc_null_test) == NULL) ||
        (CU_add_test(pSuite, "get_test", get_test) == NULL) ||
        (CU_add_test(pSuite, "set_test", set_test) == NULL) ||
        (CU_add_test(pSuite, "alloc_ref_test", alloc_ref_test) == NULL))
    {
        // (CU_add_test(pSuite, "mulTester", mulTester) == NULL) ||
        // (CU_add_test(pSuite, "powSimple", powSimple) == NULL) ||
        // (CU_add_test(pSuite, "allocRefTest", allocRefTest) == NULL)) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    // Run all tests using the basic interface
    //CU_basic_set_mode(CU_BRM_NORMAL);
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    printf("\n");
    CU_basic_show_failures(CU_get_failure_list());
    printf("\n\n");

    /* Clean up registry and return */
    CU_cleanup_registry();
    return CU_get_error();
}