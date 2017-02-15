#ifndef FONTBITMAP_H
#define FONTBITMAP_H

#include <QAbstractListModel>
#include <QBitArray>

class FontBitmap : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FontBitmap(QObject *parent = 0);
    virtual ~FontBitmap() { }

    enum BitmapRoles {
        RowRole = Qt::UserRole + 1,
        ColumnRole = Qt::UserRole + 2,
        BitOnRole = Qt::UserRole + 3
    };

    virtual int rowCount(const QModelIndex &parent) const { return m_matrix.size(); }
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    QHash<int, QByteArray> roleNames() const;

    Q_PROPERTY(int rows READ rows WRITE setRows NOTIFY rowsChanged)
    Q_PROPERTY(int columns READ columns WRITE setColumns NOTIFY columnsChanged)
    Q_INVOKABLE int rows() { return m_rows; }
    Q_INVOKABLE int columns() { return m_columns; }
    Q_INVOKABLE int insertRow(int position, int count = 1);
    Q_INVOKABLE int insertColumn(int position, int count = 1);
    //Q_INVOKABLE int appendRow(int count = 1);
    //Q_INVOKABLE int appendColumn(int count = 1);
    Q_INVOKABLE void setDimensions(int rows, int columns);
    Q_INVOKABLE void setRows(int rows);
    Q_INVOKABLE void setColumns(int columns);
    Q_INVOKABLE void setBit(int row, int column, bool on = true);
    Q_INVOKABLE void setBit(QVector<QPair<int,int>> bit, bool on = true);
    Q_INVOKABLE void setRow(int row, bool on = true);
    Q_INVOKABLE void setColumn(int column, bool on = true);
    Q_INVOKABLE void init();

signals:
    void rowsChanged(int rows);
    void columnsChanged(int columns);

public slots:

private:
    int m_matrixIndex(int row, int column) const;
    QModelIndex m_modelIndex(int row, int column) const;
    QBitArray m_matrix;
    int m_rows;
    int m_columns;
};

#endif // FONTBITMAP_H
