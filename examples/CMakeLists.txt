cmake_minimum_required(VERSION 2.8.9)

project(grantlee_examples)

include(ExternalProject)

ExternalProject_Add(books
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/books
  INSTALL_COMMAND ""
)
ExternalProject_Add(codegen
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/codegen
  INSTALL_COMMAND ""
  DEPENDS books
)

ExternalProject_Add(contacts
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/contacts
  INSTALL_COMMAND ""
  DEPENDS codegen
)

ExternalProject_Add(htmlapps
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/htmlapps
  INSTALL_COMMAND ""
  DEPENDS contacts
)

ExternalProject_Add(textedit
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/textedit
  INSTALL_COMMAND ""
  DEPENDS htmlapps
)
