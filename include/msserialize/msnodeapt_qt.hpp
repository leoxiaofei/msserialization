#ifndef MSJSONNODEAPT_QT_H__
#define MSJSONNODEAPT_QT_H__


#include "msarchive.hpp"
#include "msnodeapt.hpp"
#include "msscenedef.h"
#include <QSharedPointer>
#include <QRect>
#include <QRegExp>
#include <QHash>
#include <QString>

#ifdef QT_GUI_LIB
#include <QFont>
#include <QPen>
#include <QBrush>
#endif

#ifdef QT_WIDGETS_LIB
#include <QGraphicsItemGroup>
#endif

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

	template<class T, class S>
	class QtStringApt;

	template<class NODE, class T, class S>
	class ISerialize<NODE, QtStringApt<T, S> >
	{
	public:
		static void serialize(NODE& vNewNode, const QtStringApt<T, S>& tValue)
		{
			QString strValue = tValue;
			ISerialize<NODE, QString>::serialize(vNewNode, strValue);
		}
	};

	template<class NODE, class T, class S>
	class OSerialize<NODE, QtStringApt<T, S> >
	{
	public:
		static void serialize(const NODE& vNewNode, QtStringApt<T, S>& tValue)
		{
			QString strValue = tValue;
			OSerialize<NODE, QString>::serialize(vNewNode, strValue);
			tValue = strValue;
		}
	};

	template<class T>
	class QtStringApt<T, class _Data_>
	{
	public:
		QtStringApt(T& t, int nKey)
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

	template<class T>
	using QtDataApt = QtStringApt<T, class _Data_>;

	template<class T, class S>
	class QtRealApt;

	template<class NODE, class T, class S>
	class ISerialize<NODE, QtRealApt<T, S> >
	{
	public:
		static void serialize(NODE& vNewNode, const QtRealApt<T, S>& tValue)
		{
			qreal qValue = tValue;
			ISerialize<NODE, qreal>::serialize(vNewNode, qValue);
		}
	};

	template<class NODE, class T, class S>
	class OSerialize<NODE, QtRealApt<T, S> >
	{
	public:
		static void serialize(const NODE& vNewNode, QtRealApt<T, S>& tValue)
		{
			qreal qValue = tValue;
			OSerialize<NODE, qreal>::serialize(vNewNode, qValue);
			tValue = qValue;
		}
	};

	template<class T>
	class QtRealApt<T, class _ZValue_>
	{
	public:
		QtRealApt(T& t)
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

	template<class T>
	using QtZValueApt = QtRealApt<T, class _ZValue_>;

	template<class T>
	class QtRealApt<T, class _TextWidth_>
	{
	public:
		QtRealApt(T& t)
			: m_t(t)
		{}

		operator qreal () const
		{
			return m_t.textWidth();
		}

		void operator = (const qreal& qValue)
		{
			m_t.setTextWidth(qValue);
		}

	private:
		T& m_t;
	};

	template<class T>
	using QtTextWidthApt = QtRealApt<T, class _TextWidth_>;

	template<class T>
	class QtStringApt<T, class _Pixmap_>
	{
	public:
		QtStringApt(T& t, int nKey)
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

	template<class T>
	using QtPixmapApt = QtStringApt<T, class _Pixmap_>;


	template<class T>
	class QtStringApt<T, class _Svg>
	{
	public:
		QtStringApt(T& t, int nKey)
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

	template<class T>
	using QtSvgApt = QtStringApt<T, class _Svg>;

	template<class T>
	class QtStringApt<T, class _Transform_>
	{
	public:
		QtStringApt(T& t, int nKey)
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
				QRegExp re[_TK_TOTAL_] = {
					QRegExp("translate\\s*\\(\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*\\)"),
					QRegExp("scale\\s*\\(\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*\\)"),
					QRegExp("rotate\\s*\\(\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(\\d)\\s*\\)"),
				};
				
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

	template<class T>
	using QtTransformApt = QtStringApt<T, class _Transform_>;


	template<class T>
	class QtStringApt<T, class _Path_>
	{
	public:
		QtStringApt(T& t, int nKey)
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
				QRegExp re[_PK_TOTAL_] = {
					QRegExp("moveTo\\s*\\(\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*\\)"),
					QRegExp("lineTo\\s*\\(\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*\\)"),
					QRegExp("quadTo\\s*\\(\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*\\)"),
					QRegExp("cubicTo\\s*\\(\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*\\)"),
					QRegExp("arcMoveTo\\s*\\(\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*\\)"),
					QRegExp("arcTo\\s*\\(\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*\\)"),
				};

				QPainterPath ppath;

				foreach(const QString & strFunc, listFunc)
				{
					for (int ix = 0; ix != _PK_TOTAL_; ++ix)
					{
						QRegExp& exp = re[ix];
						if (exp.indexIn(strFunc) != -1)
						{
							switch (ix)
							{
							case PK_MOVETO:
								ppath.moveTo(exp.cap(1).toDouble(), exp.cap(2).toDouble());
								break;
							case PK_LINETO:
								ppath.lineTo(exp.cap(1).toDouble(), exp.cap(2).toDouble());
								break;
							case PK_QUADTO:
								ppath.quadTo(exp.cap(1).toDouble(), exp.cap(2).toDouble(), 
									exp.cap(3).toDouble(), exp.cap(4).toDouble());
								break;
							case PK_CUBICTO:
								ppath.cubicTo(exp.cap(1).toDouble(), exp.cap(2).toDouble(),
									exp.cap(3).toDouble(), exp.cap(4).toDouble(),
									exp.cap(5).toDouble(), exp.cap(6).toDouble());
								break;
							case PK_ARCMOVETO:
								ppath.arcMoveTo(exp.cap(1).toDouble(), exp.cap(2).toDouble(),
									exp.cap(3).toDouble(), exp.cap(4).toDouble(), exp.cap(5).toDouble());
								break;
							case PK_ARCTO:
								ppath.arcTo(exp.cap(1).toDouble(), exp.cap(2).toDouble(),
									exp.cap(3).toDouble(), exp.cap(4).toDouble(),
									exp.cap(5).toDouble(), exp.cap(6).toDouble());
								break;
							default:
								break;
							}
							break;
						}
					}
				}

				m_t.setPath(ppath);
			}
		}

	private:
		T& m_t;
		int m_nKey;
	};

	template<class T>
	using QtPathApt = QtStringApt<T, class _Path_>;


	template<class T>
	class QtStringApt<T, class _Html_>
	{
	public:
		QtStringApt(T& t)
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

	template<class T>
	using QtHtmlApt = QtStringApt<T, class _Html_>;


#ifdef QT_GUI_LIB
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

	template<class T, class S = QBrush>
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

	template<class NODE, class T, class S>
	class ISerialize<NODE, QtBrushApt<T, S> >
	{
	public:
		static void serialize(NODE& vNewNode, const QtBrushApt<T, S>& tValue)
		{
			ISerialize<NODE, QBrush>::serialize(vNewNode, tValue);
		}
	};

	template<class NODE, class T, class S>
	class OSerialize<NODE, QtBrushApt<T, S> >
	{
	public:
		static void serialize(const NODE& vNewNode, QtBrushApt<T, S>& tValue)
		{
			QBrush brush = tValue;
			OSerialize<NODE, QBrush>::serialize(vNewNode, brush);
			tValue = brush;
		}
	};

	template<class T>
	class QtBrushApt<T, class _Foreground_>
	{
	public:
		QtBrushApt(T& t)
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

	template<class T>
	using QtForegroundBrushApt = QtBrushApt<T, class _Foreground_>;

	template<class T>
	class QtBrushApt<T, class _Background_>
	{
	public:
		QtBrushApt(T& t)
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

	template<class T>
	using QtBackgroundBrushApt = QtBrushApt<T, class _Background_>;
	
	template<class T>
	class QtChildItemsApt
	{
	public:
		QtChildItemsApt(T& t)
			: m_t(t)
		{}

		operator QList<T*>() const
		{
			return m_t.childItems();
		}

		void operator = (const QList<T*>& rfValue)
		{
			if (QGraphicsItemGroup* pGroup =
				qgraphicsitem_cast<QGraphicsItemGroup*>(&m_t))
			{
				foreach(T* t, rfValue)
				{
					if (t)
					{
						pGroup->addToGroup(t);
					}
				}
			}
			else
			{
				foreach(T* t, rfValue)
				{
					if (t)
					{
						t->setParentItem(&m_t);
					}
				}
			}
		}

	private:
		T& m_t;
	};

	template<class NODE, class T>
	class ISerialize<NODE, QtChildItemsApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, const QtChildItemsApt<T>& tValue)
		{
			ISerialize<NODE, QList<T*> >::serialize(vNewNode, tValue);
		}
	};

	template<class NODE, class T>
	class OSerialize<NODE, QtChildItemsApt<T> >
	{
	public:
		static void serialize(NODE& vNewNode, QtChildItemsApt<T>& tValue)
		{
			QList<T*> rfValue;
			OSerialize<NODE, QList<T*> >::serialize(vNewNode, rfValue);
			tValue = rfValue;
		}
	};
#endif

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
			QVariant vVal = m_t.data(m_nKey);
			return vVal.value<QHash<QString, QString> >();
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
	class ArrayReshape<C, F, QSharedPointer<ELEM> >
	{
	public:
		ArrayReshape(C& val, const F& f)
			: conta(val)
			, func(f)
			, idx(0)
		{}

		ArrayReshape(const ArrayReshape& other)
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
			++(const_cast<ArrayReshape*>(this)->idx);
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
