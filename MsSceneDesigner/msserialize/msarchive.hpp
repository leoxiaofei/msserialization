#ifndef MSARCHIVE_H__
#define MSARCHIVE_H__

namespace MSRPC
{
	class NValTest
	{
	public:
		template <class T>
		void in_serialize(T& tValue)
		{
		}
	};

	class NObjTest
	{
	public:
		NObjTest sub_obj(const char* strName)
		{
			return NObjTest();
		}

		void end_obj(const char* strName, NObjTest& vNode)
		{

		}

		NValTest sub_val(const char* strName)
		{
			return NValTest();
		}

		void end_val(const char* strName, NValTest& vTest)
		{
		}

		operator bool() const
		{
			return true;
		}
	};


	template <class NVAL, class NOBJ>
	class ArchiveHelper
	{
	private:
		NOBJ& m_vCurNode;

	public:
		typedef NOBJ OBJ;

	public:
		ArchiveHelper(NOBJ& vNode)
			: m_vCurNode(vNode) {}

		template <class T>
		ArchiveHelper& operator & (T& tValue)
		{
			ex_serialize(*this, tValue);
			return *this;
		}

		template <class T>
		ArchiveHelper& io(const char* strName, T& tValue)
		{
			if (NOBJ vNewNode = m_vCurNode.sub_obj(strName))
			{
				ArchiveHelper<NVAL, NOBJ> oh(vNewNode);
				ex_serialize(oh, tValue);
				m_vCurNode.end_obj(strName, vNewNode);
			}

			return *this;
		}

		template <class T, class N>
		ArchiveHelper& io(const char* strName, T(&tValue)[N])
		{
			for (int ix = 0; ix != N; ++ix)
			{
				this->io(strName, tValue[ix]);
			}
			return *this;
		}

		template <class T>
		inline void seri_val(const char* strName, T& tValue)
		{
			if (typename NVAL vNewNode = m_vCurNode.sub_val(strName))
			{
				vNewNode.in_serialize(tValue);
				m_vCurNode.end_val(strName, vNewNode);
			}
		}
		 
		template <>
		ArchiveHelper& io(const char* strName, int& tValue)
		{
			seri_val(strName, tValue);
			return *this;
		}

		template <>
		ArchiveHelper& io(const char* strName, double& tValue)
		{
			seri_val(strName, tValue);
			return *this;
		}

	};
}

#endif // MSARCHIVE_H__
