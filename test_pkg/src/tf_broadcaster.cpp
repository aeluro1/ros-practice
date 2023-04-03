#include <rclcpp/rclcpp.hpp>
#include <rclcpp_components/register_node_macro.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf2/LinearMath/Quaternion.h>

namespace test_pkg {

class TfBroadcasterNode : public rclcpp::Node {
    public:
        explicit TfBroadcasterNode(const rclcpp::NodeOptions& options) : rclcpp::Node("tf_broadcaster_node", options), tf_broadcaster_(*this) {
            timer_ = create_wall_timer(std::chrono::seconds(1), std::bind(&TfBroadcasterNode::TimerCallback, this));
        }
    private:
        tf2_ros::TransformBroadcaster tf_broadcaster_;
        rclcpp::TimerBase::SharedPtr timer_;
        
        void TimerCallback() {
            geometry_msgs::msg::TransformStamped transform;
            transform.header.frame_id = "parent";
            transform.header.stamp = now();
            transform.child_frame_id = "child";
            transform.transform.translation.x = 1.0;
            transform.transform.translation.y = 2.0;
            transform.transform.translation.z = 3.0;
            tf2::Quaternion orientation;
            orientation.setRPY(0.0, 0.0, M_PI_2);
            transform.transform.rotation = tf2::toMsg(orientation);
            tf_broadcaster_.sendTransform(transform);
        }
    };
}

RCLCPP_COMPONENTS_REGISTER_NODE(test_pkg::TfBroadcasterNode)