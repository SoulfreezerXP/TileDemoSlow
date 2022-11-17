cmake_minimum_required(VERSION 3.21)

project(TileDemoSlow VERSION 1.0
        DESCRIPTION "TileDemoSlow"
        LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(TileDemoSlow WIN32 "${CMAKE_CURRENT_SOURCE_DIR}/src/application.qrc" )

if ( MSVC )
    set_target_properties(TileDemoSlow  PROPERTIES LINK_FLAGS "/ignore:4099")
    target_compile_options(TileDemoSlow  PRIVATE "/WX")
endif()

set_target_properties(TileDemoSlow  PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)
set_target_properties(TileDemoSlow  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
  set_target_properties(TileDemoSlow PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL OpenGLWidgets Widgets)
qt_standard_project_setup()
target_link_libraries(TileDemoSlow  PRIVATE Qt6::Core)
target_link_libraries(TileDemoSlow  PRIVATE Qt6::Gui)
target_link_libraries(TileDemoSlow  PRIVATE Qt6::OpenGL)
target_link_libraries(TileDemoSlow  PRIVATE Qt6::OpenGLWidgets)
target_link_libraries(TileDemoSlow  PRIVATE Qt6::Widgets)

target_include_directories(TileDemoSlow  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_sources(TileDemoSlow  PRIVATE
  #Header
  "${CMAKE_CURRENT_SOURCE_DIR}/include/main_window.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/pixmap_atlas.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/render_tile.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/tile.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/tile_map.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/tile_map_layer.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/tile_map_scene.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/tile_map_view.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/tile_palette.h"

  #Souces
  "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/main_window.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/pixmap_atlas.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/render_tile.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/tile.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/tile_map.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/tile_map_layer.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/tile_map_scene.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/tile_map_view.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/tile_palette.cpp"

)
