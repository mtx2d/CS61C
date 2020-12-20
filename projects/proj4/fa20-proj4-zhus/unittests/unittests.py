from utils import *
from unittest import TestCase

"""
For each operation, you should write tests to test  on matrices of different sizes.
Hint: use dp_mc_matrix to generate dumbpy and numc matrices with the same data and use
      cmp_dp_nc_matrix to compare the results
"""
class TestAdd(TestCase):
    def test_small_add(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(2, 2, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(2, 2, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "add")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium_add(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(200, 345, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(200, 345, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "add")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large1_add(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(1234, 650, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(1234, 650, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "add")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large2_add(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(2000, 2500, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(2000, 2500, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "add")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_huge1_add(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(7456, 5678, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(7456, 5678, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "add")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_huge2_add(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(11000, 10500, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(11000, 10500, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "add")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_exceptions_add(self):
        try:
            nc.Matrix(3, 3) + nc.Matrix(2, 2)
            self.assertTrue(False)
        except ValueError as e:
            print(e)
            pass


class TestSub(TestCase):
    def test_small_sub(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(2, 2, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(2, 2, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "sub")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium_sub(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(324, 234, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(324, 234, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "sub")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large_sub(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(2659, 1678, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(2659, 1678, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "sub")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_huge1_sub(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(7234, 9876, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(7234, 9876, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "sub")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_huge2_sub(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(13456, 10201, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(13456, 10201, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "sub")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

class TestAbs(TestCase):
    def test_small_abs(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(2, 2, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "abs")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium_abs(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(1234, 2345, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "abs")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large_abs(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(6789, 2895, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "abs")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_huge1_abs(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(8579, 7468, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "abs")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_huge2_abs(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(11456, 9800, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "abs")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

class TestNeg(TestCase):
    def test_small_neg(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(2, 2, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "neg")
        self.assertTrue(is_correct)
        print_speedup(speed_up)
    def test_medium_neg(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(345, 468, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "neg")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large_neg(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(1890, 1789, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "neg")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_huge1_neg(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(3456, 2345, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "neg")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_huge2_neg(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(12056, 9990, seed=0)
        is_correct, speed_up = compute([dp_mat], [nc_mat], "neg")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

class TestMul(TestCase):
    def test_small_mul(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(2, 2, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(2, 2, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "mul")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium_mul(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(123, 345, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(345, 234, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "mul")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large_mul(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(1234, 1357, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(1357, 1230, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "mul")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_huge1_mul(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(1456, 1678, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(1678, 2678, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "mul")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_huge2_mul(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(2145, 2567, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(2567, 2678, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "mul")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_huge3_mul(self):
        # TODO: YOUR CODE HERE
        # SLOW
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(4321, 4567, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(4567, 5678, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "mul")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_huge4_mul(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(5678, 67, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(67, 6789, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "mul")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_huge5_mul(self):
        # TODO: YOUR CODE HERE
        dp_mat1, nc_mat1 = rand_dp_nc_matrix(12234, 1, seed=0)
        dp_mat2, nc_mat2 = rand_dp_nc_matrix(1, 9876, seed=1)
        is_correct, speed_up = compute([dp_mat1, dp_mat2], [nc_mat1, nc_mat2], "mul")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

class TestPow(TestCase):
    def test_small_pow1(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(2, 2, seed=0)
        is_correct, speed_up = compute([dp_mat, 2], [nc_mat, 2], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_small_pow2(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(3, 3, seed=0)
        is_correct, speed_up = compute([dp_mat, 4], [nc_mat, 4], "pow")
        if is_correct == False:
            #print("Testing")
            print(dp_mat)
            print(nc_mat)
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_small_pow3(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(12, 12, seed=0)
        is_correct, speed_up = compute([dp_mat, 10], [nc_mat, 10], "pow")
        # if is_correct == False:
        #     print(dp_mat)
        #     print(nc_mat)

    def test_medium_pow1(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(25, 25, seed=0)
        is_correct, speed_up = compute([dp_mat, 2], [nc_mat, 2], "pow")
        # if is_correct == False:
        #     for i in range(25):
        #         print("Dumbpy ", i, dp_mat[i])
        #         print("Our implementation", i, nc_mat[i])
            #print(dp_mat)
            #print(nc_mat)
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium_pow2(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(20, 20, seed=0)
        is_correct, speed_up = compute([dp_mat, 4], [nc_mat, 4], "pow")
        # if is_correct == False:
        #     print(dp_mat)
        #     print(nc_mat)
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium_pow3(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(50, 50, seed=0)
        is_correct, speed_up = compute([dp_mat, 4], [nc_mat, 4], "pow")
        # if is_correct == False:
        #     print(dp_mat)
        #     print(nc_mat)
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium_pow2_1(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(20, 20, seed=0)
        is_correct, speed_up = compute([dp_mat, 50], [nc_mat, 50], "pow")
        # if is_correct == False:
        #     print(dp_mat)
        #     print(nc_mat)
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium_pow3_1(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(50, 50, seed=0)
        is_correct, speed_up = compute([dp_mat, 100], [nc_mat, 100], "pow")
        # if is_correct == False:
        #     print(dp_mat)
        #     print(nc_mat)
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_medium_pow4(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(100, 100, seed=0)
        is_correct, speed_up = compute([dp_mat, 4], [nc_mat, 4], "pow")
        # if is_correct == False:
        #     print(dp_mat)
        #     print(nc_mat)
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow1_40(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(150, 150, seed=0)
        is_correct, speed_up = compute([dp_mat, 40], [nc_mat, 40], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow1_02(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(150, 150, seed=0)
        is_correct, speed_up = compute([dp_mat, 2], [nc_mat, 2], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow1_25(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(150, 150, seed=0)
        is_correct, speed_up = compute([dp_mat, 25], [nc_mat, 25], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow1_30(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(150, 150, seed=0)
        is_correct, speed_up = compute([dp_mat, 30], [nc_mat, 30], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow1_31(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(150, 150, seed=0)
        is_correct, speed_up = compute([dp_mat, 31], [nc_mat, 31], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow1_32(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(150, 150, seed=0)
        is_correct, speed_up = compute([dp_mat, 32], [nc_mat, 30], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow1_33(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(150, 150, seed=0)
        is_correct, speed_up = compute([dp_mat, 33], [nc_mat, 33], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow2(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(180, 180, seed=0)
        is_correct, speed_up = compute([dp_mat, 15], [nc_mat, 15], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow2_1(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(184, 184, seed=0)
        is_correct, speed_up = compute([dp_mat, 2], [nc_mat, 2], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow2_2(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(185, 185, seed=0)
        is_correct, speed_up = compute([dp_mat, 2], [nc_mat, 2], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow2_3(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(211, 211, seed=0)
        is_correct, speed_up = compute([dp_mat, 6], [nc_mat, 6], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)
    
    def test_marge_pow3_0(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(200, 200, seed=0)
        is_correct, speed_up = compute([dp_mat, 71], [nc_mat, 71], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow3_1(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(201, 201, seed=0)
        is_correct, speed_up = compute([dp_mat, 71], [nc_mat, 71], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow3_2(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(202, 202, seed=0)
        is_correct, speed_up = compute([dp_mat, 71], [nc_mat, 71], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow3_3(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(203, 203, seed=0)
        is_correct, speed_up = compute([dp_mat, 71], [nc_mat, 71], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow3_4(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(204, 204, seed=0)
        is_correct, speed_up = compute([dp_mat, 71], [nc_mat, 71], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow3_5(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(205, 205, seed=0)
        is_correct, speed_up = compute([dp_mat, 71], [nc_mat, 71], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow3_6(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(206, 206, seed=0)
        is_correct, speed_up = compute([dp_mat, 71], [nc_mat, 71], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow3_7(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(207, 207, seed=0)
        is_correct, speed_up = compute([dp_mat, 71], [nc_mat, 71], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow3_8(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(208, 208, seed=0)
        is_correct, speed_up = compute([dp_mat, 71], [nc_mat, 71], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow3_9(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(209, 209, seed=0)
        is_correct, speed_up = compute([dp_mat, 5], [nc_mat, 5], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow3_10(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(210, 210, seed=0)
        is_correct, speed_up = compute([dp_mat, 71], [nc_mat, 71], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow3_11_71(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(211, 211, seed=0)
        is_correct, speed_up = compute([dp_mat, 71], [nc_mat, 71], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow3_11_70(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(211, 211, seed=0)
        is_correct, speed_up = compute([dp_mat, 70], [nc_mat, 70], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow3_11_72(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(211, 211, seed=0)
        is_correct, speed_up = compute([dp_mat, 72], [nc_mat, 72], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow3_11_69(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(211, 211, seed=0)
        is_correct, speed_up = compute([dp_mat, 69], [nc_mat, 69], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow3_11_5(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(211, 211, seed=0)
        is_correct, speed_up = compute([dp_mat, 5], [nc_mat, 5], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow3_11_6(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(211, 211, seed=0)
        is_correct, speed_up = compute([dp_mat, 6], [nc_mat, 6], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow3_11_4(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(211, 211, seed=0)
        is_correct, speed_up = compute([dp_mat, 4], [nc_mat, 4], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow3_11_7(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(211, 211, seed=0)
        is_correct, speed_up = compute([dp_mat, 7], [nc_mat, 7], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow3_11_16(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(211, 211, seed=0)
        is_correct, speed_up = compute([dp_mat, 16], [nc_mat, 16], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow3_12(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(212, 212, seed=0)
        is_correct, speed_up = compute([dp_mat, 5], [nc_mat, 5], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow3_13(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(213, 213, seed=0)
        is_correct, speed_up = compute([dp_mat, 71], [nc_mat, 71], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow3_14(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(214, 214, seed=0)
        is_correct, speed_up = compute([dp_mat, 71], [nc_mat, 71], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow3_15(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(215, 215, seed=0)
        is_correct, speed_up = compute([dp_mat, 71], [nc_mat, 71], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow3_16(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(216, 216, seed=0)
        is_correct, speed_up = compute([dp_mat, 71], [nc_mat, 71], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_marge_pow3_17(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(222, 222, seed=0)
        is_correct, speed_up = compute([dp_mat, 71], [nc_mat, 71], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large_pow1(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(234, 234, seed=0)
        is_correct, speed_up = compute([dp_mat, 56], [nc_mat, 56], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)


    def test_large_pow2(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(279, 279, seed=0)
        is_correct, speed_up = compute([dp_mat, 17], [nc_mat, 17], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large_pow3(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(301, 301, seed=0)
        is_correct, speed_up = compute([dp_mat, 5], [nc_mat, 5], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_large_pow4(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(400, 400, seed=0)
        is_correct, speed_up = compute([dp_mat, 105], [nc_mat, 105], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    # def test_large_pow5(self):
    #     # TODO: YOUR CODE HERE
    #     dp_mat, nc_mat = rand_dp_nc_matrix(400, 400, seed=0)
    #     is_correct, speed_up = compute([dp_mat, 1010], [nc_mat, 1010], "pow")
    #     self.assertTrue(is_correct)
    #     print_speedup(speed_up)

        
    '''
    def test_huge1_pow(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(345, 345, seed=0)
        is_correct, speed_up = compute([dp_mat, 99], [nc_mat, 99], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_huge2_pow(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(403, 403, seed=0)
        is_correct, speed_up = compute([dp_mat, 80], [nc_mat, 80], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_huge3_pow(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(511, 511, seed=0)
        is_correct, speed_up = compute([dp_mat, 106], [nc_mat, 106], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)

    def test_huge4_pow(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(750, 750, seed=0)
        is_correct, speed_up = compute([dp_mat, 10], [nc_mat, 10], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)
    
    def test_huge5_pow(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(1112, 1112, seed=0)
        is_correct, speed_up = compute([dp_mat, 43], [nc_mat, 43], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)
    '''
    

    
    
    
    def test_zero_pow(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(1245, 1245, seed=0)
        is_correct, speed_up = compute([dp_mat, 0], [nc_mat, 0], "pow")
        self.assertTrue(is_correct)
        print_speedup(speed_up)
    
    

class TestGet(TestCase):
    def test_get(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(2, 2, seed=0)
        rand_row = np.random.randint(dp_mat.shape[0])
        rand_col = np.random.randint(dp_mat.shape[1])
        self.assertEqual(round(dp_mat[rand_row][rand_col], decimal_places),
            round(nc_mat[rand_row][rand_col], decimal_places))

class TestSet(TestCase):
    def test_set(self):
        # TODO: YOUR CODE HERE
        dp_mat, nc_mat = rand_dp_nc_matrix(2, 2, seed=0)
        rand_row = np.random.randint(dp_mat.shape[0])
        rand_col = np.random.randint(dp_mat.shape[1])
        self.assertEqual(round(dp_mat[rand_row][rand_col], decimal_places),
            round(nc_mat[rand_row][rand_col], decimal_places))

class TestShape(TestCase):
    def test_shape(self):
        dp_mat, nc_mat = rand_dp_nc_matrix(2, 2, seed=0)
        self.assertTrue(dp_mat.shape == nc_mat.shape)