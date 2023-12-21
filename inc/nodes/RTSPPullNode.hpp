#include "node.hpp"
#include "RTSP/include/rtspclisvr/RTSPClient.h"

#include "ax_vdec_api.h"

#include <string.h>

static void frameHandlerFunc(void *arg, RTP_FRAME_TYPE frame_type, int64_t timestamp, unsigned char *buf, int len)
{
    switch (frame_type)
    {
    case FRAME_TYPE_VIDEO:

        break;
    case FRAME_TYPE_AUDIO:
        break;
    case FRAME_TYPE_ETC:
        break;
    default:
        break;
    }
}

namespace ax
{
    class RTSPPullNode : public Node
    {
    private:
        RTSPClient m_client;
        const char* m_rtspUrl;

    public:
        RTSPPullNode():
            Node("RTSP_Pull")
        { }

        int Init(const Json::Value& config)
        {
            AddOutputPort("frame_output");
            m_rtspUrl = config.asCString();
            return AX_SUCCESS;
        }

        int Run()
        {
            const char* node_name = m_name.c_str();
            printf("[%s]: %s start\n", node_name, node_name);

            auto frame_output_port = FindOutputPort("frame_output");

            // open client
            if (m_client.openURL(m_rtspUrl, 1) != 0)
            {
                printf("open url: %s falied!\n", m_rtspUrl);
                return AX_ERR_INIT_FAIL;
            }

            // play
            if (m_client.playURL(frameHandlerFunc, this, NULL, NULL) != 0)
            {
                printf("play url: %s falied!\n", m_rtspUrl);
                return AX_ERR_INIT_FAIL;
            }

            while (m_isRunning)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            printf("[%s]: Stop\n", node_name);
            m_client.closeURL();

            return AX_SUCCESS;
        }
    };
}
