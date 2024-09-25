# Protocol 

This repository is supposed to be a playground for testing ideias on infrastructure code

Should get only filename
Implement filtering
Implement syncronizationr
Move stuff for other classes
Implement uptime counter

LoggerBase:
    declares pure virtual log functions for all Log Levels

Logger:
    has a DomainName
    impl methods for all levels

LoggerAdapter:
    impl static method to access its std::ostream
    
