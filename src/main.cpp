#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <stdio.h>
#include <vector>

using namespace cv;

// ABSOLUTE PATH REQUIRED
#define IMAGE_DIR "/bigssd/Dokumente/git/sign_detection/signs"

int main(int argc, char **argv) {
  // Find all files in dir
  DIR *dir;
  char **filenames;
  int filenames_len = 0;
  filenames = (char **)malloc(sizeof(char *));
  dir = opendir(IMAGE_DIR);
  {
    struct dirent *current_file;
    int i = 0;
    while ((current_file = readdir(dir)) != NULL) {
      filenames = (char **)realloc(filenames, (i + 1) * sizeof(char *));

      char *filename_stringed;
      filename_stringed = (char *)malloc(strlen(current_file->d_name) + 1);
      strcpy(filename_stringed, current_file->d_name);

      filenames[i] = filename_stringed;
      i++;
      filenames_len++;
    }
  }

  // Check image for signs
  printf("Filenames_len: %d\n", filenames_len);

  // i is 2 to ignore . and ..
  int minHessian = 400;
  Ptr<xfeatures2d::SURF> detector = xfeatures2d::SURF::create(minHessian);

  Mat image;
  image = imread("/bigssd/Dokumente/git/sign_detection/singnthing.jpg",
                 IMREAD_GRAYSCALE);
  for (int i = 2; i < filenames_len; i++) {
    char *filepath;
    int str_len = sizeof(filenames[i]) + strlen(IMAGE_DIR) + 3;
    filepath = (char *)malloc(str_len);
    snprintf(filepath, str_len, "%s/%s", IMAGE_DIR, filenames[i]);
    ////////////////////////////////////////////////////////////////////////////////////////////
    printf("2\n");
    ///
    Mat img_template = imread(filepath, IMREAD_GRAYSCALE);
    std::vector<KeyPoint> keypoints_template, keypoints_scene;
    Mat descriptors_template, descriptors_scene;
    detector->detectAndCompute(img_template, noArray(), keypoints_template,
                               descriptors_template);
    detector->detectAndCompute(image, noArray(), keypoints_scene,
                               descriptors_scene);

    Ptr<DescriptorMatcher> matcher =
        DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
    std::vector<std::vector<DMatch>> knn_matches;
    matcher->knnMatch(descriptors_template, descriptors_scene, knn_matches, 2);

    const float ratio_thresh = 0.5f;
    std::vector<DMatch> good_matches;
    for (int i = 0; i < knn_matches.size(); i++) {
      if (knn_matches[i][0].distance <
          ratio_thresh * knn_matches[i][1].distance)
        good_matches.push_back(knn_matches[i][0]);
    }

    Mat img_matches = image;
    drawMatches(img_template, keypoints_template, image, keypoints_scene,
                good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
                std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    std::vector<Point2f> obj, scene;
    for (int i = 0; i < good_matches.size(); i++) {
      obj.push_back(keypoints_template[good_matches[i].queryIdx].pt);
      scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
    }

    Mat H = findHomography(obj, scene, RANSAC);
    std::vector<Point2f> template_corners(4);
    template_corners[0] = Point2f(0, 0);
    template_corners[1] = Point2f((float)img_template.cols, 0);
    template_corners[2] =
        Point2f((float)img_template.cols, (float)img_template.rows);
    template_corners[3] = Point2f(0, img_template.rows);
    std::vector<Point2f> scene_corners(4);

    perspectiveTransform(template_corners, scene_corners, H);

    line(img_matches, scene_corners[0] + Point2f((float)img_template.cols, 0),
         scene_corners[1] + Point2f((float)img_template.cols, 0),
         Scalar(0, 255, 0), 4);
    line(img_matches, scene_corners[1] + Point2f((float)img_template.cols, 0),
         scene_corners[2] + Point2f((float)img_template.cols, 0),
         Scalar(0, 255, 0), 4);
    line(img_matches, scene_corners[2] + Point2f((float)img_template.cols, 0),
         scene_corners[3] + Point2f((float)img_template.cols, 0),
         Scalar(0, 255, 0), 4);
    line(img_matches, scene_corners[3] + Point2f((float)img_template.cols, 0),
         scene_corners[0] + Point2f((float)img_template.cols, 0),
         Scalar(0, 255, 0), 4);

    imwrite("PlsWork.jpg", img_matches);
    ////////////////////////////////////////////////////////////////////////////////////////////
  }
  return 0;
}
