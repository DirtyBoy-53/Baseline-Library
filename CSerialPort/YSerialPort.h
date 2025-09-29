#ifndef YSERIALPORT_H
#define YSERIALPORT_H

#include <QObject>
#include <memory>

#include "CSerialPort/SerialPort.h"
#include "CSerialPort/SerialPortInfo.h"

using namespace itas109;

class YSerialPort : public QObject, public CSerialPortListener
{
    Q_OBJECT
public:
    explicit YSerialPort(QObject *parent = nullptr);
    ~YSerialPort();
    // 发送数据
    bool sendData(QByteArray data);

    // 读取数据
    QByteArray readSerialData(int timeOut=1000);

    // 读取串口列表
    std::vector<SerialPortInfo> getSerialPortInfo();

    // 连接串口
    bool open(const char *portName,
              int baudRate = itas109::BaudRate9600,
              itas109::Parity parity = itas109::ParityNone,
              itas109::DataBits dataBits = itas109::DataBits8,
              itas109::StopBits stopbits = itas109::StopOne,
              itas109::FlowControl flowControl = itas109::FlowNone,
              unsigned int readBufferSize = 4096) ;

    // 串口是否打开
    bool isOpen();

    // 串口关闭
    void close();

    // 获取错误信息
    QString getErrorInfo();

    // CSerialPortListener interface
    void setAsynchronous(bool async);
    bool isAsynchronous() const;
protected:
    // 信号槽方式：emitUpdateReceive 连接后可接受数据，  重载本方法：不处理,则可以同步方式读取
    virtual void onReadEvent(const char *portName, unsigned int readBufferLen) override;

    // 继承后使用该方法回读
    virtual void recvMsg(QByteArray data){}


signals:
    void emitUpdateReceive(QByteArray data);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

#endif // YSERIALPORT_H
