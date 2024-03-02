#pragma once

#include <QDebug>
#include <QQuickFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QOpenGLTexture>

#include <QtConcurrent/QtConcurrent>

#include <list>

#include "myrenderer.h"

struct sCamera
{
	QVector3D mEye;
	QVector3D mView;
	QVector3D mUp;
};

class GraphicsLayer : public QQuickFramebufferObject
{
	Q_OBJECT
public:

	Q_PROPERTY(QString warningMessage             READ warningMessage        WRITE setWarningMessage        NOTIFY warningMessageChanged)
	Q_PROPERTY(bool perspective                   READ perspective           WRITE setPerspective           NOTIFY perspectiveChanged)
	Q_PROPERTY(bool allowCameraRotation           READ allowCameraRotation   WRITE setAllowCameraRotation   NOTIFY allowCameraRotationChanged)

	GraphicsLayer();

	MyRenderer* createRenderer() const override;

	MyRenderer* mRenderer = nullptr;

	QMap<int, bool> mKeyDown;
	std::list<int> mWheelValue;

	Q_INVOKABLE void setLineThickness(float lineThickness);

	bool allowCameraRotation() const;
	void setAllowCameraRotation(const bool p);

	bool perspective() const;
	void setPerspective(const bool p);

	QString warningMessage() const;
	void setWarningMessage(const QString& msg);

	float lineThickness() const;

	bool syncRequired();
	void setSyncRequired(bool newState);

	QPoint mousePosition() const;

	bool leftMouseButtonPressed() const;
	QPoint leftMouseButtonPressPosition() const;

	bool rightMouseButtonPressed() const;
	QPoint rightMouseButtonPressPosition() const;

	Q_INVOKABLE void onKeyPressed(QVariant v);
	Q_INVOKABLE void onKeyReleased(QVariant v);

	Q_INVOKABLE void onWheeled(QVariant wheelEventVariant);

	Q_INVOKABLE void onMousePressed(QVariant vx, QVariant vy, QVariant vb);
	Q_INVOKABLE void onMouseReleased(QVariant vx, QVariant vy, QVariant vb);
	Q_INVOKABLE void onMousePositionChanged(QVariant vx, QVariant vy);

	// if the item has focus these would be called
	//void keyPressEvent(QKeyEvent *e) override;
	//void keyReleaseEvent(QKeyEvent *e) override;

	Q_INVOKABLE void resetCamera();

    std::vector<QVector2D>& stringPositions();


Q_SIGNALS:
	void warningMessageChanged();
	void perspectiveChanged();
	void allowCameraRotationChanged();

public Q_SLOTS:
	void on_mTimer_timeout();
	void onWindowChanged(QQuickWindow* window);

private:

	QString mWarningMessage;

	friend class MyRenderer;

	std::vector<sCamera> mCameraCommands;

    std::vector<QVector2D> mStringPositions;

	bool mPerspective;
	bool mAllowCameraRotation;

	bool mSyncRequired = false;

	QTimer* mTimer;

	QPoint mMousePosition;
	QPoint mLeftMouseButtonPressPosition;
	QPoint mRightMouseButtonPressPosition;

	bool mLeftMouseButtonPressed = false;
	bool mRightMouseButtonPressed = false;

	float mLineThickness = 1.0/800.0f;

};
