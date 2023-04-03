#include <rclcpp/rclcpp.hpp>
#include <rclcpp_components/register_node_macro.hpp>
#include <example_interfaces/srv/add_two_ints.hpp>

namespace test_pkg {

class ServiceServerDemoNode : public rclcpp::Node {
    public:
        explicit ServiceServerDemoNode(const rclcpp::NodeOptions& options) : rclcpp::Node("service_server_demo_node", options) {
            service_ = create_service<example_interfaces::srv::AddTwoInts>("/add_ints", std::bind(&ServiceServerDemoNode::Callback, this, std::placeholders::_1, std::placeholders::_2));
        }
    private:
        rclcpp::Service<example_interfaces::srv::AddTwoInts>::SharedPtr service_;
        
        void Callback(const example_interfaces::srv::AddTwoInts::Request::SharedPtr request,
                example_interfaces::srv::AddTwoInts::Response::SharedPtr response) {
                    response -> sum = request -> a + request -> b;
                }
    };
}

RCLCPP_COMPONENTS_REGISTER_NODE(test_pkg::ServiceServerDemoNode)