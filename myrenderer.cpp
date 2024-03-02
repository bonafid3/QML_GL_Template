#include "myrenderer.h"

#include <QQuickWindow>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLExtraFunctions>

#include <QOpenGLVertexArrayObject>

#include "graphicslayer.h"
#include "myrenderer.h"
#include "utils.h"

#define qd qDebug()

MyRenderer::MyRenderer() : mEye(0, 0, 2), mView(0, 0, 0), mUp(0, 1, 0)
{
	initializeOpenGLFunctions();

	QOpenGLContext *ctx = QOpenGLContext::currentContext();
	auto f = ctx->functions();
	auto ef = ctx->extraFunctions();

	//qd << "Supported shading language version"  << QString((char*)glGetString(GL_VENDOR))<< QString((char*)glGetString(GL_RENDERER)) << QString((char*)glGetString(GL_VERSION)) << QString((char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

	// drawing billboard like lines
	initShader(mLineShader, ":/line_vs.glsl", ":/line_gs.glsl", ":/line_fs.glsl");

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);

}

void MyRenderer::initShader(QOpenGLShaderProgram& shaderProgram, const QString& vs, const QString& gs, const QString& fs)
{
	if(!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, vs)) {
		qDebug()<<"compiler vertex error";
		exit(0);
	}

	if(gs.size())
	{
		if(!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Geometry, gs)) {
			qDebug()<<"compiler geometry error";
			exit(0);
		}
	}

	if(!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, fs)) {
		qDebug()<<"compiler fragment error";
		exit(0);
	}

	if(!shaderProgram.link()) {
		qDebug()<<"link shader error";
		exit(0);
	}
}

QOpenGLFramebufferObject* MyRenderer::createFramebufferObject(const QSize& s)
{
	QOpenGLFramebufferObjectFormat format;
	format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
	return new QOpenGLFramebufferObject(s, format);
}

