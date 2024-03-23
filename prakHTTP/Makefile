all: compile run


compile:
	g++ main.cpp thread.cpp mutex.cpp

run:
	./a.out http://localhost/file.txt

run_multithr:
	./a.out http://localhost/file.txt -j 5