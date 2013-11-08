#include "webbridgers.h"

#include "p3JsonRS.h"
#include "retroshare/rsposted.h"
#include <retroshare/rsexpr.h>
#include "gui/RetroShareLink.h"

#include "retroshare/rsmsgs.h"
//#include "retroshare/rspeers.h"
//#include "retroshare/rsnotify.h"
#include "retroshare/rsforums.h"
#include "retroshare/rschannels.h"
//#include "retroshare/rsnotify.h"
#include "gui/notifyqt.h"
#include "filedownloader.h"
#include <QFile>
#include <QDir>

//rsPosted->;
//rsMsgs->getPublicChatQueue()

#include <QString>
WebBridgeRS::WebBridgeRS(QObject *parent) :
    QObject(parent)
{

    /*
    rsPeers->getOnlineList();
    rsPeers->getFriendList();
    */
    //rsFiles->FileRequest();
    //virtual bool FileRequest(const std::string& fname, const std::string& hash,
    //nt64_t size, const std::string& dest, TransferRequestFlags flags, const std::list<std::string>& srcIds) = 0;
    //rsFiles->getSharedDirectories();

    connect(NotifyQt::getInstance(), SIGNAL(downloadComplete(QString)), this, SLOT(onDownloadComplete(QString)));

}

void WebBridgeRS::onDownloadComplete(QString hash)
{
    emit rsDownloaded(hash);
}

QVariantList WebBridgeRS::getPublicChatQueue()
{
    std::list<ChatInfo> newchat;
    QVariantList qResults;
    if (!rsMsgs->getPublicChatQueue(newchat)){

        std::list<ChatInfo>::iterator it;
        for(it = newchat.begin(); it != newchat.end(); it++) {
            ChatInfo dd;
            dd = *it;
            QVariantMap qdd;
            qdd.insert("chatflags",dd.chatflags);
            qdd.insert("msg",QString::fromStdWString(dd.msg));
            qdd.insert("peer_nickname",QString::fromStdString(dd.peer_nickname));
            qdd.insert("recvTime",dd.recvTime);
            qdd.insert("rsid",QString::fromStdString(dd.rsid));
            qdd.insert("sendTime",dd.sendTime);
        }
    }
    return qResults;
}

void WebBridgeRS::broadcastToRS(QString msg)
{
    rsMsgs->sendPublicChat(msg.toStdWString());
}

void WebBridgeRS::downloadURL(QString url){
    QUrl imageUrl(url);
    FileDownloader* fileDownloader = new FileDownloader(imageUrl, this);

    connect(fileDownloader, SIGNAL(downloaded()), SLOT(onUrlDownloaded()));

}

void WebBridgeRS::onUrlDownloaded(){
    FileDownloader * fileDownloader = qobject_cast<FileDownloader *>(QObject::sender());

    //QDir::absoluteFilePath()
    QString fpath = getDownloadDirectory();

    fpath.append("/");
    QString urlstring(fileDownloader->url().toString());
    fpath.append(QFileInfo(urlstring).fileName());
    std::cout << fpath.toStdString() << std::endl;
    QFile file(fpath);
    file.open(QIODevice::WriteOnly);

    file.write(fileDownloader->downloadedData());

    // optional, as QFile destructor will already do it:
    file.close();

    std::cout << fileDownloader->downloadedData().data() << std::endl;
    emit urlDownloaded(fpath, urlstring);

}


void WebBridgeRS::downloadFile(QString qname, QString qhash, int qsize){
    std::list<std::string> srcIds;

    // Add the link built-in source. This is needed for EXTRA files, where the source is specified in the link.

    /*if(link.type() == TYPE_EXTRAFILE)
    {
    #ifdef DEBUG_RSLINK
        std::cerr << " RetroShareLink::process Adding built-in source " << link.SSLId().toStdString() << std::endl;
    #endif
        srcIds.push_back(link.SSLId().toStdString()) ;
    }*/

    // Get a list of available direct sources, in case the file is browsable only.
    //
    FileInfo finfo ;
    rsFiles->FileDetails(qhash.toStdString(), RS_FILE_HINTS_REMOTE, finfo) ;

    for(std::list<TransferInfo>::const_iterator it(finfo.peers.begin());it!=finfo.peers.end();++it)
    {
    #ifdef DEBUG_RSLINK
        std::cerr << "  adding peerid " << (*it).peerId << std::endl ;
    #endif
        srcIds.push_back((*it).peerId) ;
    }

    /*QString cleanname = link.name() ;
    static const QString bad_chars_str = "/\\\"*:?<>|" ;

    for(int i=0;i<cleanname.length();++i)
        for(int j=0;j<bad_chars_str.length();++j)
            if(cleanname[i] == bad_chars_str[j])
            {
                cleanname[i] = '_';
                flag |= RSLINK_PROCESS_NOTIFY_BAD_CHARS ;
            }
    */
    if (rsFiles->FileRequest(qname.toStdString(), qhash.toStdString(), qsize, "", RS_FILE_REQ_ANONYMOUS_ROUTING, srcIds)) {
        //fileAdded.append(link.name());
        //return true;
    } else {
        //fileExist.append(link.name());
        //return false;
    }
}

