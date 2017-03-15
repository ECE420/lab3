all:
	gcc Parallel_program_version1.c Lab3IO.c -o Version1 -fopenmp -lm
	gcc Parallel_program_version2.c Lab3IO.c -o Version2 -fopenmp -lm
	gcc Parallel_program_version3.c Lab3IO.c -o Version3 -fopenmp -lm
	gcc Parallel_program_version4.c Lab3IO.c -o Version4 -fopenmp -lm
	gcc serialtester.c Lab3IO.c -o serialtester -fopenmp -lm
	gcc datagen.c Lab3IO.c -o datagen
