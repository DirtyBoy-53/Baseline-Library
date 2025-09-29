#include "YSerialPort.h"

#include <QMutex>
#include <QWaitCondition>
#include <QElapsedTimer>

// ==========================================================
// YSerialPort::Impl 内部结构：同步机制的核心
// ==========================================================
struct YSerialPort::Impl
{
    // CSerialPort 实例
    std::unique_ptr<CSerialPort> serialPort;

    // 同步读取所需的成员
    QMutex mutex;               // 互斥锁，保护数据和条件
    QWaitCondition waitCondition; // 等待条件，用于阻塞 readData
    QByteArray syncReadBuffer;    // 临时同步读取缓冲区

    // 异步模式标志
    bool isAsynchronous = true; // 默认使用异步模式

    Impl() : serialPort(std::make_unique<CSerialPort>()) {}
};

YSerialPort::YSerialPort(QObject *parent)
    : QObject(parent), m_impl(std::make_unique<Impl>())
{
    // m_impl->serialPort->addListener(this);
}

YSerialPort::~YSerialPort()
{
    // 在析构时移除监听器并关闭串口
    if (m_impl->serialPort) {
        // m_impl->serialPort->removeListener(this);
    }
    close();
}

// ==========================================================
// 核心：同步/异步分流实现 (onReadEvent)
// ==========================================================

void YSerialPort::onReadEvent(const char *portName, unsigned int readBufferLen)
{
    // 读取所有可用数据
    QByteArray receivedData;
    receivedData.resize(static_cast<int>(readBufferLen));

    // 注意：CSerialPort 的 read 方法返回读取的字节数，此处应使用底层 read 方法
    // 假设 CSerialPort 实例的 read 方法可以读取数据到缓冲区
    // 实际实现中，CSerialPort 的 onReadEvent 通常意味着数据已在内部缓冲区
    // 这里我们直接使用 SerialPort.h 提供的 read 接口进行读取

    int bytesRead = m_impl->serialPort->readData(receivedData.data(), readBufferLen);

    if (bytesRead <= 0) {
        return; // 未读到数据或读取失败
    }

    receivedData.resize(bytesRead); // 调整实际读取大小

    if (m_impl->isAsynchronous) {
        // 1. 异步模式：发射信号，数据在另一个线程处理
        emit emitUpdateReceive(receivedData);
        // 2. 继承方法：调用 protected 的 recvMsg 供子类重载处理
        recvMsg(receivedData);
    } else {
        // 3. 同步模式：将数据写入同步缓冲区并唤醒等待线程
        QMutexLocker locker(&m_impl->mutex);
        m_impl->syncReadBuffer.append(receivedData);
        m_impl->waitCondition.wakeOne(); // 唤醒正在等待的 readData 调用
    }
}

// ==========================================================
// 同步接口实现 (readSerialData)
// ==========================================================

QByteArray YSerialPort::readSerialData(int timeOut)
{
    // 切换到同步模式，并保存原有模式以便恢复
    bool originalMode = m_impl->isAsynchronous;
    m_impl->isAsynchronous = false;

    QByteArray result;
    QElapsedTimer timer;
    timer.start();

    // 锁定互斥锁
    QMutexLocker locker(&m_impl->mutex);

    while (m_impl->syncReadBuffer.isEmpty()) {
        qint64 remainingTime = timeOut - timer.elapsed();

        if (remainingTime <= 0) {
            break; // 超时退出
        }

        // 阻塞等待数据到达或超时
        // waitCondition.wait() 会自动释放锁，并在被唤醒时重新获取锁
        if (!m_impl->waitCondition.wait(&m_impl->mutex, remainingTime)) {
            // 如果 wait 返回 false，表示超时
            break;
        }
    }

    // 成功读取到数据 (或超时后读到部分数据)
    result = m_impl->syncReadBuffer;
    m_impl->syncReadBuffer.clear(); // 清空缓冲区以准备下次读取

    // 恢复原始模式
    m_impl->isAsynchronous = originalMode;

    return result;
}

bool YSerialPort::open(const char *portName, int baudRate,
                       Parity parity, DataBits dataBits,
                       StopBits stopbits, FlowControl flowControl,
                       unsigned int readBufferSize)
{
    m_impl->serialPort->init(portName, baudRate, parity, dataBits, stopbits, flowControl, readBufferSize);
    m_impl->serialPort->open();
    return isOpen();
}

bool YSerialPort::isOpen()
{
    return m_impl->serialPort->isOpen();
}

void YSerialPort::close()
{
    return m_impl->serialPort->close();
}

bool YSerialPort::sendData(QByteArray data)
{
    if (!isOpen()) return false;
    int ret{-1};
    ret = m_impl->serialPort->writeData(data.data(), data.size());
    if(ret == -1) return false;
    else return true;
}


QString YSerialPort::getErrorInfo()
{
    return m_impl->serialPort->getLastErrorMsg();
}
// ==========================================================
// 新增：切换模式的辅助函数
// ==========================================================
// 用于在运行时切换同步/异步模式
void YSerialPort::setAsynchronous(bool async) {
    m_impl->isAsynchronous = async;
}

bool YSerialPort::isAsynchronous() const {
    return m_impl->isAsynchronous;
}
