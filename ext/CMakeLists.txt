include(FetchContent)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(Findfreetype)
set_target_properties(freetype PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(idros PUBLIC "$<LINK_LIBRARY:WHOLE_ARCHIVE,freetype>")