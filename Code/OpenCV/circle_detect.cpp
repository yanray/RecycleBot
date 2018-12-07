#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>

void check_program_arguments(int argc) {
	if(argc != 2) {
		std::cout << "Error! Program usage:" << std::endl;
		std::cout << "./circle_detect image_circles_path" << std::endl;
		std::exit(-1);
	}	
}

void check_if_image_exist(const cv::Mat &img, const std::string &path) {
	if(img.empty()) {
		std::cout << "Error! Unable to load image: " << path << std::endl;
		std::exit(-1);
	}	
}

// convert coordinate linearly
// first four numbers are 2 sample points, returns the y value corsponding to x
double convert(double x1, double y1, double x2, double y2, double x) {

	double slope = (y2 - y1)/(x2 - x1);
	double intercept = y1 - x1*slope;
	return x * slope+intercept;
}


int main(int argc, char **argv) {
	// Usage: ./circle_detect image_circles_path
	check_program_arguments(argc);
	
	// Load input image
	std::string path_image{argv[1]};
	cv::Mat bgr_image = cv::imread(path_image);

	// Check if the image can be loaded
	check_if_image_exist(bgr_image, path_image);

	cv::Mat orig_image = bgr_image.clone();

	cv::medianBlur(bgr_image, bgr_image, 3);

	// Convert input image to HSV
	cv::Mat hsv_image;
	cv::cvtColor(bgr_image, hsv_image, cv::COLOR_BGR2HSV);

	// Threshold the HSV image, keep only the red pixels
	cv::Mat lower_red_hue_range;
	cv::Mat upper_red_hue_range;
	cv::inRange(hsv_image, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), lower_red_hue_range);
	cv::inRange(hsv_image, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), upper_red_hue_range);

	// Combine the above two images
	cv::Mat red_hue_image;
	cv::addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);

	cv::GaussianBlur(red_hue_image, red_hue_image, cv::Size(9, 9), 2, 2);

	// Use the Hough transform to detect circles in the combined threshold image
	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(red_hue_image, circles, CV_HOUGH_GRADIENT, 1, red_hue_image.rows/8, 100, 20, 0, 0);

	// Loop over all detected circles and outline them on the original image
	if(circles.size() == 0) std::exit(-1);
	for(size_t current_circle = 0; current_circle < circles.size(); ++current_circle) {
		/* Now need to cnvert from pixel value to real distance
		 * We are using a spline interpolation program to map the coordinates
		 */
		
		double x_dist = convert(2110, 0.1, 743, 0.25, center.y); 
		double y_dist = convert(833, -0.1, 2723, 0.1, center.x);
		
		std::cout<<x_dist<<" "<<y_dist<<" "<<0.08<<std::endl;
		
	}	
	return 0;
}
