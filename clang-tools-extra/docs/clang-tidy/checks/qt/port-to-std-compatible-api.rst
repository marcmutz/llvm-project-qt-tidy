.. title:: clang-tidy - qt-port-to-std-compatible-api

qt-port-to-std-compatible-api
===========================================

Detects uses of non-std-compatible API of Qt classes and ports to the
std-compatible API subset, for easier replacement with std components,
such as ``std::unique_ptr`` and ``std::shared_ptr``.

Currently fixes the following cases:

- ``data()`` -> ``get()`` on ``QScopedPointer``, ``QPointer``, ``QSharedPointer``
- ``clear()`` -> ``reset()`` on ``QPointer``, ``QSharedPointer``
- ``toStrongRef()`` -> ``lock()`` on ``QWeakPointer``

Options
-------

.. option:: QtTarget

   The target Qt version, in ``QT_VERSION`` format (``0xMMmmPP``,
   where ``MM`` denotes the major, ``mm`` the minor version number,
   and ``PP`` the patch level. The default is ``0x06FFFF``, which
   means "latest", until Qt 7.0 is released. This check will not
   attempt to port to API added since `QtTarget`.

.. option:: Scope

   Semicolon-separated list of scopes. The only available scope at the
   moment is ``SmartPtr``, which is also the default.

.. option:: QtSmartPointerClasses

   Semicolon-separated list of names of Qt smart pointer classes. By
   default includes ``QExplicitlySharedDataPointer``, ``QPointer``,
   ``QScopedPointer``, ``QSharedDataPointer``, ``QSharedPointer``, and
   ``QWeakPointer``.
