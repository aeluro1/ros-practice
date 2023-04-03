#include <rclcpp/rclcpp.hpp>

class FirstNode : public rclcpp::Node {
    public:
        explicit FirstNode() : rclcpp::Node("first_node") {
            RCLCPP_INFO(get_logger(), "First node is ready!");
        }
    private:
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<FirstNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}