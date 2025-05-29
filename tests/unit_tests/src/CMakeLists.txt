############################################################################
#                                                                          #
#   Copyright (C) 2025 by Linlin Jia, 2018 by David B. Blumenthal                              #
#                                                                          #
#   This file is part of GEDLIB.                                           #
#                                                                          #
#   GEDLIB is free software: you can redistribute it and/or modify it      #
#   under the terms of the GNU Lesser General Public License as published  #
#   by the Free Software Foundation, either version 3 of the License, or   #
#   (at your option) any later version.                                    #
#                                                                          #
#   GEDLIB is distributed in the hope that it will be useful,              #
#   but WITHOUT ANY WARRANTY; without even the implied warranty of         #
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           #
#   GNU Lesser General Public License for more details.                    #
#                                                                          #
#   You should have received a copy of the GNU Lesser General Public       #
#   License along with GEDLIB. If not, see <http://www.gnu.org/licenses/>. #
#                                                                          #
############################################################################

# This is a separate project for unit tests of GEDLIB.

# todo: we only support ged_computation_tests for now for efficiency.

cmake_minimum_required(VERSION 3.28)
project(UnitTests)

# These have already been set in the root CMakeLists.txt, but we set it here 
# again to ensure that the unit tests are built with the same settings:
set(CMAKE_CXX_STANDARD 17)

set(PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR}/../../..)
set(LIBRARY_OUTPUT_PATH ${PROJECT_ROOT_DIR}/lib)
set(LSAPE_HOME ${PROJECT_ROOT_DIR}/ext/lsape.5)
set(NOMAD_HOME ${PROJECT_ROOT_DIR}/ext/nomad.3.8.1)
set(LIBSVM_HOME ${PROJECT_ROOT_DIR}/ext/libsvm.3.22)
set(EIGEN_HOME ${PROJECT_ROOT_DIR}/ext/eigen.3.3.4)
set(FANN_HOME ${PROJECT_ROOT_DIR}/ext/fann.2.2.0)
set(BOOST_HOME ${PROJECT_ROOT_DIR}/ext/boost.1.69.0)

# Add include and link directories:
include_directories(SYSTEM ${BOOST_HOME} ${NOMAD_HOME}/ext/sgtelib/src ${NOMAD_HOME}/src ${LSAPE_HOME}/include ${EIGEN_HOME}/Eigen ${LIBSVM_HOME} ${FANN_HOME}/include ${GUROBI_HOME}/include ${OMP_HOME}/include)
link_directories(${NOMAD_HOME}/lib ${FANN_HOME}/lib ${LIBSVM_HOME} ${GUROBI_HOME}/lib ${OMP_HOME}/lib)
link_directories(${PROJECT_ROOT_DIR}/lib)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../bin)

# add_custom_target(unit_tests DEPENDS ged_env_tests lsap_solver_tests ged_computation_tests)

# add_executable(ged_env_tests catch.cpp ged_env_test.cpp)
add_executable(ged_computation_tests ged_computation_tests.cpp)

# target_link_libraries(ged_env_tests gxlgedlib)
target_link_libraries(ged_computation_tests gxlgedlib nomad fann svm)
if(APPLE)
  add_custom_command(TARGET ged_env_tests POST_BUILD COMMAND install_name_tool -change libnomad.so ${NOMAD_HOME}/lib/libnomad.so ${EXECUTABLE_OUTPUT_PATH}/ged_env_tests)
  add_custom_command(TARGET ged_env_tests POST_BUILD COMMAND install_name_tool -change libdoublefann.2.dylib ${FANN_HOME}/lib/libdoublefann.2.dylib ${EXECUTABLE_OUTPUT_PATH}/ged_env_tests)
  add_custom_command(TARGET ged_env_tests POST_BUILD COMMAND install_name_tool -change libsvm.so ${LIBSVM_HOME}/libsvm.so ${EXECUTABLE_OUTPUT_PATH}/ged_env_tests)
endif()

# add_executable(lsap_solver_tests catch.cpp lsap_solver_test.cpp)