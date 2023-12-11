
/**********************************************************************************/
/* wsvideo/broadcaster.h                                                          */
/*                                                                                */
/* This file contains the details of the broadcaster structure                    */
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

#include <string>
#include <vector>
#include <wsserver/server.h>

#include <wsvideo/packet.h>

using namespace std;

struct VideoBroadcaster {
private:
    vector<VideoPacket*> packets;

    int last_joined_count = 0;
    string flux_name;

    WebSocketServer* server;

    void sendPacket (int idPacket, int idClient);

    void sendAllPackets   (int idClient);
    void sendToNewClients ();
public:
    VideoBroadcaster (WebSocketServer* server, string flux_name);

    void tick ();

    void sendPacket (string packet);
};
