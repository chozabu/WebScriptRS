#include "webbridgers.h"

#include "p3JsonRS.h"
#include "retroshare/rsposted.h"
#include <retroshare/rsexpr.h>
#include "gui/RetroShareLink.h"

#include "retroshare/rsmsgs.h"
//#include "retroshare/rspeers.h"
//#include "retroshare/rsnotify.h"
#include "retroshare/rsforums.h"
#include "retroshare/rsdisc.h"
#include "retroshare/rschannels.h"
//#include "retroshare/rsnotify.h"
#include "gui/notifyqt.h"
#include "gui/common/AvatarDefs.h"
#include "filedownloader.h"
#include <QFile>
#include <QDir>
#include <QTextStream>

#include "gui/MainWindow.h"
//#include "gui/PluginsPage.h"


#include "retroshare/rsplugin.h"
//#include "gui/ChatLobbyWidget.h"
//#include "gui/ForumsDialog.h"

#include <QString>
#include <QWidget>
WebBridgeRS::WebBridgeRS(QObject *parent) :
    QJsonRpcService(parent)
{
    this->setObjectName("bridge");
    //rsFiles->getSharedDirectories();
    //embpyqt = new EmbeddedPyQt();
    //todo, perhaps - deeper intergration with retroshare-gui
    //MainWindow *pMainWindow = MainWindow::getInstance();
    //pMainWindow->forumsDialog->
    //pMainWindow->chatLobbyDialog->
    //pMainWindow->//blast - adding actions/plugins is private...
    connect(NotifyQt::getInstance(), SIGNAL(downloadComplete(QString)), this, SLOT(onDownloadComplete(QString)));
    connect(NotifyQt::getInstance(), SIGNAL(gotTurtleSearchResult(qulonglong,FileDetail)), this, SLOT(gotTurtleSearchResult(qulonglong,FileDetail)));

}

void WebBridgeRS::runPython(QString text){
    embpyqt->execute(text, true);
}
void WebBridgeRS::runPythonFile(QString fileName){
    QString text = "execfile('"+fileName+"')";
    embpyqt->execute(text, false);
}

QPixmap WebBridgeRS::getAvatarDetails(QString ssl_id){
    if (!avatars.contains(ssl_id)){
        QPixmap avatar;
        AvatarDefs::getAvatarFromGpgId(rsPeers->getGPGId(ssl_id.toStdString()), avatar);
        avatars.insert(ssl_id,avatar);
        return avatar;
    }
    return avatars[ssl_id];
}

//const std::string &ssl_or_gpg_id, RsPeerDetails &d
QVariantMap WebBridgeRS::getPeerDetails(QString ssl_id){
    RsPeerDetails d;
    rsPeers->getPeerDetails(ssl_id.toStdString(),d);
    QVariantMap qdd;
    qdd.insert("trustLvl",d.trustLvl);
    qdd.insert("connectPeriod",d.connectPeriod);
    qdd.insert("connectState",d.connectState);
    qdd.insert("connectStateString",QString::fromStdString(d.connectStateString));
    qdd.insert("hasSignedMe",d.hasSignedMe);
    //qdd.insert("gpgSigners",d.gpgSigners);
    qdd.insert("extAddr",QString::fromStdString(d.extAddr));
    qdd.insert("extPort",d.extPort);
    qdd.insert("email",QString::fromStdString(d.email));
	qdd.insert("id",QString::fromStdString(d.id));
	qdd.insert("gpg_id",QString::fromStdString(d.gpg_id));
    //qdd.insert("ipAddressList",d.ipAddressList);
    qdd.insert("localAddr",QString::fromStdString(d.localAddr));
    qdd.insert("localPort",d.localPort);
    qdd.insert("location",QString::fromStdString(d.location));
    qdd.insert("name",QString::fromStdString(d.name));
    qdd.insert("validLvl",d.validLvl);
    qdd.insert("visState",d.visState);
    return qdd;
}
QString WebBridgeRS::getPeerName(QString ssl_id){
    std::list<std::string> ssl_ids;
    return QString(rsPeers->getPeerName(ssl_id.toStdString()).c_str());
}
QStringList strListToQStrList(std::list<std::string> in){

    QStringList qResults;
    std::list<std::string>::iterator it;
    for(it = in.begin(); it != in.end(); it++) {
        std::string dd;
        dd = *it;
        qResults.append(dd.c_str());
    }
    return qResults;
}

QStringList WebBridgeRS::getFriendList(){
    std::list<std::string> ssl_ids;
    rsPeers->getFriendList(ssl_ids);
    return strListToQStrList(ssl_ids);
}
QStringList WebBridgeRS::getGPGAllList(){
    std::list<std::string> gpg_ids;
    rsPeers->getGPGAllList(gpg_ids);
    return strListToQStrList(gpg_ids);
}
QString WebBridgeRS::getGPGId(QString inId){
    return QString(rsPeers->getGPGId(inId.toStdString()).c_str());
}

