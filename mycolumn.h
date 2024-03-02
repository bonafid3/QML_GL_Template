#pragma once

#include <QQuickItem>

class MyColumn : public QQuickItem
{
	Q_OBJECT

	Q_PROPERTY(qreal spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)
	Q_PROPERTY(QQuickItem* background READ background WRITE setBackground NOTIFY backgroundChanged)

public:
	explicit MyColumn(QQuickItem* parent = nullptr);
	~MyColumn();

	qreal spacing() const;
	void setSpacing(qreal r);

	QQuickItem* background() const;
	void setBackground(QQuickItem* item);

	void positionItems();
	void componentComplete() override;
	void itemChange(ItemChange, const ItemChangeData&) override;

Q_SIGNALS:
	void spacingChanged();
	void backgroundChanged();

public /*slots*/:
	void onChildImplicitWidthChanged();
	void onChildImplicitHeightChanged();

	// old fashioned slot
	Q_SLOT void onChildCheckedChanged();

private:
	qreal mSpacing;
	QQuickItem* mBackground;
};
