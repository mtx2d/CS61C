from unittest import TestCase
from framework import AssemblyTest, print_coverage


class TestAbs(TestCase):
    def test_zero(self):
        t = AssemblyTest(self, "abs.s")
        # load 0 into register a0
        t.input_scalar("a0", 0)
        # call the abs function
        t.call("abs")
        # check that after calling abs, a0 is equal to 0 (abs(0) = 0)
        t.check_scalar("a0", 0)
        # generate the `assembly/TestAbs_test_zero.s` file and run it through venus
        t.execute()

    def test_one(self):
        # same as test_zero, but with input 1
        t = AssemblyTest(self, "abs.s")
        t.input_scalar("a0", 1)
        t.call("abs")
        t.check_scalar("a0", 1)
        t.execute()

    def test_minus_one(self):
        t = AssemblyTest(self, "abs.s")
        t.input_scalar("a0", -1)
        t.call("abs")
        t.check_scalar("a0", 1)
        t.execute()
        
	

    @classmethod
    def tearDownClass(cls):
        print_coverage("abs.s", verbose=False)


class TestRelu(TestCase):
    def test_simple(self):
        t = AssemblyTest(self, "relu.s")
        # create an array in the data section
        array0 = t.array([1, -2, 3, -4, 5, -6, 7, -8, 9])
        # load address of `array0` into register a0
        t.input_array("a0", array0)
        # set a1 to the length of our array
        t.input_scalar("a1", len(array0))
        # call the relu function
        t.call("relu")
        # check that the array0 was changed appropriately
        t.check_array(array0, [1, 0, 3, 0, 5, 0, 7, 0, 9])
        # generate the `assembly/TestRelu_test_simple.s` file and run it through venus
        t.execute()
    def test1(self):
        t = AssemblyTest(self, "relu.s")
        array0 = t.array([1])
        t.input_array("a0", array0)
        t.input_scalar("a1", len(array0))
        t.call("relu")
        t.check_array(array0, [1])
        t.execute(code = 0)
    def test2(self):
        t = AssemblyTest(self, "relu.s")
        array0 = t.array([-4])
        t.input_array("a0", array0)
        t.input_scalar("a1", len(array0))
        t.call("relu")
        t.check_array(array0, [0])
        t.execute(code = 0)
    def test3(self):
        t = AssemblyTest(self, "relu.s")
        array0 = t.array([1, -1])
        t.input_array("a0", array0)
        t.input_scalar("a1", len(array0))
        t.call("relu")
        t.check_array(array0, [1, 0])
        t.execute(code = 0)
    def test4(self):
        t = AssemblyTest(self, "relu.s")
        array0 = t.array([-1, -1])
        t.input_array("a0", array0)
        t.input_scalar("a1", len(array0))
        t.call("relu")
        t.check_array(array0, [0, 0])
        t.execute(code = 0)
    def test5(self):
        t = AssemblyTest(self, "relu.s")
        array0 = t.array([20, 20])
        t.input_array("a0", array0)
        t.input_scalar("a1", len(array0))
        t.call("relu")
        t.check_array(array0, [20, 20])
        t.execute(code = 0)
    def test6(self):
        t = AssemblyTest(self, "relu.s")
        array0 = t.array([])
        t.input_array("a0", array0)
        t.input_scalar("a1", len(array0))
        t.call("relu")
        t.execute(code = 78)
        #t.check_array(array0, [])
    
    @classmethod
    def tearDownClass(cls):
        print_coverage("relu.s", verbose=False)