QVariantList WebBridgeRS::getChannelList(){
    std::list<ChannelInfo> chanList;
    rsChannels->getChannelList(chanList);

    QVariantList qResults;
    std::list<ChannelInfo>::iterator it;
    for(it = chanList.begin(); it != chanList.end(); it++) {
        ChannelInfo dd;
        dd = *it;
        QVariantMap qdd;
        qdd.insert("autoDownload",dd.autoDownload);
        qdd.insert("channelDesc",QString::fromStdWString(dd.channelDesc));
        qdd.insert("channelFlags",dd.channelFlags);
        qdd.insert("channelId",QString::fromStdString(dd.channelId));
        qdd.insert("channelName",QString::fromStdWString(dd.channelName));
        qdd.insert("destination_directory",QString::fromStdString(dd.destination_directory));
        qdd.insert("lastPost",QString::number(dd.lastPost));
        //qdd.insert("pngChanImage",QString::fromStdWString(dd.pngChanImage));
        //qdd.insert("pngImageLen",QString::fromStdWString(dd.pngImageLen));
        qdd.insert("pop",dd.pop);

        qResults.append(qdd);
    }

    return qResults;
}

QVariantList WebBridgeRS::getForums(){
    std::list<ForumInfo> forumList;
    rsForums->getForumList(forumList);

    QVariantList qResults;
    std::list<ForumInfo>::iterator it;
    for(it = forumList.begin(); it != forumList.end(); it++) {
        ForumInfo dd;
        dd = *it;
        QVariantMap qdd;
        qdd.insert("forumDesc",QString::fromStdWString(dd.forumDesc));
        qdd.insert("forumFlags",dd.forumFlags);
        qdd.insert("forumId",QString::fromStdString(dd.forumId));
        qdd.insert("forumName",QString::fromStdWString(dd.forumName));
        qdd.insert("lastPost",QString::number(dd.lastPost));
        qdd.insert("pop",dd.pop);
        qdd.insert("subscribeFlags",dd.subscribeFlags);

        qResults.append(qdd);
    }

    return qResults;
}

QVariantList WebBridgeRS::getForumInfo(QString flid){
    std::list<ThreadInfoSummary> msgs;
    rsForums->getForumThreadList(flid.toStdString(), msgs);

    QVariantList qResults;
    std::list<ThreadInfoSummary>::iterator it;
    for(it = msgs.begin(); it != msgs.end(); it++) {
        ThreadInfoSummary dd;
        dd = *it;
        QVariantMap qdd;
        qdd.insert("childTS",QString::number(dd.childTS));
        qdd.insert("count",QString::number(dd.count));
        qdd.insert("forumId",QString::fromStdString(dd.forumId));
        qdd.insert("msg",QString::fromStdWString(dd.msg));
        qdd.insert("msgflags",QString::number(dd.msgflags));
        qdd.insert("msgId",QString::fromStdString(dd.msgId));
        qdd.insert("parentId",QString::fromStdString(dd.parentId));
        qdd.insert("threadId",QString::fromStdString(dd.threadId));
        qdd.insert("title",QString::fromStdWString(dd.title));
        qdd.insert("ts",QString::number(dd.ts));

        qResults.append(qdd);
    }

    return qResults;
}
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

void WebBridgeRS::broadcastMessage(QString msg)
{
    QMapIterator<QString, QString> mi(p3service->compatablePeers);
    //std::cout << "boardcasting\n";
    while ( mi.hasNext() ) {
      mi.next();
      //std::cout << "to: " << mi.key().toStdString() << std::endl;
      //qDebug() << mi.key() << ":" << mi.value().surname() << mi.value().forename();
      p3service->msgPeer(mi.key().toStdString(),msg.toStdString());
  }
}
void WebBridgeRS::sendMessage(QString peer, QString msg)
{
    p3service->msgPeer(peer.toStdString(),msg.toStdString());
}


QVariantMap WebBridgeRS::getPeers()
{
    QVariantMap qm;
    QMapIterator<QString, QString> mi(p3service->compatablePeers);
    while ( mi.hasNext() ) {
      mi.next();
      qm.insert(mi.key(),mi.value());
    }
    //QVariantMap qm = p3service->compatablePeers;
    return qm;
}
