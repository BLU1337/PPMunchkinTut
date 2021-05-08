#include "Button.h"

Button::Button(int _id, const cv::Rect & _rect, const cv::Scalar& _color, bool _visible, ButtonOrigin _origin, const button_callback & _callback) :
	id{_id},
	rect{_rect},
	color{_color},
	visible{_visible},
	origin{_origin},
	callback{_callback}
{
}

bool Button::poll_click(const cv::Point & point, const cv::Size& canvas_size) const
{
	if (contains(point, canvas_size))
	{
		if (callback)
			callback(*this);
		return true;
	}
	else
		return false;
}

void Button::draw(cv::Mat & canvas) const
{
	if (visible)
	{
		int w = canvas.cols;
		int h = canvas.rows;
		int bw = rect.br().x - rect.tl().x;
		int bh = rect.br().y - rect.tl().y;
		int bx0 = rect.tl().x;
		int by0 = rect.tl().y;
		int bx1 = rect.br().x;
		int by1 = rect.br().y;

		switch (origin)
		{
			case ButtonOrigin::topleft:
				cv::rectangle(canvas, rect, cv::Scalar(color[2], color[1], color[0]), cv::FILLED);
				break;
			case ButtonOrigin::topright:
				cv::rectangle(canvas, cv::Point(w - bx0 - bw, by0), cv::Point(w - bx0, by1), cv::Scalar(color[2], color[1], color[0]), cv::FILLED);
				break;
			case ButtonOrigin::bottomleft:
				cv::rectangle(canvas, cv::Point(bx0, h - by0 - bh), cv::Point(bx1, h - by0), cv::Scalar(color[2], color[1], color[0]), cv::FILLED);
				break;
			case ButtonOrigin::bottomright:
				cv::rectangle(canvas, cv::Point(w - bx0 - bw, h - by0 - bh), cv::Point(w - bx0, h - by0), cv::Scalar(color[2], color[1], color[0]), cv::FILLED);
				break;
			default:
				break;
		}
	}
		
}

bool Button::contains(const cv::Point & pt, const cv::Size& canvas_size) const
{
	switch (origin)
	{
		case ButtonOrigin::topleft:
			return rect.contains(pt);
			break;
		case ButtonOrigin::topright:
			return cv::Rect(canvas_size.width - rect.tl().x - rect.width, rect.tl().y, rect.width, rect.height).contains(pt);
			break;
		case ButtonOrigin::bottomleft:
			return cv::Rect(rect.tl().x, canvas_size.height - rect.tl().y - rect.height, rect.width, rect.height).contains(pt);
			break;
		case ButtonOrigin::bottomright:
			return cv::Rect(canvas_size.width - rect.tl().x - rect.width, canvas_size.height - rect.tl().y - rect.height, rect.width, rect.height).contains(pt);
			break;
		default:
			break;
	}
}
