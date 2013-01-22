#ifndef WIN32

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>


#include "filter.h"
using namespace cv;

void facedetect(Mat& inout);
void HoughLines(Mat& src_img);
void HoughCircles(Mat& src_img);
void opticalflow(const Mat& img1,Mat& img2);


void
copy(const BMP4b& in,Mat& out){
  bmp_for(in)
    out.at<Vec3b>(y,x)={in(x,y,2),in(x,y,1),in(x,y,0)};
}

void
copy(const Mat& in,BMP4b& out){
  bmp_for(out){
    Vec3b p=in.at<Vec3b>(y,x);
    out(x,y,2)=p.val[0];
    out(x,y,1)=p.val[1];
    out(x,y,0)=p.val[2];
  }
}

void
copy_alpha(const Mat& in,BMP4b& out){
  bmp_for(out){
    Vec3b p=in.at<Vec3b>(y,x);
    if(p.val[0]||p.val[1]||p.val[2])
      out(x,y,3)=255;
    else
      out(x,y,3)=0;
    out(x,y,2)=p.val[0];
    out(x,y,1)=p.val[1];
    out(x,y,0)=p.val[2];
  }
}

void
facedetect(const BMP4b& in,BMP4b& out){
  Mat img(in.h,in.w,CV_8UC3);
  copy(in,img);
  facedetect(img);
  copy_alpha(img,out);
}


void
houghlines(const BMP4b& in,BMP4b& out){
  Mat img(in.h,in.w,CV_8UC3);
  copy(in,img);
  HoughLines(img);
  copy_alpha(img,out);
}


void
houghcircles(const BMP4b& in,BMP4b& out){
  Mat img(in.h,in.w,CV_8UC3);
  copy(in,img);
  HoughCircles(img);
  copy(img,out);
}


void
opticalflow(const BMP4b& pre,const BMP4b& cur,BMP4b& out){
  Mat p(pre.h,pre.w,CV_8UC3);
  Mat c(cur.h,cur.w,CV_8UC3);
  copy(pre,p);
  copy(cur,c);
  opticalflow(p,c);
  copy(c,out);
}


void
facedetect(Mat& img){
  double scale = 1.0;
  Mat gray, smallImg(saturate_cast<int>(img.rows/scale), saturate_cast<int>(img.cols/scale), CV_8UC1);
  // グレースケール画像に変換
  cvtColor(img, gray, CV_BGR2GRAY);
  // 処理時間短縮のために画像を縮小
  resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);
  equalizeHist( smallImg, smallImg);

  //  imwrite("small.png",smallImg);

  // 分類器の読み込み
  //  std::string cascadeName = "/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml"; // Haar-like
  //    std::string cascadeName = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml"; // Haar-like
  std::string cascadeName = "haarcascade_frontalface_alt.xml"; // Haar-like
  //    std::string cascadeName = "haarcascade_animeface2.xml"; // Haar-like
  //std::string cascadeName = "./lbpcascade_frontalface.xml"; // LBP
  CascadeClassifier cascade;
  if(!cascade.load(cascadeName)){
    printf("cascade can not init\n");
    return;
  }

  std::vector<Rect> faces;
  /// マルチスケール（顔）探索xo
  // 画像，出力矩形，縮小スケール，最低矩形数，（フラグ），最小矩形
  cascade.detectMultiScale(smallImg, faces,
                           1.1, 2,
                           CV_HAAR_SCALE_IMAGE,
                           Size(5, 5));

  for(int c=0;c<img.cols;c++)
    for(int r=0;r<img.rows;r++)
      img.at<Vec3b>(r,c)={0,0,0};
  
  // 結果の描画
  std::vector<Rect>::const_iterator r = faces.begin();
  for(; r != faces.end(); ++r) {
    Point center;
    int radius;
    center.x = saturate_cast<int>((r->x + r->width*0.5)*scale);
    center.y = saturate_cast<int>((r->y + r->height*0.5)*scale);
    radius = saturate_cast<int>((r->width + r->height)*0.25*scale);
    circle( img, center, radius, Scalar(80,80,255), 3, 8, 0 );
  }


  
}


