#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sensor_msgs/image_encodings.h>

namespace enc = sensor_msgs::image_encodings;

class image_rec
{
public:
    ros::NodeHandle nh;
    image_transport::ImageTransport it;
    image_transport::Subscriber sub;
    image_transport::Publisher pub;

    image_rec()
        :  it(nh)
    {
        std::string in;
        nh.param("in",in,in);
        sub = it.subscribe(in, 30, &image_rec::imageCallback,this);
        pub = it.advertise("camera/image_raw", 1);
        cv::namedWindow("window");

    }

    ~image_rec(){
        cv::destroyWindow("window");
    }


    void imageCallback(const sensor_msgs::Image::ConstPtr& msg)
    {
        cv_bridge::CvImagePtr cv_ptr;
        try
        {
            cv_ptr = cv_bridge::toCvCopy(msg, enc::BGR8);
        }
        catch (cv_bridge::Exception e)
        {
            ROS_ERROR("cv_bridge exception: %s", e.what());
            return;
        }

        if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
            cv::circle(cv_ptr->image, cv::Point(30,50), 10, CV_RGB(0,0,255));

        cv::imshow("window", cv_ptr->image);
        cv::waitKey(3);

        pub.publish(cv_ptr->toImageMsg());
    }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_rec");
  image_rec image;
  ros::spin();
  return 0;
}
