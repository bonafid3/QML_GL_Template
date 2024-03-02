#pragma once

#include <QQuickFramebufferObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <QOpenGLVertexArrayObject>

#include <QVector3D>
#include <QMatrix4x4>

#define ZNEAR 0.001f
#define ZFAR 50.0f

class GraphicsLayer;

class MyRenderer : public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions
{
public:
	MyRenderer();
	~MyRenderer() = default;

	QOpenGLFramebufferObject* createFramebufferObject(const QSize& s);

	virtual void synchronize(QQuickFramebufferObject *fbo);

	void render();

	void mousePositionChanged(float mouseX, float mouseY);

	bool screenToWorld(const QPoint& winCoord, int z_ndc, QVector3D &pw);
private:


	QQuickWindow* mWindow = nullptr;

	QOpenGLShaderProgram mLineShader;

	QMatrix4x4 mProjectionMatrix;

	QVector3D mEye;
	QVector3D mView;
	QVector3D mUp;

	float mImageOpacity = 0.0f;
	float mNailDiameter = 1.0f;
	float mLineThickness = 1.0/800.0f;

	int mMouseX = 0;
	int mMouseY = 0;

	QVector3D mCachedMousePos;

	QVector2D mDimensions;

	QSize mImageSize;

	int mSavedX = 0;
	int mSavedY = 0;

	bool mPerspectiveProjection = false;
	bool mAllowCameraRotation = false;

	bool mLeftMouseButtonPressed = false;
	bool mRightMouseButtonPressed = false;

	QPoint mLeftMouseButtonPressCoord;
	QPoint mRightMouseButtonPressCoord;

	std::vector<QVector3D> mStringPoints;
	std::vector<QVector3D> mStringColors;

	void initShader(QOpenGLShaderProgram &shaderProgram, const QString &vs, const QString &gs, const QString &fs);
};
