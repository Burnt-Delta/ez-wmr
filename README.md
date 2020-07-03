# ez-wmr
A tool to quickly alter settings related to the use of SteamVR with Windows Mixed Reality.

The tool reads the default.vrsettings file and toggles the settings 
between true and false for the following settings:

- thumbstickControlsTurnEnabled

- thumbstickControlsMoveEnabled

- thumbstickTurnSmooth

# Changing the Filepath
On initial startup, the tool assumes a file location in the C:\ directory shown below. 

![filepath on first startup](https://i.imgur.com/IXBygPT.png)

In general, most default.vrsettings files will be in a directory with a similar format to the above shown.
If your WMR driver is in a different location or drive, the browse button can be used to
change the current filepath as well as set it to the default on future startup.

![changing the default filepath](https://i.imgur.com/WQq6I8n.png)

