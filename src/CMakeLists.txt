
add_executable(Http_Server_CLI
  cli.cpp constants.h version.h
)

add_executable(Http_Server
  main.cpp constants.h version.h
  mainwindow.h mainwindow.cpp mainwindow.ui
  aboutdialog.h aboutdialog.cpp aboutdialog.ui
)


target_link_libraries(
    Http_Server_CLI
    Qt${QT_VERSION_MAJOR}::Core httpserver)

target_link_libraries(
    Http_Server
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network)

install(TARGETS Http_Server Http_Server_CLI
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
