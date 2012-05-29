
test: *.cpp *.h ../*.cpp ../*.h
	g++ -Wall -O3 test_main.cpp test_suffixnodestoredisk.cpp ./suffixcore/SuffixNode.cpp ./suffixcore/tialloc/tialloc.cpp ../SuffixNodeStoreDisk.cpp -o test -I.. -I./suffixcore/tialloc -I./suffixcore
clean:
	rm test
