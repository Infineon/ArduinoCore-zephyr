************************************
Arduino Core for Infineon PSOC™
************************************

This project brings Arduino support to selected Infineon boards powered by
Zephyr. See :doc:`Supported Boards <hw-platforms>` for an overview of
currently supported boards.

Looking for Arduino support for other Infineon microcontrollers? See:

- `PSOC™ 6 for Arduino <https://github.com/Infineon/arduino-core-psoc6>`_
- `XMC™ for Arduino <https://github.com/Infineon/XMC-for-Arduino>`_

Architecture
============

.. graphviz:: img/architecture.dot

Unlike native Arduino cores (e.g. for AVR or SAMD boards), where the Arduino
APIs talk almost directly to the microcontroller, this core runs on top of
Zephyr RTOS. Zephyr provides the kernel, drivers, devicetree-based build
system, and board support; the Arduino Core for Zephyr exposes those
capabilities through familiar Arduino APIs.

.. toctree::
   :maxdepth: 2
   :caption: CONTENT

   Introduction<self>
   installation-instructions
   hw-platforms

.. toctree::
    :maxdepth: 1
    :caption: CONTRIBUTING
    :hidden:

    Introduction <contributing>
    env-setup
    run-tests

License
=======

Please find the license file for this core `here
<https://github.com/Infineon/ArduinoCore-zephyr/blob/main/LICENSE>`_.
