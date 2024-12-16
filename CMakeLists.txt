cmake_minimum_required(VERSION 3.20)

project(LessonDB)

set(PostgreSQL_ADDITIONAL_VERSIONS "17")

add_executable(LessonDB main.cpp "CreateDB.h" "CreateDB.cpp")

add_subdirectory("D:\\libpqxx-7.9.2" libpqxx-build)

target_compile_features(LessonDB PRIVATE cxx_std_17)

target_link_libraries(LessonDB PRIVATE pqxx)





