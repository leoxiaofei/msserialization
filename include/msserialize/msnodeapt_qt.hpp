#ifndef MSJSONNODEAPT_QT_H__
#define MSJSONNODEAPT_QT_H__


#include "msarchive.hpp"
#include "msnodeapt.hpp"

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
		static void serialize(const NODE& vNewNode, QtPosApt<T>& tValue)
		{
			QPointF ptValue = tValue;
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
		static void serialize(const NODE& vNewNode, QtRectApt<T>& tValue)
		{
			QRectF rfValue = tValue;
			OSerialize<NODE, QRectF>::serialize(vNewNode, rfValue);
			tValue = rfValue;
		}
	};

	template<class T>
	class QtPolygonApt
	{
	public:
		QtPolygonApt(T& t)
			: m_t(t) {}

		operator QPolygonF () const
		{
			return m_t.polygon();
		}

		void operator = (const QPolygonF& pgValue)
		{
			m_t.setPolygon(pgValue);
		}

	private:
		T& m_t;
	};

	template<class NODE, class T>
	class ISerialize<NODE, QtPolygonApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QtPolygonApt<T>& tValue)
		{
			ISerialize<NODE, QPolygonF>::serialize(vNewNode, tValue);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QtPolygonApt<T> >
	{
	public:
		static void serialize(const NODE& vNewNode, QtPolygonApt<T>& tValue)
		{
			QPolygonF rfValue = tValue;
			OSerialize<NODE, QPolygonF>::serialize(vNewNode, rfValue);
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
		static void serialize(const NODE& vNewNode, QtLineApt<T>& tValue)
		{
			QLineF lfValue = tValue;
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
		static void serialize(const NODE& vNewNode, QtDataApt<T>& tValue)
		{
			QString strValue = tValue;
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
		static void serialize(const NODE& vNewNode, QtZValueApt<T>& tValue)
		{
			qreal qValue = tValue;
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
		static void serialize(const NODE& vNewNode, QtPixmapApt<T>& tValue)
		{
			QString strValue = tValue;
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
		static void serialize(const NODE& vNewNode, QtSvgApt<T>& tValue)
		{
			QString strValue = tValue;
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
				static const char* szTfRegExp[_TK_TOTAL_] = {
					"translate\\s*\\(\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*\\)",
					"scale\\s*\\(\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*\\)",
					"rotate\\s*\\(\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(\\d)\\s*\\)",
				};

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
		static void serialize(const NODE& vNewNode, QtTransformApt<T>& tValue)
		{
			QString strValue = tValue;
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
		static void serialize(const NODE& vNewNode, QtHtmlApt<T>& tValue)
		{
			QString strValue = tValue;
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
		static void serialize(const NODE& vNewNode, QtFontApt<T>& tValue)
		{
			QFont font = tValue;
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
		static void serialize(const NODE& vNewNode, QtPenApt<T>& tValue)
		{
			QPen pen = tValue;
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
		static void serialize(const NODE& vNewNode, QtBrushApt<T>& tValue)
		{
			QBrush brush = tValue;
			OSerialize<NODE, QBrush>::serialize(vNewNode, brush);
			tValue = brush;
		}
	};

	template<class T>
	class QtSceneRectApt
	{
	public:
		QtSceneRectApt(T& t)
			: m_t(t)
		{}

		operator QRectF () const
		{
			return m_t.sceneRect();
		}

		void operator = (const QRectF& rfValue)
		{
			m_t.setSceneRect(rfValue);
		}

	private:
		T& m_t;
	};

	template<class NODE, class T>
	class ISerialize<NODE, QtSceneRectApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QtSceneRectApt<T>& tValue)
		{
			ISerialize<NODE, QRectF>::serialize(vNewNode, tValue);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QtSceneRectApt<T> >
	{
	public:
		static void serialize(const NODE& vNewNode, QtSceneRectApt<T>& tValue)
		{
			QRectF rfValue = tValue;
			OSerialize<NODE, QRectF>::serialize(vNewNode, rfValue);
			tValue = rfValue;
		}
	};

	template<class T>
	class QtForegroundBrushApt
	{
	public:
		QtForegroundBrushApt(T& t)
			: m_t(t)
		{}

		operator QBrush () const
		{
			return m_t.foregroundBrush();
		}

		void operator = (const QBrush& rfValue)
		{
			m_t.setForegroundBrush(rfValue);
		}

	private:
		T& m_t;
	};

	template<class NODE, class T>
	class ISerialize<NODE, QtForegroundBrushApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QtForegroundBrushApt<T>& tValue)
		{
			ISerialize<NODE, QBrush>::serialize(vNewNode, tValue);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QtForegroundBrushApt<T> >
	{
	public:
		static void serialize(const NODE& vNewNode, QtForegroundBrushApt<T>& tValue)
		{
			QBrush rfValue = tValue;
			OSerialize<NODE, QBrush>::serialize(vNewNode, rfValue);
			tValue = rfValue;
		}
	};

	template<class T>
	class QtBackgroundBrushApt
	{
	public:
		QtBackgroundBrushApt(T& t)
			: m_t(t)
		{}

		operator QBrush () const
		{
			return m_t.backgroundBrush();
		}

		void operator = (const QBrush& rfValue)
		{
			m_t.setBackgroundBrush(rfValue);
		}

	private:
		T& m_t;
	};

	template<class NODE, class T>
	class ISerialize<NODE, QtBackgroundBrushApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QtBackgroundBrushApt<T>& tValue)
		{
			ISerialize<NODE, QBrush>::serialize(vNewNode, tValue);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QtBackgroundBrushApt<T> >
	{
	public:
		static void serialize(const NODE& vNewNode, QtBackgroundBrushApt<T>& tValue)
		{
			QBrush rfValue = tValue;
			OSerialize<NODE, QBrush>::serialize(vNewNode, rfValue);
			tValue = rfValue;
		}
	};

	template<class T>
	class QtFillRuleApt
	{
	public:
		QtFillRuleApt(T& t)
			: m_t(t)
		{}

		operator int () const
		{
			return m_t.fillRule();
		}

		void operator = (const int& rfValue)
		{
			m_t.setFillRule(static_cast<Qt::FillRule>(rfValue));
		}

	private:
		T& m_t;
	};

	template<class NODE, class T>
	class ISerialize<NODE, QtFillRuleApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QtFillRuleApt<T>& tValue)
		{
			ISerialize<NODE, int>::serialize(vNewNode, tValue);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QtFillRuleApt<T> >
	{
	public:
		static void serialize(const NODE& vNewNode, QtFillRuleApt<T>& tValue)
		{
			int rfValue = tValue;
			OSerialize<NODE, int>::serialize(vNewNode, rfValue);
			tValue = rfValue;
		}
	};

	template<class T>
	class QtHashDataApt
	{
	public:
		QtHashDataApt(T& t, int nKey)
			: m_t(t)
			, m_nKey(nKey)
		{}

		operator QHash<QString, QString> () const
		{
			return m_t.data(m_nKey).value<QHash<QString, QString> >();
		}

		void operator = (const QHash<QString, QString>& hsValue)
		{
			m_t.setData(m_nKey, QVariant::fromValue(hsValue));
		}

	private:
		T& m_t;
		int m_nKey;
	};

	template<class NODE, class T>
	class ISerialize<NODE, QtHashDataApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QtHashDataApt<T>& tValue)
		{
			QHash<QString, QString> hsValue = tValue;
			ISerialize<NODE, QHash<QString, QString> >::serialize(vNewNode, hsValue);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QtHashDataApt<T> >
	{
	public:
		static void serialize(const NODE& vNewNode, QtHashDataApt<T>& tValue)
		{
			QHash<QString, QString> hsValue = tValue;
			OSerialize<NODE, QHash<QString, QString> >::serialize(vNewNode, hsValue);
			tValue = hsValue;
		}
	};

	template<typename C, typename F, typename ELEM>
	class VecReshape<C, F, QSharedPointer<ELEM> >
	{
	public:
		VecReshape(C& val, const F& f)
			: conta(val)
			, func(f)
			, idx(0)
		{}

		VecReshape(const VecReshape& other)
			: conta(other.conta)
			, func(other.func)
			, idx(other.idx)
		{

		}

		typedef typename function_traits<F>::return_type item_type;

		item_type operator*() const
		{
			return func(conta[idx]);
		}

		item_type push()
		{
			if (idx >= conta.size())
			{
				QSharedPointer<ELEM> t(new ELEM);
				conta.push_back(t);
			}
			return func(conta[idx++]);
		}

		void operator ++() const
		{
			++(const_cast<VecReshape*>(this)->idx);
		}

		operator bool() const
		{
			return idx < conta.size();
		}

	public:
		C& conta;
		F func;
		size_t idx;
	};

}

#endif // MSJSONNODEAPT_QT_H__
