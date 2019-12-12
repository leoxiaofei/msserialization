#ifndef MSNODEAPT_H__
#define MSNODEAPT_H__


namespace MSRPC
{
	template <typename Enum>
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

		EnumApt(Enum& en, const char** str, int nN)
			: m_en(en)
			, m_szText(str)
			, m_n(nN)
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

	template <typename Enum>
	class StrApt<EnumApt<Enum> >
	{
	public:
		EnumApt<Enum>& m_data;

	public:
		StrApt(EnumApt<Enum>& data) : m_data(data) {}
		StrApt(const EnumApt<Enum>& data) 
			: m_data(const_cast<EnumApt<Enum>&>(data)) {}

		const char* Get() const
		{
			return m_data;
		}

		void Set(const char* tValue, const size_t& sSize)
		{
			m_data = tValue;
		}
	};

	template<class NODE, class T>
	class ISerialize<NODE, EnumApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const EnumApt<T>& tValue)
		{
			vNewNode.in_serialize(StrApt<EnumApt<T> >(tValue));
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, EnumApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, EnumApt<T>& tValue)
		{
			vNewNode.in_serialize(StrApt<EnumApt<T> >(tValue));
		}
	};
}


#endif // MSNODEAPT_H__
