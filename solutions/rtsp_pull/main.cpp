#include "nodes/RTSPPullNode.hpp"

#include <signal.h>

static bool gIsRunnging = true;

void sig_handler(int sig)
{  
    if (sig == SIGINT)
    {
        signal(SIGINT, sig_handler);
        gIsRunnging = false;
    }
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Usage: rtsp_pull [rtsp_url]\n");
        return -1;
    }

    signal(SIGINT, sig_handler); 

    int ret = AX_SUCCESS;
    ret = AX_SYS_Init();
    if (ret != AX_SUCCESS)
    {
        printf("AX_SYS_Init failed! ret=0x%x\n", ret);
        return -1;
    }

    AX_POOL_FLOORPLAN_T pool_plan = {0};
    AX_POOL_CONFIG_T cmm_config = {0};
    cmm_config.BlkSize = 1920 * 1080 * 3 / 2;
    cmm_config.BlkCnt = 10;

    memcpy(&pool_plan.CommPool[0], &cmm_config, sizeof(AX_POOL_CONFIG_T));

    ret = AX_POOL_SetConfig(&pool_plan);
    if (ret != AX_SUCCESS)
    {
        printf("AX_POOL_SetConfig failed! ret=0x%x\n", ret);
        return -1;
    }

    ret = AX_POOL_Init();
    if (ret != AX_SUCCESS)
    {
        printf("AX_POOL_Init failed! ret=0x%x\n", ret);
        return -1;
    }

    const char* rtsp_url = argv[1];
    Json::Value config;
    config["rtsp_url"] = rtsp_url;

    ax::RTSPPullNode node;
    if (AX_SUCCESS != node.Init(config))
    {
        printf("init failed!\n");
        return -1;
    }

    node.SetRunning();
    node.Run();

    while(gIsRunnging)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    node.Stop();
    std::this_thread::sleep_for(std::chrono::seconds(3));

    AX_POOL_Exit();
    AX_SYS_Deinit();

    printf("Exit\n");
    return 0;
}