class TestArgmax(TestCase):
    # create an array in the data section
        # TODO
        # load address of the array into register a0
        # TODO
        # set a1 to the length of the array
        # TODO
        # call the `argmax` function
        # TODO
        # check that the register a0 contains the correct output
        # TODO
        # generate the `assembly/TestArgmax_test_simple.s` file and run it through venus

    def test_simple(self):
        t = AssemblyTest(self, "argmax.s")
        array0 = t.array([1, -2, 3, -4, 5, -6, 7, -8, 9])
        # load address of `array0` into register a0
        t.input_array("a0", array0)
        # set a1 to the length of our array
        t.input_scalar("a1", len(array0))
        # call the argmax function
        t.call("argmax")
        t.check_scalar("a0", 8)
        t.execute()
    def test1(self):
        t = AssemblyTest(self, "argmax.s")
        array0 = t.array([1])
        # load address of `array0` into register a0
        t.input_array("a0", array0)
        # set a1 to the length of our array
        t.input_scalar("a1", len(array0))
        # call the argmax function
        t.call("argmax")
        t.check_scalar("a0", 0)
        t.execute()
    def test2(self):
        t = AssemblyTest(self, "argmax.s")
        array0 = t.array([5, 5])
        # load address of `array0` into register a0
        t.input_array("a0", array0)
        # set a1 to the length of our array
        t.input_scalar("a1", len(array0))
        # call the argmax function
        t.call("argmax")
        t.check_scalar("a0", 0)
        t.execute()
    def test3(self):
        t = AssemblyTest(self, "argmax.s")
        array0 = t.array([9, 10])
        # load address of `array0` into register a0
        t.input_array("a0", array0)
        # set a1 to the length of our array
        t.input_scalar("a1", len(array0))
        # call the argmax function
        t.call("argmax")
        t.check_scalar("a0", 1)
        t.execute()
    def test4(self):
        t = AssemblyTest(self, "argmax.s")
        array0 = t.array([])
        # load address of `array0` into register a0
        t.input_array("a0", array0)
        # set a1 to the length of our array
        t.input_scalar("a1", len(array0))
        # call the argmax function
        t.call("argmax")
        t.execute(code = 77)


    @classmethod
    def tearDownClass(cls):
        print_coverage("argmax.s", verbose=False)


class TestDot(TestCase):
    def test1(self):
        t = AssemblyTest(self, "dot.s")
        # create arrays in the data section raise NotImplementedError("TODO")
        array0 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        array1 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        t.input_array("a0", array0) # a0: Load array address for array0
        t.input_array("a1", array1) # a1: Load array address for array1
        t.input_scalar("a2", len(array0)) # a2: Load vector length
        t.input_scalar("a3", 1) # a3: Set the stride length for array0
        t.input_scalar("a4", 1) # a4: Set the stride length for array1
        t.call("dot") # call the `dot` function
        t.check_scalar("a0", 285) # check the return value
        # TODO
        t.execute()
    def test2(self):
        t = AssemblyTest(self, "dot.s")
        # create arrays in the data section raise NotImplementedError("TODO")
        array0 = t.array([1, 2, 3])
        array1 = t.array([1, 2, 3])
        t.input_array("a0", array0) # a0: Load array address for array0
        t.input_array("a1", array1) # a1: Load array address for array1
        t.input_scalar("a2", len(array0)) # a2: Load vector length
        t.input_scalar("a3", 1) # a3: Set the stride length for array0
        t.input_scalar("a4", 1) # a4: Set the stride length for array1
        t.call("dot")
        t.check_scalar("a0", 14) # Check the return value
        # TODO
        t.execute()
    def test3(self):
        t = AssemblyTest(self, "dot.s")
        # create arrays in the data section raise NotImplementedError("TODO")
        array0 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        array1 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        t.input_array("a0", array0) # a0: Load array address for array0
        t.input_array("a1", array1) # a1: Load array address for array1
        t.input_scalar("a2", 3) # a2: Load vector length
        t.input_scalar("a3", 1) # a3: Set the stride length for array0
        t.input_scalar("a4", 2) # a4: Set the stride length for array1
        t.call("dot") # call the `dot` function
        t.check_scalar("a0", 22) # check the return value
        # TODO
        t.execute()
    def test4(self):
        t = AssemblyTest(self, "dot.s")
        # create arrays in the data section raise NotImplementedError("TODO")
        array0 = t.array([])
        array1 = t.array([])
        t.input_array("a0", array0) # a0: Load array address for array0
        t.input_array("a1", array1) # a1: Load array address for array1
        t.input_scalar("a2", len(array0)) # a2: Load vector length
        t.input_scalar("a3", 1) # a3: Set the stride length for array0
        t.input_scalar("a4", 1) # a4: Set the stride length for array1
        t.call("dot") # call the `dot` function
        t.check_scalar("a0", 285) # check the return value
        # TODO
        t.execute(code = 75)
    def test5(self):
        t = AssemblyTest(self, "dot.s")
        # create arrays in the data section raise NotImplementedError("TODO")
        array0 = t.array([2, 7, 1])
        array1 = t.array([2, 3, 5])
        t.input_array("a0", array0) # a0: Load array address for array0
        t.input_array("a1", array1) # a1: Load array address for array1
        t.input_scalar("a2", len(array0)) # a2: Load vector length
        t.input_scalar("a3", 0) # a3: Set the stride length for array0
        t.input_scalar("a4", 0) # a4: Set the stride length for array1
        t.call("dot") # call the `dot` function
        t.check_scalar("a0", 285) # check the return value
        # TODO
        t.execute(code = 76)
    @classmethod
    def tearDownClass(cls):
        print_coverage("dot.s", verbose=False)


