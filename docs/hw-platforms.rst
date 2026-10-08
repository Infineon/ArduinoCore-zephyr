Supported Boards
******************

KIT-PSE84-AI
=============

The `Infineon KIT-PSE84-AI <https://www.infineon.com/evaluation-board/kit-pse84-ai>`_
(PSOC™ Edge E84) is the first supported board for the PSOC™ Edge family.

- FQBN: ``infineon:zephyr_pse84:kit_pse84_ai``
- Silicon: PSE846GPS2DBZC4A

.. image:: img/KIT_PSE84_AI_Pinout.svg
   :width: 400

Macros
------

For the exact macro definitions, refer to
``variants/kit_pse84_ai_pse846gps2dbzc4a_m33/variant.h``. General rules:

- Every exposed GPIO pin from the pinout has a macro identical to the
  documentation name, e.g. ``P17_3``, ``P16_0``, ``P15_3``, etc.
- Digital pins have a macro in the format ``Dx``, where ``x`` is the index of
  the Arduino pin in the range [0, 47], e.g. ``D0``, ``D15``, ``D47``, etc.
- Analog pins have a macro in the format ``Ax``, where ``x`` is the index of
  the Arduino pin in the range [0, 15], e.g. ``A0``, ``A1``, ``A15``, etc.
- Onboard LED and button macros are:

  - ``LED_BUILTIN`` / ``LED_BUILTIN_1`` / ``LED_BUILTIN_2``: the onboard LEDs.
  - ``LED_BUILTIN_ACTIVE``: the logic level that turns an onboard LED on.
  - ``LED_RED`` / ``LED_GREEN`` / ``LED_BLUE``: the individual channels of
    the onboard RGB LED.
  - ``BTN_BUILTIN``: the onboard user button (``SW1``).

- Expansion header pins include the macros as printed on the board:
  ``SERIAL_INTx`` where ``x`` is the index in the range [0, 3], e.g.
  ``SERIAL_INT0``.
