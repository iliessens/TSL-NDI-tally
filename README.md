# TSL to NDI tally converter

Convert TSL 3.1 messages to NDI sources.  
Can be used in combination with [Tally Arbiter](https://github.com/josephdadams/TallyArbiter) to send Tally from Blackmagic ATEM or other hardware video mixers into NDI tally signals.  

The software can convert multiple signals at the same time. Only the metadata from the NDI stream is received, so the load on the computer and network is kept low.

This software is still in an alpha state. **Use at you own risk.**

### Example signal flow
```
+------------+       +---------------+      +-------------+     +--------------+
|            |       |               |      +-------------+     | Tally on     |
|Video mixer +------>| Tally arbiter +----->| TSL to NDI  +---->| NDI devices  |
|            |       |               |      +-------------+     |              |
+------------+       +---------------+      +-------------+     +--------------+
```

## Installation
The project was made using Visual Studio Code 2019. Due to the use of Windows API's, other operating systems are currently not supported.  

The only dependency is the *NDI runtime V4*. You can [download the installer from Newtek](http://new.tk/NDIRedistV4).  

A precompiled executable is available in the *releases* section.

## Configuration
Configure your TSL sender to send to the IP of your PC (beware of the Windows firewall).  
The application currently listens on UDP port 8888. TCP is not implemented.  

The mapping between the TSL ID's and the NDI source names is made in the file *tallymap.txt*.  
This should be located in the running directory of the application.  

The format of the file is quite simple. It is just the TSL ID and the NDI name, separated by a semicolon.

An example configuration file:
```
1:PC (Test Pattern (2))
2:PC (Test Pattern)
```

Note that the NDI source names must be exactly right. These are used to discover the sources on the network.  

## Running
Double click te appropriate executable for your system (x86 or x64). A prompt will open to display status information and loggging.  

Should the prompt crash too quick to read the status information, you can start the .exe from a command prompt.

## FAQ
* *Why do not use the TSL labels for mapping to the NDI source names?*  
As the TSL protocol is limited to 16 characters, this might often be not enough to fully specify the NDI name. Additionally, specifying the mapping separate provides more flexibility.

## License
The code is distributed under the MIT license.  

The NDI headers are distributed under the same license by Newtek. Check the license header in the include files for more information.  