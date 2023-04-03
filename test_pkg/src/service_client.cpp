#include <rclcpp/rclcpp.hpp>
#include <rclcpp_components/register_node_macro.hpp>
#include <turtlesim/srv/spawn.hpp>

namespace test_pkg {

class ServiceClientDemoNode : public rclcpp::Node {
    public:
        explicit ServiceClientDemoNode(const rclcpp::NodeOptions& options) : rclcpp::Node("service_demo_node", options) {
            client_ = create_client<turtlesim::srv::Spawn>("/spwn");
            std::thread(std::bind(&ServiceClientDemoNode::SendRequest, this)).detach();
        }
    private:
        rclcpp::Client<turtlesim::srv::Spawn>::SharedPtr client_;
        
        void SendRequest() {
            RCLCPP_INFO(get_logger(), "Waiting for service...");
            if (!client_ -> wait_for_service(std::chrono::seconds(10))) {
                RCLCPP_ERROR(get_logger(), "Couldn't find service!");
                return;
            }
            RCLCPP_INFO(get_logger(), "Service ready!");
            auto request = std::make_shared<turtlesim::srv::Spawn::Request>();
            request -> x = 2.5;
            request -> y = 2.5;
            auto result = client_ -> async_send_request(request);
            RCLCPP_INFO(get_logger(), "Request sent");
            if (result.wait_for(std::chrono::seconds(10)) == std::future_status::timeout) {
                RCLCPP_ERROR(get_logger(), "Service took too long to complete!");
                return;
            }
            RCLCPP_INFO(get_logger(), "Turtle spawned with name %s", result.get() -> name.c_str());
        }

    };
}

RCLCPP_COMPONENTS_REGISTER_NODE(test_pkg::ServiceClientDemoNode)