BUILD_DIR=build
PROJECT_NAME=nucleo-l552ze-q

BOARD_MOUNT_POINT=/media/antoine/NODE_L552ZE

TOOLCHAIN_FILE=arm-none-eabi-gcc.cmake
TOOLCHAIN_ARG=-DCMAKE_TOOLCHAIN_FILE=${TOOLCHAIN_FILE}

all: clean mkdir_build cmake_config cmake_build copy

buildcp: cmake_build copy

clean:
	rm -rf ${BUILD_DIR}

mkdir_build:
	mkdir -p ${BUILD_DIR}

cmake_config:
	cd ${BUILD_DIR} && cmake ${TOOLCHAIN_ARG} -DCMAKE_BUILD_TYPE=Release ..

cmake_build:
	cd ${BUILD_DIR} && cmake --build . -j

copy:
	cp ${BUILD_DIR}/${PROJECT_NAME}.bin ${BOARD_MOUNT_POINT}
