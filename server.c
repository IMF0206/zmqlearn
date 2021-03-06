//包含zmq的头文件
#include <zmq.h>
#include "stdio.h"

int main(int argc, char * argv[])
{
    void * pCtx = NULL;
    void * pSock = NULL;
    const char * pAddr = "tcp://127.0.0.1:7766";

    //创建context，zmq的socket 需要在context上进行创建
    if((pCtx = zmq_ctx_new()) == NULL)
    {
	printf("zmq_ctx_new error\n");
        return 0;
    }
    //创建zmq socket ，socket目前有6中属性 ，这里使用dealer方式
    //具体使用方式请参考zmq官方文档（zmq手册）
    //if((pSock = zmq_socket(pCtx, ZMQ_DEALER)) == NULL)
    if((pSock = zmq_socket(pCtx, ZMQ_REP)) == NULL)
    {
	printf("zmq_socket error\n");
        zmq_ctx_destroy(pCtx);
        return 0;
    }
    //int iRcvTimeout = 5000;// millsecond
    //设置zmq的接收超时时间为5秒
    //if(zmq_setsockopt(pSock, ZMQ_RCVTIMEO, &iRcvTimeout, sizeof(iRcvTimeout)) < 0)
    //{
    //    printf("zmq_setsocketopt error\n");
    //    zmq_close(pSock);
    //    zmq_ctx_destroy(pCtx);
    //    return 0;
    //}
    //绑定地址 tcp://*:7766
    //也就是使用tcp协议进行通信，使用网络端口 7766
    if(zmq_bind(pSock, pAddr) < 0)
    {
	printf("zmq_bind failed : %d\n", zmq_bind(pSock, pAddr));
        zmq_close(pSock);
        zmq_ctx_destroy(pCtx);
        return 0;
    }
    printf("bind at : %s\n", pAddr);
    while(1)
    {
        char szMsg[1024] = {0};
        printf("waitting...\n");
        errno = 0;
        //循环等待接收到来的消息，当超过5秒没有接到消息时，
        //zmq_recv函数返回错误信息 ，并使用zmq_strerror函数进行错误定位
        if(zmq_recv(pSock, szMsg, sizeof(szMsg), 0) < 0)
        {
            printf("error = %s\n", zmq_strerror(errno));
            //continue;
        }
	printf("received message : %s\n", szMsg);
	if(zmq_send(pSock, "Get!", sizeof("Get!"), 0) < 0)
	{
	    printf("error = %s\n", zmq_strerror(errno));
	    //continue;
	}
	printf("send over \n");
    }

    return 0;
}
