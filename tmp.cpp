
// NOTE: SIGN DETECTION
Ptr<xfeatures2d::SURF> detector = xfeatures2d::SURF::create();
std::vector<KeyPoint> keypoints_scene, keypoints_template;
Mat descriptors_template, descriptors_scene;

detector->detectAndCompute(image, noArray(), keypoints_scene,
                           descriptors_scene);
detector->detectAndCompute(img_template, noArray(), keypoints_template,
                           descriptors_template);

Ptr<DescriptorMatcher> matcher =
    DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
std::vector<cv::DMatch> matches;
matcher->match(descriptors_template, descriptors_scene, matches);

std::sort(matches.begin(), matches.end());
const int numGoodMatches = matches.size() * 0.15;
matches.erase(matches.begin() + numGoodMatches, matches.end());

Mat img_matches;
// drawMatches(img_template, keypoints_template, image, keypoints_scene,
//             matches, img_matches);

std::vector<Point2f> template_points;
std::vector<Point2f> scene_points;

for (size_t i = 0; i < matches.size(); i++) {
  template_points.push_back(keypoints_template[matches[i].queryIdx].pt);
  scene_points.push_back(keypoints_scene[matches[i].trainIdx].pt);
}

Mat H = findHomography(template_points, scene_points, RANSAC);

// Get the corners from the template image
std::vector<Point2f> template_corners(4);
template_corners[0] = Point2f(0, 0);
template_corners[1] = Point2f((float)img_template.cols, 0);
template_corners[2] =
    Point2f((float)img_template.cols, (float)img_template.rows);
template_corners[3] = Point2f(0, (float)img_template.rows);

std::vector<Point2f> scene_corners(4);
perspectiveTransform(template_corners, scene_corners, H);

// Draw lines between the corners (the mapped object in the scene - image)
line(image, scene_corners[0] + Point2f((float)img_template.cols, 0),
     scene_corners[1] + Point2f((float)img_template.cols, 0), Scalar(0, 255, 0),
     4);
line(image, scene_corners[1] + Point2f((float)img_template.cols, 0),
     scene_corners[2] + Point2f((float)img_template.cols, 0), Scalar(0, 255, 0),
     4);
line(image, scene_corners[2] + Point2f((float)img_template.cols, 0),
     scene_corners[3] + Point2f((float)img_template.cols, 0), Scalar(0, 255, 0),
     4);
line(image, scene_corners[3] + Point2f((float)img_template.cols, 0),
     scene_corners[0] + Point2f((float)img_template.cols, 0), Scalar(0, 255, 0),
     4);
