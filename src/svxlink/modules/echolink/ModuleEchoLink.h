/**
@file	 ModuleEchoLink.h
@brief   A module that provides EchoLink connection possibility
@author  Tobias Blomberg / SM0SVX
@date	 2004-03-07

\verbatim
A module (plugin) for the multi purpose tranciever frontend system.
Copyright (C) 2004  Tobias Blomberg / SM0SVX

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
\endverbatim
*/


#ifndef MODULE_ECHOLINK_INCLUDED
#define MODULE_ECHOLINK_INCLUDED


/****************************************************************************
 *
 * System Includes
 *
 ****************************************************************************/

#include <string>
#include <vector>



/****************************************************************************
 *
 * Project Includes
 *
 ****************************************************************************/

#include <version/SVXLINK.h>
#include <Module.h>
#include <EchoLinkQso.h>
#include <EchoLinkStationData.h>


/****************************************************************************
 *
 * Local Includes
 *
 ****************************************************************************/

#include "QsoImpl.h"


/****************************************************************************
 *
 * Forward declarations
 *
 ****************************************************************************/

namespace Async
{
  class Timer;
};
namespace EchoLink
{
  class Directory;
  class StationData;
};


/****************************************************************************
 *
 * Namespace
 *
 ****************************************************************************/

//namespace MyNameSpace
//{


/****************************************************************************
 *
 * Forward declarations of classes inside of the declared namespace
 *
 ****************************************************************************/

class MsgHandler;
class AudioPacer;
class QsoImpl;
  

/****************************************************************************
 *
 * Defines & typedefs
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Exported Global Variables
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Class definitions
 *
 ****************************************************************************/

/**
@brief	A module for providing EchoLink connections
@author Tobias Blomberg
@date   2004-03-07
*/
class ModuleEchoLink : public Module
{
  public:
    ModuleEchoLink(void *dl_handle, Logic *logic, const std::string& cfg_name);
    ~ModuleEchoLink(void);
    bool initialize(void);
    const char *compiledForVersion(void) const { return SVXLINK_VERSION; }

    
  private:
    typedef enum
    {
      STATE_NORMAL,
      STATE_CONNECT_BY_CALL
    } State;
    typedef std::vector<EchoLink::StationData> StnList;

    EchoLink::Directory *dir;
    Async::Timer      	*dir_refresh_timer;
    std::string       	mycall;
    std::string       	location;
    std::string       	sysop_name;
    std::string       	description;
    std::string       	allow_ip;
    bool      	      	remote_activation;
    int       	      	pending_connect_id;
    std::string       	last_message;
    QsoImpl		*outgoing_con_pending;
    std::list<QsoImpl*>	qsos;
    unsigned       	max_connections;
    unsigned       	max_qsos;
    QsoImpl   	      	*talker;
    bool      	      	squelch_is_open;
    State		state;
    StnList		cbc_stns;
    Async::Timer	*cbc_timer;

    void moduleCleanup(void);
    void activateInit(void);
    void deactivateCleanup(void);
    void dtmfDigitReceived(char digit);
    void dtmfCmdReceived(const std::string& cmd);
    void squelchOpen(bool is_open);
    int audioFromRx(float *samples, int count);
    void allMsgsWritten(void);
    void reportState(void);

    void onStatusChanged(EchoLink::StationData::Status status);
    void onStationListUpdated(void);
    void onError(const std::string& msg);
    void onIncomingConnection(const Async::IpAddress& ip,
      	    const std::string& callsign, const std::string& name);
    void onStateChange(QsoImpl *qso, EchoLink::Qso::State qso_state);
    void onChatMsgReceived(QsoImpl *qso, const std::string& msg);
    void onIsReceiving(bool is_receiving, QsoImpl *qso);
    void onDestroyMe(QsoImpl *qso);

    void getDirectoryList(Async::Timer *timer=0);

    void createOutgoingConnection(const EchoLink::StationData &station);
    int audioFromRemote(float *samples, int count, QsoImpl *qso);
    void audioFromRemoteRaw(QsoImpl::GsmVoicePacket *packet, QsoImpl *qso);
    QsoImpl *findFirstTalker(void) const;
    void broadcastTalkerStatus(void);
    void updateDescription(void);
    void updateEventVariables(void);
    void connectByCallsign(std::string cmd);
    void handleConnectByCall(const std::string& cmd);
    void cbcTimeout(Async::Timer *t);
    int numConnectedStations(void);

};  /* class ModuleEchoLink */


//} /* namespace */

#endif /* MODULE_ECHOLINK_INCLUDED */



/*
 * This file has not been truncated
 */
