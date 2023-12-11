
/**********************************************************************************/
/* wsvideo/broadcaster.cpp                                                        */
/*                                                                                */
/* This file contains the implementation of the web socket broadcaster            */
/**********************************************************************************/
/*                  This file is part of the ERT-Tiroir project                   */
/*                      github.com/ert-tiroir/wsserver-video                      */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2023 ert-tiroir                                                  */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#include <wsvideo/broadcaster.h>

/**
 * Create a new broadcaster
 * 
 * @param server the web socket server to broadcast on
 * @param flux_name the name of the video flux
 */
VideoBroadcaster::VideoBroadcaster (WebSocketServer* server, string flux_name) {
    this->flux_name = flux_name;
    this->server    = server;
}

/**
 * Tick the broadcast, sends necessary packets that were missed by clients
 * that were late.
 */
void VideoBroadcaster::tick () {
    sendToNewClients();
}

/**
 * Send all the packets to the new clients
 */
void VideoBroadcaster::sendToNewClients () {
    /* Send all the packets to the new clients */
    for (; this->last_joined_count < server->clientCount(); this->last_joined_count ++)
        this->sendAllPackets( this->last_joined_count );
}

/**
 * Create a new packet, and send it to the old clients, new clients will receive it
 * during the tick procedure
 */
void VideoBroadcaster::sendPacket(string buffer) {
    // append flux name
    string tbuffer = "video: " + flux_name + ": " + buffer;

    // create packet
    VideoPacket* packet = new VideoPacket(tbuffer);

    packets.push_back(packet);

    // send it to all the clients that are already connected
    int iP = packets.size() - 1;
    for (int iC = 0; iC < this->last_joined_count; iC ++)
        sendPacket(iP, iC);
}

/**
 * Send all the packets to a client
 * 
 * @param idClient the id of the client that will receive all the packets
 */
void VideoBroadcaster::sendAllPackets (int idClient) {
    for (int iP = 0; iP < packets.size(); iP ++)
        sendPacket(iP, idClient);
}

/**
 * Send a packet to a client
 * 
 * @param idPacket the id of the packet
 * @param idClient the id of the client
 */
void VideoBroadcaster::sendPacket (int idPacket, int idClient) {
    WebSocketClient* client = server->getClient(idClient);

    string buffer = packets[idPacket]->getBuffer();
    client->send( buffer, 2 );
}
