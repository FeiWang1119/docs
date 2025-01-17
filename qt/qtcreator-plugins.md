@startuml

QObject <|-- IPlugin 
IPlugin <|-- CorePlugin
IPlugin <|-- GitPlugin

@enduml