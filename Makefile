CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
COVERAGE_FLAGS = -std=c++17 -Wall -Wextra -O0 -g --coverage
COVERAGE_MIN = 90
COVERAGE_CSV = build/coverage_report/summary.csv

STRUCT_SOURCES = $(wildcard struct/*/*.cpp)
SOURCES = ProcessCommand.cpp $(STRUCT_SOURCES)
HEADERS = ProcessCommand.h $(wildcard struct/*/*.h)
TEST_SOURCES = $(wildcard tests/*.cpp)
FORMAT_SOURCES = main.cpp benchmark.cpp $(SOURCES) $(HEADERS) $(TEST_SOURCES) $(wildcard tests/*.h)
GTEST_FLAGS = $(shell pkg-config --cflags --libs gtest_main)
TARGET = build/app

GCOV = gcov
ifeq ($(shell uname -s),Darwin)
GCOV = xcrun llvm-cov gcov
endif

define check_cli
	@set -eu; \
	app="$(abspath $(1))"; \
	cli_dir=$$(mktemp -d "$(CURDIR)/build/cli-check.XXXXXX"); \
	trap 'rm -rf "$$cli_dir"' EXIT; \
	cli_file="$$cli_dir/stack.bin"; \
	expect_failure() { \
		expected="$$1"; shift; status=0; \
		"$$app" "$$@" > "$$cli_dir/output" 2>&1 || status=$$?; \
		test "$$status" -eq 1; grep -q -- "$$expected" "$$cli_dir/output"; \
	}; \
	"$$app" > "$$cli_dir/output"; grep -q -- '--query' "$$cli_dir/output"; \
	"$$app" --help > "$$cli_dir/output"; grep -q 'SInitStack' "$$cli_dir/output"; \
	expect_failure 'Ошибка' --file; \
	expect_failure 'Ошибка' --unknown; \
	expect_failure '--query' --file "$$cli_file"; \
	expect_failure '--file' --query STop; \
	expect_failure 'Ошибка' --file "$$cli_file" --file "$$cli_file"; \
	"$$app" --file "$$cli_file" --query SInitStack; \
	"$$app" --file "$$cli_file" --query 'SPush "hello world"'; \
	"$$app" --file "$$cli_file" --query STop > "$$cli_dir/output"; \
	test "$$(cat "$$cli_dir/output")" = 'hello world'; \
	expect_failure 'Ошибка' --file "$$cli_file" --query SBad; \
	echo 'CLI: 11 проверок прошли'
endef

all: $(TARGET)

$(TARGET): main.cpp $(SOURCES) $(HEADERS)
	mkdir -p build
	$(CXX) $(CXXFLAGS) main.cpp $(SOURCES) -o $(TARGET)

build/benchmark: benchmark.cpp $(STRUCT_SOURCES) $(HEADERS)
	mkdir -p build
	$(CXX) $(CXXFLAGS) benchmark.cpp $(STRUCT_SOURCES) -o $@

bench: build/benchmark
	./build/benchmark

build/run_tests: $(TEST_SOURCES) tests/test_support.h $(SOURCES) $(HEADERS)
	mkdir -p build
	$(CXX) $(CXXFLAGS) -I. $(TEST_SOURCES) $(SOURCES) $(GTEST_FLAGS) -o $@

test: build/run_tests $(TARGET)
	cd build && ./run_tests
	$(call check_cli,$(TARGET))

build/coverage/run_tests: $(TEST_SOURCES) tests/test_support.h $(SOURCES) $(HEADERS)
	mkdir -p build/coverage
	$(CXX) $(COVERAGE_FLAGS) -I. $(TEST_SOURCES) $(SOURCES) $(GTEST_FLAGS) -o $@

build/coverage/app: main.cpp $(SOURCES) $(HEADERS)
	mkdir -p build/coverage
	$(CXX) $(COVERAGE_FLAGS) main.cpp $(SOURCES) -o $@

build/coverage/benchmark: benchmark.cpp $(STRUCT_SOURCES) $(HEADERS)
	mkdir -p build/coverage
	$(CXX) $(COVERAGE_FLAGS) benchmark.cpp $(STRUCT_SOURCES) -o $@

gcov_report: build/coverage/run_tests build/coverage/app build/coverage/benchmark
	find build/coverage -name '*.gcda' -delete
	cd build/coverage && ./run_tests
	$(call check_cli,build/coverage/app)
	./build/coverage/benchmark > build/coverage/benchmark_output.txt
	mkdir -p build/coverage_report
	gcovr --root . build/coverage --gcov-executable '$(GCOV)' --filter 'struct/' --filter 'ProcessCommand.cpp' --filter 'main.cpp' --filter 'benchmark.cpp' --html-details build/coverage_report/index.html --json-summary build/coverage_report/summary.json --csv build/coverage_report/summary.csv --txt-summary --fail-under-line $(COVERAGE_MIN) --fail-under-function $(COVERAGE_MIN)
	$(MAKE) --no-print-directory check-coverage

check-coverage:
	@awk -F, -v minimum=$(COVERAGE_MIN) '\
	NR == 1 { if ($$2 != "line_total" || $$3 != "line_covered" || $$8 != "function_total" || $$9 != "function_covered") failed = 1 } \
	NR > 1 { rows++; \
		if (($$2 > 0 && 100 * $$3 < minimum * $$2) || ($$8 > 0 && 100 * $$9 < minimum * $$8)) { \
			print "Покрытие ниже порога:", $$1; failed = 1 \
		} \
	} END { \
		if (!rows) { print "Нет статистики покрытия"; failed = 1 } \
		if (!failed) print "В каждом файле: строки и функции не ниже " minimum "%"; \
		exit failed \
	}' '$(COVERAGE_CSV)'

coverage: gcov_report

format:
	clang-format -i $(FORMAT_SOURCES)

format-check:
	clang-format --dry-run --Werror $(FORMAT_SOURCES)

lint:
	cppcheck --std=c++17 --enable=all --suppress=missingIncludeSystem -I. main.cpp ProcessCommand.cpp benchmark.cpp struct tests

clean:
	rm -rf build

.PHONY: all bench test gcov_report coverage check-coverage format format-check lint clean
