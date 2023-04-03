cmake_minimum_required(VERSION 3.8)
project(test_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rclcpp_components REQUIRED)

find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)

find_package(example_interfaces REQUIRED)
find_package(rclcpp_action REQUIRED)

add_executable(first_node src/first_node.cpp)
ament_target_dependencies(first_node rclcpp) # for non-component node
install(TARGETS first_node DESTINATION lib/${PROJECT_NAME})


# Library setup for component
add_library(${PROJECT_NAME} SHARED
  src/first_component.cpp
  src/test_node.cpp
  src/topics_node.cpp
  src/tf_broadcaster.cpp
  src/tf_listener.cpp

  src/action_server_node.cpp
  src/action_client_node.cpp
)

ament_target_dependencies(${PROJECT_NAME}
  rclcpp
  rclcpp_components
  std_msgs
  
  tf2_ros
  tf2_geometry_msgs

  example_interfaces
  rclcpp_action
) # try removing std_msgs

install(TARGETS ${PROJECT_NAME} DESTINATION lib)

# Tell ROS about node in library
rclcpp_components_register_node(
  ${PROJECT_NAME} # Library
  PLUGIN "test_pkg::SecondNode" # Class
  EXECUTABLE second_node # Executable name
)

rclcpp_components_register_node(
  ${PROJECT_NAME} # Library
  PLUGIN "test_pkg::TestNode" # Class
  EXECUTABLE test_node # Executable name
)

rclcpp_components_register_node(
  ${PROJECT_NAME} # Library
  PLUGIN "test_pkg::TopicsNode" # Class/type
  EXECUTABLE topics_node # Executable name
)

rclcpp_components_register_node(
  ${PROJECT_NAME} # Library
  PLUGIN "test_pkg::TfListenerNode" # Class/type
  EXECUTABLE tf_listener_node # Executable name
)

rclcpp_components_register_node(
  ${PROJECT_NAME} # Library
  PLUGIN "test_pkg::TfBroadcasterNode" # Class/type
  EXECUTABLE tf_broadcaster_node # Executable name
)

rclcpp_components_register_node(
  ${PROJECT_NAME} # Library
  PLUGIN "test_pkg::ActionServerNode" # Class/type
  EXECUTABLE action_server_node # Executable name
)

rclcpp_components_register_node(
  ${PROJECT_NAME} # Library
  PLUGIN "test_pkg::ActionClientNode" # Class/type
  EXECUTABLE action_client_node # Executable name
)

# target_include_directories(first_node PUBLIC
#   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#   $<INSTALL_INTERFACE:include>)
# target_compile_features(first_node PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17

install(DIRECTORY launch config DESTINATION share/${PROJECT_NAME})


if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
