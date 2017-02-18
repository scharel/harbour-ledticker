#include "bitmapmodel.h"
#include "font8x8_basic.h"

#include <QDebug>

BitmapModel::BitmapModel(QObject *parent) : QAbstractListModel(parent)
{
    m_rows = 0;
    m_columns = 0;
}

BitmapModel::~BitmapModel() {
    beginResetModel();
    m_bitmap.clear();
    m_rows = 0;
    emit rowsChanged(m_rows);
    m_columns = 0;
    emit columnsChanged(m_columns);
    endResetModel();
}

QHash<int, QByteArray> BitmapModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[RowRole] = "row";
    roles[ColumnRole] = "column";
    roles[BitOnRole] = "bitOn";
    return roles;
}

QVariant BitmapModel::data(const QModelIndex &index, int role) const {
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
        return QVariant(m_bitmap.testBit(index.row()));
    }
    return QVariant();
}

bool BitmapModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid()) {
        return false;
    }
    if (role == RowRole || role == ColumnRole) {
        return false;
    }
    if (role == BitOnRole) {
        m_bitmap.setBit(index.row(), value.toBool());
        emit dataChanged(index, index, QVector<int>(1, BitOnRole));
        return true;
    }
    return false;
}

bool BitmapModel::insertRows(int row, int count, const QModelIndex &parent) {
    beginInsertRows(parent, row, row + count-1);
    m_bitmap.resize(rowCount(parent) + count);
    endInsertRows();
    for (int i = rowCount(parent)-1; i >= row+count; --i) {
        setData(index(i), data(index(i-count), BitOnRole), BitOnRole);
        setData(index(i-count), false, BitOnRole);
    }
    return true;
}

int BitmapModel::insertRow(int position, int count) {
    if (count > 0) {
        insertRows(position * m_columns, count * m_columns);
        m_rows += count;
        emit rowsChanged(m_rows);
    }
    return m_rows;
}

int BitmapModel::appendRow(int count) {
    return insertRow(m_rows, count);
}

int BitmapModel::insertColumn(int position, int count) {
    if (count > 0) {
        for (int i = m_rows-1; i >= 0; --i) {
            insertRows(position + i * m_columns, count);
        }
        m_columns += count;
        emit columnsChanged(m_columns);
    }
    return m_columns;
}

int BitmapModel::appendColumn(int count) {
    return insertColumn(m_columns, count);
}

void BitmapModel::setDimensions(int rows, int columns) {
    if (rows >= 0 && columns >= 0 && (rows != m_rows || columns != m_columns)) {
        beginRemoveRows(QModelIndex(), 0, rowCount(QModelIndex()));
        m_bitmap = QBitArray();
        endRemoveRows();
        beginInsertRows(QModelIndex(), 0, rows * columns);
        m_bitmap = QBitArray(rows * columns);
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

void BitmapModel::setRows(int rows) {
    if (m_columns < 1)
        m_columns = 1;
    setDimensions(rows, m_columns);
}

void BitmapModel::setColumns(int columns) {
    if (m_rows < 1)
        m_rows = 1;
    setDimensions(m_rows, columns);
}

void BitmapModel::setBit(int row, int column, bool on) {
    if (row < 0 || column < 0 || row >= m_rows || column >= m_columns) {
        return;
    }
    setData(m_modelIndex(row, column), on, BitOnRole);
}

void BitmapModel::setBit(QVector<QPair<int,int>> bit, bool on) {
    for (int i = 0; i < bit.size(); ++i) {
        setBit(bit.at(i).first, bit.at(i).second, on);
    }
}

void BitmapModel::setRow(int row, bool on) {
    for (int i = 0; i < m_columns; ++i) {
        setBit(row, i, on);
    }
}

void BitmapModel::setColumn(int column, bool on) {
    for (int i = 0; i < m_rows; ++i) {
        setBit(i, column, on);
    }
}

bool BitmapModel::setChar8x8(int row, int column, QChar character, bool resize) {
    qDebug() << "Letter" << character << "Code" << character.unicode();
    if ((row + 8 < m_rows) && resize)
        appendRow((row+8)-m_rows);
    if ((column + 8 < m_columns) && resize)
        appendColumn((column+8)-m_columns);
    const char* charPattern = font8x8_basic[character.unicode()];
    for (int r = 0; r < 8; ++r) {
        char mask = 0x01;
        for (int c = 0; c < 8; ++c) {
            setBit(r+row, c+column, charPattern[r] & mask);
            mask<<=1;
        }
    }
    return false;
}

bool BitmapModel::setString8x8(int row, int column, QString text, bool resize) {
    for (int i = 0; i < text.length(); i++) {
        qDebug() << text[i].unicode();
    }
    return false;
}

void BitmapModel::init() {
    /*setColumn(0);
    setColumn(2);
    setColumn(4);
    setColumn(6);
    setColumn(8);
    setColumn(10);
    setColumn(12);
    setColumn(14);*/
    //insertColumn(3);
    //insertRow(3);
    //appendRow(2);
    //appendColumn(2);
    setChar8x8(1, 0, 'J');
    setChar8x8(1, 8, 'o');
    setChar8x8(1, 16, 'l');
    setChar8x8(1, 24, 'l');
    setChar8x8(1, 32, 'a');
    //setChar8x8(1, 25, 'i');
    //setString8x8(8, 1, "SailfishOS rules!");
}

int BitmapModel::m_bitmapIndex(int row, int column) const {
    if (row < 0 || column < 0 || row >= m_rows || column >= m_columns) {
        return -1;
    }
    return row * m_columns + column;
}

QModelIndex BitmapModel::m_modelIndex(int row, int column) const {
    return index(m_bitmapIndex(row, column));
}
