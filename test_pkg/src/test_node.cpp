#include <rclcpp/rclcpp.hpp>
#include <rclcpp_components/register_node_macro.hpp>

namespace test_pkg { //match package name

class TestNode : public rclcpp::Node {
    public:
        explicit TestNode(const rclcpp::NodeOptions& options) : rclcpp::Node("test_node", options) {
            timer_ = create_wall_timer(std::chrono::seconds(1), std::bind(&TestNode::TimerCallback, this));
            RCLCPP_INFO(get_logger(), "Component node is ready!");
        }
    private:
        rclcpp::TimerBase::SharedPtr timer_;

        void TimerCallback() {
            RCLCPP_INFO(get_logger(), "Hello!");
        }
};
}

RCLCPP_COMPONENTS_REGISTER_NODE(test_pkg::TestNode)