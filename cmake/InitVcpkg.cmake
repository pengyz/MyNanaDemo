# setup vcpkg
if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/../third_party/vcpkg")
  message(FATAL_ERROR "vcpkg submodule not found, plese call git submodule update --init --recursive at first !")
endif(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/../third_party/vcpkg")

if(NOT CMAKE_TOOLCHAIN_FILE)
	message("using default cmake toolchain file")
	set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/../third_party/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE INTERNAL "")
	
	set(VCPKG_MANIFEST_MODE ON)
	set(VCPKG_MANIFEST_INSTALL ON)
	set(VCPKG_MANIFEST_DIR "${CMAKE_CURRENT_LIST_DIR}/.." CACHE INTERNAL "")

	set(VCPKG_FEATURE_FLAGS "manifests,binarycaching" CACHE INTERNAL "")
	# set(VCPKG_BINARY_SOURCES "files,J:/ot_ngl_release/vcpkg_cache/archieves,readwrite" CACHE INTERNAL "vcpkg binary caching directory")
	# set(VCPKG_DOWNLOADS "J:/ot_ngl_release/vcpkg_cache/downloads" CACHE INTERNAL "vcpkg download directory")
	# set(VCPKG_OVERLAY_PORTS ${CMAKE_CURRENT_LIST_DIR}/../third_party/vcpkg-custom-ports CACHE INTERNAL "custom ports directory")
endif()

set(VCPKG_TARGET_TRIPLET x86-windows-static CACHE INTERNAL "triple as static")
