#pragma once

#include <QString>
#include <QTextStream>

#ifdef QT_GUI_LIB
#include <QColor>
#include <QFont>
#include <QPen>
#endif

namespace MSRPC
{
	template<typename S, typename T>
	class BaseTypeApt;

	
	template<typename T>
	using QStrBaseApt = BaseTypeApt<QString, T>;

#ifdef QT_GUI_LIB
	template<>
	class BaseTypeApt<QString, QColor>
	{
		QColor& m_t;
	public:
		BaseTypeApt(const QColor& t)
			: m_t(const_cast<QColor&>(t))
		{
		}

		inline operator QString () const
		{
			return m_t.name(QColor::HexArgb);
		}

		inline void operator = (const QString& strValue)
		{
			m_t.setNamedColor(strValue);
		}
	};

	template<>
	class BaseTypeApt<QString, QFont>
	{
		QFont& m_t;
	public:
		BaseTypeApt(const QFont& t)
			: m_t(const_cast<QFont&>(t))
		{
		}

		inline operator QString () const
		{
			return m_t.toString();
		}

		inline void operator = (const QString& strValue)
		{
			m_t.fromString(strValue);
		}
	};

	template<>
	class BaseTypeApt<QString, QBrush>
	{
		QBrush& m_t;
	public:
		BaseTypeApt(const QBrush& t)
			: m_t(const_cast<QBrush&>(t))
		{
		}

		operator QString () const
		{
			return QString("%1,%2")
				.arg((const QString&)QStrBaseApt<QColor>(m_t.color()))
				.arg(m_t.style());;
		}

		void operator = (const QString& strValue)
		{
			QStringList listValue = strValue.split(',');

			int nSize = qMin(listValue.size(), 2);

			switch (nSize)
			{
			case 2:
				m_t.setStyle(static_cast<Qt::BrushStyle>(
					listValue[--nSize].toInt()));
			case 1:
				m_t.setColor(QColor(listValue[--nSize]));
			default:
				break;
			}
		}
	};

	template<>
	class BaseTypeApt<QString, QPen>
	{
		QPen& m_t;
	public:
		BaseTypeApt(const QPen& t)
			: m_t(const_cast<QPen&>(t))
		{
		}

		operator QString () const
		{
			return QString("%1,%2,%3,%4,%5").arg((const QString&)QStrBaseApt<QColor>(m_t.color()))
				.arg(m_t.widthF()).arg(m_t.style()).arg(m_t.capStyle()) .arg(m_t.joinStyle());
		}

		void operator = (const QString& strValue)
		{
			QStringList listValue = strValue.split(',');

			int nSize = qMin(listValue.size(), 5);

			switch (nSize)
			{
			case 5:
				m_t.setJoinStyle(static_cast<Qt::PenJoinStyle>(
					listValue[--nSize].toInt()));
			case 4:
				m_t.setCapStyle(static_cast<Qt::PenCapStyle>(
					listValue[--nSize].toInt()));
			case 3:
				m_t.setStyle(static_cast<Qt::PenStyle>(
					listValue[--nSize].toInt()));
			case 2:
				m_t.setWidthF(listValue[--nSize].toDouble());
			case 1:
				m_t.setColor(QColor(listValue[--nSize]));
			default:
				break;
			}
		}
	};

#endif

	template<>
	class BaseTypeApt<QString, QPointF>
	{
		QPointF& m_t;
	public:
		BaseTypeApt(const QPointF& t)
			: m_t(const_cast<QPointF&>(t))
		{
		}

		operator QString () const
		{
			return QString("%1,%2").arg(m_t.rx()).arg(m_t.ry());
		}

		void operator = (const QString& strValue)
		{
			char ch(0);
			QTextStream ts(const_cast<QString*>(&strValue));
			ts >> m_t.rx() >> ch >> m_t.ry();
		}
	};

	template<>
	class BaseTypeApt<QString, QPoint>
	{
		QPoint& m_t;
	public:
		BaseTypeApt(const QPoint& t)
			: m_t(const_cast<QPoint&>(t))
		{
		}

		operator QString () const
		{
			return QString("%1,%2").arg(m_t.rx()).arg(m_t.ry());
		}

		void operator = (const QString& strValue)
		{
			char ch(0);
			QTextStream ts(const_cast<QString*>(&strValue));
			ts >> m_t.rx() >> ch >> m_t.ry();
		}
	};

	template<>
	class BaseTypeApt<QString, QSizeF>
	{
		QSizeF& m_t;
	public:
		BaseTypeApt(const QSizeF& t)
			: m_t(const_cast<QSizeF&>(t))
		{
		}

		operator QString () const
		{
			return QString("%1,%2").arg(m_t.rwidth()).arg(m_t.rheight());
		}

