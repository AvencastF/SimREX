include_directories(include)

file(GLOB_RECURSE HEADERS "include/*.h")
file(GLOB_RECURSE SOURCES "src/*.cpp")

root_generate_dictionary(G__GEM ${HEADERS} LINKDEF include/global-event-model/GEMLinkDef.h)
add_library(GEM SHARED ${HEADERS} ${SOURCES} G__GEM.cxx)

install(TARGETS GEM LIBRARY DESTINATION lib )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libGEM_rdict.pcm DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libGEM.rootmap DESTINATION lib)
