#ifndef MSNODEAPT_H__
#define MSNODEAPT_H__


namespace MSRPC
{
	template <class Enum>
	class EnumApt
	{
	public:
		template<int N>
		EnumApt(Enum& en, const char* (&str)[N])
			: m_en(en)
			, m_szText(str)
			, m_n(N)
		{

		}

		operator const char* () const
		{
			return m_szText[m_en];
		}

		void operator = (const char* szText)
		{
			for (int ix = 0; ix != m_n; ++ix)
			{
				if (strcmp(m_szText[ix], szText) == 0)
				{
					m_en = static_cast<Enum>(ix);
				}
			}
		}

	private:
		Enum& m_en;
		const char** m_szText;
		int m_n;
	};

	template<class NODE, class T>
	class ISerialize<NODE, EnumApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const EnumApt<T>& tValue)
		{
			ISerialize<NODE, char*>::serialize(vNewNode, tValue);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, EnumApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, EnumApt<T>& tValue)
		{
			char* pValue = 0;
			OSerialize<NODE, char*>::serialize(vNewNode, pValue);
			tValue = pValue;
		}
	};
}


#endif // MSNODEAPT_H__
