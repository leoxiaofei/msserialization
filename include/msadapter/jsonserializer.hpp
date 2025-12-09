#ifndef JSONSERIALIZER_HPP__
#define JSONSERIALIZER_HPP__


///General JSON Serialization Interface


#include <fstream>


namespace MSRPC
{

template<class T, class StrBuf = std::string, class SEDOC = MSRPC::SeDoc>
StrBuf ToJsonS(const T& t, unsigned int indent = 0)
{
	//Serialization

	SEDOC seDoc;
	seDoc << t;

	//Output the JSON string
	return seDoc.Stringify<StrBuf>(indent);
}

template<class T, class StrBuf, class DEDOC = MSRPC::DeDoc>
bool FromJsonS(T& t, StrBuf& strJson)
{
	DEDOC deDoc;
	deDoc.Parse(strJson);
	if (deDoc)
	{
		deDoc >> t;
	}

	return deDoc;
}

template<class T, class SEDOC = MSRPC::SeDoc>
bool ToJsonF(const T& t, const char* strFilePath, unsigned int indent = 0)
{
	//Serialization
	SEDOC seDoc;
	seDoc << t;

	return seDoc.Save(strFilePath, indent);
}

template<class T, class DEDOC = MSRPC::DeDoc>
bool FromJsonF(T& t, const char* strFilePath)
{
	DEDOC deDoc;
	deDoc.Load(strFilePath);
	if (deDoc)
	{
		deDoc >> t;
	}

	return deDoc;
}

}


#endif // JSONSERIALIZER_HPP__
