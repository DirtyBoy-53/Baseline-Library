#include "YParamXmlHandle.h"

#include <QDomDocument>
#include <QFile>
#include <QTextStream>

YParamXmlHandle::YParamXmlHandle() {}


//写入XML
bool YParamXmlHandle::writeXML()
{
    QFile file("test.xml");//打开或新建xml文件
    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate))//Truncate表示清空原来的内容
    {
        // QMessageBox::warning(this,"错误","文件打开失败");
        return false;
    }

    QDomDocument doc;
    //写入xml头部
    QDomProcessingInstruction instruction;//添加处理指令
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    //添加根节点
    QDomElement root = doc.createElement("Parameters");
    doc.appendChild(root);
    //添加第一个子节点，及其子元素
    QDomElement book = doc.createElement("book");
    book.setAttribute("id",1);//方法1，创建属性，键值对可以是各种类型
    QDomAttr time = doc.createAttribute("time");//方法2，创建属性，值必须是字符串
    time.setValue("2020/6/3");
    book.setAttributeNode(time);

    QDomElement title = doc.createElement("title");//创建子元素
    QDomText text = doc.createTextNode("C++ primer");//设置括号标签中间的值
    book.appendChild(title);
    title.appendChild(text);
    QDomElement author = doc.createElement("author");//创建子元素
    text = doc.createTextNode("Stanley B.Lippman");
    author.appendChild(text);
    book.appendChild(author);
    root.appendChild(book);



    //添加第二个子节点，部分变量只需重新赋值
    book=doc.createElement("book");
    book.setAttribute("id",2);
    time = doc.createAttribute("time");
    time.setValue("2007/5/25");
    book.setAttributeNode(time);
    title = doc.createElement("title");
    text = doc.createTextNode("Thinking in Java");
    book.appendChild(title);
    title.appendChild(text);
    author = doc.createElement("author");
    text = doc.createTextNode("Bruce Eckel");
    author.appendChild(text);
    book.appendChild(author);
    root.appendChild(book);

    //输出文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4);//缩进4格
    file.close();
}
#if 0
//读XML文件
void YParamXmlHandle::readXML()
{
    QFile file("test.xml");
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"错误","读XML，文件打开失败");
        return;
    }
    QDomDocument doc;
    if(!doc.setContent(&file))//从字节数组中解析XML文档，并将其设置为文档的内容
    {
        file.close();
        return;
    }
    file.close();
    QDomElement root = doc.documentElement();//返回根节点
    qDebug()<<"["<<__FILE__<<"]"<<__LINE__<<__FUNCTION__<<"root.nodeName "<<root.nodeName();//打印根节点
    QDomNode node = root.firstChild();//获得第一个子节点
    while(!node.isNull())//如果节点不为空
    {
        if(node.isElement())//如果节点是元素
        {
            QDomElement e= node.toElement();//节点转换为元素
            //打印键值对，tagName和nodeName相同
            qDebug() << e.tagName()<< " " <<e.attribute("id")<<" "<<e.attribute("time");
            QDomNodeList list = e.childNodes();//子节点列表
            for(int i=0;i<list.count();i++)//遍历子节点
            {
                QDomNode n = list.at(i);
                if(n.isElement())
                    qDebug() << n.nodeName()<<":"<<n.toElement().text();
            }
        }
        node=node.nextSibling();//下一个兄弟节点
    }
}

//增加XML内容
//先把文件读进来，再重写
void YParamXmlHandle::addXML()
{
    QFile file("test.xml");
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"错误","增加XML，文件打开失败1");
        return;
    }
    QDomDocument doc;
    if(!doc.setContent(&file))//从字节数组中解析XML文档，并将其设置为文档的内容
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root=doc.documentElement();
    QDomElement book=doc.createElement("book");
    book.setAttribute("id",3);
    book.setAttribute("time","1813/1/27");
    QDomElement title=doc.createElement("title");
    QDomText text;
    text=doc.createTextNode("Pride and Prejudice");
    title.appendChild(text);
    book.appendChild(title);
    QDomElement author=doc.createElement("author");
    text=doc.createTextNode("Jane Austen");
    author.appendChild(text);
    book.appendChild(author);
    root.appendChild(book);

    if(!file.open(QFile::WriteOnly|QFile::Truncate))//重写文件，如果不用truncate就是在后面追加内容，就无效了
    {
        QMessageBox::warning(this,"错误","增加XML，文件打开失败2");
        return;
    }
    QTextStream out_stream(&file);
    doc.save(out_stream,4);
    file.close();
}

//删减XML内容
void YParamXmlHandle::removeXML()
{
    QFile file("test.xml");
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"错误","增加XML，文件打开失败1");
        return;
    }
    QDomDocument doc;
    if(!doc.setContent(&file))//从字节数组中解析XML文档，并将其设置为文档的内容
    {
        file.close();
        return;
    }
    file.close();
    QDomElement root=doc.documentElement();
    QDomNodeList list = doc.elementsByTagName("book");//指定名称的节点列表
    for(int i=0;i<list.count();i++)
    {
        QDomElement e = list.at(i).toElement();
        if(e.attribute("time")=="2007/5/25")
            root.removeChild(list.at(i));
    }
    if(!file.open(QFile::WriteOnly|QFile::Truncate))//重写文件，如果不用truncate就是在后面追加内容，就无效了
    {
        QMessageBox::warning(this,"错误","删减XML内容，文件打开失败");
        return;
    }
    QTextStream out_stream(&file);
    doc.save(out_stream,4);
    file.close();
}

//更新XML内容
//如果了解XML结构，可以直接定位到指定标签上更新
//或者用遍历的方法去匹配tagname或者attribut，value来更新
void YParamXmlHandle::updateXML()
{
    QFile file("test.xml");
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"错误","更新XML，文件打开失败1");
        return;
    }
    QDomDocument doc;
    if(!doc.setContent(&file))//从字节数组中解析XML文档，并将其设置为文档的内容
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root = doc.documentElement();//获得根节点
    QDomNodeList list = root.elementsByTagName("book");//指定名称的节点列表
    QDomNode node = list.at(list.count()-1).firstChild();//定位到第三个一级子节点的子元素
    QDomNode oldNode = node.firstChild();//标签之间的内容作为节点的子节点出现,当前是Pride and Projudice
    oldNode.setNodeValue("dalao");//修改元素内容
    //    node.firstChild().setNodeValue("diannao");
    //    QDomNode newNode = node.firstChild();
    //    node.replaceChild(newNode,oldNode);

    if(!file.open(QFile::WriteOnly|QFile::Truncate))//重写文件，如果不用truncate就是在后面追加内容，就无效了
    {
        QMessageBox::warning(this,"错误","更新XML内容，文件打开失败2");
        return;
    }
    QTextStream out_stream(&file);
    doc.save(out_stream,4);
    file.close();
}
#endif
