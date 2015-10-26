#include "mythread.h"

// Include standard OpenCV headers
#include "opencv/cv.h"
#include "opencv/highgui.h"

// Include Boost headers for system time and threading
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/thread.hpp"


using namespace std;
using namespace boost::posix_time;
using namespace cv;

void captureFunc(Mat *frame, VideoCapture *capture){
  //loop infinitely
  for(;;){
        //capture from webcame to Mat frame
        (*capture) >> (*frame);
    }
}

Mythread::Mythread()
{

}

void Mythread::run()
{
    time_duration td, td1;
    ptime nextFrameTimestamp, currentFrameTimestamp, initialLoopTimestamp, finalLoopTimestamp;
    int delayFound;
    //int totalDelay= 0;

    // initialize capture on default source

    VideoCapture capture(0);

    // set framerate to record and capture at
    int framerate = 120;



    // Get the properties from the camera
    double width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    double height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);

    // print camera frame size
  // cout << "Camera properties\n";
  // cout << "width = " << width << endl <<"height = "<< height << endl;

    // Create a matrix to keep the retrieved frame
    Mat frame;

    // Create the video writer
    VideoWriter video("capture.avi",CV_FOURCC('D','I','V','X'), framerate, cvSize((int)width,(int)height) );

    // initialize initial timestamps
    nextFrameTimestamp = microsec_clock::local_time();
    currentFrameTimestamp = nextFrameTimestamp;
    td = (currentFrameTimestamp - nextFrameTimestamp);

    // start thread to begin capture and populate Mat frame
    boost::thread captureThread(captureFunc, &frame, &capture);
   // namedWindow("Video",WINDOW_NORMAL);
   // namedWindow("image",WINDOW_AUTOSIZE);
    // loop infinitely
    int i=0;
    for(;;)
    {

        // wait for X microseconds until 1second/framerate time has passed after previous frame write
        while(td.total_microseconds() < 1000000/framerate){
        //determine current elapsed time
            currentFrameTimestamp = microsec_clock::local_time();
            td = (currentFrameTimestamp - nextFrameTimestamp);
        }

        //	 determine time at start of write
        initialLoopTimestamp = microsec_clock::local_time();


        if((frame.rows && frame.cols)){

    std::ostringstream strs;
    strs << i;
    std::string str = strs.str();
    putText(frame, strs.str() , cvPoint(100,100),FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,255,250), 1, CV_AA);
    waitKey(1) && 0xFF;
    i++;

        }

        // Save frame to video
        video << frame;

        //write previous and current frame timestamp to console
       // cout << nextFrameTimestamp << " " << currentFrameTimestamp << " ";


        // add 1second/framerate time for next loop pause
        nextFrameTimestamp = nextFrameTimestamp + microsec(1000000/framerate);

        // reset time_duration so while loop engages
        td = (currentFrameTimestamp - nextFrameTimestamp);
       // cout<< (td) <<" "<<endl;
        //determine and print out delay in ms, should be less than 1000/FPS
        //occasionally, if delay is larger than said value, correction will occur
        //if delay is consistently larger than said value, then CPU is not powerful
        // enough to capture/decompress/record/compress that fast.
        finalLoopTimestamp = microsec_clock::local_time();
        td1 = (finalLoopTimestamp - initialLoopTimestamp);
        delayFound = td1.total_milliseconds();
//        cout << delayFound << endl;

        //output will be in following format
        //[TIMESTAMP OF PREVIOUS FRAME] [TIMESTAMP OF NEW FRAME] [TIME DELAY OF WRITING]

    }

    // Exit
}
