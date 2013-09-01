#include <cv.h>

#define BOARD_ROWS 4
#define BOARD_COLS 4

#define STOPPED 0
#define SCANNING 1

class Scanner {
  private:
    int state;
    IplImage *frame, *gray_frame;
    CvCapture* capture;
    int board_squares;
    int corner_count;
    CvSize board_size;
    CvPoint2D32f* corners;
    bool has_pattern;

  public:
    Scanner(CvCapture* cap) {
      capture       = cap;
      board_squares = BOARD_COLS * BOARD_ROWS;
      board_size    = cvSize(BOARD_COLS, BOARD_ROWS);
      corners       = new CvPoint2D32f[ board_squares ];
      frame         = cvQueryFrame(capture);
    }

    IplImage* getFrame() {
      return frame;
    }

    void setCorners() {
      gray_frame = cvCreateImage(cvGetSize(frame), frame->depth, 1);
      cvCvtColor(frame, gray_frame, CV_BGR2GRAY);

      bool found = cvFindChessboardCorners(frame, board_size, corners,
          &corner_count, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

      if (&found) {

        cvFindCornerSubPix(gray_frame, corners, corner_count,
            cvSize(11, 11), cvSize(-1, -1),
            cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
      }
    }

    CvPoint2D32f* getCorners() {
      return corners;
    }

    void start() {
      state = SCANNING;
      boost::thread snapping(&Scanner::snap, this);
      boost::thread scanning(&Scanner::scan, this);
    }

    void stop() {
      state = STOPPED;
    }

    void snap() {
      while (state == SCANNING) {
        frame = cvQueryFrame(capture);
        if (!frame) { break; }
      }
    }

    void scan() {
      while (state == SCANNING) {
        setCorners();
      }
    }
};
