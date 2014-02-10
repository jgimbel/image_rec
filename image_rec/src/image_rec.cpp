#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/objdetect/objdetect.hpp>
#include <geometry_msgs/Twist.h>
namespace enc = sensor_msgs::image_encodings;
using namespace cv;

/** Global variables */
char const static face_cascade_name[] = "haarcascade_frontalface_alt.xml";
char const static eyes_cascade_name[] = "haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
char const static window_name[] = "window";


class image_rec
{
public:
    ros::NodeHandle nh;
    image_transport::ImageTransport it;
    image_transport::Subscriber sub;
    image_transport::Publisher pub;
    ros::Publisher drone;

    image_rec()
        :  it(nh)
    {
        if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); exit(-1);}else{printf("--(#)foundfile\n");};
        if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading\n"); exit(-1);}else{printf("--(#)foundfile\n");};
        sub = it.subscribe("in", 1, &image_rec::imageCallback,this);
        drone = nh.advertise<geometry_msgs::Twist>("cmd_vel",1);
        pub = it.advertise("out",1);
        cv::namedWindow(window_name);

    }

    ~image_rec(){
        cv::destroyWindow(window_name);
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

        std::vector<Rect> faces;
        Mat frame_gray;
        Mat frame(cv_ptr->image);

        cvtColor( frame, frame_gray, CV_BGR2GRAY );
        equalizeHist( frame_gray, frame_gray );


          //-- Detect faces
        face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
        for( size_t i = 0; i < faces.size(); i++ )
            {
                Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
                ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

                Mat faceROI = frame_gray( faces[i] );
                std::vector<Rect> eyes;

                //-- In each face, detect eyes
                eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CASCADE_SCALE_IMAGE, Size(30, 30) );

                for( size_t j = 0; j < eyes.size(); j++ )
                {
                    Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
                    int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
                    circle( frame, eye_center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
                }
            }

        geometry_msgs::Twist twist;
        if(faces.size() != 0){
            if(faces[0].x+(faces[0].width/2) < cv_ptr->image.cols/2){
                twist.linear.y = .01;
            }else {
                twist.linear.y = -.01;
            }
            if(faces[0].y+(faces[0].height/2) < cv_ptr->image.rows/2){
                twist.linear.z = .1;
            } else {
                twist.linear.z = -.1;
            }
        }
        drone.publish(twist);


        cv::imshow(window_name, cv_ptr->image);
        cv::waitKey(1);

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

