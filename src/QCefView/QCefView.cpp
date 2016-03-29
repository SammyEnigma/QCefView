#include "stdafx.h"
#pragma region qt_headers
#include <QVBoxLayout>
#include <QWindow>
#include <QResizeEvent>
#include <QVariant>
#pragma endregion qt_headers

#pragma region cef_headers
#include <include/cef_app.h>
#include <include/cef_browser.h>
#include <include/cef_frame.h>
#include <include/cef_sandbox_win.h>
#pragma endregion cef_headers

#include "inc/QCefQuery.h"
#include "inc/QCefView.h"
#include "CCefManager.h"
#include "CCefWindow.h"

//////////////////////////////////////////////////////////////////////////
QCefView::QCefView(const QString& url, QWidget* parent /*= 0*/)
	: QWidget(parent)
	, cefWindow_(NULL)
{
	QGridLayout* layout = new QGridLayout;
	layout->setContentsMargins(0, 0, 0, 0);

	/*
	 * Here we must create a QWidget as a wrapper 
	 * to encapsulate the QWindow
	 */
	cefWindow_ = new CCefWindow(url, this, window()->windowHandle());
	QWidget* windowContainer = createWindowContainer(cefWindow_, this);
	if (windowContainer)
	{
		layout->addWidget(windowContainer);
	}

	setLayout(layout);
}

QCefView::~QCefView()
{

}

void QCefView::processQCefUrlRequest(const QString& url)
{

}

void QCefView::processQCefQueryRequest(QCefQuery query)
{

}

void QCefView::navigateToString(const QString& content, const QString& url)
{
	if (cefWindow_)
	{
		CefString strContent;
		strContent.FromString(content.toStdString());
		CefString strUrl;
		strUrl.FromString(url.toStdString());
		cefWindow_->cefViewHandler()->GetBrowser()->GetMainFrame()->LoadString(strContent, strUrl);
	}
}

void QCefView::navigateToUrl(const QString& url)
{
	if (cefWindow_)
	{
		CefString strUrl;
		strUrl.FromString(url.toStdString());
		cefWindow_->cefViewHandler()->GetBrowser()->GetMainFrame()->LoadURL(strUrl);
	}
}

bool QCefView::browserCanGoBack()
{
	if (cefWindow_)
	{
		return cefWindow_->cefViewHandler()->GetBrowser()->CanGoBack();
	}
	return false;
}

bool QCefView::browserCanGoForward()
{
	if (cefWindow_)
	{
		return cefWindow_->cefViewHandler()->GetBrowser()->CanGoForward();
	}
	return false;
}

void QCefView::browserGoBack()
{
	if (cefWindow_)
	{
		cefWindow_->cefViewHandler()->GetBrowser()->GoBack();
	}
}

void QCefView::browserGoForward()
{
	if (cefWindow_)
	{
		cefWindow_->cefViewHandler()->GetBrowser()->CanGoForward();
	}
}

bool QCefView::browserIsLoading()
{
	if (cefWindow_)
	{
		return cefWindow_->cefViewHandler()->GetBrowser()->IsLoading();
	}
	return false;
}

void QCefView::browserReload()
{
	if (cefWindow_)
	{
		cefWindow_->cefViewHandler()->GetBrowser()->Reload();
	}
}

void QCefView::browserStopLoad()
{
	if (cefWindow_)
	{
		cefWindow_->cefViewHandler()->GetBrowser()->StopLoad();
	}
}

void QCefView::onLoadStateChange(bool isLoading)
{

}

void QCefView::onLoadStart()
{

}

void QCefView::onLoadEnd(int httpStatusCode)
{

}

void QCefView::onLoadError(int errorCode,
	const QString& errorMsg,
	const QString& failedUrl)
{

}

bool QCefView::triggerEvent(const QString& name, const QVariantList& args)
{
	if (name.isEmpty())
	{
		return false;
	}

	CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create(
		TRIGGER_EVENT_MESSAGE_NAME);
	CefRefPtr<CefListValue> arguments = msg->GetArgumentList();
	CefString eventName;
	eventName.FromString(name.toStdString());
	arguments->SetString(0, eventName);

	CefString strTemp;
	for (int i = 0; i < args.size(); i++)
	{
		if (args[i].type() == QMetaType::Bool)
		{
			arguments->SetBool(i + 1,
				args[i].value<bool>());
		}
		else if (args[i].type() == QMetaType::Int || args[i].type() == QMetaType::UInt)
		{
			arguments->SetInt(i + 1,
				args[i].value<int>());
		}
		else if (args[i].type() == QMetaType::Double)
		{
			arguments->SetDouble(i + 1,
				args[i].value<double>());
		}
		else if (args[i].type() == QMetaType::QString)
		{
			QString val = args[i].value<QString>();
			strTemp.FromString(val.toStdString());
			arguments->SetString(i + 1, strTemp);
		}
		else
		{
			arguments->SetNull(i + 1);
			__noop(_T("GxxCefView"), _T("Unknow Type!"));
		}
	}

	return cefWindow_->cefViewHandler()->TriggerEvent(msg);
}

void QCefView::NotifyMoveOrResizeStarted()
{
	if (cefWindow_)
	{
		CefRefPtr<QCefViewBrowserHandler> handler = cefWindow_->cefViewHandler();
		if (handler)
		{
			CefRefPtr<CefBrowser> browser = handler->GetBrowser();
			if (browser)
			{
				CefRefPtr<CefBrowserHost> host = browser->GetHost();
				if (host)
				{
					host->NotifyMoveOrResizeStarted();
				}
			}
		}
	}
}