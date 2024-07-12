all:
	ninja -C build


run: all
	./build/c_http

clear:
	rm -rf build/*
	rm -rf build/.*

setup: clear
	cmake -B build -S . -GNinja
