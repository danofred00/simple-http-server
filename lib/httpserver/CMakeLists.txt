
set(SOURCES_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/httpserver.cpp)

set(HEADERS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/httpserver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/helpers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/file.h)

add_library(httpserver SHARED ${SOURCES_FILES} ${HEADERS_FILES})

target_include_directories(httpserver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(httpserver Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::HttpServer)

qt_add_resources(httpserver "app_templates"
    PREFIX "/"
    FILES
        template/index.html
        template/error-404.html
)

install(TARGETS httpserver
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
