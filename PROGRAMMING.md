# Programming

## Code Structure

The code is structered in a way that everything belonging to the gui is declared and defined in the _MainWindow_ class. All program logic is defined in the classes contained in `Lib` the main actor being the _Lablib_ class.

For example:
If the _Boot_ button on the gui is clicked, the boot function of the existing _Lablib_ instance is executed. _Lablib_'s boot function checks for all activated clients and calls the boot functions of the Client instances, which make a call to wakeonlan.
=> MainWindow::on_PBBoot_clicked() calls Lablib::boot() calls Client::boot(QString *network_broadcast_address)

For _z-Tree_ itself the logic differs a bit. A click on _Start zTree_ does not directly create a new _z-Tree_ instance. All running _z-Tree_ instances are stored in a std::list of instances of the _Session_ class containing all important informations of the running session. The constructor of these _Session_ instances starts a new _z-Tree_.
