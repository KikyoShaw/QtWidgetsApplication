#pragma once

#include <QtWidgets/QWidget>
#include "ui_LineEdit.h"

class LineEdit : public QWidget
{
    Q_OBJECT

public:
    LineEdit(QWidget *parent = Q_NULLPTR);
	~LineEdit();

private slots:
	void sltNumLimitToggled(bool check);
	void sltNumLetterLimitToggled(bool check);
	void sltNoLimitToggled(bool check);
	void sltLimitChInput();
	void sltLimitInputLength();
	void sltInputIsPassWord();
	void sltTextEdited(const QString& text);

private:
    Ui::LineEditClass ui;

	QValidator *validator1 = nullptr;
	QValidator *validator2 = nullptr;
	QValidator *validator3 = nullptr;

	bool m_IsLimitChInput = true;
	bool m_IsPassWordInput = false;

	QStringList m_userIdList = QStringList();
	QString m_oldLineEditText = QString();
	QCompleter *m_completer = nullptr;
};
