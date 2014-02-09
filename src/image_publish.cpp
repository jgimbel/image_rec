#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv/cvwimage.h>
#include <opencv/highgui.h>
#include <cv_bridge/cv_bridge.h>

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_publisher");
  ros::NodeHandle nh;
  std::string input;
  image_transport::ImageTransport it(nh);
  image_transport::Publisher pub = it.advertise("camera/image", 1);

  cv::WImageBuffer3_b image( cvLoadImage("/dev/video0", CV_LOAD_IMAGE_COLOR) );
  cv::Mat imageMat(image.Ipl());

  cv_bridge::CvImage out_msg;
  out_msg.image = imageMat;


  ros::Rate loop_rate(5);
  while (nh.ok()) {
    pub.publish(out_msg.toImageMsg());
    ros::spinOnce();
    loop_rate.sleep();
  }
}
