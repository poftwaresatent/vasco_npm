/* Vasco prototype component.
 *
 * Copyright (C) 2014 Roland Philippsen. All rights reserved.
 *
 * BSD license:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of
 *    contributors to this software may be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR THE CONTRIBUTORS TO THIS SOFTWARE BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <npm/Plugin.hpp>
#include <npm/Factory.hpp>
#include <npm/ext/Zombie.hpp>
#include <npm/RobotServer.hpp>
#include <npm/Lidar.hpp>

#include "ros/ros.h"
#include "vasco_npm/Scan.h"

using namespace std;


class VascoRobot
  : public npm::LidarZombie
{
public:
  VascoRobot (string const & name)
    : LidarZombie (name),
      scan_topic_ ("scan"),
      server_ (0)
  {
    reflectParameter ("scan_topic", &scan_topic_);
  }
  
  
  virtual bool Initialize (npm::RobotServer &server)
  {
    if ( ! LidarZombie::Initialize (server)) {
      return false;
    }
    ros::NodeHandle node;
    scan_pub_ = node.advertise <vasco_npm::Scan> (scan_topic_, 1);
    server_ = &server;
    return true;
  }
  
  
  virtual bool PrepareAction (double timestep)
  {
    if ( ! LidarZombie::PrepareAction (timestep)) {
      return false;
    }
    
    //// in case we want to receive messages as well...
    //
    // if ( ! ros::ok()) {
    //   return false;
    // }
    
    sfl::Frame const & pose (server_->GetPose());
    
    vasco_npm::Scan msg;
    msg.px = pose.X();
    msg.py = pose.Y();
    msg.pth = pose.Theta();
    msg.mx = m_lidar->mount->X();
    msg.my = m_lidar->mount->Y();
    msg.mth = m_lidar->mount->Theta();
    msg.nscans = m_lidar->nscans;
    msg.rhomax = m_lidar->rhomax;
    msg.phi0 = m_lidar->phi0;
    msg.phirange = m_lidar->phirange;
    for (size_t ii (0); ii < m_lidar->nscans; ++ii) {
      msg.rho.push_back (m_lidar->GetNoisyRho (ii));
    }
    scan_pub_.publish (msg);
    
    ros::spinOnce();
    
    return true;
  }
  
  
  std::string scan_topic_;
  ros::Publisher scan_pub_;
  npm::RobotServer * server_;
};


int npm_plugin_init ()
{
  if ( ! ros::isInitialized()) {
    int argc = 0;
    char** argv = NULL;
    ros::init(argc, argv, "vasco_npm",
	      ros::init_options::NoSigintHandler |
	      ros::init_options::AnonymousName);
  }
  
  npm::Factory::Instance().declare<VascoRobot>("VascoRobot");
  
  return 0;
}
