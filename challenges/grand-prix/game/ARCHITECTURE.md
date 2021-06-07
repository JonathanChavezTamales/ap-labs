# Architecture

The architecture of this project consists of two structs:
- Race
- Car

### Race

Race is the status of the game, it means that this state represents the physical world around.
It's members are the following:

- name
- laps
- turns
- lapDistance
- time
- results

### Car

Car is the skeleton of the project, you can add as many as you want.
It has the following members:

- Driver
- Team
- Lane
- GearSpeeds
- CurrentGear
- CurrentDistance
- Revs
- FinishTime
- Finished

## Threading

This project uses a thread for each Car, even if the car did not enter the race.

They make part in a WaitGroup.

They are all synchronized with the `globalTime` which is a global variable that helps keep track of the time, otherwise, the performance of each car will be dependent on the performance of the thread.

