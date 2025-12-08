#ifndef NLJSONSERIALIZER_HPP__
#define NLJSONSERIALIZER_HPP__


///General JSON Serialization Interface

#include <msserialize/nljsonnode.hpp>
#include <msserialize/msarchive_stl.hpp>

namespace MSRPC
{
	typedef MSRPC::SeNodeNlohmannJson SeNode;
	typedef MSRPC::DeNodeNlohmannJson DeNode;
	typedef MSRPC::SeDocNlohmannJson SeDoc;
	typedef MSRPC::DeDocNlohmannJson DeDoc;
}

#include "jsonserializer.hpp"

namespace MSRPC
{

// template<class T, class StrBuf = std::string>
// StrBuf ToJsonS(const T& t, unsigned int indent = -1)
// {
// 	//Serialization
// 	MSRPC::SeDoc seDoc;
// 	MSRPC::SeJsonArc ia(seDoc);
// 	ia & t;

// 	//Output the JSON string
// 	return seDoc.Stringify<StrBuf>(indent);
// }

// template<class T, class StrBuf>
// bool FromJsonS(T& t, StrBuf& strJson)
// {
// 	MSRPC::DeDoc deDoc;
// 	deDoc.Parse(strJson);
// 	if (deDoc)
// 	{
// 		MSRPC::DeJsonArc oa(deDoc);
// 		oa & t;
// 	}

// 	return deDoc;
// }

// template<class T, class StrBuf>
// bool FromJsonO(T& t, const nlohmann::json& obj)
// {
// 	MSRPC::DeNode deNode(&obj);
// 	if (deNode)
// 	{
// 		MSRPC::DeJsonArc oa(deNode);
// 		oa & t;
// 	}

// 	return deNode;
// }

// template<class T>
// bool ToJsonF(const T& t, const char* strFilePath, unsigned int indent = -1)
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
// 	deDoc.Load(strFilePath);
// 	if (deDoc)
// 	{
// 		MSRPC::DeJsonArc oa(deDoc);
// 		oa & t;
// 	}

// 	return deDoc;
// }

}


#endif // NLJSONSERIALIZER_HPP__
