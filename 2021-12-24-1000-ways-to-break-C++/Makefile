all: include_any_text limit_include_depth undef___STDC__ stdlib_headers_inside_namespace success_message

include_any_text:
	@echo "Built target include_any_text..."
	@g++ include_any_text.cpp -o include_any_text.out

limit_include_depth:
	@echo "Built target limit_include_depth... "
	@################################################## 5 to dive deeper into the stdlib
	@if ! gcc limit_include_depth.c -fmax-include-depth=5 2>&1; then \
		echo "catched error"; \
	else \
		echo "expected error"; \
	fi

undef___STDC__:
	@echo -n "Built target undef___GNUC__... "
	@if ! g++ undef___STDC__.cpp 2>&1; then \
		echo "catched error"; \
	else \
		echo "expected error"; \
	fi

stdlib_headers_inside_namespace:
	@echo "Built target stdlib_headers_inside_namespace..."
	@g++ stdlib_headers_inside_namespace.cpp -o stdlib_headers_inside_namespace.out

success_message:
	@echo "Success!"

clean:
	rm -rf *.o
	rm -rf *.out
