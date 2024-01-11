CMAKE_MINIMUM_REQUIRED(VERSION 3.22)

# Toolchain
set(VCPKG_ROOT $ENV{VCPKG_ROOT} CACHE STRING "Root of the local vcpkg install")
set(ENV{VCPKG_FEATURE_FLAGS} "versions") # Enables version pinning in vcpkg.json
if (IS_DIRECTORY ${VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
endif()


if (NOT DEFINED ENV{$VCPKG_ROOT})
    cmake_path(SET VCPKG_SCRIPTS_PATH "${CMAKE_TOOLCHAIN_FILE}")
    cmake_path(REMOVE_FILENAME VCPKG_SCRIPTS_PATH)
    cmake_path(APPEND VCPKG_SCRIPTS_PATH "..")
    cmake_path(APPEND VCPKG_SCRIPTS_PATH "..")
    cmake_path(ABSOLUTE_PATH VCPKG_SCRIPTS_PATH)
    message(status "The final path: ${VCPKG_SCRIPTS_PATH}")
    set(VCPKG_ROOT "${VCPKG_SCRIPTS_PATH}")
endif()

message(STATUS ${VCPKG_ROOT})

if (DEFINED ENV{NUGET_EXE})
    set(NUGET_EXE $ENV{NUGET_EXE})
else()

    if (UNIX)
        set(NUGET_EXE ${VCPKG_ROOT}/downloads/tools/nuget-6.2.1-linux/nuget.exe)
    else()
        set(NUGET_EXE ${VCPKG_ROOT}/downloads/tools/nuget-6.2.1-windows/nuget.exe)
    endif()
endif()

project(game-host-adapter)

# C++17 is required
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)

file(READ ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg.json VCPKG_JSON)
string(JSON GAME_HOST_ADAPTER_VERSION GET ${VCPKG_JSON} version)

message(STATUS "${PROJECT_NAME} version ${GAME_HOST_ADAPTER_VERSION}")

set(VERSION_HEADER_DIR "${CMAKE_CURRENT_BINARY_DIR}/version")
configure_file(src/complete/version.h.in "${VERSION_HEADER_DIR}/version.h" @ONLY)
if (UNIX)
    cmake_host_system_information(RESULT OS_VERSION QUERY OS_VERSION)
    message("The os version ${OS_VERSION}")
    string(FIND ${OS_VERSION} "22.04" IS_22_INDEX)
    string(FIND ${OS_VERSION} "20.04" IS_20_INDEX)
    if (NOT IS_22_INDEX EQUAL -1)
        message(STATUS "The os version is 22.04")
        set(NUPKG_OS "Ubuntu.22_04")
        set(NUSPEC_NAME game-host-adapter-ubuntu-22.04)
    endif()
    if (NOT IS_20_INDEX EQUAL -1)
        message(STATUS "The os version is 20.04")
        set(NUPKG_OS "Ubuntu.20_04")
        set(NUSPEC_NAME game-host-adapter-ubuntu-20.04)
    endif()
    configure_file(${NUSPEC_NAME}.nuspec.in ${NUSPEC_NAME}.nuspec)
else()
    configure_file(game-host-adapter-windows-x64.nuspec.in ${CMAKE_CURRENT_BINARY_DIR}/game-host-adapter-windows-x64.nuspec.tmp)
    file(GENERATE OUTPUT game-host-adapter-windows-x64-$<CONFIG>.nuspec INPUT ${CMAKE_CURRENT_BINARY_DIR}/game-host-adapter-windows-x64.nuspec.tmp)
endif()

set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
    set(DEF_INSTALL_CMAKE_DIR CMake)
else()
    set(DEF_INSTALL_CMAKE_DIR lib/CMake/${PROJECT_NAME})
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files")

set(SOURCE_DIR src)
file(GLOB ${PROJECT_NAME}_SOURCES
        ${SOURCE_DIR}/complete/*.cpp
)
file(GLOB ${PROJECT_NAME}_HEADERS
        ${SOURCE_DIR}/complete/*.h
)

# GCC and Clang will autodetect a color-enabled terminal, but Ninja won't, so this option can force it for us
option(ENABLE_COLORED_OUTPUT "Produce ANSI-colored output" TRUE)
if (${ENABLE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_compile_options(-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options(-fcolor-diagnostics)
    endif()
endif()

add_library(${PROJECT_NAME} SHARED
        ${${PROJECT_NAME}_HEADERS}
        ${${PROJECT_NAME}_SOURCES}
        ${VERSION_HEADER_DIR}/version.h
)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)

target_include_directories(${PROJECT_NAME} PRIVATE ${VERSION_HEADER_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/game-host-adapter)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ext)

#find_library(ONE_ARCUS_LIB one_arcus)
#target_link_libraries(${PROJECT_NAME} PRIVATE ${ONE_ARCUS_LIB})

find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC OpenSSL::SSL OpenSSL::Crypto)
find_package(Boost REQUIRED COMPONENTS date_time regex system thread atomic filesystem url json program_options)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::url Boost::json)
find_package(certify CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC certify::core)
find_package(prometheus-cpp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE prometheus-cpp::core prometheus-cpp::pull)

target_compile_definitions(${PROJECT_NAME} PRIVATE "$<IF:$<CONFIG:Debug>,-DDEFAULT_LOG_LEVEL=RH_LOG_LEVEL_TRACE,-DDEFAULT_LOG_LEVEL=RH_LOG_LEVEL_ERROR>")

#target_compile_options(${PROJECT_NAME} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-gdwarf-4>")
#target_link_options(${PROJECT_NAME} PRIVATE "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-gdwarf-4>")

#target_link_options(${PROJECT_NAME} PRIVATE "-Wl,--exclude-libs,ALL")

target_compile_definitions(${PROJECT_NAME} PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/DWIN32_LEAN_AND_MEAN=1>")
target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/bigobj>")

if (UNIX)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND objcopy --only-keep-debug $<TARGET_FILE:${PROJECT_NAME}> $<PATH:REPLACE_EXTENSION,LAST_ONLY,$<TARGET_FILE:${PROJECT_NAME}>,sym>)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND objcopy --strip-debug $<TARGET_FILE:${PROJECT_NAME}>)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND objcopy --add-gnu-debuglink=$<PATH:REPLACE_EXTENSION,LAST_ONLY,$<TARGET_FILE:${PROJECT_NAME}>,sym> $<TARGET_FILE:${PROJECT_NAME}>)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND mono ${NUGET_EXE} pack ${CMAKE_CURRENT_BINARY_DIR}/${NUSPEC_NAME}.nuspec -Version ${GAME_HOST_ADAPTER_VERSION} -OutputDirectory ${CMAKE_CURRENT_BINARY_DIR}
            BYPRODUCTS RallyHere.GameHostAdapter.${NUPKG_OS}.${GAME_HOST_ADAPTER_VERSION}.nupkg)
else()
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${NUGET_EXE} pack ${CMAKE_CURRENT_BINARY_DIR}/game-host-adapter-windows-x64-$<CONFIG>.nuspec -Version ${GAME_HOST_ADAPTER_VERSION} -OutputDirectory $<TARGET_FILE_DIR:${PROJECT_NAME}>
            BYPRODUCTS RallyHere.GameHostAdapter.Windows.x64.${GAME_HOST_ADAPTER_VERSION}.nupkg)
endif()

find_path(LEST_INCLUDE_DIRS "lest/lest.hpp")

if (LEST_INCLUDE_DIRS)
    include(CTest)
    set(shared_sources
            test/shared/configuration.h
            test/shared/configuration.cpp
            test/shared/profile_reader.hpp
    )
    set(c_interface_test_sources
            test/c_interface/main.cpp
            test/c_interface/test_sic.h
            test/c_interface/test_sic.cpp
            test/c_interface/test_a2s.h
            test/c_interface/test_a2s.cpp
            test/c_interface/test_multiplay.h
            test/c_interface/test_multiplay.cpp
            test/c_interface/test_multiplay_a2s.cpp
            test/c_interface/shared_test_data.h
            test/c_interface/shared_test_data.cpp
            test/c_interface/test_metrics.h
            test/c_interface/test_metrics.cpp
            test/c_interface/test_statsd_metrics.h
            test/c_interface/test_statsd_metrics.cpp
            )
    add_executable(${PROJECT_NAME}_test
            ${c_interface_test_sources}
            ${shared_sources}
            )
    target_include_directories(${PROJECT_NAME}_test PRIVATE ${LEST_INCLUDE_DIRS})
    target_include_directories(${PROJECT_NAME}_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/game-host-adapter)
    target_include_directories(${PROJECT_NAME}_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/test-helpers)
    target_include_directories(${PROJECT_NAME}_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/test/shared)
    target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME})
    target_link_libraries(${PROJECT_NAME}_test PRIVATE ${Boost_LIBRARIES})
    target_link_libraries(${PROJECT_NAME}_test PRIVATE Boost::thread Boost::url Boost::program_options Boost::json)
    target_link_libraries(${PROJECT_NAME}_test PRIVATE fmt::fmt)
    set_target_properties(${PROJECT_NAME}_test PROPERTIES EXCLUDE_FROM_ALL 0 EXCLUDE_FROM_DEFAULT_BUILD 1)
    add_test(NAME c_interface_test_demo_cred_file_user_pass COMMAND ${PROJECT_NAME}_test --generate-cred-file --rallyhereenv=Demo)
    add_test(NAME c_interface_test_demo_cred_file_clientid_secret COMMAND ${PROJECT_NAME}_test --generate-cred-file --rallyhereenv=Demo --use-clientid-secret)
    #add_test(NAME c_interface_test_demo_user_pass COMMAND ${PROJECT_NAME}_test --use-user-pass --rallyhereenv=Demo)
    #add_test(NAME c_interface_test_demo_clientid_secret COMMAND ${PROJECT_NAME}_test --use-clientid-secret --rallyhereenv=Demo)
    add_test(NAME c_interface_test_dev_cred_file_user_pass COMMAND ${PROJECT_NAME}_test --generate-cred-file --rallyhereenv=Dev)
    add_test(NAME c_interface_test_dev_cred_file_clientid_secret COMMAND ${PROJECT_NAME}_test --generate-cred-file --rallyhereenv=Dev --use-clientid-secret)
    #add_test(NAME c_interface_test_dev_user_pass COMMAND ${PROJECT_NAME}_test --use-user-pass --rallyhereenv=Dev)
    #add_test(NAME c_interface_test_dev_clientid_secret COMMAND ${PROJECT_NAME}_test --use-clientid-secret --rallyhereenv=Dev)

    set(complete_test_sources
            test/complete/main.cpp
            test/complete/test_sic.h
            test/complete/test_sic.cpp
            test/complete/test_a2s.h
            test/complete/test_a2s.cpp
            test/complete/test_multiplay_a2s.cpp
            test/complete/shared_test_data.h
            test/complete/shared_test_data.cpp
            test/complete/test_sdk_helpers.cpp
            test/complete/test_base64.cpp
            test/complete/test_base64.h
    )
    add_executable(${PROJECT_NAME}_complete_test
            ${complete_test_sources}
            ${shared_sources}
    )
    target_include_directories(${PROJECT_NAME}_complete_test PRIVATE ${LEST_INCLUDE_DIRS})
    target_include_directories(${PROJECT_NAME}_complete_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/game-host-adapter)
    target_include_directories(${PROJECT_NAME}_complete_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/complete)
    target_include_directories(${PROJECT_NAME}_complete_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ext)
    target_include_directories(${PROJECT_NAME}_complete_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/test/shared)
    target_link_libraries(${PROJECT_NAME}_complete_test PRIVATE ${PROJECT_NAME})
    target_link_libraries(${PROJECT_NAME}_complete_test PRIVATE ${Boost_LIBRARIES})
    target_link_libraries(${PROJECT_NAME}_complete_test PRIVATE Boost::thread Boost::url Boost::program_options Boost::json)
    target_link_libraries(${PROJECT_NAME}_complete_test PRIVATE fmt::fmt)
    set_target_properties(${PROJECT_NAME}_complete_test PROPERTIES EXCLUDE_FROM_ALL 0 EXCLUDE_FROM_DEFAULT_BUILD 1)
    add_test(NAME complete_test_demo_cred_file_user_pass COMMAND ${PROJECT_NAME}_complete_test --generate-cred-file --rallyhereenv=Demo)
    add_test(NAME complete_test_demo_cred_file_clientid_secret COMMAND ${PROJECT_NAME}_complete_test --generate-cred-file --rallyhereenv=Demo --use-clientid-secret)
    #add_test(NAME complete_test_demo_user_pass COMMAND ${PROJECT_NAME}_complete_test --use-user-pass --rallyhereenv=Demo)
    #add_test(NAME complete_test_demo_clientid_secret COMMAND ${PROJECT_NAME}_complete_test --use-clientid-secret --rallyhereenv=Demo)
    add_test(NAME complete_test_dev_cred_file_user_pass COMMAND ${PROJECT_NAME}_complete_test --generate-cred-file --rallyhereenv=Dev)
    add_test(NAME complete_test_dev_cred_file_clientid_secret COMMAND ${PROJECT_NAME}_complete_test --generate-cred-file --rallyhereenv=Dev --use-clientid-secret)
    #add_test(NAME complete_test_dev_user_pass COMMAND ${PROJECT_NAME}_complete_test --use-user-pass --rallyhereenv=Dev)
    #add_test(NAME complete_test_dev_clientid_secret COMMAND ${PROJECT_NAME}_complete_test --use-clientid-secret --rallyhereenv=Dev)
endif()

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}-targets
        ARCHIVE
        OPTIONAL)