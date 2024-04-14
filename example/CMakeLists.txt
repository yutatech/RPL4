file(GLOB EXAMPLE_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ./*.cpp)

message("[RPL4] ---- Build examples ----")

foreach(example_src ${EXAMPLE_SRCS})
  message("[RPL4]    ${example_src}")
  string(REPLACE ".cpp" "" exe_name ${example_src})
  add_executable(${exe_name} ${example_src})
  target_link_libraries(${exe_name}
    rpl4
  )
endforeach()

message("[RPL4] ------------------------")