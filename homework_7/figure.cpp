#include <algorithm>
#include <numeric>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

void line(cv::Mat image, const cv::Point &from, const cv::Point &to,
          const cv::Vec3b &color) {
  const int dx = std::abs(from.x - to.x), dy = std::abs(from.y - to.y);
  const int signX = from.x < to.x ? 1 : -1, signY = from.y < to.y ? 1 : -1;
  int error = dx - dy;
  image.at<cv::Vec3b>(to) = color;
  cv::Point p = from;
  while (p.x != to.x || p.y != to.y) {
    image.at<cv::Vec3b>(p) = color;
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

void polygon(const cv::Mat &image, std::vector<cv::Point> const &vertices,
             bool flag, const cv::Vec3b &color) {
  for (std::size_t i = 0; i < vertices.size() - 1; ++i) {
    line(image, vertices[i], vertices[i + 1], color);
  }
  if (flag)
    line(image, vertices.back(), vertices.front(), color);
}

void fillPolygon(cv::Mat image, std::vector<cv::Point> const &vertices,
                 const cv::Vec3b &color) {
  int maxy = std::accumulate(std::next(vertices.begin()), vertices.end(),
                             vertices.front(),
                             [](const cv::Point &p1, const cv::Point &p2) {
                               return p1.y > p2.y ? p1 : p2;
                             }).y;
  int miny = std::accumulate(std::next(vertices.begin()), vertices.end(),
                             vertices.front(),
                             [](const cv::Point &p1, const cv::Point &p2) {
                               return p1.y > p2.y ? p2 : p1;
                             }).y;
  std::vector<std::vector<double>> arrY(maxy);
  for (std::size_t i = 0; i < vertices.size(); ++i) {
    std::size_t next = 0;
    if (i != vertices.size() - 1)
      next = i + 1;
    std::size_t up, down;
    if (vertices[i].y > vertices[next].y) {
      up = i;
      down = next;
    } else if (vertices[i].y < vertices[next].y) {
      up = next;
      down = i;
    } else {
      continue;
    }

    double k = double(vertices[up].y - vertices[down].y) /
               (vertices[up].x - vertices[down].x);
    for (std::size_t j = vertices[down].y; j < vertices[up].y; ++j) {
      arrY[j].push_back(static_cast<float>(j - vertices[down].y) / k +
                        vertices[down].x);
    }
    for (std::size_t y = miny; y < maxy; ++y) {
      auto arrX = arrY[y];
      std::sort(arrX.begin(), arrX.end());
      for (std::size_t j = 0; j < arrX.size() / 2; ++j) {
        for (auto x = arrX[j << 1]; x < arrX[(j << 1) + 1]; ++x) {
          image.at<cv::Vec3b>(y, std::floor(x)) = color;
        }
      }
    }
  }
}

float cosine(const cv::Vec3f& a, const cv::Vec3f& b) {
  return a.dot(b) /
         (std::sqrt(std::pow(a[0], 2) + std::pow(a[1], 2) + std::pow(a[2], 2)) *
          std::sqrt(std::pow(b[0], 2) + std::pow(b[1], 2) + std::pow(b[2], 2)));
}

void renderDiamond(const cv::Mat& image) {
  int x0 = 250, y0 = 200, z0 = -200;
  std::vector<cv::Vec3f> diamondVertices = {
      cv::Vec3f(x0 + 0, y0 + 0, z0 + 78),
      cv::Vec3f(x0 + 45, y0 + 45, z0 + 0),
      cv::Vec3f(x0 + 45, y0 + -45, z0 + 0),
      cv::Vec3f(x0 - 45, y0 + -45, z0 + 0),
      cv::Vec3f(x0 - 45, y0 + 45, z0 + 0),
      cv::Vec3f(x0 + 0, y0 + 0, z0 - 78)};
  float alphaX = M_PI / 2, alphaY = M_PI / 6;

  float dataX[] = {1, 0, 0, 0,
                   0, std::cos(alphaX), -std::sin(alphaX), 0,
                   0, std::sin(alphaX), std::cos(alphaX), 0,
                   0, 0, 0, 1};

  float dataY[] = {std::sin(alphaY), 0, std::cos(alphaY),  0, 0, 1, 0, 0,
                   std::cos(alphaY), 0, -std::sin(alphaY), 0, 0, 0, 0, 1};
  cv::Mat mx(4, 4, CV_32FC1, dataX);
  cv::Mat my(4, 4, CV_32FC1, dataY);
  for (auto &elem : diamondVertices) {
    cv::Mat rotated = mx * cv::Vec4f(elem[0], elem[1], elem[2], 1);
    rotated = my * rotated;
    elem = cv::Vec3f(rotated.at<float>(0, 0), rotated.at<float>(0, 1),
                     rotated.at<float>(0, 2));
  }
  std::vector<std::vector<size_t>> diamondFacets = {
      {0, 1, 2}, {0, 2, 3}, {0, 3, 4}, {0, 4, 1},
      {5, 4, 3}, {5, 3, 2}, {5, 2, 1}, {5, 1, 4}};
  float I_fon = 20, I_p = 235, K_p = 0.4;
  cv::Vec3f lamp(50, 20, 5);
  for (const auto &elem : diamondFacets) {
    auto p0 = diamondVertices[elem[0]], p1 = diamondVertices[elem[1]],
         p2 = diamondVertices[elem[2]];
    auto center12 = (p1 + p2) / 2;
    auto center = (center12 + p0) / 2;
    auto light = lamp - center;
    cv::Vec3f normal(
        (p1[1] - p0[1]) * (p2[2] - p0[2]) - (p2[1] - p0[1]) * (p1[2] - p0[2]),
        (p2[0] - p0[0]) * (p1[2] - p0[2]) - (p1[0] - p0[0]) * (p2[2] - p0[2]),
        (p1[0] - p0[0]) * (p2[1] - p0[1]) - (p2[0] - p0[0]) * (p1[1] - p0[1]));
    cv::Vec3f observer(0, 0, -1);
    if (cosine(normal, observer) >= 0) {
      float cosP = cosine(light, normal);
      float I = I_fon + K_p * I_p * cosP;
      cv::Point p0_ = cv::Point(p0[0], p0[1]), p1_ = cv::Point(p1[0], p1[1]),
                p2_ = cv::Point(p2[0], p2[1]);
      fillPolygon(image, {p0_, p1_, p2_}, cv::Vec3b(I, I, I));
      line(image, p0_, p1_, cv::Vec3b(I, I, I));
      line(image, p0_, p2_, cv::Vec3b(I, I, I));
      line(image, p2_, p1_, cv::Vec3b(I, I, I));
    }
  }
}

int main() {
  cv::Mat image(800, 800, CV_8UC3, cv::Scalar(235, 235, 235));
  renderDiamond(image);
  cv::namedWindow("lab07");
  cv::imshow("lab07", image);
  cv::waitKey();
  return 0;
}