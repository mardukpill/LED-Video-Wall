cmake_minimum_required(VERSION 3.16)

# TODO: changing main to src causes issues with components
# set(EXTRA_COMPONENT_DIRS ${CMAKE_SOURCE_DIR}/src)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(client)

