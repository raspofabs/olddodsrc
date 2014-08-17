if [ ! -d build ] ; then
	mkdir build
	sudo apt-get install xorg-dev libglu1-mesa-dev
	cd glfw
	mkdir -p build
	cd build
	cmake .. -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DBUILD_SHARED_LIBS=OFF
	make
	cd ..
fi
cd build
cmake ..
make

