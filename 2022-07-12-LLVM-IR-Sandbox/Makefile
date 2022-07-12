all: llc

llc:
	llc code.ll
	clang++ -o code code.s

clean:
	rm -rf code.s code