class TestMatmul(TestCase):

    #   a0 (int*)  is the pointer to the start of m0 
    #   a1 (int)   is the # of rows (height) of m0
    #   a2 (int)   is the # of columns (width) of m0
    #   a3 (int*)  is the pointer to the start of m1
    #   a4 (int)   is the # of rows (height) of m1
    #   a5 (int)   is the # of columns (width) of m1
    #   a6 (int*)  is the pointer to the the start of d

    def do_matmul(self, m0, m0_rows, m0_cols, m1, m1_rows, m1_cols, result, code=0):
        t = AssemblyTest(self, "matmul.s")
        # we need to include (aka import) the dot.s file since it is used by matmul.s
        t.include("dot.s")

        # create arrays for the arguments and to store the result
        array0 = t.array(m0)
        array1 = t.array(m1)
        array_out = t.array([0] * len(result))
        #array_check = t.array(result)
        # load address of input matrices and set their dimensions
        #raise NotImplementedError("TODO")
        # TODO

        # Load values to run matmul
        t.input_array("a0", array0) # a0: Load array address for array0
        t.input_scalar("a1", m0_rows)
        t.input_scalar("a2", m0_cols)
        t.input_array("a3", array1) # a1: Load array address for array1
        t.input_scalar("a4", m1_rows)
        t.input_scalar("a5", m1_cols)
        t.input_array("a6", array_out)



    
        # call the matmul function
        t.call("matmul")
        # check the content of the output array
        # TODO
        t.check_array(array_out,result)
        # generate the assembly file and run it through venus, we expect the simulation to exit with code `code`
        t.execute(code=code)
    
    # Correctly multiplies
    def test1(self):
        self.do_matmul(
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 3, 3,
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 3, 3,
            [30, 36, 42, 66, 81, 96, 102, 126, 150]
        )
    # Correctly multiplies
    def test2(self):
        self.do_matmul(
            [1, 2, 3, 4], 2, 2,
            [1, 2, 3, 4], 2, 2,
            [7, 10, 15, 22]
        )

    # Error 72: m0 height
    def test3(self):
        self.do_matmul(
            [1, 2, 3], 0, 3,
            [1, 2, 3], 2, 2,
            [7, 10, 15, 22],
            code = 72
        )
    # Error 72: m0 width
    def test4(self):
        self.do_matmul(
            [1, 2, 3, 4], 2, 0,
            [1, 2, 3, 4], 2, 2,
            [7, 10, 15, 22],
            code = 72
        )
    # Error 73: m1 height
    def test5(self):
        self.do_matmul(
            [1, 2, 3, 4], 2, 2,
            [1, 2, 3, 4], 0, 2,
            [7, 10, 15, 22],
            code = 73
        )
    # Error 73: m1 width
    def test6(self):
        self.do_matmul(
            [1, 2, 3, 4], 2, 2,
            [1, 2, 3, 4], 2, 0,
            [7, 10, 15, 22],
            code = 73
        )
    # Error 74: dimensions don't align
    def test7(self):
        self.do_matmul(
            [1, 2, 3, 4], 2, 2,
            [1, 2, 3, 4], 3, 1,
            [7, 10, 15, 22],
            code = 74
        )
    # Error 73: Prioritize error in m1 dims over dims don't align
    def test8(self):
        self.do_matmul(
            [1, 2, 3, 4], 2, 1,
            [1, 2, 3, 4], 4, 0,
            [7, 10, 15, 22],
            code = 73
        )
    # Error 72: Prioritize error in m0 dims over m1 dims erro4
    def test9(self):
        self.do_matmul(
            [1, 2, 3, 4], 0, 4,
            [1, 2, 3, 4], 4, 0,
            [7, 10, 15, 22],
            code = 72
        )
    # Error 72: Prioritize error in m0 dims over dims don't align
    def test10(self):
        self.do_matmul(
            [1, 2, 3, 4], 0, 1,
            [1, 2, 3, 4], 4, 1,
            [7, 10, 15, 22],
            code = 72
        )
    # Error 72: Prioritize error in m0 dims over m1 error & dims don't align error
    def test11(self):
        self.do_matmul(
            [1, 2, 3, 4], 4, 0,
            [1, 2, 3, 4], 4, 0,
            [7, 10, 15, 22],
            code = 72
        )
    # WORKS: 2x3 * 3x4
    def test12(self):
        self.do_matmul(
            [1, 2, 3, 4, 5, 6], 2, 3,
            [1, 2, 3, 4, -1, -2, -3, -4, 2, 2, 2, 2], 3, 4,
            [5, 4, 3, 2, 11, 10, 9, 8]
        )
    # WORKS: 1x1 * 1x1
    def test13(self):
        self.do_matmul(
            [5], 1, 1, 
            [6], 1, 1, 
            [30]
        )
    def test14(self):
        self.do_matmul(
            [-1, -2, 3, 4, -5, 6, -7, 8, 9, -10, 11, -12], 3, 4,
            [1, 2, 3, 4], 4, 1,
            [20, 18, -26]
            )
    @classmethod
    def tearDownClass(cls):
        print_coverage("matmul.s", verbose=False)


