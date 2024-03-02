#include <QApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QFontDatabase>

#include "graphicslayer.h"

int main(int argc, char *argv[])
{

/* no texture shown when using this code
	QSurfaceFormat fmt;
	fmt.setSamples(8);
	fmt.setDepthBufferSize(24);
	fmt.setStencilBufferSize(8);
	fmt.setVersion(3, 3);
	//fmt.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(fmt);
*/

	QApplication app(argc, argv);

	QFontDatabase::addApplicationFont(":/HelveticaNeueCyr-Roman.ttf");
	QFontDatabase::addApplicationFont(":/HelveticaNeueCyr-Bold.ttf");

	QFont qtFont = app.font();
	qtFont.setFamily("HelveticaNeueCyr");
	qtFont.setPixelSize(14);
	app.setFont(qtFont);

	qmlRegisterType<GraphicsLayer>("GraphicsLayer", 1, 0, "GraphicsLayer");
	//qmlRegisterType<MyRow>("MyRow", 1, 0, "MyRow");
	//qmlRegisterType<MyColumn>("MyColumn", 1, 0, "MyColumn");
	//qmlRegisterUncreatableMetaObject(Enums::staticMetaObject, "Enums", 1, 0, "SelectedImageType", "Error: enums only");
	//qmlRegisterUncreatableMetaObject(Enums::staticMetaObject, "Enums", 1, 0, "ProgramState", "Error: enums only");

	QQuickView view;
	view.setMinimumSize(QSize(1024, 800));
	view.setResizeMode(QQuickView::SizeRootObjectToView);

	view.rootContext()->setContextProperty("view", &view);
	view.setSource(QStringLiteral("qrc:/main.qml"));

	//Qt::WindowFlags flags = view.flags();
	//flags |= Qt::FramelessWindowHint;
	//flags |= Qt::Window;
	//view.setFlags(flags);

	view.show();

	return app.exec();
}
