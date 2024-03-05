#ifndef TOPTUIO_H
#define TOPTUIO_H

#include "TuioServer.h"
#include "TuioCursor.h"
#include "osc/OscTypes.h"
#include <list>
#include <math.h>

#include "FlashSender.h"
#include "TcpSender.h"
#include "WebSockSender.h"

#ifdef __WIN32__
#include <windows.h>
#endif

using namespace TUIO;

class TopTUIO
{
public:
    TopTUIO();
    ~TopTUIO();
    std::list<TuioCursor*> stickyCursorList;
    std::list<TuioCursor*> jointCursorList;
    std::list<TuioCursor*> activeCursorList;
    void mousePressed(float x, float y);
    void mouseReleased(float x, float y);
    void mouseDragged(float x, float y);
    //Chamar este método periodicamente
    void updateSessionTime();
    void removeAllCursors();
private:
    TuioServer *tuioServer;
    TuioTime frameTime;
};

#endif // TOPTUIO_H
