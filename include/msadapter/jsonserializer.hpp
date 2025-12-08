#ifndef JSONSERIALIZER_HPP__
#define JSONSERIALIZER_HPP__


///General JSON Serialization Interface


#include <fstream>


namespace MSRPC
{

template<class T, class StrBuf = std::string, class SEDOC = MSRPC::SeDoc, class SENODE = MSRPC::SeNode>
StrBuf ToJsonS(const T& t, unsigned int indent = 0)
{
	//Serialization
	typedef MSRPC::IArchiveHelper<SENODE> SeJsonArc;

	SEDOC seDoc;
	SeJsonArc ia(seDoc);
	ia & t;

	//Output the JSON string
	return seDoc.Stringify<StrBuf>(indent);
}

template<class T, class StrBuf, class DEDOC = MSRPC::DeDoc, class DENODE = MSRPC::DeNode>
bool FromJsonS(T& t, StrBuf& strJson)
{
	typedef MSRPC::OArchiveHelper<DENODE> DeJsonArc;

	DEDOC deDoc;
	deDoc.Parse(strJson);
	if (deDoc)
	{
		DeJsonArc oa(deDoc);
		oa & t;
	}

	return deDoc;
}

template<class T, class SEDOC = MSRPC::SeDoc, class SENODE = MSRPC::SeNode>
bool ToJsonF(const T& t, const char* strFilePath, unsigned int indent = 0)
{
	typedef MSRPC::IArchiveHelper<SENODE> SeJsonArc;
	//Serialization
	SEDOC seDoc;
	SeJsonArc ia(seDoc);
	ia & t;

	return seDoc.Save(strFilePath, indent);
}

template<class T, class DEDOC = MSRPC::DeDoc, class DENODE = MSRPC::DeNode>
bool FromJsonF(T& t, const char* strFilePath)
{
	typedef MSRPC::OArchiveHelper<DENODE> DeJsonArc;

	DEDOC deDoc;
	deDoc.Load(strFilePath);
	if (deDoc)
	{
		DeJsonArc oa(deDoc);
		oa & t;
	}

	return deDoc;
}

}


#endif // JSONSERIALIZER_HPP__
