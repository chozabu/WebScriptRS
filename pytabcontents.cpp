#include "pytabcontents.h"
#include "p3JsonRS.h"

PyTabContents::PyTabContents(QWidget *parent, p3JsonRS *p3servicein) :
    QWidget(parent)
{

    p3service = p3servicein;
    embpyqt = new EmbeddedPyQt();
    embpyqt->registerObject(*p3service->bridge);
    embpyqt->registerMetaObject(WebBridgeRS::staticMetaObject);
    embpyqt->registerObject(*this);
    embpyqt->init("embpyqt/python/initembpyqt.py");
    //embpyqt->execute("embpyqt_console.Visible = True", true);
}


void PyTabContents::setP3service(p3JsonRS *p3servicein)
{
    p3service = p3servicein;
}
