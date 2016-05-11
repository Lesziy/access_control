cmake_minimum_required(VERSION 3.5)
project(access_control)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "~/ClionProjects/access_control/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES
    applicationServer/AuthenticationProtocol.cpp
    applicationServer/AuthenticationProtocol.h
    applicationServer/CommunicationProtocol.cpp
    applicationServer/CommunicationProtocol.h
    applicationServer/Connection.cpp
    applicationServer/Connection.h
    applicationServer/FirewallManager.cpp
    applicationServer/FirewallManager.h
    applicationServer/json.hpp
    applicationServer/jsonFileLoader.cpp
    applicationServer/jsonFileLoader.h
    applicationServer/Logger.cpp
    applicationServer/Logger.h
    applicationServer/main.cpp
    applicationServer/ServerApplication.cpp
    applicationServer/ServerApplication.h
    djApplication/migrations/__init__.py
    djApplication/__init__.py
    djApplication/admin.py
    djApplication/apps.py
    djApplication/models.py
    djApplication/tests.py
    djApplication/urls.py
    djApplication/views.py
    djTestProject/__init__.py
    djTestProject/settings.py
    djTestProject/urls.py
    djTestProject/wsgi.py
    static/css/index.css
    static/css/loginSite.css
    static/logoEiTI.png
    templates/index.html
    templates/reservationSite.html
    db.sqlite3
    manage.py
    README.md)

add_executable(access_control ${SOURCE_FILES})