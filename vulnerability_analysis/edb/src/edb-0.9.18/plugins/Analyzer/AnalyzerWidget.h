
#include <QWidget>

class AnalyzerWidget : public QWidget {
	Q_OBJECT
public:
	AnalyzerWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);
	
protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	
private:
	bool mouse_pressed_;
};
