#ifndef BITMAPMODEL_H
#define BITMAPMODEL_H

#include <QAbstractListModel>
#include <QBitArray>

/**
 * @brief The BitmapModel class
 *
 * This class provides a 2D model where each element is a single bit.
 * The class is a subclass of the 1D QAbstractListModel but provides the functionality to be used as a 2D model.
 * QImage is used to store the bit information.
 */
class BitmapModel : public QAbstractListModel
{
    Q_OBJECT
public:
    /**
     * @brief BitmapModel constructor
     * @param QObject parent object
     *
     * Creates an empty bitmap with zero elements.
     */
    explicit BitmapModel(QObject *parent = 0);

    /**
     * @brief ~BitmapModel destructor
     *
     * Removes all elements
     */
    virtual ~BitmapModel();

    /**
     * @brief The BitmapRoles enum
     * @see roleNames()
     */
    enum BitmapRoles {
        OnRole = Qt::UserRole + 1,
        ColumnRole = Qt::UserRole + 2,
        RowRole = Qt::UserRole + 3
    };

    /**
     * @param parent    Gets ignored.
     * @return  The number of elements in the model.
     * @note    The function returns the total number of elements, not the number of rows.
     * @see     QAbstractItemModel::rowCount()
     */
    virtual int rowCount(const QModelIndex &parent) const;

    /** @see    AbstractItemModel::data() */
    virtual QVariant data(const QModelIndex &index, int role) const;

    /** @see    QAbstractItemModel::setData() */
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);

    /** @see    QAbstractItemModel::roleNames() */
    QHash<int, QByteArray> roleNames() const;

    /** @brief  The number of columns of the bitmap, including the non visible. */
    Q_INVOKABLE int virtualColumns() const { return m_virtualColumns; }
    Q_INVOKABLE void setVirtualColumns(int virtualColumns);
    Q_PROPERTY(int virtualColumns READ virtualColumns WRITE setVirtualColumns NOTIFY virtualColumnsChanged)

    /** @brief  The number of visible columns of the bitmap. */
    Q_INVOKABLE int columns() const { return m_columns; }
    Q_INVOKABLE void setColumns(int columns);
    Q_PROPERTY(int columns READ columns WRITE setColumns NOTIFY columnsChanged)

    /** @brief  The number of visible rows of the bitmap. */
    Q_INVOKABLE int rows() const { return m_rows; }
    Q_INVOKABLE void setRows(int rows);
    Q_PROPERTY(int rows READ rows WRITE setRows NOTIFY rowsChanged)

    /** @brief  If true, the model shows all columns, including the non visible. */
    Q_INVOKABLE bool virtualVisible() const { return m_virtualVisible; }
    Q_INVOKABLE void setVirtualVisible(bool visible);
    Q_PROPERTY(bool virtualVisible READ virtualVisible WRITE setVirtualVisible NOTIFY virtualVisibleChanged)

    /**
     * @brief   Clear the bitmap.
     * The bitmap is set to an empty array and the columns and rows are set to zero.
     */
    Q_INVOKABLE void clear();

    /**
     * @brief Set a single bit of the bitmap.
     * @param column    The column of the bit to set.
     * @param row       The row of the bit to set.
     * @param on        Either set (true) or unset (false) the bit.
     */
    void drawBit(int column, int row, bool on = true);

    /**
     * @brief Set all bits of a column.
     * @param column    The column of the bits to set.
     * @param on        Either set (true) or unset (false) the bits.
     */
    void drawColumn(int column, bool on = true);

    /**
     * @brief Set all bits of a row.
     * @param row       The row of the bits to set.
     * @param on        Either set (true) or unset (false) the bits.
     */
    void drawRow(int row, bool on = true);

    /**
     * @brief Set all bits of a rectangular area.
     * @param topleftcolumn     The column of the top left bit to set.
     * @param topleftrow        The row of the top left bit to set.
     * @param bottomrightcolumn The column of the bottom right bit to set.
     * @param bottomrightrow    The row of the bottom right bit to set.
     * @param on        Either set (true) or unset (false) the bits.
     */
    void drawRect(int topleftcolumn, int topleftrow, int bottomrightcolumn, int bottomrightrow, bool on = true);

    void drawChar4x7(char letter, int column, int row, bool on = true);
    void drawChar5x8(char letter, int column, int row, bool on = true);
    void drawChar7x9(char letter, int column, int row, bool on = true);
    //void drawText(QString text, bool on = true);

    /** @todo Remove this. */
    Q_INVOKABLE void init();

signals:
    /**
     * @brief virtualColumnsChanged
     * @param columns   The new number of columns of the bitmap, including the non visible columns.
     *
     * This signal get emittet when the number of virtual columns in the bitmap changes.
     */
    void virtualColumnsChanged(int columns);

    /**
     * @brief columnsChanged
     * @param columns   The new number of visible columns of the bitmap.
     *
     * This signal get emittet when the number of visible columns in the bitmap changes.
     */
    void columnsChanged(int columns);

    /**
     * @brief rowsChanged
     * @param rows      The new number of visible rows of the bitmap.
     *
     * This signal get emittet when the number of visible rows in the bitmap changes.
     */
    void rowsChanged(int rows);

    /**
     * @brief virtualVisibleChanged
     * @param visible   True if all columns, including the non visible are represented by the model.
     */
    void virtualVisibleChanged(bool visible);

public slots:

private:
    /**
     * @brief The bitmap.
     * The pixels of this QImage represent the bits of the bitmap.
     * The Format ist QImage::Format_Mono, therefore Qt::color0 and Qt::color1 represent the state of the bits.
     */
    QBitArray m_bitmap;

    int m_virtualColumns;
    int m_columns;
    int m_rows;
    bool m_virtualVisible;

    /**
     * @brief Set the dimensions of the bitmap.
     * @param columns           The number of visible columns.
     * @param rows              The number of visible rows.
     * @param virtualColumns    The number of columns including the non visible. Is equal to columns if set to less than columns.
     */
    void m_setDimensions(int columns, int rows, int virtualColumns = -1);

    /**
     * @brief Get the index of the model.
     * @param column    The column of the bit.
     * @param row       The row of the bit.
     * @return          The index of the bit inside the model.
     */
    int m_bitmapIndex(int column, int row) const;

    /**
     * @brief Get the index of the model.
     * @param index The index of the model.
     * @return      The index of the bit inside the model.
     */
    int m_bitmapIndex(QModelIndex index) const;

    /**
     * @brief m_modelIndex
     * @param column    The column of the bit.
     * @param row       The row of the bit.
     * @return          The index of the bit inside the model.
     */
    QModelIndex m_modelIndex(int column, int row) const;

    /**
     * @brief Get the column of the index.
     * @param index     The index of the model.
     * @return          The column.
     */
    int m_indexColumn(QModelIndex index) const;

    /**
     * @brief Get the row of the index.
     * @param index     The index of the model.
     * @return          The row.
     */
    int m_indexRow(QModelIndex index) const;

    /**
     * @brief Get the point of the index.
     * @param index     The index of the model.
     * @return          The point.
     */
    QPoint m_indexPoint(QModelIndex index) const;
};

#endif // BITMAPMODEL_H
