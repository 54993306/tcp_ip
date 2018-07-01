
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
//�ͻ���
bool HelloWorld::CreateSecket()
{
	WSADATA data;
	WSAStartup((MAKEWORD(2,0)),&data);
	//���õ�Socket���������֣���ʽSocket��SOCK_STREAM�������ݱ�ʽSocket��SOCK_DGRAM����
	//��ʽ��һ���������ӵ�Socket��������������ӵ�TCP����Ӧ�ã����ݱ�ʽSocket��һ�������ӵ�Socket����Ӧ�������ӵ�UDP����Ӧ�á�
	//�����������֮���һ���������Ӱ���������Ϣ��ͨ��Э�顢����Э���ַ�����������˿ڡ�Զ��������ַ��Զ��Э��˿ڡ�Socket���ݽṹ�а�����������Ϣ�� 
	//Socket��̻�������listen��accept�Լ�send��write�ȼ��������Ĳ�����
	//����TCP/IP�����ǻ�֪��TCP��UDP��ǰ�߿��Ա�֤���ݵ���ȷ�Ϳɿ��ԣ��������������ݶ�ʧ���ڽ�������ǰ������֪���Է���IP��ַ�Ͷ˿ں�
	//�����׽���
	m_Socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//���������������޸�,�޸�UDPģʽ,��TCP�쵫�ǰ�ȫ�Բ�,�ᶪ��
	if (!m_Socket)
	{
		CCLOG("[ ERROE ]Create SOCKET Failed\n");
		return false;
	}
	//���ӿ��ܻ�Ͽ����Ͽ�֮����Ҫ���·����������
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(5203);
	int res = connect(m_Socket,(sockaddr*)&addr,sizeof(sockaddr_in));	//�׽���m_Socket���ӵ���ַaddr��

	if (res == 0)					//����0��ʱ���ӳɹ�,����-1��ʾ����ʧ��
		return true;				//�������ӳɹ�
	CCLOG("[ ERROE ]Create Connect Failed\n");
	return false;					//��������ʧ��
}

bool HelloWorld::init()
{
    if ( !CCLayer::init() )return false;

	
	CreateMenu();					/*�����˵���ť*/
	
	m_Connect = CreateSecket();		/*����socket����*/

    return true;
}

void HelloWorld::NetControl( CCObject* pSender )
{
	if (m_Connect)			//�ж��Ƿ��Ѿ���������
	{
		int SendType = ((CCNode*)pSender)->getTag();

		/************************************************************************/
		/*		
				protobuf��һ��Э��⣬���ڶ��ַ�����Ϣ�Ľӿ���:SerializeToArray��SerializeAsString(����)
				����һ��Э��Ķ���Ϊ����ֵ֮��Э��Ķ�������Ϣ����ʽ����
				�Ӷ�ʵ�����ݵĴ���
		*/
		/************************************************************************/
		protos::common::Skill skill;					//����һ��protobuf�ṹ��������ֵ
		skill.set_id(SendType);
		skill.set_anger(100+SendType);

		switch (SendType)
		{
		case OneTypeSend_:	//��ʽһ,�ײ�����ʹ�õķ������ݴ��䷽��,ͨ��const char*��������
			{
				int MsgSize = send(m_Socket,"hello",strlen("hello")+1,0); 
				if ( MsgSize == strlen("hello")+1 )
				{
					printf("Send Message Success for OneType\n");
				}
			}
			break;
		case TwoTypeSend_:		//��ʽ��
			{
				char buff[10240];										//������Э�������֮�󷢳���һ������³��������ֽӿ�
				skill.SerializeToArray(buff,sizeof(buff));
				int MsgSize = send(m_Socket,buff,sizeof(buff),0);
				if (MsgSize == sizeof(buff))
				{
					printf("Send Message Success for TwoType\n");
				}
			}
			break;
		case ThreeTypeSend_:	//��ʽ��
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
	//�˳���ť
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
	/*�����Ƿ������Ϣ��ť*/
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
		int ret = recv(m_Socket,buff,1024,0);	//������Ϣ��������Ϣֻ���ж���Ϣ�Ĵ�С��
		if (ret>0)
		{
			printf("Receive Message Content :%s\n",buff);
		}
	}
}
