#pragma once

#include <retroshare/rsplugin.h>
#include <retroshare-gui/mainpage.h>
#include "p3JsonRS.h"
#include "topjcdialog.h"

class WebScriptRSPlugin: public RsPlugin
{
	public:
        WebScriptRSPlugin() ;
        virtual ~WebScriptRSPlugin() {}

		virtual RsCacheService *rs_cache_service() 		const	;
        virtual RsPQIService * rs_pqi_service() const;

		virtual MainPage       *qt_page()       			const	;
		virtual QIcon          *qt_icon()       			const	;
        //virtual uint16_t        rs_service_id()         const { return RS_SERVICE_TYPE_RANK ; }
		virtual QTranslator    *qt_translator(QApplication *app, const QString& languageCode, const QString& externalDir) const;

		virtual void getPluginVersion(int& major,int& minor,int& svn_rev) const ;
		virtual void setPlugInHandler(RsPluginHandler *pgHandler);

		virtual std::string configurationFileName() const { return std::string() ; }

		virtual std::string getShortPluginDescription() const ;
		virtual std::string getPluginName() const;
		virtual void setInterfaces(RsPlugInInterfaces& interfaces);
    private:
		mutable RsPluginHandler *mPlugInHandler;
		mutable RsFiles* mFiles;
		mutable RsPeers* mPeers;
        mutable MainPage* mainpage ;
        mutable WebScriptDialog* tpage;
        mutable QIcon* mIcon ;
        mutable p3JsonRS* mJsonP3 ;
        mutable msgQue* mMsgque;
};

