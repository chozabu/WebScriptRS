#ifndef PYTABCONTENTS_H
#define PYTABCONTENTS_H

#include <QWidget>
#include "embpyqt/embeddedpyqt.h"
#include "p3JsonRS.h"

class PyTabContents : public QWidget
{
    Q_OBJECT
public:

    EmbeddedPyQt *embpyqt;
    explicit PyTabContents(QWidget *parent = 0, p3JsonRS *p3servicein = NULL);

    void setP3service(p3JsonRS *p3servicein);
    p3JsonRS *p3service;

signals:

public slots:

};

#endif // PYTABCONTENTS_H