QStringList WebBridgeRS::getOnlineList(){
    std::list<std::string> ssl_ids;
    rsPeers->getOnlineList(ssl_ids);
    return strListToQStrList(ssl_ids);
}
QStringList WebBridgeRS::getDiscGPGFriends(QString gpg_id){
	std::list<std::string> friendList;
	rsDisc->getDiscGPGFriends(gpg_id.toStdString(), friendList);
    return strListToQStrList(friendList);
}
void WebBridgeRS::newTabAt(QString url){
    emit newTabUrl(url);
}

#ifdef BRIDGEGXS
QVariantList WebBridgeRS::getPostedList(){
    u_int32_t token;
    std::list<RsGxsGroupId> groupList;
    rsPosted->getGroupList(token, groupList);

    QVariantList qResults;
    std::list<RsGxsGroupId>::iterator it;
    for(it = groupList.begin(); it != groupList.end(); it++) {
        RsGxsGroupId dd;
        dd = *it;
        std::cout << dd << std::endl;
    }

    return qResults;
}
#endif

QVariantMap WebBridgeRS::fileDetails(QString qhash)
{
    QVariantMap qdd;

    FileInfo finfo;

    /* look up path */
    if (rsFiles->FileDetails(qhash.toStdString(), RS_FILE_HINTS_EXTRA | RS_FILE_HINTS_DOWNLOAD, finfo)){
        qdd.insert("status","found");
        qdd.insert("path",QString::fromStdString(finfo.path));
        qdd.insert("hash",QString::fromStdString(finfo.hash));
        qdd.insert("fname",QString::fromStdString(finfo.fname));
        qdd.insert("size",QString::number(finfo.size));
        qdd.insert("avail",QString::number(finfo.avail));
        qdd.insert("downloadStatus",QString::number(finfo.downloadStatus));
        qdd.insert("tfRate",QString::number(finfo.tfRate));
        qdd.insert("transfered",QString::number(finfo.transfered));
    } else {
        qdd.insert("status","notfound");
    }
    return qdd;
}

void WebBridgeRS::gotTurtleSearchResult(qulonglong search_id,FileDetail file)
{
    QVariantMap qdd;

    std::cerr << "\n\nWSRS: turtle result: " << file.name.c_str() << "\n\n";
    FileInfo fi;
    if (rsFiles->alreadyHaveFile(file.hash, fi))
        qdd.insert("status","downloaded");
    else
        qdd.insert("status","notdownloaded");
    qdd.insert("path",QString::fromStdString(file.path));
    qdd.insert("hash",QString::fromStdString(file.hash));
    qdd.insert("age",QString::number(file.age));
    qdd.insert("id",QString::fromStdString(file.id));
    qdd.insert("name",QString::fromStdString(file.name));
    qdd.insert("rank",QString::number(file.rank));
    qdd.insert("size",QString::number(file.size));

    qdd.insert("search_id",QString::number(search_id));
    emit rsTurtleResult(qdd);
}
void WebBridgeRS::onDownloadComplete(QString hash)
{
    QVariantMap qdd;

    FileInfo fi;

    /* look up path */
    if (rsFiles->alreadyHaveFile(hash.toStdString(), fi)){
        qdd.insert("status","downloaded");
        qdd.insert("path",QString::fromStdString(fi.path));
        qdd.insert("hash",QString::fromStdString(fi.hash));
        qdd.insert("fname",QString::fromStdString(fi.fname));
        qdd.insert("size",QString::number(fi.size));
        emit rsDownloaded(qdd);
    }
}

/*QVariantList WebBridgeRS::getPublicChatQueue()
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
}*/

void WebBridgeRS::broadcastToRS(QString msg)
{
    rsMsgs->sendPublicChat(msg.toStdWString());
}

