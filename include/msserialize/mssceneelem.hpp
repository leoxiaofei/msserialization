#include "msnodeapt_qt.hpp"
#include "msscenedef.h"


#include <QGraphicsRectItem>
#include <QGraphicsTextItem>


#ifdef QT_SVG_LIB
#include <QSvgRenderer>
#include <QGraphicsSvgItem>
#endif

namespace MSRPC
{
	template<class NODE>
	class ISerialize<NODE, QGraphicsItem*>
	{
	public:
		static void serialize(NODE& vNewNode, const QGraphicsItem* tValue)
		{
			if (tValue)
			{
				vNewNode.set_object();
				IArchiveHelper<NODE> ar(vNewNode);
				int nType = tValue->type();
				ar.io("type", nType);

				switch (nType)
				{
				case QGraphicsRectItem::Type:
					ISerialize<NODE, QGraphicsRectItem*>::serialize(vNewNode,
						qgraphicsitem_cast<const QGraphicsRectItem*>(tValue));
					break;
				case QGraphicsLineItem::Type:
					ISerialize<NODE, QGraphicsLineItem*>::serialize(vNewNode,
						qgraphicsitem_cast<const QGraphicsLineItem*>(tValue));
					break;
				case QGraphicsEllipseItem::Type:
					ISerialize<NODE, QGraphicsEllipseItem*>::serialize(vNewNode,
						qgraphicsitem_cast<const QGraphicsEllipseItem*>(tValue));
					break;
				case QGraphicsPixmapItem::Type:
					ISerialize<NODE, QGraphicsPixmapItem*>::serialize(vNewNode,
						qgraphicsitem_cast<const QGraphicsPixmapItem*>(tValue));
					break;
#ifdef QT_SVG_LIB
				case QGraphicsSvgItem::Type:
					ISerialize<NODE, QGraphicsSvgItem*>::serialize(vNewNode,
						qgraphicsitem_cast<const QGraphicsSvgItem*>(tValue));
					break;
#endif
				case QGraphicsTextItem::Type:
					ISerialize<NODE, QGraphicsTextItem*>::serialize(vNewNode,
						qgraphicsitem_cast<const QGraphicsTextItem*>(tValue));
					break;
				case QGraphicsPolygonItem::Type:
					ISerialize<NODE, QGraphicsPolygonItem*>::serialize(vNewNode,
						qgraphicsitem_cast<const QGraphicsPolygonItem*>(tValue));
					break;
				case QGraphicsPathItem::Type:
					ISerialize<NODE, QGraphicsPathItem*>::serialize(vNewNode,
						qgraphicsitem_cast<const QGraphicsPathItem*>(tValue));
					break;
				case QGraphicsItemGroup::Type:
					ISerialize<NODE, QGraphicsItemGroup*>::serialize(vNewNode,
						qgraphicsitem_cast<const QGraphicsItemGroup*>(tValue));
					break;
				default:
					break;
				}
			}
		}
	};

	template<class NODE>
	class OSerialize<NODE, QGraphicsItem*>
	{
	public:
		static void serialize(const NODE& vNewNode, QGraphicsItem*& tValue)
		{
			int nType;
			OArchiveHelper<NODE> ar(vNewNode);
			ar.io("type", nType);
			switch (nType)
			{
			case QGraphicsRectItem::Type:
			{
				QGraphicsRectItem* pVal = 0;
				OSerialize<NODE, QGraphicsRectItem*>::serialize(vNewNode, pVal);
				tValue = pVal;
				break;
			}
			case QGraphicsLineItem::Type:
			{
				QGraphicsLineItem* pVal = 0;
				OSerialize<NODE, QGraphicsLineItem*>::serialize(vNewNode, pVal);
				tValue = pVal;
				break;
			}
			case QGraphicsEllipseItem::Type:
			{
				QGraphicsEllipseItem* pVal = 0;
				OSerialize<NODE, QGraphicsEllipseItem*>::serialize(vNewNode, pVal);
				tValue = pVal;
				break;
			}
			case QGraphicsPixmapItem::Type:
			{
				QGraphicsPixmapItem* pVal = 0;
				OSerialize<NODE, QGraphicsPixmapItem*>::serialize(vNewNode, pVal);
				tValue = pVal;
				break;
			}
#ifdef QT_SVG_LIB
			case QGraphicsSvgItem::Type:
			{
				QGraphicsSvgItem* pVal = 0;
				OSerialize<NODE, QGraphicsSvgItem*>::serialize(vNewNode, pVal);
				tValue = pVal;
				break;
			}
#endif
			case QGraphicsTextItem::Type:
			{
				QGraphicsTextItem* pVal = 0;
				OSerialize<NODE, QGraphicsTextItem*>::serialize(vNewNode, pVal);
				tValue = pVal;
				break;
			}
			case QGraphicsPolygonItem::Type:
			{
				QGraphicsPolygonItem* pVal = 0;
				OSerialize<NODE, QGraphicsPolygonItem*>::serialize(vNewNode, pVal);
				tValue = pVal;
				break;
			}
			case QGraphicsPathItem::Type:
			{
				QGraphicsPathItem* pVal = 0;
				OSerialize<NODE, QGraphicsPathItem*>::serialize(vNewNode, pVal);
				tValue = pVal;
				break;
			}
			case QGraphicsItemGroup::Type:
			{
				QGraphicsItemGroup* pVal = 0;
				OSerialize<NODE, QGraphicsItemGroup*>::serialize(vNewNode, pVal);
				tValue = pVal;
			}
			default:
				tValue = nullptr;
				break;
			}
		}
	};

