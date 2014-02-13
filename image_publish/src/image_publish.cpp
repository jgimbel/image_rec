#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv/cvwimage.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
using namespace cv;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_publisher");
  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  image_transport::Publisher pub = it.advertise("cam/a", 1);

  cv::VideoCapture cap(0);

  if(!cap.isOpened()){
      ROS_ERROR("camera not opened");
      return -1;
  }

  ros::Rate loop_rate(5);
  while (nh.ok()) {
    Mat frame;

    cap >> frame; // get a new frame from camera

    cv_bridge::CvImage out_msg;
    out_msg.image = frame;
    out_msg.encoding ="bgr8";
    pub.publish(out_msg.toImageMsg());
    ros::spinOnce();
    loop_rate.sleep();
  }
}
