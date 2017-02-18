#ifndef BITMAPMODEL_H
#define BITMAPMODEL_H

#include <QAbstractListModel>
#include <QBitArray>

/**
 * @brief The BitmapModel class
 *
 * This class provides a 2D model where each element is a single bit.
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
        RowRole = Qt::UserRole + 1,
        ColumnRole = Qt::UserRole + 2,
        BitOnRole = Qt::UserRole + 3
    };

    /**
     * @param parent    Gets ignored.
     * @return  The number of elements in the model.
     * @note    The function returns the total number of elements, not the number of rows.
     * @see     QAbstractItemModel::rowCount()
     */
    virtual int rowCount(const QModelIndex &parent) const { return m_bitmap.size(); }

    /** @see    AbstractItemModel::data() */
    virtual QVariant data(const QModelIndex &index, int role) const;

    /** @see    QAbstractItemModel::setData() */
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);

    /** @see    QAbstractItemModel::insertRows() */
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());

    /** @see    QAbstractItemModel::roleNames() */
    QHash<int, QByteArray> roleNames() const;

    /**
     * @brief Set the dimensions of the bitmap.
     * @param rows      The number of rows.
     * @param columns   The number of columns.
     * @note  This function resets the bitmap. All elements are unset (false).
     */
    Q_INVOKABLE void setDimensions(int rows, int columns);

    /** @brief  The number of rows of the bitmap. */
    Q_INVOKABLE int rows() { return m_rows; }
    Q_INVOKABLE void setRows(int rows);
    Q_PROPERTY(int rows READ rows WRITE setRows NOTIFY rowsChanged)

    /** @brief  The number of columns of the bitmap. */
    Q_INVOKABLE int columns() { return m_columns; }
    Q_INVOKABLE void setColumns(int columns);
    Q_PROPERTY(int columns READ columns WRITE setColumns NOTIFY columnsChanged)

    /**
     * @brief Insert rows to the model.
     * @param position  The new rows are inserted before this row.
     * @param count     The number of rows to insert.
     * @return The total number of rows after ther insertion.
     */
    Q_INVOKABLE int insertRow(int position, int count = 1);

    /**
     * @brief Append rows to the end of the model.
     * @param count     The number of rows to append.
     * @return The total number of rows after ther insertion.
     */
    Q_INVOKABLE int appendRow(int count = 1);

    /**
     * @brief Insert columns to the model.
     * @param position  The new columns are inserted before this column.
     * @param count     The number of columns to insert.
     * @return The total number of columns after ther insertion.
     */
    Q_INVOKABLE int insertColumn(int position, int count = 1);

    /**
     * @brief Append columns to the end of the model.
     * @param count     The number of columns to append.
     * @return The total number of columns after ther insertion.
     */
    Q_INVOKABLE int appendColumn(int count = 1);

    /**
     * @brief Set a single bit of the bitmap.
     * @param row       The row of the bit to set.
     * @param column    The column of the bit to set.
     * @param on        Either set (true) or unset (false) the bit.
     */
    Q_INVOKABLE void setBit(int row, int column, bool on = true);

    /**
     * @brief Set multiple bits of the bitmap.
     * @param bit       A vector containing the coordinates of the bits. The first value of the QPair represents the row. The second value of the QPair represents the column.
     * @param on        Either set (true) or unset (false) the bits.
     */
    Q_INVOKABLE void setBit(QVector<QPair<int,int>> bit, bool on = true);

    /**
     * @brief Set all bits of a row.
     * @param row       The row of the bits to set.
     * @param on        Either set (true) or unset (false) the bits.
     */
    Q_INVOKABLE void setRow(int row, bool on = true);

    /**
     * @brief Set all bits of a column.
     * @param column    The column of the bits to set.
     * @param on        Either set (true) or unset (false) the bits.
     */
    Q_INVOKABLE void setColumn(int column, bool on = true);

    /**
     * @brief Write a character to the bitmap.
     * @param row       The row of the top left point of the character.
     * @param column    The column of the top left point of the character.
     * @param resize    Resize the bitmap if the character does not fit.
     * @return  true if the character has been added.
     *          false if the character has not been added, because it either is unknown or does not fit into the bitmap.
     */
    Q_INVOKABLE bool setChar8x8(int row, int column, QChar character, bool resize = true);

    /**
     * @brief Write a text to the bitmap.
     * @param row       The row of the top left point of the text.
     * @param column    The column of the top left point of the text.
     * @param resize    Resize the bitmap if the text does not fit.
     * @return  true if the text has been added.
     *          false if the tet has not been added, because it either is unknown or does not fit into the bitmap.
     */
    Q_INVOKABLE bool setString8x8(int row, int column, QString text, bool resize = true);

    /** @todo Remove this. */
    Q_INVOKABLE void init();

signals:
    /**
     * @brief rowsChanged
     * @param rows      The new number of rows of the bitmap.
     *
     * This signal get emittet when the number of rows in the bitmap changes.
     */
    void rowsChanged(int rows);

    /**
     * @brief columnsChanged
     * @param columns   The new number of columns of the bitmap.
     *
     * This signal get emittet when the number of columns in the bitmap changes.
     */
    void columnsChanged(int columns);

public slots:

private:
    /**
     * @brief The data of the model.
     *
     * Each element of the array represents an element of the model.
     */
    QBitArray m_bitmap;

    /**
     * @brief m_rows
     *
     * Contains the number of rows of the bitmap.
     */
    int m_rows;

    /**
     * @brief m_columns
     *
     * Contains the number of columns of the bitmap.
     */
    int m_columns;

    /**
     * @brief m_bitmapIndex
     * @param row       The row of the element.
     * @param column    The column of the element.
     * @return          The index of the element in the model.
     *
     * This function converts the coordinates of an element in the bitmap to the index in the model.
     */
    int m_bitmapIndex(int row, int column) const;

    /**
     * @brief m_modelIndex
     * @param row       The row of the element.
     * @param column    The column of the element.
     * @return          The index of the element in the model.
     *
     * This function converts the coordinates of an element in the bitmap to the index in the model.
     */
    QModelIndex m_modelIndex(int row, int column) const;
};

#endif // BITMAPMODEL_H
