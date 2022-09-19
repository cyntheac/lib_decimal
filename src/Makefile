CC =gcc
FLAG_C = -c
FLAG_O = -o
CFLAGS =-Wall -Werror -Wextra -pedantic -std=c11
GCOVF =-fprofile-arcs -ftest-coverage

all: s21_gcov_decimal.a test gcov_report

clean:
	rm -rf *.o *.a test *.out *.gcda *.gcno *.html *.css tests_s21_decimal.c

test: s21_gcov_decimal.a tests_s21_decimal.c
	$(CC) $(CFLAGS) $(GCOVF) -lcheck $? $(FLAG_O) $@

tests_s21_decimal.c:
	checkmk tests_s21_decimal > $@

s21_gcov_decimal.a: s21_*.c
	$(CC) $(CFLAGS) $(GCOVF) $(FLAG_C) $?
	ar -rcs $@ *.o
	rm -f *.o

s21_decimal.a: s21_*.c
	$(CC) $(FLAG_C) $(CFLAGS) $?
	ar -rcs $@ *.o
	rm -f *.o

rebuild: clean all

gcov_report: test
	./$?
	gcovr -r . --html --html-details $(FLAG_O) coverage_report.html
	rm -f *.gcda *.gcno
	open coverage_report.html

linter:
	python3 ../materials/linters/cpplint.py *.c *.h
