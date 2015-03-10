# this is set up to by default to make the top level and test with CppUTest
all: codeCppUTest

clean: cleanCodeCppUTest

codeCppUTest: CppUTest
	make -i -f MakefileCppUTest.mk 

cleanCodeCppUTest:
	make -i -f MakefileCppUTest.mk clean
	
CppUTest: CppUTest/lib/libCppUTest.a

CppUTest/lib/libCppUTest.a:
	pwd
	make -i -C CppUTest
