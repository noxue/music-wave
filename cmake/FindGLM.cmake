cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
cmake_policy(VERSION 3.2)

set(_IMPORT_PREFIX "D:/libs/glm") 

set(GLM_VERSION 0.9.9)

if (_IMPORT_PREFIX STREQUAL "/")
  set(_IMPORT_PREFIX "")
endif()

# Set the old GLM_INCLUDE_DIRS variable for backwards compatibility
set(GLM_INCLUDE_DIRS ${_IMPORT_PREFIX})

add_library(glm::glm INTERFACE IMPORTED)
set_target_properties(glm::glm PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${GLM_INCLUDE_DIRS})

mark_as_advanced(glm_DIR)
set(_IMPORT_PREFIX)