		void operator = (const QString& strValue)
		{
			char ch(0);
			QTextStream ts(const_cast<QString*>(&strValue));
			ts >> m_t.rwidth() >> ch >> m_t.rheight();
		}
	};

	template<>
	class BaseTypeApt<QString, QSize>
	{
		QSize& m_t;
	public:
		BaseTypeApt(const QSize& t)
			: m_t(const_cast<QSize&>(t))
		{
		}

		operator QString () const
		{
			return QString("%1,%2").arg(m_t.rwidth()).arg(m_t.rheight());
		}

		void operator = (const QString& strValue)
		{
			char ch(0);
			QTextStream ts(const_cast<QString*>(&strValue));
			ts >> m_t.rwidth() >> ch >> m_t.rheight();
		}
	};

	template<>
	class BaseTypeApt<QString, QRectF>
	{
		QRectF& m_t;
	public:
		BaseTypeApt(const QRectF& t)
			: m_t(const_cast<QRectF&>(t))
		{
		}

		operator QString () const
		{
			return QString("%1,%2,%3,%4").arg(m_t.x())
				.arg(m_t.y()).arg(m_t.width()).arg(m_t.height());
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
	};

	template<>
	class BaseTypeApt<QString, QRect>
	{
		QRect& m_t;
	public:
		BaseTypeApt(const QRect& t)
			: m_t(const_cast<QRect&>(t))
		{
		}

		operator QString () const
		{
			return QString("%1,%2,%3,%4").arg(m_t.x())
				.arg(m_t.y()).arg(m_t.width()).arg(m_t.height());
		}

		void operator = (const QString& strValue)
		{
			int dX(0);
			int dY(0);
			int dW(0);
			int dH(0);
			char ch(0);
			QTextStream ts(const_cast<QString*>(&strValue));
			ts >> dX >> ch >> dY >> ch >> dW >> ch >> dH;
			m_t.setRect(dX, dY, dW, dH);
		}
	};

	template<>
	class BaseTypeApt<QString, QMarginsF>
	{
		QMarginsF& m_t;
	public:
		BaseTypeApt(const QMarginsF& t)
			: m_t(const_cast<QMarginsF&>(t))
		{
		}

		operator QString () const
		{
			return QString("%1,%2,%3,%4").arg(m_t.left())
				.arg(m_t.top()).arg(m_t.right()).arg(m_t.bottom());
		}

		void operator = (const QString& strValue)
		{
			qreal dL(0);
			qreal dT(0);
			qreal dR(0);
			qreal dB(0);
			char ch(0);
			QTextStream ts(const_cast<QString*>(&strValue));
			ts >> dL >> ch >> dT >> ch >> dR >> ch >> dB;
			m_t = { dL, dT, dR, dB };
		}
	};


	template<>
	class BaseTypeApt<QString, QMargins>
	{
		QMargins& m_t;
	public:
		BaseTypeApt(const QMargins& t)
			: m_t(const_cast<QMargins&>(t))
		{
		}

		operator QString () const
		{
			return QString("%1,%2,%3,%4").arg(m_t.left())
				.arg(m_t.top()).arg(m_t.right()).arg(m_t.bottom());
		}

		void operator = (const QString& strValue)
		{
			int nL(0);
			int nT(0);
			int nR(0);
			int nB(0);
			char ch(0);
			QTextStream ts(const_cast<QString*>(&strValue));
			ts >> nL >> ch >> nT >> ch >> nR >> ch >> nB;
			m_t = { nL, nT, nR, nB };
		}
	};

	template<>
	class BaseTypeApt<QString, QLineF>
	{
		QLineF& m_t;
	public:
		BaseTypeApt(const QLineF& t)
			: m_t(const_cast<QLineF&>(t))
		{
		}

		operator QString () const
		{
			return QString("%1,%2 %3,%4").arg(m_t.x1())
				.arg(m_t.y1()).arg(m_t.x2()).arg(m_t.y2());
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
	};

	template<>
	class BaseTypeApt<QString, QLine>
	{
		QLine& m_t;
	public:
		BaseTypeApt(const QLine& t)
			: m_t(const_cast<QLine&>(t))
		{
		}

		operator QString () const
		{
			return QString("%1,%2 %3,%4").arg(m_t.x1())
				.arg(m_t.y1()).arg(m_t.x2()).arg(m_t.y2());
		}

		void operator = (const QString& strValue)
		{
			int nX1(0);
			int nY1(0);
			int nX2(0);
			int nY2(0);
			char ch(0);
			QTextStream ts(const_cast<QString*>(&strValue));
			ts >> nX1 >> ch >> nY1 >> ch >> nX2 >> ch >> nY2;
			m_t.setLine(nX1, nY1, nX2, nY2);
		}
	};

}