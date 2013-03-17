
find_package(Gear2D REQUIRED)

add_library(collider MODULE collider.cc)

target_link_libraries(${Gear2D_LIBRARY})

install(TARGETS collider LIBRARY DESTINATION collider)
