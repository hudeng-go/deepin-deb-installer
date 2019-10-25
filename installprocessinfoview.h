#ifndef INSTALLPROCESSINFOVIEW_H
#define INSTALLPROCESSINFOVIEW_H

#include <QPainter>
#include <QPaintEvent>

#include <DTextEdit>

DWIDGET_USE_NAMESPACE

class InstallProcessInfoView : public DWidget
{
    Q_OBJECT
public:
    explicit InstallProcessInfoView(QWidget *parent = nullptr);
    virtual ~InstallProcessInfoView() override;

    void appendText(QString text);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void initUI();

    DTextEdit *m_editor;
};

#endif // INSTALLPROCESSINFOVIEW_H
