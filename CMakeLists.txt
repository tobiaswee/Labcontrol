cmake_minimum_required(VERSION 3.12.3 FATAL_ERROR)
project(Labcontrol CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 14)

find_package(Qt5 COMPONENTS Core Network Widgets REQUIRED)
set(labcontrol_srcs
    src/localzleafstarter.cpp
    src/main.cpp
    src/mainwindow.cpp
    src/manualprintingsetup.cpp
    src/Lib/client.cpp
    src/Lib/clientpinger.cpp
    src/Lib/netstatagent.cpp
    src/Lib/receiptsprinter.cpp
    src/Lib/sessionsmodel.cpp
    src/Lib/ztree.cpp
    src/Lib/clienthelpnotificationserver.cpp
    src/Lib/lablib.cpp
    src/Lib/receipts_handler.cpp
    src/Lib/session.cpp
    src/Lib/settings.cpp)
add_executable(labcontrol ${labcontrol_srcs})
target_link_libraries(labcontrol Qt5::Core Qt5::Network Qt5::Widgets)
