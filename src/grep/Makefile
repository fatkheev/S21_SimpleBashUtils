GCC=gcc -Wall -Werror -Wextra -g


all: s21_grep

s21_grep: s21_grep.o
	$(GCC) s21_grep.o -o s21_grep

s21_grep.o: s21_grep.c
	$(GCC) -c s21_grep.c -o s21_grep.o

test: s21_grep test_func_grep.sh
	sh test_func_grep.sh

valgrind: 
	CK_FORK=no valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all --verbose --log-file=RESULT.txt ./s21_grep -i hello test1.txt
	cat RESULT.txt
	rm -rf RESULT.txt

rebuild: clean all

clean:
	rm -rf *.o
	rm -rf *.a
	rm -rf *.out
	rm -rf s21_grep