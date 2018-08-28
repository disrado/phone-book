cmake_minimum_required(VERSION 3.10)

project(app CXX)

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES
	"src/*.cpp"
)

file(GLOB_RECURSE ${PROJECT_NAME}_UIS
	"src/*.ui"
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
	${${PROJECT_NAME}_SOURCES}
)

set_property(
	TARGET ${PROJECT_NAME}
	PROPERTY POSITION_INDEPENDENT_CODE ON)

target_link_libraries(${PROJECT_NAME}
	PUBLIC
		libpb
		Qt5::Widgets
		Qt5::Gui
		Qt5::Core
)
target_include_directories(${PROJECT_NAME}
	PUBLIC
		src
)

cotire(${PROJECT_NAME})