class TestReadMatrix(TestCase):

    def testNegs(self, fail = '', code = 0):
        t = AssemblyTest(self, "read_matrix.s")
        # load address to the name of the input file into register a0
        t.input_read_filename("a0", "inputs/simple1/bin/m0.bin")
        # allocate space to hold the rows and cols output parameters
        rows = t.array([-1])
        cols = t.array([-1])

        # load the addresses to the output parameters into the argument registers
       # raise NotImplementedError("TODO")
        # TODO
        t.input_array("a1", rows)
        t.input_array("a2", cols)

        # call the read_matrix function
        t.call("read_matrix")

        # check the output from the function
        # TODO
        t.check_array(rows, [3])
        t.check_array(cols, [4])
        t.check_array_pointer("a0", [-1, -2, 3, 4, -5, 6, -7, 8, 9, -10, 11, -12])

        t.execute(fail=fail, code=code)
    def test6(self):
        self.testNegs()


    def do_read_matrix(self, fail='', code=0):
        t = AssemblyTest(self, "read_matrix.s")
        # load address to the name of the input file into register a0
        t.input_read_filename("a0", "inputs/test_read_matrix/test_input.bin")
        #t.input_read_filename("a0", "inputs/simple1/bin/m0.bin")
		#t.input_read_filename("a0", "test.txt")
        # allocate space to hold the rows and cols output parameters
        rows = t.array([-1])
        cols = t.array([-1])

        # load the addresses to the output parameters into the argument registers
        t.input_array("a1", rows)
        t.input_array("a2", cols)

        # call the read_matrix function
        t.call("read_matrix")

        # check the output from the function
        # TODO
        t.check_array(rows, [3])
        t.check_array(cols, [3])
        t.check_array_pointer("a0", [1, 2, 3, 4, 5, 6, 7, 8, 9])

        # generate assembly and run it through venus
        t.execute(fail=fail, code=code)

    # Faulty Malloc Test
    def test1(self):
        self.do_read_matrix(fail = 'malloc', code = 88)
    # Faulty Open Test
    def test2(self):
        self.do_read_matrix(fail = 'fopen', code = 90)
    # Faulty Read Test
    def test3(self):
        self.do_read_matrix(fail = 'fread', code = 91)
    # Faulty Close Test
    def test4(self):
        self.do_read_matrix(fail = 'fclose', code = 92)

    # Actual Working Test
    def test5(self):
        self.do_read_matrix()
    

    @classmethod
    def tearDownClass(cls):
        print_coverage("read_matrix.s", verbose=False)


class TestWriteMatrix(TestCase):

    def do_write_matrix(self, fail='', code=0):
        t = AssemblyTest(self, "write_matrix.s")
        outfile = "outputs/test_write_matrix/student.bin"
        # load output file name into a0 register
        t.input_write_filename("a0", outfile)
        # load input array and other arguments
        # a1 should point to start of matrix in memory
        # a2 should be an integer for # rows = height
        # a3 should be an integer for # cols = width
        inpArray = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        t.input_array("a1", inpArray)
        t.input_scalar("a2", 3)
        t.input_scalar("a3", 3)

        #raise NotImplementedError("TODO")

        # TODO
        # call `write_matrix` function
        t.call("write_matrix")

        # generate assembly and run it through venus
        t.execute(fail=fail, code=code)
        # compare the output file against the reference
        if code == 0:
            t.check_file_output(outfile, "outputs/test_write_matrix/reference.bin")
    def test1(self):
        self.do_write_matrix()
    def test2(self):
        self.do_write_matrix(fail = 'fopen', code = 93)
    def test3(self):
        self.do_write_matrix(fail = 'fwrite', code = 94)
    def test4(self):
        self.do_write_matrix(fail = 'fclose', code = 95)

    @classmethod
    def tearDownClass(cls):
        print_coverage("write_matrix.s", verbose=False)


