#ifndef RAJSONSERIALIZER_HPP__
#define RAJSONSERIALIZER_HPP__


///通用Json序列化接口

#include <msadapter/msrajsonadapter.hpp>
#include <msserialize/rajsonnode.hpp>
#include <msserialize/msarchive_stl.hpp>
#include <rapidjson/prettywriter.h>
#include <rapidjson/writer.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <fstream>

namespace MSRPC
{

template<class RsvBuffer>
bool BuffDocWrite(rapidjson::Document& doc, RsvBuffer& buffer, bool bFormat)
{
	bool bRet = false;

	if (bFormat)
	{
		rapidjson::PrettyWriter<RsvBuffer> writer(buffer);
		bRet = doc.Accept(writer);
	}
	else
	{
		rapidjson::Writer<RsvBuffer> writer(buffer);
		bRet = doc.Accept(writer);
	}

	return bRet;
}

template<class T>
std::string ToJsonS(const T& t, bool bFormat = false)
{
	//序列化
	rapidjson::Document doc;
	MSRPC::IJsonArc::Node nObjI(&doc);
	MSRPC::IJsonArc ia(nObjI);
	ia & t;

	//输出json字符串
	std::string strRet;
	typedef MSRPC::TBufferAdapter<std::string> RsvBuffer;
	RsvBuffer buffer(strRet);

	BuffDocWrite(doc, buffer, bFormat);

	return strRet;
}

template<class T, class StrBuf>
bool FromJsonS(T& t, StrBuf& strJson)
{
	rapidjson::Document doc;
	doc.Parse(strJson.data(), strJson.size());

	MSRPC::OJsonArc::Node objO(&doc);
	MSRPC::OJsonArc oa(objO);

	if (objO)
	{
		oa & t;
	}

	return objO;
}

template<class T>
bool ToJsonFile(const T& t, const char* strFilePath, bool bFormat = false)
{
	//序列化
	rapidjson::Document doc;
	MSRPC::IJsonArc::Node nObjI(&doc);
	MSRPC::IJsonArc ia(nObjI);
	ia & t;

	//输出json
	std::ofstream outfile(strFilePath);

	rapidjson::OStreamWrapper osw(outfile);

	return outfile ? BuffDocWrite(doc, osw, bFormat) : false;
}

template<class T>
bool FromJsonFile(T& t, const char* strFilePath)
{
	bool bRet(false);
	rapidjson::Document doc;
	std::ifstream ifs(strFilePath);
	if(ifs)
	{
		rapidjson::IStreamWrapper isw(ifs);
		doc.ParseStream(isw);

		MSRPC::OJsonArc::Node objO(&doc);
		MSRPC::OJsonArc oa(objO);

		if (objO)
		{
			oa & t;
			bRet = true;
		}
	}

	return bRet;
}

}


#endif // RAJSONSERIALIZER_HPP__
