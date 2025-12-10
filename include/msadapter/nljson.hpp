#ifndef NLJSONSERIALIZER_HPP__
#define NLJSONSERIALIZER_HPP__


///General JSON Serialization Interface

#include <msserialize/nljsonnode.hpp>
#include <msserialize/msarchive_stl.hpp>

namespace MSRPC
{
	typedef MSRPC::SeNodeNlohmannJson SeNode;
	typedef MSRPC::DeNodeNlohmannJson DeNode;
	typedef MSRPC::SeDocNlohmannJson SeDoc;
	typedef MSRPC::DeDocNlohmannJson DeDoc;
}

#include "jsonserializer.hpp"

namespace MSRPC
{

template<class T>
bool FromJsonO(T& t, const nlohmann::json& obj)
{
	MSRPC::DeNode deNode(&obj);
	if (deNode)
	{
		deNode >> t;
	}

	return deNode;
}

}


#endif // NLJSONSERIALIZER_HPP__
