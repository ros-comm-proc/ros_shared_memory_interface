/*
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2014, Chien-Liang Fok
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#include "ros/ros.h"
#include "shared_memory_interface/shared_memory_publisher.hpp"
#include "shared_memory_interface/shared_memory_subscriber.hpp"
#include <std_msgs/Int64.h>

shared_memory_interface::Publisher<std_msgs::Int64> pub;

bool first_message_received;

void rttTxCallback(std_msgs::Int64& msg)
{
  if (!pub.publish(msg))
  {
    ROS_ERROR("Slave: Failed to publish message. Aborting.");
  }

  first_message_received = true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "slave", ros::init_options::AnonymousName);
  ros::NodeHandle n;

  pub.advertise("/rtt_rx");

  shared_memory_interface::Subscriber<std_msgs::Int64> sub;
  sub.subscribe("/rtt_tx", boost::bind(&rttTxCallback, _1));

  // std_msgs::Int64 msg;
  // while(!first_message_received)
  // {
  //   sub.getCurrentMessage(msg);
  //   pub.publish(msg);
  //   ROS_WARN_THROTTLE(1.0, "Waiting for first message");
  // }

  ros::spin();
  return 0;
}
