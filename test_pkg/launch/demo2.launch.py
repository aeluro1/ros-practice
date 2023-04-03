from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        DeclareLaunchArgument(
            name="background_r",
            default_value="0"
        ),
        DeclareLaunchArgument(
            name="background_g",
            default_value="255"
        ),
        DeclareLaunchArgument(
            name="background_b",
            default_value="0"
        ),
        Node(
            package="turtlesim",
            executable="turtlesim_node",
            parameters=[{
                "background_r": LaunchConfiguration("background_r"),
                "background_g": LaunchConfiguration("background_g"),
                "background_b": LaunchConfiguration("background_b")
            }]
        ),
        Node(
            package="turtlesim",
            executable="draw_square"
        )
    ])