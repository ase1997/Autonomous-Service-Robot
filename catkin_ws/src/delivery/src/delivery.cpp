#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>


typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;	// create a client to send target goal pose to move_base


// define x-y coordinate for pick-up and drop-off location
double PICKUP_X = 5.15, PICKUP_Y = -4.51;
double DROPOFF_X = 2.7, DROPOFF_Y = 10.1;


int main(int argc, char **argv)
{
  ros::init(argc, argv, "navigation_goal");	// initialize ROS node named navigation_goal
  MoveBaseClient ac("move_base", true);			// tell the action client that we want to spin a thread by default


  while (!ac.waitForServer(ros::Duration(5.0)))
  {
    ROS_INFO("Waiting for the move_base action server to come up");		// wait for the action server to come up
  }


  move_base_msgs::MoveBaseGoal goal;					// create an instance object named goal of move_base message type
  goal.target_pose.header.frame_id = "map";				// fixed frame is the map
  goal.target_pose.header.stamp = ros::Time::now();


  // define pick-up location 
  goal.target_pose.pose.position.x = PICKUP_X;
  goal.target_pose.pose.position.y = PICKUP_Y;
  goal.target_pose.pose.orientation.w = 1.0;
  // send pick-up location to move_base
  ROS_INFO("robot is travelling to the pick-up location");
  ac.sendGoal(goal);
  ac.waitForResult();

  // check if the robot has sucessfully reached the pick-up location
  if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
  {
    ROS_INFO("robot has arrived at pick-up location and is picking up the object");
    ros::Duration(5).sleep();	// Wait for 5 seconds before sending robot to drop-off location

    // define drop-off location 
    goal.target_pose.pose.position.x = DROPOFF_X;
    goal.target_pose.pose.position.y = DROPOFF_Y;
    goal.target_pose.pose.orientation.w = 1.0;
    // send drop-off location to move_base
    ROS_INFO("robot is travaling to drop-off location");
    ac.sendGoal(goal);
    ac.waitForResult();
    
    // check if robot has sucessfully reached drop-off location
    if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    {
      ROS_INFO("robot has arrived at drop-off location and is dropping off the object");
    }
    else
    {
      ROS_INFO("fail to reach drop-off location");
    }
  }
  else
  {
    ROS_INFO("fail to reach pick-up location");
  }


  return 0;
}
