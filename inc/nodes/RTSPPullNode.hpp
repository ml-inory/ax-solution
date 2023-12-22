#include "node.hpp"
#include "RTSP/include/rtspclisvr/RTSPClient.h"

#include "ax_vdec_api.h"

#include <string.h>

// 16字节对齐
#define ALIGN_16(x)     (x / 16 * 16)

static void frameHandlerFunc(void *arg, RTP_FRAME_TYPE frame_type, int64_t timestamp, unsigned char *buf, int len)
{
    ax::RTSPPullNode* node = (ax::RTSPPullNode*)arg;
    switch (frame_type)
    {
    case FRAME_TYPE_VIDEO:
        node->SendStream(buf, len);
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

        // 解码参数
        const int nVdecGrp = 0;
        const int nPicWidth = 1920;
        const int nPicHeight = 1080;

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

        int OpenVDEC()
        {
            int ret = AX_SUCCESS;

            // 初始化VDEC
            ret = AX_VDEC_Init(NULL);
            if (ret != AX_SUCCESS)
            {
                printf("AX_VDEC_Init failed! ret=0x%x\n", ret);
                return ret;
            }

            // 创建解码通道
            AX_VDEC_GRP_ATTR_T stGrpAttr;
            memset(&stGrpAttr, 0, sizeof(AX_VDEC_GRP_ATTR_T));
            stGrpAttr.enCodecType = PT_H264;
            stGrpAttr.enInputMode = AX_VDEC_INPUT_MODE_STREAM;
            stGrpAttr.enLinkMode = AX_UNLINK_MODE;
            stGrpAttr.enOutOrder = AX_VDEC_OUTPUT_ORDER_DISP;
            stGrpAttr.u32PicWidth = ALIGN_16(nPicWidth);
            stGrpAttr.u32PicHeight = ALIGN_16(nPicHeight);
            stGrpAttr.u32FrameHeight = ALIGN_16(nPicHeight);
            stGrpAttr.u32StreamBufSize = nPicHeight * nPicWidth * 3 / 2;
            stGrpAttr.enVdecVbSource = AX_POOL_SOURCE_COMMON;
            stGrpAttr.u32FrameBufCnt = 10;
            stGrpAttr.s32DestroyTimeout = 0;

            ret = AX_VDEC_CreateGrp(nVdecGrp, &stGrpAttr);
            if (ret != AX_SUCCESS)
            {
                printf("AX_VDEC_CreateGrp failed! ret=0x%x\n", ret);
                return ret;
            }

            // 开始接收码流
            AX_VDEC_RECV_PIC_PARAM_T stRecvParam;
            memset(&stRecvParam, 0, sizeof(AX_VDEC_RECV_PIC_PARAM_T));
            stRecvParam.s32RecvPicNum = -1;
            ret = AX_VDEC_StartRecvStream(nVdecGrp, &stRecvParam);
            if (ret != AX_SUCCESS)
            {
                printf("AX_VDEC_StartRecvStream failed! ret=0x%x\n", ret);
                return ret;
            }

            return AX_SUCCESS;
        }

        void CloseGVDEC()
        {
            int ret = AX_SUCCESS;

            // 销毁解码通道
            ret = AX_VDEC_DestroyGrp(nVdecGrp);
            if (ret != AX_SUCCESS)
            {
                printf("AX_VDEC_DestroyGrp failed! ret=0x%x\n", ret);
            }

            // 关闭VDEC
            ret = AX_VDEC_Deinit();
            if (ret != AX_SUCCESS)
            {
                printf("AX_VDEC_Deinit failed! ret=0x%x\n", ret);
            }
        }

        int SendStream(unsigned char* buf, int len)
        {
            int ret = AX_SUCCESS;
            AX_VDEC_STREAM_T stream;
            memset(&stream, 0, sizeof(AX_VDEC_STREAM_T));
            stream.pu8Addr = buf;
            stream.u32StreamPackLen = len;
            ret = AX_VDEC_SendStream(nVdecGrp, &stream, -1);
            if (ret != AX_SUCCESS)
            {
                printf("AX_VDEC_SendStream failed! ret=0x%x\n", ret);
                return ret;
            }
            return AX_SUCCESS;
        }

        int Run()
        {
            const char* node_name = m_name.c_str();
            printf("[%s]: %s start\n", node_name, node_name);

            auto frame_output_port = FindOutputPort("frame_output");

            // 打开VDEC
            if (OpenVDEC() != AX_SUCCESS)
            {
                printf("open vdec failed!\n");
                return AX_ERR_INIT_FAIL;
            }

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

            int ret = AX_SUCCESS;
            while (m_isRunning)
            {
                // 获取帧
                AX_VIDEO_FRAME_INFO_T stFrameInfo;
                ret = AX_VDEC_GetFrame(nVdecGrp, &stFrameInfo, -1);
                if (ret != AX_SUCCESS)
                {
                    printf("AX_VDEC_GetFrame failed! ret=0x%x\n", ret);
                    continue;
                }

                
                // stFrameInfo.stVFrame

                // 释放帧
                ret = AX_VDEC_ReleaseFrame(nVdecGrp, &stFrameInfo);
                if (ret != AX_SUCCESS)
                {
                    printf("AX_VDEC_ReleaseFrame failed! ret=0x%x\n", ret);
                    continue;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            printf("[%s]: Stop\n", node_name);
            m_client.closeURL();
            CloseGVDEC();

            return AX_SUCCESS;
        }
    };
}
