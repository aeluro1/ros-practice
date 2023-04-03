#include <rclcpp/rclcpp.hpp>
#include <rclcpp_components/register_node_macro.hpp>
#include <std_msgs/msg/string.hpp>

namespace test_pkg { //match package name

class TopicsNode : public rclcpp::Node {
    public:
        explicit TopicsNode(const rclcpp::NodeOptions& options) : rclcpp::Node("topics_node", options) {
            _publisher = create_publisher<std_msgs::msg::String>("/greeting", rclcpp::SystemDefaultsQoS());
            _subscription = create_subscription<std_msgs::msg::String>(
                "/name",
                rclcpp::SystemDefaultsQoS(),
                std::bind(&TopicsNode::Callback, this, std::placeholders::_1));
            RCLCPP_INFO(get_logger(), "Component node is ready!");
        }

    private:
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr _publisher;

        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr _subscription;

        void Callback(const std_msgs::msg::String::SharedPtr msg) {
            std_msgs::msg::String greeting_msg;
            greeting_msg.data = "Hello, " + msg -> data + "!";
            _publisher -> publish(greeting_msg);
        }


};
}

RCLCPP_COMPONENTS_REGISTER_NODE(test_pkg::TopicsNode)