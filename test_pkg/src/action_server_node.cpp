#include <rclcpp/rclcpp.hpp>
#include <rclcpp_components/register_node_macro.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <example_interfaces/action/fibonacci.hpp>

namespace test_pkg {

class ActionServerNode : public rclcpp::Node {
  public:
    using Fibonacci = example_interfaces::action::Fibonacci;
    using FibonacciGoalHandle= rclcpp_action::ServerGoalHandle<Fibonacci>;
    explicit ActionServerNode(const rclcpp::NodeOptions &options)
        : rclcpp::Node("action_server_node", options) {
      server_ = rclcpp_action::create_server<Fibonacci>(
          this,
          "/fibonacci",
          std::bind(&ActionServerNode::NewGoalCallback, this, std::placeholders::_1, std::placeholders::_2),
          std::bind(&ActionServerNode::CancelCallback, this, std::placeholders::_1),
          std::bind(&ActionServerNode::AcceptedGoalCallback, this, std::placeholders::_1));
    }
  private:
    rclcpp_action::Server<Fibonacci>::SharedPtr server_;
    rclcpp_action::GoalResponse NewGoalCallback(
        const rclcpp_action::GoalUUID &uuid,
        std::shared_ptr<const Fibonacci::Goal> goal) {
      RCLCPP_INFO(get_logger(), "New goal (%s) with order %d", rclcpp_action::to_string(uuid).c_str(), goal -> order);
      return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }

    rclcpp_action::CancelResponse CancelCallback(const std::shared_ptr<FibonacciGoalHandle> goal_handle) {
      RCLCPP_INFO(get_logger(), "Cancelling goal %s", rclcpp_action::to_string(goal_handle -> get_goal_id()).c_str());
      return rclcpp_action::CancelResponse::ACCEPT;
    }

    void AcceptedGoalCallback(const std::shared_ptr<FibonacciGoalHandle> goal_handle) {
      std::thread([this, goal_handle]() {
        Execute(goal_handle);
      }).detach();
    }

    void Execute(const std::shared_ptr<FibonacciGoalHandle> goal_handle) {
      RCLCPP_INFO(get_logger(), "Executing goal %s", rclcpp_action::to_string(goal_handle -> get_goal_id()).c_str());
      const auto goal = goal_handle -> get_goal();
      auto feedback = std::make_shared<Fibonacci::Feedback>();
      auto &sequence = feedback -> sequence;
      sequence.push_back(0);
      sequence.push_back(1);
      auto result = std::make_shared<Fibonacci::Result>();

      rclcpp::Rate rate(1);
      for (int i = 1; i < goal -> order && rclcpp::ok(); ++i) {
        if (goal_handle -> is_canceling()) {
          result -> sequence = sequence;
          goal_handle -> canceled(result);
          RCLCPP_INFO(get_logger(), "Goal cancelled (%s)", rclcpp_action::to_string(goal_handle -> get_goal_id()).c_str());
          return;
        }
        sequence.push_back(sequence[i] + sequence[i - 1]);
        goal_handle -> publish_feedback(feedback);
        rate.sleep();
      }

      if (rclcpp::ok()) {
        result -> sequence = sequence;
        goal_handle -> succeed(result);
        RCLCPP_INFO(get_logger(), "Goal succeeded (%s)", rclcpp_action::to_string(goal_handle -> get_goal_id()).c_str());
      }
    }

};

}

RCLCPP_COMPONENTS_REGISTER_NODE(test_pkg::ActionServerNode)
