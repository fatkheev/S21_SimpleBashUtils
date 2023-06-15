GCC=gcc -Wall -Werror -Wextra


all: s21_cat

s21_cat: s21_cat.o
	$(GCC) s21_cat.o -o s21_cat

s21_cat.o: s21_cat.c
	$(GCC) -c s21_cat.c -o s21_cat.o

test: s21_cat test_func_cat.sh
	sh test_func_cat.sh

valgrind: 
	CK_FORK=no valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all --verbose --log-file=RESULT.txt ./s21_cat -i test1.txt
	cat RESULT.txt
	rm -rf RESULT.txt

rebuild: clean all

clean:
	rm -rf *.o
	rm -rf *.a
	rm -rf *.out
	rm -rf s21_cat