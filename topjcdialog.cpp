#include "topjcdialog.h"
#include "ui_topjcdialog.h"

/** Constructor
LinksDialog::LinksDialog(RsPeers *peers, RsFiles *files, QWidget *parent)
: MainPage(parent), mPeers(peers), mFiles(files)
{

}**/

TopJCDialog::TopJCDialog(QWidget *parent) :
    MainPage(parent),
    ui(new Ui::TopJCDialog)
{
    ui->setupUi(this);
}

TopJCDialog::~TopJCDialog()
{
    delete ui;
}
