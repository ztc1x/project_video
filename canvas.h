#ifndef CANVAS_H
#define CANVAS_H

#include <QLabel>
#include <QMouseEvent>
#include <QObject>
#include <QImage>

class canvas : public QLabel
{
    Q_OBJECT
public:
    canvas();
private:
    int next_x, next_y;
    QImage frame;
protected:
    void mousePressEvent(QMouseEvent *ev);
    //void paintEvent(QPaintEvent *ev);
signals:
    //void sig_mark_point(int x, int y);
public slots:
    void slot_set_image(QImage image);
};

#endif // CANVAS_H