void WebBridgeRS::writeString(QString fname, QString data){

    QString fpath = getDownloadDirectory();

    fpath.append("/");
    fpath.append(fname);
    std::cout << fpath.toStdString() << std::endl;
    QFile outfile(fpath);

	if (outfile.open(QFile::WriteOnly | QFile::Truncate)) {
	    QTextStream out(&outfile);
	    out << data;
	}

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


QVariantMap WebBridgeRS::downloadFile(QString qname, QString qhash, int qsize){

    QVariantMap qdd;

    FileInfo fi;

    /* look up path */
    if (rsFiles->alreadyHaveFile(qhash.toStdString(), fi)){
        qdd.insert("status","downloaded");
        qdd.insert("path",QString::fromStdString(fi.path));
        qdd.insert("hash",QString::fromStdString(fi.hash));
        qdd.insert("fname",QString::fromStdString(fi.fname));
        qdd.insert("size",QString::number(fi.size));
        return qdd;
    }

    // Get a list of available direct sources, in case the file is browsable only.
    //
    FileInfo finfo ;
    rsFiles->FileDetails(qhash.toStdString(), RS_FILE_HINTS_REMOTE, finfo) ;

    std::list<std::string> srcIds;
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
        qdd.insert("status","starting");
    } else {
        //fileExist.append(link.name());
        //return false;
        qdd.insert("status","downloading");
    }
    return qdd;
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

        if (dd.subscribeFlags & RS_DISTRIB_ADMIN) {
            qdd.insert("forumRelationship","ADMIN");
        } else if (dd.subscribeFlags & RS_DISTRIB_SUBSCRIBED) {
            /* subscribed forum */
            qdd.insert("forumRelationship","SUBSCRIBED");
        } else {
            /* rate the others by popularity */
            qdd.insert("forumRelationship","OTHER");
        }

        qResults.append(qdd);
    }

    return qResults;
}

QVariantList WebBridgeRS::getForumThreadList(QString flid){
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

QVariantList WebBridgeRS::getForumThreadMsgList(QString flid, QString msgid){
    std::list<ThreadInfoSummary> msgs;
    rsForums->getForumThreadMsgList(flid.toStdString(),msgid.toStdString(), msgs);

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

QVariantMap WebBridgeRS::getForumMessage(QString flid, QString msgid){
    ForumMsgInfo msg;
    rsForums->getForumMessage(flid.toStdString(),msgid.toStdString(), msg);

        QVariantMap qdd;
        qdd.insert("childTS",QString::number(msg.childTS));
        qdd.insert("srcId",QString::fromStdString(msg.srcId));
        qdd.insert("status",QString::number(msg.status));
        qdd.insert("forumId",QString::fromStdString(msg.forumId));
        qdd.insert("msg",QString::fromStdWString(msg.msg));
        qdd.insert("msgflags",QString::number(msg.msgflags));
        qdd.insert("msgId",QString::fromStdString(msg.msgId));
        qdd.insert("parentId",QString::fromStdString(msg.parentId));
        qdd.insert("threadId",QString::fromStdString(msg.threadId));
        qdd.insert("title",QString::fromStdWString(msg.title));
        qdd.insert("ts",QString::number(msg.ts));


    return qdd;
}

void WebBridgeRS::processLinks(QStringList urls){
    RetroShareLink::process(urls, RetroShareLink::TYPE_UNKNOWN, RSLINK_PROCESS_NOTIFY_ALL);
    //rsFiles->FileRequest()
}

QVariantMap WebBridgeRS::searchKeywords(const QString& keywords, QVariantMap searchOptions)
{
    std::cerr << "webscriptrs: keywords in " << keywords.toStdString().c_str() << std::endl;
    QVariantMap qReturn;
    QVariantList qResults;
    if (keywords.length() < 3){
        std::cerr << "webscriptrs: not searching due to keywords.length = " << keywords.length() << std::endl;
        qReturn.insert("message","search terms < 3 chars");
        qReturn.insert("status","fail");
        qReturn.insert("results",qResults);
        return qReturn;
    }

    QStringList qWords = keywords.split(" ", QString::SkipEmptyParts);
    std::list<std::string> words;
    QStringListIterator qWordsIter(qWords);
    while (qWordsIter.hasNext())
        words.push_back(qWordsIter.next().toUtf8().constData());

    int n = words.size() ;

    if (n < 1){
        std::cerr << "webscriptrs: searching due to words.size() = " << n << std::endl;
        qReturn.insert("message","search terms < 1 word");
        qReturn.insert("status","fail");
        qReturn.insert("results",qResults);
        return qReturn;
    }

    NameExpression exprs(ContainsAllStrings,words,true) ;
    LinearizedExpression lin_exp ;
    exprs.linearize(lin_exp) ;

    TurtleRequestId req_id ;

    if(searchOptions.value("turtle", false).toBool())
    {
        if(n==1)
            req_id = rsTurtle->turtleSearch(words.front()) ;
        else
            req_id = rsTurtle->turtleSearch(lin_exp) ;
        qReturn.insert("turtleID",req_id);
    }
    else
        req_id = ((((uint32_t)rand()) << 16)^0x1e2fd5e4) + (((uint32_t)rand())^0x1b19acfe) ; // generate a random 32 bits request id




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
    qReturn.insert("status","sucess");
    qReturn.insert("results",qResults);
    return qReturn;
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
    return QString::fromUtf8(rsFiles->getDownloadDirectory().c_str());
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
