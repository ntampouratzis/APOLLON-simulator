#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "HLA" for configuration "Debug"
set_property(TARGET HLA APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(HLA PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "/usr/lib/x86_64-linux-gnu/librt.so"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libHLAd.so.4.0.0"
  IMPORTED_SONAME_DEBUG "libHLAd.so.4"
  )

list(APPEND _IMPORT_CHECK_TARGETS HLA )
list(APPEND _IMPORT_CHECK_FILES_FOR_HLA "${_IMPORT_PREFIX}/lib/libHLAd.so.4.0.0" )

# Import target "CERTI" for configuration "Debug"
set_property(TARGET CERTI APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(CERTI PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "/usr/lib/x86_64-linux-gnu/libxml2.so;HLA"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libCERTId.so.4.0.0"
  IMPORTED_SONAME_DEBUG "libCERTId.so.4"
  )

list(APPEND _IMPORT_CHECK_TARGETS CERTI )
list(APPEND _IMPORT_CHECK_FILES_FOR_CERTI "${_IMPORT_PREFIX}/lib/libCERTId.so.4.0.0" )

# Import target "rtig" for configuration "Debug"
set_property(TARGET rtig APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(rtig PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/rtig"
  )

list(APPEND _IMPORT_CHECK_TARGETS rtig )
list(APPEND _IMPORT_CHECK_FILES_FOR_rtig "${_IMPORT_PREFIX}/bin/rtig" )

# Import target "rtia" for configuration "Debug"
set_property(TARGET rtia APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(rtia PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/rtia"
  )

list(APPEND _IMPORT_CHECK_TARGETS rtia )
list(APPEND _IMPORT_CHECK_FILES_FOR_rtia "${_IMPORT_PREFIX}/bin/rtia" )

# Import target "RTI" for configuration "Debug"
set_property(TARGET RTI APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(RTI PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "CERTI;FedTime"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libRTI-NGd.so.1.0.0"
  IMPORTED_SONAME_DEBUG "libRTI-NGd.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS RTI )
list(APPEND _IMPORT_CHECK_FILES_FOR_RTI "${_IMPORT_PREFIX}/lib/libRTI-NGd.so.1.0.0" )

# Import target "FedTime" for configuration "Debug"
set_property(TARGET FedTime APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(FedTime PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libFedTimed.so.1.0.0"
  IMPORTED_SONAME_DEBUG "libFedTimed.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS FedTime )
list(APPEND _IMPORT_CHECK_FILES_FOR_FedTime "${_IMPORT_PREFIX}/lib/libFedTimed.so.1.0.0" )

# Import target "RTI1516" for configuration "Debug"
set_property(TARGET RTI1516 APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(RTI1516 PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "CERTI;FedTime1516"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libRTI1516d.so.1.0.0"
  IMPORTED_SONAME_DEBUG "libRTI1516d.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS RTI1516 )
list(APPEND _IMPORT_CHECK_FILES_FOR_RTI1516 "${_IMPORT_PREFIX}/lib/libRTI1516d.so.1.0.0" )

# Import target "FedTime1516" for configuration "Debug"
set_property(TARGET FedTime1516 APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(FedTime1516 PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libFedTime1516d.so.1.0.0"
  IMPORTED_SONAME_DEBUG "libFedTime1516d.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS FedTime1516 )
list(APPEND _IMPORT_CHECK_FILES_FOR_FedTime1516 "${_IMPORT_PREFIX}/lib/libFedTime1516d.so.1.0.0" )

# Import target "RTI1516e" for configuration "Debug"
set_property(TARGET RTI1516e APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(RTI1516e PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "CERTI;FedTime1516e;HLA"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libRTI1516ed.so.1.0.0"
  IMPORTED_SONAME_DEBUG "libRTI1516ed.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS RTI1516e )
list(APPEND _IMPORT_CHECK_FILES_FOR_RTI1516e "${_IMPORT_PREFIX}/lib/libRTI1516ed.so.1.0.0" )

# Import target "FedTime1516e" for configuration "Debug"
set_property(TARGET FedTime1516e APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(FedTime1516e PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libFedTime1516ed.so.1.0.0"
  IMPORTED_SONAME_DEBUG "libFedTime1516ed.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS FedTime1516e )
list(APPEND _IMPORT_CHECK_FILES_FOR_FedTime1516e "${_IMPORT_PREFIX}/lib/libFedTime1516ed.so.1.0.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
