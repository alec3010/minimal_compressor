#include "ros/ros.h"
#include "sensor_msgs/Image.h"

#include "zdepth.hpp"

//#include <stdio.h>     // printf
#include <stdlib.h>    // free
//#include <string.h>    // strlen, strcat, memset

class MinimalCompression
{
  public: 
    zdepth::DepthCompressor compressor;
    zdepth::VideoParameters params{};
    
    bool keyframe = true;

  public:
    MinimalCompression()
    {
      params.Type = zdepth::VideoType::H265;
      params.Fps = 30;
    }
    void minimalCallback(const sensor_msgs::Image::ConstPtr& depth_msg)
    {
      //initialize st::vector as container for compressed data
      std::vector<uint8_t> compressed;

      //initialize pointer to hold image message from ros
      const uint16_t* frame = (uint16_t*)depth_msg->data.data();

      params.Width = depth_msg->width;
      params.Height = depth_msg->height;
      

      compressor.Compress(params, frame, compressed, keyframe);
      std::cout<<"Compressed Depth Image"<<std::endl;

    }
};
int main(int argc, char **argv)
{
  ros::init(argc, argv, "minimal_compression");
  MinimalCompression min_compression;
  ros::NodeHandle n;
  ros::Subscriber minimal_depth_sub = n.subscribe("/depth/image_raw", 1000, &MinimalCompression::minimalCallback, &min_compression);
  ros::spin();

  return 0;
}