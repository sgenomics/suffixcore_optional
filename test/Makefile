
test: *.cpp *.h ../*.cpp ../*.h
	g++ -Wall -g test_main.cpp test_suffixnodestoredisk.cpp test_diskvector.cpp ./suffixcore/SuffixNode.cpp ./suffixcore/tialloc/tialloc.cpp ../SuffixNodeStoreDisk.cpp -o test -I.. -I./suffixcore/tialloc -I. -I./suffixcore
clean:
	rm test
