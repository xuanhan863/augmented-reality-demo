#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <boost/thread.hpp>
#include "scanner.cpp"
#include "marker.cpp"


int main(int argc, const char* argv[]) {
  if (!argv[1]) { return 1; }

  //Data Structure to store cam.
  CvCapture* capture = cvCaptureFromCAM(0);
  Scanner scanner = Scanner(capture);

  IplImage* image = cvLoadImage(argv[1]);
  IplImage *tmpFrame, *frame;

  Marker marker = Marker(image);

  //Window to show livefeed
  cvNamedWindow("LiveFeed", CV_WINDOW_AUTOSIZE);
  scanner.start();

  while (true) {
    tmpFrame = scanner.getFrame();
    marker.trace(scanner.getCorners(), tmpFrame);

    // Flip for mirror view
    frame = cvCreateImage(cvGetSize(tmpFrame), 8, 3);
    cvFlip(tmpFrame, frame, 1);

    cvShowImage("LiveFeed", frame);

    char c = cvWaitKey(33);
    if (c == 27) {  // Esc key
      scanner.stop();
      break;
    }
  }

  //CleanUp
  cvReleaseCapture(&capture);
  cvDestroyAllWindows();

  return 0;
}
