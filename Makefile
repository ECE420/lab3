all:
	gcc Parallel_program_version1.c Lab3IO.c -o Version1 -fopenmp -lm
	gcc Parallel_program_version2.c Lab3IO.c -o Version2 -fopenmp -lm
	gcc serialtester.c Lab3IO.c -o serialtester -fopenmp -lm
	gcc datagen.c Lab3IO.c -o datagen
