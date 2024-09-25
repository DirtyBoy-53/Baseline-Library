- 1.包含CSerialPort.pri
- 2.创建SerialHandler类，继承YSerialPort
- 3.根据需要，重写YSerialPort的成员函数
  - 3.1. （异步）通过信号槽接收数据：连接 emitUpdateReceive信号到槽函数即可
  - 3.2. （异步）通过固定接口接收数据：重写recvMsg函数
  - 3.3. （同步）通过固定接口读取数据：readData函数
  - - ----
- 4.YSerialPort接口介绍
  4.1. 连接
   bool connect(const char *portName,
              int baudRate = itas109::BaudRate9600,
              itas109::Parity parity = itas109::ParityNone,
              itas109::DataBits dataBits = itas109::DataBits8,
              itas109::StopBits stopbits = itas109::StopOne,
              itas109::FlowControl flowControl = itas109::FlowNone,
              unsigned int readBufferSize = 4096) ;
    形参：
        串口号：portName
        波特率：baudRate
        校验位：parity
        数据位：dataBits
        停止位：stopbits
        流控制：flowControl
        缓冲区大小：readBufferSize
    返回值：连接成功返回true，否则返回false
    解释：连接串口（同时串口初始化）

    4.2. 断开连接
    void close()
    解释：断开串口连接

    4.3. 串口是否连接
    bool isOpen()
    返回值：打开返回true，否则返回false
    解释：判断串口是否打开

    4.4. 发送数据
    bool sendData(QByteArray data)
    参数：data：待发送数据
    返回值：发送成功返回true，否则返回false
    解释：发送数据

    4.5. 读取数据
    QByteArray readData()
    返回值：读取到的数据
    解释：读取数据

    4.6. 获取错误信息
    QString getErrorInfo()
    返回值：错误信息
    解释：获取错误信息

    4.7. 接收数据
    virtual void recvMsg(QByteArray data)
    参数：data：接收到的数据
    解释：当有数据时，并且没有连接emitUpdateReceive信号到指定槽函数时，会自动调用该函数，使用者可以重写该函数，就可以通过该方法接收数据。

    4.8. 读取事件
    virtual void onReadEvent(const char *portName, unsigned int readBufferLen) override
    参数：portName：串口号
          readBufferLen：读取到的数据长度
    解释：读取事件，当有数据到达时，会自动调用该函数，如果有连接 emitUpdateReceive信号到指定槽函数，就是自动发送数据到指定槽函数。如果没有连接则将数据通过recvMsg发送。
    注意：如果重写该事件，那么recvMsg和信号槽都不会自动调用，需要使用者通过readData方法手动读取数据。

    4.9. 数据信号
    void emitUpdateReceive(QByteArray data)
    参数：data：接收到的数据
    解释：当有数据到达时，并且有连接 emitUpdateReceive信号到指定槽函数，就会自动发送该信号，使用者可以连接该信号到指定槽函数，就可以通过该信号接收数据。

### 同步读取数据案例
SerialHandle.h
` 
#include <CSerialPort/YSerialPort.h>

class SerialHandle : public YSerialPort
{
public:
    SerialHandle();

    // CSerialPortListener interface
public:
    virtual void onReadEvent(const char *portName, unsigned int readBufferLen) override{}
};
`
### 异步读取数据案例
可以重写recvMsg方法，也可以通过信号槽接收数据
SerialHandle.h
` 
#include <CSerialPort/YSerialPort.h>

class SerialHandle : public YSerialPort
{
public:
    SerialHandle();

    // YSerialPort interface
protected:
    virtual void recvMsg(QByteArray data) override;
    
};
`
