#include "Xml.h"
#include <QFile>
#include <QIODevice>
#include <QDomDocument>
#include <QDomProcessingInstruction>
#include <QDomElement>
#include <QDomText>
#include <QDomAttr>
#include <QDomNode>
#include <QTextStream>

Xml::Xml(QObject *parent)
	: QObject(parent)
{

}

Xml::~Xml()
{

}

void Xml::write(QString fileName, QString rootName)
{
	QDomDocument doc;
	QDomProcessingInstruction introduction = doc.createProcessingInstruction("xml", "version=\'1.0\' encoding=\'UTF-8\'");
	doc.appendChild(introduction);

	QDomElement root = doc.createElement(rootName);
	doc.appendChild(root);

	QDomElement node = doc.createElement("Notes");
	root.appendChild(node);

	node.setNodeValue("Good");

	QDomText node_text = doc.createTextNode("Text");
	node.appendChild(node_text);

	//QDomAttr node_attr = doc.createAttribute("Attribute");
	node.setAttribute("Attribute", "Cool");

	QFile file(fileName);

	if (!file.exists())
	{
		file.open(QIODevice::WriteOnly);
		file.close();
	}

	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) return;

	QTextStream stream(&file);
	stream.setCodec("UTF-8");
	doc.save(stream, 4);
	file.close();
}

QString Xml::readText(QString fileName, QString nodeName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return QString("");

	QDomDocument doc;
	if (!doc.setContent(&file)) return QString("");

	file.close();

	QDomElement currentNode = doc.documentElement();
	QDomElement lastNode = currentNode.lastChildElement();
	currentNode = currentNode.firstChildElement();
	while(currentNode.tagName() != nodeName)
	{
		currentNode = currentNode.nextSiblingElement();
		if (currentNode == lastNode) return QString("");
	}

	return currentNode.text();
}

QString Xml::readAttribute(QString fileName, QString nodeName, QString attrName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return QString("");

	QDomDocument doc;
	if (!doc.setContent(&file)) return QString("");

	file.close();

	QDomElement currentNode = doc.documentElement();
	QDomElement lastNode = currentNode.lastChildElement();
	currentNode = currentNode.firstChildElement();
	while(currentNode.tagName() != nodeName)
	{
		currentNode = currentNode.nextSiblingElement();
		if (currentNode == lastNode) return QString("");
	}

	if (!currentNode.hasAttribute(attrName)) return  QString("");

	return currentNode.attribute(attrName);
}

void Xml::insertNode(QString fileName, QString parentNodeName, QString nodeName, QString nodeText, QStringList attrNames, QStringList attrs)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) return;

	QDomDocument doc;
	if (!doc.setContent(&file)) return;

	QDomElement currentNode = doc.documentElement();
	if (!parentNodeName.isEmpty())
	{
		currentNode = currentNode.firstChildElement();
		while(currentNode.tagName() != parentNodeName)
		{
			currentNode = currentNode.nextSiblingElement();
		}
	}

	QDomElement newNode =doc.createElement(nodeName);
	currentNode.appendChild(newNode);

	if (!nodeText.isEmpty())
	{
		QDomText newTextNode = doc.createTextNode(nodeText);
		newNode.appendChild(newTextNode);
	}

	if (attrNames.size() > 0)
	{
		for (int i = 0; i < attrNames.size(); ++i)
		{
			if (attrs.size() > 0)
			{
				newNode.setAttribute(attrNames[i], attrs[i]);
			}
			else
			{
				newNode.setAttribute(attrNames[i], "");
			}
		}
	}

	file.resize(0);
	QTextStream stream(&file);
	stream.setCodec("UTF-8");
	doc.save(stream, 4);
	file.close();
}

void Xml::deleteNode(QString fileName, QString nodeName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) return;

	QDomDocument doc;
	if (!doc.setContent(&file)) return;

	QDomElement rootNode = doc.documentElement();
	if (rootNode.tagName() != nodeName)
	{
		QDomElement currentNode = rootNode.firstChildElement();
		QDomElement lastNode = rootNode.lastChildElement();
		while(currentNode.tagName() != nodeName)
		{
			if (lastNode == currentNode) return;
			currentNode = currentNode.nextSiblingElement();
			QString name = currentNode.tagName();
		}
		rootNode.removeChild(currentNode);
	}
	else
	{
		doc.removeChild(rootNode);
	}

	file.resize(0);
	QTextStream stream(&file);
	stream.setCodec("UTF-8");
	doc.save(stream, 4);
	file.close();
}