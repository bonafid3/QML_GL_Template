#include "mycolumn.h"

MyColumn::MyColumn(QQuickItem* parent) : QQuickItem{ parent }, mSpacing{ 0.0 }, mBackground{ nullptr }
{
	setFlag(ItemHasContents, true);
}

MyColumn::~MyColumn()
{
}

qreal MyColumn::spacing() const
{
	return mSpacing;
}

void MyColumn::setSpacing(qreal s)
{
	if (mSpacing == s) return;
	mSpacing = s;

	Q_EMIT spacingChanged();

	if (isComponentComplete())
		positionItems();
}

QQuickItem *MyColumn::background() const
{
	return mBackground;
}

void MyColumn::setBackground(QQuickItem* item)
{
	mBackground = item;
	mBackground->setParentItem(this);
	Q_EMIT backgroundChanged();
}

void MyColumn::positionItems()
{
	qreal maxImplicitWidth = 0.0;
	qreal totalImplicitHeight = 0.0;

	QList<QQuickItem*> children = childItems();
	for (int i = 0; i < children.count(); ++i) {
		QQuickItem* child = children.at(i);

		child->setY(totalImplicitHeight);

		if (child->implicitWidth() > maxImplicitWidth)
			maxImplicitWidth = child->implicitWidth();
		if (child->implicitHeight() > 0) {
			totalImplicitHeight += child->implicitHeight();
			totalImplicitHeight += mSpacing;
		}
	}

	totalImplicitHeight -= mSpacing;

	setImplicitWidth(maxImplicitWidth);
	setImplicitHeight(totalImplicitHeight);
}

void MyColumn::componentComplete()
{
	positionItems();
	QQuickItem::componentComplete();
}

void MyColumn::onChildImplicitWidthChanged()
{
	positionItems();
}

void MyColumn::onChildImplicitHeightChanged()
{
	positionItems();
}

void MyColumn::onChildCheckedChanged()
{
	QQuickItem* s = dynamic_cast<QQuickItem*>(sender());
	if(mBackground != nullptr && s != nullptr) {
		mBackground->setProperty("x", s->x());
		mBackground->setProperty("y", s->y());

		mBackground->setProperty("width", s->width());
		mBackground->setProperty("height", s->height());
	}
}

void MyColumn::itemChange(ItemChange change, const ItemChangeData& value)
{
	if (change == ItemChildAddedChange)
	{
		QObject::connect(value.item, &QQuickItem::implicitWidthChanged, this, &MyColumn::onChildImplicitWidthChanged);
		QObject::connect(value.item, &QQuickItem::implicitHeightChanged, this, &MyColumn::onChildImplicitHeightChanged);

		QObject::connect(value.item, SIGNAL(checkedChanged()), this, SLOT(onChildCheckedChanged()));
	}
	else
	{
		positionItems();
	}
	QQuickItem::itemChange(change, value);
}
