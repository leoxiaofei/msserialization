#ifndef QTREEITEMNODE_H__
#define QTREEITEMNODE_H__

#include <QTreeWidgetItem>

#include "msarchive.hpp"


namespace MSRPC
{
	class INodeTreeItem
	{
	private:
		QTreeWidgetItem* m_node;

	public:
		template <class T>
		void in_serialize(T& tValue)
		{
			m_node->setData(1, Qt::DisplayRole, QVariant::fromValue(tValue));
		}

		void in_serialize(char* tValue)
		{
			m_node->setData(1, Qt::DisplayRole, QString(tValue));
		}

		INodeTreeItem new_node()
		{
			return INodeTreeItem();
		}

		void add_member(const char* strName, INodeTreeItem& vNode)
		{
			vNode.data()->setData(0, Qt::DisplayRole, QString(strName));
			m_node->addChild(vNode.data());
		}

		void push_node(INodeTreeItem& vNode)
		{
			m_node->addChild(vNode.data());
		}

	public:
		INodeTreeItem()
			: m_node(new QTreeWidgetItem)
		{
		}

		QTreeWidgetItem*& data()
		{
			return m_node;
		}

	};

	typedef MSRPC::IArchiveHelper<MSRPC::INodeTreeItem> ITreeItemArc;

}


#endif // QTREEITEMNODE_H__
