#pragma once

#include <QGraphicsView>

class View : public QFrame
{
    QGraphicsView * gView{ nullptr };
public:
    explicit View( QWidget *parent = nullptr );
    auto getView() const -> QGraphicsView&;
};
