
#include "HelloWorldScene.h"
#include "skill_common.pb.h"
USING_NS_CC;


HelloWorld::HelloWorld()
	:m_Connect(false)
	,m_OpenReceive(false)
{

}

CCScene* HelloWorld::scene()
{
    CCScene *scene = CCScene::create();
    HelloWorld *layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif
}
//客户端
bool HelloWorld::CreateSecket()
{
	WSADATA data;
	WSAStartup((MAKEWORD(2,0)),&data);
	//常用的Socket类型有两种：流式Socket（SOCK_STREAM）和数据报式Socket（SOCK_DGRAM）。
	//流式是一种面向连接的Socket，针对于面向连接的TCP服务应用；数据报式Socket是一种无连接的Socket，对应于无连接的UDP服务应用。
	//两个网络程序之间的一个网络连接包括五种信息：通信协议、本地协议地址、本地主机端口、远端主机地址和远端协议端口。Socket数据结构中包含这五种信息。 
	//Socket编程基本就是listen，accept以及send，write等几个基本的操作。
	//对于TCP/IP，我们还知道TCP和UDP，前者可以保证数据的正确和可靠性，后者则允许数据丢失。在建立连接前，必须知道对方的IP地址和端口号
	//创建套接字
	m_Socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//第三个参数可以修改,修改UDP模式,比TCP快但是安全性差,会丢包
	if (!m_Socket)
	{
		CCLOG("[ ERROE ]Create SOCKET Failed\n");
		return false;
	}
	//链接可能会断开，断开之后需要重新发送这个链接
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(5203);
	int res = connect(m_Socket,(sockaddr*)&addr,sizeof(sockaddr_in));	//套接字m_Socket链接到地址addr上

	if (res == 0)					//返回0表时连接成功,返回-1表示连接失败
		return true;				//创建连接成功
	CCLOG("[ ERROE ]Create Connect Failed\n");
	return false;					//创建连接失败
}

bool HelloWorld::init()
{
    if ( !CCLayer::init() )return false;

	
	CreateMenu();					/*创建菜单按钮*/
	
	m_Connect = CreateSecket();		/*创建socket链接*/

    return true;
}

void HelloWorld::NetControl( CCObject* pSender )
{
	if (m_Connect)			//判断是否已经链接上了
	{
		int SendType = ((CCNode*)pSender)->getTag();

		/************************************************************************/
		/*		
				protobuf是一个协议库，存在多种发送消息的接口如:SerializeToArray、SerializeAsString(常用)
				创建一个协议的对象，为对象赋值之后将协议的对象以信息的形式发出
				从而实现数据的传递
		*/
		/************************************************************************/
		protos::common::Skill skill;					//定义一个protobuf结构并给它赋值
		skill.set_id(SendType);
		skill.set_anger(100+SendType);

		switch (SendType)
		{
		case OneTypeSend_:	//方式一,底层最终使用的方法数据传输方法,通过const char*传输数据
			{
				int MsgSize = send(m_Socket,"hello",strlen("hello")+1,0); 
				if ( MsgSize == strlen("hello")+1 )
				{
					printf("Send Message Success for OneType\n");
				}
			}
			break;
		case TwoTypeSend_:		//方式二
			{
				char buff[10240];										//构建好协议的内容之后发出，一般情况下常用这两种接口
				skill.SerializeToArray(buff,sizeof(buff));
				int MsgSize = send(m_Socket,buff,sizeof(buff),0);
				if (MsgSize == sizeof(buff))
				{
					printf("Send Message Success for TwoType\n");
				}
			}
			break;
		case ThreeTypeSend_:	//方式三
			{
				std::string str =skill.SerializeAsString();
				int MsgSize = send(m_Socket,str.c_str(),sizeof(str),0);
				if (MsgSize == sizeof(str))
				{
					printf("Send Message Success for ThreeType\n");
				}
			}
			break;
		case ReceiveControl_:
			{
				m_OpenReceive = !m_OpenReceive;
				ReceiveMessage();
			}
			break;
		default:
			CCLOG("[ TIPS ] Can't Understand The Button Type\n");
			break;
		}
	}else{ CCLOG("[ TIPS ]Connect Failed\n"); }
}

void HelloWorld::CreateMenu()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	//退出按钮
	SEL_MenuHandler select = menu_selector(HelloWorld::menuCloseCallback);
	CCMenuItemImage *pCloseItem = CCMenuItemImage::create("CloseNormal.png","CloseSelected.png",this,select);
	int x = origin.x + visibleSize.width - pCloseItem->getContentSize().width/2;
	int y = origin.y + pCloseItem->getContentSize().height/2;
	pCloseItem->setPosition(ccp(x,y));

	/************************************************************************/

	CCLabelTTF* oneSendLabel = CCLabelTTF::create("OneTypeSend_Mssage","Arial",30);
	CCMenuItemLabel* oneSendMsg = CCMenuItemLabel::create(oneSendLabel,this,menu_selector(HelloWorld::NetControl));
	oneSendMsg->setTag(OneTypeSend_);
	oneSendMsg->setPosition(ccp(visibleSize.width/2,100));

	CCLabelTTF* TwoSendLabel = CCLabelTTF::create("TwoTypeSend_Mssage","Arial",30);
	CCMenuItemLabel* TwoSendMsg = CCMenuItemLabel::create(TwoSendLabel,this,menu_selector(HelloWorld::NetControl));
	TwoSendMsg->setTag(TwoTypeSend_);
	TwoSendMsg->setPosition(ccp(visibleSize.width/2,150));

	CCLabelTTF* ThreeSendLabel = CCLabelTTF::create("ThreeTypeSend_Mssage","Arial",30);
	CCMenuItemLabel* ThreeSendMsg = CCMenuItemLabel::create(ThreeSendLabel,this,menu_selector(HelloWorld::NetControl));
	ThreeSendMsg->setTag(ThreeTypeSend_);
	ThreeSendMsg->setPosition(ccp(visibleSize.width/2,200));
	/*控制是否接收消息按钮*/
	CCLabelTTF* ReceiveLabel = CCLabelTTF::create("ControlReceive","Arial",30);
	CCMenuItemLabel* ReceiveLabelMsg = CCMenuItemLabel::create(ReceiveLabel,this,menu_selector(HelloWorld::NetControl));
	ReceiveLabelMsg->setTag(ReceiveControl_);
	ReceiveLabelMsg->setPosition(ccp(ReceiveLabel->getContentSize().width,visibleSize.height/2));

	CCMenu* pMenu = CCMenu::create(pCloseItem,oneSendMsg,TwoSendMsg,ThreeSendMsg,ReceiveLabelMsg,NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);
}

void HelloWorld::ReceiveMessage()
{
	while (true)
	{
		if (!m_OpenReceive)break;
		char buff[1024];
		int ret = recv(m_Socket,buff,1024,0);	//接收消息，但是消息只能判断消息的大小吗？
		if (ret>0)
		{
			printf("Receive Message Content :%s\n",buff);
		}
	}
}
