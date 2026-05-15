#ifndef RAXML_HPP__
#define RAXML_HPP__


#include <msserialize/raxmlnode.hpp>
#include <msserialize/msarchive_stl.hpp>
#include <msserialize/msnodeapt_stl.hpp>
#include <msserialize/msnodeapt.hpp>
#include <rapidxml/rapidxml_utils.hpp>
#include <fstream>

// ============================================================================
// RaXml adapter namespace
// ============================================================================
namespace MSRPC
{
namespace RaXml
{
	template<class T>
	std::string ToXmlS(const T& t, const char* strRootName)
	{
		rapidxml::xml_document<> doc;
		MSRPC::IXmlArc::Node nObjI(&doc);
		MSRPC::IXmlArc::Node root = nObjI.add_element();
		MSRPC::IXmlArc ia(root);
		ia & t;

		nObjI.add_member(strRootName, root);

		std::string text;
		rapidxml::print(std::back_inserter(text), doc);
		return text;
	}

	template<class T, class StrBuf>
	bool FromXmlS(T& t, StrBuf& strXml)
	{
		bool bRet(false);
		rapidxml::xml_document<> doc;
		doc.parse<rapidxml::parse_full>(&strXml[0]);
		rapidxml::xml_node<>* node = doc.first_node();
		while (node && node->type() != rapidxml::node_element)
		{
			node = node->next_sibling();
		}

		if (node)
		{
			MSRPC::OXmlArc::Node objO(node);
			MSRPC::OXmlArc oa(objO);

			if (objO)
			{
				oa & t;
				bRet = true;
			}
		}

		return bRet;
	}

	template<class T>
	bool ToXmlF(const T& t, const char* strFilePath, const char* strRootName)
	{
		bool bRet = false;

		rapidxml::xml_document<> doc;
		MSRPC::IXmlArc::Node nObjI(&doc);
		MSRPC::IXmlArc::Node root = nObjI.add_element();
		MSRPC::IXmlArc ia(root);
		ia & t;

		nObjI.add_member(strRootName, root);

		std::ofstream ofs(strFilePath, std::ios::binary);
		if (ofs)
		{
			ofs << doc;
			bRet = true;
		}

		return bRet;
	}

	template<class T>
	bool FromXmlF(T& t, const char* strFilePath)
	{
		bool bRet(false);

		try
		{
			rapidxml::file<> file(strFilePath);
			auto p = file.data();
			bRet = FromXmlS(t, p);
		}
		catch (...)
		{
		}

		return bRet;
	}
}
}

// ============================================================================
// Backward compatibility: define in MSRPC base namespace (only first xml adapter)
// ============================================================================
#ifndef MSRPC_XML_COMPAT_DEFINED
#define MSRPC_XML_COMPAT_DEFINED

namespace MSRPC
{
	using namespace MSRPC::RaXml;
}

#endif // MSRPC_XML_COMPAT_DEFINED


#endif // RAXML_HPP__