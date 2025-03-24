#ifndef ASPECTRATIOPIXMAPLABEL_H
#define ASPECTRATIOPIXMAPLABEL_H

#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>

class AspectRatioPixmapLabel : public QLabel {
    Q_OBJECT
public:
    explicit AspectRatioPixmapLabel(QWidget* parent = nullptr);
    void setPixmap(const QPixmap& pm);
protected:
    void resizeEvent(QResizeEvent* event) override;
private:
    QPixmap originalPixmap;
    void updatePixmap();
};

#endif // ASPECTRATIOPIXMAPLABEL_H
