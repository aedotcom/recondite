set(project_files)

list(APPEND project_files ${CMAKE_CURRENT_SOURCE_DIR}/reProject.hpp)
list(APPEND project_files ${CMAKE_CURRENT_SOURCE_DIR}/reProject.cpp)

list(APPEND project_files ${CMAKE_CURRENT_SOURCE_DIR}/reComponent.hpp)
list(APPEND project_files ${CMAKE_CURRENT_SOURCE_DIR}/reComponent.cpp)

list(APPEND project_files ${CMAKE_CURRENT_SOURCE_DIR}/reSelectionManager.hpp)
list(APPEND project_files ${CMAKE_CURRENT_SOURCE_DIR}/reSelectionManager.cpp)

set(project_sources ${project_files} PARENT_SCOPE)