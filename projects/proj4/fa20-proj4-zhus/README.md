# numc - 61C Project 4

Here's what we did in project 4:
Zachary - Matrix.c focused task 1 and 2. Lots of testing
Peter - Numc.c focused task 3.
Task 4 - together
Task 5 - together
Good work from both of us 10/10 teamwork!


Task 1

For the initial implementation of matrix.c, we worked on a naive version of the different matrix operations. This included element-wise addition, subtraction, absolute value, and negation, in addition to functionality for multiplying matrices and raising matrices to a power. We were not concerned about speedup, so we simply wanted to ensure that the basic arithmetic operations would be working, alongside the other matrix manipulations, such as the initial allocation, deallocation, and slicing of a matrix, and set/get operations that would change individual values within the matrix. The power operation was slightly tricky, as we had to account for the fact that we didn't want to clobber over the values of the original matrix, so we used another slightly modified version of multiplying matrices in which a temporary matrix would be allocated for the repeated multiplication process.

Task 2

This task was perhaps the most straightforward: this task enabled us to set up the tertiary task, which would actually allow us to test our C matrix operations on a Python user interface. We just created the module and set up as specified by the reference. In addition, we took advantage of the compile and argument flags that were provided to us. The distutils.core Extension and Setup functions proved especially useful for this task.

Task 3

As the name of this task suggests, we wanted to actually be able to translate Python commands into the C methods that were created in the first task. This included the initial creation of a matrix, the elemental operations, multiplication, power, and all other operations. In addition, this section was responsible for the bulk of the error checking, to make sure that the parameters given could be accommodated. For instance, we wanted to make sure that the elemental matrix operations (add/sub/neg/abs) had proper dimensions, that we had enough memory for allocation, etc.

For adding, subtracting, multiplying, negating, taking the absolute value, and raising matrices to a power, the workflow was relatively similar. We had to check to make sure that we had valid arguments (valid matrices with valid dimensions), allocating a matrix to store the result of the called function using allocate_matrix function from matrix.c, then called the corresponding operations which were also located in matrix.c. If at any point an error is encountered, then the operation would ultimately failed to complete.

The getting and setting of individual values of the matrix operated pretty similarly to the above operations, only with slightly different error checks. Once again, the methods that were created in matrix.c could be called.

Lastly, perhaps the most involved part of this task were the subscript and set_subscript methods, which were used to obtain a slice of the matrix (whether that be a sub-matrix, an individual row or column, or a portion of the row or column). In addition to adhering to the error checks--making sure we had valid matrices, the dimensions to be obtained were also valid, etc.--we also wanted to make sure that all the standard Pythonic list indexing was supporting. For instance, given a matrix mat, mat[:, :] should return the original matrix, mat[1:3, 2:4] returns a specific subset of the rows/columns as specified, mat[:, 1] would return a specific column, mat[0][1] would return an individual specific value. Here, we took advantage of the allocate_matrix_ref function because we wanted to be able to modify the parent matrix as well when the child that was created by through the slicing of the original matrix was modified.

Task 4

In this part, we used SIMD, loop unrolling, and parallelization/multithreading in order to speed up the basic element-wise operations: addition, subtraction, absolute value, and negation. However, when this proved insufficient to achieve the basic "simple test" speedup benchmark, we reworked the data component of the matrix struct so that it was functionally a contiguous array instead of a sequence of pointers to a row of doubles. As a result, we were able to treat the entirety of the matrix as essentially contiguous in memory, leading to a diminished number of tail cases that needed to be accommodated in each operation. The contiguous nature of the new double** data portion of the new matrix struct required modifications to the original set value so that we were able to change the parent matrix. We included the row/col offset so that we knew where the original parent matrix had to be modified when we wanted to change a value in the child matrix.

To speed up multiply, it was necessary to make numerous changes. In addition to the SIMD and parallelization that was implemented with the previous operations, we utilized transpose for more efficient memory access patterns to achieve the desired speedup.

Meanwhile, to speed up pow, instead of multiplying a matrix by a matrix in sequence (mat1 * mat1 * mat1...) we took advantage of squared exponentiation to dramatically speed up the power operation, as now this reduced the number of matrix multiplications that were necessary to make. Now, O(logN) calls to multiplication were made instead of O(N), which had a profound impact on runtime for larger values that we raised the matrix to, regardless of the original size of that matrix. Coupled with the speedup in matrix, we were able to speed up pow quite a bit.

Findings/Example Uses

The simplest uses of this project would be to test different "matrix" operations on Python. For instance, creating a multi-dimensional list on Python and then indexing, adding/subtracting matrices, slicing. Given the following matrix, a = nc.Matrix(3, 3), we could use a[0] to get the first row, a[0:2] to get a slice of the first 2 rows, a[0, 0] to get a single value at a single point, a[0][1] = 5 to set a value at a specific point. If another matrix b had the same dimensions, a + b would yield the element-wise sum of the two matrices.

One somewhat surprising finding was that the speedups that we tested were not necessarily independent. For instance, while in isolation, we might achieve 1.4x speedup from SIMD and a 1.4x from loop unrolling, combined, there was not always ~1.96x speedup from both combined, which we would've expected had the speedups been independent of one another. Moreover, there was a large variation in the degree to which each function was "sped up," with pow being the primary culprit for a lack of a consistent output. However, by using our custom tests which were more robust, we were able to gain a more objective understanding of the degree to which we had succeeded in increasing the performance of each individual function compared to the naive implementation.

How to use:
After logging on to the hive machine, we clone this repo, create a virtual environment, install requirements, and call the Makefile.
```
git clone https://github.com/61c-student/fa20-proj4-zhus.git
python3.6 -m venv .venv
source .venv/bin/activate
pip3 install -r requirements.txt
make
```
This is how we call numc
```
python3
>> import numc as mc
>> a = nc.Matrix(3,3)
>> b = a[1,0:2]
>> etc..
```
We can compare this to dumbpy (staff slow version) and numpy (python package)
