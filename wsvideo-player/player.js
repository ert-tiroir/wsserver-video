
/**********************************************************************************/
/* player.js                                                                      */
/*                                                                                */
/* This file contains the video player for the web socket based live stream       */
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

class WSVideo {
    /**
     * Create a WSVideo object and start listening to video data
     * 
     * @param {WebSocket} websocket the web socket used for the video flux
     * @param {string} sourceName the name of the web socket flux in which the video data will be. 
     * @param {string} codec the video codec so that the browser can decode the flux
     * @param {number} duration the duration in seconds of the video
     */
    constructor (websocket, sourceName, codec, duration = 4294967296) {
        this.sourceName = sourceName;
        this.prefix     = `video: ${sourceName}: `;

        websocket.addEventListener("message", (data) => this.handle(data.data))

        this.onsourceopen = []

        this.mediaSource = new MediaSource();
        this.mediaSource.addEventListener("sourceopen", () => {
            this.mediaSource.duration = duration;
            this.sourceBuffer = this.mediaSource.addSourceBuffer(codec);
            
            for (let listener of this.onsourceopen) listener();
            this.onsourceopen = []
        })

        this.element = undefined;
    }

    /**
     * Calls the passed function when the media source has been opened or instantly
     * if it has already been opened
     * 
     * @param {Function} callback the callback when the source has been created
     */
    addSourceOpenListener (callback) {
        if (this.sourceBuffer) callback();
        else this.onsourceopen.push(callback);
    }

    /**
     * Attach a video object to the player
     * 
     * @param {HTMLElement} element the element that will contain the video
     */
    attach (element) {
        if (this.element !== undefined)
            throw 'Only one video can receive a WSVideo object'

        this.element = element;

        element.src = URL.createObjectURL(this.mediaSource);
    }

    /**
     * @param {Blob} blob the data the web socket sent
     */
    async handle (blob) {
        if (!(blob instanceof Blob)) return ;

        const text = await blob.text();
        if (!text.startsWith(this.prefix)) return ;
        
        const buffer = (await blob.arrayBuffer()).slice(this.prefix.length);
        
        this.addSourceOpenListener(() => {
            this.sourceBuffer.appendBuffer(buffer)
            
            this.element.play();
        })
    }
}
