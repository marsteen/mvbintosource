cmake_minimum_required(VERSION 3.10)
project("mvbintosource")

include_directories(./include)

# strip .exe
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")

add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
    lib/NStringTool.cpp
    lib/CFileIO.cpp
)


