all: include_any_text limit_include_depth undef___STDC__

include_any_text:
	@echo "Built target include_any_text..."
	@g++ include_any_text.cpp -o include_any_text.out

limit_include_depth:
	@echo "Built target limit_include_depth..."
	@gcc limit_include_depth.c -fmax-include-depth=100 -o limit_include_depth.out

undef___STDC__:
	@echo -n "Built target undef___GNUC__... "
	@if ! g++ undef___STDC__.cpp 2>/dev/null; then \
		echo "catched error"; \
	else \
		echo "expected error"; \
	fi
		
	
clean:
	rm -rf *.o
	rm -rf *.out