class TestClassify(TestCase):

    def make_test(self):
        t = AssemblyTest(self, "classify.s")
        t.include("argmax.s")
        t.include("dot.s")
        t.include("matmul.s")
        t.include("read_matrix.s")
        t.include("relu.s")
        t.include("write_matrix.s")
        return t
    def test_simple0_input0(self):
        t = self.make_test()
        out_file = "outputs/test_basic_main/student0.bin"
        ref_file = "outputs/test_basic_main/reference0.bin"
        args = ["inputs/simple0/bin/m0.bin", "inputs/simple0/bin/m1.bin",
                "inputs/simple0/bin/inputs/input0.bin", out_file]

        t.input_scalar("a2", 0)
        # call classify function
        t.call("classify")
        # generate assembly and pass program arguments directly to venus
        t.execute(args=args)

        t.check_file_output(out_file, ref_file)
        t.check_stdout("2") 

    def test_malloc_fail(self):
        t = self.make_test()
        out_file = "outputs/test_basic_main/student0.bin"
        ref_file = "outputs/test_basic_main/reference0.bin"
        args = ["inputs/simple0/bin/m0.bin", "inputs/simple0/bin/m1.bin",
                "inputs/simple0/bin/inputs/input0.bin", out_file]

        t.input_scalar("a2", 1)
        # call classify function
        t.call("classify")
        # generate assembly and pass program arguments directly to venus
        t.execute(args=args, fail = "malloc", code = 88)
    def test_args_fail(self):
        t = self.make_test()
        out_file = "outputs/test_basic_main/student0.bin"
        ref_file = "outputs/test_basic_main/reference0.bin"
        args = ["inputs/simple0/bin/m0.bin", "inputs/simple0/bin/m1.bin",
                out_file]

        t.input_scalar("a2", 1)
        # call classify function
        t.call("classify")
        # generate assembly and pass program arguments directly to venus
        t.execute(args=args, code = 89)



    @classmethod
    def tearDownClass(cls):
        print_coverage("classify.s", verbose=True)


class TestMain(TestCase):

    def run_main(self, inputs, output_id, label):
        args = [f"{inputs}/m0.bin", f"{inputs}/m1.bin", f"{inputs}/inputs/input0.bin",
                f"outputs/test_basic_main/student{output_id}.bin"]
        reference = f"outputs/test_basic_main/reference{output_id}.bin"
        t = AssemblyTest(self, "main.s", no_utils=True)
        t.call("main")
        t.execute(args=args, verbose=False)
        t.check_stdout(label)
        t.check_file_output(args[-1], reference)

    
    def test0(self):
        self.run_main("inputs/simple0/bin", "0", "2")

    def test1(self):
        self.run_main("inputs/simple1/bin", "1", "1")

'''
class TestMNIST(TestCase):
    def run_mnist(self, inputs, output_id, label):
        args = [f"{inputs}/m0.bin", f"{inputs}/m1.bin", f"{inputs}/inputs/mnist_input{output_id}.bin",
                f"outputs/test_mnist_main/student{output_id}.bin"]
        t = AssemblyTest(self, "main.s", no_utils=True)
        t.call("main")
        t.execute(args=args, verbose=False)
        t.check_stdout(label)

        results = [6, 9, 9, 2, 9, 4, 4, 8, 7] # inp2 (7), inp7 (2) are misclassified
        
    def testMNIST0(self):
        self.run_mnist("inputs/mnist/bin", "0", "6")
    def testMNIST1(self):
        self.run_mnist("inputs/mnist/bin", "1", "9")
    def testMNIST2(self):
        self.run_mnist("inputs/mnist/bin", "2", "9")
    def testMNIST3(self):
        self.run_mnist("inputs/mnist/bin", "3", "2")
    def testMNIST4(self):
        self.run_mnist("inputs/mnist/bin", "4", "9")
    def testMNIST5(self):
        self.run_mnist("inputs/mnist/bin", "5", "4")
    def testMNIST6(self):
        self.run_mnist("inputs/mnist/bin", "6", "4")
    def testMNIST7(self):
        self.run_mnist("inputs/mnist/bin", "7", "8")
    def testMNIST8(self):
        self.run_mnist("inputs/mnist/bin", "8", "7")

'''
