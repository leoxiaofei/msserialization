#ifndef CPPYAML_HPP__
#define CPPYAML_HPP__


#include <msserialize/msarchive_stl.hpp>
#include <msserialize/cppyamlnode.hpp>
#include <fstream>

// ============================================================================
// CppYaml adapter namespace
// ============================================================================
namespace MSRPC
{
namespace CppYaml
{
	typedef MSRPC::SeNodeCppYaml SeNode;
	typedef MSRPC::DeNodeCppYaml DeNode;
	typedef MSRPC::SeDocCppYaml SeDoc;
	typedef MSRPC::DeDocCppYaml DeDoc;

	template<class T, class StrBuf = std::string>
	StrBuf ToYamlS(const T& t)
	{
		SeDoc seDoc;
		seDoc << t;
		return seDoc.template Stringify<StrBuf>();
	}

	template<class T, class StrBuf>
	bool FromYamlS(T& t, StrBuf& strYaml)
	{
		DeDoc deDoc;
		deDoc.Parse(strYaml);
		if (deDoc)
		{
			deDoc >> t;
		}
		return deDoc;
	}

	template<class T>
	bool ToYamlF(const T& t, const char* strFilePath)
	{
		SeDoc seDoc;
		seDoc << t;
		return seDoc.Save(strFilePath);
	}

	template<class T>
	bool FromYamlF(T& t, const char* strFilePath)
	{
		DeDoc deDoc;
		deDoc.Load(strFilePath);
		if (deDoc)
		{
			deDoc >> t;
		}
		return deDoc;
	}

	template <typename T>
	bool FromYamlO(T& t, const YAML::Node& node)
	{
		DeNode deDoc(node);
		deDoc >> t;
		return deDoc;
	}
}
}

// ============================================================================
// Backward compatibility: import into MSRPC base namespace (only first yaml adapter)
// ============================================================================
#ifndef MSRPC_YAML_COMPAT_DEFINED
#define MSRPC_YAML_COMPAT_DEFINED

namespace MSRPC
{
	using namespace MSRPC::CppYaml;
}

#endif // MSRPC_YAML_COMPAT_DEFINED


#endif // CPPYAML_HPP__