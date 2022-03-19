cmake_minimum_required(VERSION 3.22.2)

project(Parser)
set(BALANCE_SOURCE Balance.cpp Balance.h)
set(CONTAINERGRID_SOURCE ContainerGrid.cpp ContainerGrid.h)
set(NODE_SOURCE Node.cpp Node.h)
set(PARSER_SOURCE Parser.cpp Parser.h)
set(SOLVER_SOURCE Solver.cpp Solver.h Point.h)

add_library(Balance STATIC ${BALANCE_SOURCE} ${NODE_SOURCE})
add_library(ContainerGrid STATIC ${CONTAINERGRID_SOURCE})
add_library(Node STATIC ${NODE_SOURCE})
add_library(Parser STATIC ${PARSER_SOURCE})
add_library(Solver STATIC ${SOLVER_SOURCE})

target_link_libraries(Solver Parser Balance ContainerGrid)
