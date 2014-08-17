all:
	mkdir -p build
	cd build && cmake ..
	cd build && make

run:
	build/TestGLFW

clean:
	cd glfw/build && make clean
	cd build && make clean
	rm -rf build
	rm -rf glfw/build
setup:
	bash linux_setup.sh
