CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

PROJECT ("Remote")

#
# Variables
#

SET (REMOTE_BUNDLES_PATH ${CMAKE_SOURCE_DIR}/Bin)
SET (REMOTE_CLIENT_BUNDLE_PATH ${REMOTE_BUNDLES_PATH}/Client)
SET (REMOTE_SERVER_BUNDLE_PATH ${REMOTE_BUNDLES_PATH}/Server)

#
# CMake config
#

SET (
	CMAKE_MODULE_PATH
	${CMAKE_SOURCE_DIR}/CMakeModules ${CMAKE_MODULE_PATH}
)

#
# Dependencies
#

set(Boost_USE_STATIC_LIBS        ON)
find_package(Boost 1.40.0 REQUIRED COMPONENTS system date_time regex)
find_package(LibVLC REQUIRED)
find_package(SFML COMPONENTS system window graphics REQUIRED)

#
# Include Directories
#

INCLUDE_DIRECTORIES (
    ${CMAKE_SOURCE_DIR}/Client/src
    ${CMAKE_SOURCE_DIR}/Server/src
    ${Boost_INCLUDE_DIR}
    ${LIBVLC_INCLUDE_DIR}
    ${SFML_INCLUDE_DIR}
)

#
# Sources
#


# Server
FILE (
    GLOB_RECURSE
    SERVER_SOURCE
    ${CMAKE_SOURCE_DIR}/Server/src/*
)

# Client
FILE (
    GLOB_RECURSE
    CLIENT_SOURCE
    ${CMAKE_SOURCE_DIR}/Client/src/*
)

#
# Exec
#

# Client
ADD_EXECUTABLE(
    Client
    MACOSX_BUNDLE
    ${CLIENT_SOURCE}
)

# Server
ADD_EXECUTABLE(
    Server
    ${SERVER_SOURCE}
)

#
# Group source files
#

FILE (
    GLOB
    SOURCE_FOLDERS
    ${CMAKE_SOURCE_DIR}/Client/src/*
    ${CMAKE_SOURCE_DIR}/Server/src/*
)

FOREACH (FOLDER ${SOURCE_FOLDERS})

    IF (IS_DIRECTORY ${FOLDER})

        # Get directory name
        GET_FILENAME_COMPONENT (
            GROUP_NAME
            ${FOLDER}
            NAME
        )

        # Get source files in directories
        FILE (
            GLOB_RECURSE
            GROUP_SOURCE
            ${FOLDER}/*
        )

        SOURCE_GROUP (
            ${GROUP_NAME}
            FILES
            ${GROUP_SOURCE}
        )

    ENDIF ()

ENDFOREACH ()

#
# Library linking
#

TARGET_LINK_LIBRARIES (
    Client
    ${Boost_LIBRARIES}
    ${LIBVLC_LIBRARY}
    ${SFML_LIBRARIES}
)


TARGET_LINK_LIBRARIES (
    Server
    ${Boost_LIBRARIES}
)

#
# Setup bundles
#

# Client bundle

SET_PROPERTY (
    TARGET Client
    PROPERTY RUNTIME_OUTPUT_DIRECTORY ${REMOTE_CLIENT_BUNDLE_PATH}
)

SET_PROPERTY (
    TARGET Client
    PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${REMOTE_CLIENT_BUNDLE_PATH}
)

SET_PROPERTY (
    TARGET Client
    PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${REMOTE_CLIENT_BUNDLE_PATH}
)

IF (APPLE)
# Mac Bundle (.app)

SET_PROPERTY (
    TARGET Client
    PROPERTY MACOSX_BUNDLE_ICON_FILE icon.icns
)

ENDIF (APPLE)

# Server bundle

SET_PROPERTY (
    TARGET Server
    PROPERTY RUNTIME_OUTPUT_DIRECTORY ${REMOTE_SERVER_BUNDLE_PATH}
)

SET_PROPERTY (
    TARGET Server
    PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${REMOTE_SERVER_BUNDLE_PATH}
)

SET_PROPERTY (
    TARGET Server
    PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${REMOTE_SERVER_BUNDLE_PATH}
)


