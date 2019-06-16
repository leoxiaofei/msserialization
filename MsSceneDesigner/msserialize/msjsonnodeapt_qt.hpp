#ifndef MSJSONNODEAPT_QT_H__
#define MSJSONNODEAPT_QT_H__


#include "msarchive.hpp"

#include <QFont>
#include <QPen>
#include <QBrush>


namespace MSRPC
{
	template<class T>
	class QtPosApt
	{
	public:
		QtPosApt(T& t) 
			: m_t(t) {}

		operator QPointF () const
		{
			return m_t.pos();
		}

		void operator = (const QPointF& ptValue)
		{
			m_t.setPos(ptValue);
		}

	private:
		T& m_t;
	};

	template<class NODE, class T>
	class ISerialize<NODE, QtPosApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QtPosApt<T>& tValue)
		{
			ISerialize<NODE, QPointF>::serialize(vNewNode, tValue);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QtPosApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, QtPosApt<T>& tValue)
		{
			QPointF ptValue;
			OSerialize<NODE, QPointF>::serialize(vNewNode, ptValue);
			tValue = ptValue;
		}
	};

	template<class T>
	class QtRectApt
	{
	public:
		QtRectApt(T& t)
			: m_t(t) {}

		operator QRectF () const
		{
			return m_t.rect();
		}

		void operator = (const QRectF& rfValue)
		{
			m_t.setRect(rfValue);
		}

	private:
		T& m_t;
	};

	template<class NODE, class T>
	class ISerialize<NODE, QtRectApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QtRectApt<T>& tValue)
		{
			ISerialize<NODE, QRectF>::serialize(vNewNode, tValue);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QtRectApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, QtRectApt<T>& tValue)
		{
			QRectF rfValue;
			OSerialize<NODE, QRectF>::serialize(vNewNode, rfValue);
			tValue = rfValue;
		}
	};

	template<class T>
	class QtLineApt
	{
	public:
		QtLineApt(T& t)
			: m_t(t) {}

		operator QLineF () const
		{
			return m_t.line();
		}

		void operator = (const QLineF& lfValue)
		{
			m_t.setLine(lfValue);
		}

	private:
		T& m_t;
	};

	template<class NODE, class T>
	class ISerialize<NODE, QtLineApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QtLineApt<T>& tValue)
		{
			ISerialize<NODE, QLineF>::serialize(vNewNode, tValue);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QtLineApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, QtLineApt<T>& tValue)
		{
			QLineF lfValue;
			OSerialize<NODE, QLineF>::serialize(vNewNode, lfValue);
			tValue = lfValue;
		}
	};


	template<class T>
	class QtDataApt
	{
	public:
		QtDataApt(T& t, int nKey)
			: m_t(t)
			, m_nKey(nKey)
		{}

		operator QString () const
		{
			return m_t.data(m_nKey).toString();
		}

		void operator = (const QString& strValue)
		{
			m_t.setData(m_nKey, strValue);
		}

	private:
		T& m_t;
		int m_nKey;
	};

	template<class NODE, class T>
	class ISerialize<NODE, QtDataApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QtDataApt<T>& tValue)
		{
			QString strValue = tValue;
			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QtDataApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, QtDataApt<T>& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			tValue = strValue;
		}
	};

	template<class T>
	class QtZValueApt
	{
	public:
		QtZValueApt(T& t)
			: m_t(t)
		{}

		operator qreal () const
		{
			return m_t.zValue();
		}

		void operator = (const qreal& qValue)
		{
			m_t.setZValue(qValue);
		}

	private:
		T& m_t;
	};

	template<class NODE, class T>
	class ISerialize<NODE, QtZValueApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QtZValueApt<T>& tValue)
		{
			qreal qValue = tValue;
			ISerialize<NODE, qreal>::serialize(vNewNode, qValue);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QtZValueApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, QtZValueApt<T>& tValue)
		{
			qreal qValue;
			OSerialize<NODE, qreal>::serialize(vNewNode, qValue);
			tValue = qValue;
		}
	};

	template<class T>
	class QtPixmapApt
	{
	public:
		QtPixmapApt(T& t, int nKey)
			: m_t(t)
			, m_nKey(nKey)
		{}

		operator QString () const
		{
			return m_t.data(m_nKey).toString();
		}

		void operator = (const QString& strValue)
		{
			m_t.setData(m_nKey, strValue);
			QPixmap pixmap(strValue);
			m_t.setPixmap(pixmap);
		}

	private:
		T& m_t;
		int m_nKey;
	};

	template<class NODE, class T>
	class ISerialize<NODE, QtPixmapApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QtPixmapApt<T>& tValue)
		{
			QString strValue = tValue;
			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QtPixmapApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, QtPixmapApt<T>& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			tValue = strValue;
		}
	};

	template<class T>
	class QtSvgApt
	{
	public:
		QtSvgApt(T& t, int nKey)
			: m_t(t)
			, m_nKey(nKey)
		{}

		operator QString () const
		{
			return m_t.data(m_nKey).toString();
		}

		void operator = (const QString& strValue)
		{
			m_t.renderer()->load(strValue);
			m_t.setElementId("");
			m_t.setData(m_nKey, strValue);
		}

	private:
		T& m_t;
		int m_nKey;
	};

	template<class NODE, class T>
	class ISerialize<NODE, QtSvgApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QtSvgApt<T>& tValue)
		{
			QString strValue = tValue;
			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QtSvgApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, QtSvgApt<T>& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			tValue = strValue;
		}
	};

	template<class T>
	class QtTransformApt
	{
	public:
		QtTransformApt(T& t, int nKey)
			: m_t(t)
			, m_nKey(nKey)
		{}

		operator QString () const
		{
			return m_t.data(m_nKey).toString();
		}

		void operator = (const QString& strValue)
		{
			m_t.setData(m_nKey, strValue);

			QStringList listFunc = strValue.split(";");
			if (!listFunc.isEmpty())
			{
				QRegExp re[_TK_TOTAL_];
				for (int ix = 0; ix != _TK_TOTAL_; ++ix)
				{
					re[ix].setPattern(szTfRegExp[ix]);
				}
				
				QTransform tf;

				foreach(const QString& strFunc, listFunc)
				{
					for (int ix = 0; ix != _TK_TOTAL_; ++ix)
					{
						QRegExp& exp = re[ix];
						if (exp.indexIn(strFunc) != -1)
						{
							switch (ix)
							{
							case TK_TRANSLATE:
								tf.translate(exp.cap(1).toDouble(), exp.cap(2).toDouble());
								break;
							case TK_SCALE:
								tf.scale(exp.cap(1).toDouble(), exp.cap(2).toDouble());
								break;
							case TK_ROTATE:
								tf.rotate(exp.cap(1).toDouble(), 
									static_cast<Qt::Axis>(exp.cap(2).toInt()));
								break;
							default:
								break;
							}
							break;
						}
					}
				}

				m_t.setTransform(tf);
			}

		}

	private:
		T& m_t;
		int m_nKey;
	};

	template<class NODE, class T>
	class ISerialize<NODE, QtTransformApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QtTransformApt<T>& tValue)
		{
			QString strValue = tValue;
			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QtTransformApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, QtTransformApt<T>& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			tValue = strValue;
		}
	};


	template<class T>
	class QtHtmlApt
	{
	public:
		QtHtmlApt(T& t)
			: m_t(t)
		{}

		operator QString () const
		{
			return m_t.toHtml();
		}

		void operator = (const QString& strValue)
		{
			m_t.setHtml(strValue);
		}

	private:
		T& m_t;
	};

	template<class NODE, class T>
	class ISerialize<NODE, QtHtmlApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QtHtmlApt<T>& tValue)
		{
			QString strValue = tValue;
			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QtHtmlApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, QtHtmlApt<T>& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			tValue = strValue;
		}
	};

	template<class T>
	class QtFontApt
	{
	public:
		QtFontApt(T& t)
			: m_t(t)
		{}

		operator QFont () const
		{
			return m_t.font();
		}

		void operator = (const QFont& fValue)
		{
			m_t.setFont(fValue);
		}

	private:
		T& m_t;
	};

	template<class NODE, class T>
	class ISerialize<NODE, QtFontApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QtFontApt<T>& tValue)
		{
			ISerialize<NODE, QFont>::serialize(vNewNode, tValue);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QtFontApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, QtFontApt<T>& tValue)
		{
			QFont font;
			OSerialize<NODE, QFont>::serialize(vNewNode, font);
			tValue = font;
		}
	};

	template<class T>
	class QtPenApt
	{
	public:
		QtPenApt(T& t)
			: m_t(t)
		{}

		operator QPen () const
		{
			return m_t.pen();
		}

		void operator = (const QPen& pValue)
		{
			m_t.setPen(pValue);
		}

	private:
		T& m_t;
	};

	template<class NODE, class T>
	class ISerialize<NODE, QtPenApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QtPenApt<T>& tValue)
		{
			ISerialize<NODE, QPen>::serialize(vNewNode, tValue);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QtPenApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, QtPenApt<T>& tValue)
		{
			QPen pen;
			OSerialize<NODE, QPen>::serialize(vNewNode, pen);
			tValue = pen;
		}
	};

	template<class T>
	class QtBrushApt
	{
	public:
		QtBrushApt(T& t)
			: m_t(t)
		{}

		operator QBrush () const
		{
			return m_t.brush();
		}

		void operator = (const QBrush& bValue)
		{
			m_t.setBrush(bValue);
		}

	private:
		T& m_t;
	};

	template<class NODE, class T>
	class ISerialize<NODE, QtBrushApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QtBrushApt<T>& tValue)
		{
			ISerialize<NODE, QBrush>::serialize(vNewNode, tValue);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QtBrushApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, QtBrushApt<T>& tValue)
		{
			QBrush brush;
			OSerialize<NODE, QBrush>::serialize(vNewNode, brush);
			tValue = brush;
		}
	};
}

#endif // MSJSONNODEAPT_QT_H__
