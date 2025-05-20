cmake_minimum_required(VERSION 3.13)

enable_language(CXX)
project(rpl4)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
  include
)

file(GLOB SRCS src/*/*.cpp)
target_sources(${PROJECT_NAME} INTERFACE ${SRCS})

if(NOT DEFINED RPL4_LOG_LEVEL)
  set(RPL4_LOG_LEVEL "WARNING")
  target_compile_definitions(${PROJECT_NAME} INTERFACE RPL4_LOG_LEBVEL=LogLevel::Warning)
  message(WARNING "[RPL4 Warning]\n"
                  "  RPL4_LOG_LEVEL was not specified, so it has been automatically set to Error.\n"
                  "  Please specify a log level from the following options:\n"
                  "    [DEBUG, INFO, WARNING, ERROR, FATAL, OFF]")
endif()

if(RPL4_LOG_LEVEL STREQUAL "DEBUG")
  target_compile_definitions(${PROJECT_NAME} INTERFACE RPL4_LOG_LEVEL=LogLevel::Debug)
  message("[RPL4] RPL4_LOG_LEBVEL=DEBUG. RPL4 will output all logs.")
elseif(RPL4_LOG_LEVEL STREQUAL "INFO")
  target_compile_definitions(${PROJECT_NAME} INTERFACE RPL4_LOG_LEVEL=LogLevel::Info)
  message("[RPL4] RPL4_LOG_LEBVEL=INFO. RPL4 will output logs more critical than or equal to Info.")
elseif(RPL4_LOG_LEVEL STREQUAL "WARNING")
  target_compile_definitions(${PROJECT_NAME} INTERFACE RPL4_LOG_LEVEL=LogLevel::Warning)
  message("[RPL4] RPL4_LOG_LEBVEL=WARNING. RPL4 will output logs more critical than or equal to Warning.")
elseif(RPL4_LOG_LEVEL STREQUAL "ERROR")
  target_compile_definitions(${PROJECT_NAME} INTERFACE RPL4_LOG_LEVEL=LogLevel::Error)
  message("[RPL4] RPL4_LOG_LEBVEL=ERROR. RPL4 will output logs more critical than or equal to Error.")
elseif(RPL4_LOG_LEVEL STREQUAL "FATAL")
  target_compile_definitions(${PROJECT_NAME} INTERFACE RPL4_LOG_LEVEL=LogLevel::Fatal)
  message("[RPL4] RPL4_LOG_LEBVEL=FATAL. RPL4 will output logs only Fatal.")
elseif(RPL4_LOG_LEVEL STREQUAL "OFF")
  message("[RPL4] RPL4_LOG_LEBVEL=OFF. RPL4 will not output any logs.")
else()
  target_compile_definitions(${PROJECT_NAME} INTERFACE RPL4_LOG_LEVEL=LogLevel::Error)
  message(WARNING "[RPL4 Warning]\n"
                  "  The specified RPL4_LOG_LEVEL : ${RPL4_LOG_LEVEL} does not exists, so it has been automatically set to ERROR.\n"
                  "  Please specify a log level from the following options:\n"
                  "    [DEBUG, INFO, WARNING, ERROR, FATAL, OFF]")
endif()

if(RPL4_BUILD_EXAMPLE)
  add_subdirectory(example)
endif()