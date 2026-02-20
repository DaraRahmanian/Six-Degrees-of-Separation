
# Require CMAKE 3.12 or higher
cmake_minimum_required(VERSION 3.12)

set(HEADER_FILES
    IMDBData.h
	IMDBGraph.h
)

set(SOURCE_FILES
    IMDBData.cpp
	IMDBGraph.cpp
)

add_library(src ${SOURCE_FILES} ${HEADER_FILES})
