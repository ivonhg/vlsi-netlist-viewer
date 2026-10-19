# -*- explicit-buffer-name: "CMakeLists.txt<M1-MOBJ/8-10>" -*-
#
# To display executed commands, use:
#     > cmake -DCMAKE_VERBOSE_MAKEFILE:STRING=YES ../src

 cmake_minimum_required(VERSION 2.8.0)
 project(SCHEMATIC)
 
 set (CMAKE_CXX_FLAGS            "-Wall -g" CACHE STRING "C++ Compiler Release options." FORCE)
 add_compile_options(-include QLineEdit)
 set (CMAKE_INSTALL_PREFIX       "../install" )

 find_package(LibXml2 REQUIRED)
 find_package(Qt5 REQUIRED Widgets Core)
#  include(${QT5_USE_FILE})

    include_directories( ${SCHEMATIC_SOURCE_DIR}
                         ${LIBXML2_INCLUDE_DIR}
                       )

                    set( includes     Indentation.h
                                      xmlUtil.h 
                                      Point.h
                                      Box.h
                                      Term.h
                                      Net.h
                                      Instance.h
                                      Node.h
                                      Line.h
                                      Shape.h
                                      EllipseShape.h 
                                      LineShape.h 
                                      ArcShape.h 
                                      BoxShape.h 
                                      TermShape.h
                                      Symbol.h
                                      Cell.h
                       )

                    set( mocIncludes  SaveCellDialog.h
                                      OpenCellDialog.h
                                      CellWidget.h
                                      CellViewer.h
                                      CellsLib.h
                                      CellsModel.h 
                                      InstancesWidget.h
                                      InstancesModel.h
                       )

                    set( cpps         Indentation.cpp
                                      xmlUtil.cpp 
                                      Point.cpp
                                      Box.cpp
                                      Term.cpp
                                      Net.cpp
                                      Instance.cpp
                                      Node.cpp
                                      Line.cpp
                                      Shape.cpp
                                      EllipseShape.cpp 
                                      LineShape.cpp 
                                      ArcShape.cpp 
                                      BoxShape.cpp 
                                      TermShape.cpp                                       
                                      Symbol.cpp
                                      Cell.cpp
                                      SaveCellDialog.cpp
                                      OpenCellDialog.cpp
                                      CellWidget.cpp
                                      CellViewer.cpp
                                      CellsLib.cpp
                                      CellsModel.cpp
                                      InstancesWidget.cpp
                                      InstancesModel.cpp
                                      Main.cpp
                       )

           qt5_wrap_cpp( mocCpps      ${mocIncludes} )

         add_executable( tme810       ${cpps} ${mocCpps} )
  target_link_libraries( tme810       Qt5::Widgets Qt5::Core ${LIBXML2_LIBRARIES} )
                install( TARGETS      tme810                      DESTINATION ../work )
                install( FILES        ${includes} ${mocIncludes}  DESTINATION include )