void MyRenderer::synchronize(QQuickFramebufferObject *fbo)
{
	GraphicsLayer* graphicsLayer = static_cast<GraphicsLayer*>(fbo);
	if(graphicsLayer == nullptr)
		return;

	mWindow = graphicsLayer->window();

	if(graphicsLayer->mCameraCommands.size())
	{
		mEye = graphicsLayer->mCameraCommands.back().mEye;
		mView = graphicsLayer->mCameraCommands.back().mView;
		mUp = graphicsLayer->mCameraCommands.back().mUp;
		graphicsLayer->mCameraCommands.pop_back();
	}

	mDimensions = QVector2D(graphicsLayer->width(), graphicsLayer->height());

	int w=graphicsLayer->width(), h=graphicsLayer->height();

	float aspect = (float) w / (float) h;
	float FOV = 45.0f;
	float os = mEye.z() * tan(FOV/2.0 * M_PI/180.0f);

	mPerspectiveProjection = graphicsLayer->perspective();
	mAllowCameraRotation = graphicsLayer->allowCameraRotation();

	mProjectionMatrix.setToIdentity();
	if(mPerspectiveProjection) {
		mProjectionMatrix.perspective(FOV, mDimensions.x()/mDimensions.y(), ZNEAR, ZFAR);
	} else {
		if(w <= h)  {
			mProjectionMatrix.ortho(-os, os, -os/aspect, os/aspect, ZNEAR, ZFAR);
		} else {
			mProjectionMatrix.ortho(-os*aspect, os*aspect, -os, os, ZNEAR, ZFAR);
		}
	}

	constexpr float widthScaler = 1.0f / 800.0f;
	constexpr float heightScaler = 1.0f / 800.0f;

	// get the texture if loaded
	if(graphicsLayer->syncRequired())
	{
		graphicsLayer->setSyncRequired(false);

		/*
		float halfW = mTexture->width() / 2.0f;
		float halfH = mTexture->height() / 2.0f;

		mPoints.clear();
		mColors.clear();
		for(auto&& nailPos : graphicsLayer->nailPositions())
		{
			mPoints.push_back(QVector3D(((float(nailPos.x())-halfW)*widthScaler), ((float(nailPos.y())-halfH)*heightScaler), 1.0/800.0f));
			mColors.push_back(QVector3D(0.3, 0.3, 1.0)); // coloring all nails to blue
		}
		*/
	} // syncRequired

    constexpr float halfW = 400;
    constexpr float halfH = 400;

    // STRING POINTS
    mStringPoints.clear();
    mStringColors.clear();
    for(auto&& stringPos : graphicsLayer->stringPositions())
    {
        mStringPoints.push_back(QVector3D(((float(stringPos.x())-halfW)*widthScaler), ((float(stringPos.y())-halfH)*heightScaler), 1.0/800.0f));
        mStringColors.push_back(QVector3D(0.3f, 0.3f, 0.3f));
    }
    if(mStringColors.size())
    {
        mStringPoints.back().setZ(3.0 / 800.0f);
        mStringColors.back() = QVector3D(1.0, 0.3, 0.3);
    }

	mLineThickness = graphicsLayer->lineThickness();

	float speed = 0.01f;
	QVector3D eyeDir = (mView-mEye).normalized();

	if(graphicsLayer->mWheelValue.size())
	{
		auto amt = graphicsLayer->mWheelValue.front() / 120.0f;
		graphicsLayer->mWheelValue.pop_front();

		QVector3D pw;
		if(screenToWorld(QPoint(mMouseX, mMouseY), -1.0, pw))
		{
			QVector3D wheelDir = (pw - mEye).normalized();
			mEye += -wheelDir * speed * amt;
			mView += -wheelDir * speed * amt;
		}
		else
		{
			mEye += -eyeDir * speed * amt;
			mView += -eyeDir * speed * amt;
		}
	}

	if(graphicsLayer->mKeyDown[Qt::Key_W])
	{
		if(!mAllowCameraRotation) {
			mEye += -mUp * speed;
			mView += -mUp * speed;
		} else {
			mEye += eyeDir * speed;
			mView += eyeDir * speed;
		}
	}

	if(graphicsLayer->mKeyDown[Qt::Key_S])
	{
		if(!mAllowCameraRotation) {
			mEye += mUp * speed;
			mView += mUp * speed;
		} else {
			mEye += -eyeDir * speed;
			mView += -eyeDir * speed;
		}
	}

	if(graphicsLayer->mKeyDown[Qt::Key_A])
	{
		QVector3D xaxis = QVector3D::crossProduct(mUp, eyeDir).normalized();
		mEye += xaxis * speed;
		mView += xaxis * speed;
	}

	if(graphicsLayer->mKeyDown[Qt::Key_D])
	{
		QVector3D xaxis = QVector3D::crossProduct(mUp, eyeDir).normalized();
		mEye += -xaxis * speed;
		mView += -xaxis * speed;
	}

	if(graphicsLayer->mKeyDown[Qt::Key_E])
	{
		mEye += mUp * speed;
		mView += mUp * speed;
	}

	if(graphicsLayer->mKeyDown[Qt::Key_Q])
	{
		mEye += -mUp * speed;
		mView += -mUp * speed;
	}
}

void MyRenderer::mousePositionChanged(float mouseX, float mouseY)
{
	QVector3D eyeDir = (mView-mEye).normalized();

	if(!mAllowCameraRotation)
		return;

	if(mLeftMouseButtonPressed)
	{
		QQuaternion result;

		float xangle = -d2r(mLeftMouseButtonPressCoord.x() - mouseX) * 0.1f;
		float yangle = -d2r(mLeftMouseButtonPressCoord.y() - mouseY) * 0.1f;

		mLeftMouseButtonPressCoord.setX(mouseX);
		mLeftMouseButtonPressCoord.setY(mouseY);

		QVector3D yaxis = QVector3D(0,1,0); // rotate around the world's up vector
		QQuaternion r_quat(cos(xangle/2), yaxis.x()*sin(xangle/2), yaxis.y()*sin(xangle/2), yaxis.z()*sin(xangle/2));
		r_quat.normalize();
		QQuaternion v_quat(0, eyeDir); // create view quaternion from view vector
		QQuaternion u_quat(0, mUp);
		result = (r_quat * v_quat) * r_quat.conjugated();
		QVector3D eyeShit = QVector3D(result.x(), result.y(), result.z()).normalized();
		mView = mEye + eyeShit;

		result = (r_quat * u_quat) * r_quat.conjugated();
		mUp = QVector3D(result.x(), result.y(), result.z()).normalized();
		qDebug()  << "eye" << mEye << "view" << mView << "up" << mUp << "eyeDir" << eyeDir;

		// new eyedir based on the previous rotation about the vertical camera axis
		eyeDir = (mView-mEye).normalized();

		// rotation axis
		QVector3D xaxis = QVector3D::crossProduct(mUp, eyeDir).normalized();

		QQuaternion rr_quat(cos(xangle/2), xaxis.x()*sin(yangle/2), xaxis.y()*sin(yangle/2), xaxis.z()*sin(yangle/2));
		rr_quat.normalize();
		QQuaternion vv_quat(0, eyeDir); // create view quaternion from view vector
		QQuaternion uu_quat(0, mUp);
		result = (rr_quat * vv_quat) * rr_quat.conjugated();
		eyeShit = QVector3D(result.x(), result.y(), result.z()).normalized();
		mView = mEye + eyeShit;

		result = (rr_quat * uu_quat) * rr_quat.conjugated();
		mUp = QVector3D(result.x(), result.y(), result.z()).normalized();

		qd << "quat res" << result;
	}

	update();
}


