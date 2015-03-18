#include "AppDelegate.h"
#include "PreGameScene.h"
#include "ConstValue.h"

USING_NS_CC;

void AppDelegate::betterWidthAndHeight()
{
//�����������µĴ��ڵ���
#ifdef MOVE_PORTRAIT_MODE
	m_WndWidth = 270;
	m_WndHeight = 480;

#else
	m_WndWidth = 480;
	m_WndHeight = 270;
	
#endif
//������Ⱥ͸߶������µ�GL��Ⱦ�������
#ifdef BK_BASE_HEIGHT_MODE
	m_GLViewHeight = BK_BASE_LENGTH;
	m_GLViewWidth = m_GLViewHeight*m_WndWidth / m_WndHeight;
#else
	m_GLViewWidth = BK_BASE_LENGTH;
	m_GLViewHeight = m_GLViewWidth*m_WndHeight / m_WndWidth;
#endif
}

AppDelegate::AppDelegate() {
	betterWidthAndHeight();
}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
		//����OPENGL��Ⱦ�Ĵ�������
		glview = GLView::createWithRect("Beyond's game", Rect(0, 0, m_WndWidth, m_WndHeight), 1.0f);
        director->setOpenGLView(glview);
    }

	//��Ļ����Ӧ����
	glview->setDesignResolutionSize(m_GLViewWidth,m_GLViewHeight, ResolutionPolicy::EXACT_FIT);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = PreGameScene::create();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
