.PHONY: docs

all:
	mkdir -p build/ && \
	cd build && \
	cmake ../ && \
	make -j12 && \
	./oqs

run:
	cd build && \
	make -j12 && \
	./oqs

test:
	cd build && \
	cmake ../ && \
	make -j12 && \
	./tests-main

clean:
	rm -rf ./build/*
	rm -rf ./docs/_build/*
