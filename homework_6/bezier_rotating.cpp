#include <opencv2/core/matx.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>

#include <cmath>

void line(cv::Mat image, const cv::Point& from, const cv::Point& to, uint8_t color) {
  const int dx = std::abs(from.x - to.x), dy = std::abs(from.y - to.y);
  const int signX = from.x < to.x ? 1 : -1,
      signY = from.y < to.y ? 1 : -1;
  int error = dx - dy;
  image.at<uint8_t>(to) = color;
  cv::Point p = from;
  while (p.x != to.x || p.y != to.y) {
    image.at<uint8_t>(p) = color;
    int error2 = error << 1;
    if (error2 > -dy) {
      error -= dy;
      p.x += signX;
    }
    if (error2 < dx) {
      error += dx;
      p.y += signY;
    }
  }
}

double distance(const cv::Point& p0, const cv::Point& p1, const cv::Point& p2) {
  double k = double(p2.y - p0.y) / (p2.x - p0.x);
  double b = -k * p0.x + p0.y;
  return std::abs(-k * p1.x + p1.y - b)/std::sqrt(k * k + 1);
}

void bezier(const cv::Mat& image, const cv::Point& p0, const cv::Point& p1, const cv::Point& p2, uint8_t color) {
  if (distance(p0, p1, p2) > 1) {
    cv::Point p0_1 = (p0 + p1) / 2,
        p1_1 = (p1 + p2) / 2,
        p0_2 = (p0_1 / 2 + p1_1 / 2);
    bezier(image, p0, p0_1, p0_2, color);
    bezier(image, p0_2, p1_1, p2, color);
  } else {
    line(image, p0, p2, color);
  }
}

void bezierRotateX(const cv::Mat& image, const cv::Point& _p0, const cv::Point& _p1, const cv::Point& _p2, uint8_t color) {
  cv::Vec3f p0(_p0.x, _p0.y, 0), p1(_p1.x, _p1.y, 0), p2(_p2.x, _p2.y, 0);
  cv::Vec3f p0_ = p0, p2_ = p2;
  for (std::size_t a = 0; a < 360; a += 10) {
    float alpha = static_cast<float>(a * M_PI) / 180;
    p0_ -= p1;
    p2_ -= p1;
    float data[] = { 1,               0,                0, 0,
                     0, std::cos(alpha), -std::sin(alpha), 0,
                     0, std::sin(alpha),  std::cos(alpha), 0,
                     0,               0,                0, 1 };
    cv::Mat mx(4, 4, CV_32FC1, data);
    cv::Mat out = mx * cv::Vec4f(p0_[0], p0_[1], p0_[2], 1);
    cv::Point p0__(out.at<float>(0, 0) + p1[0], out.at<float>(0, 1) + p1[1]);
    out = mx * cv::Vec4f(p2_[0], p2_[1], p2_[2], 1);
    cv::Point p2__(out.at<float>(0, 0) + p1[0], out.at<float>(0, 1) + p1[1]);
    bezier(image, p0__, _p1, p2__, color);

    p0_ = p0;
    p2_ = p2;
  }
}

void bezierRotateY(const cv::Mat& image, const cv::Point& _p0, const cv::Point& _p1, const cv::Point& _p2, uint8_t color) {
  cv::Vec3f p0(_p0.x, _p0.y, 0), p1(_p1.x, _p1.y, 0), p2(_p2.x, _p2.y, 0);
  cv::Vec3f p0_ = p0, p2_ = p2;
  for (std::size_t a = 0; a < 360; a += 10) {
    float alpha = static_cast<float>(a * M_PI) / 180;
    p0_ -= p1;
    p2_ -= p1;
    float data[] = { std::sin(alpha), 0,  std::cos(alpha), 0,
                     0, 1,                0, 0,
                     std::cos(alpha), 0, -std::sin(alpha), 0,
                     0, 0,                0, 1 };
    cv::Mat mx(4, 4, CV_32FC1, data);
    cv::Mat out = mx * cv::Vec4f(p0_[0], p0_[1], p0_[2], 1);
    cv::Point p0__(out.at<float>(0, 0) + p1[0], out.at<float>(0, 1) + p1[1]);
    out = mx * cv::Vec4f(p2_[0], p2_[1], p2_[2], 1);
    cv::Point p2__(out.at<float>(0, 0) + p1[0], out.at<float>(0, 1) + p1[1]);
    bezier(image, p0__, _p1, p2__, color);

    p0_ = p0;
    p2_ = p2;
  }
}

