#pragma once


#include <msserialize/msarchive_stl.hpp>
#include <msserialize/cppyamlnode.hpp>


namespace MSRPC
{
	typedef MSRPC::SeNodeCppYaml SeNode;
	typedef MSRPC::DeNodeCppYaml DeNode;
	typedef MSRPC::SeDocCppYaml SeDoc;
	typedef MSRPC::DeDocCppYaml DeDoc;
	
} // namespace MSRPC

#include "ymlserializer.hpp"

namespace MSRPC
{
	template <typename T>
	bool FromYamlO(T& t, const YAML::Node& node)
	{
		DeNode deDoc(node);
		deDoc >> t;
		return deDoc;
	}
}