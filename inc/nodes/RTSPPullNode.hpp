#include "node.hpp"
#include "RTSP/include/rtspclisvr/RTSPClient.h"

namespace ax
{
    class RTSPPullNode : public Node
    {
    private:
        RTSPClient m_client;

    public:
        RTSPPullNode():
            Node("RTSP_Pull")
        { }

        int Init()
        {
            AddOutputPort("frame_output");
            return AX_SUCCESS;
        }

        int Run()
        {
            
        }
    };
}
