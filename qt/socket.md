@startuml

QAbstractSocketEngine <|-- QNativeSocketEngine
QAbstractSocketEngine <|-- QHttpSocketEngine
QObject <|-- QLocalServer
QIODevice <|--  QLocalSocket
QTcpSocket  <|-- QLocalUnixSocket
QAbstractSocket <|--  QTcpSocket 
QAbstractSocket <|-- QUdpSocket
QIODevice  <|--  QAbstractSocket

@enduml