#pragma once

#include <QDockWidget>
#include <QTreeView>
#include <QStandardItemModel>

class TilePalette : public QDockWidget
{
    Q_OBJECT
    QStandardItemModel *model;
public:
     TilePalette( QObject *parent = nullptr );
     QTreeView *view;
};