void
HoughLines(Mat& src_img){

  Mat dst_img, work_img;
  //  dst_img = src_img.clone();
  cvtColor(src_img, work_img, CV_BGR2GRAY);
  Canny(work_img, work_img, 50, 200, 3);
  
  // 確率的Hough変換
  std::vector<Vec4i> lines;
  // 入力画像，出力，距離分解能，角度分解能，閾値，線分の最小長さ，
  // 2点が同一線分上にあると見なす場合に許容される最大距離
  HoughLinesP(work_img, lines, 1, CV_PI/180, 50, 50, 10);

  std::vector<Vec4i>::iterator it = lines.begin();
  
  for(int c=0;c<src_img.cols;c++)
    for(int r=0;r<src_img.rows;r++)
      src_img.at<Vec3b>(r,c)={0,0,0};

  
  for(; it!=lines.end(); ++it) {
    Vec4i l = *it;
    line(src_img, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 2, CV_AA);
  }


}

void
HoughCircles(Mat& src_img){

  Mat  work_img;
  //  dst_img = src_img.clone();
  cvtColor(src_img, work_img, CV_BGR2GRAY);

  // Hough変換のための前処理（画像の平滑化を行なわないと誤検出が発生しやすい）
  GaussianBlur(work_img, work_img, Size(11,11), 2, 2);
  
  // Hough変換による円の検出と検出した円の描画
  std::vector<Vec3f> circles;
  HoughCircles(work_img, circles, CV_HOUGH_GRADIENT, 1, 100, 20, 50);

  std::vector<Vec3f>::iterator it = circles.begin();
  for(; it!=circles.end(); ++it) {
    Point center(saturate_cast<int>((*it)[0]), saturate_cast<int>((*it)[1]));
    int radius = saturate_cast<int>((*it)[2]);
    circle(src_img, center, radius, Scalar(0,0,255), 2);
  }

}



void
opticalflow(const Mat& img1,Mat& img2){

  Mat prev, next;
  cvtColor(img1, prev, CV_BGR2GRAY);
  cvtColor(img2, next, CV_BGR2GRAY);

  std::vector<Point2f> prev_pts;
  
  // 初期化
  Size flowSize(30,30);
  Point2f center = Point(prev.cols/2., prev.rows/2.);
  for(int i=0; i<flowSize.width; ++i) {
    for(int j=0; j<flowSize.width; ++j) {
      Point2f p(i*float(prev.cols)/(flowSize.width-1), 
                    j*float(prev.rows)/(flowSize.height-1));
      prev_pts.push_back((p-center)*0.9f+center);
    }
  }

  // Gunnar Farnebackのアルゴリズムに基づくオプティカルフロー
  // 画像ピラミッド作成のスケール(<1)，ピラミッドの層数，
  // 平均化窓サイズ，各層での繰り返し数，
  // 各ピクセルの隣接領域サイズ．通常は 5 or 7，
  // ガウシアンの標準偏差，フラグ
  Mat flow;
  std::vector<float> error;
  calcOpticalFlowFarneback(prev, next, flow, 0.5, 3, 15, 3, 5, 1.1, 0);

  // オプティカルフローの表示
  std::vector<Point2f>::const_iterator p = prev_pts.begin();
  for(; p!=prev_pts.end(); ++p) {
    const Point2f& fxy = flow.at<Point2f>(p->y, p->x);
    line(img2, *p, *p+fxy, Scalar(150,0,0),2);
  }
}



Camera::Camera(){
  cap=NULL;
}
Camera::~Camera(){
  if(cap!=NULL)
    delete (cv::VideoCapture*)cap;
}
int
Camera::open(int w,int h,int devid){
  VideoCapture* capp=new VideoCapture(devid);
  VideoCapture& cap=*capp;
  cap.set(CV_CAP_PROP_FRAME_WIDTH, w);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT,h);
  if(!cap.isOpened()){
    delete capp;
    return -1;
  }
  return 0;
}
BMP4b&
Camera::get(){
  cv::Mat frame;
  ((cv::VideoCapture*)cap)->read(frame);
  copy(frame,img);
  return img;
}

BMP4b
getcamera(int w,int h,int devid){
  BMP4b a(w,h);
  VideoCapture capture(devid);
  capture.set(CV_CAP_PROP_FRAME_WIDTH, w);
  capture.set(CV_CAP_PROP_FRAME_HEIGHT, h);
  if(!capture.isOpened()) return a;

  //n  cv::namedWindow("Capture", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
  Mat frame;
  capture.read(frame);
  copy(frame,a);
  return a;
}

int
get_num_cams(){
  for(int device = 0; device<10; device++) {
    VideoCapture cap(device);
    if (!cap.isOpened())
      return device;          
  }
}
#endif

