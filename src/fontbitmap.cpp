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

bool FontBitmap::insertRows(int row, int count, const QModelIndex &parent) {
    beginInsertRows(parent, row, row + count-1);
    m_matrix.resize(rowCount(parent) + count);
    endInsertRows();
    for (int i = rowCount(parent)-1; i >= row+count; --i) {
        setData(index(i), data(index(i-count), BitOnRole), BitOnRole);
        setData(index(i-count), false, BitOnRole);
    }
    return true;
}

int FontBitmap::insertRow(int position, int count) {
    if (count > 0) {
        insertRows(position * m_columns, count * m_columns);
        m_rows += count;
        emit rowsChanged(m_rows);
    }
    return m_rows;
}

int FontBitmap::insertColumn(int position, int count) {
    if (count > 0) {
        for (int i = m_rows-1; i >= 0; --i) {
            insertRows(position + i * m_columns, count);
        }
        m_columns += count;
        emit columnsChanged(m_columns);
    }
    return m_columns;
}

void FontBitmap::setDimensions(int rows, int columns) {
    if (rows >= 0 && columns >= 0 && (rows != m_rows || columns != m_columns)) {
        beginRemoveRows(QModelIndex(), 0, rowCount(QModelIndex()));
        m_matrix = QBitArray();
        endRemoveRows();
        beginInsertRows(QModelIndex(), 0, rows * columns);
        m_matrix = QBitArray(rows * columns);
        if (rows != m_rows) {
            m_rows = rows;
            emit rowsChanged(m_rows);
        }
        if (columns != m_columns) {
            m_columns = columns;
            emit columnsChanged(m_columns);
        }
        endInsertRows();
    }
}

void FontBitmap::setRows(int rows) {
    if (m_columns < 1)
        m_columns = 1;
    setDimensions(rows, m_columns);
}

void FontBitmap::setColumns(int columns) {
    if (m_rows < 1)
        m_rows = 1;
    setDimensions(m_rows, columns);
}

void FontBitmap::setBit(int row, int column, bool on) {
    if (row < 0 || column < 0 || row >= m_rows || column >= m_columns) {
        return;
    }
    setData(m_modelIndex(row, column), on, BitOnRole);
}

void FontBitmap::setBit(QVector<QPair<int,int>> bit, bool on) {
    for (int i = 0; i < bit.size(); ++i) {
        setBit(bit.at(i).first, bit.at(i).second, on);
    }
}

void FontBitmap::setRow(int row, bool on) {
    for (int i = 0; i < m_columns; ++i) {
        setBit(row, i, on);
    }
}

void FontBitmap::setColumn(int column, bool on) {
    for (int i = 0; i < m_rows; ++i) {
        setBit(i, column, on);
    }
}

void FontBitmap::init() {
    setColumn(0);
    setColumn(2);
    setColumn(4);
    setColumn(6);
    setColumn(8);
    setColumn(10);
    setColumn(12);
    setColumn(14);
    insertColumn(3);
    insertRow(3);
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
