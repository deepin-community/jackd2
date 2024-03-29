/*
Copyright (C) 2003-2007 Jussi Laako <jussi@sonarnerd.net>
Copyright (C) 2008 Grame & RTL 2008

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
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#ifndef __JackOSSDriver__
#define __JackOSSDriver__

#include "JackAudioDriver.h"

namespace Jack
{

typedef jack_default_audio_sample_t jack_sample_t;

#define OSS_DRIVER_DEF_DEV "/dev/dsp"
#define OSS_DRIVER_DEF_FS 48000
#define OSS_DRIVER_DEF_BLKSIZE 1024
#define OSS_DRIVER_DEF_NPERIODS 1
#define OSS_DRIVER_DEF_BITS 16
#define OSS_DRIVER_DEF_INS 2
#define OSS_DRIVER_DEF_OUTS 2

/*!
\brief The OSS driver.
*/

class JackOSSDriver : public JackAudioDriver
{
    private:

        int fInFD;
        int fOutFD;

        int fBits;
        int fNperiods;
        bool fCapture;
        bool fPlayback;
        bool fExcl;
        bool fIgnoreHW;
        jack_nframes_t fExtraCaptureLatency;
        jack_nframes_t fExtraPlaybackLatency;

        unsigned int fInSampleSize;
        unsigned int fOutSampleSize;

        unsigned int fInputBufferSize;
        unsigned int fOutputBufferSize;

        void* fInputBuffer;
        void* fOutputBuffer;

        jack_nframes_t fInBlockSize;
        jack_nframes_t fOutBlockSize;
        jack_nframes_t fInMeanStep;
        jack_nframes_t fOutMeanStep;
        jack_nframes_t fOSSInBuffer;
        jack_nframes_t fOSSOutBuffer;

        jack_time_t fOSSReadSync;
        long long fOSSReadOffset;
        jack_time_t fOSSWriteSync;
        long long fOSSWriteOffset;

        // Buffer balance and sync correction
        long long fBufferBalance;
        bool fForceBalancing;
        bool fForceSync;

        int OpenInput();
        int OpenOutput();
        int OpenAux();
        void CloseAux();
        void DisplayDeviceInfo();
        int ProbeInBlockSize();
        int ProbeOutBlockSize();
        int Discard(jack_nframes_t frames);
        int WriteSilence(jack_nframes_t frames);
        int WaitAndSync();

    public:

        JackOSSDriver(const char* name, const char* alias, JackLockedEngine* engine, JackSynchro* table)
                : JackAudioDriver(name, alias, engine, table),
                fInFD(-1), fOutFD(-1), fBits(0),
                fNperiods(0), fCapture(false), fPlayback(false), fExcl(false), fIgnoreHW(true),
                fExtraCaptureLatency(0), fExtraPlaybackLatency(0),
                fInSampleSize(0), fOutSampleSize(0),
                fInputBufferSize(0), fOutputBufferSize(0),
                fInputBuffer(NULL), fOutputBuffer(NULL),
                fInBlockSize(1), fOutBlockSize(1),
                fInMeanStep(0), fOutMeanStep(0),
                fOSSInBuffer(0), fOSSOutBuffer(0),
                fOSSReadSync(0), fOSSReadOffset(0), fOSSWriteSync(0), fOSSWriteOffset(0),
                fBufferBalance(0), fForceBalancing(false), fForceSync(false)
        {}

        virtual ~JackOSSDriver()
        {}

        int Open(jack_nframes_t frames_per_cycle,
                 int user_nperiods,
                 jack_nframes_t rate,
                 bool capturing,
                 bool playing,
                 int chan_in,
                 int chan_out,
                 bool vmix,
                 bool monitor,
                 const char* capture_driver_name,
                 const char* playback_driver_name,
                 jack_nframes_t capture_latency,
                 jack_nframes_t playback_latency,
                 int bits,
                 bool ignorehwbuf);

        int Close();

        int Read();
        int Write();

        // BufferSize can be changed
        bool IsFixedBufferSize()
        {
            return false;
        }

        int SetBufferSize(jack_nframes_t buffer_size);

};

} // end of namespace

#endif
