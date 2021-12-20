#include <algorithm>
#include <numeric>
#include <cstdint>
#include <vector>
#include <cmath>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

void line(cv::Mat image, const cv::Point &from, const cv::Point &to,
          uint8_t color) {
  const auto dx = std::abs(from.x - to.x), dy = std::abs(from.y - to.y);
  const auto signX = from.x < to.x ? 1 : -1, signY = from.y < to.y ? 1 : -1;
  auto error = dx - dy;
  image.at<uint8_t>(to) = color;
  cv::Point p = from;
  while (p.x != to.x || p.y != to.y) {
    image.at<uint8_t>(p) = color;
    auto error2 = error << 1;
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

void polygon(const cv::Mat& image, std::vector<cv::Point> const& vertices,
             bool flag, uint8_t color) noexcept {
  for (std::size_t i = 0; i < vertices.size() - 1; ++i) {
    line(image, vertices[i], vertices[i + 1], color);
  }
  if (flag)
    line(image, vertices.back(), vertices.front(), color);
}

void fillPolygon(cv::Mat image, std::vector<cv::Point> const &vertices,
                 uint8_t color) noexcept {
  auto maxy = std::accumulate(std::next(vertices.begin()), vertices.end(),
                             vertices.front(),
                             [](const cv::Point &p1, const cv::Point &p2) {
                               return p1.y > p2.y ? p1 : p2;
                             }).y;
  auto miny = std::accumulate(std::next(vertices.begin()), vertices.end(),
                             vertices.front(),
                             [](const cv::Point &p1, const cv::Point &p2) {
                               return p1.y > p2.y ? p2 : p1;
                             }).y;
  std::vector<std::vector<float>> arrY(maxy);
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

    auto k = static_cast<float>(vertices[up].y - vertices[down].y) /
          static_cast<float>(vertices[up].x - vertices[down].x);
    for (int j = vertices[down].y; j < vertices[up].y; ++j) {
      arrY[j].push_back(static_cast<float>(j - vertices[down].y) / k +
                        vertices[down].x);
    }
    for (int y = miny; y < maxy; ++y) {
      auto arrX = arrY[y];
      std::sort(arrX.begin(), arrX.end());
      for (std::size_t j = 0; j < arrX.size(); j += 2) {
        uint8_t* row = image.ptr(y);
        for (float x = arrX[j]; x < arrX[j + 1]; x += 1.0f)
          row[static_cast<int>(std::floor(x))] = color;
      }
    }
  }
}



struct Params {
  cv::Mat image;
  std::vector<cv::Point> vertices;
  bool isDrawn;
};

void mouseCallback(int event, int x, int y, int flags, void* params) noexcept {
  auto* tmp = reinterpret_cast<Params*>(params);
  switch (event) {
  case cv::EVENT_LBUTTONDOWN:
    if (!tmp->isDrawn)
      tmp->vertices.emplace_back(x, y);
    break;
  case cv::EVENT_MOUSEMOVE:
    if (!tmp->vertices.empty() && !tmp->isDrawn) {
      tmp->image = cv::Scalar(255);
      polygon(tmp->image, tmp->vertices, tmp->isDrawn, 0);
      cv::Point from = tmp->vertices.back();
      line(tmp->image, from, cv::Point(x, y), 0);
      cv::imshow("lab05", tmp->image);
    }
    break;
  case cv::EVENT_RBUTTONDOWN:
    if (!tmp->vertices.empty()) {
      tmp->isDrawn = true;
      tmp->image = cv::Scalar(255);
      polygon(tmp->image, tmp->vertices, tmp->isDrawn, 0);
      fillPolygon(tmp->image, tmp->vertices, 0);
      cv::imshow("lab05", tmp->image);
    }
    break;
  default:
    break;
  }
}

int main() {
  Params params = {cv::Mat(500, 500, CV_8UC1, cv::Scalar(255)),
                   std::vector<cv::Point>(), false};
  cv::namedWindow("lab05");
  cv::imshow("lab05", params.image);
  cv::setMouseCallback("lab05", mouseCallback, &params);
  cv::waitKey(0);
  return EXIT_SUCCESS;
}