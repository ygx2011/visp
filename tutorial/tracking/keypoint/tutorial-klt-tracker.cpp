//! \example tutorial-klt-tracker.cpp
//! [Include]
#include <visp/vpImageConvert.h>
#include <visp/vpKltOpencv.h>
#include <visp/vpDisplayOpenCV.h>
#include <visp/vpVideoReader.h>
//! [Include]

int main(int argc, const char *argv[])
{
  //! [Check 3rd party]
#ifdef VISP_HAVE_OPENCV
  //! [Check 3rd party]
  try {
    bool opt_init_by_click = false;
#if (VISP_HAVE_OPENCV_VERSION >= 0x020408)
    for (int i=0; i<argc; i++) {
      if (std::string(argv[i]) == "--init-by-click")
        opt_init_by_click = true;
      else if (std::string(argv[i]) == "--help") {
        std::cout << "Usage: " << argv[0] << " [--init-by-click] [--help]" << std::endl;
        return 0;
      }
    }
#else
    (void)argc;
    (void)argv;
#endif
    //! [Create reader]
    vpVideoReader reader;
    reader.setFileName("video-postcard.mpeg");
    //! [Create reader]

    //! [Acquire]
    vpImage<unsigned char> I;
    reader.acquire(I);
    //! [Acquire]

    //! Convert to OpenCV image
#if (VISP_HAVE_OPENCV_VERSION < 0x020408)
    IplImage * cvI = NULL;
#else
    cv::Mat cvI;
#endif
    vpImageConvert::convert(I, cvI);
    //! Convert to OpenCV image

    //! [Init display]
    vpDisplayOpenCV d(I, 0, 0, "Klt tracking");
    vpDisplay::display(I);
    vpDisplay::flush(I);
    //! [Init display]

    //! [Create tracker]
    vpKltOpencv tracker;
    tracker.setMaxFeatures(200);
    tracker.setWindowSize(10);
    tracker.setQuality(0.01);
    tracker.setMinDistance(15);
    tracker.setHarrisFreeParameter(0.04);
    tracker.setBlockSize(9);
    tracker.setUseHarris(1);
    tracker.setPyramidLevels(3);
    //! [Create tracker]

    // Initialise the tracking
    if (opt_init_by_click) {
#if (VISP_HAVE_OPENCV_VERSION >= 0x020408)
      vpMouseButton::vpMouseButtonType button;
      std::vector<cv::Point2f> guess;
      vpImagePoint ip;
      do {
        vpDisplay::displayText(I, 10, 10, "Left click to select a point, right to start tracking", vpColor::red);
        if (vpDisplay::getClick(I, ip, button, false)) {
          if (button == vpMouseButton::button1) {
            guess.push_back(cv::Point2f((float)ip.get_u(), (float)ip.get_v()));
            vpDisplay::displayCross(I, ip, 12, vpColor::green);
          }
        }
        vpDisplay::flush(I);
        vpTime::wait(20);
      } while(button != vpMouseButton::button3);
      tracker.initTracking(guess);
#endif
    }
    else {
      //! [Init tracker]
      tracker.initTracking(cvI);
      //! [Init tracker]
    }
    //! [While loop]
    while ( ! reader.end() )
    {
      reader.acquire(I);
      vpDisplay::display(I);

      vpImageConvert::convert(I, cvI);
      tracker.track(cvI);

      tracker.display(I, vpColor::red);
      vpDisplay::flush(I);
    }
    //! [While loop]

    //! [Wait click]
    vpDisplay::getClick(I);
    //! [Wait click]

    //! [Release IplImage]
#if (VISP_HAVE_OPENCV_VERSION < 0x020408)
    cvReleaseImage(&cvI);
#endif
    //! [Release IplImage]

    return 0;
  }
  catch(vpException e) {
    std::cout << "Catch an exception: " << e << std::endl;
  }
#else
  (void)argc;
  (void)argv;
#endif
}
