#include <rclcpp/rclcpp.hpp>
#include <rclcpp_components/register_node_macro.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <example_interfaces/action/fibonacci.hpp>

namespace test_pkg {

class ActionClientNode : public rclcpp::Node {
  public:
    using Fibonacci = example_interfaces::action::Fibonacci;
    using FibonacciGoalHandle = rclcpp_action::ClientGoalHandle<Fibonacci>;

    explicit ActionClientNode(const rclcpp::NodeOptions &options)
        : rclcpp::Node("action_client_node", options) {
      client_ = rclcpp_action::create_client<Fibonacci>(this, "/fibonacci");
      timer_ = create_wall_timer(std::chrono::seconds(1), std::bind(&ActionClientNode::TimerCallback, this));
    }

  private:
    rclcpp_action::Client<Fibonacci>::SharedPtr client_;
    rclcpp::TimerBase::SharedPtr timer_;

    void TimerCallback() {
      timer_ -> cancel(); // single-shot timer so client sends goal after node is constructed & ROS event loop is started

      rclcpp_action::Client<Fibonacci>::SendGoalOptions goal_options;

      goal_options.goal_response_callback = std::bind(&ActionClientNode::GoalResponseCallback, this, std::placeholders::_1);
      goal_options.feedback_callback = std::bind(&ActionClientNode::FeedbackCallback, this, std::placeholders::_1, std::placeholders::_2);
      goal_options.result_callback = std::bind(&ActionClientNode::ResultCallback, this, std::placeholders::_1);

      Fibonacci::Goal goal;
      goal.order = 4;

      client_ -> async_send_goal(goal, goal_options);
    }

    // void GoalResponseCallback(std::shared_future<FibonacciGoalHandle::SharedPtr> future) {
    void GoalResponseCallback(FibonacciGoalHandle::SharedPtr goal_handle) {
      if (!goal_handle) {
        RCLCPP_ERROR(get_logger(), "Action server rejected goal!");
      } else {
        RCLCPP_INFO(get_logger(), "Action server accepted goal (%s)!", rclcpp_action::to_string(goal_handle -> get_goal_id()).c_str());
      }
    }

    void FeedbackCallback(FibonacciGoalHandle::SharedPtr goal_handle, const std::shared_ptr<const Fibonacci::Feedback> feedback) {
      RCLCPP_INFO(get_logger(), "Feedback for goal (%s): %d numbers available.", rclcpp_action::to_string(goal_handle -> get_goal_id()).c_str(), feedback -> sequence.size());
    }

    void ResultCallback(const FibonacciGoalHandle::WrappedResult &result) {
      switch(result.code) {
        case rclcpp_action::ResultCode::SUCCEEDED:
          RCLCPP_INFO(get_logger(), "Goal (%s) succeeded! Final Fibonacci number: %d", rclcpp_action::to_string(result.goal_id).c_str(), result.result -> sequence.back());
          break;
        case rclcpp_action::ResultCode::ABORTED:
          break;
        case rclcpp_action::ResultCode::CANCELED:
          RCLCPP_ERROR(get_logger(), "Goal (%s) canceled!", rclcpp_action::to_string(result.goal_id).c_str());
          break;
        default:
          RCLCPP_ERROR(get_logger(), "Unkown result code for goal (%s)", rclcpp_action::to_string(result.goal_id).c_str());
          break;
      }
      rclcpp::shutdown();
    }

};

}

RCLCPP_COMPONENTS_REGISTER_NODE(test_pkg::ActionClientNode)
