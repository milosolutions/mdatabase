if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif ()

cmake_minimum_required(VERSION 3.5)

project(mdatabase
  VERSION 0.0.1
  DESCRIPTION "Milo SQL Migrations System"
  LANGUAGES CXX
)

find_package(Qt5 COMPONENTS REQUIRED Core Sql)

set(CMAKE_AUTOMOC ON)

set(SOURCES
  connectionproviders/mconnectionproviderbase.h
  connectionproviders/mconnectionprovidersqlite.h
  connectionproviders/mconnectionprovidersqlitecipher.h
  connectionproviders/mconnectionprovidersqlitesee.h
  mdbhelpers.h
  mmigration.h
  mmigrationmanager.h
  mabstractmigrationmanager.h
  mmigrationsdata.h
  connectionproviders/mconnectionproviderbase.cpp
  connectionproviders/mconnectionprovidersqlite.cpp
  connectionproviders/mconnectionprovidersqlitecipher.cpp
  connectionproviders/mconnectionprovidersqlitesee.cpp
  mdbhelpers.cpp
  mmigration.cpp
  mabstractmigrationmanager.cpp
)

set(OTHER_FILES README.md mdatabase.doxyfile)

add_library(mdatabase STATIC ${SOURCES} ${OTHER_FILES})

target_include_directories(mdatabase
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(mdatabase
  Qt5::Core
  Qt5::Sql
)

add_subdirectory(tst_migrations)
add_subdirectory(example)
