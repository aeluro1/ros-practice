#include <rclcpp/rclcpp.hpp>
#include <rclcpp_components/register_node_macro.hpp>

namespace test_pkg { //match package name

class SecondNode : public rclcpp::Node {
    public:
        explicit SecondNode(const rclcpp::NodeOptions& options) : rclcpp::Node("second_node", options) {
            RCLCPP_INFO(get_logger(), "Component node is ready!");
        }
    private:
};
}

RCLCPP_COMPONENTS_REGISTER_NODE(test_pkg::SecondNode)