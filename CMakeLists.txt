
cmake_minimum_required(VERSION 2.8)

project(turbo2)

# if this is the first time running cmake, perform pre-build dependency install script (or if the user manually triggers re-building the dependencies)
set(BUILD_DEPS "YES" CACHE BOOL "If YES, will install dependencies.  Automatically reset to NO after dependencies are installed.")
set(ROVERNET_PATH "${PROJECT_BINARY_DIR}/rovernet" CACHE FILEPATH "Path to rovernet repo")
set(INFERENCE_PATH "${PROJECT_BINARY_DIR}/jetson-inference" CACHE FILEPATH "Path to jetson-inference repo")
#set(ROVERNET_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../rovernet" CACHE FILEPATH "Path to rovernet repo")

if( ${BUILD_DEPS} )
	message("Launching pre-build dependency installer script...")

	execute_process(COMMAND sh ../CMakePreBuild.sh
				WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
				RESULT_VARIABLE PREBUILD_SCRIPT_RESULT)

	set(BUILD_DEPS "NO" CACHE BOOL "If YES, will install dependencies.  Automatically reset to NO after dependencies are installed." FORCE)
	message("Finished installing dependencies")
endif()

find_package(CUDA)

# Pass options to NVCC
set(
	CUDA_NVCC_FLAGS
	${CUDA_NVCC_FLAGS};
	-O3 -gencode arch=compute_53,code=sm_53
)


# build turbo2
file(GLOB turboSources *.cpp)
file(GLOB turboIncludes *.h)

# rpLIDAR sdk
file(GLOB rpSrcA rplidar-sdk/src/arch/linux/*.cpp)
file(GLOB rpSrcB rplidar-sdk/src/hal/*.cpp)
file(GLOB rpSrcC rplidar-sdk/src/*.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/rplidar-sdk/include ${CMAKE_CURRENT_SOURCE_DIR}/rplidar-sdk/src)
include_directories(${ROVERNET_PATH}/c ${ROVERNET_PATH}/cuda)
link_directories(${ROVERNET_PATH}/build)
message("path to rovernet:  ${ROVERNET_PATH}")


# compile executable
cuda_add_executable(turbo2 ${turboSources} ${rpSrcA} ${rpSrcB} ${rpSrcC})

target_link_libraries(turbo2 usb-1.0 pthread rovernet phidget21)


# transfer all LUA scripts to bin directory
file(GLOB rovernetScripts ${ROVERNET_PATH}/lua/*.lua)

foreach(rovernetScript ${rovernetScripts})
	message("-- Configuring ${rovernetScript}")
	configure_file(${rovernetScript} ${PROJECT_BINARY_DIR} COPYONLY)
endforeach()



# transfer all startup scripts to bin directory
file(GLOB init_scripts scripts/init*.sh)

foreach(init_script ${init_scripts})
	message("-- Configuring ${init_script}")
	configure_file(${init_script} ${PROJECT_BINARY_DIR} COPYONLY)
endforeach()

