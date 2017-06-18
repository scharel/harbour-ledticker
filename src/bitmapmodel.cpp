#include "bitmapmodel.h"
#include "font4x7.h"
#include "font5x8.h"
#include "font7x9.h"

#include <QDebug>

BitmapModel::BitmapModel(QObject *parent) : QAbstractListModel(parent) {
    clear()
}

BitmapModel::~BitmapModel() {
    clear();
}

QHash<int, QByteArray> BitmapModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[OnRole] = "on";
    roles[ColumnRole] = "column";
    roles[RowRole] = "row";
    return roles;
}

int BitmapModel::rowCount(const QModelIndex &parent) const {
    if (m_virtualVisible)
        return m_virtualColumns * m_rows;
    else
        return m_columns * m_rows;
}

QVariant BitmapModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    if (role == OnRole) {
        return QVariant(m_bitmap.testBit(m_bitmapIndex(index)));
    }
    if (role == ColumnRole) {
        return m_indexColumn(index);
    }
    if (role == RowRole) {
        return m_indexRow(index);
    }
    return QVariant();
}

bool BitmapModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid()) {
        return false;
    }
    if (role == OnRole) {
        if (value == true && data(index, OnRole) != true) {
            m_bitmap.setBit(m_bitmapIndex(index));
            emit dataChanged(index, index, QVector<int>(Qt::DecorationRole, OnRole));
        }
        else if (value == false && data(index, OnRole) != false) {
            m_bitmap.clearBit(m_bitmapIndex(index));
            emit dataChanged(index, index, QVector<int>(Qt::DecorationRole, OnRole));
        }
    }
    if (role == RowRole || role == ColumnRole) {
        return false;
    }
    return false;
}

bool BitmapModel::insertRows(int row, int count, const QModelIndex &parent) {
    if (count > 0) {
        int oldSize = m_bitmap.size();
        beginInsertRows(parent, row, row + count);
        m_bitmap.resize(oldSize +  count);
        for (int position = oldSize - 1; position >= row; position--)
            m_bitmap[position + count] = m_bitmap[position];
        // TODO test!
        endInsertRows();
    }
    else
        return false;
    return true;
}

void BitmapModel::setColumns(int columns) {
    int rows = m_rows;
    if (columns > 0 && rows <= 0)
        rows = 1;
    m_setDimensions(columns, rows, m_virtualColumns);
}

void BitmapModel::setRows(int rows) {
    int columns = m_columns;
    if (rows > 0 && columns <= 0)
        columns = 1;
    m_setDimensions(columns, rows, m_virtualColumns);
}

void BitmapModel::setVirtualColumns(int virtualColumns) {
    int columns = m_columns;
    int rows = m_rows;
    if (virtualColumns > 0 && columns <= 0)
        columns = 1;
    if (virtualColumns > 0 && rows <= 0)
        rows = 1;
    m_setDimensions(columns, rows, virtualColumns);
}

void BitmapModel::setVirtualVisible(bool visible) {
    // FIXME insertRows()
    if (m_virtualVisible != visible) {
        m_virtualVisible = visible;
        emit virtualVisibleChanged(m_virtualVisible);
        emit columnsChanged(m_bitmap.width());
        //emit dataChanged(m_modelIndex(0, 0), m_modelIndex(m_bitmap.width(), m_bitmap.height()), QVector<int>(Qt::DecorationRole, OnRole));
    }
}

void BitmapModel::clear() {
    beginResetModel();
    setVirtualColumns(0);
    setColumns(0);
    setRows(0);
    setVirtualVisible(false);
    m_bitmap = QBitArray;
    endResetModel();
}

void BitmapModel::drawBit(int column, int row, bool on) {
    if ((on == true && data(m_modelIndex(column, row), OnRole) != true) || (on == false && data(m_modelIndex(column, row), OnRole) != false)) {
        m_getPainter(on).drawPoint(column, row);
        qDebug() << m_indexPoint(m_modelIndex(column, row)) << on << data(m_modelIndex(column, row), OnRole);
        emit dataChanged(m_modelIndex(column, row), m_modelIndex(column, row), QVector<int>(Qt::DecorationRole, OnRole));
    }
}

void BitmapModel::drawColumn(int column, bool on) {
    m_getPainter(on).drawLine(column, 0, column, rows() - 1);
    emit dataChanged(m_modelIndex(column, 0), m_modelIndex(column, rows() - 1), QVector<int>(Qt::DecorationRole, OnRole));
}

void BitmapModel::drawRow(int row, bool on) {
    m_getPainter(on).drawLine(0, row, columns() - 1, row);
    emit dataChanged(m_modelIndex(0, row), m_modelIndex(columns() - 1, row), QVector<int>(Qt::DecorationRole, OnRole));
}

void BitmapModel::drawRect(int topleftcolumn, int topleftrow, int bottomrightcolumn, int bottomrightrow, bool on) {
    m_getPainter(on).drawRect(topleftcolumn, topleftrow, bottomrightcolumn - topleftcolumn, bottomrightrow - topleftrow);
    emit dataChanged(m_modelIndex(topleftcolumn, topleftrow), m_modelIndex(bottomrightcolumn, bottomrightrow), QVector<int>(Qt::DecorationRole, OnRole));
}

