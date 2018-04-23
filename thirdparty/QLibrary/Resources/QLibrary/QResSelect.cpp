#include "QResSelect.h"
#include <QFileDialog>
#include "QPropertyModel.h"
#include <engine/core/util/PathUtil.h>
#include <engine/core/io/IO.h>

namespace QT_UI
{
	QResSelect::OpenFileDialogFunction QResSelect::m_openFileFunction;

	// ���캯��
	QResSelect::QResSelect(class QPropertyModel* model, QString propertyName, const char* exts, const char* files, QWidget* parent)
		: QWidget( parent)
		, m_propertyModel(model)
		, m_propertyName(propertyName)
		, m_exts(exts)
		, m_files(files)
	{
		// ���ֿؼ�
		m_horizonLayout = new QHBoxLayout( this);
		m_horizonLayout->setSpacing( 0);
		m_horizonLayout->setContentsMargins(0, 0, 0, 0);
		m_horizonLayout->setObjectName( QString::fromUtf8("horizontalLayout"));

		// LineEdit
		m_lineEdit = new QLineEdit( this);
		m_lineEdit->setObjectName( QString::fromUtf8("lineEdit"));
		m_lineEdit->setContentsMargins(0, 0, 0, 0);
		m_horizonLayout->addWidget( m_lineEdit);

		// ToolButton
		m_toolButton = new QToolButton( this);
		m_toolButton->setObjectName( QString::fromUtf8("toolButton"));
		m_toolButton->setText( "...");
		m_toolButton->setContentsMargins(0, 0, 0, 0);
		m_horizonLayout->addWidget( m_toolButton);

		setFocusProxy( m_toolButton);

		// ��Ϣ
		QObject::connect( m_toolButton, SIGNAL(clicked()), this, SLOT(OnSelectPath()));
		QObject::connect(m_lineEdit, SIGNAL(returnPressed()), this, SLOT(onEditFinished()));

		if (isTextureRes())
		{
			m_lineEdit->setMinimumHeight(m_lineEdit->geometry().height()*1.6);
			m_toolButton->setMinimumHeight(m_toolButton->geometry().height() * 1.6);
		}
	}

	// ѡ��·��
	void QResSelect::OnSelectPath()
	{
		Q_ASSERT_X(m_openFileFunction, "", "");
		Echo::String qFileName = m_openFileFunction(this, m_exts.c_str(), m_files.toStdString().c_str(), "");
		if (!qFileName.empty())
		{
			m_lineEdit->setText(qFileName.c_str());
			onEditFinished();
		}
	}

	// is texture res
	bool QResSelect::isTextureRes()
	{
		Echo::StringArray exts = Echo::StringUtil::Split(m_exts, "|");
		for (Echo::String ext : exts)
		{
			if (ext == ".png")
				return true;
		}

		return false;
	}

	// redefine paintEvent
	void QResSelect::paintEvent(QPaintEvent* event)
	{
		// get label background color
		//QColor background = m_toolButton->palette().color(QPalette::Window);

		// add background color for this widget.(Transparent is inappropriate here)
		//QPainter painter(this);
		//painter.setBrush(background);
		//painter.drawRect(rect());
		//painter.setPen(background);
		//painter.drawRect(rect());

		if (isTextureRes())
		{

		}
		else
		{
			QWidget::paintEvent(event);
		}
	}

	// edit finished
	void QResSelect::onEditFinished()
	{
		m_propertyModel->setValue(m_propertyName, m_lineEdit->text().toStdString().c_str());
	}

	// MVC��Ⱦ
	void QResSelect::ItemDelegatePaint(QPainter *painter, const QRect& rect, const Echo::String& val)
	{
		Echo::String path = val;
		Echo::String fullPath = Echo::IO::instance()->getFullPath(path);
		Echo::String ext = Echo::PathUtil::GetFileExt(path, true);
		if (ext == ".png")
		{
			QPixmap pixmap(fullPath.c_str());
			QRect tRect = QRect(rect.left() + 3, rect.top() + 2, rect.height() - 4, rect.height() - 4);
			painter->drawPixmap( tRect, pixmap);
		}
	}
}