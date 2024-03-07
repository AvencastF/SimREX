include_directories(include)

file(GLOB_RECURSE HEADERS "include/*.h")
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(GEM SHARED ${HEADERS} ${SOURCES} GEM_dict.cxx)
root_generate_dictionary(GEM_dict ${HEADERS} LINKDEF GEMLinkDef.h)

install(TARGETS GEM LIBRARY DESTINATION lib )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libGEM_rdict.pcm DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libGEM.rootmap DESTINATION lib)
