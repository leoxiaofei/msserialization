#ifndef MSARCHIVE_H__
#define MSARCHIVE_H__

namespace MSRPC
{
	class INodeTest
	{
	public:
		template <class T>
		void in_serialize(T& tValue)
		{
		}

		INodeTest new_node()
		{
			return INodeTest();
		}

		void add_member(const char* strName, INodeTest& vNode)
		{

		}

		void push_node(INodeTest& vNode)
		{

		}
	};

	class ONodeTest
	{
	public:
		template <class T>
		void in_serialize(T& tValue)
		{
		}

		ONodeTest sub_member(const char* strName)
		{

		}

		class ONodeTestIter
		{
		public:
			ONodeTest operator *()
			{
				return ONodeTest();
			}

			operator bool()
			{
				return true;
			}

			ONodeTestIter& operator ++ ()
			{
				return *this;
			}
		};

		typedef ONodeTestIter ITER;
		ITER sub_node()
		{
			return ITER();
		}

		operator bool()
		{
			return true;
		}
	};

	class ArrayReader
	{
	public:

	};

	class ArraryWriter
	{
	public:


	};

	template <class NODE>
	class IArchiveHelper
	{
	private:
		NODE& m_vCurNode;

	public:
		typedef NODE Node;

	public:
		IArchiveHelper(NODE& vNode)
			: m_vCurNode(vNode) {}

		template <class T>
		IArchiveHelper& operator & (T& tValue)
		{
			ex_serialize(*this, tValue);
			return *this;
		}

		template <class T>
		void serialize(NODE& vNewNode, T& tValue)
		{
			IArchiveHelper<NODE> oh(vNewNode);
			ex_serialize(oh, tValue);
		}

		template<>
		void serialize(NODE& vNewNode, int& tValue)
		{
			vNewNode.in_serialize(tValue);
		}

		template<>
		void serialize(NODE& vNewNode, double& tValue)
		{
			vNewNode.in_serialize(tValue);
		}

		template <class T, int N>
		void serialize(NODE& vNewNode, T(&tValue)[N])
		{
			for (int ix = 0; ix != N; ++ix)
			{
				NODE vNode = vNewNode.new_node();
				serialize(vNode, tValue[ix]);
				vNewNode.push_node(vNode);
			}
		}

		template <class T>
		IArchiveHelper& io(const char* strName, T& tValue)
		{
			NODE vNewNode = m_vCurNode.new_node();
			serialize(vNewNode, tValue);
			m_vCurNode.add_member(strName, vNewNode);

			return *this;
		}
		 
	};

	template <class NODE>
	class OArchiveHelper
	{
	private:
		NODE& m_vCurNode;

	public:
		typedef NODE Node;

	public:
		OArchiveHelper(NODE& vNode)
			: m_vCurNode(vNode) {}

		template <class T>
		OArchiveHelper& operator & (T& tValue)
		{
			ex_serialize(*this, tValue);
			return *this;
		}

		template <class T>
		void serialize(NODE& vNewNode, T& tValue)
		{
			OArchiveHelper<NODE> oh(vNewNode);
			ex_serialize(oh, tValue);
		}

		template<>
		void serialize(NODE& vNewNode, int& tValue)
		{
			vNewNode.in_serialize(tValue);
		}

		template<>
		void serialize(NODE& vNewNode, double& tValue)
		{
			vNewNode.in_serialize(tValue);
		}

		template <class T, int N>
		void serialize(NODE& vNewNode, T(&tValue)[N])
		{
			typename NODE::ITER itor = vNewNode.sub_node();
			for (int ix = 0; ix != N && itor; ++itor, ++ix)
			{
				serialize(*itor, tValue[ix]);
			}
		}

		template <class T>
		OArchiveHelper& io(const char* strName, T& tValue)
		{
			if (NODE vNewNode = m_vCurNode.sub_member(strName))
			{
				serialize(vNewNode, tValue);
			}

			return *this;
		}

	};

}

#endif // MSARCHIVE_H__
