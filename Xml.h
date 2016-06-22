#ifndef XML_H
#define XML_H

#include <QObject>

class Xml : public QObject
{
	Q_OBJECT

public:
	Xml(QObject *parent = 0);
	~Xml();
	void write(QString fileName, QString rootName = "Root");
	QString readText(QString fileName, QString nodeName);
	QString readAttribute(QString fileName, QString nodeName, QString attrName);
	void insertNode(QString fileName, QString parentNodeName, QString nodeName, QString nodeText , QStringList attrNames, QStringList attrs);
	void deleteNode(QString fileName, QString nodeName);
private:
	
};

#endif // XML_H
