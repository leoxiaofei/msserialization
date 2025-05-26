#ifndef RAXMLSERIALIZER_HPP__
#define RAXMLSERIALIZER_HPP__



#include <msserialize/raxmlnode.hpp>
#include <msserialize/msarchive_stl.hpp>
#include <msserialize/msnodeapt_stl.hpp>
#include <msserialize/msnodeapt.hpp>
#include <rapidxml/rapidxml_utils.hpp>
#include <fstream>

namespace MSRPC
{

template<class T>
std::string ToXmlS(const T& t, const char* strRootName)
{
	rapidxml::xml_document<> doc;
	MSRPC::IXmlArc::Node nObjI(&doc);
	MSRPC::IXmlArc::Node root = nObjI.new_node();
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
 bool ToXmlFile(const T& t, const char* strFilePath, const char* strRootName)
 {
	 bool bRet = false;

	 rapidxml::xml_document<> doc;
	 MSRPC::IXmlArc::Node nObjI(&doc);
	 MSRPC::IXmlArc::Node root = nObjI.new_node();
	 MSRPC::IXmlArc ia(root);
	 ia & t;

	 nObjI.add_member(strRootName, root);

	 std::ofstream ofs(strFilePath, std::ios::binary);
	 if (ofs)
	 {
		 ofs << doc;
		 //rapidxml::print(ofs, doc);
		 bRet = true;
	 }

	 return bRet;
 }
 
 template<class T>
 bool FromXmlFile(T& t, const char* strFilePath)
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


#endif // RAXMLSERIALIZER_HPP__
