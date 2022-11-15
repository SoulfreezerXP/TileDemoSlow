#pragma once

#include <QAbstractTableModel>

class TilePaletteModel : public QAbstractTableModel
{
     Q_OBJECT
public:
     TilePaletteModel( QObject *parent );
     int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
     int columnCount( const QModelIndex &parent = QModelIndex() ) const override;
     QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;
};
