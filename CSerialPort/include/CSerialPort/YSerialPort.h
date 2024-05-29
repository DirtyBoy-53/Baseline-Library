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
    bool sendData(QByteArray data);
    std::vector<SerialPortInfo> getSerialPortInfo();
    bool connect(const char *portName,
              int baudRate = itas109::BaudRate9600,
              itas109::Parity parity = itas109::ParityNone,
              itas109::DataBits dataBits = itas109::DataBits8,
              itas109::StopBits stopbits = itas109::StopOne,
              itas109::FlowControl flowControl = itas109::FlowNone,
              unsigned int readBufferSize = 4096) ;

    bool isOpen();
    void close();
    QString getErrorInfo();



    // CSerialPortListener interface
protected:
    virtual void onReadEvent(const char *portName, unsigned int readBufferLen) override;

signals:
    void emitUpdateReceive(QByteArray data);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

#endif // YSERIALPORT_H
