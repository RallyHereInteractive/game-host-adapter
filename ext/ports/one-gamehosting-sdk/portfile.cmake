vcpkg_from_github(
        OUT_SOURCE_PATH SOURCE_PATH
        REPO i3D-net/ONE-GameHosting-SDK
        REF aa1e1e402a0ec251283221c20fbf04a468e23a7a
        SHA512 21ca06aa171758d226493714331e9b251c96c49ac014e6da7294cc4275ce6c01cc9339a1dc889eaafc09573fb121607ac90fc491504b928257012c2d8ee0e325
        HEAD_REF master
        PATCHES 0001-Remove-testing-from-default-build.patch 0002-Add-install-targets-for-Arcus.patch 0003-Use-std-size_t.patch 0004-Include-cstdlib-in-the-allocator.patch 0005-Fix-ping-s-allocator.patch 0006-Install-public-header-files.patch
)

string(COMPARE NOTEQUAL "${VCPKG_LIBRARY_LINKAGE}" "static" SHARED_ARCUS_LIB)

vcpkg_configure_cmake(
        SOURCE_PATH "${SOURCE_PATH}"
        PREFER_NINJA
)
vcpkg_install_cmake()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

file(
        INSTALL "${SOURCE_PATH}/LICENSE.TXT"
        DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}"
        RENAME copyright
)