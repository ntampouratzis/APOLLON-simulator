# - Config file for the CERTI package
# It defines the following variables
#  CERTI_INCLUDE_DIRS          - include directories for CERTI
#  CERTI_LIBRARIES             - libraries to link against
#  CERTI_RTIA_EXECUTABLE       - the converter executable
#  CERTI_RTIG_EXECUTABLE       - the tracer executable
 
# Compute paths
get_filename_component(CERTI_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
if(EXISTS "${CERTI_CMAKE_DIR}/CMakeCache.txt")
   # In build tree
   include("${CERTI_CMAKE_DIR}/CERTIBuildTreeSettings.cmake")
else()
   set(CERTI_INCLUDE_DIRS "${CERTI_CMAKE_DIR}/../../include")
endif()
 
# Our library dependencies (contains definitions for IMPORTED targets)
include("${CERTI_CMAKE_DIR}/CERTIDepends.cmake")
 
# These are IMPORTED targets created by CERTIDepends.cmake
set(CERTI_LIBRARIES CERTI RTI HLA)
set(CERTI_RTIA_EXECUTABLE rtia)
set(CERTI_RTIG_EXECUTABLE rtig)
