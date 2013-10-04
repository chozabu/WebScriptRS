#include <retroshare/rsplugin.h>
#include <util/rsversion.h>
#include <QTranslator>
#include <QIcon>

#include "EmptyPluginPlugin.h"
//#include "LinksDialog.h"
#include "topjcdialog.h"

//static void *inited = new EmptyPluginPlugin();

//inited->rs_cache_service();

extern "C" {
	void *RETROSHARE_PLUGIN_provide()
	{
        static EmptyPluginPlugin *p = new EmptyPluginPlugin() ;

		return (void*)p ;
	}

	// This symbol contains the svn revision number grabbed from the executable. 
	// It will be tested by RS to load the plugin automatically, since it is safe to load plugins
	// with same revision numbers, assuming that the revision numbers are up-to-date.
	//
	uint32_t RETROSHARE_PLUGIN_revision = SVN_REVISION_NUMBER ;

	// This symbol contains the svn revision number grabbed from the executable. 
	// It will be tested by RS to load the plugin automatically, since it is safe to load plugins
	// with same revision numbers, assuming that the revision numbers are up-to-date.
	//
	uint32_t RETROSHARE_PLUGIN_api = RS_PLUGIN_API_VERSION ;
}

#define IMAGE_LINKS ":/images/dice.png"

void EmptyPluginPlugin::getPluginVersion(int& major,int& minor,int& svn_rev) const
{
	major = 5 ;
	minor = 4 ;
	svn_rev = SVN_REVISION_NUMBER ;
}

EmptyPluginPlugin::EmptyPluginPlugin()
{
    //mRanking = NULL ;
	mainpage = NULL ;
        mIcon = NULL ;
        mPlugInHandler = NULL;
        mPeers = NULL;
        mFiles = NULL;
}

void EmptyPluginPlugin::setInterfaces(RsPlugInInterfaces &interfaces){

    mPeers = interfaces.mPeers;
    mFiles = interfaces.mFiles;
}

MainPage *EmptyPluginPlugin::qt_page() const
{
	if(mainpage == NULL)
                mainpage = new TopJCDialog();//mPeers, mFiles) ;

	return mainpage ;
}

RsCacheService *EmptyPluginPlugin::rs_cache_service() const
{
    /*if(mRanking == NULL)
	{
                mRanking = new p3Ranking(mPlugInHandler) ; // , 3600 * 24 * 30 * 6); // 6 Months
		rsRanks = mRanking ;
	}

    return mRanking ;*/
    return NULL;
}

void EmptyPluginPlugin::setPlugInHandler(RsPluginHandler *pgHandler){
    mPlugInHandler = pgHandler;

}

QIcon *EmptyPluginPlugin::qt_icon() const
{
	if(mIcon == NULL)
	{
        Q_INIT_RESOURCE(EmptyPlugin_images) ;

        mIcon = new QIcon(IMAGE_LINKS) ;
	}

	return mIcon ;
}

std::string EmptyPluginPlugin::getShortPluginDescription() const
{
    return "infosoon";//QApplication::translate("EmptyPluginPlugin", "This plugin provides a set of cached links, and a voting system to promote them.").toUtf8().constData();
}

std::string EmptyPluginPlugin::getPluginName() const
{
    return "EmptyPlugin";//QApplication::translate("EmptyPluginPlugin", "EmptyPlugin").toUtf8().constData();
}

QTranslator* EmptyPluginPlugin::qt_translator(QApplication */*app*/, const QString& languageCode, const QString& externalDir) const
{
	if (languageCode == "en") {
		return NULL;
	}

	QTranslator* translator = new QTranslator();

    if (translator->load(externalDir + "/EmptyPlugin_" + languageCode + ".qm")) {
		return translator;
    } else if (translator->load(":/lang/EmptyPlugin_" + languageCode + ".qm")) {
		return translator;
	}

	delete(translator);
	return NULL;
}
