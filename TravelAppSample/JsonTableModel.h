#pragma once

#include <QAbstractTableModel>

class JsonTablemodel : public QAbstractTableModel
{
	Q_OBJECT

public:
	explicit JsonTablemodel(QObject *parent = 0);

	void setData(QList<QJsonObject>);
    void addMapping(QString key, QString header);

    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const override;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;


private:
	typedef QAbstractTableModel BaseClass_t;
	QList<QJsonObject> mData;
	QList<QString> mMapping;
    QList<QString> mHeader;
};
