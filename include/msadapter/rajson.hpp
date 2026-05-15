#ifndef RAJSONSERIALIZER_HPP__
#define RAJSONSERIALIZER_HPP__


///General JSON Serialization Interface

#include <msadapter/msrajsonadapter.hpp>
#include <msserialize/rajsonnode.hpp>
#include <msserialize/msarchive_stl.hpp>
#include <fstream>

// ============================================================================
// RaJson adapter namespace
// ============================================================================
namespace MSRPC
{
namespace RaJson
{
	typedef MSRPC::SeNodeRapidJson SeNode;
	typedef MSRPC::DeNodeRapidJson DeNode;
	typedef MSRPC::SeDocRapidJson SeDoc;
	typedef MSRPC::DeDocRapidJson DeDoc;

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
	bool FromJsonO(T& t, const rapidjson::Value& obj)
	{
		MSRPC::DeNodeRapidJson deNode(&obj);
		if (deNode)
		{
			deNode >> t;
		}
		return deNode;
	}

	template<class T>
	bool ToJsonO(const T& t, rapidjson::Value& obj,
		rapidjson::Document::AllocatorType& allocator)
	{
		MSRPC::SeNodeRapidJson seNode(&obj, &allocator);
		seNode << t;
		return seNode;
	}
}
}

// ============================================================================
// Backward compatibility: skip if already defined by another adapter
// ============================================================================
#ifndef MSRPC_JSON_COMPAT_DEFINED
#define MSRPC_JSON_COMPAT_DEFINED

namespace MSRPC
{
	using namespace MSRPC::RaJson;
}

#endif // MSRPC_JSON_COMPAT_DEFINED


#endif // RAJSONSERIALIZER_HPP__