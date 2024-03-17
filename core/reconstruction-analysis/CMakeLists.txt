# include global-event-model
include_directories(${PROJECT_SOURCE_DIR}/core/global-event-model/include)

# include simulation headers
include_directories(${PROJECT_SOURCE_DIR}/core/reconstruction-analysis/include)

file(GLOB_RECURSE rec_source "src/**/*.cpp")
file(GLOB_RECURSE rec_header "include/**/*.h")

add_library(RecoAndAna SHARED ${rec_source} ${rec_header} src/reconstruction.cpp include/reconstruction.h)
target_link_libraries(RecoAndAna PUBLIC GEM)

install(TARGETS RecoAndAna LIBRARY DESTINATION lib)
