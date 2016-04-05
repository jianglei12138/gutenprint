# Gutenprint

High quality drivers for Canon, Epson, Lexmark, Sony, Olympus, and PCL printers for use with CUPS, Ghostscript, Foomatic, and GIMP.

Build step:

#### 1.Configure

First of all, copy project to gutenprint-android and gutenprint-pc. Configure your project with android and pc:

for android in gutenprint-android:

```shell
./configure --host=arm-linux-androideabi --disable-nls --disable-rpath --enable-cups-1_2-enhancements --disable-test --disable-testpattern --disable-libgutenprintui2 --prefix=/system/usr/root
```

for pc  in gutenprint-pc:

```shell
./configure
```

#### 2.Make

- edit libtool to remove version number after shared library name.

  Replace 

  ```shell
  library_names_spec="\${libname}\${release}\${shared_ext}\$versuffix \${libname}\${release}\${shared_ext}\$major \$libname\${shared_ext}"
  soname_spec="\${libname}\${release}\${shared_ext}\$major"
  ```

  with

  ```shell
  library_names_spec="\${libname}.so"
  soname_spec="\${libname}.so"
  ```

- make pc and android

  ```shell
  make
  ```

  and you will get error during make for android. Just copy files under gutenprint-pc/src/xml/.libs(this is a invisible folder)  to gutenprint-android/src/xml/.libs. And then make.

#### 3.Make install

make install.

 