#pragma once

#ifdef WIN32
#define _WINSOCK2API_   /* Prevent inclusion of winsock2.h in enet.h */
typedef struct fd_set fd_set;
typedef ptrdiff_t SOCKET;
#endif
#include <enet/enet.h>

typedef DWORD DPID;

//Direct play identifiers
#define DPPLAYERTYPE_GROUP          0x0000
#define DPPLAYERTYPE_PLAYER         0x0001
#define DPSYS_DESTROYPLAYERORGROUP  0x0005
#define DPSYS_SESSIONLOST           0x0031
#define DPSYS_HOST                  0x0101

struct DPPacket
{
    ULONG messageType;
    DWORD playerType;
    DPID  dpId;
};

struct SBNetworkPlayer
{
    unsigned int ID;
    ENetPeer* peer;
};

struct SBNetworkPeer
{
    unsigned int ID;
    ENetAddress address;
};

enum SBSessionEnum
{
    SBNETWORK_SESSION_DEFAULT,
    SBNETWORK_SESSION_SEARCHING,
    SBNETWORK_SESSION_MASTER,
    SBNETWORK_SESSION_CLIENT,
    SBNETWORK_SESSION_FINISHED
};

struct SBNetworkCreation
{
    SBStr sessionName;
    int maxPlayers;
    int flags;
};

struct SBSessionInfo
{
    unsigned int hostID;
    char sessionName[26];
    ENetAddress address;
};

class SBNetwork
{
public:
    SBNetwork(bool);

    int GetMessageCount();
    bool Connect(SBStr);
    bool Connect(SBStr, char*);
    void DisConnect();
    bool CreateSession(SBStr, SBNetworkCreation*);
    void CloseSession();
    unsigned int GetLocalPlayerID();
    SBList<SBStr>* GetConnectionList();
    SBList<SBStr>* GetSessionListAsync();
    bool StartGetSessionListAsync();
    SLONG GetProviderID(char*);
    bool IsEnumSessionFinished();
    bool IsInSession();
    bool Send(BUFFER<UBYTE>&, unsigned int, unsigned int, bool);
    bool Receive(UBYTE**, unsigned int&);
    bool JoinSession(SBStr, SBStr);
    SBList<SBNetworkPlayer>* GetAllPlayers();

private:
    SBSessionEnum mState;
    unsigned int mLocalID;
    SBList<SBStr> mConnections;
    SBList<SBStr> mSessions;
    SBList<SBSessionInfo> mSessionInfo;
    SBList<SBNetworkPlayer> mPlayers;

    unsigned int mSearchTime;
    ENetAddress mServer;
    ENetHost* mHost;
    ENetPeer* mMaster;
    ENetSocket mSocket;
    SBList<ENetPacket*> mPackets;
};
