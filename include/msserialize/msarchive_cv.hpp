#ifndef MSARCHIVE_CV_H__
#define MSARCHIVE_CV_H__

#include "msarchive_stl.hpp"



namespace MSRPC
{
	template<class NODE, class T>
	class ISerialize<NODE, cv::Point_<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const cv::Point_<T>& tValue)
		{
			std::stringstream ss;
			ss << tValue.x << ',' << tValue.y;
			ISerialize<NODE, std::string>::serialize(vNewNode, ss.str());
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, cv::Point_<T> >
	{
	public:
		static void serialize(const NODE& vNewNode, cv::Point_<T>& tValue)
		{
			std::string strValue;
			OSerialize<NODE, std::string>::serialize(vNewNode, strValue);
			std::stringstream ss(strValue);
			char ch(0);
			ss >> tValue.x >> ch >> tValue.y;
		}
	};

	template<class NODE, class T>
	class ISerialize<NODE, cv::Rect_<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const cv::Rect_<T>& tValue)
		{
			std::stringstream ss;
			ss << tValue.x << ',' << tValue.y << ',' << tValue.width << ',' << tValue.height;
			ISerialize<NODE, std::string>::serialize(vNewNode, ss.str());
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, cv::Rect_<T> >
	{
	public:
		static void serialize(const NODE& vNewNode, cv::Rect_<T>& tValue)
		{
			std::string strValue;
			OSerialize<NODE, std::string>::serialize(vNewNode, strValue);
			std::stringstream ss(strValue);
			char ch(0);
			ss >> tValue.x >> ch >> tValue.y >> ch >> tValue.width >> ch >> tValue.height;
		}
	};

}

#endif // MSARCHIVE_CV_H__
