#include <rclcpp/rclcpp.hpp>
#include <rclcpp_components/register_node_macro.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf2_ros/buffer.h>

namespace test_pkg {

class TfListenerNode : public rclcpp::Node {
    public:
        explicit TfListenerNode(const rclcpp::NodeOptions& options) : rclcpp::Node("tf_listener_node", options), tf_buffer_(get_clock()), tf_listener_(tf_buffer_) {
            timer_ = create_wall_timer(std::chrono::seconds(1), std::bind(&TfListenerNode::TimerCallback, this));
        }
    private:
        tf2_ros::Buffer tf_buffer_;
        tf2_ros::TransformListener tf_listener_;
        rclcpp::TimerBase::SharedPtr timer_;

        void TimerCallback() {
            if (!tf_buffer_.canTransform("child", "parent", tf2::TimePointZero)) { // See if transform exists at most recent time point
                RCLCPP_WARN_ONCE(get_logger(), "Waiting for TF data...");
                return;
            }
            PrintTransform();
            TransformPoint();
        }

        void PrintTransform() {
            const auto &transform = tf_buffer_.lookupTransform("child", "parent", tf2::TimePointZero);
            std::stringstream output;
            output << "\nTransform parent --> child:\n";
            output << "\tTranslation:\n";
            output << "\t\tX = " << transform.transform.translation.x << "\n";
            output << "\t\tY = " << transform.transform.translation.y << "\n";
            output << "\t\tZ = " << transform.transform.translation.z << "\n";
            output << "\tRotation:\n";
            tf2::Quaternion rotation;
            tf2::fromMsg(transform.transform.rotation, rotation);
            tf2::Matrix3x3 rotation_matrix(rotation);
            double roll;
            double pitch;
            double yaw;
            rotation_matrix.getRPY(roll, pitch, yaw);
            output << "\t\tR = " << roll << "\n";
            output << "\t\tP = " << pitch << "\n";
            output << "\t\tY = " << yaw;
            RCLCPP_INFO(get_logger(), "%s", output.str().c_str());
        }

        void TransformPoint() {
            geometry_msgs::msg::PointStamped point;
            point.header.frame_id = "child";
            point.point.x = 1.0;
            point.point.y = 0.0;
            point.point.z = 0.0;
            const auto transformed_point = tf_buffer_.transform(point, "parent");
            RCLCPP_INFO(get_logger(), "Transformed point: (%f, %f, %f)", transformed_point.point.x, transformed_point.point.y, transformed_point.point.z);
        }
};
}

RCLCPP_COMPONENTS_REGISTER_NODE(test_pkg::TfListenerNode)