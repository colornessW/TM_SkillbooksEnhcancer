cmake_minimum_required(VERSION 3.21)

project(
	TM_SkillbooksEnhcancer
	LANGUAGES CXX
	VERSION 1.0.0
)

set(ENABLE_SKYRIM_SE ON CACHE BOOL "Enable Skyrim SE support")
set(ENABLE_SKYRIM_AE ON CACHE BOOL "Enable Skyrim AE support")
set(ENABLE_SKYRIM_VR ON CACHE BOOL "Enable Skyrim VR support")
set(BUILD_TESTS OFF CACHE BOOL "Disable CommonLibSSE-NG tests" FORCE)
set(SKSE_SUPPORT_XBYAK OFF CACHE BOOL "Disable Xbyak" FORCE)

set(COMMONLIBSSE_PATH "D:/WorkSpace/Thu'mundus/CommonLibSSE-NG" CACHE PATH "Path to CommonLibSSE-NG source tree")

add_subdirectory(
	"${COMMONLIBSSE_PATH}"
	"${CMAKE_CURRENT_BINARY_DIR}/CommonLibSSE-NG"
)

add_library(${PROJECT_NAME} SHARED
	src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
	CommonLibSSE
)

target_compile_features(${PROJECT_NAME} PUBLIC
	cxx_std_23
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
	WINVER=0x0601
	_WIN32_WINNT=0x0601
)

if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE
		/we4715
		/wd4005
		/wd4061
		/wd4068
		/wd4200
		/wd4201
		/wd4265
		/wd4266
		/wd4371
		/wd4514
		/wd4582
		/wd4583
		/wd4623
		/wd4625
		/wd4626
		/wd4710
		/wd4711
		/wd4820
		/wd4996
		/wd5026
		/wd5027
		/wd5045
		/wd5053
		/wd5204
		/wd5220
	)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
	PREFIX ""
)
