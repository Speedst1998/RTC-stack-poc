

#ifndef RTPRTCP_STREAM_H
#define RTPRTCP_STREAM_H

#include <cstdint>
#include <vector>
#include <string>
#include <mutex>

#include <RtpRtcp/Extension.h>
#include <RtpRtcp/RtpPacket.h>
#include <RtpRtcp/Frame.h>
#include <RtpRtcp/PacketizedData.h>

namespace rtp {
    /**
     *
     */
    class Stream {
    public:
        enum MediaType : uint8_t {
            ANY,
            AUDIO,
            VIDEO,
            DATA
        };

        struct Config {
            struct Rtp {
                // Sender SSRC.
                uint32_t ssrc = 0;

                // RTP header extensions used for the sent stream.
                std::vector<Extension> extensions;

                // RTCP CNAME, see RFC 3550.
                std::string c_name;
            } rtp;

            MediaType type;
        };

        Stream(const Config &config);

        virtual ~Stream();

        /**
         * Reports if at least one packet was sent to ReportPacketizedData
         * @return true if at least one packet was reported
         */
        bool SeenOnePacket() const { return processedOnePacket; }

        /**
         * RTP Timestamp of the last keyframe ReportEncodedFrame saw
         *
         * @return RTP Timestamp of the last keyframe
         */
        uint32_t LastKeyframeRtpTimestamp() const { return lastKeyframeRtpTimestamp; }

        /**
         * RTP Timestamp of the last reported packetized data
         *
         * @return RTP timestamp of the last ReportPacketizedData call
         */
        uint32_t LastRtpTimestamp() const { return lastRtpTimestamp; }

        uint8_t LastRtpPayloadType() const { return lastRtpPayloadType; }

    protected:
        void ReportPacketizedData(const PacketizedData &data);

        void ReportEncodedFrame(const EncodedFrame &frame);

    private:
        std::mutex statsCriticalLock;

        std::chrono::system_clock::time_point lastPacketizedReportTime;
        std::chrono::system_clock::time_point lastKeyFrameLocalTime;
        bool processedOnePacket = false;
        uint8_t lastRtpPayloadType = 0;
        uint32_t lastKeyframeRtpTimestamp = 0;
        uint32_t lastRtpTimestamp = 0;
        size_t lastPayloadLength = 0;
    };
}

#endif
