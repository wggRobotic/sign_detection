all:
	ninja -C build


run: all
	./build/main

clear:
	rm -rf build/*
	rm -rf build/.*

setup: clear
	cmake -B build -S . -GNinja 
