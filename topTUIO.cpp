#include "topTUIO.h"

TopTUIO::TopTUIO()
{
    TuioServer *server = NULL;
    server = new TuioServer(); // default is UDP port 3333 on localhost

    // add an additional TUIO/TCP sender
    OscSender *tcp_sender = NULL;
    try { tcp_sender = new TcpSender(3333); }
    catch (std::exception e) { tcp_sender = NULL; }
    if (tcp_sender) server->addOscSender(tcp_sender);


    // add an additional TUIO/WEB sender
    OscSender *web_sender = NULL;
    try { web_sender = new WebSockSender(8080); }
    catch (std::exception e) { web_sender = NULL; }
    if (web_sender) server->addOscSender(web_sender);

    // add an additional TUIO/FLC sender
    OscSender *flash_sender = NULL;
    try { flash_sender = new FlashSender(); }
    catch (std::exception e) { flash_sender = NULL; }
    if (flash_sender) server->addOscSender(flash_sender);

    TuioTime::initSession();
    frameTime = TuioTime::getSessionTime();

    tuioServer = server;
    tuioServer->setSourceName("TableTop");
    tuioServer->enableObjectProfile(false);
    tuioServer->enableBlobProfile(false);
    tuioServer->setVerbose(false);
}

TopTUIO::~TopTUIO()
{
    delete tuioServer;
}

void TopTUIO::mousePressed(float x, float y)
{
//    TuioCursor *match = NULL;
//    float distance  = 0.01f;
//    float width = 640;
//    for (std::list<TuioCursor*>::iterator iter = stickyCursorList.begin(); iter!=stickyCursorList.end(); iter++)
//    {
//        TuioCursor *tcur = (*iter);
//        float test = tcur->getDistance(x,y);
//        if ((test < distance) && (test < 8.0f/width)) {
//            distance = test;
//            match = tcur;
//        }
//    }
//    if (match==NULL) {
        TuioCursor *cursor = tuioServer->addTuioCursor(x,y);
        activeCursorList.push_back(cursor);
//    } else activeCursorList.push_back(match);
}

void TopTUIO::mouseReleased(float x, float y)
{
    TuioCursor *cursor = NULL;
    float distance  = 0.01f;
//    for (std::list<TuioCursor*>::iterator iter = stickyCursorList.begin(); iter!=stickyCursorList.end(); iter++) {
//        TuioCursor *tcur = (*iter);
//        float test = tcur->getDistance(x,y);
//        if (test<distance) {
//            distance = test;
//            cursor = tcur;
//        }
//    }

//    if (cursor!=NULL) {
//        activeCursorList.remove(cursor);
//        return;
//    }

//    distance = 0.01f;
    for (std::list<TuioCursor*>::iterator iter = activeCursorList.begin(); iter!=activeCursorList.end(); iter++) {
        TuioCursor *tcur = (*iter);
        float test = tcur->getDistance(x,y);
        if (test<distance) {
            distance = test;
            cursor = tcur;
        }
    }

    if (cursor!=NULL) {
        activeCursorList.remove(cursor);
        tuioServer->removeTuioCursor(cursor);
    }
}

void TopTUIO::mouseDragged(float x, float y)
{
    TuioCursor *cursor = NULL;
    float distance  = 640;//Somente para testes
    for (std::list<TuioCursor*>::iterator iter = activeCursorList.begin(); iter!=activeCursorList.end(); iter++) {
        TuioCursor *tcur = (*iter);
        float test = tcur->getDistance(x,y);
        if (test<distance) {
            distance = test;
            cursor = tcur;
        }
    }
    if (cursor==NULL) return;
    if (cursor->getTuioTime()==frameTime) return;

    std::list<TuioCursor*>::iterator joint = std::find( jointCursorList.begin(), jointCursorList.end(), cursor );
    if( joint != jointCursorList.end() ) {
        float dx = x-cursor->getX();
        float dy = y-cursor->getY();
        for (std::list<TuioCursor*>::iterator iter = jointCursorList.begin(); iter!=jointCursorList.end(); iter++) {
            TuioCursor *jointCursor = (*iter);
            tuioServer->updateTuioCursor(jointCursor,jointCursor->getX()+dx,jointCursor->getY()+dy);
        }
    } else tuioServer->updateTuioCursor(cursor,x,y);
}

void TopTUIO::removeAllCursors()
{
    for (std::list<TuioCursor*>::iterator iter = activeCursorList.begin(); iter!=activeCursorList.end(); iter++) {
        TuioCursor *cursor = (*iter);
        tuioServer->removeTuioCursor(cursor);
    }
    jointCursorList.clear();
    activeCursorList.clear();
}

void TopTUIO::updateSessionTime() {
    tuioServer->stopUntouchedMovingCursors();
    tuioServer->commitFrame();
    // simulate 50Hz compensating the previous processing time
    int delay = 20 - (TuioTime::getSessionTime().getTotalMilliseconds() - frameTime.getTotalMilliseconds());
    if (delay>0) Sleep(delay);
    frameTime = TuioTime::getSessionTime();
    tuioServer->initFrame(frameTime);
    //    tuioServer->stopUntouchedMovingCursors();
    //    tuioServer->commitFrame();

}
