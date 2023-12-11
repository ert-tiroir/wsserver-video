
/**********************************************************************************/
/* example.cpp                                                                    */
/*                                                                                */
/* This file contains an example on how to create a simple web socket live        */
/* broadcasting service from an mp4 file                                          */
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

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

#include <wsserver/server.h>
#include <wsvideo/broadcaster.h>

int main () {
    printf("Creating web socket server...\n");
    WebSocketServer server;
    server.init(5420);

    printf("Creating broadcast\n");
    VideoBroadcaster broadcast (&server, "flux.mp4");
    
    std::ifstream     file("test.mp4");
    std::stringstream buffer;
    buffer << file.rdbuf();

    printf("Writing initial packet...\n");
    broadcast.sendPacket(buffer.str());

    while (true) {
        string buffer; getline(cin, buffer);

        if (buffer == "listen") while (!server.listen()) continue ;
        if (buffer == "exit") break ;

        broadcast.tick();
    }

    server.close();
}
