#include <Windows.h>
#include "UIlib.h"

#include "Utils/UIDelegate.h"

using namespace DuiLib;

class CTestWnd : public WindowImplBase
{
public:
	CTestWnd();
	~CTestWnd();

	// 配置界面布局和设置文件
	virtual DuiLib::CDuiString GetSkinFile();
	// 框架窗口类名
	virtual LPCTSTR    GetWindowClassName() const;
	// 在create时调用, 查找相关的控件类并进行处理
	virtual void InitWindow();
	// xml资源文件中肯定包含自定义控件的添加声明，此时在CDialogbuilder进行解析过程中会，查找到
	// 用户自定义控件，所以会调用CreateControl函数来进行，自定义控件的创建和绘制
	virtual CControlUI* CreateControl( LPCTSTR pstrClass );

	// 在操作界面顶部栏时触发这个函数
	virtual LRESULT OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	// 接受键盘按下的消息
	virtual LRESULT OnChar( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	// 划过应用时会触发这个消息
	virtual LRESULT OnMouseHover( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	// 接受鼠标滚轮的消息
	virtual LRESULT OnMouseWheel( UINT, WPARAM, LPARAM, BOOL& bHandled );

	// 在OnFinalMessage中delete this是因为DuiLib中需要使用new来生成一个窗口,delete
	// 可以防止内存泄漏(在后面的DuiLib程序中可以看到都只有new而没有delete,这是因
	// 为DuiLib内部在窗口销毁时已经做了delete的操作
	virtual void OnFinalMessage( HWND hWnd );

	// 这个是一个消息回调函数, 所有消息都会到这个函数这里来, WM_LBUTTONDOWN 或者是 WM_MOUSEWHEEL 操作相关处理
	// 在这个消息回调优先级前还有 HandleMessage 这个常用于窗口的创建, WM_CREATE ;  这个函数优先级后还有 MessageHandle
	LRESULT HandleCustomMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	// 这个回调函数是最优先的回调, 一般常用于窗口的创建, WM_CREATE
	LRESULT HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );

	// 事件触发 响应系统产生的消息
	// DuiLib将发送的Notify消息分为了同步和异步消息。同步就是立即调用，异步就是先放到队列中，下次再处理
	void Notify( TNotifyUI& msg );
	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnClick( TNotifyUI& msg );			// 消息映射建立的连接 优先级较低


	bool OnNotifyCommonUseItem( void *pParam );

	bool OntestEvent( void* pParam );

	bool OntestInit( void* pParam );

	bool OnSizeChange( void* pParam );

	void OnTimer( UINT_PTR nIDEvent );
private:
	CButtonUI* m_testui;
};

DUI_BEGIN_MESSAGE_MAP( CTestWnd, WindowImplBase )
DUI_ON_MSGTYPE( DUI_MSGTYPE_CLICK, OnClick )
DUI_END_MESSAGE_MAP()
CTestWnd::CTestWnd()
{

}

CTestWnd::~CTestWnd()
{

}

DuiLib::CDuiString CTestWnd::GetSkinFile()
{
	// 配置界面布局和设置
	return _T( "test.xml" );
}

LPCTSTR CTestWnd::GetWindowClassName() const
{
	// 框架窗口类名
	return _T( "DUIMainFrame" );
}

void CTestWnd::InitWindow()
{
	// 在create时调用, 查找相关的控件类并进行处理
	m_testui = static_cast< CButtonUI* >( m_pm.FindControl( _T( "closebtn" ) ) );
	if( m_testui ) {
		m_testui->OnNotify += MakeDelegate( this, &CTestWnd::OnNotifyCommonUseItem );	// 类似绑定过滤消息
		m_testui->OnEvent += MakeDelegate( this, &CTestWnd::OntestEvent );				// 类似绑定事件消息
		m_testui->OnInit += MakeDelegate( this, &CTestWnd::OntestInit );				// ???
		m_testui->OnSize += MakeDelegate( this, &CTestWnd::OnSizeChange );				// 大小布局发生改变产生消息
	}

	//??? 需要学习一下自定义控件的开发使用
// 	CVerticalLayoutUI* pverlayui = nullptr;
// 	CDialogBuilder buidler;
// 	CButtonUI* pbtn = dynamic_cast<CButtonUI*>( buidler.Create( _T( "" ), nullptr, nullptr, &m_pm ) );
// 	pverlayui->Add( pbtn );
// 	pbtn->SetManager( &m_pm, nullptr, false );

	// 发出一个WM_Timer消息出去, 然后可以根据消息去调用OnTimer
	ASSERT( ::IsWindow( m_hWnd ) );
	::SetTimer( m_hWnd, 1, 500, nullptr );		
}

CControlUI * CTestWnd::CreateControl( LPCTSTR pstrClass )
{
	// OnNotify += 意思就是用于添加一个事件相应处理函数，当接收到该时间之后，就会在响应的函数内处理
	// btn->OnNotify += MakeDelegate(this, &MainWindow::OnBtn, DUI_MSGTYPE_CLICK);
	if( m_testui ) {
		m_testui->OnNotify += MakeDelegate( this, &CTestWnd::OnNotifyCommonUseItem );
	}

	return m_testui;
}

LRESULT CTestWnd::OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	// 在操作界面顶部栏时触发这个函数
	if ( wParam == SC_CLOSE )
	{
	//	::PostQuitMessage( 0 );
		return 0;
	}

