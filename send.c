//包含zmq的头文件
#include <zmq.h>
#include "stdio.h"

int main(int argc, char * argv[])
{
    void * pCtx = NULL;
    void * pSock = NULL;
    //使用tcp协议进行通信，需要连接的目标机器IP地址为192.168.1.2
    //通信使用的网络端口 为7766
    const char * pAddr = "tcp://127.0.0.1:7766";

    //创建context
    if((pCtx = zmq_ctx_new()) == NULL)
    {
        return 0;
    }
    //创建socket
    //if((pSock = zmq_socket(pCtx, ZMQ_DEALER)) == NULL)
    if((pSock = zmq_socket(pCtx, ZMQ_REQ)) == NULL)
    {
        zmq_ctx_destroy(pCtx);
        return 0;
    }
    int iSndTimeout = 5000;// millsecond
    //设置接收超时
    if(zmq_setsockopt(pSock, ZMQ_RCVTIMEO, &iSndTimeout, sizeof(iSndTimeout)) < 0)
    {
        zmq_close(pSock);
        zmq_ctx_destroy(pCtx);
        return 0;
    }
    //连接目标IP192.168.1.2，端口7766
    if(zmq_connect(pSock, pAddr) < 0)
    {
        zmq_close(pSock);
        zmq_ctx_destroy(pCtx);
        return 0;
    }
    //循环发送消息
    while(1)
    {
        static int i = 0;
        char szMsg[1024] = {0};
        scanf("%[^\n]",szMsg);
        //snprintf(szMsg, sizeof(szMsg), "hello world : %3d", i++);
        printf("Enter to send...\n");
        if(zmq_send(pSock, szMsg, sizeof(szMsg), 0) < 0)
        {
            fprintf(stderr, "send message faild\n");
            continue;
        }
        printf("send message : [%s] succeed\n", szMsg);
        getchar();
    }

    return 0;
}
