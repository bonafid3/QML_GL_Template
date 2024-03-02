#include "graphicslayer.h"
#include "myrenderer.h"

#include <QQuickItem>
#include <QFileDialog>
#include <QQuickWindow>

#include <QPainter>

#include <cmath>

#include <immintrin.h>

GraphicsLayer::GraphicsLayer() :
	mPerspective(false),
	mAllowCameraRotation(false)
{

	this->setAcceptHoverEvents(true);
	this->setFlags(QQuickItem::ItemClipsChildrenToShape | QQuickItem::ItemHasContents | QQuickItem::ItemIsFocusScope | QQuickItem::ItemAcceptsDrops);
	this->setAcceptedMouseButtons(Qt::AllButtons);

	mTimer = new QTimer(this);
	mTimer->setObjectName("mTimer");
	connect(mTimer, SIGNAL(timeout()), this, SLOT(on_mTimer_timeout()));
	mTimer->start(15);

    QObject::connect(this, &QQuickItem::windowChanged, this, &GraphicsLayer::onWindowChanged);

    mStringPositions.push_back(QVector2D(0,0));
    mStringPositions.push_back(QVector2D(100,100));

    setLineThickness(50);

}

void GraphicsLayer::on_mTimer_timeout()
{
}

void GraphicsLayer::onWindowChanged(QQuickWindow *window)
{
	/*
	if (window)
	{
		this->window()->setColor(QColor{ 0, 0, 0, 255 });
		this->window()->setClearBeforeRendering(true);
		this->window()->setPersistentOpenGLContext(true);
	}*/
}

MyRenderer* GraphicsLayer::createRenderer() const
{
	return new MyRenderer;
}



Q_INVOKABLE void GraphicsLayer::resetCamera()
{
    mCameraCommands.push_back({QVector3D(0,0,2), QVector3D(0,0,0), QVector3D(0,1,0)});
}

std::vector<QVector2D> &GraphicsLayer::stringPositions()
{
    return mStringPositions;
}


bool GraphicsLayer::allowCameraRotation() const
{
	return mAllowCameraRotation;
}

void GraphicsLayer::setAllowCameraRotation(const bool allow)
{
	if(mAllowCameraRotation == allow)
		return;
	mAllowCameraRotation = allow;
	Q_EMIT allowCameraRotationChanged();
}

bool GraphicsLayer::perspective() const
{
	return mPerspective;
}

void GraphicsLayer::setPerspective(const bool p)
{
	if(mPerspective == p) return;
	mPerspective = p;
	Q_EMIT(perspectiveChanged());
}


QString GraphicsLayer::warningMessage() const
{
	return mWarningMessage;
}

void GraphicsLayer::setWarningMessage(const QString &msg)
{
	//if(mWarningMessage == msg)
	//return;

	mWarningMessage = msg;
	Q_EMIT warningMessageChanged();
}

bool GraphicsLayer::leftMouseButtonPressed() const
{
	return mLeftMouseButtonPressed;
}

QPoint GraphicsLayer::leftMouseButtonPressPosition() const
{
	return mLeftMouseButtonPressPosition;
}

bool GraphicsLayer::rightMouseButtonPressed() const
{
	return mRightMouseButtonPressed;
}

QPoint GraphicsLayer::rightMouseButtonPressPosition() const
{
	return mRightMouseButtonPressPosition;
}

void GraphicsLayer::setLineThickness(float lineThickness)
{
	mLineThickness = lineThickness / 5000.f;
}

float GraphicsLayer::lineThickness() const
{
	return mLineThickness;
}

bool GraphicsLayer::syncRequired()
{
	return mSyncRequired;
}

void GraphicsLayer::setSyncRequired(bool newState)
{
	mSyncRequired = newState;
}

QPoint GraphicsLayer::mousePosition() const
{
	return mMousePosition;
}

void GraphicsLayer::onKeyPressed(QVariant v)
{
	auto key = v.value<int>();
	mKeyDown[key] = true;
}

void GraphicsLayer::onKeyReleased(QVariant v)
{
	auto key = v.value<int>();
	mKeyDown[key] = false;
}

void GraphicsLayer::onWheeled(QVariant wheelEventVariant)
{
	auto we = wheelEventVariant.value<QObject*>();
	auto adv = we->property("angleDelta");

	if(adv.isValid())
	{
		auto ad = adv.value<QPoint>();
		mWheelValue.push_back(ad.y());
	}
}

void GraphicsLayer::onMousePressed(QVariant vx, QVariant vy, QVariant vb)
{
	if(vb.value<int>() == Qt::LeftButton)
	{
		mLeftMouseButtonPressed = true;
		mLeftMouseButtonPressPosition = QPoint(vx.value<int>(), vy.value<int>());
	}

	if(vb.value<int>() == Qt::RightButton)
	{
		mRightMouseButtonPressed = true;
		mRightMouseButtonPressPosition = QPoint(vx.value<int>(), vy.value<int>());
	}
}

void GraphicsLayer::onMouseReleased(QVariant vx, QVariant vy, QVariant vb)
{
	if(vb.value<int>() == Qt::LeftButton)
		mLeftMouseButtonPressed = false;

	if(vb.value<int>() == Qt::RightButton)
		mRightMouseButtonPressed = false;
}

void GraphicsLayer::onMousePositionChanged(QVariant vx, QVariant vy)
{
	mMousePosition = QPoint(vx.value<int>(), vy.value<int>());
}
