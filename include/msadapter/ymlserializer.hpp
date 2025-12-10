#ifndef JSONSERIALIZER_HPP__
#define JSONSERIALIZER_HPP__


///General JSON Serialization Interface


#include <fstream>


namespace MSRPC
{

template<class T, class StrBuf = std::string, class SEDOC = MSRPC::SeDoc>
StrBuf ToYamlS(const T& t)
{
	//Serialization
	SEDOC seDoc;
	seDoc << t;

	//Output the JSON string
	return seDoc.template Stringify<StrBuf>();
}

template<class T, class StrBuf, class DEDOC = MSRPC::DeDoc>
bool FromYamlS(T& t, StrBuf& strYaml)
{
	DEDOC deDoc;
	deDoc.Parse(strYaml);
	if (deDoc)
	{
		deDoc >> t;
	}

	return deDoc;
}

template<class T, class SEDOC = MSRPC::SeDoc>
bool ToYamlF(const T& t, const char* strFilePath)
{
	//Serialization
	SEDOC seDoc;
	seDoc << t;

	return seDoc.Save(strFilePath);
}

template<class T, class DEDOC = MSRPC::DeDoc>
bool FromYamlF(T& t, const char* strFilePath)
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
