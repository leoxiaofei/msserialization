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
#include <QPointF>

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

	//////////////////////////////////////////////////////////////////////////

	template<class T>
	class ExtractApt<QPointF, T, class _Pos_>
	{
	public:
		ExtractApt(T& t)
			: m_t(t) {
		}

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

	template<class T>
	using EtPosApt = ExtractApt<QPointF, T, class _Pos_>;

	//////////////////////////////////////////////////////////////////////////

	template<class T>
	class ExtractApt<QRectF, T, class _Rect_>
	{
	public:
		ExtractApt(T& t)
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

	template<class T>
	using EtRectApt = ExtractApt<QRectF, T, class _Rect_>;

	//////////////////////////////////////////////////////////////////////////

	template<class T>
	class ExtractApt<int, T, class _StartAngle_>
	{
	public:
		ExtractApt(T& t)
			: m_t(t) {}

		operator int () const
		{
			return m_t.startAngle();
		}

		void operator = (int rfValue)
		{
			m_t.setStartAngle(rfValue);
		}

	private:
		T& m_t;
	};

	template<class T>
	using EtStartAngleApt = ExtractApt<int, T, class _StartAngle_>;

	//////////////////////////////////////////////////////////////////////////

	template<class T>
	class ExtractApt<int, T, class _SpanAngle_>
	{
	public:
		ExtractApt(T& t)
			: m_t(t) {}

		operator int () const
		{
			return m_t.spanAngle();
		}

		void operator = (int rfValue)
		{
			m_t.setSpanAngle(rfValue);
		}

	private:
		T& m_t;
	};

	template<class T>
	using EtSpanAngleApt = ExtractApt<int, T, class _SpanAngle_>;

	//////////////////////////////////////////////////////////////////////////

	template<class T>
	class ExtractApt<QLineF, T, class _Line_>
	{
	public:
		ExtractApt(T& t)
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

	template<class T>
	using EtLineApt = ExtractApt<QLineF, T, class _Line_>;

	//////////////////////////////////////////////////////////////////////////

	template<class T>
	class ExtractApt<qreal, T, class _ZValue_>
	{
	public:
		ExtractApt(T& t)
			: m_t(t)
		{
		}

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
	using EtZValueApt = ExtractApt<qreal, T, class _ZValue_>;

	//////////////////////////////////////////////////////////////////////////

	template<class T>
	class ExtractApt<qreal, T, class _TextWidth_>
	{
	public:
		ExtractApt(T& t)
			: m_t(t)
		{
		}

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
	using EtTextWidthApt = ExtractApt<qreal, T, class _TextWidth_>;

	//////////////////////////////////////////////////////////////////////////

	template<class T>
	class ExtractApt<QColor, T, class _DefaultTextColor_>
	{
	public:
		ExtractApt(T& t)
			: m_t(t)
		{
		}

		operator QColor () const
		{
			return m_t.defaultTextColor();
		}

		void operator = (const QColor& qValue)
		{
			m_t.setDefaultTextColor(qValue);
		}

	private:
		T& m_t;
	};

	template<class T>
	using EtDefaultTextColorApt = ExtractApt<QColor, T, class _DefaultTextColor_>;

	//////////////////////////////////////////////////////////////////////////

	template<class T>
	class ExtractApt<QString, T, class _Data_>
	{
	public:
		ExtractApt(T& t, int nKey)
			: m_t(t)
			, m_nKey(nKey)
		{
		}

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
	using EtDataApt = ExtractApt<QString, T, class _Data_>;


	template<class T>
	class ExtractApt<QString, T, class _Pixmap_>
	{
	public:
		ExtractApt(T& t, int nKey)
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
	using EtPixmapApt = ExtractApt<QString, T, class _Pixmap_>;


	template<class T>
	class ExtractApt<QString, T, class _Svg>
	{
	public:
		ExtractApt(T& t, int nKey)
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
	using EtSvgApt = ExtractApt<QString, T, class _Svg>;

	template<class T>
	class ExtractApt<QString, T, class _Transform_>
	{
	public:
		ExtractApt(T& t, int nKey)
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
	using EtTransformApt = ExtractApt<QString, T, class _Transform_>;


	template<class T>
	class ExtractApt<QString, T, class _Path_>
	{
	public:
		ExtractApt(T& t, int nKey)
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
					QRegExp("M\\s*\\(\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*\\)"),
					QRegExp("L\\s*\\(\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*\\)"),
					QRegExp("Q\\s*\\(\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*\\)"),
					QRegExp("C\\s*\\(\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*\\)"),
					QRegExp("AM\\s*\\(\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*\\)"),
					QRegExp("A\\s*\\(\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*,\\s*(-?\\d+\\.?\\d*)\\s*\\)"),
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
	using EtPathApt = ExtractApt<QString, T, class _Path_>;


	template<class T>
	class ExtractApt<QString, T, class _Html_>
	{
	public:
		ExtractApt(T& t)
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
	using EtHtmlApt = ExtractApt<QString, T, class _Html_>;


	template<class T>
	class ExtractApt<QString, T, class _PlainText_>
	{
	public:
		ExtractApt(T& t)
			: m_t(t)
		{
		}

		operator QString () const
		{
			return m_t.toPlainText();
		}

		void operator = (const QString& strValue)
		{
			m_t.setPlainText(strValue);
		}

	private:
		T& m_t;
	};

	template<class T>
	using EtPlainTextApt = ExtractApt<QString, T, class _PlainText_>;

#ifdef QT_GUI_LIB
	//////////////////////////////////////////////////////////////////////////

	template<class T>
	class ExtractApt<QPolygonF, T, class _Polygon_>
	{
	public:
		ExtractApt(T& t)
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

	template<class T>
	using EtPolygonApt = ExtractApt<QPolygonF, T, class _Polygon_>;

	//////////////////////////////////////////////////////////////////////////

	template<class T>
	class ExtractApt<QFont, T, class _Font_>
	{
	public:
		ExtractApt(T& t)
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

	template<class T>
	using EtFontApt = ExtractApt<QFont, T, class _Font_>;
	

	//////////////////////////////////////////////////////////////////////////

	template<class T>
	class ExtractApt<QPen, T, class _Pen_>
	{
	public:
		ExtractApt(T& t)
			: m_t(t)
		{}

		operator QPen() const
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

	template<class T>
	using EtPenApt = ExtractApt<QPen, T, class _Pen_>;

	//////////////////////////////////////////////////////////////////////////

	template<class T>
	class ExtractApt<QBrush, T, class _Brush_>
	{
	public:
		ExtractApt(T& t)
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

	template<class T>
	using EtBrushApt = ExtractApt<QBrush, T, class _Brush_>;

	//////////////////////////////////////////////////////////////////////////

	template<class T>
	class ExtractApt<QBrush, T, class _Foreground_>
	{
	public:
		ExtractApt(T& t)
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
	using EtForegroundBrushApt = ExtractApt<QBrush, T, class _Foreground_>;

	//////////////////////////////////////////////////////////////////////////

	template<class T>
	class ExtractApt<QBrush, T, class _Background_>
	{
	public:
		ExtractApt(T& t)
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
	using EtBackgroundBrushApt = ExtractApt<QBrush, T, class _Background_>;
	
	//////////////////////////////////////////////////////////////////////////

	template<class T>
	class ExtractApt<QList<T*>, T, class _ChildItems_>
	{
	public:
		ExtractApt(T& t)
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

	template<class T>
	using EtChildItemsApt = ExtractApt<QList<T*>, T, class _ChildItems_>;

#endif

	//////////////////////////////////////////////////////////////////////////

	template<class T>
	class ExtractApt<QRectF, T, class _SceneRect_>
	{
	public:
		ExtractApt(T& t)
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

	template<class T>
	using EtSceneRectApt = ExtractApt<QRectF, T, class _SceneRect_>;

	//////////////////////////////////////////////////////////////////////////

	template<class T>
	class ExtractApt<int, T, class _FillRule_>
	{
	public:
		ExtractApt(T& t)
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

	template<class T>
	using EtFillRuleApt = ExtractApt<int, T, class _FillRule_>;

	//////////////////////////////////////////////////////////////////////////

	template<class T>
	class ExtractApt<QHash<QString, QString>, T, class _HashData_>
	{
	public:
		ExtractApt(T& t, int nKey)
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

	template<class T>
	using EtHashDataApt = ExtractApt<QHash<QString, QString>, T, class _HashData_>;

	//////////////////////////////////////////////////////////////////////////

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
