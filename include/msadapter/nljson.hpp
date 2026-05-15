#ifndef NLJSONSERIALIZER_HPP__
#define NLJSONSERIALIZER_HPP__


///General JSON Serialization Interface

#include <msserialize/nljsonnode.hpp>
#include <msserialize/msarchive_stl.hpp>
#include <fstream>

// ============================================================================
// NlJson adapter namespace
// ============================================================================
namespace MSRPC
{
namespace NlJson
{
	typedef MSRPC::SeNodeNlohmannJson SeNode;
	typedef MSRPC::DeNodeNlohmannJson DeNode;
	typedef MSRPC::SeDocNlohmannJson SeDoc;
	typedef MSRPC::DeDocNlohmannJson DeDoc;

	template<class T, class StrBuf = std::string>
	StrBuf ToJsonS(const T& t, unsigned int indent = 0)
	{
		SeDoc seDoc;
		seDoc << t;
		return seDoc.template Stringify<StrBuf>(indent);
	}

	template<class T, class StrBuf>
	bool FromJsonS(T& t, StrBuf& strJson)
	{
		DeDoc deDoc;
		deDoc.Parse(strJson);
		if (deDoc)
		{
			deDoc >> t;
		}
		return deDoc;
	}

	template<class T>
	bool ToJsonF(const T& t, const char* strFilePath, unsigned int indent = 0)
	{
		SeDoc seDoc;
		seDoc << t;
		return seDoc.Save(strFilePath, indent);
	}

	template<class T>
	bool FromJsonF(T& t, const char* strFilePath)
	{
		DeDoc deDoc;
		deDoc.Load(strFilePath);
		if (deDoc)
		{
			deDoc >> t;
		}
		return deDoc;
	}

	template<class T>
	bool FromJsonO(T& t, const nlohmann::json& obj)
	{
		MSRPC::DeNodeNlohmannJson deNode(&obj);
		if (deNode)
		{
			deNode >> t;
		}
		return deNode;
	}
}
}

// ============================================================================
// Backward compatibility: import into MSRPC base namespace (only first adapter)
// ============================================================================
#ifndef MSRPC_JSON_COMPAT_DEFINED
#define MSRPC_JSON_COMPAT_DEFINED

namespace MSRPC
{
	using namespace MSRPC::NlJson;
}

#endif // MSRPC_JSON_COMPAT_DEFINED


#endif // NLJSONSERIALIZER_HPP__