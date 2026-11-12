GO = go
PACKAGES = . ./commands ./structures
SOURCES = $(wildcard *.go commands/*.go structures/*.go)
COVERAGE_MIN = 90

all: build

build:
	mkdir -p build
	$(GO) build -o build/lab3go .

test:
	$(GO) test $(PACKAGES)

bench:
	mkdir -p build
	$(GO) test ./structures -run '^$$' -bench . -benchmem -benchtime=100ms -count=1 > build/bench_results.txt
	cat build/bench_results.txt

lint:
	$(GO) vet ./...

format:
	gofmt -w .

format-check:
	@test -z "$$(gofmt -l $(SOURCES) internal/testutil/*.go tools/*.go)" || (gofmt -l $(SOURCES) internal/testutil/*.go tools/*.go; exit 1)

gcov_report:
	mkdir -p build
	$(GO) test $(PACKAGES) -coverprofile=build/coverage.out -covermode=atomic
	$(GO) tool cover -func=build/coverage.out > build/coverage_functions.txt
	$(GO) tool cover -html=build/coverage.out -o build/coverage.html
	$(GO) run ./tools build/coverage.out $(COVERAGE_MIN)

coverage: gcov_report

clean:
	rm -rf build

.PHONY: all build test bench lint format format-check gcov_report coverage clean
