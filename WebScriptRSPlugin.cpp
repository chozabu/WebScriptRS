#include <retroshare/rsplugin.h>
#include <util/rsversion.h>
#include <QTranslator>
#include <QIcon>

#include "WebScriptRSPlugin.h"
//#include "LinksDialog.h"
#include "topjcdialog.h"

#include "p3JsonRS.h"

//static void *inited = new WebScriptRSPlugin();

//inited->rs_cache_service();

extern "C" {
	void *RETROSHARE_PLUGIN_provide()
	{
        static WebScriptRSPlugin *p = new WebScriptRSPlugin() ;

		return (void*)p ;
	}

	// This symbol contains the svn revision number grabbed from the executable. 
	// It will be tested by RS to load the plugin automatically, since it is safe to load plugins
	// with same revision numbers, assuming that the revision numbers are up-to-date.
	//
    uint32_t RETROSHARE_PLUGIN_revision = RsUtil::retroshareRevision();// SVN_REVISION_NUMBER ;

	// This symbol contains the svn revision number grabbed from the executable. 
	// It will be tested by RS to load the plugin automatically, since it is safe to load plugins
	// with same revision numbers, assuming that the revision numbers are up-to-date.
	//
	uint32_t RETROSHARE_PLUGIN_api = RS_PLUGIN_API_VERSION ;
}

#define IMAGE_LINKS ":/images/dice.png"

void WebScriptRSPlugin::getPluginVersion(int& major,int& minor,int& svn_rev) const
{
	major = 5 ;
	minor = 4 ;
    svn_rev = RsUtil::retroshareRevision();//SVN_REVISION_NUMBER ;
}

WebScriptRSPlugin::WebScriptRSPlugin()
{
    //mRanking = NULL ;
    mMsgque = new msgQue();
    tpage = new WebScriptDialog();
    mJsonP3 = NULL;
	mainpage = NULL ;
        mIcon = NULL ;
        mPlugInHandler = NULL;
        mPeers = NULL;
        mFiles = NULL;
}

void WebScriptRSPlugin::setInterfaces(RsPlugInInterfaces &interfaces){

    mPeers = interfaces.mPeers;
    mFiles = interfaces.mFiles;
}

MainPage *WebScriptRSPlugin::qt_page() const
{
	if(mainpage == NULL)
        mainpage = tpage;//new WebScriptDialog();//mPeers, mFiles) ;

	return mainpage ;
}

RsPQIService * WebScriptRSPlugin::rs_pqi_service() const
{
    if(mJsonP3 == NULL){
        mJsonP3 = new p3JsonRS(mPlugInHandler, mPeers , mMsgque) ;
        p3JsonRS *jsonRs = mJsonP3;
        tpage->setP3service(mJsonP3);
    }

    return mJsonP3 ;
}

RsCacheService *WebScriptRSPlugin::rs_cache_service() const
{
    /*if(mRanking == NULL)
	{
                mRanking = new p3Ranking(mPlugInHandler) ; // , 3600 * 24 * 30 * 6); // 6 Months
		rsRanks = mRanking ;
	}

    return mRanking ;*/
    return NULL;
}

void WebScriptRSPlugin::setPlugInHandler(RsPluginHandler *pgHandler){
    mPlugInHandler = pgHandler;

}

QIcon *WebScriptRSPlugin::qt_icon() const
{
	if(mIcon == NULL)
	{
        Q_INIT_RESOURCE(WebScriptRS_images) ;

        mIcon = new QIcon(IMAGE_LINKS) ;
	}

	return mIcon ;
}

std::string WebScriptRSPlugin::getShortPluginDescription() const
{
    return "infosoon";//QApplication::translate("WebScriptRSPlugin", "This plugin provides a set of cached links, and a voting system to promote them.").toUtf8().constData();
}

std::string WebScriptRSPlugin::getPluginName() const
{
    return "WebScriptRS";//QApplication::translate("WebScriptRSPlugin", "WebScriptRS").toUtf8().constData();
}

QTranslator* WebScriptRSPlugin::qt_translator(QApplication */*app*/, const QString& languageCode, const QString& externalDir) const
{
	if (languageCode == "en") {
		return NULL;
	}

	QTranslator* translator = new QTranslator();

    if (translator->load(externalDir + "/WebScriptRS_" + languageCode + ".qm")) {
		return translator;
    } else if (translator->load(":/lang/WebScriptRS_" + languageCode + ".qm")) {
		return translator;
	}

	delete(translator);
	return NULL;
}
