# Add Cmake
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/AMAS_Cmake_Loader.cpp")

# Add Script
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/AMAS.cpp")
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/AMAS_Config.cpp")
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/AMAS_Random_Ban.cpp")
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/AMAS_Commands.cpp")
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/AMAS_Script.cpp")

# Add script loader
AC_ADD_SCRIPT_LOADER("AMAS" "${CMAKE_CURRENT_LIST_DIR}/src/loader.h")

# Add custom config
AC_ADD_CONFIG_FILE("${CMAKE_CURRENT_LIST_DIR}/conf/AMAS.conf.dist")
