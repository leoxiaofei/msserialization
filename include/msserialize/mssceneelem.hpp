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
	class Serializer<NODE, QGraphicsItem*>
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
					Serializer<NODE, QGraphicsRectItem*>::serialize(vNewNode,
						qgraphicsitem_cast<const QGraphicsRectItem*>(tValue));
					break;
				case QGraphicsLineItem::Type:
					Serializer<NODE, QGraphicsLineItem*>::serialize(vNewNode,
						qgraphicsitem_cast<const QGraphicsLineItem*>(tValue));
					break;
				case QGraphicsEllipseItem::Type:
					Serializer<NODE, QGraphicsEllipseItem*>::serialize(vNewNode,
						qgraphicsitem_cast<const QGraphicsEllipseItem*>(tValue));
					break;
				case QGraphicsPixmapItem::Type:
					Serializer<NODE, QGraphicsPixmapItem*>::serialize(vNewNode,
						qgraphicsitem_cast<const QGraphicsPixmapItem*>(tValue));
					break;
#ifdef QT_SVG_LIB
				case QGraphicsSvgItem::Type:
					Serializer<NODE, QGraphicsSvgItem*>::serialize(vNewNode,
						qgraphicsitem_cast<const QGraphicsSvgItem*>(tValue));
					break;
#endif
				case QGraphicsTextItem::Type:
					Serializer<NODE, QGraphicsTextItem*>::serialize(vNewNode,
						qgraphicsitem_cast<const QGraphicsTextItem*>(tValue));
					break;
				case QGraphicsPolygonItem::Type:
					Serializer<NODE, QGraphicsPolygonItem*>::serialize(vNewNode,
						qgraphicsitem_cast<const QGraphicsPolygonItem*>(tValue));
					break;
				case QGraphicsPathItem::Type:
					Serializer<NODE, QGraphicsPathItem*>::serialize(vNewNode,
						qgraphicsitem_cast<const QGraphicsPathItem*>(tValue));
					break;
				case QGraphicsItemGroup::Type:
					Serializer<NODE, QGraphicsItemGroup*>::serialize(vNewNode,
						qgraphicsitem_cast<const QGraphicsItemGroup*>(tValue));
					break;
				default:
					break;
				}
			}
		}

		static void deserialize(const NODE& vNewNode, QGraphicsItem*& tValue)
		{
			int nType;
			OArchiveHelper<NODE> ar(vNewNode);
			ar.io("type", nType);
			switch (nType)
			{
			case QGraphicsRectItem::Type:
			{
				QGraphicsRectItem* pVal = 0;
				Serializer<NODE, QGraphicsRectItem*>::deserialize(vNewNode, pVal);
				tValue = pVal;
				break;
			}
			case QGraphicsLineItem::Type:
			{
				QGraphicsLineItem* pVal = 0;
				Serializer<NODE, QGraphicsLineItem*>::deserialize(vNewNode, pVal);
				tValue = pVal;
				break;
			}
			case QGraphicsEllipseItem::Type:
			{
				QGraphicsEllipseItem* pVal = 0;
				Serializer<NODE, QGraphicsEllipseItem*>::deserialize(vNewNode, pVal);
				tValue = pVal;
				break;
			}
			case QGraphicsPixmapItem::Type:
			{
				QGraphicsPixmapItem* pVal = 0;
				Serializer<NODE, QGraphicsPixmapItem*>::deserialize(vNewNode, pVal);
				tValue = pVal;
				break;
			}
#ifdef QT_SVG_LIB
			case QGraphicsSvgItem::Type:
			{
				QGraphicsSvgItem* pVal = 0;
				Serializer<NODE, QGraphicsSvgItem*>::deserialize(vNewNode, pVal);
				tValue = pVal;
				break;
			}
#endif
			case QGraphicsTextItem::Type:
			{
				QGraphicsTextItem* pVal = 0;
				Serializer<NODE, QGraphicsTextItem*>::deserialize(vNewNode, pVal);
				tValue = pVal;
				break;
			}
			case QGraphicsPolygonItem::Type:
			{
				QGraphicsPolygonItem* pVal = 0;
				Serializer<NODE, QGraphicsPolygonItem*>::deserialize(vNewNode, pVal);
				tValue = pVal;
				break;
			}
			case QGraphicsPathItem::Type:
			{
				QGraphicsPathItem* pVal = 0;
				Serializer<NODE, QGraphicsPathItem*>::deserialize(vNewNode, pVal);
				tValue = pVal;
				break;
			}
			case QGraphicsItemGroup::Type:
			{
				QGraphicsItemGroup* pVal = 0;
				Serializer<NODE, QGraphicsItemGroup*>::deserialize(vNewNode, pVal);
				tValue = pVal;
				break;
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
		ar.io("name", EtStringDataApt<QGraphicsItem>(tValue, IDK_NAME));
		ar.io("childItems", EtChildItemsApt<QGraphicsItem>(tValue));
		ar.io("pos", EtPosApt<QGraphicsItem>(tValue));
		ar.io("zValue", EtZValueApt<QGraphicsItem>(tValue));
		ar.io("transform", EtTransformApt<QGraphicsItem>(tValue, IDK_TRANSFORM));
		ar.io("extended", EtHashDataApt<QGraphicsItem>(tValue, IDK_EXTATTR));
		ar.io("userdata", EtHashDataApt<QGraphicsItem>(tValue, IDK_USERDATA));
	}

	template<class Ar>
	void ex_serialize(Ar& ar, QAbstractGraphicsShapeItem& tValue)
	{
		ex_serialize(ar, static_cast<QGraphicsItem&>(tValue));
		ar.io("brush", EtBrushApt<QAbstractGraphicsShapeItem>(tValue));
		ar.io("pen", EtPenApt<QAbstractGraphicsShapeItem>(tValue));
	}

	template<class Ar>
	void ex_serialize(Ar& ar, QGraphicsRectItem& tValue)
	{
		ex_serialize(ar, static_cast<QAbstractGraphicsShapeItem&>(tValue));
		ar.io("rect", EtRectApt<QGraphicsRectItem>(tValue));
	}

	template<class Ar>
	void ex_serialize(Ar& ar, QGraphicsLineItem& tValue)
	{
		ex_serialize(ar, static_cast<QGraphicsItem&>(tValue));
		ar.io("pen", EtPenApt<QGraphicsLineItem>(tValue));
		ar.io("line", EtLineApt<QGraphicsLineItem>(tValue));
	}

	template<class Ar>
	void ex_serialize(Ar& ar, QGraphicsEllipseItem& tValue)
	{
		ex_serialize(ar, static_cast<QAbstractGraphicsShapeItem&>(tValue));
		ar.io("rect", EtRectApt<QGraphicsEllipseItem>(tValue));
		ar.io("spanAngle", EtSpanAngleApt<QGraphicsEllipseItem>(tValue));
		ar.io("startAngle", EtStartAngleApt<QGraphicsEllipseItem>(tValue));
	}

	template<class Ar>
	void ex_serialize(Ar& ar, QGraphicsPolygonItem& tValue)
	{
		ex_serialize(ar, static_cast<QAbstractGraphicsShapeItem&>(tValue));
		ar.io("polygon", EtPolygonApt<QGraphicsPolygonItem>(tValue));
		ar.io("fillRule", EtFillRuleApt<QGraphicsPolygonItem>(tValue));
	}

	template<class Ar>
	void ex_serialize(Ar& ar, QGraphicsPathItem& tValue)
	{
		ex_serialize(ar, static_cast<QAbstractGraphicsShapeItem&>(tValue));
		ar.io("path", EtPathApt<QGraphicsPathItem>(tValue));
	}

	template<class Ar>
	void ex_serialize(Ar& ar, QGraphicsPixmapItem& tValue)
	{
		ex_serialize(ar, static_cast<QGraphicsItem&>(tValue));
		ar.io("image", EtPixmapApt<QGraphicsPixmapItem>(tValue, IDK_PIXMAP));
	}
#ifdef QT_SVG_LIB
	template<class Ar>
	void ex_serialize(Ar& ar, QGraphicsSvgItem& tValue)
	{
		ex_serialize(ar, static_cast<QGraphicsItem&>(tValue));
		ar.io("image", EtSvgApt<QGraphicsSvgItem>(tValue, IDK_SVG));
	}
#endif
	template<class Ar>
	void ex_serialize(Ar& ar, QGraphicsTextItem& tValue)
	{
		ex_serialize(ar, static_cast<QGraphicsItem&>(tValue));
		ar.io("font", EtFontApt<QGraphicsTextItem>(tValue));
		ar.io("text", EtPlainTextApt<QGraphicsTextItem>(tValue));
		ar.io("textWidth", EtTextWidthApt<QGraphicsTextItem>(tValue));
		ar.io("textColor", EtDefaultTextColorApt<QGraphicsTextItem>(tValue));
	}

	template<class Ar>
	void ex_serialize(Ar& ar, QGraphicsItemGroup& tValue)
	{
		ex_serialize(ar, static_cast<QGraphicsItem&>(tValue));
	}

	template<class Ar>
	void ex_serialize(Ar& ar, QGraphicsScene& tValue)
	{
		ar.io("sceneRect", EtSceneRectApt<QGraphicsScene>(tValue));
		ar.io("backgroundBrush", EtBackgroundBrushApt<QGraphicsScene>(tValue));
		ar.io("foregroundBrush", EtForegroundBrushApt<QGraphicsScene>(tValue));
	}

}