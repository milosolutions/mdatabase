find_package(Qt5 COMPONENTS REQUIRED Core Sql Test)

set(CMAKE_AUTOMOC ON)

add_executable(mdatabase_test tst_migrations.cpp)

target_link_libraries(mdatabase_test mdatabase
  Qt5::Core Qt5::Sql Qt5::Test
)

add_test(mdatabase_test mdatabase_test)
