#ifndef ASPECTRATIOLABEL_H
#define ASPECTRATIOLABEL_H

#include <QLabel>

class AspectRatioLabel : public QLabel
{
public:
    explicit AspectRatioLabel(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~AspectRatioLabel();

public slots:
    void setPixmapWithAspectRatio(const QPixmap& pm);

private:
    double aspectRatio;
    QPixmap originalPixmap;

    void updatePixmap();
    double computeAspectRatio(int w, int h);
};

#endif // ASPECTRATIOLABEL_H
