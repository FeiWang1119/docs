@startuml
QPlatformSurface <|-- QPlatformWindow 
QPlatformWindow <|-- QXcbWindow
QXcbWindow <|-- DForeignPlatformWindow
class QXcbWindow
note left: QNativeWindow

QSurface <|-- QWindow 
QWindow <|-- QWidgetWindow
@enduml
