#include <cv.h>

class Marker {
  private:
    IplImage *image, *blank_image;
    CvPoint2D32f p[4];
    CvPoint2D32f q[4];
    CvPoint p_trace[4];
    CvMat* warp_matrix;

    IplImage *frame, *copy_frame, *negative_frame;
  public:
    Marker(IplImage* img) {
      image       = img;
      blank_image = cvCreateImage(cvGetSize(image), 8, 3);
      warp_matrix = cvCreateMat(3, 3, CV_32FC1);
    }

    void mark(CvPoint2D32f* corners, IplImage* frame) {
      copy_frame     = cvCreateImage(cvGetSize(frame), 8, 3);
      negative_frame = cvCreateImage(cvGetSize(frame), 8, 3);

      q[0].x = (float) 0.0;
      q[0].y = (float) 0.0;
      q[1].x = (float) image->width;
      q[1].y = (float) 0.0;

      q[2].x = (float) image->width;
      q[2].y = (float) image->height;
      q[3].x = (float) 0.0;
      q[3].y = (float) image->height;

      p[0].x = (int) corners[0].x;
      p[0].y = (int) corners[0].y;
      p[1].x = (int) corners[3].x;
      p[1].y = (int) corners[3].y;

      p[2].x = (int) corners[15].x;
      p[2].y = (int) corners[15].y;
      p[3].x = (int) corners[12].x;
      p[3].y = (int) corners[12].y;

      cvGetPerspectiveTransform(q, p, warp_matrix);
      cvZero(negative_frame);
      cvZero(copy_frame);
      cvZero(blank_image);
      cvNot(blank_image, blank_image);

      cvWarpPerspective(image, negative_frame, warp_matrix);
      cvWarpPerspective(blank_image, copy_frame, warp_matrix);
      cvNot(copy_frame, copy_frame);
      cvAnd(copy_frame, frame, copy_frame);
      cvOr(copy_frame, negative_frame, frame);
    }

    void trace(CvPoint2D32f* corners, IplImage* frame) {
      p_trace[0].x = (int) corners[0].x;
      p_trace[0].y = (int) corners[0].y;
      p_trace[1].x = (int) corners[3].x;
      p_trace[1].y = (int) corners[3].y;

      p_trace[2].x = (int) corners[15].x;
      p_trace[2].y = (int) corners[15].y;
      p_trace[3].x = (int) corners[12].x;
      p_trace[3].y = (int) corners[12].y;

      cvLine(frame, p_trace[0], p_trace[1], CV_RGB(255,0,0),   2); // Red
      cvLine(frame, p_trace[1], p_trace[2], CV_RGB(0,255,0),   2); // Green
      cvLine(frame, p_trace[2], p_trace[3], CV_RGB(0,0,255),   2); // Blue
      cvLine(frame, p_trace[3], p_trace[0], CV_RGB(255,255,0), 2); // Yellow
    }
};