void MyRenderer::render()
{
	QMatrix4x4 viewMatrix;
	viewMatrix.lookAt(mEye, mView, mUp);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendEquation( GL_FUNC_ADD );

	QOpenGLContext *ctx = QOpenGLContext::currentContext();
	auto f = ctx->functions();
	auto ef = ctx->extraFunctions();

	////////////
	// DRAW QUAD
	////////////

	float w = 1.0f;
	float h = 1.0f;


	///////////////
	// DRAW STRINGS
	///////////////
	if(mStringPoints.size())
	{
		mLineShader.bind();
		mLineShader.setUniformValue("qt_modelViewMatrix", viewMatrix);
		mLineShader.setUniformValue("qt_projectionMatrix", mProjectionMatrix);
		mLineShader.setUniformValue("qt_cameraPos", mEye);

		mLineShader.setUniformValue("qt_lineWidth", mLineThickness);

		mLineShader.enableAttributeArray(0);
		mLineShader.enableAttributeArray(1);
		mLineShader.setAttributeArray(0, GL_FLOAT, mStringPoints.data(), 3);
		mLineShader.setAttributeArray(1, GL_FLOAT, mStringColors.data(), 3);
		glDrawArrays(GL_LINE_STRIP, 0, mStringPoints.size());
		mLineShader.disableAttributeArray(0);
		mLineShader.disableAttributeArray(1);
		mLineShader.release();
	}

	mWindow->resetOpenGLState();
}

bool MyRenderer::screenToWorld(const QPoint& winCoord, int z_ndc, QVector3D &pw)
{
	// define the plane
	sPlane plane(QVector3D(0,0,1), QVector3D(0,0,0));

	QMatrix4x4 modelViewMatrix;

	QVector4D ray_clip((2.0f * winCoord.x()) / mDimensions.x() - 1.0f, (2.0f * winCoord.y()) / mDimensions.y() - 1.0f, z_ndc, 1.0);
	QVector4D ray_eye = mProjectionMatrix.inverted() * ray_clip;

	ray_eye.setZ(-1);
	ray_eye.setW(0);

	modelViewMatrix.lookAt(mEye, mView, mUp);
	QVector3D rw = QVector4D(modelViewMatrix.inverted() * ray_eye).toVector3D();

	float c, d = QVector3D::dotProduct(plane.n, plane.p);

	QVector3D rayDir = (mView - mEye).normalized();

	if(mPerspectiveProjection) {
		c = QVector3D::dotProduct(plane.n, mEye) - d;
		if(c < 0) { return false; } // we are behind the plane
	} else {
		rw += mEye;
		c = QVector3D::dotProduct(plane.n, rw) - d;
	}

	float a;
	if(mPerspectiveProjection) {
		a = QVector3D::dotProduct(plane.n, rw);
	} else {
		a = QVector3D::dotProduct(plane.n, rayDir);
	}

	if(a==0) { return false; } // ray is parallel to the plane

	float t = c / a;

	if(t==0) return false;

	if(mPerspectiveProjection) {
		pw = mEye - rw * t;
	} else {
		pw = rw - rayDir * t;
	}

	//qd << "ray clip" << ray_clip << "ray eye" << ray_eye << "mEye" << mEye << rw.normalized();
	//qd << "ray_world" << rw << "point in world" << pw << "rw*t" << rw*t << "t" << t;

	return true;
}
