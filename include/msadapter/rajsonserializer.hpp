#ifndef RAJSONSERIALIZER_HPP__
#define RAJSONSERIALIZER_HPP__


///General JSON Serialization Interface

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

template<class T, class StrBuf = std::string>
StrBuf ToJsonS(const T& t, unsigned int indent = 0)
{
	//Serialization
	MSRPC::IDoc iDoc;
	MSRPC::IJsonArc ia(iDoc);
	ia & t;

	//Output the JSON string
	return iDoc.Stringify<StrBuf>(indent);
}

template<class T, class StrBuf>
bool FromJsonS(T& t, StrBuf& strJson)
{
	rapidjson::Document doc;
	if(std::is_const_v<StrBuf>)
	{
		doc.Parse(strJson.data(), strJson.size());
	}
	else
	{
		doc.ParseInsitu((char*)strJson.data());
	}

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
	//Serialization
	rapidjson::Document doc;
	MSRPC::IJsonArc::Node nObjI(&doc);
	MSRPC::IJsonArc ia(nObjI);
	ia & t;

	//Output the JSON string
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
