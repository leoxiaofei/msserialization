#ifndef RAJSONSERIALIZER_HPP__
#define RAJSONSERIALIZER_HPP__

///General-purpose JSON serialization interface

#include <msserialize/pojsonnode.hpp>
#include <msserialize/msarchive_stl.hpp>

namespace MSRPC
{

template<class T>
std::string ToJsonS(const T& t, unsigned int indent = 0)
{
	MSRPC::IDoc doc;
	MSRPC::IJsonArc ia(doc);
	ia & t;

	return doc.Stringify(indent);
}

template<class T, class StrBuf>
bool FromJsonS(T& t, StrBuf& strJson)
{
	bool bRet(false);
	try
	{
		MSRPC::ODoc doc;

		if (doc.Parse(strJson))
		{
			MSRPC::OJsonArc oa(doc);
			oa & t;
			bRet = true;
		}
	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return bRet;
}

template<class T>
bool ToJsonFile(const T& t, const char* strFilePath, unsigned int indent = 0)
{
	MSRPC::IDoc doc;
	MSRPC::IJsonArc ia(doc);
	ia & t;

	return doc.Save(strFilePath, indent);
}

template<class T>
bool FromJsonFile(T& t, const char* strFilePath)
{
	bool bRet(false);

	try
	{
		MSRPC::ONodeDoc doc;

		if (doc.Load(strFilePath))
		{
			MSRPC::OJsonArc oa(doc);
			oa & t;
			bRet = true;
		}
	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return bRet;
}

}


#endif // RAJSONSERIALIZER_HPP__
