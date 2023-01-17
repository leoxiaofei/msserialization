#ifndef MSARCHIVE_H__
#define MSARCHIVE_H__

namespace MSRPC
{
	template<typename T>
	class StrApt
	{
	public:
		/// 两个Get选其一: 如果可以取得char字符串常量，
		/// 那么使用带const的在rapidjson下可以少拷贝一次，
		/// 如果无法取得常量字符串，那么使用不带const的get。
		/*
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
		*/

		StrApt(T& str)
			: m_str(str) {}

		StrApt(const T& str)
			: m_str(const_cast<T&>(str)) {}

		const char* Get() const
		{
			return m_str.c_str();
		}

		void Set(const char* tValue, const size_t& sSize)
		{
			m_str.assign(tValue, sSize);
		}

	private:
		T& m_str;
	};

	template <class NODE>
	class IArchiveHelper;

	//template <class AR, typename T>
	//void ex_serialize(AR& ar, T& tValue)
	//{
	//	#pragma message(__FUNCSIG__)
	//}

	template <class NODE, typename T>
	class ISerialize
	{
	public:
		static void serialize (NODE& vNewNode, const T& tValue)
		{
			vNewNode.set_object();
			IArchiveHelper<NODE> oh(vNewNode);
			ex_serialize(oh, const_cast<T&>(tValue));
		}
	};

