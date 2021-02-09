# -*- sh -*-
# UjoImro, 2010

# ----------------------------------------------
# -------- Documentation -----------------------
# ----------------------------------------------


# This file generates the documentation of PInK the best it can


# check if doxygen is even installed
find_package(Doxygen)

if (DOXYGEN_FOUND STREQUAL "NO")
    message("No doxygen package found.")
endif (DOXYGEN_FOUND STREQUAL "NO")


# prepare doxygen configuration file
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/pink.dox ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile )

# add doxygen as target
add_custom_target(doxygen ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

# cleanup $build/doc on "make clean"
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES doc)

# add doxygen as dependency to pink-target
add_dependencies(pink doxygen)

# install HTML API documentation and manual pages
#set(DOC_PATH "share/doc/${CPACK_PACKAGE_NAME}-${VERSION}")

#install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/api-doc/html
#         DESTINATION ${DOC_PATH}
#       )

# install man pages into packages, scope is now project root..
#install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/api-doc/man/man3
#         DESTINATION share/man/man3/
#       )





























# LuM end of file