void BitmapModel::drawChar4x7(char letter, int column, int row, bool on) {
    QImage letterImage(&font4x7[letter*7], 4, 7, 1, QImage::Format_Mono);
    letterImage.setColor(0, m_offColor.rgba());
    letterImage.setColor(1, m_onColor.rgba());
    m_getPainter(on).drawImage(column, row, letterImage);
    emit dataChanged(m_modelIndex(column, row), m_modelIndex(column + 3, row + 6), QVector<int>(Qt::DecorationRole, OnRole));
}

void BitmapModel::drawChar5x8(char letter, int column, int row, bool on) {
    QImage letterImage(&font5x8[letter*8], 5, 8, 1, QImage::Format_Mono);
    letterImage.setColor(0, m_offColor.rgba());
    letterImage.setColor(1, m_onColor.rgba());
    m_getPainter(on).drawImage(column, row, letterImage);
    emit dataChanged(m_modelIndex(column, row), m_modelIndex(column + 4, row + 7), QVector<int>(Qt::DecorationRole, OnRole));
}

void BitmapModel::drawChar7x9(char letter, int column, int row, bool on) {
    QImage letterImage(&font7x9[letter*9], 7, 9, 1, QImage::Format_Mono);
    letterImage.setColor(0, m_offColor.rgba());
    letterImage.setColor(1, m_onColor.rgba());
    m_getPainter(on).drawImage(column, row, letterImage);
    emit dataChanged(m_modelIndex(column, row), m_modelIndex(column + 6, row + 8), QVector<int>(Qt::DecorationRole, OnRole));
}

void BitmapModel::m_setDimensions(int columns, int rows, int virtualColumns) {
    int oldColumns = m_columns;
    int oldRows = m_bitmap.height();
    int oldVirtualColumns = m_bitmap.width();
    if (columns <= 0 || rows <= 0)
        columns = rows = 0;
    if (virtualColumns < columns)
        virtualColumns = columns;

    m_columns = columns;
    m_bitmap = QImage(virtualColumns, rows, QImage::Format_Alpha8);

    if (m_columns != oldColumns)
        emit columnsChanged(m_columns);
    if (m_bitmap.height() != oldRows)
        emit rowsChanged(m_bitmap.height());
    if (m_bitmap.width() != oldVirtualColumns)
        emit virtualColumnsChanged(m_bitmap.width());
    //emit dataChanged(m_modelIndex(0, 0), m_modelIndex(m_columns, m_bitmap.height()), QVector<int>(Qt::DecorationRole, OnRole));
}

QPainter& BitmapModel::m_getPainter(bool on) {
    if (m_painter.device() != &m_bitmap) {
        if (m_painter.isActive())
            m_painter.end();
        m_painter.begin(&m_bitmap);
        m_painter.setCompositionMode(QPainter::CompositionMode_Source);
    }
    m_painter.setPen(on? m_onColor : m_offColor);
    return m_painter;
}

int BitmapModel::m_bitmapIndex(int column, int row) const {
    // TODO fix
    if (m_virtualVisible) {
        if (column >= 0 && column < virtualColumns() && row >= 0 && row < rows())
            return row * virtualColumns() + column;
        else
            return -1;
    }
    else {
        if (column >= 0 && column < columns() && row >= 0 && row < rows())
            return row * virtualColumns() + column;
        else
            return -1;
    }
}

int BitmapModel::m_bitmapIndex(QModelIndex index) const {
    // TODO
}

QModelIndex BitmapModel::m_modelIndex(int column, int row) const {
    return index(m_bitmapIndex(column, row));
}

int BitmapModel::m_indexColumn(QModelIndex index) const {
    if (m_virtualVisible)
        return index.row() % virtualColumns();
    else
        return index.row() % columns();
}

int BitmapModel::m_indexRow(QModelIndex index) const {
    if (m_virtualVisible)
        return index.row() / virtualColumns();
    else
        return index.row() / columns();
}

QPoint BitmapModel::m_indexPoint(QModelIndex index) const {
    return QPoint(m_indexColumn(index), m_indexRow(index));
}

void BitmapModel::init() {
    /*drawColumn(0);
    drawColumn(2);
    drawColumn(4);
    drawColumn(6);
    drawColumn(8);
    drawColumn(10);
    drawColumn(12);
    drawColumn(14);*/
    //drawRow(4, false);
    //drawColumn(4, false);
    //drawBit(0, 0, false);
    /*drawRow(0);
    drawRow(2);
    drawRow(4);
    drawRow(6);
    drawRow(8);*/
    /*drawChar4x7('H', 0, 2);
    drawChar4x7('a', 4, 2);
    drawChar4x7('l', 8, 2);
    drawChar4x7('l', 12, 2);
    drawChar4x7('o', 16, 2);
    drawChar4x7('!', 20, 2);*/
    drawChar5x8('J', 0, 1);
    drawChar5x8('o', 5, 1);
    drawChar5x8('l', 10, 1);
    drawChar5x8('l', 15, 1);
    drawChar5x8('a', 20, 1);
    /*drawChar7x9('H', 0, 0);
    drawChar7x9('a', 7, 0);
    drawChar7x9('l', 14, 0);
    drawChar7x9('l', 21, 0);
    drawChar7x9('o', 28, 0);
    drawChar7x9('!', 35, 0);*/
    //drawText("Hallo");
}