	template <class NODE, class T>
	class ISerialize<NODE, T&>
	{
	public:
		static void serialize(NODE& vNewNode, const T& tValue)
		{
			ISerialize<NODE, T>::serialize(vNewNode, tValue);
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
	class ISerialize<NODE, char>
	{
	public:
		static void serialize(NODE& vNewNode, const char& tValue)
		{
			vNewNode.in_serialize(static_cast<short>(tValue));
		}
	};

	template<class NODE>
	class ISerialize<NODE, unsigned char>
	{
	public:
		static void serialize(NODE& vNewNode, const unsigned char& tValue)
		{
			vNewNode.in_serialize(static_cast<unsigned short>(tValue));
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
	class ISerialize<NODE, long>
	{
	public:
		static void serialize(NODE& vNewNode, const long& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class ISerialize<NODE, unsigned long>
	{
	public:
		static void serialize(NODE& vNewNode, const unsigned long& tValue)
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
		static void serialize(NODE& vNewNode, char* tValue)
		{
			vNewNode.in_serialize(tValue);
		}

	};

	template<class NODE>
	class ISerialize<NODE, const char*>
	{
	public:
		static void serialize(NODE& vNewNode, const char* tValue)
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

	template<class NODE, class T>
	class ISerialize<NODE, StrApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const StrApt<T>& tValue)
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
		}

		template <class T>
		IArchiveHelper &operator&(const T &tValue)
		{
			ISerialize<NODE, T>::serialize(m_vCurNode, tValue);
			m_vCurNode.finish();
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

	template <class NODE>
	class OArchiveHelper;

	template<class NODE, typename T>
	class OSerialize
	{
	public:
		static void serialize(const NODE& vNewNode, T& tValue)
		{
			OArchiveHelper<NODE> oh(vNewNode);
			ex_serialize(oh, tValue);
		}
	};

	template<class NODE, typename T, int N>
	class OSerialize<NODE, T[N]>
	{
	public:
		static void serialize(const NODE& vNewNode, T(&tValue)[N])
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
		static void serialize(const NODE& vNewNode, T*& tValue)
		{
			tValue = new T;
			OSerialize<NODE, T>::serialize(vNewNode, *tValue);
		}
	};

	template<class NODE, typename T>
	class OSerialize<NODE, T&>
	{
	public:
		static void serialize(const NODE& vNewNode, T& tValue)
		{
			OSerialize<NODE, T>::serialize(vNewNode, tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, bool>
	{
	public:
		static void serialize(const NODE& vNewNode, bool& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, char>
	{
	public:
		static void serialize(const NODE& vNewNode, char& tValue)
		{
			int uValue(0);
			vNewNode.in_serialize(uValue);
			tValue = static_cast<char>(uValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, unsigned char>
	{
	public:
		static void serialize(const NODE& vNewNode, unsigned char& tValue)
		{
			unsigned int uValue(0);
			vNewNode.in_serialize(uValue);
			tValue = static_cast<unsigned char>(uValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, short>
	{
	public:
		static void serialize(const NODE& vNewNode, short& tValue)
		{
			int uValue(0);
			vNewNode.in_serialize(uValue);
			tValue = static_cast<short>(uValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, unsigned short>
	{
	public:
		static void serialize(const NODE& vNewNode, unsigned short& tValue)
		{
			unsigned int uValue(0);
			vNewNode.in_serialize(uValue);
			tValue = static_cast<unsigned short>(uValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, int>
	{
	public:
		static void serialize(const NODE& vNewNode, int& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, unsigned int>
	{
	public:
		static void serialize(const NODE& vNewNode, unsigned int& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, long>
	{
	public:
		static void serialize(const NODE& vNewNode, long& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, unsigned long>
	{
	public:
		static void serialize(const NODE& vNewNode, unsigned long& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, long long>
	{
	public:
		static void serialize(const NODE& vNewNode, long long& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, unsigned long long>
	{
	public:
		static void serialize(const NODE& vNewNode, unsigned long long& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, double>
	{
	public:
		static void serialize(const NODE& vNewNode, double& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, float>
	{
	public:
		static void serialize(const NODE& vNewNode, float& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template<class NODE>
	class OSerialize<NODE, char*>
	{
	public:
		static void serialize(const NODE& vNewNode, const char*& tValue)
		{
			vNewNode.in_serialize(tValue);
		}

		static void serialize(const NODE& vNewNode, char*& tValue)
		{
			vNewNode.in_serialize(const_cast<const char*&>(tValue));
		}

	};

	template<class NODE, int N>
	class OSerialize<NODE, char[N]>
	{
	public:
		static void serialize(const NODE& vNewNode, char(&tValue)[N])
		{
			vNewNode.in_serialize(tValue, N);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, StrApt<T> >
	{
	public:
		static void serialize(const NODE& vNewNode, StrApt<T>& tValue)
		{
			vNewNode.in_serialize(tValue);
		}
	};

	template <class NODE>
	class OArchiveHelper
	{
	private:
		const NODE& m_vCurNode;

	public:
		typedef NODE Node;

	public:
		OArchiveHelper(const NODE& vNode)
			: m_vCurNode(vNode) {}

		template <class T>
		const OArchiveHelper& operator & (T& tValue) const
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

		const NODE& node() const
		{
			return m_vCurNode;
		}

	};

#define DiExSe(EX) \
	namespace MSRPC { \
	template <class NODE> \
	class ISerialize<NODE, EX> \
	{ \
	public: \
		static void serialize(NODE& vNewNode, EX const& tValue) \
		{ \
			vNewNode.set_object(); \
			IArchiveHelper<NODE> oh(vNewNode); \
			ex_serialize(oh, const_cast<EX&>(tValue)); \
		} \
	}; \
	template<class NODE> \
	class OSerialize<NODE, EX> \
	{ \
	public: \
		static void serialize(const NODE& vNewNode, EX& tValue) \
		{ \
			OArchiveHelper<NODE> oh(vNewNode); \
			ex_serialize(oh, tValue); \
		} \
	}; \
	}
}

#define MS_CONCAT(A, B) _MS_CONCAT(A, B)
#define _MS_CONCAT(A, B) __MS_CONCAT(A, B)
#define __MS_CONCAT(A, B) A##B

#define MS_MACROARGCHECK(...) MS_PROT(_MS_MACROARGCHECK)(__VA_ARGS__,\
	N, N, N, N, N, N, N, N, N, N, \
	N, N, N, N, N, N, N, N, N, N, \
	N, N, N, N, N, N, N, N, N, N, \
	N, N, N, N, N, N, N, N, N, N, \
	N, N, N, N, N, N, N, N, N, N, \
	N, N, N, N, N, N, N, N, N, N, \
	N, N, N, N, N, N, N, N, N, N, \
	N, N, N, N, N, N, N, N, N, N, \
	N, N, N, N, N, N, N, N, N, N, \
	N, N, N, N, N, N, N, N, N, N, \
	1)

#define _MS_MACROARGCHECK(_0, \
	_01, _02, _03, _04, _05, _06, _07, _08, _09, _10, \
	_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
	_21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
	_31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
	_41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
	_51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
	_61, _62, _63, _64, _65, _66, _67, _68, _69, _70, \
	_71, _72, _73, _74, _75, _76, _77, _78, _79, _80, \
	_81, _82, _83, _84, _85, _86, _87, _88, _89, _90, \
	_91, _92, _93, _94, _95, _96, _97, _98, _99, _100, \
	TARGET, ...) TARGET

#define MS_ENUMARGS(MSFUNC,...) MS_EXPAND(MS_FOREACH(MSFUNC, __VA_ARGS__))

#define MS_FOREACH(MSFUNC, ...) MS_PROT(MS_CONCAT(_MS_FOREACH, MS_MACROARGCHECK(__VA_ARGS__))) (MSFUNC, __VA_ARGS__)
#define _MS_FOREACH() MS_FOREACH
#define _MS_FOREACH1(MSFUNC, A) MSFUNC(A)
#define _MS_FOREACHN(MSFUNC, A, ...) MSFUNC(A) MS_PROT(_MS_FOREACH)() (MSFUNC, __VA_ARGS__)

#define MS_EMPTY()
#define MS_PROT(FUNC) FUNC MS_EMPTY()


#define MS_EXPAND(...)  MS_EXPAND1(MS_EXPAND1(MS_EXPAND1(__VA_ARGS__)))
#define MS_EXPAND1(...) MS_EXPAND2(MS_EXPAND2(MS_EXPAND2(__VA_ARGS__)))
#define MS_EXPAND2(...) MS_EXPAND3(MS_EXPAND3(MS_EXPAND3(__VA_ARGS__)))
#define MS_EXPAND3(...) MS_EXPAND4(MS_EXPAND4(MS_EXPAND4(__VA_ARGS__)))
#define MS_EXPAND4(...) MS_EXPAND5(MS_EXPAND5(MS_EXPAND5(__VA_ARGS__)))
#define MS_EXPAND5(...) MS_EXPAND6(MS_EXPAND6(MS_EXPAND6(__VA_ARGS__)))
#define MS_EXPAND6(...) __VA_ARGS__


#define SiArIo(MEM) ar.io(#MEM, tValue.MEM); \

#define SiExSe(TYPE, ...) template<class Ar> \
void ex_serialize(Ar& ar, TYPE& tValue) \
{ \
	MS_ENUMARGS(SiArIo, __VA_ARGS__) \
}

#define SiExSeVoid(TYPE) template<class Ar> \
void ex_serialize(Ar& ar, TYPE& tValue) \
{ \
}

#define SiExSeTemp(TMP, TYPE, ...) template<class Ar, typename TMP> \
void ex_serialize(Ar& ar, TYPE& tValue) \
{ \
	MS_ENUMARGS(SiArIo, __VA_ARGS__) \
}


#endif // MSARCHIVE_H__
