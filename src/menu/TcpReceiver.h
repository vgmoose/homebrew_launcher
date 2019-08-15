#ifndef TCP_RECEIVER_H_
#define TCP_RECEIVER_H_

#include <vector>
#include <string>

#include "ProgressWindow.h"
#include "system/CThread.h"
#include "gui/sigslot.h"

class TcpReceiver : public GuiFrame, public CThread {
public:
    enum eLoadResults {
        SUCCESS = 0,
        INVALID_INPUT = -1,
        FILE_OPEN_FAILURE = -2,
        FILE_READ_ERROR = -3,
        NOT_ENOUGH_MEMORY = -4,
        FILE_SAVE_ERROR = -5,
    };

    TcpReceiver(int port);
    ~TcpReceiver();

    sigslot::signal2<GuiElement *, uint32_t> serverReceiveStart;
    sigslot::signal3<GuiElement *, uint32_t, int> serverReceiveFinished;

private:

    void executeThread();
    int loadToMemory(int32_t clientSocket, uint32_t ipAddress);

    bool exitRequested;
    int32_t serverPort;
    int32_t serverSocket;
    ProgressWindow progressWindow;
};


#endif