void bezierRotateZ(const cv::Mat& image, const cv::Point& _p0, const cv::Point& _p1, const cv::Point& _p2, uint8_t color) {
  cv::Vec3f p0(_p0.x, _p0.y, 0), p1(_p1.x, _p1.y, 0), p2(_p2.x, _p2.y, 0);
  cv::Vec3f p0_ = p0, p2_ = p2;
  for (std::size_t a = 0; a < 360; a += 10) {
    float alpha = static_cast<float>(a * M_PI) / 180;
    p0_ -= p1;
    p2_ -= p1;
    float data[] = { std::cos(alpha), -std::sin(alpha), 0, 0,
                     std::sin(alpha),  std::cos(alpha), 0, 0,
                     0,                0, 0, 0,
                     0,                0, 0, 1 };
    cv::Mat mx(4, 4, CV_32FC1, data);
    cv::Mat out = mx * cv::Vec4f(p0_[0], p0_[1], p0_[2], 1);
    cv::Point p0__(out.at<float>(0, 0) + p1[0], out.at<float>(0, 1) + p1[1]);
    out = mx * cv::Vec4f(p2_[0], p2_[1], p2_[2], 1);
    cv::Point p2__(out.at<float>(0, 0) + p1[0], out.at<float>(0, 1) + p1[1]);
    bezier(image, p0__, _p1, p2__, color);

    p0_ = p0;
    p2_ = p2;
  }
}


struct Params {
  cv::Mat image;
  cv::Point first, second, third;
};

void mouseCallback(int event, int x, int y, int flags, void* params) {
  auto* tmp = reinterpret_cast<Params*>(params);
  switch (event) {
  case cv::EVENT_LBUTTONDOWN:
    if (tmp->first == cv::Point(-1, -1)) {
      tmp->first = cv::Point(x, y);
    } else if (tmp->second == cv::Point(-1, -1)) {
      tmp->second = cv::Point(x, y);
    } else if (tmp->third == cv::Point(-1, -1)) {
      tmp->third = cv::Point(x, y);
      tmp->image = cv::Scalar(255);
      bezier(tmp->image, tmp->first, tmp->second, tmp->third, 0);
      cv::imshow("lab06", tmp->image);
    }
    break;
  case cv::EVENT_MOUSEMOVE:
    if (tmp->first != cv::Point(-1, -1) && tmp->second != cv::Point(-1, -1)
        && tmp->third == cv::Point(-1, -1)) {
      cv::Point third(x, y);
      tmp->image = cv::Scalar(255);
      bezier(tmp->image, tmp->first, tmp->second, third, 0);
      cv::imshow("lab06", tmp->image);
    }
    break;
  case cv::EVENT_RBUTTONDOWN:
    bezierRotateX(tmp->image, tmp->first, tmp->second, tmp->third, 0);
    cv::imshow("lab06", tmp->image);
    break;
  default:
    break;
  }
}

int main() {
  Params params = { cv::Mat(500, 500, CV_8UC1, cv::Scalar(255)),
                    cv::Point(-1, -1),
                    cv::Point(-1, -1),
                    cv::Point(-1, -1)
  };
  cv::namedWindow("lab06");
  cv::imshow("lab06", params.image);
  cv::setMouseCallback("lab06", mouseCallback, &params);
  cv::waitKey(0);
  return 0;
}