Read me for ECE420 lab3
March.15 2017

=====
Overview:
	There are four groups of file in this folder

-----
	Implementations:
        Parallel_program_version1.c : Find all components of blocks that can be parallelized, use the most basic way to handle critical section.
	Parallel_program_version2.c : Improve the critical section implementation of finding maximum leading entry.
	Parallel_program_version3.c : Further improvement in the critical section implementation of finding maximum leading entry.
	Parallel_program_version4.c : Using dynamic scheduling policy to parallize "for" loop.
	Parallel_program_version5.c : Using guided scheduling policy to parallize "for" loop.
	Parallel_program_version6.c : Using sections to handle pivoting procedure in solving the linear system.

-----
	SerialTester:
	serialtester.c : used for solving linear system of equations in sequantial order

-----
	Makefile
		use $make to compile all of the files needed

-----
	Important functions:
	timer.h	   macro definition for time measurement
        Lab3IO.c : used to generate input output matrix
	
=====
Compile Instructions:

Direct into this folder and type "make" to compile all the files. Results can be validated by running ./check.sh.







