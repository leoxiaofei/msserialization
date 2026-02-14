#ifndef RAJSONSERIALIZER_HPP__
#define RAJSONSERIALIZER_HPP__


///General JSON Serialization Interface

#include <msadapter/msrajsonadapter.hpp>
#include <msserialize/rajsonnode.hpp>
#include <msserialize/msarchive_stl.hpp>


namespace MSRPC
{
	typedef MSRPC::SeNodeRapidJson SeNode;
	typedef MSRPC::DeNodeRapidJson DeNode;
	typedef MSRPC::SeDocRapidJson SeDoc;
	typedef MSRPC::DeDocRapidJson DeDoc;
	
} // namespace MSRPC

#include "jsonserializer.hpp"

namespace MSRPC
{

template<class T>
bool FromJsonO(T& t, const rapidjson::Value& obj)
{
	MSRPC::DeNode deNode(&obj);
	if (deNode)
	{
		deNode >> t;
	}

	return deNode;
}

template<class T>
bool ToJsonO(const T& t, rapidjson::Value& obj,
	 rapidjson::Document::AllocatorType& allocator)
{
	MSRPC::SeNode seNode(&obj, &allocator);
	seNode << t;
	return seNode;
}

// template<class T, class StrBuf>
// bool FromJsonS(T& t, StrBuf& strJson)
// {
// 	MSRPC::DeDoc deDoc;
// 	MSRPC::DeJsonArc oa(deDoc);

// 	if (deDoc.Parse(strJson) && deDoc)
// 	{
// 		oa & t;
// 	}

// 	return deDoc;
// }

// template<class T>
// bool ToJsonF(const T& t, const char* strFilePath, unsigned int indent = 0)
// {
// 	//Serialization
// 	MSRPC::SeDoc seDoc;
// 	MSRPC::SeJsonArc ia(seDoc);
// 	ia & t;

// 	return seDoc.Save(strFilePath, indent);
// }

// template<class T>
// bool FromJsonF(T& t, const char* strFilePath)
// {
// 	MSRPC::DeDoc deDoc;
// 	MSRPC::DeJsonArc oa(deDoc);

// 	if (deDoc.Load(strFilePath) && deDoc)
// 	{
// 		oa & t;
// 	}

// 	return deDoc;
// }

}


#endif // RAJSONSERIALIZER_HPP__
