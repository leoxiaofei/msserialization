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

		typedef ONodeTestIter ArrIter;
		ArrIter sub_nodes()
		{
			return ArrIter();
		}

		operator bool()
		{
			return true;
		}
	};

	template<typename T>
	class StrApt
	{
	public:
		/// 两个Get选其一：带const的在rapidjson下可以少拷贝一次。
		const char* Get() const
		{
			return 0;
		}

		char* Get() const
		{
			return 0;
		}

		void Set (const char* tValue, const size_t& sSize)
		{
		}
	};

	//template <class AR, typename T>
	//void ex_serialize(AR& ar, T& tValue);

	template <class NODE, typename T>
	class ISerialize
	{
	public:
		static void serialize (NODE& vNewNode, const T& tValue)
		{
			IArchiveHelper<NODE> oh(vNewNode);
			ex_serialize(oh, const_cast<T&>(tValue));
		}
	};

	template<class NODE, typename T, int N>
	class ISerialize<NODE, T[N]>
	{
	public:
		static void serialize(NODE& vNewNode, const T(&tValue)[N])
		{
			vNewNode.set_array();

			for (int ix = 0; ix != N; ++ix)
			{
				NODE vNode = vNewNode.new_node();
				ISerialize<NODE, T>::serialize(vNode, tValue[ix]);
				vNewNode.push_node(vNode);
			}
		}
	};

	template<class NODE, typename T>
	class ISerialize<NODE, T*>
	{
	public:
		static void serialize(NODE& vNewNode, const T* tValue)
		{
			if (tValue)
			{
				ISerialize<NODE, T>::serialize(vNewNode, *tValue);
			}
		}
	};

	template<class NODE>
	class ISerialize<NODE, bool>
	{
	public:
		static void serialize(NODE& vNewNode, const bool& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};


	template<class NODE>
	class ISerialize<NODE, short>
	{
	public:
		static void serialize(NODE& vNewNode, const short& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class ISerialize<NODE, unsigned short>
	{
	public:
		static void serialize(NODE& vNewNode, const unsigned short& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class ISerialize<NODE, int>
	{
	public:
		static void serialize(NODE& vNewNode, const int& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class ISerialize<NODE, unsigned int>
	{
	public:
		static void serialize(NODE& vNewNode, const unsigned int& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};


	template<class NODE>
	class ISerialize<NODE, long long>
	{
	public:
		static void serialize(NODE& vNewNode, const long long& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class ISerialize<NODE, unsigned long long>
	{
	public:
		static void serialize(NODE& vNewNode, const unsigned long long& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class ISerialize<NODE, double>
	{
	public:
		static void serialize(NODE& vNewNode, const double& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class ISerialize<NODE, float>
	{
	public:
		static void serialize(NODE& vNewNode, const float& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class ISerialize<NODE, char*>
	{
	public:
		static void serialize(NODE& vNewNode, const char* tValue)
		{
			vNewNode.in_serialize(tValue);
		}

		static void serialize(NODE& vNewNode, char* tValue)
		{
			vNewNode.in_serialize(tValue);
		}

	};

	template<class NODE, int N>
	class ISerialize<NODE, char[N]>
	{
	public:
		static void serialize(NODE& vNewNode, const char(&tValue)[N])
		{
			vNewNode.in_serialize(tValue);
		}
	};


	template <class NODE>
	class IArchiveHelper
	{
	private:
		NODE& m_vCurNode;

	public:
		typedef NODE Node;

	public:
		IArchiveHelper(NODE &vNode)
			: m_vCurNode(vNode)
		{
			m_vCurNode.set_object(true);
		}

		template <class T>
		IArchiveHelper &operator&(const T &tValue)
		{
			ISerialize<NODE, T>::serialize(m_vCurNode, tValue);
			return *this;
		}

		template <class T>
		IArchiveHelper& io(const char* strName, const T& tValue)
		{
			NODE vNewNode = m_vCurNode.new_node();
			ISerialize<NODE, T>::serialize(vNewNode, tValue);
			m_vCurNode.add_member(strName, vNewNode);

			return *this;
		}
		 
	};


	template<class NODE, typename T>
	class OSerialize
	{
	public:
		static void serialize(NODE& vNewNode, T& tValue)
		{
			OArchiveHelper<NODE> oh(vNewNode);
			ex_serialize(oh, tValue);
		}
	};

	template<class NODE, typename T, int N>
	class OSerialize<NODE, T[N]>
	{
	public:
		static void serialize(NODE& vNewNode, T(&tValue)[N])
		{
			typename NODE::ArrIter itor = vNewNode.sub_nodes();
			for (int ix = 0; ix != N && itor; ++itor, ++ix)
			{
				OSerialize<NODE, T>::serialize(*itor, tValue[ix]);
			}
		}

	};

	template<class NODE, typename T>
	class OSerialize<NODE, T*>
	{
	public:
		static void serialize(NODE& vNewNode, T*& tValue)
		{
			tValue = new T;
			OSerialize<NODE, T>::serialize(vNewNode, *tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, bool>
	{
	public:
		static void serialize(NODE& vNewNode, bool& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, short>
	{
	public:
		static void serialize(NODE& vNewNode, short& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, unsigned short>
	{
	public:
		static void serialize(NODE& vNewNode, unsigned short& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, int>
	{
	public:
		static void serialize(NODE& vNewNode, int& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, unsigned int>
	{
	public:
		static void serialize(NODE& vNewNode, unsigned int& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};


	template<class NODE>
	class OSerialize<NODE, long long>
	{
	public:
		static void serialize(NODE& vNewNode, long long& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, unsigned long long>
	{
	public:
		static void serialize(NODE& vNewNode, unsigned long long& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, double>
	{
	public:
		static void serialize(NODE& vNewNode, double& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, float>
	{
	public:
		static void serialize(NODE& vNewNode, float& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, char*>
	{
	public:
		static void serialize(NODE& vNewNode, const char*& tValue)
		{
			vNewNode.in_serialize(tValue);
		}

		static void serialize(NODE& vNewNode, char*& tValue)
		{
			vNewNode.in_serialize(const_cast<const char*&>(tValue));
		}

	};

	template<class NODE, int N>
	class OSerialize<NODE, char[N]>
	{
	public:
		static void serialize(NODE& vNewNode, char(&tValue)[N])
		{
			vNewNode.in_serialize(tValue, N);
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
			OSerialize<NODE, T>::serialize(m_vCurNode, tValue);
			return *this;
		}

		template <class T>
		OArchiveHelper& io(const char* strName, const T& tValue)
		{
			if (NODE vNewNode = m_vCurNode.sub_member(strName))
			{
				OSerialize<NODE, T>::serialize(vNewNode, const_cast<T&>(tValue));
			}

			return *this;
		}

	};

}

#endif // MSARCHIVE_H__
