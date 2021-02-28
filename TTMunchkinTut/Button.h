#pragma once
#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <functional>

class Button;
using button_callback = std::function<void(const Button&)>;

enum class ButtonOrigin
{
	topleft,
	topright,
	bottomleft,
	bottomright
};

class Button
{
public:
	Button(int _id, const cv::Rect& _rect, const cv::Scalar& _color, bool _visible = true, ButtonOrigin _origin = ButtonOrigin::topleft, const button_callback& _callback = {});
	Button(const Button&) = default;
	Button(Button&&) = default;
	Button& operator=(const Button&) = default;
	Button& operator=(Button&&) = default;

	bool poll_click(const cv::Point& point, const cv::Size& canvas_size) const;
	void draw(cv::Mat& canvas) const;

//private:
	int id;
	cv::Rect rect;
	button_callback callback;
	cv::Scalar color;
	bool visible;
	ButtonOrigin origin;

private:
	bool contains(const cv::Point& pt, const cv::Size& canvas_size) const;
};