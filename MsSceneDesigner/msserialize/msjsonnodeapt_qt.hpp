#ifndef MSJSONNODEAPT_QT_H__
#define MSJSONNODEAPT_QT_H__


#include "msarchive.hpp"
#include <QTextStream>


namespace MSRPC
{
	template<class T>
	class QtPosApt
	{
	public:
		QtPosApt(T& t) 
			: m_t(t) {}

		operator QString () const
		{
			QPointF pt = m_t.pos();
			return QString("%1,%2").arg(pt.x()).arg(pt.y());
		}

		void operator = (const QString& strValue)
		{
			QPointF pt;
			char ch(0);
			QTextStream ts(const_cast<QString*>(&strValue));
			ts >> pt.rx() >> ch >> pt.ry();
			m_t.setPos(pt);
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
			QString strValue = tValue;
			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QtPosApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, QtPosApt<T>& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			tValue = strValue;
		}
	};

	template<class T>
	class QtRectApt
	{
	public:
		QtRectApt(T& t)
			: m_t(t) {}

		operator QString () const
		{
			QRectF rf = m_t.rect();
			return QString("%1,%2,%3,%4").arg(rf.x())
				.arg(rf.y()).arg(rf.width()).arg(rf.height());
		}

		void operator = (const QString& strValue)
		{
			qreal dX(0);
			qreal dY(0);
			qreal dW(0);
			qreal dH(0);
			char ch(0);
			QTextStream ts(const_cast<QString*>(&strValue));
			ts >> dX >> ch >> dY >> ch >> dW >> ch >> dH;
			m_t.setRect(dX, dY, dW, dH);
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
			QString strValue = tValue;
			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QtRectApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, QtRectApt<T>& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			tValue = strValue;
		}
	};

	template<class T>
	class QtLineApt
	{
	public:
		QtLineApt(T& t)
			: m_t(t) {}

		operator QString () const
		{
			QLineF lf = m_t.line();
			return QString("%1,%2,%3,%4").arg(lf.x1())
				.arg(lf.y1()).arg(lf.x2()).arg(lf.y2());
		}

		void operator = (const QString& strValue)
		{
			qreal dX1(0);
			qreal dY1(0);
			qreal dX2(0);
			qreal dY2(0);
			char ch(0);
			QTextStream ts(const_cast<QString*>(&strValue));
			ts >> dX1 >> ch >> dY1 >> ch >> dX2 >> ch >> dY2;
			m_t.setLine(dX1, dY1, dX2, dY2);
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
			QString strValue = tValue;
			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QtLineApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, QtLineApt<T>& tValue)
		{
			QString strValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			tValue = strValue;
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
}

#endif // MSJSONNODEAPT_QT_H__
