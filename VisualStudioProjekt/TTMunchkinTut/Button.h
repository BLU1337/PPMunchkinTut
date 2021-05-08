/** \file Button.h
*	\author Benjamin Lueben
*
*	\brief Class to define buttons which are used for user input

	With this class different buttons can be defined which can be used for user input.
	Buttons are used when the user needs to confirm an action or to exit the tutorial.

*/

#pragma once
#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <functional>

class Button;
using button_callback = std::function<void(const Button&)>;

/**
	\brief specifies in what corner the button should be shown
*/
enum class ButtonOrigin
{
	topleft,		///< top left corner
	topright,		///< top right corner
	bottomleft,		///< bottom left corner
	bottomright		///< bottom right corner
};

/**
	\brief class for Buttons
*/
class Button
{
public:
	/**
	\brief constructor for a button
	\param _id Button id
	\param _rect rectangle which defines the dimensions of the button
	\param _color defines the color of the button in GBR 
	\param _visible used to hide button when not needed
	\param _origin origin of the button as in enum class ButtonOrigin
	\param _callback callback function for button. Can be changed so that one button can serve mulitple purposes if necessary
	*/
	Button(int _id, const cv::Rect& _rect, const cv::Scalar& _color, bool _visible = true, ButtonOrigin _origin = ButtonOrigin::topleft, const button_callback& _callback = {});
	/// Default copy constructor
	Button(const Button&) = default;
	/// Default move constructor
	Button(Button&&) = default;
	/// Default copy assignment
	Button& operator=(const Button&) = default;
	/// Default move assignment
	Button& operator=(Button&&) = default;

	/**
	\brief function to check if click was inside the button
	\param point point of click
	\param canvas_size size of the canvas that defines the button size
	*/
	bool poll_click(const cv::Point& point, const cv::Size& canvas_size) const;

	/**
	\brief draw function for buttons
	\param canvas defines the button that should be drawn
	*/
	void draw(cv::Mat& canvas) const;

//private:
	int id;							///< Button id
	cv::Rect rect;					///< rectangle that defines size of button
	button_callback callback;		///< callback function for button to apply action to button
	cv::Scalar color;				///< color of the button
	bool visible;					///< wether or not button should be visible
	ButtonOrigin origin;			///< position of the button

private:

	/**
		\brief function that checks if given point is inside buton
		\param pt point of click
		\param canvas_size canvas size that defines the size of the button
	*/
	bool contains(const cv::Point& pt, const cv::Size& canvas_size) const;
};