	return 0;
}

LRESULT CTestWnd::OnChar( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled )
{
	return LRESULT();
}

LRESULT CTestWnd::OnMouseHover( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled )
{
	return LRESULT();
}

LRESULT CTestWnd::OnMouseWheel( UINT, WPARAM, LPARAM, BOOL & bHandled )
{
	return LRESULT();
}

void CTestWnd::OnFinalMessage( HWND hWnd )
{
	__super::OnFinalMessage( hWnd );
	delete this;
}

LRESULT CTestWnd::HandleCustomMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	// 这个是一个消息回调函数, 所有消息都会到这个函数这里来, WM_LBUTTONDOWN 或者是 WM_MOUSEWHEEL 操作相关处理
	// 在这个消息回调优先级前还有 HandleMessage 这个常用于窗口的创建, WM_CREATE ;  这个函数优先级后还有 MessageHandle
	switch ( uMsg )
	{
		case WM_LBUTTONDOWN:
		{
			printf( "事件触发\n" );
			break;
		}
		case WM_TIMER:
		{
			OnTimer( wParam );
		}
	}
	return __super::HandleCustomMessage( uMsg, wParam, lParam, bHandled );
}

 LRESULT CTestWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
 {
 	// 这个回调函数是最优先的回调, 一般常用于窗口的创建, WM_CREATE
 	switch ( uMsg )
 	{
 	case WM_CREATE:
 	{
 		printf( "消息触发" );

 		break;
 	}
 	}

// 	在m_pm的MessageHandler()方法中首先调用了
// 	LRESULT lResult = static_cast<IMessageFilterUI*>( m_aMessageFilters[ i ] )->MessageHandler( uMsg, wParam, lParam, bHandled );

	// 不处理 或者 把消息继续路由的时候,  需要按照__super::HandleMssage 的方式进行处理
	return __super::HandleMessage( uMsg, wParam, lParam );
 }

void CTestWnd::Notify( TNotifyUI& msg )
{
	// 事件触发, 相应系统产生的消息
	return WindowImplBase::Notify( msg );
}

void CTestWnd::OnClick( TNotifyUI & msg )
{
	if ( msg.pSender == m_testui )
	{
		::PostQuitMessage( 0 );
	}
}

bool CTestWnd::OnNotifyCommonUseItem( void *pParam )
{
	TNotifyUI* pNotify = static_cast< TNotifyUI* >( pParam );
	if ( !pNotify || !pNotify->pSender )
	{
		return false;
	}

	if ( pNotify->sType == DUI_MSGTYPE_CLICK )
	{
		OutputDebugStringW( _T("one\n") );
		return true;			// 需要把消息继续向后面路由的时候 返回为true, 截至的时候就返回为false
	}
	
	// 把消息继续路由的时候 返回就需要为true
	return true;
}

bool CTestWnd::OntestEvent( void* pParam )
{
	TEventUI* evt = static_cast< TEventUI* >( pParam );
	if ( evt )
	{
		if( evt->Type == UIEVENT_BUTTONDOWN )
		{
			OutputDebugStringW( _T("help\n") );
			return true;
		}
		else if( evt->Type == UIEVENT_MOUSEENTER )
		{
			return true;
		}
	}
	else
	{
		return false;
	}
	return true;
}

bool CTestWnd::OntestInit( void * pParam )
{
	return true;
}

bool CTestWnd::OnSizeChange( void * pParam )
{
	return true;
}

void CTestWnd::OnTimer( UINT_PTR nIDEvent )
{
	KillTimer( m_hWnd, nIDEvent );
	OutputDebugStringW( _T( "call ontimer" ) );
}

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	HRESULT Hr = ::CoInitialize(nullptr);

	if ( FAILED(Hr) )
	{
		return 0;
	}
	CPaintManagerUI::SetInstance( hInstance );

	CPaintManagerUI::SetResourcePath( CPaintManagerUI::GetInstancePath() );
	CTestWnd* pFrame = new CTestWnd();
	pFrame->Create( NULL, _T( "AAAA" ), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE );
	pFrame->CenterWindow();
	
	pFrame->ShowWindow( true );

	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	return 0;
}