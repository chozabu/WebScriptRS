#include "webbridgers.h"

#include "p3JsonRS.h"
#include "retroshare/rsposted.h"
#include <retroshare/rsexpr.h>
#include "gui/RetroShareLink.h"

//#include "retroshare/rsmsgs.h"
//#include "retroshare/rspeers.h"
//#include "retroshare/rsnotify.h"

//rsPosted->;
//rsMsgs->getPublicChatQueue()

#include <QString>
WebBridgeRS::WebBridgeRS(QObject *parent) :
    QObject(parent)
{
    //mRSLink = new Retro

    /*
    rsPeers->getOnlineList();
    rsPeers->getFriendList();
    rsFiles->FileDetails();
    rsFiles->SearchKeywords();
    */
    //rsFiles->FileRequest();
    //virtual bool FileRequest(const std::string& fname, const std::string& hash,
    //nt64_t size, const std::string& dest, TransferRequestFlags flags, const std::list<std::string>& srcIds) = 0;
    //rsFiles->getSharedDirectories();
    /*std::list<std::string> keywords;
    std::list<DirDetails> *results;
    keywords.push_back("mp3");
    rsFiles->SearchKeywords(keywords,results,);
    rsFiles->SearchBoolExp()*/
    //SharedDirInfo *sd;
    //sd->

}

/*QString WebBridgeRS::processLink(QString url){
   // RetroShareLink(url).;
    return url;
}*/

void WebBridgeRS::processLinks(QStringList urls){
    RetroShareLink::process(urls, RetroShareLink::TYPE_UNKNOWN, RSLINK_PROCESS_NOTIFY_ALL);
    //rsFiles->FileRequest()
}

QVariantList WebBridgeRS::searchKeywords(const QString& keywords, QVariantMap searchOptions)
{
    std::cerr << "webscriptrs: keywords in " << keywords.toStdString().c_str() << std::endl;
    QVariantList qResults;
    if (keywords.length() < 3){
        std::cerr << "webscriptrs: not searching due to keywords.length = " << keywords.length() << std::endl;
        return qResults;
    }

    QStringList qWords = keywords.split(" ", QString::SkipEmptyParts);
    std::list<std::string> words;
    QStringListIterator qWordsIter(qWords);
    while (qWordsIter.hasNext())
        words.push_back(qWordsIter.next().toUtf8().constData());

    int n = words.size() ;

    if (n < 1){
        std::cerr << "webscriptrs: searching due to words.size() = " << n << std::endl;
        return qResults;
    }

    NameExpression exprs(ContainsAllStrings,words,true) ;
    LinearizedExpression lin_exp ;
    exprs.linearize(lin_exp) ;



    /* extract keywords from lineEdit */
    // make a compound expression with an AND
    //

    //std::list<DirDetails> finalResults ;

    std::list<DirDetails> initialResults;
    //RS_FILE_HINTS_REMOTE
    //rsFiles->SearchBoolExp(&exprs, initialResults, RS_FILE_HINTS_LOCAL);// | DIR_FLAGS_NETWORK_WIDE | DIR_FLAGS_BROWSABLE) ;
    FileSearchFlags fsf;
    if (searchOptions.value("localindexed", false).toBool()){
        //std::cerr << "incuding local\n";
         fsf = RS_FILE_HINTS_LOCAL;
    }
    if (searchOptions.value("remoteindexed", false).toBool()){
        //std::cerr << "incuding remote\n";
         fsf |= RS_FILE_HINTS_REMOTE;
    }
    if (searchOptions.value("boolexp", false).toBool()){
        rsFiles->SearchBoolExp(&exprs, initialResults, fsf);
    }else{
        rsFiles->SearchKeywords(words, initialResults, fsf);
    }
    //if(searchFriends) fsf = RS_FILE_HINTS_REMOTE;
    //rsFiles->getSharedDirectories();
    //SharedDirInfo sdinfo;
    //sdinfo.

    std::cerr << "webscriptrs: result count: " << initialResults.size() << std::endl;
    /* which extensions do we use? */
    DirDetails dd;


    for(std::list<DirDetails>::iterator resultsIter = initialResults.begin(); resultsIter != initialResults.end(); resultsIter ++)
    {
        //std::cout << "webscriptrs: " << dd.hash << std::endl;
        dd = *resultsIter;
        QVariantMap qdd;
        qdd.insert("age",dd.age);//QString::number(dir.age)?
        //qdd.insert("message",dd.children);
        qdd.insert("size",QString::number(dd.count));
        //qdd.insert("message",dd.flags);
        qdd.insert("hash",QString::fromStdString(dd.hash));
        qdd.insert("id",QString::fromStdString(dd.id));
        qdd.insert("mind_age",dd.min_age);
        qdd.insert("srname",QString::fromUtf8(dd.name.c_str()));
        //qdd.insert("message",dd.parent);
        //qdd.insert("message",dd.parent_groups);
        qdd.insert("path",QString::fromUtf8(dd.path.c_str()));
        qdd.insert("prow",dd.prow);
        //qdd.insert("message",dd.ref);
        qdd.insert("type",dd.type);

        FileInfo info;

        if (rsFiles->FileDetails(dd.hash, fsf, info)){
            /* make path for downloaded or downloading files */
            //QFileInfo qinfo;
            std::string path;
            path = info.path.substr(0,info.path.length()-info.fname.length());
            qdd.insert("fullpath",QString::fromStdString(path));

            /* open folder with a suitable application */
            /*qinfo.setFile(QString::fromUtf8(path.c_str()));
            if (qinfo.exists() && qinfo.isDir()) {
                if (!RsUrlHandler::openUrl(QUrl::fromLocalFile(qinfo.absoluteFilePath()))) {
                    std::cerr << "openFolderSearch(): can't open folder " << path << std::endl;
                }
            }*/
        } else {
            std::cout << "file details failed\n";
        }



        qResults.push_back(qdd);
        //finalResults.push_back(dd);

    }

    /* abstraction to allow reusee of tree rendering code */
    //resultsToTree(keywords,req_id, finalResults);
    return qResults;
}

void WebBridgeRS::pushMsgToJs(QVariantMap message)
{
    emit msgPush(message);
}

QString WebBridgeRS::getOwnId()
{
    return QString(rsPeers->getOwnId().c_str());
}
QString WebBridgeRS::getDownloadDirectory()
{
    return QString(rsFiles->getDownloadDirectory().c_str());
}
