#ifndef RAJSONSERIALIZER_HPP__
#define RAJSONSERIALIZER_HPP__


///ͨ��Json���л��ӿ�

#include <msserialize/pojsonnode.hpp>
#include <msserialize/msarchive_stl.hpp>
#include <fstream>

namespace MSRPC
{

template<class T>
std::string ToJsonS(const T& t, unsigned int indent = 0)
{
	//���л�
	Poco::Dynamic::Var doc;
	MSRPC::IJsonArc::Node nObjI(&doc);
	MSRPC::IJsonArc ia(nObjI);
	ia & t;

	//���json�ַ���
	std::ostringstream os;
	Poco::JSON::Stringifier::stringify(doc, os, indent, 1, Poco::JSON_WRAP_STRINGS);

	return os.str();
}

template<class T, class StrBuf>
bool FromJsonS(T& t, StrBuf& strJson)
{
	Poco::JSON::Parser parser;
	Poco::Dynamic::Var doc = parser.parse(strJson);

	MSRPC::OJsonArc::Node objO(&doc);
	MSRPC::OJsonArc oa(objO);

	if (objO)
	{
		oa & t;
	}

	return objO;
}

template<class T>
bool ToJsonFile(const T& t, const char* strFilePath, unsigned int indent = 0)
{
	//���л�
	Poco::Dynamic::Var doc;
	MSRPC::IJsonArc::Node nObjI(&doc);
	MSRPC::IJsonArc ia(nObjI);
	ia & t;

	bool bRet = false;
	//���json
	std::ofstream outfile(strFilePath);
	if (outfile)
	{
		Poco::JSON::Stringifier::stringify(doc, outfile, indent, 1, Poco::JSON_WRAP_STRINGS);
		bRet = true;
	}

	return bRet;
}

template<class T>
bool FromJsonFile(T& t, const char* strFilePath)
{
	bool bRet(false);

	std::ifstream ifs(strFilePath);
	if(ifs)
	{
		Poco::JSON::Parser parser;
		Poco::Dynamic::Var doc = parser.parse(ifs);

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
