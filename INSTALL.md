# WilliamsCRC - Installation and Use <!-- omit in toc -->

Fundamentally, **WilliamsCRC** is a classic-form C/C++ library, insofar as it has
implementation files in its **src** directory and header files in its
**include/WilliamsCRC** directory. Thus, once "installed", one must simply
include **WilliamsCRC/WilliamsCRC.h** (or one of the utility headers), and compile-in
or link-in the implementation. There are several ways to do this:

- [CMake](#cmake)
- [Bundled](#bundled)


## CMake

The primary choice for installation is by use of **CMake**.

1. Obtain the latest distribution of **WilliamsCRC**, from
   https://github.com/synesissoftware/WilliamsCRC/, e.g.

   ```bash
   $ mkdir -p ~/open-source
   $ cd ~/open-source
   $ git clone https://github.com/synesissoftware/WilliamsCRC/
   ```

2. Obtain the latest distribution of **STLSoft**, from
   https://github.com/synesissoftware/STLSoft/, e.g.

   ```bash
   $ mkdir -p ~/open-source
   $ cd ~/open-source
   $ git clone https://github.com/synesissoftware/STLSoft/
   ```

3. Prepare the CMake configuration, via the **prepare_cmake.sh** script,
   passing the **STLSoft** root directory obtained in step 2 with the
   `--stlsoft-root-dir` option, as in:

   ```bash
   # Assuming step-2 cloned into ~/open-source/STLSoft
   $ cd ~/open-source/WilliamsCRC
   $ ./prepare_cmake.sh --stlsoft-root-dir ~/open-source/STLSoft -v
   ```

   (**Hint**: execute `$ ./prepare_cmake.sh --help` for more information.)

   If you have already installed **STLSoft** on your system and have (or can
   provide) the environment variable `STLSOFT` set to the root directory
   of your **STLSoft** installation then you can omit the
   `--stlsoft-root-dir` option, as in:

   ```bash
   $ if [ -z "${STLSOFT}" ]; then echo "STLSOFT not defined\!"; else ./prepare_cmake.sh -v; fi
   ```

   or:

   ```bash
   $ STLSOFT=~/open-source/STLSoft ./prepare_cmake.sh -v
   ```

   If you have already obtained **STLSoft** and
   installed it via its **CMake** scripts then you need not specify any
   further details related to **STLSoft**, as in:

   ```bash
   $ ./prepare_cmake.sh -v
   ```

4. Run a build of the generated **CMake**-derived build files via the
   **build_cmake.sh** script, as in:

   ```bash
   $ ./build_cmake.sh
   ```

   (**NOTE**: if you provide the flag `--run-make` in step 3 then you do
   not need this step.)

5. As a check, execute the built unit-test programs via the
   **run_all_unit_tests.sh** script, as in:

   ```bash
   $ ./run_all_unit_tests.sh
   ```

   (**NOTE**: building tests requires [**xTests**](https://github.com/synesissoftware/xTests)
   to be available to **CMake**, in addition to **STLSoft**. Pass
   `--disable-testing` to **prepare_cmake.sh** if you do not need them.)

6. Install the library on the host, via `cmake`, as in:


   ```bash
   $ sudo cmake --install ./_build --config Release
   ```

7. Then to use the library, it is a simple matter as follows:

   1. Assuming a simplest possible program to verify the installation:

      ```cpp
      // main.cpp
      #include <WilliamsCRC/WilliamsCRC.h>
      #include <stdio.h>

      int main(int argc, char* argv[])
      {
         int const r = WilliamsCRC_Init();

         . . .
      }
      ```

   2. Compile your project against **WilliamsCRC**:

      Due to the installation step (Step 6 above) there is no requirement
      for an explicit include directory for **WilliamsCRC**:

      ```bash
      $ g++ -c -I ${STLSOFT}/include -std=c++11 main.cpp
      ```

      (**NOTE**: there is no need to specify the **STLSoft** include
      explicitly if **STLSoft** was installed via **CMake**.)

   3. Link your project against **WilliamsCRC**:

      Due to the installation step (Step 6 above) there is no requirement
      for an explicit library directory for **WilliamsCRC**:

      ```bash
      $ g++ main.o -lWilliamsCRC
      ```

   4. Test your project:

      Then you can run this, as in:

      ```bash
      $ ./a.out
      . . .
      ```


## Bundled

T.B.C.


<!-- ########################### end of file ########################### -->

