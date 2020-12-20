from distutils.core import setup, Extension
import sysconfig

def main():
	CFLAGS = ['-g', '-Wall', '-std=c99', '-fopenmp', '-mavx', '-mfma', '-pthread', '-O3']
	LDFLAGS = ['-fopenmp']
	# Use the setup function we imported and set up the modules.
	# You may find this reference helpful: https://docs.python.org/3.6/extending/building.html
	# TODO: YOUR CODE HERE

	# Creating an instance of a module--does the first name matter?
	module = Extension('numc', 
					   sources = ['matrix.c', 'numc.c'],
					   include_dirs = ['/usr/include/python3.6m'],
					   extra_compile_args = CFLAGS,
					   extra_link_args = LDFLAGS)
	# Call to setup
	setup(name = 'numc', 
		  version = '1.0',
		  description = 'Matrix Operations',
		  author = 'Peter & Zachary Zhu',
		  long_description = 'Matrix Operations for Fall 2020 Project 4',
		  url = 'https://cs61c.org/fa20/projects/proj4/',
		  ext_modules = [module]
		)

if __name__ == "__main__":
    main()
