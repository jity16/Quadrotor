## `Drone Grapper`  Daily Log



> 2019/07/02

#### `RemoteAPI Calling Process`

* CS，19997 port

* default port setting：remoteApiConnections.txt

---



> 2019/07/03

#### `Calculation module properties dialog`

* Location
  * [Menu bar --> Tools --> Calculation module properties]
  * toolbar button `f(x)`
* four parts
  * Collison detection
  * Distance calculation
  * Inverse kinematics
  * Dynamics



#### `BubbleRob`

* Distance Calculation
  * minimum distance calculation module
  * enable: `Collidabel ,Measurable, Renderable ,Detectable`(object common properties)
* Proximity Sensor
  * [Menu bar --> Add --> Proximity sensor --> Cone type]
* Add joints for the wheels
* Collections
* Graph Object
  * attach graph
  * set-up a 3D curve
* vision sensor



#### `Drone Drapper`

* get-version
* PID 
* Vertical Control
* thrust

---





> 2019/07/05

#### `Line Following BubbleRob tutorial`

vision sensors

---





> 2019/07/15

1. window PX4
2. `jMAVSim`: Simulation

---



> 2019/07/16

* **V-rep**
  * install` CLion`,
  * make and build V-rep Landing project

* **PX4**

  * `make px4_sitl jmavsim`

    * ==Problem== : cannot show the sky scene

    * find console error: `ERROR [uorb] telemetry_status advertise failed`

      * quickly check a few things:  `issues/12265`

        ~~~c
        mavlink status
        listener telemetry_status
        uorb top -1
        ~~~

    

  * `User Commands`

    * **take off**: `commander takeoff

      

  * `Windows CR+LF`

    ~~~c
    git config core.autocrlf false
    git config core.eol lf
    ~~~

    

  * `Unix Permissions Execution Bit`

    * globally disabling the permission check on Windows to avoid the problem

      ~~~c
      git config --global core.fileMode false # disable execution bit check globally for the machine
      ~~~

      



