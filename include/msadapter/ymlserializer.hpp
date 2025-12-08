#ifndef JSONSERIALIZER_HPP__
#define JSONSERIALIZER_HPP__


///General JSON Serialization Interface


#include <fstream>


namespace MSRPC
{

template<class T, class StrBuf = std::string, class SEDOC = MSRPC::SeDoc, class SENODE = MSRPC::SeNode>
StrBuf ToYamlS(const T& t)
{
	//Serialization
	typedef MSRPC::IArchiveHelper<SENODE> SeYamlArc;

	SEDOC seDoc;
	SeYamlArc ia(seDoc);
	ia & t;

	//Output the JSON string
	return seDoc.Stringify<StrBuf>();
}

template<class T, class StrBuf, class DEDOC = MSRPC::DeDoc, class DENODE = MSRPC::DeNode>
bool FromYamlS(T& t, StrBuf& strYaml)
{
	typedef MSRPC::OArchiveHelper<DENODE> DeYamlArc;

	DEDOC deDoc;
	deDoc.Parse(strYaml);
	if (deDoc)
	{
		DeYamlArc oa(deDoc);
		oa & t;
	}

	return deDoc;
}

template<class T, class SEDOC = MSRPC::SeDoc, class SENODE = MSRPC::SeNode>
bool ToYamlF(const T& t, const char* strFilePath)
{
	typedef MSRPC::IArchiveHelper<SENODE> SeYamlArc;
	//Serialization
	SEDOC seDoc;
	SeYamlArc ia(seDoc);
	ia & t;

	return seDoc.Save(strFilePath);
}

template<class T, class DEDOC = MSRPC::DeDoc, class DENODE = MSRPC::DeNode>
bool FromYamlF(T& t, const char* strFilePath)
{
	typedef MSRPC::OArchiveHelper<DENODE> DeYamlArc;

	DEDOC deDoc;
	deDoc.Load(strFilePath);
	if (deDoc)
	{
		DeYamlArc oa(deDoc);
		oa & t;
	}

	return deDoc;
}

}


#endif // JSONSERIALIZER_HPP__
