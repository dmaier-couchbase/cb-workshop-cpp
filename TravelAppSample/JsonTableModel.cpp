#include "JsonTableModel.h"
#include <QJsonObject>

JsonTablemodel::JsonTablemodel(QObject *parent)
	: BaseClass_t(parent)
{

}

void JsonTablemodel::setData(QList<QJsonObject> data)
{
	beginResetModel();
	mData = data;
	endResetModel();
}

void JsonTablemodel::addMapping(QString key, QString header)
{
    mMapping << key;
    mHeader << header;
}

QVariant JsonTablemodel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && section >= 0 && section < mHeader.count() && role == Qt::DisplayRole)
    {
        return mHeader[section];
    }
    return QVariant();
}

int JsonTablemodel::rowCount(const QModelIndex &parent) const
{
	return mData.count();
}

int JsonTablemodel::columnCount(const QModelIndex &parent) const
{
	return mMapping.count();
}

QVariant JsonTablemodel::data(const QModelIndex &index, int role) const
{
	if (index.row() >= mData.count() || index.column() >= mMapping.count())
	{
		return QVariant();
	}

	if (role == Qt::DisplayRole || role == Qt::EditRole)
	{
		QJsonObject object = mData.at(index.row());
		QString name = mMapping.at(index.column());
        QJsonValue value = object[name];
        return value.toVariant();
	}
	return QVariant();
}
