#include "fontbitmap.h"
#include "font8x8_basic.h"

#include <QDebug>

FontBitmap::FontBitmap(QObject *parent) : QAbstractListModel(parent)
{
    m_rows = 0;
    m_columns = 0;
}

QHash<int, QByteArray> FontBitmap::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[RowRole] = "row";
    roles[ColumnRole] = "column";
    roles[BitOnRole] = "bitOn";
    return roles;
}

QVariant FontBitmap::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    if (role == RowRole) {
        return index.row() / m_columns;
    }
    if (role == ColumnRole) {
        return index.row() % m_columns;
    }
    if (role == BitOnRole) {
        return QVariant(m_matrix.testBit(index.row()));
    }
    return QVariant();
}

bool FontBitmap::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid()) {
        return false;
    }
    if (role == RowRole || role == ColumnRole) {
        return false;
    }
    if (role == BitOnRole) {
        m_matrix.setBit(index.row(), value.toBool());
        emit dataChanged(index, index, QVector<int>(1, BitOnRole));
        return true;
    }
    return false;
}

void FontBitmap::setRows(int rows) {
    if (rows >= 0 && rows != m_rows) {
        beginRemoveRows(QModelIndex(), 0, m_rows * m_columns);
        m_matrix = QBitArray();
        endRemoveRows();
        beginInsertRows(QModelIndex(), 0, rows * m_columns);
        m_rows = rows;
        m_matrix = QBitArray(m_rows * m_columns);
        endInsertRows();
        emit rowsChanged(m_rows);
    }
}

void FontBitmap::setColumns(int columns) {
    if (columns >= 0 && columns != m_columns) {
        beginRemoveRows(QModelIndex(), 0, m_rows * m_columns);
        m_matrix = QBitArray();
        endRemoveRows();
        beginInsertRows(QModelIndex(), 0, m_rows * columns);
        m_columns = columns;
        m_matrix = QBitArray(m_rows * m_columns);
        endInsertRows();
        emit columnsChanged(m_columns);
    }
}

void FontBitmap::setBit(int row, int column, bool on) {
    if (row < 0 || column < 0 || row >= m_rows || column >= m_columns) {
        return;
    }
    setData(m_modelIndex(row, column), on, BitOnRole);
}

void FontBitmap::setBit(QVector<QPair<int,int>> bit, bool on) {
    for (int i = 0; i < bit.size(); ++i) {
        setData(m_modelIndex(bit.at(i).first, bit.at(i).second), on, BitOnRole);
    }
}

void FontBitmap::init() {
    QVector<QPair<int,int>> bits;
    bits.append(QPair<int,int>(4,2));
    bits.append(QPair<int,int>(3,3));
    bits.append(QPair<int,int>(5,3));
    bits.append(QPair<int,int>(2,4));
    bits.append(QPair<int,int>(6,4));
    bits.append(QPair<int,int>(2,5));
    bits.append(QPair<int,int>(6,5));
    bits.append(QPair<int,int>(2,6));
    bits.append(QPair<int,int>(6,6));
    bits.append(QPair<int,int>(2,7));
    bits.append(QPair<int,int>(6,7));
    bits.append(QPair<int,int>(2,8));
    bits.append(QPair<int,int>(6,8));
    bits.append(QPair<int,int>(3,9));
    bits.append(QPair<int,int>(5,9));
    bits.append(QPair<int,int>(4,10));
    bits.append(QPair<int,int>(5,11));
    bits.append(QPair<int,int>(3,11));
    setBit(bits, true);
}

int FontBitmap::m_matrixIndex(int row, int column) const {
    if (row < 0 || column < 0 || row >= m_rows || column >= m_columns) {
        return -1;
    }
    return row * m_columns + column;
}

QModelIndex FontBitmap::m_modelIndex(int row, int column) const {
    return index(m_matrixIndex(row, column));
}