	template<class Ar>
	void ex_serialize(Ar& ar, QGraphicsItem& tValue)
	{
		ar.io("name", QtDataApt<QGraphicsItem>(tValue, IDK_NAME));
		ar.io("childItems", QtChildItemsApt<QGraphicsItem>(tValue));
		ar.io("pos", QtPosApt<QGraphicsItem>(tValue));
		ar.io("zValue", QtZValueApt<QGraphicsItem>(tValue));
		ar.io("transform", QtTransformApt<QGraphicsItem>(tValue, IDK_TRANSFORM));
		ar.io("custom", QtHashDataApt<QGraphicsItem>(tValue, IDK_CUSTOM));
	}

	template<class Ar>
	void ex_serialize(Ar& ar, QAbstractGraphicsShapeItem& tValue)
	{
		ex_serialize(ar, static_cast<QGraphicsItem&>(tValue));
		ar.io("brush", QtBrushApt<QAbstractGraphicsShapeItem>(tValue));
		ar.io("pen", QtPenApt<QAbstractGraphicsShapeItem>(tValue));
	}

	template<class Ar>
	void ex_serialize(Ar& ar, QGraphicsRectItem& tValue)
	{
		ex_serialize(ar, static_cast<QAbstractGraphicsShapeItem&>(tValue));
		ar.io("rect", QtRectApt<QGraphicsRectItem>(tValue));
	}

	template<class Ar>
	void ex_serialize(Ar& ar, QGraphicsLineItem& tValue)
	{
		ex_serialize(ar, static_cast<QGraphicsItem&>(tValue));
		ar.io("pen", QtPenApt<QGraphicsLineItem>(tValue));
		ar.io("line", QtLineApt<QGraphicsLineItem>(tValue));
	}

	template<class Ar>
	void ex_serialize(Ar& ar, QGraphicsEllipseItem& tValue)
	{
		ex_serialize(ar, static_cast<QAbstractGraphicsShapeItem&>(tValue));
		ar.io("rect", QtRectApt<QGraphicsEllipseItem>(tValue));
	}

	template<class Ar>
	void ex_serialize(Ar& ar, QGraphicsPolygonItem& tValue)
	{
		ex_serialize(ar, static_cast<QAbstractGraphicsShapeItem&>(tValue));
		ar.io("polygon", QtPolygonApt<QGraphicsPolygonItem>(tValue));
		ar.io("fillRule", QtFillRuleApt<QGraphicsPolygonItem>(tValue));
	}

	template<class Ar>
	void ex_serialize(Ar& ar, QGraphicsPathItem& tValue)
	{
		ex_serialize(ar, static_cast<QAbstractGraphicsShapeItem&>(tValue));
		ar.io("path", QtPathApt<QGraphicsPathItem>(tValue, IDK_PPATH));
	}

	template<class Ar>
	void ex_serialize(Ar& ar, QGraphicsPixmapItem& tValue)
	{
		ex_serialize(ar, static_cast<QGraphicsItem&>(tValue));
		ar.io("image", QtPixmapApt<QGraphicsPixmapItem>(tValue, IDK_PIXMAP));
	}
#ifdef QT_SVG_LIB
	template<class Ar>
	void ex_serialize(Ar& ar, QGraphicsSvgItem& tValue)
	{
		ex_serialize(ar, static_cast<QGraphicsItem&>(tValue));
		ar.io("image", QtSvgApt<QGraphicsSvgItem>(tValue, IDK_SVG));
	}
#endif
	template<class Ar>
	void ex_serialize(Ar& ar, QGraphicsTextItem& tValue)
	{
		ex_serialize(ar, static_cast<QGraphicsItem&>(tValue));
		ar.io("font", QtFontApt<QGraphicsTextItem>(tValue));
		ar.io("text", QtPlainTextApt<QGraphicsTextItem>(tValue));
		ar.io("textWidth", QtTextWidthApt<QGraphicsTextItem>(tValue));
	}

	template<class Ar>
	void ex_serialize(Ar& ar, QGraphicsItemGroup& tValue)
	{
		ex_serialize(ar, static_cast<QGraphicsItem&>(tValue));
	}

	template<class Ar>
	void ex_serialize(Ar& ar, QGraphicsScene& tValue)
	{
		ar.io("sceneRect", QtSceneRectApt<QGraphicsScene>(tValue));
		ar.io("backgroundBrush", QtBackgroundBrushApt<QGraphicsScene>(tValue));
		ar.io("foregroundBrush", QtForegroundBrushApt<QGraphicsScene>(tValue));
	}

}