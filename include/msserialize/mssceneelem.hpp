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

BeginBaExSe(QGraphicsItem)
    static std::map<int32_t, QGraphicsItemConvT> map = {
    {QGraphicsRectItem::Type, QGraphicsItemConv<Ar, QGraphicsRectItem>},
    {QGraphicsLineItem::Type, QGraphicsItemConv<Ar, QGraphicsLineItem>},
    {QGraphicsEllipseItem::Type, QGraphicsItemConv<Ar, QGraphicsEllipseItem>},
    {QGraphicsPixmapItem::Type, QGraphicsItemConv<Ar, QGraphicsPixmapItem>},
#ifdef QT_SVG_LIB
    {QGraphicsSvgItem::Type, QGraphicsItemConv<Ar, QGraphicsSvgItem>},
#endif
    {QGraphicsTextItem::Type, QGraphicsItemConv<Ar, QGraphicsTextItem>},
    {QGraphicsPolygonItem::Type, QGraphicsItemConv<Ar, QGraphicsPolygonItem>},
    {QGraphicsPathItem::Type, QGraphicsItemConv<Ar, QGraphicsPathItem>},
    {QGraphicsItemGroup::Type, QGraphicsItemConv<Ar, QGraphicsItemGroup>}
    };
    int32_t type = tValue ? tValue->type() : 0;
    ar.io("type", type);
EndBaExSe(map, type)
