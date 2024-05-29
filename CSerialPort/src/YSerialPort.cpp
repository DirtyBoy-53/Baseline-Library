#include "CSerialPort/YSerialPort.h"
#include <QDebug>

struct YSerialPort::Impl{
    CSerialPort m_serialPort;

    char m_buf[1024]{0};
};


YSerialPort::YSerialPort(QObject *parent)
    : QObject(parent)
    , m_impl(std::make_unique<Impl>())
{
    m_impl->m_serialPort.connectReadEvent(this);
}

YSerialPort::~YSerialPort() { }

std::vector<SerialPortInfo> YSerialPort::getSerialPortInfo()
{
    return CSerialPortInfo::availablePortInfos();
}

bool YSerialPort::connect(const char *portName, int baudRate,
                       Parity parity, DataBits dataBits,
                       StopBits stopbits, FlowControl flowControl,
                       unsigned int readBufferSize)
{
    m_impl->m_serialPort.init(portName, baudRate, parity, dataBits, stopbits, flowControl, readBufferSize);
    m_impl->m_serialPort.open();
    return isOpen();
}

bool YSerialPort::isOpen()
{
    return m_impl->m_serialPort.isOpen();
}

void YSerialPort::close()
{
    return m_impl->m_serialPort.close();
}

bool YSerialPort::sendData(QByteArray data)
{
    int ret{-1};
    ret = m_impl->m_serialPort.writeData(data.data(), data.size());
    if(ret == -1) return false;
    else return true;
}

QString YSerialPort::getErrorInfo()
{
    return m_impl->m_serialPort.getLastErrorMsg();
}

void YSerialPort::onReadEvent(const char *portName, unsigned int readBufferLen)
{
    Q_UNUSED(portName)
    if(readBufferLen > 0){
        memset(m_impl->m_buf, 0, sizeof(m_impl->m_buf));
        QByteArray buf;
        int len = m_impl->m_serialPort.readData(m_impl->m_buf, readBufferLen > 1023 ? 1023 : readBufferLen);
        if(len > 0){
            auto sigConNum = receivers(SIGNAL(emitUpdateReceive(QByteArray)));
            if(sigConNum > 0 ){
                buf.resize(len);
                memcpy(buf.data(),m_impl->m_buf,len);
                emitUpdateReceive(buf);
            }else{
                qInfo() << QString("Unable to send serial data because there is no signal slot connection.[%1]").arg(sigConNum);
            }
        }
    }
}
