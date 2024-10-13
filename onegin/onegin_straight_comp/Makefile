osc: onegin_ctors.o onegin_output.o onegin_sort_func.o onegin_main.o
	gcc onegin_ctors.o onegin_output.o onegin_sort_func.o onegin_main.o -o osc

onegin_ctors.o: onegin_ctors.cpp
	gcc -c onegin_ctors.cpp -o onegin_ctors.o

onegin_output.o: onegin_output.cpp
	gcc -c onegin_output.cpp -o onegin_output.o

onegin_sort_func.o: onegin_sort_func.cpp
	gcc -c onegin_sort_func.cpp -o onegin_sort_func.o

onegin_main.o: onegin_main.cpp
	gcc -c onegin_main.cpp -o onegin_main.o

clean:
	rm -f *.o osc