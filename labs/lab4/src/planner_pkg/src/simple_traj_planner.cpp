#include <geometry_msgs/msg/pose_array.hpp>
#include <geometry_msgs/msg/transform.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <rclcpp/publisher.hpp>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/subscription.hpp>
#include <sstream>
#include <std_msgs/msg/string.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <trajectory_msgs/msg/multi_dof_joint_trajectory.hpp>
#include <trajectory_msgs/msg/multi_dof_joint_trajectory_point.hpp>

class SimpleTrajPlanner : public rclcpp::Node {
public:
  SimpleTrajPlanner() : Node("simple_traj_planner") {
    // Subscriber for desired trajectory vertices
    desire_traj_vertices_sub_ =
        this->create_subscription<geometry_msgs::msg::PoseArray>(
            "desired_traj_vertices", 10,
            std::bind(&SimpleTrajPlanner::trajCB, this, std::placeholders::_1));

    // Publisher for desired states for controller
    desired_state_pub_ = this->create_publisher<
        trajectory_msgs::msg::MultiDOFJointTrajectoryPoint>("desired_state", 1);

    // Transform broadcaster
    br_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
  }

private:
  rclcpp::Subscription<geometry_msgs::msg::PoseArray>::SharedPtr
      desire_traj_vertices_sub_;
  rclcpp::Publisher<trajectory_msgs::msg::MultiDOFJointTrajectoryPoint>::
      SharedPtr desired_state_pub_;
  std::unique_ptr<tf2_ros::TransformBroadcaster> br_;

  void trajCB(const geometry_msgs::msg::PoseArray::SharedPtr traj_msg) {
    // Sanity check for traj_msg size
    if (traj_msg->poses.empty()) {
      RCLCPP_ERROR_THROTTLE(get_logger(), *get_clock(), 1000,
                            "Empty trajectory vertices msg.");
      return;
    }


    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //  PART 0 |  16.485 - Fall 2020  - Lab 4 coding assignment  (10 pts)
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    //
    //  As a simple warm up exercise before we get to the actual 'real deal',
    //  let's just make our quadcopter fly to the first gate in the course.
    //  In this section:
    //   1. Extract the first vertex of the trajectory
    //   2. Set the acceleration and velocities to zero
    //   3. Publish the desired MultiDOFJointTrajectoryPoint
    //   4. Create and publish TF transform of the desired pose
    // ~~~~ begin solution
    // const auto& traj_msg->poses[0]
    geometry_msgs::msg::Transform transform;
    geometry_msgs::msg::TransformStamped transformStamped;
    geometry_msgs::msg::Twist accelerations;
    geometry_msgs::msg::Twist velocities;
    geometry_msgs::msg::Pose desired_pose;
    trajectory_msgs::msg::MultiDOFJointTrajectoryPoint desired_MDOF;

    // 0. stamped transform
    transformStamped.header.stamp = this->get_clock()->now();
    transformStamped.header.frame_id = "world";
    transformStamped.child_frame_id = "desired_pose";
    

    // 1. get desired pose
    desired_pose.position.x = traj_msg->poses[0].position.x;
    desired_pose.position.y = traj_msg->poses[0].position.y;
    desired_pose.position.z = traj_msg->poses[0].position.z;

    desired_pose.orientation.x = traj_msg->poses[0].orientation.x;
    desired_pose.orientation.y = traj_msg->poses[0].orientation.y;
    desired_pose.orientation.z = traj_msg->poses[0].orientation.z;
    desired_pose.orientation.w = traj_msg->poses[0].orientation.w;

    transform.translation.x = desired_pose.position.x;
    transform.translation.y = desired_pose.position.y;
    transform.translation.z = desired_pose.position.z;

    transform.rotation.w = desired_pose.orientation.w;
    transform.rotation.x = desired_pose.orientation.x;
    transform.rotation.y = desired_pose.orientation.y;
    transform.rotation.z = desired_pose.orientation.z;
  
    // 2. set acc and vel to zero
    velocities.linear.x = 0;
    velocities.linear.y = 0;
    velocities.linear.z = 0;
    velocities.angular.x = 0;
    velocities.angular.y = 0;
    velocities.angular.z = 0;
    
    accelerations.linear.x = 0;
    accelerations.linear.y = 0;
    accelerations.linear.z = 0;
    accelerations.angular.x = 0;
    accelerations.angular.y = 0;
    accelerations.angular.z = 0;

    // Desired MDOF
    desired_MDOF.time_from_start = rclcpp::Duration(0, 0);
    desired_MDOF.transforms.push_back(transform);
    desired_MDOF.velocities.push_back(velocities);
    desired_MDOF.accelerations.push_back(accelerations);

    
    desired_state_pub_->publish(desired_MDOF);

    transformStamped.transform = transform;
    br_->sendTransform(transformStamped);

    // ~~~~ end solution
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    //                                 end part 0
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Logging to terminal
    std::stringstream ss;
    ss << "Trajectory Position"
       << " x:" << transform.translation.x << " y:" << transform.translation.y
       << " z:" << transform.translation.z;
    RCLCPP_INFO(get_logger(), "%s", ss.str().c_str());
  }
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SimpleTrajPlanner>());
  rclcpp::shutdown();
  return 0;
}
