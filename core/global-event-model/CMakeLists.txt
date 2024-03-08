include_directories(include)

file(GLOB_RECURSE HEADERS "include/*.h")
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(GEM SHARED ${HEADERS} ${SOURCES} GEM_dict.cxx)
root_generate_dictionary(GEM_dict ${HEADERS} LINKDEF GEMLinkDef.h)

install(TARGETS GEM LIBRARY DESTINATION lib )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libGEM_rdict.pcm DESTINATION lib)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libGEM.rootmap DESTINATION lib)

###########
# Testing #
###########
# Enable testing with CTest
enable_testing()

add_executable(PhysicsDefTest tests/physics_process_def_test.cpp)
target_link_libraries(PhysicsDefTest GEM)
add_test(NAME PhysicsDefTest COMMAND PhysicsDefTest)


add_executable(EventTest tests/event_test.cpp)
target_link_libraries(EventTest PRIVATE GEM)
add_test(NAME test_event COMMAND test_event)