set(SCOPES_BIN_DIR ${SCOPESLIB_LIBDIR})

include_directories(${SCOPESLIB_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SCOPE_SOURCES
    mock-scope-ttl.cpp
    )

add_library(mock-scope-ttl MODULE ${SCOPE_SOURCES})
target_link_libraries(mock-scope-ttl ${SCOPESLIB_LDFLAGS})

configure_file(mock-scope-ttl.ini.in mock-scope-ttl.ini)
