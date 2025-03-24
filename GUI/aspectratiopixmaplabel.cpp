#include "aspectratiopixmaplabel.h"

AspectRatioPixmapLabel::AspectRatioPixmapLabel(QWidget* parent): QLabel(parent) {
    setMinimumSize(1, 80);  // Минимальный размер для правильного масштабирования
    setMaximumSize(1, 80);
}

void AspectRatioPixmapLabel::setPixmap(const QPixmap& pm) {
    originalPixmap = pm;  // Сохраняем оригинальное изображение
    updatePixmap();        // Обновляем изображение, чтобы оно правильно масштабировалось
}

void AspectRatioPixmapLabel::resizeEvent(QResizeEvent* event) {
    updatePixmap();  // Обновляем изображение при изменении размера
    QLabel::resizeEvent(event);  // Стандартная обработка события изменения размера
}

void AspectRatioPixmapLabel::updatePixmap() {
    if (originalPixmap.isNull()) {
        return;  // Если изображения нет, ничего не делаем
    }

    // Масштабируем изображение с сохранением пропорций с увеличением
    QPixmap scaled = originalPixmap.scaled(
        size().width() * 3,  // Увеличение по ширине (на 5)
        size().height() * 3, // Увеличение по высоте (на 5)
        Qt::KeepAspectRatio,  // Сохраняем пропорции
        Qt::SmoothTransformation  // Плавное преобразование
    );

    // Устанавливаем масштабированное изображение в QLabel
    QLabel::setPixmap(scaled);
}
