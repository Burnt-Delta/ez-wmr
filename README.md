# ez-wmr
A tool to quickly alter settings related to the use of SteamVR with Windows Mixed Reality.

Regardless of if joystick movement is supported in a given SteamVR game, this tool allows for an easy method of
toggling artificial joystick movement. Please note that you must be closed out of SteamVR while executing this tool.

The tool reads the default.vrsettings file and toggles 
between true and false the following settings:

- thumbstickControlsTurnEnabled

- thumbstickControlsMoveEnabled

- thumbstickTurnSmooth

These settings can be individually toggled, and your preference will be saved between instances.

# Changing the Filepath
On initial startup, the tool assumes a file location in the C:\ directory shown below. 

![filepath on first startup](https://i.imgur.com/ciL3C85.png)

In general, most default.vrsettings files will be in a directory with a similar format to the above shown.
If your WMR driver is in a different location or drive, the browse button can be used to
change the current filepath as well as set it to the default on future startup.


