vcpkg_from_github(
        OUT_SOURCE_PATH SOURCE_PATH
        REPO djarek/certify
        REF 97f5eebfd99a5d6e99d07e4820240994e4e59787
        SHA512 d8588fa46e1262e1af2de8930093df64b62798f546b7a0c99629a783c980a6efe8d8b7a6c95ae7a2ac2c6588e0ee5a98fc47435edc3aa6b1e55e0fb208aee427
        HEAD_REF master
        PATCHES 0001-Fix-main-CMakeLists.txt.patch 0002-Put-the-cmake-files-into-the-correct-directory-for-v.patch
)
vcpkg_configure_cmake(
        SOURCE_PATH "${SOURCE_PATH}"
        PREFER_NINJA
)
vcpkg_install_cmake()
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
vcpkg_cmake_config_fixup()

#vcpkg_build_cmake(TARGET all)
#vcpkg_install_cmake()
#vcpkg_fixup_cmake_targets(CONFIG_PATH lib/CMake/cpp-framecommon)