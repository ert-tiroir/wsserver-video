
SOURCEDIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))/build
SOURCES   := $(shell find $(SOURCEDIR) -name '*.cpp')

init:
	git submodule update --init --remote

clean:
	-[ -e build ] && rm -rf build
	-[ -e out ] && rm -f out
copysrc:
	mkdir -p build

	cp example.cpp build
	cp -r libs/wsserver/wsserver build/

compile:
	g++ -o build/out -I./build $(SOURCES)
	mv build/out ./out
build:
	make -B copysrc
	make -B compile
