

all : unit-test
	./unit-test

unit-test : unit-test.cpp ../src/PLMN_RK.cpp ../src/PLMN_RK.h libwiringgcc
	gcc unit-test.cpp ../src/PLMN_RK.cpp UnitTestLib/libwiringgcc.a -DUNITTEST -std=c++17 -lc++ -IUnitTestLib -I../src -o unit-test

check : unit-test.cpp ../src/PLMN_RK.cpp ../src/PLMN_RK.h libwiringgcc
	gcc unit-test.cpp ../src/PLMN_RK.cpp UnitTestLib/libwiringgcc.a -DUNITTEST -g -O0 -std=c++17 -lc++ -IUnitTestLib -I../src -o unit-test && valgrind --leak-check=yes ./unit-test 

libwiringgcc :
	cd UnitTestLib && make libwiringgcc.a 	
	
.PHONY: libwiringgcc
