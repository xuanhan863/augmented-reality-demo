#include <cv.h>
#include <highgui.h>

#define BOARD_ROWS 4
#define BOARD_COLS 4

int main() {
  //Data Structure to store cam.
  CvCapture* capture = cvCaptureFromCAM(0);

  //Image variable to store frame
  IplImage* frame;

  //Window to show livefeed
  cvNamedWindow("LiveFeed", CV_WINDOW_AUTOSIZE);

  int board_squares     = BOARD_COLS * BOARD_ROWS;
  CvSize board_size     = cvSize(BOARD_COLS, BOARD_ROWS);
  CvPoint2D32f* corners = new CvPoint2D32f[ board_squares ];
  int corner_count;

  while (true) {
    frame = cvQueryFrame(capture);
    if(!frame) { break; }
    cvFlip(frame, frame,1);

    IplImage* gray_frame = cvCreateImage( cvGetSize(frame), frame->depth, 1);
    cvCvtColor(frame, gray_frame, CV_BGR2GRAY);

    bool found = cvFindChessboardCorners(frame, board_size, corners,
        &corner_count, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

    if (found) {
      cvFindCornerSubPix(gray_frame, corners, corner_count,
          cvSize(11,11), cvSize(-1,-1),
          cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));

      if (corner_count == board_squares) {
        CvPoint p[4];

        p[0].x = (int) corners[0].x;
        p[0].y = (int) corners[0].y;
        p[1].x = (int) corners[3].x;
        p[1].y = (int) corners[3].y;

        p[2].x = (int) corners[15].x;
        p[2].y = (int) corners[15].y;
        p[3].x = (int) corners[12].x;
        p[3].y = (int) corners[12].y;

        cvLine(frame, p[0], p[1], CV_RGB(255,0,0),   2); // Red
        cvLine(frame, p[1], p[2], CV_RGB(0,255,0),   2); // Green
        cvLine(frame, p[2], p[3], CV_RGB(0,0,255),   2); // Blue
        cvLine(frame, p[3], p[0], CV_RGB(255,255,0), 2); // Yellow
      }
    }

    //Show the present frame
    cvShowImage("LiveFeed", frame);

    char c = cvWaitKey(33);
    if (c == 27) { break; } // Esc key
  }

  //CleanUp
  cvReleaseCapture(&capture);
  cvDestroyAllWindows();
}
