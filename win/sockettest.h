#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
    virtual bool init();
	HelloWorld();
    static cocos2d::CCScene* scene();
    void menuCloseCallback(CCObject* pSender);
    CREATE_FUNC(HelloWorld);
	void CreateMenu();
	bool CreateSecket();
	void NetControl(CCObject* pSender);
	void ReceiveMessage();
private:
	SOCKET m_Socket;
	bool m_Connect;
	bool m_OpenReceive;
};

enum Tag
{
	End_,
	OneTypeSend_,
	TwoTypeSend_,
	ThreeTypeSend_,
	ReceiveControl_,
};
#endif // __HELLOWORLD_SCENE_H__
