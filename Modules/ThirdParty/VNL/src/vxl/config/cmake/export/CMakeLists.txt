# This CMakeLists.txt file handles the creation of files needed by
# other client projects that use VXL.  Nothing is built by this
# CMakeLists.txt file.  This CMakeLists.txt file must be processed by
# CMake after all the other CMakeLists.txt files in the VXL tree,
# which is why the SUBDIRS(config/cmake/export) command is at the end
# of the top level CMakeLists.txt file.

# Needed to get non-cached variable settings used in VXLConfig.cmake.in
INCLUDE( ${MODULE_PATH}/FindNetlib.cmake )
INCLUDE( ${MODULE_PATH}/FindQv.cmake )
INCLUDE( ${MODULE_PATH}/FindZLIB.cmake )
INCLUDE( ${MODULE_PATH}/FindPNG.cmake )
INCLUDE( ${MODULE_PATH}/FindJPEG.cmake )
INCLUDE( ${MODULE_PATH}/FindTIFF.cmake )
INCLUDE( ${MODULE_PATH}/FindGEOTIFF.cmake )
INCLUDE( ${MODULE_PATH}/FindMPEG2.cmake )
SET( EXPAT_FIND_QUIETLY "YES" )
INCLUDE( ${vxl_SOURCE_DIR}/contrib/brl/bmods/FindEXPAT.cmake )
SET( EXPAT_FIND_QUIETLY )

# Save the compiler settings so another project can import them.
INCLUDE(${CMAKE_ROOT}/Modules/CMakeExportBuildSettings.cmake)
SET(VXL_BUILD_SETTINGS_FILE ${vxl_BINARY_DIR}/VXLBuildSettings.cmake)
SET(VXL_EXPORT_BUILD_SETTINGS_FILE 
  ${CMAKE_INSTALL_PREFIX}/share/vxl/cmake/VXLBuildSettings.cmake
)
CMAKE_EXPORT_BUILD_SETTINGS(${VXL_BUILD_SETTINGS_FILE})

# Save library dependencies.
SET(VXL_CMAKE_DOXYGEN_DIR  ${vxl_SOURCE_DIR}/config/cmake/doxygen)
SET(VXL_LIBRARY_DEPENDS_FILE ${vxl_BINARY_DIR}/VXLLibraryDepends.cmake)
SET(VXL_INSTALL_LIBRARY_DEPENDS_FILE 
  ${CMAKE_INSTALL_PREFIX}/share/vxl/cmake/VXLLibraryDepends.cmake
)
EXPORT_LIBRARY_DEPENDENCIES(${VXL_LIBRARY_DEPENDS_FILE})

# Create the VXLConfig.cmake file for the build tree.
CONFIGURE_FILE(${VXL_CMAKE_DIR}/VXLConfig.cmake.in
               ${vxl_BINARY_DIR}/VXLConfig.cmake @ONLY IMMEDIATE)

CONFIGURE_FILE(${VXL_CMAKE_DIR}/VXLConfig_export.cmake.in
               ${vxl_BINARY_DIR}/config/cmake/export/VXLConfig.cmake
               @ONLY IMMEDIATE)

INSTALL_FILES(/share/vxl/cmake FILES 
  ${vxl_BINARY_DIR}/config/cmake/export/VXLConfig.cmake
  ${vxl_BINARY_DIR}/VXLBuildSettings.cmake
  ${vxl_BINARY_DIR}/VXLLibraryDepends.cmake
  ${VXL_CMAKE_DIR}/VXLStandardOptions.cmake
  ${VXL_CMAKE_DIR}/UseVXL.cmake
  ${VXL_CMAKE_DIR}/UseVGUI.cmake
)
